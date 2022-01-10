/***************************************************
Teensy R4ge Pro graphics test app

Requires:
 - Teensy R4ge Pro
 - Teensy 4.1 board
 - The ILI9341_t3n library from KurtE:  https://github.com/KurtE/ILI9341_t3n
 
Copyright (c) 2021 Paul Pagel
This is free software; see the license.txt file for more information.
There is no warranty; not even for merchantability or fitness for a particular purpose.
*****************************************************/

#include "SD.h"
#include <SPI.h>
#include <ILI9341_t3n.h>
#include <Audio.h>
#include "teensy_r4ge_pro.h"
#include "ground.h"
#include "starship.h"
#include "rot_trunk.h"
#include "wav_hihat.h"

bool btnA_pressed, btnB_pressed, btnX_pressed, btnY_pressed;
bool btnUp_pressed, btnDown_pressed, btnLeft_pressed, btnRight_pressed;
bool spkrLeft_on, spkrRight_on;
bool btnTouch_pressed, btnTouch_released;
bool joy_pressed;
int16_t joy_x, joy_y; 

int offset_x, offset_y;
int ship_x = 140;
int ship_y = 100;

struct rotator_type {
  int x;
  int y;
  int ctrx;
  int ctry;
  int r;
  int w;
  int h;
  float angle;
  uint16_t color;
};

#define ROT_CNT 4
struct rotator_type rotators[ROT_CNT] = {
  { 0, 0, 144, 104, 70, 32, 32, 0.0, ILI9341_RED},
  { 0, 0, 144, 104, 70, 32, 32, 1.6, ILI9341_BLUE},
  { 0, 0, 144, 104, 70, 32, 32, 3.1, ILI9341_YELLOW},
  { 0, 0, 144, 104, 70, 32, 32, 4.7, ILI9341_GREEN}
};
int r_chg = 0;
int r_dir = -1;

ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST);

#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  43  // not actually used
#define SDCARD_SCK_PIN   45  // not actually used

AudioPlayMemory    sound1;
AudioMixer4        mix1;    // two 4-channel mixers are needed in
AudioMixer4        mix2;    // tandem to combine 6 audio sources
AudioOutputI2S     headphones;
AudioConnection c8(mix1, 0, headphones, 0);
AudioConnection c9(mix1, 0, headphones, 1);
AudioConnection c1(sound1, 0, mix1, 0);
AudioControlSGTL5000 audioShield;

void updateScreen();
void checkButtonPresses();
void checkJoysticks();

/*
 * Set up the board
 */
void setup() 
{
  Serial.begin(9600);
  Serial.println("Teensy R4ge Pro Graphics Test"); 
  delay(100);

  // Set up the buttons
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_X, INPUT_PULLUP);
  pinMode(BTN_Y, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  
  // Set up the joystick
  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(JBTN, INPUT_PULLUP);

  // Set up the rotary encoder knob
  pinMode(ENC_1, INPUT);
  pinMode(ENC_2, INPUT);
  pinMode(ENC_BTN, INPUT_PULLUP);
  delay(100);

  // Set up the TFT and touch screen
  tft.begin();
  tft.setRotation(SCREEN_ROT); 

  // Start main screen display
  tft.fillScreen(ILI9341_BLACK);

  AudioMemory(10);
  audioShield.enable();
  audioShield.volume(0.5);
  //sound1.play(wav_hihat);
  
   
  Serial.println("done."); 
}

/*
 * Extends the writeRect function by allowing a transparent color to be specified.
 */
void writeRectEx(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors, uint16_t transcolor)
{
  uint16_t color, idx;

  idx = 0;
  for (int16_t y1 = 0; y1 < h; y1++)
  {
    for (int16_t x1 = 0; x1 < w; x1++)
    {
      color = pcolors[idx++];
      if (color != transcolor)
      {
        tft.drawPixel(x + x1, y + y1, color);
      }
    }
  }
}

/*
 * Extends the writeRect function by allowing a transparent color to be specified.
 */
void replaceRectEx(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors, uint16_t transcolor, uint16_t replacecolor)
{
  uint16_t color, idx;

  idx = 0;
  for (int16_t y1 = 0; y1 < h; y1++)
  {
    for (int16_t x1 = 0; x1 < w; x1++)
    {
      color = pcolors[idx++];
      if (color != transcolor)
      {
        tft.drawPixel(x + x1, y + y1, replacecolor);
      }
    }
  }
}

