#include "SYN_i2s.h"

SYN_i2s::SYN_i2s()
{
    // i2s configuration
}

/*
 * Initialize the I2S audio output
 */
bool SYN_i2s::initAudio(AudioPlayQueue *audio_queue)
{
    _audio_queue = audio_queue;

	return true;
}

/* 
 * Plays the audio sample.  
 * Does not return until sample is done playing.
 */
void SYN_i2s::playAudio(SYN_buffer *buff, size_t length)
{
  bool     audio_playing = true;
  int      sample_pos = 0;
  float    buff_val = 0;
  size_t   samples_read = 0;
  SYN_buff_err err;

  // Fill I2S transfer audio buffer from sample buffer
  while (audio_playing)
  {
    int16_t *p = _audio_queue->getBuffer();

    for (int i = 0; i < SYN_I2S_SAMPLES_PER_BUFFER; i++)
    {
        // get the next sample from the buffer
        err = buff->pop(&buff_val);
        if (err == SYN_BUFF_ERR_OK)
        {
            samples_read++;
        }
        else
        {
            //Serial.print(F("I2S play buffer pop error: ")); Serial.println(err);
            //audio_playing = false;
            buff_val = 0;
            //break;
        }

        // Convert audio -1.0 to 1.0 buffer sample to 16-bit signed int sample
        p[i]  = (int16_t)(buff_val * 16000); // sample[sample_pos + i] * 16000);
    }
    _audio_queue->playBuffer();

    sample_pos += SYN_I2S_SAMPLES_PER_BUFFER;
    if (sample_pos >= length - 1) audio_playing = false;
  }
  buff->readComplete(samples_read);
}

void SYN_i2s::stopAudio()
{
  _initialized = false;
}