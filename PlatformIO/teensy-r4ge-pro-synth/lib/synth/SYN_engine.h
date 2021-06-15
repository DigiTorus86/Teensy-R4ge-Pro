#ifndef _SYN_ENGINE_
#define _SYN_ENGINE_

#include <Audio.h>
#include "SYN_common.h"
#include "SYN_buffer.h"
#include "SYN_filter.h"
#include "SYN_i2s.h"
#include "SYN_midi.h"
#include "SYN_operator.h"

#define SYN_ENG_OP_CNT         4
#define SYN_ENG_FILTER_CNT     1
#define SYN_ENG_AUDIO_LEN   8192  // Must be power of 2 
#define SYN_ENG_UPDATE_LEN  1024
#define SYN_ENG_PLAY_LEN    1024

class SYN_engine
{
  public:
    SYN_engine();
    void setAudioQueue(AudioPlayQueue *audio_queue);
    void setOpConfig(uint8_t op_num, SYN_op_config_t *op_cfg);
    void setFilterConfig(uint8_t filter_num, SYN_filter_config_t *filter_cfg);
    void setGlobalConfig(SYN_global_config_t *global_cfg);
    void update();
    void noteOn(uint8_t channel, uint8_t note_num, uint8_t velocity);
    void noteOff(uint8_t channel, uint8_t note_num);
    void allOff();
    void pitchBend(float bend);
    void modLevel(float modulation);
    
    
  private:
    void mixBuffers();

    SYN_operator _op[SYN_ENG_OP_CNT];
    SYN_filter _fltr = SYN_filter();
    SYN_i2s _i2s = SYN_i2s();
    SYN_buffer _buff  = SYN_buffer(SYN_ENG_AUDIO_LEN);
    SYN_buffer _buff2 = SYN_buffer(SYN_ENG_UPDATE_LEN * 2);  // 8192 caused blank screen and garbled serial output
    SYN_global_config_t _global_cfg = { .route = SYN_ROUTE_1234 };
    
    SYN_played_note_type _played_note[SYN_MAX_VOICES];
    uint8_t              _note_idx = 0;

    //float    _note_freq[SYN_ENG_OP_CNT]; 
    //uint32_t _note_chg_time[SYN_ENG_OP_CNT];
    //bool     _released[SYN_ENG_OP_CNT];
    
    float    _mod_level = 1.0;  // No modulation change
    float    _pitch_bend = 1.0; // No pitch bend
};

#endif // _SYN_ENGINE_
