#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

namespace Ui {
class SettingsForm;
}

// Enum to control which slider toggle we are currently using
namespace FORMAT { enum option { RGB, HSL, BC, UNSET }; }

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();

signals:
    // Send back the current slider values - either RGB or HSL
    void returnValues(int, int, int, FORMAT::option);
    void window_destroyed(bool);

private:
    Ui::SettingsForm   *ui;

    // Methods
    QList<int> getSliderValues(FORMAT::option);
    QList<int> values;

private slots:
    void on_slideSaturation_sliderReleased();
    void on_slideHue_sliderReleased();
    void on_sliderLightness_sliderReleased();
    void on_slideR_sliderReleased();
    void on_slideG_sliderReleased();
    void on_slideB_sliderReleased();
    void on_btnDone_pressed();
};

#endif // SETTINGSFORM_H
