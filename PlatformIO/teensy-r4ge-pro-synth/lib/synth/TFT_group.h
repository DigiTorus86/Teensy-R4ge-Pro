#ifndef _TFT_GROUP_
#define _TFT_GROUP_

#include <ILI9341_t3.h>
#include "TFT_checkbox.h"
#include "TFT_control.h"
#include "TFT_envelope.h"
#include "TFT_select_wave.h"
#include "TFT_select_note.h"
#include "TFT_select_route.h"
#include "TFT_slider.h"
#include "SYN_common.h"

#define TFT_GRP_MAX_ITEMS        28

class TFT_group
{
  public:
    TFT_group();
    void     draw(ILI9341_t3 *tft, bool force_redraw);
    void     addItem();
    bool     handleTouch(int16_t x, int16_t y);
    bool     handleButtons(bool up_pressed, bool down_pressed, bool left_pressed, bool right_pressed);
    bool     getChanged();
    uint8_t  getSelected();
    void     setSelected(uint8_t selected_idx);
    
  protected:
    float   scaleFrequency(int16_t coarse_lvl, int16_t fine_lvl);
    float   scalePhase(int16_t phase);
    float   scaleLevel(int16_t control_lvl);
    float   scaleDuration(int16_t control_dur);
    float   scaleTempo(int16_t tempo);

    int16_t unscaleCoarseFrequency(float frequency);
    int16_t unscaleFineFrequency(float frequency);
    int16_t unscalePhase(float phase_offset);
    int16_t unscaleLevel(float config_lvl);
    int16_t unscaleDuration(float config_dur);
    int16_t unscaleTempo(float tempo);

    uint8_t _item_count;
    uint8_t _selected_idx;
    TFT_control** _items = new TFT_control*[TFT_GRP_MAX_ITEMS];

};

#endif // _TFT_GROUP_
