/**
 * @file TFT_select_wave.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief  An up-down waveform selector control with a graphical icon display
 *         of the selected item. 
 * @version 0.1
 * @date 2020-07-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _TFT_SELECT_WAVE_
#define _TFT_SELECT_WAVE_

#include <ILI9341_t3.h>
#include "SYN_common.h"
#include "TFT_control.h"
#include "TFT_waveform.h"

#define TFT_SEL_DEFAULT_WD  32
#define TFT_SEL_DEFAULT_HT  80

#define TFT_SEL_BTN_HT      10 
#define TFT_SEL_LBL_HT       8
#define TFT_SEL_MIN_WD      20
#define TFT_SEL_MIN_HT      40

#define TFT_SEL_DEFAULT_BGD_COLOR  ILI9341_DARKGREY
#define TFT_SEL_DEFAULT_BTN_COLOR  ILI9341_ORANGE
#define TFT_SEL_SELECTED_BTN_COLOR ILI9341_YELLOW
#define TFT_SEL_OUTLINE_BTN_COLOR  0x7281

#define TFT_SEL_TOUCH_REPEAT_DELAY_MS 250

class TFT_select_wave : public TFT_control
{
  public:
    TFT_select_wave(int16_t x, int16_t y, const char* label);
    void     draw(ILI9341_t3 *tft, bool force_redraw);
    bool     handleTouch(int16_t x, int16_t y);
    bool     handleButtons(bool up_pressed, bool down_pressed);  
    
  private:
    int16_t  _ctr_x;
    int16_t  _ctr_y;
    uint8_t  _item_count;
    uint16_t _bgd_color;
    uint16_t _btn_color;
    uint32_t _touched_ms;

    TFT_waveform _waveform = TFT_waveform(0, 0, 2, 2);

};

#endif // _TFT_SELECT_WAVE_