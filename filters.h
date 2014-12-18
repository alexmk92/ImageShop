#ifndef FILTERS_H
#define FILTERS_H

#include <QObject>
#include <QPixmap>

// Filter class
class Filters
{
public:
    Filters();

    QImage serialFilterBlackWhite(QImage);
};

#endif // FILTERS_H
