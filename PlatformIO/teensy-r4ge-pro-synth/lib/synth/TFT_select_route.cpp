#include "TFT_select_route.h"

TFT_select_route::TFT_select_route(int16_t x, int16_t y, const char* label):TFT_control(x, y, label)
{
    _x = x;
    _y = y;
    _label = label;

    _ht = TFT_SEL_ROUTE_DEFAULT_HT;
    _wd = TFT_SEL_ROUTE_DEFAULT_WD;

    _bgd_color = TFT_SEL_ROUTE_DEFAULT_BGD_COLOR;
    _btn_color = TFT_SEL_ROUTE_DEFAULT_BTN_COLOR;

    _ctr_x = _x + (_wd / 2);
    _ctr_y = _y + (_ht / 2);

    _line1_y = _ctr_y - 20;
    _line2_y = _line1_y + 10;
    _line3_y = _line2_y + 10;

    _min = 0;
    _max = SYN_ROUTE_TYPE_COUNT - 1; 
    _item_count = SYN_ROUTE_TYPE_COUNT;
    _value = 0;
    _selected = false;
    _changed = true; // To ensure first draw
}

void TFT_select_route::draw(ILI9341_t3 *tft, bool force_redraw)
{
    if (!_changed && !force_redraw) return;

    tft->fillRect(_x, _y, _wd, _ht, _bgd_color); // background
    tft->fillRoundRect(_x + 2, _ctr_y - 26, _wd - 4, _wd + 12, 4, ILI9341_BLACK); // selection bgd

    if (_selected)
    {
        tft->drawRoundRect(_x + 2, _ctr_y - 26, _wd - 4, _wd + 12, 4, TFT_SEL_ROUTE_SELECTED_BTN_COLOR);    // selection box outline
        tft->fillTriangle(_ctr_x, _y, _x + 2, _y + 10, _x + _wd - 2, _y + 10, TFT_SEL_ROUTE_SELECTED_BTN_COLOR);  // top button
        tft->fillTriangle(_ctr_x, _y + _ht - 10, _x + 2, _y + _ht - 18, _x + _wd - 2, _y + _ht - 18, TFT_SEL_ROUTE_SELECTED_BTN_COLOR);  // bottom button
    }
    else
    {
        tft->drawRoundRect(_x + 2, _ctr_y - 26, _wd - 4, _wd + 12, 4, _btn_color);    // selection box outline
        tft->fillTriangle(_ctr_x, _y, _x + 2, _y + 10, _x + _wd - 2, _y + 10, _btn_color);  // top button
        tft->fillTriangle(_ctr_x, _y + _ht - 10, _x + 2, _y + _ht - 18, _x + _wd - 2, _y + _ht - 18, _btn_color);  // bottom button
    }
    
    tft->drawTriangle(_ctr_x, _y, _x + 2, _y + 10, _x + _wd - 2, _y + 10, TFT_SEL_ROUTE_OUTLINE_BTN_COLOR);  // top button outline
    tft->drawTriangle(_ctr_x, _y + _ht - 10, _x + 2, _y + _ht - 18, _x + _wd - 2, _y + _ht - 18, TFT_SEL_ROUTE_OUTLINE_BTN_COLOR);  // bottom button outline

    int16_t  x1, y1; 
    uint16_t text_wd, text_ht;

    // Display the route icon
    drawRoute(tft, (SYN_route_type)_value);

    // Display the label 
    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    //tft->getTextBounds(_label, _x, _y + _ht - TFT_SEL_ROUTE_LBL_HT, &x1, &y1, &text_wd, &text_ht);
    tft->setCursor(_ctr_x - (text_wd / 2), _y + _ht - TFT_SEL_ROUTE_LBL_HT);
    tft->print(_label);
    
    _changed = false;  // internal state and display are now in sync
}

bool TFT_select_route::handleButtons(bool up_pressed, bool down_pressed)
{
    if (up_pressed && (_value < _item_count - 1))
    {
        setValue(_value + 1);
    }

    if (down_pressed && (_value > 0))
    {
        setValue(_value - 1);
    }
    return true;  // consumed buttons
}

bool TFT_select_route::handleTouch(int16_t x, int16_t y)
{
    if (!(x >= _x && x < _x + _wd && y >= _y && y < _y + _ht))
    {
        return false; // no hit within this control
    }

    bool up_hit, dn_hit;

    // Only need to check y values, since x was already checked above
    up_hit = (bool)(y < _y + 10);
    dn_hit = (bool)(y > _y + _ht - 18);

    if (!up_hit && !dn_hit) return true; // control touched, but no button hit

    if (millis() - _touched_ms < TFT_SEL_ROUTE_TOUCH_REPEAT_DELAY_MS)
    {
        return true; // Control touched, but not ready to act
    }
    
    if (up_hit)
    {
        setValue(_value + 1);
        _touched_ms = millis();
    }

    if (dn_hit)
    {
        setValue(_value - 1);
        _touched_ms = millis();
    }
    
    return true;
}


void TFT_select_route::drawRoute(ILI9341_t3 *tft, SYN_route_type route)
{
    tft->setTextSize(1);
    tft->setTextColor(ILI9341_GREEN, ILI9341_BLACK);

    switch(route)
    {
        case SYN_ROUTE_1234:
            tft->setCursor(_x + 7, _line1_y - 1);
            tft->print(" 1");
            tft->setCursor(_x + 7, _line2_y - 2);
            tft->print(" 2");
            tft->setCursor(_x + 7, _line3_y - 3);
            tft->print(" 3");
            tft->setCursor(_x + 7, _line3_y + 6);
            tft->print(" 4");
            break;
        
        case SYN_ROUTE_12_34:
            tft->setCursor(_x + 6, _line1_y);
            tft->print("1 3");
            tft->setCursor(_x + 6, _line2_y);
            tft->print("| |");
            tft->setCursor(_x + 6, _line3_y);
            tft->print("2 4");
            break;

        case SYN_ROUTE_123_4:
            tft->setCursor(_x + 6, _line1_y);
            tft->print("1 |");
            tft->setCursor(_x + 6, _line2_y);
            tft->print("2 |");
            tft->setCursor(_x + 6, _line3_y);
            tft->print("3 4");
            break;

        case SYN_ROUTE_1_2_3_4:
            tft->setCursor(_x + 6, _line1_y);
            tft->print("1 2");
            tft->setCursor(_x + 6, _line3_y);
            tft->print("3 4");
            break;
        
        default:
            break;
    }

}
