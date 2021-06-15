#ifndef _SYN_FILTER_
#define _SYN_FILTER_

#include "SYN_common.h"
#include "SYN_buffer.h"

class SYN_filter
{
  public:
    SYN_filter();
    void  setConfig(SYN_filter_config_t *filter_cfg);
    void  setType(SYN_filter_type filter_type);
    void  setCutoffFreq(float frequency);
    void  setResonance(float resonance);
    float next(float sample);
    void  apply(SYN_buffer *buff, size_t length);
    bool  getActive();
    void  setActive(bool active);
    
  private:
    SYN_filter_type _filter_type;
    float  _frequency;
    float  _cutoff_freq;
    float  _resonance;
    float  _a0, _a1;
    float  _z0, _z1;
    bool   _active = false;
};

#endif // _SYN_FILTER_