#ifndef SLIDERS_H
#define SLIDERS_H

#include <QObject>
#include <QPixmap>

// Enum to control which slider toggle we are currently using
namespace SETTING { enum option { HSL, RGB, BC, UNSET }; }

class Sliders
{
public:
    Sliders();

    // Public methods
    QImage modifyHSL(int h, int s, int l, QImage);
    QImage modifyRGB(int r, int g, int b, QImage);

private:
    // Private methods
    int  getPercentageIncrease(double pixel, double value, double modifier);
    int  getMemberValue(SETTING::option);
    void setMemberValue(SETTING::option, int value);
    void setImageData(QImage image);

    // Sets the data for the working image
    bool   imageSet;
    QImage workingImage;

    // Member variables
    int m_redValue;
    int m_greenValue;
    int m_blueValue;
    int m_brightnessValue;
    int m_contrastValue;
    int m_hueValue;
    int m_saturationValue;
    int m_lightnessValue;

};

#endif // SLIDERS_H
