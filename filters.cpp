#include "filters.h"

Filters::Filters()
{

}

/*
 * SERIAL IMPLEMENTATIONS
 */

/*
 * ---------------------------------------------------------------------------------
 * BLACK AND WHITE - SERIAL
 * ---------------------------------------------------------------------------------
 * Directly modifies the given QPixmap object, desaturating each pixel of the object
 * to produce a black and white representation (serial implementation)
 *
 * @param QPixmap image : the QPixmap object we wish to convert
 * @return QPixmap : the edited QPixmap object
 * ---------------------------------------------------------------------------------
 */
QImage Filters::serialFilterBlackWhite(QImage image)
{
    // Convert our Pixmap object to an image so we can manipulate the pixel data.
    QRgb   currentColor;
    int    gray;

    // Loop through each pixel, getting the current RGB values as grayscale using qGray
    // then set the new value of the image using qRgb
    for(int i = 0; i < image.width(); i++)
    {
        for(int j = 0; j < image.height(); j++)
        {
            currentColor = image.pixel(i , j);
            gray = qGray(currentColor);
            image.setPixel(i, j, qRgb(gray, gray, gray));
        }
    }

    // Convert the QImage object back to a QPixmap and return it
    return image;
}

/*
 * ---------------------------------------------------------------------------------
 * BLACK AND WHITE - SERIAL
 * ---------------------------------------------------------------------------------
 * Directly modifies the given QPixmap object, desaturating each pixel of the object
 * to produce a black and white representation (serial implementation)
 *
 * @param QPixmap image : the QPixmap object we wish to convert
 * @return QPixmap : the edited QPixmap object
 * ---------------------------------------------------------------------------------
 */
