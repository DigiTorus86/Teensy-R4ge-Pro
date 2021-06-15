/**
 * @file TFT_slider.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief  A slider control for picking an integer value in a specified range.
 * @version 0.1
 * @date 2020-07-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _TFT_SLIDER_
#define _TFT_SLIDER_

#include <ILI9341_t3.h>
#include "TFT_control.h"

#define TFT_SLDR_DEFAULT_WD         19
#define TFT_SLDR_DEFAULT_HT         80

#define TFT_SLDR_BTN_HT              8
#define TFT_SLDR_LBL_HT              8
#define TFT_SLDR_MIN_WD             12
#define TFT_SLDR_MIN_HT             40

#define TFT_SLDR_DEFAULT_VALUE       0
#define TFT_SLDR_DEFAULT_MIN_VAL     0
#define TFT_SLDR_DEFAULT_MAX_VAL   100

#define TFT_SLDR_DEFAULT_BGD_COLOR  ILI9341_DARKGREY
#define TFT_SLDR_DEFAULT_BTN_COLOR  ILI9341_ORANGE
#define TFT_SLDR_SELECTED_BTN_COLOR ILI9341_YELLOW
#define TFT_SLDR_OUTLINE_BTN_COLOR  0x7281

class TFT_slider : public TFT_control
{
  public:
    TFT_slider(int16_t x, int16_t y, const char* label);
    void     draw(ILI9341_t3 *tft, bool force_redraw);
    bool     handleTouch(int16_t x, int16_t y);
    bool     handleButtons(bool up_pressed, bool down_pressed);
    void     setRange(int16_t min_value, int16_t max_value);
    
  private:
    int16_t  _ctr_x;
    int16_t  _min_y;
    int16_t  _max_y;
    int16_t  _scale_value;
    float    _scale_factor;
    uint16_t _bgd_color;
    uint16_t _btn_color;

};

#endif // _TFT_SLIDER_
