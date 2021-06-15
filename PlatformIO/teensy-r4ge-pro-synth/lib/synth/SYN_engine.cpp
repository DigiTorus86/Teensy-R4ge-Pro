#include "SYN_engine.h"

SYN_engine::SYN_engine()
{    
}

void SYN_engine::setAudioQueue(AudioPlayQueue *audio_queue)
{    
    _i2s.initAudio(audio_queue);
}

/**
 * @brief Set the configuration for the specified operator.
 * 
 * @param op_num The zero-based operator index to change.
 * @param op_cfg The new configuration for the operator.
 */
void SYN_engine::setOpConfig(uint8_t op_num, SYN_op_config_t *op_cfg)
{
    if (op_num > 0 && op_num <= SYN_ENG_OP_CNT)
    {
        op_cfg->op_mode = (op_num == 1 ? SYN_OP_MODE_CARRIER : SYN_OP_MODE_OSCILLATOR);
        _op[op_num - 1].setConfig(op_cfg);
    }
}

void SYN_engine::setFilterConfig(uint8_t filter_num, SYN_filter_config_t *filter_cfg)
{
    if (filter_num > 0 && filter_num <= SYN_ENG_FILTER_CNT)
    {
        //_filter[filter_num - 1].setConfig(filter_cfg);
        _fltr.setConfig(filter_cfg);
    }
}

void SYN_engine::setGlobalConfig(SYN_global_config_t *global_cfg)
{
    _global_cfg.route = global_cfg->route;
}


/**
 * @brief Calculates all sound sample values and writes to the output buffer.
 *        Call continuously on every program loop.
 */
void SYN_engine::update()
{
    uint32_t elapsed_time;
    float freq;
    bool released, data_present = false;
    uint8_t active_note_cnt = 0;

    for (uint8_t note_idx = 0; note_idx < SYN_MAX_VOICES; note_idx++)
    {
        if (_played_note[note_idx].status > 0)
        {
            active_note_cnt += 1;
        }
    }
    // TODO: vary routing/mix level based on # of active notes

    for (uint8_t note_idx = 0; note_idx < SYN_MAX_VOICES; note_idx++)
    {
        if (_played_note[note_idx].start_time > 0 && _played_note[note_idx].status > 0)
        {
            
            freq = _played_note[note_idx].frequency * _pitch_bend;
            if (_played_note[note_idx].release_time > 0)
            {
                released = true;
                elapsed_time = millis() - _played_note[note_idx].release_time;
            }
            else
            {
                released = false;
                elapsed_time = millis() - _played_note[note_idx].start_time;
            }

            if (!data_present)
            {
                // Initial op data has not been loaded to first op yet
                // TODO: add volume parameter
                _op[0].fillBuffer(note_idx, freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released);
            }
            else
            {
                _op[0].mixBuffer(note_idx, freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released);
            }
            
            if (_op[1].getActive())
                _op[1].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);

            if (_op[2].getActive())
                _op[2].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);

            if (_op[3].getActive())
                _op[3].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);

            /*
            switch(_global_cfg.route)
            {
                case SYN_ROUTE_1234:
                    _op[0].fillBuffer(1, freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released);
                    
                    if (_op[1].getActive())
                        _op[1].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);

                    if (_op[2].getActive())
                        _op[2].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);

                    if (_op[3].getActive())
                        _op[3].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);
                    
                    break;
        
                case SYN_ROUTE_12_34:
                    _op[0].fillBuffer(0, freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released);
                    
                    if (_op[1].getActive())
                        _op[1].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);

                    _op[2].fillBuffer(0, freq, SYN_I2S_SAMPLE_RATE, &_buff2, SYN_ENG_UPDATE_LEN, elapsed_time, released);

                    if (_op[3].getActive())
                        _op[3].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff2, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);
                    
                    mixBuffers();
                    break;

                case SYN_ROUTE_123_4:
                    _op[0].fillBuffer(0, freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released);
                    
                    if (_op[1].getActive())
                        _op[1].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);

                    if (_op[2].getActive())
                        _op[2].modulateBuffer(freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released, _mod_level);
                    
                    _op[3].fillBuffer(0, freq, SYN_I2S_SAMPLE_RATE, &_buff2, SYN_ENG_UPDATE_LEN, elapsed_time, released);
                    
                    mixBuffers();
                    break;

                case SYN_ROUTE_1_2_3_4:
                    _op[0].fillBuffer(0, freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released);
                    _op[1].mixBuffer(0, freq, SYN_I2S_SAMPLE_RATE, &_buff, SYN_ENG_UPDATE_LEN, elapsed_time, released);
                    
                    _op[2].fillBuffer(0, freq, SYN_I2S_SAMPLE_RATE, &_buff2, SYN_ENG_UPDATE_LEN, elapsed_time, released);
                    _op[3].mixBuffer(0, freq, SYN_I2S_SAMPLE_RATE, &_buff2, SYN_ENG_UPDATE_LEN, elapsed_time, released);
                    
                    mixBuffers();
                    break;
                
                default:
                    break;
            }
            */
            data_present = true;
        } // played_note start_time
        
    } // for (note_idx)

    
    if (data_present &&_fltr.getActive())
        _fltr.apply(&_buff, SYN_ENG_UPDATE_LEN);

    // TODO: other operators and effects
    
    if (!data_present)
    {
        // Zero out the audio buffer
        for (size_t i = 0; i < SYN_ENG_UPDATE_LEN; i++)
            _buff.push(0); 
    }
    
    _buff.updateComplete(SYN_ENG_UPDATE_LEN);
    //Serial.print("Buff read size: "); Serial.println(_buff.getReadPopSize());

    _i2s.playAudio(&_buff, SYN_ENG_UPDATE_LEN);
}

