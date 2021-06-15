#ifndef _TFT_GROUP_FLTR_
#define _TFT_GROUP_FLTR_

#include "TFT_group.h"

class TFT_group_fltr : public TFT_group
{
  public:
    TFT_group_fltr();
    void getFilterConfig(uint8_t filter_num, SYN_filter_config_t *filter_cfg);
    void setFilterConfig(uint8_t filter_num, SYN_filter_config_t *filter_cfg);

    void getGlobalConfig(SYN_global_config_t *global_cfg);
    void setGlobalConfig(SYN_global_config_t *global_cfg);
    
  private:

};

#endif // _TFT_GROUP_FLTR_
