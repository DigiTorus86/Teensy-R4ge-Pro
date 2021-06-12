# Teensy-R4ge-Pro
<p>
Software and resources for the Teensy R4ge Pro custom badge. See documents folder for schematic.  
</p>

<p>
Harness the power of the 600MHz ARM Cortex M-7 that powers the Teensy 4.1 with flexible controls, a large display, CD-quality sound input and output.  Runs a variety of emulators for a retro-tastic mobile arcade experience.  Works as a high-fidelity sound generating and processing platform.  Includes a 10dB booster circuit with gain control, suitable for applying digital effects to a variety of audio input sources - including electric guitars.  
</p>

![alt text](https://raw.githubusercontent.com/DigiTorus86/Teensy-R4ge-Pro/master/images/teensy-r4ge-pro-test.png)

<h3>Key Components/Requirements</h3>
<p>
- Custom PCB, bread board, or protoboard.<br>
- Teensy 4.1<br>
- ILI9341 320x240 TFT with touch screen<br>
- Analog joystick<br>
- Rotary encoder<br>
- PCM5102a I2S DAC w/headphone jack<br>
- PAM8403 amplifier and 8 Ohm/1W speakers and/or powered external speakers w/3.5mm jack<br>
- PCM1802 I2S Stereo ADC<br>
- TL072 OpAmp<br>
- 8 MB PSRAM, SD card, and USB host cable for Teensy strongly recommended, as these are required to run different platform emulators<br>
- Some sketches have additional hardware requirements (components attached to I2C or Serial pins, or SD card)<br>
- See BOM file in the documents folder for full list of components.<br>
- STL file for printing a simple case also included in the documents folder.<br>
</p>

<p>
  Runs Doom and the other MCUME emulators by Jean MarcHarvengt.    https://github.com/Jean-MarcHarvengt/MCUME
  Requires minor config and IO pin mapping changes.  
</p>

<h3>App Descriptions</h3>
<p>
- Test:   Exercises all the hardware on the board to verify operation and calibration.
</p>

![alt text](https://raw.githubusercontent.com/DigiTorus86/Teensy-R4ge-Pro/master/images/Teensy-R4ge-Pro-PCB-3D.png)
