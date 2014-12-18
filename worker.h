#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "sliders.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();

signals:
    void imageUpdated(QImage newImage);

public slots:
    void updateImage(QImage image, int r, int g, int b, QString setting);

private:
    QImage  *image;
    Sliders *slider;
    SETTING::option get_setting(QString setting);

};

#endif // WORKER_H
