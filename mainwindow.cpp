#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Instanciate a scene object that we can draw to and then set up the ui
    //scene  = new QGraphicsScene(this);
    graphics_view = new ImageView;
    filter = new Filters;
    x = new QMutex;
    ui->setupUi(this);

    // Set window w/h to desktop size
    setResolution(this);

    dragging   = false;
    cropping   = false;

    // we have no threads open by
    workerOpen = false;
    settingsOpen = false;

    ui->centralWidget->layout()->addWidget( graphics_view );
    scene = graphics_view->getScene();
    setMouseTracking(true);

    // Set up link between mainwindow.cpp and imageview.cpp so imageview.cpp has access to the image data
    connect(this, SIGNAL(sendCroppingValue(bool)), graphics_view, SLOT(setCropping(bool)));
    connect(this, SIGNAL(sendImageDataForCrop(QImage)), graphics_view, SLOT(setCropImage(QImage)));
    connect(graphics_view, SIGNAL(croppedImage(QImage)), this, SLOT(renderImageToCanvas(QImage)));

    // Disable the edit buttons as nothing is availabe
    ui->cmdRGB_HSL->setEnabled(false);
    ui->cmdBlack_White->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(this->workerOpen)
    {
        delete worker;
        delete thread;
    }
}

/*
 * -----------------------------------------------------------------------------------------
 * EVENT: SET RESOLUTION
 * -----------------------------------------------------------------------------------------
 * Uses the QDesktop widget to get the current desktops width and height, the main window
 * is then set to fill the parent.
 * -----------------------------------------------------------------------------------------
 */
void MainWindow::setResolution(MainWindow *app)
{
    QRect monitor = QApplication::desktop()->screenGeometry();
    app->setGeometry(0,0, monitor.width(), monitor.height());
}

/*
 * -----------------------------------------------------------------------------------------
 * EVENT: OPEN
 * -----------------------------------------------------------------------------------------
 * Creates an Open Dialog for the user to select the file they wish to open
 * Triggered when using the Open option under "File > Open"
 * -----------------------------------------------------------------------------------------
 */
void MainWindow::on_cmdOpen_triggered()
{
    // Get the path of the selected image and call the openImage() function, the return value
    // will set the global imageSet toggle (see mainwindow.h for explanation)
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if(!fileName.isEmpty())
        imageSet = openImage(fileName);
}

/*
 * -----------------------------------------------------------------------------------------
 * EVENT: BLACK AND WHITE FILTER
 * -----------------------------------------------------------------------------------------
 * Runs the serial black and white filter implementation, declarations of the methods
 * can be found in the filters class.
 * Upon the method completing, the modified flag is updated
 * -----------------------------------------------------------------------------------------
 */
void MainWindow::on_cmdBlack_White_triggered()
{
    renderImageToCanvas(
        filter->serialFilterBlackWhite(image)
    );
    modified = true;
}

/*
 * -----------------------------------------------------------------------------------------
 * EVENT: IMAGE CHANNELS
 * -----------------------------------------------------------------------------------------
 * When the Image Channels button is selected from the Edit menu, it creates the image
 * channels window, from here the user is presented with a panel where they can modify
 * HSL and RGB values of the window
 * -----------------------------------------------------------------------------------------
 */
void MainWindow::on_cmdRGB_HSL_triggered()
{
    // We only ever want one window open
    if(!settingsOpen)
    {
        settings = new SettingsForm();
        settings->show();

        settingsOpen = true;

        // Set up the relevant connections
        connect(settings, SIGNAL(returnValues(int, int, int, FORMAT::option)),
                    this, SLOT(updateImage(int, int, int, FORMAT::option)));

        // Destructor method to reset settingsOpen
        connect(settings, SIGNAL(window_destroyed(bool)), this, SLOT(unlock_settings(bool)));
    }
}

void MainWindow::unlock_settings(bool unlock)
{
    if (unlock == true)
        settingsOpen = false;
}

// UPDATE IMAGE SLOT
void MainWindow::updateImage(int r, int g, int b, FORMAT::option channel)
{
    if(channel == FORMAT::RGB)
    {
        emit requestUpdatedImage(image, r, g, b, "RGB");
    } else if (channel == FORMAT::HSL) {
        emit requestUpdatedImage(image, r, g, b, "HSL");
    }
}

/*
 * -----------------------------------------------------------------------------------------
 * ACTION: OPEN IMAGE
 * -----------------------------------------------------------------------------------------
 * Accepts a given file name string and loads the paths contents into a QPixmap object, the
 * current scene is cleared of any old objects and then paints the new image to the scene.
 *
 * @param  fileName : The relative path to the image we wish to draw to our canvas
 * @return true if a valid image was requested and drawn to the scene, else return false
 * -----------------------------------------------------------------------------------------
 */
