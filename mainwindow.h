#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtGui>
#include <Qt>
#include <QResizeEvent>
#include <QFileDialog>
#include <QDir>
#include <QPixmap>
#include <QGraphicsScene>
#include <QThread>
#include <QShortcut>
#include <QDesktopWidget>
#include <QRubberBand>
#include "filters.h"
#include "imageview.h"
#include "worker.h"
#include "settingsform.h"

namespace Ui {
class MainWindow;
}

// Enum to control which slider toggle we are currently using

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    // Method prototypes for shared actions


    ~MainWindow();

private slots:
    // Slots for any of our image manipulation commands
    void on_cmdOpen_triggered();
    void on_cmdBlack_White_triggered();
    void on_cmdRGB_HSL_triggered();
    void on_btnCrop_clicked();
    void on_btnPointer_clicked();
    void on_btnZoomOut_clicked();
    void on_btnZoomIn_clicked();

public slots:
    void renderImageToCanvas(QImage image);
    void updateImage(int r, int g, int b, FORMAT::option);
    void unlock_settings(bool unlock);

signals:
    void requestUpdatedImage(QImage image, int r, int g, int b, QString type);
    void sendImageDataForCrop(QImage);
    void sendCroppingValue(bool);

private:
    Ui::MainWindow   *ui;
    SettingsForm     *settings;
    Filters          *filter;
    QThread          *thread;
    Worker           *worker;
    ImageView        *graphics_view;

    // Global references to the current working image and scene
    QGraphicsScene *scene;
    QMutex         *x;
    QImage          image;

    // Controls the modified property of our image, resets whenever a Save is made
    bool modified;
    bool dragging;
    bool cropping;
    bool workerOpen;
    bool settingsOpen;

    // Checks whether an image has been drawn to the canvas, needs to toggle so we can
    // clear the QGraphicsView when a new object is loaded as we only work with one image at a time (no layer system in this version)
    bool imageSet;

    // Prototypes for any of our methods
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *format);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void resizeImage(QImage *image, const QSize &newSize);
    void createImageWorker();
    void zoomScene(int direction);
    void setResolution(MainWindow *window);
    void cleanup(void);

};

#endif // MAINWINDOW_H
