#ifndef _TEENSY_R4GE_PRO_
#define _TEENSY_R4GE_PRO_

// Pin assignments for v1 board
// Currently unused pins:

// GPIO pin assignments for the ILI9341 TFT display
#define TFT_DC     9  // Data (high)/Command (low) pin
#define TFT_CS    10  // SPI Chip Select pin for screen (active low)
#define TFT_RST    1  // Reset pin NOT USED, connected to 3V3 (always high)
#define TFT_MISO  12  // SPI Master In Slave Out       
#define TFT_MOSI  11  // SPI Master Out Slave In         
#define TFT_CLK   13  // SPI Clock
#define TFT_LED   -1  // TFT LED brightness control
#define TCH_CS    37  // SPI Touch controller Chip Select (active low)
#define TCH_IRQ   -1  // Touch IRQ interrupt - Not Connected
#define SD_CS     -1  // SPI SD card reader Chip Select pin NOT USED (active low)  

#define JOYX_L    16  // Left analog joystick X axis
#define JOYY_L    17  // Left analog joystick Y axis
#define JBTN_L    25  // Left joystick button 

#define ENC_1    14  // Rotary encoder pin 1
#define ENC_2    15  // Rotary encoder pin 2
#define ENC_BTN  24  // Rotary encoder button/switch

#define MIC       -1  // No microphone
#define I2S_LRCK  20  // I2S left right select, also known as WSEL/word select (DAC1 pin)
#define I2S_BCLK  21  // I2S bit clock (DAC2 pin)
#define I2S_DOUT   7  // Sound data out to DIN pin on PCM5102a  (was 22 on rev1.0, 4 on rev1.1)

#define USR_LED   13  // built-in LED on Teensy dev board

// Teensy 4.1 I2C Pins 
//      SDA       18
//      SCL       19

// Serial 2 pins (broken out on right of display)
//      RX2       34  (RX8)
//      TX2       35  (TX8)

#define BTN_Y     5 
#define BTN_X     4  
#define BTN_B     3 
#define BTN_A     2 
#define BTN_RIGHT 32 
#define BTN_LEFT  31 
#define BTN_DOWN  30
#define BTN_UP    29 

#define TFT_LED_CHANNEL 5 // screen brightness PWM control (lower channels used by I2S)

// Calibration constants
// These may need to be adjusted due to variations in boards and joystick pots
#define JOYX_L_CTR  462
#define JOYY_L_CTR  456
#define JOY_5BIT_CTR 14

// Screen properties and orientation
#define SCREEN_ROT    3  // horizontal, w/SD pins on the right
#define SCREEN_WD   320
#define SCREEN_HT   240
#define TCHSCRN_ROT   1  // horizontal

#endif // _TEENSY_R4GE_PRO_