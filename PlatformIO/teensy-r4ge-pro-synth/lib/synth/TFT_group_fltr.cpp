#include "TFT_group_fltr.h"

TFT_group_fltr::TFT_group_fltr():TFT_group()
{
    _items[0] = new TFT_select_wave(0, 36, "FLTR");
    _items[1] = new TFT_slider( 35, 36, "CRS");
    _items[2] = new TFT_slider( 55, 36, "FIN");
    _items[3] = new TFT_slider( 75, 36, "LVL");
    _items[4] = new TFT_slider( 95, 36, "CUT");
    _items[5] = new TFT_slider( 115, 36, "RES");
    _items[6] = new TFT_slider( 135, 36, "A1");

    _items[7] = new TFT_checkbox(155, 36, "Test");

    _items[8] = new TFT_select_route(0, 122, "ALGO");

    // TODO: set custom ranges: coarse=0-24, fine=0-99
    
    _item_count = 9;
    _items[0]->setSelected(true);
}

void TFT_group_fltr::getFilterConfig(uint8_t filter_num, SYN_filter_config_t *filter_cfg)
{
    filter_cfg->filter_type = SYN_FLTR_LOPASS;  // TODO!
    filter_cfg->cutoff = scaleFrequency(_items[1]->getValue(), _items[2]->getValue()); // coarse.fine
    filter_cfg->level = scaleLevel(_items[3]->getValue());
    filter_cfg->cutoff = (float)_items[4]->getValue();
    filter_cfg->resonance = (float)_items[5]->getValue();
    filter_cfg->param1 = scaleLevel(_items[6]->getValue());
    filter_cfg->active = true;
}

void TFT_group_fltr::setFilterConfig(uint8_t filter_num, SYN_filter_config_t *filter_cfg)
{
    _items[0]->setValue(filter_cfg->filter_type);  
    _items[1]->setValue(unscaleCoarseFrequency(filter_cfg->frequency));
    _items[2]->setValue(unscaleFineFrequency(filter_cfg->frequency));
    _items[3]->setValue(unscaleLevel(filter_cfg->level));
    _items[4]->setValue((int16_t)filter_cfg->cutoff);
    _items[5]->setValue((int16_t)filter_cfg->resonance);
    _items[6]->setValue(unscaleLevel(filter_cfg->param1));
    // active??
}

void TFT_group_fltr::getGlobalConfig(SYN_global_config_t *global_cfg)
{
    global_cfg->route = (SYN_route_type)_items[8]->getValue();
}

void TFT_group_fltr::setGlobalConfig(SYN_global_config_t *global_cfg)
{
     _items[8]->setValue(global_cfg->route);
}
    