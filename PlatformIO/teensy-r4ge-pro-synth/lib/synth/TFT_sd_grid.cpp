#include "TFT_sd_grid.h"

TFT_sd_grid::TFT_sd_grid(int16_t x, int16_t y, const char* label):TFT_control(x, y, label)
{
    _x = x;
    _y = y;
    _label = label;
    _selected = false;
    _changed = true; // To ensure first draw

    _ht = TFT_SDGRID_DEFAULT_HT;
    _wd = TFT_SDGRID_DEFAULT_WD;
    _min = 0;
    _max = TFT_SDGRID_CELL_CNT;
    _value = TFT_SDGRID_DEFAULT_VALUE;
    _bgd_color = TFT_SDGRID_DEFAULT_BGD_COLOR;
    _btn_color = TFT_SDGRID_DEFAULT_BTN_COLOR;

    _load_btn_x = _x + 40;
    _load_btn_y = _y + 16 +  (TFT_SDGRID_CELL_HT * 3);
    _save_btn_x = _x + _wd - TFT_SDGRID_BTN_WD - 40;
    _save_btn_y = _load_btn_y;

    reset();
}

void TFT_sd_grid::draw(ILI9341_t3 *tft, bool force_redraw)
{
    if (!_changed && !force_redraw) return;

    if (force_redraw) _selection_done = false;

    if (!_sd_present)
    {
        tft->fillRect(_x, _y, _wd, _ht, _bgd_color);
        tft->setTextSize(2);
        tft->setTextColor(TFT_SDGRID_TEXT_BTN_COLOR);
        tft->setCursor(_x + 40, _y + 40);
        tft->print("No SD card detected.");
        _value = 0;
        _changed = false;
        return;
    }

    //tft->fillRect(_x, _y, _wd, _ht, _bgd_color); // background - TOO FLICKERY!
    int16_t  y1; 
    uint16_t out_color;
    
    // Draw the SD slot cells
    bool selected = false;
    for (uint8_t i = 1; i <= TFT_SDGRID_CELL_CNT; i++)
    {
        selected = (bool)(_value == i);
        drawCell(tft, i, selected);
    }

    // Erase bottom portion of screen below cells
    y1 = _y + 12 + (TFT_SDGRID_CELL_HT * 3);
    tft->fillRect(_x, y1, _wd, _y + _ht - y1, _bgd_color); // background

    // Draw the LOAD button
    out_color = TFT_SDGRID_OUTLINE_BTN_COLOR;
    if (_btn_idx == TFT_SD_GRID_BTN_LOAD)
    {
        out_color = TFT_SDGRID_TEXT_BTN_COLOR;
    }
    //x1 = _x + 40;
    //y1 = _y + 16 +  (TFT_SDGRID_CELL_HT * 3);
    tft->fillRoundRect(_load_btn_x, _load_btn_y, TFT_SDGRID_BTN_WD, TFT_SDGRID_BTN_HT, 4, TFT_SDGRID_DEFAULT_BTN_COLOR);
    tft->drawRoundRect(_load_btn_x, _load_btn_y, TFT_SDGRID_BTN_WD, TFT_SDGRID_BTN_HT, 4, out_color);
    tft->setTextSize(2);
    tft->setTextColor(TFT_SDGRID_TEXT_BTN_COLOR);
    tft->setCursor(_load_btn_x + 15, _load_btn_y + 5);
    tft->print("LOAD");

    // Draw the SAVE button
    out_color = TFT_SDGRID_OUTLINE_BTN_COLOR;
    if (_btn_idx == TFT_SD_GRID_BTN_SAVE)
    {
        out_color = TFT_SDGRID_TEXT_BTN_COLOR;
    }
    //x1 = _x + _wd - TFT_SDGRID_BTN_WD - 40;
    //y1 = _y + 16 + (TFT_SDGRID_CELL_HT * 3);
    tft->fillRoundRect(_save_btn_x, _save_btn_y, TFT_SDGRID_BTN_WD, TFT_SDGRID_BTN_HT, 4, TFT_SDGRID_DEFAULT_BTN_COLOR);
    tft->drawRoundRect(_save_btn_x, _save_btn_y, TFT_SDGRID_BTN_WD, TFT_SDGRID_BTN_HT, 4, out_color);
    tft->setTextSize(2);
    tft->setTextColor(TFT_SDGRID_TEXT_BTN_COLOR);
    tft->setCursor(_save_btn_x + 15, _save_btn_y + 5);
    tft->print("SAVE");

    _changed = false;  // internal state and display are now in sync
}

bool TFT_sd_grid::handleButtons(bool up_pressed, bool down_pressed, bool left_pressed, bool right_pressed)
{
    //if (!_selected) return false;

    if (down_pressed && (_value < _max))
    {
        setValue(_value + 1);
    }

    if (up_pressed && (_value > _min))
    {
       setValue(_value - 1);
    }

    if (left_pressed)
    {
        setButton(TFT_SD_GRID_BTN_LOAD);  // LOAD btn
        if (millis() - _btn_start_time > TFT_SDGRID_BTN_PRESS_DELAY_MS)
        {
            _selection_done = true;
        }
        return true;
    }

    if (right_pressed)
    {
        setButton(TFT_SD_GRID_BTN_SAVE);  // SAVE btn
        if (millis() - _btn_start_time > TFT_SDGRID_BTN_PRESS_DELAY_MS)
        {
            _selection_done = true;
        }
        return true;
    }

    _btn_start_time = 0;
    return true;
}

