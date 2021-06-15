#include "SYN_operator.h"

SYN_operator::SYN_operator()
{
    
}

/**
 * @brief Set the operator configuration
 * 
 * @param op_cfg 
 */
void SYN_operator::setConfig(SYN_op_config_t *op_cfg)
{
    // TODO: handle envelope parameters

    _op_cfg = {
        .op_mode = op_cfg->op_mode,
        .osc_wave = op_cfg->osc_wave,
        .osc_freq = op_cfg->osc_freq,
        .osc_phase = op_cfg->osc_phase,
        .osc_fixed = op_cfg->osc_fixed,
        .osc_lvl = op_cfg->osc_lvl,
        .atk_lvl = op_cfg->atk_lvl,
        .atk_dur = op_cfg->atk_dur,
        .dec_lvl = op_cfg->dec_lvl,
        .dec_dur = op_cfg->dec_dur,
        .sus_lvl = op_cfg->sus_lvl,
        .sus_dur = op_cfg->sus_dur,
        .rel_lvl = op_cfg->rel_lvl,
        .rel_dur = op_cfg->rel_dur
    };

    reset();
    fillOscTable();
}

/**
 * @brief Push data to the audio output buffer as a signal carrier.
 * 
 * @param buff       The audio output buffer.
 * @param length     The number of samples to calculate and push.
 * @param elapsed_ms Milliseconds since the note was triggered or released.
 * @param released   If true, elapsed_ms is time from note release.  
 *                   If false, elapsed_ms is time from note trigger.
 */
void SYN_operator::fillBuffer(uint8_t voice, float frequency, float sample_rate, SYN_buffer *buff, 
                              size_t length, uint32_t elapsed_ms, bool released)
{
    float osc_step, osc_idx, sample, amp, ms_per_sample;

    osc_step = getOscStep(frequency, sample_rate);
    ms_per_sample = 1000 / sample_rate;

    osc_idx = _osc_idx[voice];

    for(size_t i = 0; i < length; i++) 
    {
        amp = getEvelopeAmp(elapsed_ms + (uint32_t)((float)i * ms_per_sample), released); 
        sample = amp * _osc_table[(size_t)osc_idx] * _op_cfg.osc_lvl;
        buff->push(sample);

        osc_idx += osc_step;

        // Apply a generalized modulus, allowing for positive and negative frequencies
        while (osc_idx >= SYN_OP_OSC_LEN) osc_idx -= SYN_OP_OSC_LEN;
        while (osc_idx < 0) osc_idx += SYN_OP_OSC_LEN;
    }
    _osc_idx[voice] = osc_idx;
}

/**
 * @brief Mix data to the audio output buffer as a secondary signal carrier.
 * 
 * @param buff       The audio output buffer.
 * @param length     The number of samples to calculate and update.
 * @param elapsed_ms Milliseconds since the note was triggered or released.
 * @param released   If true, elapsed_ms is time from note release.  
 *                   If false, elapsed_ms is time from note trigger.
 */
void SYN_operator::mixBuffer(uint8_t voice, float frequency, float sample_rate, SYN_buffer *buff, 
                             size_t length, uint32_t elapsed_ms, bool released)
{
    float osc_step, osc_idx, sample, amp;
    
    osc_step = getOscStep(frequency, sample_rate);
    osc_idx = _osc_idx[voice];

    for(size_t i = 0; i < length; i++) 
    {    
        amp = getEvelopeAmp(elapsed_ms + (uint32_t)((float)i * 0.01), released); 
        sample = amp * _osc_table[(size_t)osc_idx] * _op_cfg.osc_lvl;
        buff->update(sample, 0.5, i); // average this op with existing audio 

        osc_idx += osc_step;

        // Apply a generalized modulus, allowing for positive and negative frequencies
        while (osc_idx >= SYN_OP_OSC_LEN) osc_idx -= SYN_OP_OSC_LEN;
        while (osc_idx < 0) osc_idx += SYN_OP_OSC_LEN;
    }
    _osc_idx[voice] = osc_idx;
}

/**
 * @brief Modify the existing data in the buffer as a signal modulator.
 * 
 * @param buff       The audio output buffer.
 * @param length     The number of samples to calculate and update.
 * @param elapsed_ms Milliseconds since the note was triggered or released.
 * @param released   If true, elapsed_ms is time from note release.  
 *                   If false, elapsed_ms is time from note trigger.
 * @param mod_level  Modifier to default oscillator level,
 *                   usually from a mod wheel, joystick, or control automation.
 */
