/**
 * @file TFT_keyboard.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief  A graphical piano keyboard for selecting or displaying note values
 * @version 0.1
 * @date 2020-08-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _TFT_KEYBOARD_
#define _TFT_KEYBOARD_

#include <ILI9341_t3.h>
#include "TFT_control.h"

#define TFT_KBRD_DEFAULT_WD         320
#define TFT_KBRD_DEFAULT_HT          36

#define TFT_KBRD_KEY_WD              16
#define TFT_KBRD_BTN_WD              12 

#define TFT_KBRD_DEFAULT_VALUE       12
#define TFT_KBRD_MIN_OCT              4
#define TFT_KBRD_MAX_OCT              6
#define TFT_KBRD_NUM_KEYS            29 
#define TFT_KBRD_KEYPOS_SCALE_X       5
#define TFT_KBRD_KEYPOS_OFFSET_X     32


#define TFT_KBRD_DEFAULT_BGD_COLOR  ILI9341_BLACK
#define TFT_KBRD_DEFAULT_BTN_COLOR  ILI9341_ORANGE
#define TFT_KBRD_SELECTED_BTN_COLOR ILI9341_YELLOW
#define TFT_KBRD_OUTLINE_BTN_COLOR  0x7281

#define TFT_KBRD_TOUCH_REPEAT_DELAY_MS 200

class TFT_keyboard : public TFT_control
{
  public:
    TFT_keyboard(int16_t x, int16_t y, const char* label);
    void     draw(ILI9341_t3 *tft, bool force_redraw);
    bool     handleTouch(int16_t x, int16_t y);
    bool     handleButtons(bool up_pressed, bool down_pressed, bool left_pressed, bool right_pressed);
    void     setRange(int16_t min_value, int16_t max_value);
    int16_t  getOctave();
    void     setOctave(int16_t octave);
    
  private:
    bool     keyHit(int16_t x, int16_t y, uint8_t key_idx);

    uint16_t _bgd_color;
    uint16_t _btn_color;
    int16_t  _octave;
    int16_t  _half_ht;
    uint32_t _touched_ms;
    

    /** 
     * @brief Array for drawing and detecting touches to keyboard keys based on the 
     *        lower left corner x,y unscaled coordinates (not pixels).     
     *              KEY IDX
     *              24   25   26  27    28
     *              12   13   14  15    16    17    18   19  20   21  22   23   
     *          Y    0    1    2   3     4     5     6    7   8    9  10   11
     *          0 |    |     |  |     |     |     |     |  |     |  |    |      |
     *            |    |     |  |     |     |     |     |  |     |  |    |      |
     *          1 |    |_____|  |_____|     |     |_____|  |_____|  |____|      |
     *            |       |        |        |        |        |        |        |
     *          2 |_______|________|________|________|________|________|________|
     * 
     *        X: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21  (Octave 1)
     *           21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42  (Octave 2)
     *           42 43 44 45 46 47 48 49 50 51                              (Partial Octave 3)
     */
    uint8_t  _key_pos[29][2] = {  // unscaled {x, y} for lower left corner
        { 0, 2}, // C1
        { 2, 1}, // C#1
        { 3, 2}, // D1
        { 5, 1}, // D#1
        { 6, 2}, // E1
        { 9, 2}, // F1
        {11, 1}, // F#1
        {12, 2}, // G1
        {14, 1}, // G#1
        {15, 2}, // A1
        {17, 1}, // A#1
        {18, 2}, // B1
        {21, 2}, // C2
        {23, 1}, // C#2
        {24, 2}, // D2
        {26, 1}, // D#2
        {27, 2}, // E2
        {30, 2}, // F2
        {32, 1}, // F#2
        {33, 2}, // G2
        {35, 1}, // G#2
        {36, 2}, // A2
        {38, 1}, // A#2
        {39, 2}, // B2
        {42, 2}, // C3
        {44, 1}, // C#3
        {45, 2}, // D3
        {47, 1}, // D#3
        {48, 2}, // E3
    };

};

#endif // _TFT_KEYBOARD_