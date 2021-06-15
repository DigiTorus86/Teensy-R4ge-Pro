#include "TFT_group.h"

TFT_group::TFT_group()
{
}

void TFT_group::draw(ILI9341_t3 *tft, bool force_redraw)
{
    for (int i = 0; i < _item_count; i++)
    {
        _items[i]->draw(tft, force_redraw);
    }
}

void TFT_group::addItem()
{

}

bool TFT_group::handleTouch(int16_t x, int16_t y)
{
    bool handled = false;

    if (x <= 0 && y <= 0) return false; 

    for (uint8_t i = 0; i < _item_count; i++)
    {
        handled = _items[i]->handleTouch(x, y);
        if (handled)
        {
            // This control is now the selected one (if it isn't already)
            setSelected(i);
            break;  // no need for further processing
        }
    }
    return handled;
}

bool TFT_group::handleButtons(bool up_pressed, bool down_pressed, bool left_pressed, bool right_pressed)
{
    if (left_pressed)
    {
        _items[_selected_idx]->setSelected(false);
        _selected_idx = _selected_idx > 0 ? _selected_idx - 1  : _item_count - 1;
        _items[_selected_idx]->setSelected(true);
    }

    if (right_pressed)
    {
        _items[_selected_idx]->setSelected(false);
        _selected_idx = _selected_idx < _item_count - 1 ? _selected_idx + 1  : 0;
        _items[_selected_idx]->setSelected(true);
    }

    if (!up_pressed && !down_pressed) return 0;

    for (uint8_t i = 0; i < _item_count; i++)
    {
        if (_items[i]->getSelected())
        {
            return _items[i]->handleButtons(up_pressed, down_pressed);
        }
    }
    return 0;
}

bool TFT_group::getChanged()
{
    for (uint8_t i = 0; i < _item_count; i++)
    {
        if (_items[i]->getChanged())
        {
            return 1;
        }
    }
    return 0;
}

uint8_t TFT_group::getSelected()
{
    return _selected_idx;
}

void TFT_group::setSelected(uint8_t selected_idx)
{
    if (selected_idx > _item_count - 1) return; // bounds check

    _items[_selected_idx]->setSelected(0);  // de-select current control
    _selected_idx = selected_idx;           // select specified control
    _items[_selected_idx]->setSelected(true);
}

/**
 * @brief Takes the 0-100 Oscillator coarse and fine values and converts them to a Hz frequency.
 * 
 * @param coarse_lvl The oscillator Coarse control value (0-100)
 * @param fine_lvl   The oscillator Fine control value (0-100)
 * @return float     The result frequency in Hz.
 */
float TFT_group::scaleFrequency(int16_t coarse_lvl, int16_t fine_lvl)
{
    return (float)coarse_lvl + ((float)fine_lvl / 100); 
}

/**
 * @brief Takes the 0-100 Phase control value and converts it to a 4096 element wavetable offset.
 * 
 * @param phase  The oscillator phase control value (0-100)
 * @return float The resulting wavetable offset index.
 */
float TFT_group::scalePhase(int16_t phase)
{
    return phase * 40; 
}

/**
 * @brief Scales the 0-100 Envelope level range and converts it to a percentage.
 * 
 * @param control_lvl The Envelope control value (0-100)
 * @return float The envelope level as a percentage (0.0 - 1.0)
 */
float TFT_group::scaleLevel(int16_t control_lvl)
{
    return (float)control_lvl / 100;
}

/**
 * @brief Takes the 0-100 Envelope range and converts it to milliseconds.
 * 
 * @param control_dur The Envelope control value (0-100)
 * @return float The envelope duration in milliseconds.
 */
float TFT_group::scaleDuration(int16_t control_dur)
{
    if (control_dur < 50)
        return (float)control_dur;

    if (control_dur < 75)
        return (float)control_dur * 8;

    return (float)control_dur * 16;
}


int16_t TFT_group::unscaleCoarseFrequency(float frequency)
{
    return (int16_t)frequency;
}

int16_t TFT_group::unscaleFineFrequency(float frequency)
{
    double integral, fractional;
    fractional = modf(frequency, &integral);
    return (float)fractional;

    //int16_t coarse = unscaleCoarseFrequency(frequency);
    //return (int16_t)(frequency - coarse);
}

int16_t TFT_group::unscalePhase(float phase_offset)
{
    return (int16_t)(phase_offset / 40);
}

int16_t TFT_group::unscaleLevel(float config_lvl)
{
    return (int16_t)(config_lvl * 100);
}

int16_t TFT_group::unscaleDuration(float config_dur)
{
    if (config_dur < 50)
        return (int16_t)config_dur;

    if (config_dur <600)
        return (float)config_dur / 8;

    return (float)config_dur / 16;
}

float TFT_group::scaleTempo(int16_t tempo)
{
    return (float)(tempo * 10);  // To milliseconds
}

int16_t TFT_group::unscaleTempo(float tempo)
{
    return (int16_t)(tempo / 10);  // From milliseconds
}
