#ifndef _TFT_GROUP_OP34_
#define _TFT_GROUP_OP34_

#include "TFT_group.h"

enum TFT_op34_type
{
  TFT_OP3_OSC3 = 0,
  TFT_OP3_CRS = 1,
  TFT_OP3_FIN = 2,
  TFT_OP3_LVL = 3,
  TFT_OP3_ATK_LVL = 4,
  TFT_OP3_DEC_LVL = 5,
  TFT_OP3_SUS_LVL = 6,
  TFT_OP3_REL_LVL = 7,
  TFT_OP3_ATK_DUR = 8,
  TFT_OP3_DEC_DUR = 9,
  TFT_OP3_SUS_DUR = 10,
  TFT_OP3_REL_DUR = 11,
  TFT_OP3_ENV = 12,

  TFT_OP4_OSC4 = 13,
  TFT_OP4_CRS = 14,
  TFT_OP4_FIN = 15,
  TFT_OP4_LVL = 16,
  TFT_OP4_ATK_LVL = 17,
  TFT_OP4_DEC_LVL = 18,
  TFT_OP4_SUS_LVL = 19,
  TFT_OP4_REL_LVL = 20,
  TFT_OP4_ATK_DUR = 21,
  TFT_OP4_DEC_DUR = 22,
  TFT_OP4_SUS_DUR = 23,
  TFT_OP4_REL_DUR = 24,
  TFT_OP4_ENV = 25
};

class TFT_group_op34 : public TFT_group
{
  public:
    TFT_group_op34();
    void updateEnvelopes();
    void getOpConfig(uint8_t op_num, SYN_op_config_t *op_cfg);
    void setOpConfig(uint8_t op_num, SYN_op_config_t *op_cfg);
    
  private:
    TFT_envelope env3 = TFT_envelope(260, 36, "OP3");
    TFT_envelope env4 = TFT_envelope(260, 122, "OP4");
    TFT_env_config_t _env_cfg;
};

#endif // _TFT_GROUP_OP34_