/**
 * @brief Start playing a note.
 * 
 * @param channel   The MIDI channel for the note
 *                  In monophonic mode, this should be 0.
 *                  In polyphonic mode, this should be the carrier op.
 * @param note_num  The MIDI note number of the note to begin playing.
 */
void SYN_engine::noteOn(uint8_t channel, uint8_t note_num, uint8_t velocity)
{
    _op[0].reset();  // TODO: handle for all Ops based on note and role    
    _op[1].reset();

    _played_note[_note_idx].channel = channel;
    _played_note[_note_idx].note_num = note_num;
    _played_note[_note_idx].velocity = velocity;
    _played_note[_note_idx].status = 1; // ready to play
    _played_note[_note_idx].start_time = millis();
    _played_note[_note_idx].release_time = 0; // not released

    if (note_num >= 48 && note_num <= 100)
    {
        //note_freq = getFrequency(pitch);
        _played_note[_note_idx].frequency = midi_note[note_num - SYN_MIDI_NOTE_OFFSET].frequency;
    }
    else
    {
        _played_note[_note_idx].frequency = 0;
    }
  
    _note_idx = (_note_idx + 1) % SYN_MAX_VOICES;
}

/**
 * @brief Stop playing a note and silence the operator.
 * 
 * @param channel   The MIDI channel # 
 * @param note_num  The MIDI note number to silence
 */
void SYN_engine::noteOff(uint8_t channel, uint8_t note_num)
{
    for (uint8_t i = 0; i < SYN_MAX_VOICES; i++)
    {
        if (channel == _played_note[i].channel && 
            note_num == _played_note[i].note_num && 
            _played_note[i].status != 3
            )
        {
            _played_note[i].release_time = millis();
            _played_note[i].status = 3;  // released
        }
    }
}

/**
 * @brief Turn all sounds off and shut down audio engine.  
 *        Similar to MIDI panic. 
 */
void SYN_engine::allOff()
{
    for(uint8_t i = 0; i < SYN_MAX_VOICES; i++)
    {
        //_played_note[i].frequency = 0;
        _played_note[i].start_time = 0;
        _played_note[i].status = 0;  // available
    }
    
    _i2s.stopAudio();
}

/**
 * @brief Set the modulation level.  1.0 = No change to current modulation level.
 * 
 * @param modulation The modulation level multiplier.
 */
void SYN_engine::modLevel(float modulation)
{
    _mod_level = modulation;
}

/**
 * @brief Set the pitch bend (frequency multiplier) value.  1.0 = no bend.
 * 
 * @param bend The frequency multiplier to apply to the base note frequency.
 */
void SYN_engine::pitchBend(float bend)
{
    _pitch_bend = bend;
}

// ------ PRIVATE METHODS ------//

void SYN_engine::mixBuffers()
{
    float sample;

    // Buff2 is not consumed by the I2S system, so the engine is both producer and consumer
    _buff2.updateComplete(SYN_ENG_UPDATE_LEN);

    for(size_t i = 0; i < SYN_ENG_UPDATE_LEN; i++) 
    {   
        _buff2.pop(&sample);    
        _buff.update(sample, 0.5, i); // average this op with existing audio 
    }

    _buff2.readComplete(SYN_ENG_UPDATE_LEN);
}
