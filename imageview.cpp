#include "imageview.h"

ImageView::ImageView(QWidget *parent) :
    QGraphicsView(parent)
{
    // Initialize our scene items
    scene      = new QGraphicsScene();
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    origin     = new QPoint;

    // Default initializer
    cropping   = false;

    // Create the messagee dialog

}

void ImageView::mousePressEvent(QMouseEvent *e)
{
    if(cropping)
    {
        rubberBand->close();
        *origin = e->pos();

        if(!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(*origin, QSize()));
        rubberBand->show();
    }

}
void ImageView::mouseMoveEvent(QMouseEvent *e)
{
    if(cropping)
        rubberBand->setGeometry(QRect(*origin, e->pos()).normalized());
}
void ImageView::mouseReleaseEvent(QMouseEvent *e)
{
    // If the user selected yes, then crop the image
    if(cropping)
    {
        // Prompt the user of the permanent change they're about to make
        QMessageBox confirm;

        confirm.setInformativeText("You are about to crop this image, are you sure you wish to continue?");
        confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        confirm.setDefaultButton(QMessageBox::Yes);

        // Capture the users request
        int res = confirm.exec();

        if(res == QMessageBox::Yes)
        {
            rubberBand->close();

            QImage copy = image;
            QImage crop;

            int x = rubberBand->x();
            int y = rubberBand->y();
            int w = rubberBand->width();
            int h = rubberBand->height();

            crop = copy.copy(x, y, w, h);
            cropping = false;
            emit croppedImage(crop);
        }
    }
}

void ImageView::setCropImage(QImage img)
{
    image = img;
}

void ImageView::setCropping(bool isCropping)
{
    cropping = isCropping;
}

QGraphicsScene *ImageView::getScene()
{
    return this->scene;
}
