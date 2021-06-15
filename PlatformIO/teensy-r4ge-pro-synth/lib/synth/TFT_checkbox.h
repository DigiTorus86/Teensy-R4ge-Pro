/**
 * @file TFT_chkbox.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief  A checkbox control for yes-no, on-off selections
 * @version 0.1
 * @date 2020-08-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _TFT_CHECKBOX_
#define _TFT_CHECKBOX_

#include <ILI9341_t3.h>
#include "TFT_control.h"

#define TFT_CHKBOX_UNCHECKED           0
#define TFT_CHKBOX_CHECKED             1

#define TFT_CHKBOX_DEFAULT_WD         60
#define TFT_CHKBOX_DEFAULT_HT         12

#define TFT_CHKBOX_BOX_WD             12
#define TFT_CHKBOX_BOX_HT             12
#define TFT_CHKBOX_LBL_HT              8
#define TFT_CHKBOX_MIN_WD              8
#define TFT_CHKBOX_MIN_HT              8

#define TFT_CHKBOX_DEFAULT_VALUE      TFT_CHKBOX_UNCHECKED

#define TFT_CHKBOX_DEFAULT_BGD_COLOR  ILI9341_DARKGREY
#define TFT_CHKBOX_DEFAULT_BTN_COLOR  ILI9341_ORANGE
#define TFT_CHKBOX_SELECTED_BTN_COLOR ILI9341_YELLOW
#define TFT_CHKBOX_OUTLINE_BTN_COLOR  0x7281



class TFT_checkbox : public TFT_control
{
  public:
    TFT_checkbox(int16_t x, int16_t y, const char* label);
    void     draw(ILI9341_t3 *tft, bool force_redraw);
    bool     handleTouch(int16_t x, int16_t y);
    bool     handleButtons(bool up_pressed, bool down_pressed);
    
  private:
    int16_t  _ctr_x;
    int16_t  _min_y;
    int16_t  _max_y;
    int16_t  _scale_value;
    float    _scale_factor;
    uint16_t _bgd_color;
    uint16_t _btn_color;

};

#endif // _TFT_CHECKBOX_
