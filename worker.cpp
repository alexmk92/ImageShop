#include "worker.h"

Worker::Worker()
{
    slider = new Sliders;
}

Worker::~Worker()
{

}

/*
 * -----------------------------------------------------------------------------------------
 * SLOT : UPDATE IMAGE
 * -----------------------------------------------------------------------------------------
 * Passes a QImage to the specified modify function
 *
 * @param  QImage  : the QImage object we wish to modify
 * @param  int     : the value we currently have set on the slider
 * @param  QString : the setting value we want to return as an enum
 * -----------------------------------------------------------------------------------------
 */
void Worker::updateImage(QImage image, int r_h, int g_s, int b_l, QString setting)
{
    // Get the enum representation of QString
    SETTING::option givenSetting = get_setting(setting);

    // Check for H,S,L rendering
    if(givenSetting == SETTING::HSL)
        image = slider->modifyHSL(r_h, g_s, b_l, image);
    // Check for R,G,B rendering
    if(givenSetting == SETTING::RGB)
        image = slider->modifyRGB(r_h, g_s, b_l, image);

    // After process, emit the signal
    emit imageUpdated(image);
}


/*
 * -----------------------------------------------------------------------------------------
 * GET SETTING
 * -----------------------------------------------------------------------------------------
 * Takes a QString which relates to a given enum and returns that enumerable to be used in
 * the modify functions
 *
 * @param  QString : the setting value we want to return as an enum
 *
 * @return SETTING : returns the setting from the given namespace, else return unset
 * -----------------------------------------------------------------------------------------
 */
SETTING::option Worker::get_setting(QString setting)
{
    if (setting == "RGB")
        return SETTING::RGB;
    if (setting == "HSL")
        return SETTING::HSL;

    // Default return value
    return SETTING::UNSET;
}
