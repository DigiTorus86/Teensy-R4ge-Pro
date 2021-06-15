#include "TFT_checkbox.h"

TFT_checkbox::TFT_checkbox(int16_t x, int16_t y, const char* label):TFT_control(x, y, label)
{
    _x = x;
    _y = y;
    _label = label;
    _selected = false;
    _changed = true; // To ensure first draw

    _ht = TFT_CHKBOX_DEFAULT_HT;
    _wd = TFT_CHKBOX_DEFAULT_WD;
    _min = TFT_CHKBOX_UNCHECKED;
    _max = TFT_CHKBOX_CHECKED;
    _value = TFT_CHKBOX_DEFAULT_VALUE;
    _bgd_color = TFT_CHKBOX_DEFAULT_BGD_COLOR;
    _btn_color = TFT_CHKBOX_DEFAULT_BTN_COLOR;
}

void TFT_checkbox::draw(ILI9341_t3 *tft, bool force_redraw)
{
    if (!_changed && !force_redraw) return;

    tft->fillRect(_x, _y, _wd, _ht, _bgd_color); // background


    // Draw box
    _scale_value = _value * _scale_factor;
    if (_selected)
    {
        tft->fillRoundRect(_x, _y, TFT_CHKBOX_BOX_WD, _ht, 4, TFT_CHKBOX_SELECTED_BTN_COLOR);
    } 
    else
    {
        tft->fillRoundRect(_x, _y, TFT_CHKBOX_BOX_WD, _ht, 4, _btn_color);
    }

    tft->drawRoundRect(_x, _y, TFT_CHKBOX_BOX_WD, _ht, 4, TFT_CHKBOX_OUTLINE_BTN_COLOR);
    tft->fillCircle(_x + TFT_CHKBOX_BOX_WD / 2, _y + TFT_CHKBOX_BOX_HT / 2, 3, TFT_CHKBOX_OUTLINE_BTN_COLOR);

    if (_value == TFT_CHKBOX_CHECKED)
    {
        // Draw checked indicator - (up,left) to (down,right)
        tft->drawLine(_x + 1, _y + 1, _x + TFT_CHKBOX_BOX_WD - 2, _y + TFT_CHKBOX_BOX_HT - 2, ILI9341_WHITE);
        tft->drawLine(_x + 2, _y + 1, _x + TFT_CHKBOX_BOX_WD - 2, _y + TFT_CHKBOX_BOX_HT - 3, ILI9341_WHITE);
        tft->drawLine(_x + 1, _y + 2, _x + TFT_CHKBOX_BOX_WD - 3, _y + TFT_CHKBOX_BOX_HT - 2, ILI9341_WHITE);

        // (down,left) to (up,right)
        tft->drawLine(_x + 1, _y + _ht - 2, _x + TFT_CHKBOX_BOX_WD - 2, _y + 1, ILI9341_WHITE);
        tft->drawLine(_x + 2, _y + _ht - 2, _x + TFT_CHKBOX_BOX_WD - 2, _y + 2, ILI9341_WHITE);
        tft->drawLine(_x + 1, _y + _ht - 3, _x + TFT_CHKBOX_BOX_WD - 3, _y + 1, ILI9341_WHITE);
    }

    // Display the label
    tft->setTextSize(1);
    tft->setTextColor(ILI9341_WHITE);
    tft->setCursor(_x + TFT_CHKBOX_BOX_WD + 4, _y + 2);
    tft->print(_label);

    _changed = false;  // internal state and display are now in sync
}

bool TFT_checkbox::handleButtons(bool up_pressed, bool down_pressed)
{
    if (!_selected) return false;

    if (up_pressed)
    {
        setValue(TFT_CHKBOX_CHECKED); 
    }

    if (down_pressed)
    {
        setValue(TFT_CHKBOX_UNCHECKED); 
    }

    return true;
}

bool TFT_checkbox::handleTouch(int16_t x, int16_t y)
{
    if (x >= _x && x < _x + _wd && y >= _y && y < _y + _ht)
    {
        setValue(1 - _value); // toggle between checked/unchecked
        return true;
    }

    return false;
}

