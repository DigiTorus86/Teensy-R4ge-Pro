/**
 * @file SYN_operator.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief In FM synthesis, an operator consists of an oscillator (waveform and frequency)
 *        along with an ADSR envelope (Attack, Decay, Sustain, Release)
 *        An operator can be used either as a signal source (carrier) or to modify a signal (modulator).
 *        Carrier waveform range:   -1.0 to 1.0
 *        Modulator waveform range:  0.0 to 1.0
 * @version 0.1
 * @date 2020-07-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _SYN_OPERATOR_
#define _SYN_OPERATOR_

#include <Arduino.h>
#include "SYN_common.h"
#include "SYN_buffer.h"

#define SYN_OP_OSC_LEN  4096

// TODO: envelope

class SYN_operator
{
  public:
    SYN_operator();
    void setConfig(SYN_op_config_t *op_cfg);
    void fillBuffer(uint8_t voice, float frequency, float sample_rate, SYN_buffer *buff, size_t length, uint32_t elapsed_ms, bool released);
    void mixBuffer(uint8_t voice, float frequency, float sample_rate, SYN_buffer *buff, size_t length, uint32_t elapsed_ms, bool released);
    void modulateBuffer(float frequency, float sample_rate, SYN_buffer *buff, size_t length, uint32_t elapsed_ms, bool released, float mod_level);
    SYN_op_mode_type getMode();
    void setMode(SYN_op_mode_type op_mode);
    void reset();
    bool getActive();
    
    
  private:
    float _osc_idx[SYN_MAX_VOICES];
    float _osc_table[SYN_OP_OSC_LEN];
    SYN_op_config_t _op_cfg; 

    float getEvelopeAmp(uint32_t elapsed_ms, bool released);
    float getOscStep(float frequency, float sample_rate);

    void fillOscTable();
    void fillSilence();
    void fillSine();
    void fillSineMod();
    void fillSquare(float min_val, float max_val);
    void fillTriangle();
    void fillTriangleMod();
    void fillSaw();
    void fillSawMod();
    void fillRamp();
    void fillRampMod();
    void fillMajor();
    void fillMinor();
    void fillOct3();
    void fillCustom(uint16_t *waveform);
    void fillCustomMod(uint16_t *waveform);
};

#endif // _SYN_OPERATOR_
