#include "TFT_slider.h"

TFT_slider::TFT_slider(int16_t x, int16_t y, const char* label):TFT_control(x, y, label)
{
    _x = x;
    _y = y;
    _label = label;
    _selected = false;
    _changed = true; // To ensure first draw

    _ht = TFT_SLDR_DEFAULT_HT;
    _wd = TFT_SLDR_DEFAULT_WD;
    _min = TFT_SLDR_DEFAULT_MIN_VAL;
    _max = TFT_SLDR_DEFAULT_MAX_VAL;
    _value = TFT_SLDR_DEFAULT_VALUE;
    _bgd_color = TFT_SLDR_DEFAULT_BGD_COLOR;
    _btn_color = TFT_SLDR_DEFAULT_BTN_COLOR;

    _ctr_x = _x + (_wd / 2);
    _scale_factor = (float)(_ht - TFT_SLDR_LBL_HT - TFT_SLDR_BTN_HT) / (float)(_max - _min);
    _scale_value = _value * _scale_factor;
    _min_y = _y + _ht - TFT_SLDR_LBL_HT - TFT_SLDR_BTN_HT;
    _max_y = _y;  // may add margin at some point
}

void TFT_slider::draw(ILI9341_t3 *tft, bool force_redraw)
{
    if (!_changed && !force_redraw) return;

    tft->fillRect(_x, _y, _wd, _ht, _bgd_color); // background
    tft->drawLine(_ctr_x, _y, _ctr_x, _y + _ht, ILI9341_BLACK);  // center line

    // Draw tick marks
    for (int i = 0; i < _ht - TFT_SLDR_LBL_HT; i += 5)
    {
        tft->drawLine(_ctr_x - 4, _y + i, _ctr_x - 2, _y + i, ILI9341_WHITE); // left tick
        tft->drawLine(_ctr_x + 2, _y + i, _ctr_x + 4, _y + i, ILI9341_WHITE); // right tick
    }

    // Draw slider button
    _scale_value = _value * _scale_factor;
    if (_selected)
    {
        tft->fillRoundRect(_x, _min_y - _scale_value, _wd, TFT_SLDR_BTN_HT, 4, TFT_SLDR_SELECTED_BTN_COLOR);
    } 
    else
    {
        tft->fillRoundRect(_x, _min_y - _scale_value, _wd, TFT_SLDR_BTN_HT, 4, _btn_color);
    }
    
    tft->drawRoundRect(_x, _min_y - _scale_value, _wd, TFT_SLDR_BTN_HT, 4, TFT_SLDR_OUTLINE_BTN_COLOR);

    // Display either the label or the current value depending on the selected status
    int16_t  x1, y1; 
    uint16_t text_wd, text_ht;
 
    tft->setTextSize(1);
    //tft->getTextBounds(_label, _x, _y + _ht - TFT_SLDR_LBL_HT, &x1, &y1, &text_wd, &text_ht);
    
    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft->setCursor(_ctr_x - (text_wd / 2), _y + _ht - TFT_SLDR_LBL_HT);
    if (_selected)
    {
        tft->print(_value); 
    }
    else
    {
        tft->print(_label);
    }
    _changed = false;  // internal state and display are now in sync
}

bool TFT_slider::handleButtons(bool up_pressed, bool down_pressed)
{
    if (!_selected) return 0;

    if (up_pressed)
    {
        setValue(_value + 1);
    }

    if (down_pressed)
    {
        setValue(_value - 1);
    }
    return true;
}

bool TFT_slider::handleTouch(int16_t x, int16_t y)
{
    if (x >= _x && x < _x + _wd && y >= _y && y < _y + _ht)
    {
        setValue((float)(_y + _ht - 1 - y) / _scale_factor);
        return true;
    }

    return false;
}

void TFT_slider::setRange(int16_t min_value, int16_t max_value)
{
    _min = min_value;
    _max = max_value;
}