bool TFT_sd_grid::handleTouch(int16_t x, int16_t y)
{
    if (x < _x || x > _x + _wd || y < _y || y > _y + _ht)
    {
        _selected = false;
        return false; // touch is not in bounds
    }

    _selected = true;

    // Check LOAD button
    if ((x > _load_btn_x) && (x < _load_btn_x + TFT_SDGRID_BTN_WD) &&
        (y > _load_btn_y) && (y < _load_btn_y + TFT_SDGRID_BTN_HT))
    {
        setButton(TFT_SD_GRID_BTN_LOAD);
        return true;
    }

    // Check SAVE button
    if ((x > _save_btn_x) && (x < _save_btn_x + TFT_SDGRID_BTN_WD) &&
        (y > _save_btn_y) && (y < _save_btn_y + TFT_SDGRID_BTN_HT))
    {
        setButton(TFT_SD_GRID_BTN_SAVE);
        return true;
    }

    bool hit = false;
    for (uint8_t i = 1; i <= TFT_SDGRID_CELL_CNT; i++)
    {
        hit = cellHit(x, y, i);
        if (hit)
        {
            setValue(i);
            return true;
        }
    }

    return true; // touch is in bounds
}

bool TFT_sd_grid::buttonHit(int16_t x, int16_t y, uint8_t btn_idx)
{
    int16_t btn_x, btn_y;

    // Determine the x, y coordinate for this button
    if (btn_idx == 1)  // TODO: make more flexible
    {
        btn_x = _x + 40;
    }
    else
    {
        btn_x = _x + _wd - TFT_SDGRID_BTN_WD - 40;
    }
    btn_y = _y + 16 +  (TFT_SDGRID_CELL_HT * 3);

    if (x >= btn_x && x < btn_x + TFT_SDGRID_BTN_WD && 
        y >= btn_y && y < btn_y + TFT_SDGRID_BTN_HT)
    {
        return true; 
    }

    return false;
}

bool TFT_sd_grid::cellHit(int16_t x, int16_t y, uint8_t cell_idx)
{
    int16_t cell_x, cell_y;

    // Determine the x, y coordinate for this cell
    cell_x = _x + SEL_BTN_SCRN_X(cell_idx);
    cell_y = _y + SEL_BTN_SCRN_Y(cell_idx);

    if ((x >= cell_x) && (x < cell_x + TFT_SDGRID_CELL_WD) && 
        (y >= cell_y) && (y < cell_y + TFT_SDGRID_CELL_HT))
    {
        return true; 
    }

    return false;
}

void TFT_sd_grid::setRange(int16_t min_value, int16_t max_value)
{
    _min = min_value;
    _max = max_value;
}

TFT_sd_grid_btn_t TFT_sd_grid::getButton()
{
    return _btn_idx;
}

void TFT_sd_grid::setButton(TFT_sd_grid_btn_t btn_idx)
{
    if (btn_idx > TFT_SDGRID_BTN_CNT) return;

    if (btn_idx != _btn_idx)
    {
         _changed = true;
         _btn_start_time = millis();
    }

    _btn_idx = btn_idx;
}

bool TFT_sd_grid::getSelectionDone()
{
    return _selection_done;
}

void TFT_sd_grid::setSdPresent(bool sd_present)
{
    _sd_present = sd_present;
}

void TFT_sd_grid::reset()
{
    _selection_done = false;
    _btn_idx = TFT_SD_GRID_BTN_NONE;
}

//------------ PRIVATE METHODS ---------------//

/*
 * Draws the specified cell, either as selected/highlighted, or normal.
 */
void TFT_sd_grid::drawCell(ILI9341_t3 *tft, uint8_t cell, bool selected)
{
  if (cell == 0 || cell > TFT_SDGRID_CELL_CNT)
    return;

  uint16_t color = selected ? ILI9341_WHITE : ILI9341_ORANGE;
  uint16_t x = _x + SEL_BTN_SCRN_X(cell);
  uint16_t y = _y + SEL_BTN_SCRN_Y(cell);
  
  tft->fillRect(x, y, TFT_SDGRID_CELL_WD, TFT_SDGRID_CELL_HT, color);

  // Erase the margins around the cell
  tft->fillRect(x + TFT_SDGRID_CELL_WD, y, 4, TFT_SDGRID_CELL_HT, _bgd_color);
  tft->fillRect(x, y + TFT_SDGRID_CELL_HT, TFT_SDGRID_CELL_WD + 4, 4, _bgd_color);

  tft->setTextSize(1);
  tft->setCursor(x + 14, y + 10);
  tft->setTextColor(ILI9341_BLACK, color);
  tft->print(cell);
}