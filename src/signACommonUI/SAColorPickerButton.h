#ifndef SACOLORPICKERBUTTON_H
#define SACOLORPICKERBUTTON_H

#include "qtcolorpicker.h"
class SAColorPickerButton : public QtColorPicker
{
public:
    SAColorPickerButton(QWidget *parent = 0,
                        int columns = -1, bool enableColorDialog = true);
};

#endif // SACOLORPICKERBUTTON_H
