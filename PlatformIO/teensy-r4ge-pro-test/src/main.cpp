/***************************************************
Teensy R4ge Pro test app to verify hardware functionality

Requires:
 - Teensy R4ge Pro v2
 - Teensy 4.1 board

 Optional: 
 - Audio source and 3.5mm phone cable to test audio in features 
 
Copyright (c) 2021 Paul Pagel
This is free software; see the license.txt file for more information.
There is no warranty; not even for merchantability or fitness for a particular purpose.
*****************************************************/

#include <SPI.h>
#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
#include <SD.h> 
#include <Audio.h>
#include <Encoder.h>
#include "teensy_r4ge_pro.h"
#include "icons.h"  
#include "r4ge_pro_title.h"

void drawSD(bool present);

// Touch screen coordinates for the touch button
#define TOUCH_X1    3333
#define TOUCH_Y1    1000
#define TOUCH_X2    3700
#define TOUCH_Y2    1888

#define TOP_LINE      35
#define BOTTOM_LINE  180  

bool btn_pressed[8], btn_released[8];
bool btnA_pressed, btnB_pressed, btnX_pressed, btnY_pressed;
bool btnUp_pressed, btnDown_pressed, btnLeft_pressed, btnRight_pressed;
bool spkrLeft_on, spkrRight_on, audioIn_active;
bool btnTouch_pressed, btnTouch_released;
bool joy_left_pressed, knob_pressed;
int16_t joy_left_x, joy_left_y, joy_right_x, joy_right_y; 
int32_t knob_value = -9999;

XPT2046_Touchscreen ts(TCH_CS);
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST);
Encoder knob(ENC_1, ENC_2);

Sd2Card card;
SdVolume volume;
SdFile root;


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=288.23333740234375,238.2333526611328
AudioSynthWaveform       waveform2;      //xy=296.23333740234375,294.23333740234375
AudioSynthWaveform       waveform1;      //xy=299.23333740234375,173.23333740234375
AudioMixer4              mixer2;         //xy=454.23333740234375,283.23333740234375
AudioMixer4              mixer1;         //xy=455.23333740234375,192.23333740234375
AudioAnalyzePeak         peak1;          //xy=605.2333374023438,179.23333740234375
AudioOutputI2S           i2s2;           //xy=606.2333374023438,238.23333740234375
AudioAnalyzePeak         peak2;          //xy=606.2333374023438,293.23333740234375
AudioConnection          patchCord1(i2s1, 0, mixer1, 1);
AudioConnection          patchCord2(i2s1, 1, mixer2, 1);
AudioConnection          patchCord3(waveform2, 0, mixer2, 0);
AudioConnection          patchCord4(waveform1, 0, mixer1, 0);
AudioConnection          patchCord5(mixer2, 0, i2s2, 1);
AudioConnection          patchCord6(mixer2, peak2);
AudioConnection          patchCord7(mixer1, 0, i2s2, 0);
AudioConnection          patchCord8(mixer1, peak1);
// GUItool: end automatically generated code



bool initSD();
void playAudio(bool play_right, bool play_left);
void drawSD(bool present);
void setTextColor(uint16_t color, bool inverted);
void updateScreen();
void checkButtonPresses();
void checkScreenTouch();
void checkJoystick();
void checkEncoder();

/*
 * Set up the board
 */
void setup() 
{
  Serial.begin(115200);
  delay(200);
  Serial.println("Teensy R4ge Pro Test"); 

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
  pinMode(JOYX_L, INPUT);
  pinMode(JOYY_L, INPUT);
  pinMode(JBTN_L, INPUT_PULLUP);

  // Set up the rotary encoder knob
  pinMode(ENC_1, INPUT);
  pinMode(ENC_2, INPUT);
  pinMode(ENC_BTN, INPUT_PULLUP);

  delay(100);

  // Set up the TFT and touch screen
  tft.begin();
  tft.setRotation(SCREEN_ROT);

  ts.begin();
  ts.setRotation(TCHSCRN_ROT);

  // Read screen diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 

  // Start main screen display
  tft.fillScreen(ILI9341_BLACK);
  tft.writeRect(  0, 0, 175, 32, (uint16_t *)r4ge_pro_title);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  tft.setCursor(205, 4);
  tft.println("TEST");
  tft.setTextSize(2);

  tft.drawLine(0, TOP_LINE, SCREEN_WD, TOP_LINE, ILI9341_BLUE);   // top line
  tft.drawLine(0, BOTTOM_LINE, SCREEN_WD, BOTTOM_LINE, ILI9341_BLUE); // bottom line

  tft.drawRect(250, 50, 70, 70, ILI9341_BLUE);  // touch button outline

  tft.writeRect(0, 196, 16, 16, (uint16_t *)microphone_icon);
  tft.writeRect(0, 220, 16, 16, (uint16_t *)touch_icon);

  bool sd_present = initSD();
  drawSD(sd_present);

  Serial.print(F("Setting up the audio....")); 
  AudioMemory(16);  // allocate memory blocks for the Teensy audio engine
  AudioNoInterrupts();

  mixer1.gain(0, 0.25f); // generally don't want total gain for a mixer to exceed 1.0
  mixer2.gain(0, 0.25f);
  mixer1.gain(1, 0.0f); 
  mixer2.gain(1, 0.0f);

  AudioInterrupts();
  delay(100);
  
  Serial.println("done."); 
}

