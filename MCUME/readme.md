### Using MCUME on the Teensy R4ge Pro

1. Install the 8MB PSRAM chip on the bottom of your Teensy 4.1.  Purchase link, instructions, and test app can be found at https://www.pjrc.com/store/psram.html 
2. Make sure you have a FAT32 formatted micro SD available.
3. Having the USB pins soldered to the Teensy and hooked up to a USB keyboard is very helpful as some games/options require keyboard input to proceed.  Can purchase the USB host cable from https://www.pjrc.com/store/cable_usb_host_t36.html   
3. Clone the MCUME repo: https://github.com/Jean-MarcHarvengt/MCUME/ 
4. Create the directories in the root of the SD card for the emulator ROM files and copy your ROM files to them.  Place the microSD in the Teensy's built-in SD card slot.
5. Copy the iopins.h and platform_config.h files from the Teensy-R4ge-Pro folders (or copy/paste the settings) to set the correct hardware configuration for each emulator.
6. Load the sketch for the emulator you want to run into the Arduino IDE.
7. Select Tools->Optimize and choose the appropriate option based on the emulator.
    - Doom:  Faster
    - Castaway/Atari ST:  Smallest Code
    - NES/Nofrendo:  Faster
    - UAE/Amiga:  Smallest Code
  
8. Compile and load the sketch to the Teensy.  If it works, you should see the game/emulator screen and be able to select from the list of detected ROMs that you loaded to the microSD. 

## Troubleshooting
- If compilation fails and exceeds available memory, try optimizing for Smallest Code
- If compiling on Mac or Linux, may need to fix some include file capitalization errors in MCUME (i.e. in tft_t_dma.cpp).
- Verify that the PSRAM is installed and working using the Teensy example sketch
- Hookup a keyboard to the Teensy USB pins and try different keypresses if screen appears frozen
- Try loading different ROMs.  Quite a few of the tested ROMs failed to work.
