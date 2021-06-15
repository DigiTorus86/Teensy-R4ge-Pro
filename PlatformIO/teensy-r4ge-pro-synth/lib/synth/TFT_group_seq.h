#ifndef _TFT_GROUP_SEQ_
#define _TFT_GROUP_SEQ_

#include "TFT_group.h"

enum TFT_seq_type
{
  
};

class TFT_group_seq : public TFT_group
{
  public:
    TFT_group_seq();
    void getSeqConfig(SYN_sequence_config_t *seq_cfg);
    void setSeqConfig(SYN_sequence_config_t *seq_cfg);
    
  private:
    float _tempo;

};

#endif // _TFT_GROUP_SEQ_