bool MainWindow::openImage(const QString &fileName)
{
    QImage loadedImage;

    // Has a valid image been specified, if not, return false
    if (!loadedImage.load(fileName))
        return false;

    // Check if an image already exists on the canvas, if so
    // destroy the old image, worker and thread
    if(imageSet)
    {
        imageSet = false;
        worker->deleteLater();
        thread->deleteLater();
        workerOpen = false;
    }

    // Draw the image to the screen and update the ui
    renderImageToCanvas(loadedImage);

    // Spawn thread and create slots
    createImageWorker();

    return true;
}

/*
 * -----------------------------------------------------------------------------------------
 * RENDER IMAGE TO CANVAS
 * -----------------------------------------------------------------------------------------
 * Uses the gloabl image object that is set by a variety of methods to draw the QPixmap image
 * to the QGraphicsScene canvas
 * -----------------------------------------------------------------------------------------
 */
void MainWindow::renderImageToCanvas(QImage img)
{
    // Assign the global image object
    image = img;

    // Set cursor back (this is used as a slot from cropping)
    QApplication::setOverrideCursor(Qt::ArrowCursor);

    // Destroy the old image
    scene->clear();

    // Convert the new object to a pixmap and add it to the graphics view
    QPixmap pixImg = QPixmap::fromImage(img);
    scene->addPixmap(pixImg);
    scene->setSceneRect(pixImg.rect());
    graphics_view->setScene(scene);

    // Enable manipulation controls
    ui->cmdRGB_HSL->setEnabled(true);
    ui->cmdBlack_White->setEnabled(true);
}

/*
 * -----------------------------------------------------------------------------------------
 * CREATE IMAGE WORKER
 * -----------------------------------------------------------------------------------------
 * Everytime a new image is set, this method creates a new thread and worker object for the
 * image to be edited on.  The new thread is started immediately for operations to take place.
 *
 * When a new image is loaded, this thread and worker are destroyed in the openImage() function,
 * then a new thread is spawned.
 * -----------------------------------------------------------------------------------------
 */
void MainWindow::createImageWorker()
{
    // Build a new thread and worker object
    worker = new Worker();
    thread = new QThread();

    // Connect slots and run the new thread
    connect(this, SIGNAL(requestUpdatedImage(QImage, int, int, int, QString)),
            worker, SLOT(updateImage(QImage,int, int, int, QString)));
    connect(worker, SIGNAL(imageUpdated(QImage)), this, SLOT(renderImageToCanvas(QImage)));

    worker->moveToThread(thread);
    thread->start();

    // We now have a thread running, ensure Qt knows about this so we can manually dispose
    // of it if the application closes:
    workerOpen = true;
}

/*
 * -----------------------------------------------------------------------------------------
 * EVENT: MOUSE WHEEL ZOOM
 * -----------------------------------------------------------------------------------------
 * Gets the direction that the user is scrolling (up or down) and then checks whether a
 * CONTROL + MOUSESCROLL event is being processed.  If this is happening, the zoom scene
 * function is called.
 * -----------------------------------------------------------------------------------------
 */
void MainWindow::wheelEvent(QWheelEvent *event)
{
    // Check for CTRL + MOUSEWHEEL
    if(event->modifiers() == Qt::ControlModifier)
    {
        // Determine whether the user scrolled forwards or backward
        int direction = (event->delta() / 8) / 15;
        zoomScene(direction);
    }
}

/*
 * -----------------------------------------------------------------------------------------
 * SHORTCUT: KEYBOARD SHORTCUTS
 * -----------------------------------------------------------------------------------------
 * Binds common events to some keyboard shourtcuts
 * -----------------------------------------------------------------------------------------
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Zoom in ctrl +
    if(event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_Equal))
        zoomScene(1);
    // Zoom out ctrl -
    if((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Minus))
        zoomScene(0);
    if(event->key() == Qt::Key_Space)
        graphics_view->setDragMode(graphics_view->ScrollHandDrag);
    else
        graphics_view->setDragMode(graphics_view->NoDrag);
}


/*
 * -----------------------------------------------------------------------------------------
 * EVENT: ZOOM SCENE
 * -----------------------------------------------------------------------------------------
 * increase or decreases the scale of the graphics view by 25% dependent on the direction
 * that the user wishes to go.
 *
 * uses a mutex to ensure the image does not scale too far in a particular direction when
 * a user scrolls their mouse wheel - ensures transaction is completed before running another
 * -----------------------------------------------------------------------------------------
 */
void MainWindow::zoomScene(int direction)
{
    x->lock();
    if(direction > 0)
        graphics_view->scale(1.20, 1.20);
    else
        graphics_view->scale(0.80, 0.80);
    x->unlock();
}

// Set the new cursor, look at the mouse events for the crop code
void MainWindow::on_btnCrop_clicked()
{
    // Sets the new image data on the ImageView
    emit(sendImageDataForCrop(image));
    emit(sendCroppingValue(true));
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

// Set the cursor back to default mode, this will stop cropping from working
void MainWindow::on_btnPointer_clicked()
{
    emit(sendCroppingValue(false));
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::on_btnZoomOut_clicked()
{
    zoomScene(0);
}

void MainWindow::on_btnZoomIn_clicked()
{
    zoomScene(1);
}

