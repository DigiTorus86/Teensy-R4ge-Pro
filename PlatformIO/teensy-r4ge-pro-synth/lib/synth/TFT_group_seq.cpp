#include "TFT_group_seq.h"

TFT_group_seq::TFT_group_seq():TFT_group()
{
    _items[0] = new TFT_select_note(  0, 36, "1");
    _items[1] = new TFT_select_note( 35, 36, "2");
    _items[2] = new TFT_select_note( 70, 36, "3");
    _items[3] = new TFT_select_note(105, 36, "4");
    _items[4] = new TFT_select_note(140, 36, "5");
    _items[5] = new TFT_select_note(175, 36, "6");
    _items[6] = new TFT_select_note(210, 36, "7");
    _items[7] = new TFT_select_note(245, 36, "8");

    _items[8] = new TFT_slider(280, 36, "TMPO");
    _items[8]->setValue(25); // 25 x 10 = 250ms per note

    _item_count = 9;
    _items[0]->setSelected(true);
}

void TFT_group_seq::getSeqConfig(SYN_sequence_config_t *seq_cfg)
{
    for (uint8_t i = 0; i < SYN_SEQ_NOTE_COUNT; i++)
    {
        seq_cfg->note_idx[i] = _items[i]->getValue();
    }

    seq_cfg->tempo = scaleTempo(_items[8]->getValue()); // tempo slider
}

void TFT_group_seq::setSeqConfig(SYN_sequence_config_t *seq_cfg)
{
    for (uint8_t i = 0; i < SYN_SEQ_NOTE_COUNT; i++)
    {
        _items[i]->setValue(seq_cfg->note_idx[i]);
    }

    _items[8]->setValue(unscaleTempo(seq_cfg->tempo));
}
