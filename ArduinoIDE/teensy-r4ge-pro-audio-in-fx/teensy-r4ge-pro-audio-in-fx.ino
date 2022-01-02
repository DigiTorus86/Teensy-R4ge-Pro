/***************************************************
Teensy R4ge Pro audio input effects
Simplest possible "Hello, world" demonstration for
adding digital effect processing to I2S audio input.

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

// See:  https://www.pjrc.com/teensy/gui/
// GUItool: begin automatically generated code
AudioInputI2S            i2s_in;         //xy=259.23333740234375,380
AudioEffectReverb        reverb2;        //xy=445.23333740234375,447.23333740234375
AudioEffectReverb        reverb1;        //xy=449.23333740234375,347.23333740234375
AudioOutputI2S           i2s_out;        //xy=597.2333374023438,383
AudioConnection          patchCord1(i2s_in, 0, reverb1, 0);
AudioConnection          patchCord2(i2s_in, 1, reverb2, 0);
AudioConnection          patchCord3(reverb1, 0, i2s_out, 1);
AudioConnection          patchCord4(reverb1, 0, i2s_out, 0);
// GUItool: end automatically generated code


void setup() 
{
 AudioMemory(40);

 reverb1.reverbTime(0.5); // in seconds
 reverb2.reverbTime(0.5);
 
}

void loop() 
{
}
