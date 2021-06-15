#include "TFT_keyboard.h"

TFT_keyboard::TFT_keyboard(int16_t x, int16_t y, const char* label):TFT_control(x, y, label)
{
    _x = x;
    _y = y;
    _label = label;
    _selected = false;
    _changed = true; // To ensure first draw
    _octave = TFT_KBRD_MIN_OCT;

    _ht = TFT_KBRD_DEFAULT_HT;
    _wd = TFT_KBRD_DEFAULT_WD;
    _min = 0;
    _max = TFT_KBRD_NUM_KEYS - 1;
    _value = TFT_KBRD_DEFAULT_VALUE;
    _bgd_color = TFT_KBRD_DEFAULT_BGD_COLOR;
    _btn_color = TFT_KBRD_DEFAULT_BTN_COLOR;
    _half_ht = _ht / 2;
}

void TFT_keyboard::draw(ILI9341_t3 *tft, bool force_redraw)
{
    if (!_changed && !force_redraw) return;

    //tft->fillRect(_x, _y, _wd, _ht, _bgd_color); // background
    int16_t  x1, y1; 
    uint16_t color = TFT_KBRD_DEFAULT_BTN_COLOR;
    if (_selected)
    {
        color = TFT_KBRD_SELECTED_BTN_COLOR;
    }

    tft->fillTriangle(_x, _y + _half_ht, _x + 12, _y, _x + 12, _y + _ht - 1, color); // down octave button
    tft->fillTriangle(_x + _wd - 1, _y + _half_ht, _x + _wd - 12, _y, _x + _wd - 12, _y + _ht - 1, color); // up octave button

    // Draw the white keys
    bool is_black;
    for (uint8_t i = 0; i < TFT_KBRD_NUM_KEYS; i++)
    {
        is_black = (bool)(_key_pos[i][1] == 1);

        if (!is_black)
        {
            x1 = _x + TFT_KBRD_KEYPOS_OFFSET_X + (_key_pos[i][0] * TFT_KBRD_KEYPOS_SCALE_X);
            tft->fillRect(x1, _y, (TFT_KBRD_KEYPOS_SCALE_X * 3) - 1, _ht, ILI9341_WHITE);
            tft->drawRect(x1, _y, (TFT_KBRD_KEYPOS_SCALE_X * 3) - 1, _ht + 1, ILI9341_BLACK);
        }
        
    }
    // Draw the black keys
    for (uint8_t i = 0; i < TFT_KBRD_NUM_KEYS; i++)
    {
        is_black = (bool)(_key_pos[i][1] == 1);

        if (is_black)
        {
            x1 = _x + TFT_KBRD_KEYPOS_OFFSET_X + (_key_pos[i][0] * TFT_KBRD_KEYPOS_SCALE_X) - 1;
            tft->fillRect(x1, _y, 2 + TFT_KBRD_KEYPOS_SCALE_X * 2, _half_ht, ILI9341_BLACK);
        }
    }

    // Display the current value (selected key) 
    x1 = _x + TFT_KBRD_KEYPOS_OFFSET_X - 2 + (_key_pos[_value][0] * TFT_KBRD_KEYPOS_SCALE_X) + (TFT_KBRD_KEY_WD / 2); 
    y1 = _y + (_key_pos[_value][1] * _half_ht) - (_half_ht / 2);
    tft->fillCircle(x1, y1, 4, ILI9341_RED);
    
    // Display the octave values on the C keys
    tft->setTextSize(1);
    tft->setTextColor(ILI9341_BLACK);
    for (uint8_t i = 0; i < 3; i++)
    {
        x1 = _x + TFT_KBRD_KEYPOS_OFFSET_X + 4 + (_key_pos[i * 12][0] * TFT_KBRD_KEYPOS_SCALE_X);
        tft->setCursor(x1, _y + _ht - 11);
        tft->print(_octave + i);
    }

    _changed = false;  // internal state and display are now in sync
}

bool TFT_keyboard::handleButtons(bool up_pressed, bool down_pressed, bool left_pressed, bool right_pressed)
{
    if (!_selected) return false;

    if (up_pressed && (_value < _max))
    {
        setValue(_value + 1);
    }

    if (down_pressed && (_value > _min))
    {
       setValue(_value - 1);
    }

    if (left_pressed)
    {
        setOctave(_octave - 1);
    }

    if (right_pressed)
    {
        setOctave(_octave + 1);
    }

    return true;
}

bool TFT_keyboard::handleTouch(int16_t x, int16_t y)
{
    if (!(x >= _x && x < _x + _wd && y >= _y && y < _y + _ht))
    {
        _selected = false;
        return false; // touch is not in bounds
    }

    _selected = true;

    if (millis() - _touched_ms < TFT_KBRD_TOUCH_REPEAT_DELAY_MS)
    {
        return true; // Control touched, but not ready to act
    }

    // Check left button
    if (x < TFT_KBRD_BTN_WD)
    {
        setOctave(_octave - 1);
        _touched_ms = millis();
        return true;
    }

    // Check right button
    if (x > _x + _wd - TFT_KBRD_BTN_WD)
    {
        setOctave(_octave + 1);
        _touched_ms = millis();
        return true;
    }

    // Check individual keys
    for (uint8_t i = 0; i < TFT_KBRD_NUM_KEYS; i++)
    {
        if (keyHit(x, y, i))
        {
            setValue(i);
            _touched_ms = millis();
            return true;
        }
    }

    return true; // touch is in bounds
}

bool TFT_keyboard::keyHit(int16_t x, int16_t y, uint8_t key_idx)
{
    int16_t key_x, key_y, key_wd;
    bool is_black = false;

    // Determine the lower-left x, y coordinate for this key
    key_x = TFT_KBRD_KEYPOS_OFFSET_X + (_key_pos[key_idx][0] * TFT_KBRD_KEYPOS_SCALE_X);
    key_y = _y + (_key_pos[key_idx][1] * _half_ht); 
    // White keys are 3 scale units wide, Black keys are 2 scale units wide
    key_wd = (1 + _key_pos[key_idx][1]) * TFT_KBRD_KEYPOS_SCALE_X;
    is_black = (bool)(_key_pos[key_idx][1] == 1);

    if (x >= key_x && x < key_x + key_wd && y <= key_y)
    {
        // We're inside the general key outline
        if (is_black) return true;  // Black keys are simple rectangles

        // Additional check for white keys to account for black key cut-out area
        if (x >= key_x + TFT_KBRD_KEYPOS_SCALE_X &&
            key_x <  key_x + key_wd - TFT_KBRD_KEYPOS_SCALE_X)
        {
            return true; 
        }
    }

    return false;
}

void TFT_keyboard::setRange(int16_t min_value, int16_t max_value)
{
    _min = min_value;
    _max = max_value;
}

int16_t TFT_keyboard::getOctave()
{
    return _octave;
}

void TFT_keyboard::setOctave(int16_t octave)
{
    if (octave != _octave) _changed = true;

    _octave = octave;

    if (_octave < TFT_KBRD_MIN_OCT) _octave = TFT_KBRD_MIN_OCT;
    if (_octave > TFT_KBRD_MAX_OCT) _octave = TFT_KBRD_MAX_OCT;
}
