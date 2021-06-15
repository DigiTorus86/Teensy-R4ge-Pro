#include "TFT_envelope.h"

TFT_envelope::TFT_envelope(int16_t x, int16_t y, const char* label):TFT_control(x, y, label)
{
    _x = x;
    _y = y;
    _label = label;
    _selected = 0;

    _ht = TFT_ENV_DEFAULT_HT;
    _wd = TFT_ENV_DEFAULT_WD;

    _env_zero_x = _x + 4;
    _env_max_x = _x + _wd - 7;
    _env_zero_y = _y + _ht - 8;  // TODO: better calc
    //_env_max_y = _y + 32;

    _scale_x = ((float)TFT_ENV_DEFAULT_WD - 6) / 400; // ADSR each 100 max
    _scale_y = ((float)TFT_ENV_DEFAULT_HT - 6) / 200;  // window 1/2 ht

    calcXY();
}

void TFT_envelope::draw(ILI9341_t3 *tft, bool force_redraw)
{
    if (!_changed && !force_redraw) return;

    tft->fillRect(_x, _y, _wd, _ht, ILI9341_DARKGREY);  // background
    tft->fillRoundRect(_x + 2, _y + 28, _wd - 4, _ht / 2, 4, ILI9341_BLACK); // env window

    tft->setTextSize(1);
    tft->setTextColor(ILI9341_WHITE);
    tft->setCursor(_x + 18, _y + 4);
    tft->print("ADSR");
    tft->setCursor(_x + 6, _y + 14);
    tft->print("ENVELOPE");
    
    // Draw the actual envelope ADSR contour
    tft->drawLine(_env_zero_x, _env_zero_y, _atk_x, _atk_y, ILI9341_GREEN);  // ATK
    tft->drawLine(_atk_x, _atk_y, _dec_x, _dec_y, ILI9341_GREEN);   // DEC
    tft->drawLine(_dec_x, _dec_y, _sus_x, _sus_y, ILI9341_GREEN);   // SUS
    tft->drawLine(_sus_x, _sus_y, _rel_x, _rel_y, ILI9341_GREEN);   // REL

    /*
    Serial.print("ADSR lvl: "); Serial.print(_atk_lvl); Serial.print(", "); Serial.print(_dec_lvl); Serial.print(", ");
                                Serial.print(_sus_lvl); Serial.print(", "); Serial.println(_rel_lvl);

    Serial.print("ADSR dur: "); Serial.print(_atk_dur); Serial.print(", "); Serial.print(_dec_dur); Serial.print(", ");
                                Serial.print(_sus_dur); Serial.print(", "); Serial.println(_rel_dur);

    Serial.print("scale x,y: "); Serial.print(_scale_x); Serial.print(", "); Serial.println(_scale_y);

    Serial.print("atk x,y: "); Serial.print(_atk_x); Serial.print(", "); Serial.println(_atk_y);
    Serial.print("dec x,y: "); Serial.print(_dec_x); Serial.print(", "); Serial.println(_dec_y); 
    Serial.print("sus x,y: "); Serial.print(_sus_x); Serial.print(", "); Serial.println(_sus_y); 
    Serial.print("rel x,y: "); Serial.print(_rel_x); Serial.print(", "); Serial.println(_rel_y);  
    Serial.println("--------");
    */
    _changed = 0;  // internal state and display are now in sync
}

bool TFT_envelope::handleButtons(bool up_pressed, bool down_pressed)
{
    return 0;
}

bool TFT_envelope::handleTouch(int16_t x, int16_t y)
{
    if (x >= _x && x < _x + _wd && y >= _y && y < _y + _ht)
    {
        // TODO: change anything??
        return 1;
    }

    return 0;
}

void TFT_envelope::setConfig(TFT_env_config_t *env_cfg)
{
    _atk_lvl = env_cfg->atk_lvl;
    _dec_lvl = env_cfg->dec_lvl;
    _sus_lvl = env_cfg->sus_lvl;
    _rel_lvl = env_cfg->rel_lvl;

    _atk_dur = env_cfg->atk_dur;
    _dec_dur = env_cfg->dec_dur;
    _sus_dur = env_cfg->sus_dur;
    _rel_dur = env_cfg->rel_dur;

    calcXY();

    _changed = true;
}

void TFT_envelope::calcXY()
{
    _atk_x = _env_zero_x + ((float)_atk_dur * _scale_x);
    _dec_x = _atk_x + ((float)_dec_dur * _scale_x);
    _sus_x = _dec_x + ((float)_sus_dur * _scale_x);
    _rel_x = _sus_x + ((float)_rel_dur * _scale_x);

    _atk_y = _env_zero_y - ((float)_atk_lvl * _scale_y);
    _dec_y = _env_zero_y - ((float)_dec_lvl * _scale_y);
    _sus_y = _env_zero_y - ((float)_sus_lvl * _scale_y);
    _rel_y = _env_zero_y - ((float)_rel_lvl * _scale_y);
}