#ifndef _SYN_I2S_
#define _SYN_I2S_

#include <Audio.h>
#include "SYN_common.h"
#include "SYN_buffer.h"

#define SYN_I2S_DEFAULT_LRCK_PIN  25
#define SYN_I2S_DEFAULT_BCLK_PIN  26
#define SYN_I2S_DEFAULT_DOUT_PIN   4

#define SYN_I2S_SAMPLE_RATE     11025
#define SYN_I2S_DMA_BUFF_CNT        8
#define SYN_I2S_DMA_BUFF_LEN       64

#define   SYN_I2S_BUFFER_SIZE          256 
#define   SYN_I2S_SAMPLES_PER_BUFFER   128   // 2 bytes per sample

class SYN_i2s
{
  public:
    SYN_i2s();
    bool initAudio(AudioPlayQueue *audio_queue);
    void playAudio(SYN_buffer *buff, size_t length);
    void stopAudio();
    
    
  private:
    AudioPlayQueue *_audio_queue;
    bool _initialized;
};

#endif // _SYN_I2S_