/*
 * Initialize the SD file system before trying using a reader or writer
 */
bool initSD()
{
  // See SDFat/src/SdCard/SdInfo.h for list of error codes

  digitalWrite(TFT_CS, HIGH); // disable TFT
  digitalWrite(TCH_CS, HIGH); // disable Touchscreen - otherwise SD.begin() will fail!

  Serial.print("Initializing SD card...");

  // We'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, BUILTIN_SDCARD)) 
  {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return false;
  } 
  else 
  {
    Serial.println("Wiring is correct and a card is present.");
  }

  // Print the type of card
  Serial.println();
  Serial.print("Card type:         ");
  switch (card.type()) 
  {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) 
  {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return false;
  }

  Serial.print("Clusters:          ");
  Serial.println(volume.clusterCount());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(volume.blocksPerCluster());

  Serial.print("Total Blocks:      ");
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
 
  return true; // card is present and checks out
}
 
/* 
 * Plays a waveform through either or both speakers.  
 */
void playAudio(bool play_left, bool play_right)
{ 
  static bool left_playing, right_playing;

  if (play_left == left_playing && play_right == right_playing)
  {
    // No status change, so avoid making unneeded audio engine calls
    // which can generate unwanted audio artifacts and noise.
    return; 
  }

  if (play_left)
  {
    waveform1.begin(0.5f, 880, WAVEFORM_SINE);  // 880Hz = A5
  }
  else
  {
    waveform1.amplitude(0); // silence
  }

  if (play_right)
  {
    waveform2.begin(0.5f, 1760, WAVEFORM_SINE);  // 1760Hz = A6
  }
  else
  {
    waveform2.amplitude(0);  // silence
  }

  left_playing = play_left;
  right_playing = play_right;
}

/* 
 * Plays the stereo audio in by toggling the mixer channel volume.
 */
void playAudioIn(bool play_in)
{
  static bool audio_playing;

  if (play_in == audio_playing)
  {
    // No status change, so avoid making unneeded audio engine calls
    // which can generate unwanted audio artifacts and noise.
    return;
  }

  if (play_in)
  {
    mixer1.gain(1, 0.75f);
    mixer2.gain(1, 0.75f);
  }
  else
  {
    mixer1.gain(1, 0);
    mixer2.gain(1, 0);
  }

  audio_playing = play_in;
}


/* 
 * Checks the left and right audio peaks and 
 * draws VU meter audio level bars.
 */
void checkAudioPeaks()
{
  if (peak1.available() && peak2.available()) 
  {
    uint16_t left_peak = peak1.read() * 600.0;
    uint16_t right_peak = peak2.read() * 600.0;
    uint16_t left_color = ILI9341_GREEN;
    uint16_t right_color = ILI9341_GREEN;

    if (left_peak > 270)
    {
      left_color = ILI9341_RED;
    }
    else if (left_peak > 220)
    {
      left_color = ILI9341_YELLOW;
    }

    if (right_peak > 270)
    {
      right_color = ILI9341_RED;
    }
    else if (left_peak > 220)
    {
      right_color = ILI9341_YELLOW;
    }

    tft.drawFastHLine(0, 186, 320, ILI9341_BLACK);
    tft.drawFastHLine(0, 186, left_peak, left_color);

    tft.drawFastHLine(0, 190, 320, ILI9341_BLACK);
    tft.drawFastHLine(0, 190, right_peak, right_color);
  }
}

/*
 * Draws the SD card status icon indicating if the card is present or not.
 */
void drawSD(bool present)
{
  tft.writeRect(300, 0, 18, 24, (uint16_t *)sd_icon);
  if (!present)
    tft.drawLine(300, 0, 318, 24, ILI9341_RED);  
}

/*
 * Use to toggle between normal and inverted text display
 */
void setTextColor(uint16_t color, bool inverted)
{
  if (inverted)
    tft.setTextColor(ILI9341_BLACK, color);
  else
    tft.setTextColor(color, ILI9341_BLACK);
}

/*
 * Updates the screen based on which buttons are being pressed
 */
