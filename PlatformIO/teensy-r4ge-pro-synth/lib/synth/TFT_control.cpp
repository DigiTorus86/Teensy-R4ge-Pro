#include "TFT_control.h"

TFT_control::TFT_control(int16_t x, int16_t y, const char* label)
{
    _x = x;
    _y = y;
    _label = label;
    _selected = false;
    _changed = true; // To ensure first draw
}

void TFT_control::draw(ILI9341_t3 *tft, bool force_redraw)
{

}

bool TFT_control::handleTouch(int16_t x, int16_t y)
{
    return false;
}

bool TFT_control::handleButtons(bool up_pressed, bool down_pressed)
{
    return false;
}

bool TFT_control::getChanged()
{
    return _changed;
}

bool TFT_control::getSelected()
{
    return _selected;
}

void TFT_control::setSelected(bool selected)
{
    // _changed = true;
    if (_selected != selected) _changed = true;

    _selected = selected;
}

int16_t TFT_control::getValue()
{
    return _value;
}

void TFT_control::setValue(int16_t value)
{
    if (_value != value) _changed = true;

    _value = value;

    if (_value < _min) _value = _min;
    if (_value > _max) _value = _max;
}
