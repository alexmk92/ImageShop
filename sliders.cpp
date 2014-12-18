#include "sliders.h"


Sliders::Sliders(){
    // Initalise base values to 0
    m_redValue = 0;
    m_blueValue = 0;
    m_greenValue = 0;
    m_brightnessValue = 0;
    m_contrastValue = 0;
    m_hueValue = 0;
    m_lightnessValue = 0;
    m_saturationValue = 0;

    imageSet = false;
}

/*
 * -----------------------------------------------------------------------------------------
 * MODIFY HSL (Hue, Saturation, Lightness)
 * -----------------------------------------------------------------------------------------
 * Changes the H,S,L values of each individual pixel and returns a new QImage object
 *
 * @param  int    : The current slider value
 * @param  QImage : The image that is to have its pixel data edited
 * @param  enum   : Either HUE, SATURATION or LIGHTNESS - determines which value to reset
 *
 * @return QImage : The new image to be drawn to the canvas
 * -----------------------------------------------------------------------------------------
 */
QImage Sliders::modifyHSL(int ih, int is, int il, QImage image)
{
    if(!imageSet)
        setImageData(image);

    image = workingImage;

    // Convert our Pixmap object to an image so we can manipulate the pixel data.
    QColor oldColor;
    QColor newColor;
    int h,s,l;

    // Loop through each pixel, getting the current RGB values as grayscale using qGray
    // then set the new value of the image using qRgb
    for(int i = 0; i < image.width(); i++)
    {
        for(int j = 0; j < image.height(); j++)
        {
            oldColor = image.pixel(i , j);
            newColor = oldColor.toHsl();

            h = newColor.hue();
            s = newColor.saturation();
            l = newColor.lightness();

            // Set the new RGB value
            h += getPercentageIncrease(h, ih, 200);
            s += getPercentageIncrease(s, is, 200);
            l += getPercentageIncrease(l, il, 200);

            // Check if new value between 0 and 255
            h = qBound(0, h, 255);
            s = qBound(0, s, 255);
            l = qBound(0, l, 255);

            newColor.setHsl(h, s, l);

            image.setPixel(i, j, qRgb(newColor.red(), newColor.green(), newColor.blue()));
        }
    }

    return image;
}

/*
 * -----------------------------------------------------------------------------------------
 * MODIFY RGB (RED, GREEN, BLUE)
 * -----------------------------------------------------------------------------------------
 * Changes the B,C values of each pixel in the given QImage object
 *
 * @param  int    : The slider value
 * @param  QImage : The image that is to have its pixel data edited
 * @param  enum   : Either RED, GREEN, BLUE - determines which value to reset
 *
 * @return QImage : The new image to be drawn to the canvas
 * -----------------------------------------------------------------------------------------
 */
QImage Sliders::modifyRGB(int ir, int ig, int ib, QImage image)
{
    if(!imageSet)
        setImageData(image);

    // Assign the working image to the function so we don't lose image data on each method call
    image = workingImage;

    // Convert our Pixmap object to an image so we can manipulate the pixel data.
    QColor oldColor;
    int r, g, b;

    // Loop through each pixel, getting the current RGB values
    for(int i = 0; i < image.width(); i++)
    {
        for(int j = 0; j < image.height(); j++)
        {
            oldColor = QColor(image.pixel(i, j));

            // Set old color data in mutable variables
            r = oldColor.red();
            g = oldColor.green();
            b = oldColor.blue();

            // Set the new RGB value
            r += getPercentageIncrease(r, ir, 200);
            g += getPercentageIncrease(g, ig, 200);
            b += getPercentageIncrease(b, ib, 200);

            // Check if new value between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image.setPixel(i, j, qRgb(r, g, b));
        }
    }


    return image;
}




/*
 * -----------------------------------------------------------------------------------------
 * GET PERCENTAGE INCREASE OR DECREASE
 * -----------------------------------------------------------------------------------------
 * Takes the H, S, L value of the current pixel and gets the percentage increase or decrease
 * of the pxiels value, this is then returned and appended to the respective H,S,L value.
 *
 * @param  int : The current pixels color data value we are targeting in the loop
 * @param  int : The sliders current value, which we multiply to get the percentage value
 * @param  int : The percentage modifier, some values such as RGB operate on -200% to 200%
 *
 * @return int : A positive or negative integer to be added or taken from the H, S, L value
 * -----------------------------------------------------------------------------------------
 */
int Sliders::getPercentageIncrease(double currPixel, double newValue, double modifier)
{
    // calculate return value
    double returnVal = (currPixel / modifier) * newValue;

    // Return the output value
    return (int)returnVal;
}

void Sliders::setImageData(QImage image)
{
    imageSet = true;
    workingImage = image;
}