void updateScreen()
{
  // Direction buttons
  tft.setCursor(45, 50);
  setTextColor(ILI9341_CYAN, btnUp_pressed);
  tft.print("UP"); 

  tft.setCursor(0, 70);
  setTextColor(ILI9341_CYAN, btnLeft_pressed);
  tft.print("<-LT"); 

  tft.setCursor(60, 70);
  setTextColor(ILI9341_CYAN, btnRight_pressed);
  tft.print("RT->");
 
  tft.setCursor(45, 90);
  setTextColor(ILI9341_CYAN, btnDown_pressed);
  tft.print("DN");

  int x = 140;

  // Action buttons
  tft.setCursor(x+30, 50);
  setTextColor(ILI9341_CYAN, btnY_pressed);
  tft.print("Y"); 

  tft.setCursor(x, 70);
  setTextColor(ILI9341_CYAN, btnX_pressed);
  tft.print("X"); 
 
  tft.setCursor(x+60, 70);
  setTextColor(ILI9341_CYAN, btnB_pressed);
  tft.print("B");

  tft.setCursor(x+30, 90);
  setTextColor(ILI9341_CYAN, btnA_pressed);
  tft.print("A");

  // Speakers
  tft.setCursor(180, 196);
  setTextColor(ILI9341_LIGHTGREY, btnLeft_pressed);
  tft.print("((L))");

  tft.setCursor(250, 196);
  setTextColor(ILI9341_LIGHTGREY, btnRight_pressed);
  tft.print("((R))");
  
  if (btnTouch_pressed)
  {
    tft.fillRect(251, 51, 68, 68, ILI9341_CYAN);
    Serial.println("Screen touch detected");
  }  
  else if (btnTouch_released)
  {
    tft.fillRect(251, 51, 68, 68, ILI9341_BLACK);
  }
    
  tft.setCursor(255, 70);
  setTextColor(ILI9341_CYAN, btnTouch_pressed); 
  tft.print("TOUCH");

  // joysticks / rage cons / rotary controller
  setTextColor(ILI9341_ORANGE, joy_left_pressed);
  tft.setCursor(28, 125);
  tft.print("X:");
  tft.print(joy_left_x);
  tft.print("  ");

  tft.setCursor(28, 145);
  tft.print("Y:");
  tft.print(joy_left_y);
  tft.print("  ");

  setTextColor(ILI9341_ORANGE, knob_pressed);  
  tft.setCursor(x, 125);
  tft.print("V:");
  tft.print(knob_value);
  tft.print("  ");

  // Audio in 
  if (btnUp_pressed)
  {
    audioIn_active = true;
  }
  else if (btnDown_pressed)
  {
    audioIn_active = false;
  }

  setTextColor(ILI9341_ORANGE, audioIn_active);  
  tft.setCursor(24, 196);
  tft.print("Audio IN");
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

  if (btnA_pressed) Serial.println("A");
  if (btnB_pressed) Serial.println("B");
  if (btnX_pressed) Serial.println("X");
  if (btnY_pressed) Serial.println("Y");

  if (btnUp_pressed) Serial.println("UP");
  if (btnDown_pressed) Serial.println("DOWN");
  if (btnLeft_pressed) Serial.println("LEFT");
  if (btnRight_pressed) Serial.println("RIGHT");
}

/*
 * Check for touches on the TFT screen and any corresponding on-screen buttons
 */
void checkScreenTouch()
{
  btnTouch_released = false;
  boolean is_touched = ts.touched();
  if (is_touched) 
  {
    TS_Point p = ts.getPoint();
    // Display the x,y touch coordinate for debugging/calibration
    tft.setCursor(24, 220);
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.print(p.x);
    tft.print(", ");
    tft.print(p.y);
    tft.print("   ");

    // Check if the touch is within the bounds of the "touch" button
    btnTouch_pressed = ((p.x > TOUCH_X1) && (p.x < TOUCH_X2) && (p.y > TOUCH_Y1) && (p.y < TOUCH_Y2)); 
  }
  else if (btnTouch_pressed)
  {
    btnTouch_released = true;
    btnTouch_pressed = false;
  }
  else
  {
    btnTouch_pressed = false;
  }
}

/*
 * Check the analog joystick values
 */
void checkJoystick()
{
  // Only use the left 10 bits of the joystick pot readings
  // May want to use analogReadResolution() at some point to eliminate the need for shifts
  joy_left_x = analogRead(JOYX_L) >> 2;
  joy_left_y = analogRead(JOYY_L) >> 2;
  joy_left_pressed = (digitalRead(JBTN_L) == LOW ? 1: 0);

  if (joy_left_pressed) Serial.println("LEFT JOY BTN");
}

/*
 * Check the rotary encoder value
 */
void checkEncoder()
{
  int32_t new_value;
  new_value = knob.read();
 
  if (new_value != knob_value) 
  {
    Serial.print("Knob = ");
    Serial.print(new_value);
    Serial.println();
    knob_value = new_value;
  }

  knob_pressed = (digitalRead(ENC_BTN) == LOW ? 1: 0);
}

/*
 * Main program loop.  Called continuously after setup.
 */
void loop(void) 
{
  checkButtonPresses();
  checkScreenTouch();
  checkJoystick();
  checkEncoder();
  
  updateScreen();

  playAudio(btnLeft_pressed, btnRight_pressed);
  playAudioIn(audioIn_active);
  checkAudioPeaks();

  delay(50);
}