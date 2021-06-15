#ifndef _TFT_GROUP_OP12_
#define _TFT_GROUP_OP12_

#include "TFT_group.h"

enum TFT_op12_type
{
  TFT_OP1_OSC1 = 0,
  TFT_OP1_CRS = 1,
  TFT_OP1_FIN = 2,
  TFT_OP1_LVL = 3,
  TFT_OP1_ATK_LVL = 4,
  TFT_OP1_DEC_LVL = 5,
  TFT_OP1_SUS_LVL = 6,
  TFT_OP1_REL_LVL = 7,
  TFT_OP1_ATK_DUR = 8,
  TFT_OP1_DEC_DUR = 9,
  TFT_OP1_SUS_DUR = 10,
  TFT_OP1_REL_DUR = 11,
  TFT_OP1_ENV = 12,

  TFT_OP2_OSC2 = 13,
  TFT_OP2_CRS = 14,
  TFT_OP2_FIN = 15,
  TFT_OP2_LVL = 16,
  TFT_OP2_ATK_LVL = 17,
  TFT_OP2_DEC_LVL = 18,
  TFT_OP2_SUS_LVL = 19,
  TFT_OP2_REL_LVL = 20,
  TFT_OP2_ATK_DUR = 21,
  TFT_OP2_DEC_DUR = 22,
  TFT_OP2_SUS_DUR = 23,
  TFT_OP2_REL_DUR = 24,
  TFT_OP2_ENV = 25
};

class TFT_group_op12 : public TFT_group
{
  public:
    TFT_group_op12();
    void updateEnvelopes();
    void getOpConfig(uint8_t op_num, SYN_op_config_t *op_cfg);
    void setOpConfig(uint8_t op_num, SYN_op_config_t *op_cfg);
    
  private:
    TFT_envelope env1 = TFT_envelope(260, 36, "OP1");
    TFT_envelope env2 = TFT_envelope(260, 122, "OP2");
    TFT_env_config_t _env_cfg;

};

#endif // _TFT_GROUP_OP12_
