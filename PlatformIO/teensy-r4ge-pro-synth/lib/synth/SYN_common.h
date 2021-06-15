/**
 * @file SYN_common.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief  Common include definitions for the synthesizer engine library.
 * @version 0.1
 * @date 2020-08-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _SYN_COMMON_
#define _SYN_COMMON_

#include <Arduino.h>

#define SYN_MAX_VOICES        4
#define SYN_WAVE_TYPE_COUNT  10
#define SYN_ROUTE_TYPE_COUNT  4
#define SYN_SEQ_NOTE_COUNT    8


#define SYN_CFG_HDR_SYN1     0x314E5953  // "SYN1" big-endian
 
enum SYN_wave_type
{
    SYN_WAVE_SILENCE,
    SYN_WAVE_SINE,
    SYN_WAVE_SQUARE,
    SYN_WAVE_TRIANGLE,
    SYN_WAVE_SAW,
    SYN_WAVE_RAMP,
    SYN_WAVE_MAJOR,
    SYN_WAVE_MINOR,
    SYN_WAVE_OCT3,
    SYN_WAVE_CUSTOM
    // Update SYN_WAVE_TYPE_COUNT above if you add more wave types!
};

enum SYN_route_type
{
    SYN_ROUTE_1234,
    SYN_ROUTE_12_34,
    SYN_ROUTE_123_4,
    SYN_ROUTE_1_2_3_4
    // Update SYN_ROUTE_TYPE_COUNT above if you add more route algorithm types!
};

enum SYN_filter_type
{
    SYN_FLTR_NONE,
    SYN_FLTR_LOPASS,
    SYN_FLTR_HIPASS,
    SYN_FLTR_BANDPASS,
    SYN_FLTR_NOTCH
};

enum SYN_op_mode_type
{
    SYN_OP_MODE_CARRIER,
    SYN_OP_MODE_OSCILLATOR
};

struct SYN_header_config_t
{
    uint32_t header_id;
    uint16_t version;
    uint16_t reserved;
};

struct SYN_op_config_t
{
    SYN_op_mode_type op_mode;
    SYN_wave_type osc_wave;
    float osc_freq;
    float osc_phase;
    bool  osc_fixed;
    float osc_lvl;
    float atk_lvl;
    float atk_dur;
    float dec_lvl;
    float dec_dur;
    float sus_lvl;
    float sus_dur;
    float rel_lvl;
    float rel_dur;
};

struct SYN_filter_config_t
{
    SYN_filter_type filter_type;
    float frequency;
    float level;
    float cutoff;
    float resonance;
    float param1;
    bool  active;
};

struct SYN_global_config_t
{
    SYN_route_type route;
};

struct SYN_sequence_config_t
{
    float tempo;
    uint8_t note_idx[SYN_SEQ_NOTE_COUNT];
};

struct SYN_midi_note_type
{
    uint8_t note_num;
    char name[4];
    float frequency;
};

struct SYN_played_note_type
{
    uint8_t channel;
    uint8_t note_num;
    uint8_t velocity;
    uint8_t status;  // 0 = available/unused, 1 = ready to play/on, 2 = playing, 3 = released, 4 = decaying
    float frequency;
    uint32_t start_time;
    uint32_t release_time;
};

#endif // _SYN_COMMON_