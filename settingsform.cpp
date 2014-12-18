#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

}

SettingsForm::~SettingsForm()
{
    delete ui;
    emit window_destroyed(true);
}

void SettingsForm::on_btnDone_pressed()
{
    emit window_destroyed(true);
    delete this;
}

void SettingsForm::on_slideR_sliderReleased()
{
    values = getSliderValues(FORMAT::RGB);

    int r = values[0];
    int g = values[1];
    int b = values[2];

    emit returnValues(r, g, b, FORMAT::RGB);
}

void SettingsForm::on_slideG_sliderReleased()
{
    values = getSliderValues(FORMAT::RGB);

    int r = values[0];
    int g = values[1];
    int b = values[2];

    emit returnValues(r, g, b, FORMAT::RGB);
}

void SettingsForm::on_slideB_sliderReleased()
{
    values = getSliderValues(FORMAT::RGB);

    int r = values[0];
    int g = values[1];
    int b = values[2];

    emit returnValues(r, g, b, FORMAT::RGB);
}

void SettingsForm::on_slideHue_sliderReleased()
{
    values = getSliderValues(FORMAT::HSL);

    int h = values[0];
    int s = values[1];
    int l = values[2];

    emit returnValues(h, s, l, FORMAT::HSL);
}

void SettingsForm::on_slideSaturation_sliderReleased()
{
    values = getSliderValues(FORMAT::HSL);

    int h = values[0];
    int s = values[1];
    int l = values[2];

    emit returnValues(h, s, l, FORMAT::HSL);
}

void SettingsForm::on_sliderLightness_sliderReleased()
{
    values = getSliderValues(FORMAT::HSL);

    int h = values[0];
    int s = values[1];
    int l = values[2];

    emit returnValues(h, s, l, FORMAT::HSL);
}

// Return integers in RGB HSL format
QList<int> SettingsForm::getSliderValues(FORMAT::option f)
{
    QList<int> res;
    switch(f)
    {
        case FORMAT::RGB:
             res.insert(0, ui->slideR->value());
             res.insert(1, ui->slideG->value());
             res.insert(2, ui->slideB->value());
            return res;
        break;
        case FORMAT::HSL:
            res.insert(0, ui->slideHue->value());
            res.insert(1, ui->slideSaturation->value());
            res.insert(2, ui->sliderLightness->value());
           return res;
        break;
        default:
            res.insert(0, ui->slideR->value());
            res.insert(1, ui->slideG->value());
            res.insert(2, ui->slideB->value());
           return res;
        break;
    }
}
