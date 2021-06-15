#include "TFT_waveform.h"

TFT_waveform::TFT_waveform(int16_t x, int16_t y, int16_t wd, int16_t ht)
{
    _x = x;
    _y = y;
    _wd = wd;
    _ht = ht;

    _half_wd = _wd / 2;
    _zero_y = _y + _ht - 1;
    _ctr_y = _y + (_ht / 2);

    _wave_color = TFT_WAV_DEFAULT_COLOR;
    _bgd_color = TFT_WAV_DEFAULT_BGD_COLOR;
}

void TFT_waveform::draw(ILI9341_t3 *tft, SYN_wave_type wave_type)
{
    switch (wave_type)
    {
        case SYN_WAVE_SINE:
            drawSine(tft);
            break;
        case SYN_WAVE_SQUARE:
            drawSquare(tft);
            break;
        case SYN_WAVE_TRIANGLE:
            drawTriangle(tft);
            break;
        case SYN_WAVE_SAW:
            drawSaw(tft);
            break;
        case SYN_WAVE_RAMP:
            drawRamp(tft);
            break;
        case SYN_WAVE_MAJOR:
            drawMajor(tft);
            break;
        case SYN_WAVE_MINOR:
            drawMinor(tft);
            break;
        case SYN_WAVE_OCT3:
            drawOct3(tft);
            break;
        case SYN_WAVE_CUSTOM:
            drawCustom(tft);
            break;
        default:
            drawSilence(tft);
            break; 
    }
}

void TFT_waveform::drawName(ILI9341_t3 *tft, SYN_wave_type wave_type)
{
    //tft->setTextSize(1);
    //tft->setTextColor(ILI9341_WHITE);
    //tft->setCursor(x, y);

    switch (wave_type)
    {
        case SYN_WAVE_SINE:
            tft->print("SINE");
            break;
        case SYN_WAVE_SQUARE:
            tft->print("SQR ");
            break;
        case SYN_WAVE_TRIANGLE:
            tft->print("TRI ");
            break;
        case SYN_WAVE_SAW:
            tft->print("SAW ");
            break;
        case SYN_WAVE_RAMP:
            tft->print("RAMP");
            break;
        case SYN_WAVE_MAJOR:
            tft->print("MAJR");
            break;
        case SYN_WAVE_MINOR:
            tft->print("MINR");
            break;
        case SYN_WAVE_OCT3:
            tft->print("OCT3");
            break;
        case SYN_WAVE_CUSTOM:
            tft->print("CUST");
            break;
        default:
            tft->print("----");
            break; 
    }
}

void TFT_waveform::setColor(uint16_t wave_color)
{
    _wave_color = wave_color;
}

void TFT_waveform::setColor(uint16_t wave_color, uint16_t bgd_color)
{
    _wave_color = wave_color;
    _bgd_color = bgd_color;
}

//----- PRIVATE METHODS -----//

/**
 * @brief Draw a silent (empty) waveform
 */
void TFT_waveform::drawSilence(ILI9341_t3 *tft)
{
    tft->drawFastHLine(_x, _ctr_y, _wd, _wave_color);
}

/**
 * @brief Fills the oscillator wave table with a single sine wave with amplitude of -1.0 to 1.0.
 */
void TFT_waveform::drawSine(ILI9341_t3 *tft)
{
    double two_pi_scaled = 2 * PI / _wd;
    float sample;
    int16_t y = 0;

    for (int16_t i = 0; i < _wd; i++)
    {
        sample = _ht * (float)sin((double)i * two_pi_scaled) / 2;
        y = _ctr_y + (int16_t)sample;
        tft->drawPixel(_x + i, y, _wave_color);
    }
}

void TFT_waveform::drawSquare(ILI9341_t3 *tft)
{
    tft->drawFastVLine(_x, _y, _ht, _wave_color);  // up
    tft->drawFastHLine(_x, _y, _half_wd, _wave_color);      // top
    tft->drawFastVLine(_x + _half_wd, _y, _ht, _wave_color); // down
    tft->drawFastHLine(_x + _half_wd, _zero_y, _half_wd, _wave_color);  // bottom
}

void TFT_waveform::drawTriangle(ILI9341_t3 *tft)
{
    tft->drawLine(_x, _zero_y, _half_wd, _y, _wave_color);  // ramp up
    tft->drawLine(_half_wd, _y, _x + _wd - 1, _zero_y, _wave_color); // ramp down
}

void TFT_waveform::drawSaw(ILI9341_t3 *tft)
{
    tft->drawFastVLine(_x, _y, _ht, _wave_color);  // up
    tft->drawLine(_x, _y, _x + _wd - 1, _zero_y, _wave_color);  // ramp down
}

void TFT_waveform::drawRamp(ILI9341_t3 *tft)
{
    tft->drawLine(_x, _zero_y, _x + _wd - 1, _y, _wave_color);  // ramp up
    tft->drawFastVLine(_x + _wd - 1, _y, _ht, _wave_color);  // down
}

void TFT_waveform::drawMajor(ILI9341_t3 *tft)
{
    tft->setTextSize(2);
    tft->setTextColor(_wave_color);
    tft->setCursor((_x + _wd / 2) - 6, _ctr_y - 6); 
    tft->print("M");
}

void TFT_waveform::drawMinor(ILI9341_t3 *tft)
{
    tft->setTextSize(2);
    tft->setTextColor(_wave_color);
    tft->setCursor((_x + _wd / 2) - 6, _ctr_y - 6); 
    tft->print("m");
}

void TFT_waveform::drawOct3(ILI9341_t3 *tft)
{
    tft->setTextSize(2);
    tft->setTextColor(_wave_color);
    tft->setCursor((_x + _wd / 2) - 6, _ctr_y - 6); 
    tft->print("3"); // TODO: better symbol
}

void TFT_waveform::drawCustom(ILI9341_t3 *tft)
{
    tft->setTextSize(2);
    tft->setTextColor(_wave_color);
    tft->setCursor((_x + _wd / 2) - 6, _ctr_y - 6); 
    tft->print("C"); // TODO: better symbol
}
