#include "TFT_group_op34.h"

TFT_group_op34::TFT_group_op34():TFT_group()
{
    _items[0] = new TFT_select_wave(0, 36, "OSC3");
    _items[1] = new TFT_slider( 35, 36, "CRS");
    _items[2] = new TFT_slider( 55, 36, "FIN");
    _items[3] = new TFT_slider( 75, 36, "PHA");
    _items[4] = new TFT_slider( 95, 36, "LVL");

    _items[5] = new TFT_slider(117, 36, "ATK");
    _items[6] = new TFT_slider(137, 36, "DEC");
    _items[7] = new TFT_slider(157, 36, "SUS");
    _items[8] = new TFT_slider(177, 36, "REL");

    _items[9] = new TFT_slider(199, 36, "atk");
    _items[10] = new TFT_slider(219, 36, "dec");
    //_items[10] = new TFT_slider(219, 36, "sus");
    _items[11] = new TFT_slider(239, 36, "rel");

    _items[12] = &env3; //new TFT_envelope(260, 36, "OP3");

    _items[13] = new TFT_select_wave(0, 122, "OSC4");
    _items[14] = new TFT_slider( 35, 122, "CRS");
    _items[15] = new TFT_slider( 55, 122, "FIN");
    _items[16] = new TFT_slider( 75, 122, "PHA");
    _items[17] = new TFT_slider( 95, 122, "LVL");

    _items[18] = new TFT_slider(117, 122, "ATK");
    _items[19] = new TFT_slider(137, 122, "DEC");
    _items[20] = new TFT_slider(157, 122, "SUS");
    _items[21] = new TFT_slider(177, 122, "REL");

    _items[22] = new TFT_slider(199, 122, "atk");
    _items[23] = new TFT_slider(219, 122, "dec");
    //_items[23] = new TFT_slider(219, 122, "sus");
    _items[24] = new TFT_slider(239, 122, "rel");

    _items[25] = &env4; //new TFT_envelope(260, 122, "OP4");
    _items[26] = new TFT_checkbox(260, 190, "Fixed");

    // Set custom ranges and starting values for controls
    _items[1]->setValue(1); // Op3 Coarse=1
    _items[4]->setValue(100); // Op3 Level

    _items[14]->setValue(1); // Op4 Coarse=1

    _item_count = 27;
    _items[0]->setSelected(true);
}

void TFT_group_op34::updateEnvelopes()
{
    if (getChanged())
    {
        _env_cfg = {
            .atk_lvl = _items[5]->getValue(),
            .dec_lvl = _items[6]->getValue(),
            .sus_lvl = _items[7]->getValue(),
            .rel_lvl = _items[8]->getValue(),

            .atk_dur = _items[9]->getValue(),
            .dec_dur = _items[10]->getValue(),
            .sus_dur = 100, //_items[10]->getValue(),
            .rel_dur = _items[11]->getValue()
        };
        
        env3.setConfig(&_env_cfg);

        _env_cfg = {
            .atk_lvl = _items[18]->getValue(),
            .dec_lvl = _items[19]->getValue(),
            .sus_lvl = _items[20]->getValue(),
            .rel_lvl = _items[21]->getValue(),

            .atk_dur = _items[22]->getValue(),
            .dec_dur = _items[23]->getValue(),
            .sus_dur = 100, //_items[23]->getValue(),
            .rel_dur = _items[24]->getValue()
        };
        
        env4.setConfig(&_env_cfg);
    }
}

