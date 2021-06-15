/**
 * @file TFT_select_route.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief  An up-down route selector control with a route name display
 *         of the selected item. 
 * @version 0.1
 * @date 2021-06-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _TFT_SELECT_ROUTE_
#define _TFT_SELECT_ROUTE_

#include <ILI9341_t3.h>
#include "SYN_common.h"
#include "SYN_midi.h"
#include "TFT_control.h"
#include "TFT_waveform.h"

#define TFT_SEL_ROUTE_DEFAULT_WD  32
#define TFT_SEL_ROUTE_DEFAULT_HT  80

#define TFT_SEL_ROUTE_BTN_HT      10 
#define TFT_SEL_ROUTE_LBL_HT       8
#define TFT_SEL_ROUTE_MIN_WD      20
#define TFT_SEL_ROUTE_MIN_HT      40

#define TFT_SEL_ROUTE_DEFAULT_BGD_COLOR  ILI9341_DARKGREY
#define TFT_SEL_ROUTE_DEFAULT_BTN_COLOR  ILI9341_ORANGE
#define TFT_SEL_ROUTE_SELECTED_BTN_COLOR ILI9341_YELLOW
#define TFT_SEL_ROUTE_OUTLINE_BTN_COLOR  0x7281

#define TFT_SEL_ROUTE_TOUCH_REPEAT_DELAY_MS 250

class TFT_select_route : public TFT_control
{
  public:
    TFT_select_route(int16_t x, int16_t y, const char* label);
    void     draw(ILI9341_t3 *tft, bool force_redraw);
    bool     handleTouch(int16_t x, int16_t y);
    bool     handleButtons(bool up_pressed, bool down_pressed);  
    
  private:
    void     drawRoute(ILI9341_t3 *tft, SYN_route_type route);

    int16_t  _ctr_x;
    int16_t  _ctr_y;
    int16_t  _line1_y, _line2_y, _line3_y;
    uint8_t  _item_count;
    uint16_t _bgd_color;
    uint16_t _btn_color;
    uint32_t _touched_ms;

};

#endif // _TFT_SELECT_ROUTE_