void updateRotators()
{

  
  for (int i = 0; i < ROT_CNT; i++)
  {
    rotators[i].angle = rotators[i].angle + .1;

    rotators[i].x = rotators[i].ctrx + (cos(rotators[i].angle) * (r_chg + rotators[i].r));
    rotators[i].y = rotators[i].ctry + (sin(rotators[i].angle) * (r_chg + rotators[i].r));

    tft.fillRect(rotators[i].x + 4, rotators[i].y + 4, rotators[i].w, rotators[i].h, ILI9341_BLACK); // shadow
    tft.fillRect(rotators[i].x, rotators[i].y, rotators[i].w, rotators[i].h, rotators[i].color);

    if (rotators[i].angle > TWO_PI)
    {
      rotators[i].angle -= TWO_PI;
    }
  }

  r_chg += r_dir;
  if (r_chg < -40) r_dir = 1;
  if (r_chg > 10) r_dir = -1;
  
}

/*
 * Updates the screen based on which buttons are being pressed
 */
void updateScreen()
{
  int col, row;

  tft.useFrameBuffer(1);
  
  // Top row
  for (col = 0; col < 320; col+=32)
  {
    tft.writeRect(col, 0, 32, 32 - offset_y, (uint16_t*)(ground + offset_y * 32));  
  }

  // Middle rows
  for (row = 32; row < 208; row+=32)
  {
    for (int col = 0; col < 320; col+=32)
    {
      tft.writeRect(col, row - offset_y, 32, 32, (uint16_t*)ground);  
    }
  }

  // Bottom row
  for (col = 0; col < 320; col+=32)
  {
    tft.writeRect(col, 224 - offset_y, 32, offset_y, (uint16_t*)ground);  
  } 

  updateRotators();

  replaceRectEx(ship_x + 6, ship_y + 6, 54, 63, (uint16_t*)starship, ILI9341_BLACK, ILI9341_BLACK); // shadow effect
  writeRectEx(ship_x, ship_y, 54, 63, (uint16_t*)starship, ILI9341_BLACK);

  tft.updateScreen();

  offset_y -= 2;
  if (offset_y <= 0)
  {
    offset_y = 32;
  }


  /*
  offset_y += 2;
  if (offset_y >= 32)
  {
    offset_y = 0;
  }
  */
}

/*
 * Check the status of the control buttons.
 */
void checkButtonPresses()
{
  btnA_pressed = (digitalRead(BTN_A) == LOW ? 1: 0);
  btnB_pressed = (digitalRead(BTN_B) == LOW ? 1: 0);
  btnX_pressed = (digitalRead(BTN_X) == LOW ? 1: 0);
  btnY_pressed = (digitalRead(BTN_Y) == LOW ? 1: 0);
  
  btnUp_pressed = (digitalRead(BTN_UP) == LOW ? 1: 0);
  btnDown_pressed = (digitalRead(BTN_DOWN) == LOW ? 1: 0);
  btnLeft_pressed = (digitalRead(BTN_LEFT) == LOW ? 1: 0);
  btnRight_pressed = (digitalRead(BTN_RIGHT) == LOW ? 1: 0);
}

/*
 * Check the analog joystick values
 */
void checkJoystick()
{
  // Only use the left 10 bits of the joystick pot readings
  // May want to use analogReadResolution() at some point to eliminate the need for shifts
  joy_x = analogRead(JOYX) >> 2;
  joy_y = analogRead(JOYY) >> 2;
  joy_pressed = (digitalRead(JBTN) == LOW ? 1: 0);
}

/*
 * Main program loop.  Called continuously after setup.
 */
void loop(void) 
{
  checkButtonPresses();
  checkJoystick();

  if (btnA_pressed)
  {
    sound1.play(wav_hihat);
  }

  if (btnLeft_pressed || joy_x < 55)
  {
    ship_x -= 4;
  }

  if (btnRight_pressed || joy_x > 200)
  {
    ship_x += 4;
  }

  if (btnUp_pressed || joy_y > 200)
  {
    ship_y -= 4;
  }

  if (btnDown_pressed || joy_y < 55)
  {
    ship_y += 4;
  }
  
  updateScreen();
  //playWav1.isPlaying();
}