void SYN_operator::modulateBuffer(float frequency, float sample_rate, SYN_buffer *buff, 
                                  size_t length, uint32_t elapsed_ms, bool released, float mod_level)
{
    float osc_step = 0; 
    float osc_idx, multiplier, amp;
    
    if (_op_cfg.osc_fixed)
    {
        osc_step = getOscStep(1, sample_rate);  // Only use fixed osc frequency
    }
    else
    {
        osc_step = getOscStep(frequency, sample_rate);  // Use osc fixed freq * note freq
    }

    for(size_t i = 0; i < length; i++) 
    {    
        amp = getEvelopeAmp(elapsed_ms, released); 
        multiplier = amp * _osc_table[(size_t)osc_idx] * _op_cfg.osc_lvl * mod_level;
        buff->update(multiplier, i);

        osc_idx += osc_step;

        // Apply a generalized modulus, allowing for positive and negative frequencies
        while (osc_idx >= SYN_OP_OSC_LEN) osc_idx -= SYN_OP_OSC_LEN;
        while (osc_idx < 0) osc_idx += SYN_OP_OSC_LEN;
    }
}

bool SYN_operator::getActive()
{
    return  (_op_cfg.osc_wave != SYN_WAVE_SILENCE && _op_cfg.osc_freq != 0);
}

SYN_op_mode_type SYN_operator::getMode()
{
    return _op_cfg.op_mode;
}

void SYN_operator::setMode(SYN_op_mode_type op_mode)
{
    _op_cfg.op_mode = op_mode;
    fillOscTable();
}

void SYN_operator::reset()
{
    for (uint8_t i = 0; i < SYN_MAX_VOICES; i++)
    {
        _osc_idx[i] = _op_cfg.osc_phase;
    }
}

//----- PRIVATE METHODS -----//

float SYN_operator::getEvelopeAmp(uint32_t elapsed_ms, bool released)
{
    float amp = 1;

    if (released)
    {
        // Check if passed the release time
        if (elapsed_ms >= _op_cfg.rel_dur) return 0;
        
        amp = _op_cfg.sus_lvl - ((_op_cfg.sus_lvl - _op_cfg.rel_lvl) * ((float)elapsed_ms / _op_cfg.rel_dur));
        return amp;  
    }

    // Attack
    if (elapsed_ms <= _op_cfg.atk_dur)
    {
        if (_op_cfg.atk_dur < 1) return _op_cfg.atk_lvl;  // avoid div by zero

        amp = _op_cfg.atk_lvl * ((float)elapsed_ms / _op_cfg.atk_dur);
        return amp;  
    }
    
    // Decay
    if (elapsed_ms <= _op_cfg.atk_dur + _op_cfg.dec_dur)
    {
        if (_op_cfg.dec_dur < 1) return _op_cfg.dec_lvl;  // avoid div by zero

        amp = _op_cfg.atk_lvl - (_op_cfg.atk_lvl - _op_cfg.dec_lvl) * ((float)elapsed_ms / (_op_cfg.dec_dur + _op_cfg.atk_dur));
        return amp;  
    }

    // Sustain is the only remaining option
    return _op_cfg.sus_lvl;
}

float SYN_operator::getOscStep(float frequency, float sample_rate)
{
    if (sample_rate == 0) return 0; // avoid divide by zero

    return _op_cfg.osc_freq * frequency * SYN_OP_OSC_LEN / sample_rate;
}

void  SYN_operator::fillOscTable()
{
    //Serial.print("OSC freq: "); Serial.println(_op_cfg.osc_freq);
    //Serial.print("OSC lvl: "); Serial.println(_op_cfg.osc_lvl);
    //Serial.print("Rel dur: "); Serial.println(_op_cfg.rel_dur);

    if (_op_cfg.op_mode == SYN_OP_MODE_CARRIER)
    {
        //Serial.print("Filling OSC carrier: "); Serial.println(_op_cfg.osc_wave);
        switch (_op_cfg.osc_wave)
        {
            case SYN_WAVE_SINE:
                fillSine();
                break;
            case SYN_WAVE_SQUARE:
                fillSquare(-1, 1);
                break;
            case SYN_WAVE_TRIANGLE:
                fillTriangle();
                break;
            case SYN_WAVE_SAW:
                fillSaw();
                break;
            case SYN_WAVE_RAMP:
                fillRamp();
                break;
            case SYN_WAVE_MAJOR:
                fillMajor();
                break;
            case SYN_WAVE_MINOR:
                fillMinor();
                break;
            case SYN_WAVE_OCT3:
                fillOct3();
                break;
            case SYN_WAVE_CUSTOM:
                fillSilence(); //fillCustom(&some_wav_ptr)
                break;
            default:
                fillSilence();
                break; 
        }
    }
    else // Fill osc table with MODULATOR wave data
    {
        //Serial.print("Filling OSC modulator: "); Serial.println(_op_cfg.osc_wave);
        switch (_op_cfg.osc_wave)
        {
            case SYN_WAVE_SINE:
                fillSineMod();
                break;
            case SYN_WAVE_SQUARE:
                fillSquare(0, 1);
                break;
            case SYN_WAVE_TRIANGLE:
                fillTriangleMod();
                break;
            case SYN_WAVE_SAW:
                fillSawMod();
                break;
            case SYN_WAVE_RAMP:
                fillRampMod();
                break;
            case SYN_WAVE_MAJOR:
                fillMajor();
                break;
            case SYN_WAVE_MINOR:
                fillMinor();
                break;
            case SYN_WAVE_OCT3:
                fillOct3();
                break;
            case SYN_WAVE_CUSTOM:
                fillSilence(); //fillCustomMod(&some_wav_ptr)
            default:
                fillSilence();
                break; 
        }
    }
    
}

