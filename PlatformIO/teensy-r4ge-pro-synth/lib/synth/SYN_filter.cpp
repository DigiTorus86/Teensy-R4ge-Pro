#include "SYN_filter.h"

SYN_filter::SYN_filter()
{    
   _a1 = 0.29;
   _a0 = _a1 - 1.0; 
   _z1 = 0;
}

void  SYN_filter::setConfig(SYN_filter_config_t *filter_cfg)
{
    setType(filter_cfg->filter_type);
    setCutoffFreq(filter_cfg->cutoff);
    setResonance(filter_cfg->resonance);
    setActive(filter_cfg->active);

    _a1 = filter_cfg->param1;  // TODO
    _a0 = _a1 - 1.0;
}

void SYN_filter::setType(SYN_filter_type filter_type)
{    
    _filter_type = filter_type; 
}

void SYN_filter::setCutoffFreq(float frequency)
{
    _cutoff_freq = frequency;
}
    
void  SYN_filter::setResonance(float resonance)
{
    _resonance = resonance;
}
    
float SYN_filter::next(float sample)
{
    float yn = _a0 * sample + _a1 * _z1;
    _z1 = sample;

    return yn;
}

void SYN_filter::apply(SYN_buffer *buff, size_t length)
{
    float sample = 0;
    float yn;

    for(size_t i = 0; i < length; i++) 
    {   
        buff->peek(&sample, i);
        yn = _a0 * sample + _a1 * _z1;
        buff->write(yn, i);
    }
    _z1 = sample;
}

bool SYN_filter::getActive()
{
    return _active;
}

void SYN_filter::setActive(bool active)
{
    _active = active;
}