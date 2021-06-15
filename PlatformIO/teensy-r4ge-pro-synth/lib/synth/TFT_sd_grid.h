/**
 * @file TFT_sd_grid.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief  A graphical grid for selecting SD files/cells
 * @version 0.1
 * @date 2020-08-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _TFT_SD_GRID_
#define _TFT_SD_GRID_

#include <ILI9341_t3.h>
#include "TFT_control.h"

#define TFT_SDGRID_DEFAULT_WD         320
#define TFT_SDGRID_DEFAULT_HT         166

#define TFT_SDGRID_CELL_WD             60
#define TFT_SDGRID_CELL_HT             40
#define TFT_SDGRID_BTN_WD              80
#define TFT_SDGRID_BTN_HT              24
#define TFT_SDGRID_BTN_CNT              2

#define TFT_SDGRID_DEFAULT_VALUE        0          
#define TFT_SDGRID_CELL_CNT            15

#define TFT_SDGRID_DEFAULT_BGD_COLOR   ILI9341_BLACK
#define TFT_SDGRID_DEFAULT_CELL_COLOR  ILI9341_ORANGE
#define TFT_SDGRID_SELECTED_CELL_COLOR ILI9341_YELLOW

#define TFT_SDGRID_DEFAULT_BTN_COLOR   0x7281
#define TFT_SDGRID_SELECTED_BTN_COLOR  0x7281
#define TFT_SDGRID_OUTLINE_BTN_COLOR   0x7281
#define TFT_SDGRID_TEXT_BTN_COLOR      ILI9341_WHITE

#define TFT_SDGRID_BTN_PRESS_DELAY_MS  500

#define SEL_BTN_SCRN_X(btn) ((((btn) - 1) % 5) * (TFT_SDGRID_CELL_WD + 4))
#define SEL_BTN_SCRN_Y(btn) ((((btn) - 1) / 5) * (TFT_SDGRID_CELL_HT + 4))

enum TFT_sd_grid_btn_t
{
    TFT_SD_GRID_BTN_NONE,
    TFT_SD_GRID_BTN_LOAD,
    TFT_SD_GRID_BTN_SAVE
};

class TFT_sd_grid : public TFT_control
{
  public:
    TFT_sd_grid(int16_t x, int16_t y, const char* label);
    void     draw(ILI9341_t3 *tft, bool force_redraw);
    bool     handleTouch(int16_t x, int16_t y);
    bool     handleButtons(bool up_pressed, bool down_pressed, bool left_pressed, bool right_pressed);
    void     setRange(int16_t min_value, int16_t max_value);
    TFT_sd_grid_btn_t getButton();  
    void     setButton(TFT_sd_grid_btn_t btn_idx);
    bool     getSelectionDone();
    void     setSdPresent(bool sd_present);
    void     reset();
    
  private:
    bool     buttonHit(int16_t x, int16_t y, uint8_t btn_idx);
    bool     cellHit(int16_t x, int16_t y, uint8_t btn_idx);
    void     drawCell(ILI9341_t3 *tft, uint8_t cell, bool selected);
    

    TFT_sd_grid_btn_t _btn_idx;
    uint16_t _bgd_color;
    uint16_t _btn_color;
    uint32_t _btn_start_time;
    bool     _selection_done;
    bool     _sd_present;
    int16_t  _load_btn_x, _load_btn_y;
    int16_t  _save_btn_x, _save_btn_y;

};

#endif // _TFT_SD_GRID_