/**
 * @brief Fill the oscillator wave table with zeros (silence).
 */
void SYN_operator::fillSilence()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = 0;
    }
}

/**
 * @brief Fills the oscillator wave table with a single sine wave with amplitude of -1.0 to 1.0.
 */
void SYN_operator::fillSine()
{
    double two_pi_scaled = 2 * PI / SYN_OP_OSC_LEN;
    float sample;

    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        sample = (float)sin((double)i * two_pi_scaled);
        _osc_table[i] = sample;
    }
}

/**
 * @brief Fills the oscillator wave table with a single sine wave with amplitude of 0.0 to 1.0.
 */
void SYN_operator::fillSineMod()
{
    double two_pi_scaled = 2 * PI / SYN_OP_OSC_LEN;
    float sample;

    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        sample = (float)((1 + sin((double)i * two_pi_scaled)) / 2);
        _osc_table[i] = sample;
    }
}

void SYN_operator::fillSquare(float min_val, float max_val)
{
    size_t half = SYN_OP_OSC_LEN >> 1;

    for (size_t i = 0; i < half; i++)
    {
        _osc_table[i] = min_val;
        _osc_table[i + half] = max_val;
    }
}

void SYN_operator::fillTriangle()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN / 2; i++)
    {
        _osc_table[i] = (((float)i * 2 / (float)SYN_OP_OSC_LEN)) - 1;
    }
    for (size_t i = SYN_OP_OSC_LEN / 2; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = 1.0 - ((float)i * 2 / (float)SYN_OP_OSC_LEN);
    }
}

void SYN_operator::fillTriangleMod()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN / 2; i++)
    {
        _osc_table[i] = (((float)i / (float)SYN_OP_OSC_LEN)) - 1;
    }
    for (size_t i = SYN_OP_OSC_LEN / 2; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = 1.0 - ((float)i / (float)SYN_OP_OSC_LEN);
    }
}

void SYN_operator::fillSaw()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = 1.0 - ((float)i * 2 / (float)SYN_OP_OSC_LEN);
    }
}

void SYN_operator::fillSawMod()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = 1.0 - ((float)i / (float)SYN_OP_OSC_LEN);
    }
}

void SYN_operator::fillRamp()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = (((float)i * 2 / (float)SYN_OP_OSC_LEN)) - 1;
    }
}

void SYN_operator::fillRampMod()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = ((float)i / (float)SYN_OP_OSC_LEN);
    }
}

void SYN_operator::fillMajor()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = 0;  // TODO!
    }
}

void SYN_operator::fillMinor()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = 0; // TODO!
    }
}

void SYN_operator::fillOct3()
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        _osc_table[i] = 0;  // TODO!
    }
}

/**
 * @brief Fills wave buffer with custom waveform stored in Flash, scaled as a carrier.
 *        Expects 16-bit waveform, with silence @ zero.
 * 
 * @param waveform 
 */
void SYN_operator::fillCustom(uint16_t *waveform)
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        // Convert from 16bit WAV to -1.0 + 1.0
        _osc_table[i] = (float)waveform[i] / 32768; 
    }
}

/**
 * @brief Fills wave buffer with custom waveform stored in Flash, scaled as a modulator.
 *        Expects 16-bit waveform, with silence @ zero.
 * 
 * @param waveform 
 */
void SYN_operator::fillCustomMod(uint16_t *waveform)
{
    for (size_t i = 0; i < SYN_OP_OSC_LEN; i++)
    {
        // Convert from 16bit WAV to 0.0 + 1.0
        _osc_table[i] = ((float)waveform[i] / 65536) + .5; 
    }
}

