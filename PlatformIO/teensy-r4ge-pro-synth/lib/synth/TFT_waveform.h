/**
 * @file TFT_waveform.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief  Displays a waveform to the screen.
 * @version 0.1
 * @date 2020-07-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _TFT_WAVEFORM_
#define _TFT_WAVEFORM_

#include <ILI9341_t3.h>
#include "TFT_control.h"
#include "SYN_common.h"

#define TFT_WAV_DEFAULT_COLOR      ILI9341_GREEN
#define TFT_WAV_DEFAULT_BGD_COLOR  ILI9341_BLACK


class TFT_waveform
{
  public:
    TFT_waveform(int16_t x, int16_t y, int16_t wd, int16_t ht);
    void     draw(ILI9341_t3 *tft, SYN_wave_type wave_type);
    void     drawName(ILI9341_t3 *tft, SYN_wave_type wave_type);
    void     setColor(uint16_t wave_color);
    void     setColor(uint16_t wave_color, uint16_t bgd_color);
    
  private:
    int16_t  _x, _y, _ht, _wd;
    int16_t  _zero_y, _ctr_y;
    int16_t  _half_wd;
    float    _scale_factor;
    uint16_t _wave_color, _bgd_color;

    void drawSilence(ILI9341_t3 *tft);
    void drawSine(ILI9341_t3 *tft);
    void drawSquare(ILI9341_t3 *tft);
    void drawTriangle(ILI9341_t3 *tft);
    void drawSaw(ILI9341_t3 *tft);
    void drawRamp(ILI9341_t3 *tft);
    void drawMajor(ILI9341_t3 *tft);
    void drawMinor(ILI9341_t3 *tft);
    void drawOct3(ILI9341_t3 *tft);
    void drawCustom(ILI9341_t3 *tft);

};

#endif // _TFT_WAVEFORM_
