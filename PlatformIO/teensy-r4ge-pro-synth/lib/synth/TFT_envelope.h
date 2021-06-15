/**
 * @file TFT_envelope.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief  Class for displaying an ADSR envelope to the screen
 * @version 0.1
 * @date 2020-08-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _TFT_ENVELOPE_
#define _TFT_ENVELOPE_

#include <ILI9341_t3.h>
#include "TFT_control.h"

#define TFT_ENV_DEFAULT_WD  60
#define TFT_ENV_DEFAULT_HT  66 //was 80

#define TFT_SEL_BTN_HT      10 
#define TFT_SEL_LBL_HT       8
#define TFT_ENV_MIN_WD      40
#define TFT_ENV_MIN_HT      40

#define TFT_ENV_DEFAULT_BGD_COLOR  ILI9341_DARKGREY
#define TFT_ENV_DEFAULT_BTN_COLOR  ILI9341_ORANGE
#define TFT_ENV_SELECTED_BTN_COLOR ILI9341_YELLOW
#define TFT_ENV_OUTLINE_BTN_COLOR  0x7281

struct TFT_env_config_t
{
    int16_t atk_lvl;
    int16_t dec_lvl;
    int16_t sus_lvl;
    int16_t rel_lvl;

    int16_t atk_dur;
    int16_t dec_dur;
    int16_t sus_dur;
    int16_t rel_dur;
};

class TFT_envelope : public TFT_control
{
  public:
    TFT_envelope(int16_t x, int16_t y, const char* label);
    void     draw(ILI9341_t3 *tft, bool force_redraw);
    bool     handleTouch(int16_t x, int16_t y);
    bool     handleButtons(bool up_pressed, bool down_pressed);
    void     setConfig(TFT_env_config_t *env_cfg);
    bool     getSelected();
    void     setSelected(bool selected);
    
    
  private:
    void     calcXY();
    int16_t  _atk_lvl, _dec_lvl, _sus_lvl, _rel_lvl;
    int16_t  _atk_dur, _dec_dur, _sus_dur, _rel_dur;
    int16_t  _atk_x, _atk_y, _dec_x, _dec_y, _sus_x, _sus_y, _rel_x, _rel_y; 
    //TFT_env_config_t _env_cfg;

    int16_t  _env_zero_x, _env_max_x;
    int16_t  _env_zero_y, _env_max_y;

    float    _scale_x, _scale_y;

    uint16_t _bgd_color;
    uint16_t _line_color;
};

#endif // _TFT_ENVELOPE_