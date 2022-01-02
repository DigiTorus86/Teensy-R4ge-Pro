/***************************************************
Teensy R4ge Pro audio input effects
Simple I2S mono input delay + reverb to stereo out effect.

Requires:
 - Teensy R4ge Pro v2
 - Teensy 4.1 board
 - Audio source and 3.5mm phone cable to test audio in features 

Copyright (c) 2021 Paul Pagel
This is free software; see the license.txt file for more information.
There is no warranty; not even for merchantability or fitness for a particular purpose.
*****************************************************/


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <ILI9341_t3.h>

#define TFT_DC     9  // Data (high)/Command (low) pin
#define TFT_CS    10  // SPI Chip Select pin for screen (active low)
#define TFT_RST    1  // Reset pin NOT USED, connected to 3V3 (always high)

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST);


// GUItool: begin automatically generated code
AudioInputI2S            i2s_in;         
//AudioEffectChorus        chorus1; 
AudioEffectDelay         delay1; 
//AudioEffectReverb        reverb1;  
AudioEffectFreeverb      freeverb1;  
//AudioEffectFreeverbStereo freeverbs1;   
//AudioEffectBitcrusher    bitcrusher1;   
//AudioFilterStateVariable filter1;   
AudioOutputI2S           i2s_out;    

// Reverb mono only
/*  
AudioConnection          patchCord1(i2s_in, 0, reverb1, 0);
AudioConnection          patchCord2(reverb1, 0, i2s_out, 1);
AudioConnection          patchCord3(reverb1, 0, i2s_out, 0);
reverb1.reverbTime(0.1); // in seconds
*/

// Freeverb mono only
/*
AudioConnection          patchCord1(i2s_in, 0, freeverb1, 0);
AudioConnection          patchCord2(freeverb1, 0, i2s_out, 0);
AudioConnection          patchCord3(freeverb1, 0, i2s_out, 1);
freeverb1.roomsize(0.1); // 0.0 (smallest) to 1.0 (largest)
freeverb1.damping(0.7); // 0.0 (no damping) to 1.0 (most damping)
*/

// Freeverb stereo only
/*
AudioConnection          patchCord1(i2s_in, 0, freeverbs1, 0);
AudioConnection          patchCord2(freeverbs1, 0, i2s_out, 0);
AudioConnection          patchCord3(freeverbs1, 1, i2s_out, 1);
freeverbs1.roomsize(0.1); // 0.0 (smallest) to 1.0 (largest)
freeverbs1.damping(0.7); // 0.0 (no damping) to 1.0 (most damping)
*/

// Delay + freeverb - sounds pretty good!
AudioConnection          patchCord1(i2s_in, 0, delay1, 0);
AudioConnection          patchCord2(delay1, 1, freeverb1, 0);
AudioConnection          patchCord3(freeverb1, 0, i2s_out, 0);
AudioConnection          patchCord4(delay1, 0, i2s_out, 1);

// Bitcrusher only (sounds pretty crappy with guitar)
/*
AudioConnection          patchCord1(i2s_in, 0, bitcrusher1, 0);
AudioConnection          patchCord2(bitcrusher1, 0, i2s_out, 0);
AudioConnection          patchCord3(bitcrusher1, 0, i2s_out, 1); 
// bitcrusher1.bits(8);
// bitcrusher1.sampleRate(22050);*/

/*
AudioConnection          patchCord1(i2s_in, 0, freeverbs1, 0);
AudioConnection          patchCord2(filter1, 0, freeverbs1, 0);
AudioConnection          patchCord3(freeverbs1, 0, i2s_out, 0);
AudioConnection          patchCord4(filter1, 1, i2s_out, 1);
*/
// GUItool: end automatically generated code


void setup() 
{
  AudioMemory(40);
  AudioNoInterrupts();

  delay1.delay(0, 0);
  delay1.delay(1, 50);

  freeverb1.roomsize(0.1); // 0.0 (smallest) to 1.0 (largest)
  freeverb1.damping(0.75); // 0.0 (no damping) to 1.0 (most damping)

 //bitcrusher1.bits(8);
 //bitcrusher1.sampleRate(44100);

 AudioInterrupts();
}

void loop() {
}