void TFT_group_op34::getOpConfig(uint8_t op_num, SYN_op_config_t *op_cfg)
{
    if (op_num == 3)
    {
        op_cfg->osc_wave = (SYN_wave_type)_items[0]->getValue();
        op_cfg->osc_freq = scaleFrequency(_items[1]->getValue(), _items[2]->getValue()); // coarse.fine
        op_cfg->osc_phase = scalePhase(_items[3]->getValue()); 
        op_cfg->osc_lvl = scaleLevel(_items[4]->getValue()); 
        op_cfg->osc_fixed = false;

        op_cfg->atk_lvl = scaleLevel(_items[5]->getValue());
        op_cfg->dec_lvl = scaleLevel(_items[6]->getValue());
        op_cfg->sus_lvl = scaleLevel(_items[7]->getValue());
        op_cfg->rel_lvl = scaleLevel(_items[8]->getValue());

        op_cfg->atk_dur = scaleDuration(_items[9]->getValue());
        op_cfg->dec_dur = scaleDuration(_items[10]->getValue());
        op_cfg->sus_dur = 100; //scaleDuration(_items[10]->getValue());
        op_cfg->rel_dur = scaleDuration(_items[11]->getValue());
    }

    if (op_num == 4)
    {   
        op_cfg->osc_wave = (SYN_wave_type)_items[13]->getValue();
        op_cfg->osc_freq = scaleFrequency(_items[14]->getValue(), _items[15]->getValue()); // coarse.fine
        op_cfg->osc_phase = scalePhase(_items[16]->getValue()); 
        op_cfg->osc_lvl = scaleLevel(_items[17]->getValue()); 
        op_cfg->osc_fixed = (bool)_items[26]->getValue();

        op_cfg->atk_lvl = scaleLevel(_items[18]->getValue());
        op_cfg->dec_lvl = scaleLevel(_items[19]->getValue());
        op_cfg->sus_lvl = scaleLevel(_items[20]->getValue());
        op_cfg->rel_lvl = scaleLevel(_items[21]->getValue());

        op_cfg->atk_dur = scaleDuration(_items[22]->getValue());
        op_cfg->dec_dur = scaleDuration(_items[23]->getValue());
        op_cfg->sus_dur = 100; //scaleDuration(_items[23]->getValue());
        op_cfg->rel_dur = scaleDuration(_items[24]->getValue());
    }
}

void TFT_group_op34::setOpConfig(uint8_t op_num, SYN_op_config_t *op_cfg)
{
    if (op_num == 3)
    {
        _items[0]->setValue(op_cfg->osc_wave);
        _items[1]->setValue(unscaleCoarseFrequency(op_cfg->osc_freq));
        _items[2]->setValue(unscaleFineFrequency(op_cfg->osc_freq));
        _items[3]->setValue(unscalePhase(op_cfg->osc_phase));
        _items[4]->setValue(unscaleLevel(op_cfg->osc_lvl));

        _items[5]->setValue(unscaleLevel(op_cfg->atk_lvl));
        _items[6]->setValue(unscaleLevel(op_cfg->dec_lvl));
        _items[7]->setValue(unscaleLevel(op_cfg->sus_lvl));
        _items[8]->setValue(unscaleLevel(op_cfg->rel_lvl));

        _items[9]->setValue(unscaleDuration(op_cfg->atk_dur));
        _items[10]->setValue(unscaleDuration(op_cfg->dec_dur));
        _items[11]->setValue(unscaleDuration(op_cfg->rel_dur));
    }

    if (op_num == 4)
    {   
        _items[13]->setValue(op_cfg->osc_wave);
        _items[14]->setValue(unscaleCoarseFrequency(op_cfg->osc_freq));
        _items[15]->setValue(unscaleFineFrequency(op_cfg->osc_freq));
        _items[16]->setValue(unscalePhase(op_cfg->osc_phase));
        _items[17]->setValue(unscaleLevel(op_cfg->osc_lvl));

        _items[18]->setValue(unscaleLevel(op_cfg->atk_lvl));
        _items[19]->setValue(unscaleLevel(op_cfg->dec_lvl));
        _items[20]->setValue(unscaleLevel(op_cfg->sus_lvl));
        _items[21]->setValue(unscaleLevel(op_cfg->rel_lvl));

        _items[22]->setValue(unscaleDuration(op_cfg->atk_dur));
        _items[23]->setValue(unscaleDuration(op_cfg->dec_dur));
        _items[24]->setValue(unscaleDuration(op_cfg->rel_dur));

        _items[26]->setValue((uint16_t)op_cfg->osc_fixed);
    }
    updateEnvelopes();
}