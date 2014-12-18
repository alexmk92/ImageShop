#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QRubberBand>
#include <QRect>
#include <QMessageBox>
#include "worker.h"

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);
    QGraphicsScene *getScene();

signals:
    void croppedImage(QImage image);

public slots:
    void setCropImage(QImage image);
    void setCropping(bool isCropping);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    QRubberBand    *rubberBand;
    QGraphicsScene *scene;
    QPoint         *origin;
    Worker         *worker;
    QImage          image;
    bool            cropping;

};

#endif // IMAGEVIEW_H
