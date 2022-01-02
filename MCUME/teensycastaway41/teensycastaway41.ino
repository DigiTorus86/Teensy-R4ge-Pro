extern "C" {
  #include "iopins.h"  
  #include "emuapi.h"  
}
#include "keyboard_osd.h"

#include "test.h"

#ifdef HAS_VGA
#include <uVGA.h>
uVGA uvga;
#if F_CPU == 144000000
#define UVGA_144M_326X240
#define UVGA_XRES 326
#define UVGA_YRES 240
#define UVGA_XRES_EXTRA 10
#elif  F_CPU == 180000000
#define UVGA_180M_360X300
#define UVGA_XRES 360
#define UVGA_YRES 300
#define UVGA_XRES_EXTRA 8 
#elif  F_CPU == 240000000
#define UVGA_240M_452X240
#define UVGA_XRES 452
#define UVGA_YRES 240
#define UVGA_XRES_EXTRA 12 
#else
#error Please select F_CPU=240MHz or F_CPU=180MHz or F_CPU=144MHz
#endif

#include <uVGA_valid_settings.h>

uint8_t * VGA_frame_buffer;
#endif


#ifdef HAS_T4_VGA
#include "vga_t_dma.h"
TFT_T_DMA tft;
#else
#include "tft_t_dma.h"
TFT_T_DMA tft = TFT_T_DMA(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO, TFT_TOUCH_CS, TFT_TOUCH_INT);
#endif

bool vgaMode = false;

static unsigned char  palette8[PALETTE_SIZE];
static unsigned short palette16[PALETTE_SIZE];
static IntervalTimer myTimer;
volatile boolean vbl=true;
static int skip=0;
static elapsedMicros tius;

static void vblCount() { 
  if (vbl) {
    vbl = false;
  } else {
    vbl = true;
  }
}

void emu_SetPaletteEntry(unsigned char r, unsigned char g, unsigned char b, int index)
{
  if (index<PALETTE_SIZE) {
    palette8[index]  = RGBVAL8(r,g,b);
    palette16[index] = RGBVAL16(r,g,b);   
  }
}

void emu_DrawVsync(void)
{
  volatile boolean vb=vbl;
  skip += 1;
  skip &= VID_FRAME_SKIP;
  if (!vgaMode) {
#ifdef HAS_T4_VGA
    tft.waitSync();
#else
    while (vbl==vb) {};
#endif    
  }
#ifdef HAS_VGA
  else {
    while (vbl==vb) {};
  }
#endif  
}

void emu_DrawWaitLine(int line)
{
  volatile boolean vb=vbl;
  skip += 1;
  skip &= VID_FRAME_SKIP;
  if (!vgaMode) {
#ifdef HAS_T4_VGA
    tft.waitLine(line);
#else
    while (vbl==vb) {};
#endif    
  }
#ifdef HAS_VGA
  else {
    while (vbl==vb) {};
  }
#endif  
}
void emu_DrawLine(unsigned char * VBuf, int width, int height, int line) 
{
  if (!vgaMode) {
#ifdef HAS_T4_VGA
    tft.writeLine(width,1,line, VBuf, palette8);
#else
    tft.writeLine(width,1,line, VBuf, palette16);
#endif    
  }
#ifdef HAS_VGA
  else {
    int fb_width=UVGA_XRES,fb_height=UVGA_YRES;
    fb_width += UVGA_XRES_EXTRA;
    int offx = (fb_width-width)/2;   
    int offy = (fb_height-height)/2+line;
    uint8_t * dst=VGA_frame_buffer+(fb_width*offy)+offx;    
    for (int i=0; i<width; i++)
    {
       uint8_t pixel = palette8[*VBuf++];
        *dst++=pixel;
    }
  }
#endif  
}  

void emu_DrawLine8(unsigned char * VBuf, int width, int height, int line) 
{
  if (!vgaMode) {
    if (skip==0) {
#ifdef HAS_T4_VGA
      tft.writeLine(width,height,line, VBuf);
#endif      
    }
  }  
#ifdef HAS_VGA 
#endif    
} 

void emu_DrawLine16(unsigned short * VBuf, int width, int height, int line) 
{
  if (!vgaMode) {
    if (skip==0) {
#ifdef HAS_T4_VGA
      tft.writeLine16(width,height,line, VBuf);
#else
      tft.writeLine(width,height,line, VBuf);
#endif      
    }
  }  
#ifdef HAS_VGA 
#endif    
} 

void emu_DrawScreen(unsigned char * VBuf, int width, int height, int stride) 
{
  if (!vgaMode) {
    if (skip==0) {
#ifdef HAS_T4_VGA
      tft.writeScreen(width,height-TFT_VBUFFER_YCROP,stride, VBuf+(TFT_VBUFFER_YCROP/2)*stride, palette8);
#else
      tft.writeScreen(width,height-TFT_VBUFFER_YCROP,stride, VBuf+(TFT_VBUFFER_YCROP/2)*stride, palette16);
#endif
    }
  }
#ifdef HAS_VGA
  else {
    int fb_width=UVGA_XRES, fb_height=UVGA_YRES;
    //uvga.get_frame_buffer_size(&fb_width, &fb_height);
    fb_width += UVGA_XRES_EXTRA;
    int offx = (fb_width-width)/2;   
    int offy = (fb_height-height)/2;
    uint8_t * buf=VGA_frame_buffer+(fb_width*offy)+offx;
    for (int y=0; y<height; y++)
    {
      uint8_t * dest = buf;
      for (int x=0; x<width; x++)
      {
        uint8_t pixel = palette8[*VBuf++];
        *dest++=pixel;
        //*dest++=pixel;
      }
      buf += fb_width;
      VBuf += (stride-width);
    }             
  }
#endif  
}

void emu_CopyLine(int width, int height, int ysrc, int ydst)
{
#ifdef HAS_T4_VGA
  tft.copyLine(width,height,ysrc,ydst);
#endif  
}

int emu_FrameSkip(void)
{
  return skip;
}

void * emu_LineBuffer(int line)
{
  if (!vgaMode) {
    return (void*)tft.getLineBuffer(line);    
  }
#ifdef HAS_VGA
  else {
    int fb_width=UVGA_XRES;
    fb_width += UVGA_XRES_EXTRA;
    return (void*)VGA_frame_buffer+(fb_width*line);    
  }   
#endif  
}

void emu_tweakVideo(int shiftdelta, int numdelta, int denomdelta) {
#ifdef HAS_T4_VGA
  tft.tweak_video(shiftdelta, numdelta, denomdelta);
#endif  
}


// ****************************************************
// the setup() method runs once, when the sketch starts
// ****************************************************
void setup() {
  
#ifdef HAS_T4_VGA
#ifdef HIRES
  tft.begin(VGA_MODE_640x480);
#else
  tft.begin(VGA_MODE_320x240);
#endif
#else
  tft.begin();
#endif

  emu_init(); 

  myTimer.begin(vblCount, 20000);  //to run every 20ms  
}

// ****************************************************
// the loop() method runs continuously
// ****************************************************

void loop(void) 
{
  static char floppy2[64]="";  
  if (menuActive()) {
    uint16_t bClick = emu_DebounceLocalKeys();
    int action = handleMenu(bClick);
    char * floppy1 = menuSelection();   
    if (action == ACTION_RUN1) {
      toggleMenu(false);
      vgaMode = false;
      emu_start();        
      emu_Init(floppy1,floppy2,0);     
      tft.fillScreenNoDma( RGBVAL16(0x00,0x00,0x00) );
      tft.startDMA();      
    }    
    else if (action == ACTION_RUN2)  {
      strcpy(floppy2,menuSelection());                        
    }         
    else if (action == ACTION_RUN3)  {
      toggleMenu(false);
      vgaMode = false;
      emu_start();        
      emu_Init(floppy1,floppy2,1);     
      tft.fillScreenNoDma( RGBVAL16(0x00,0x00,0x00) );
      tft.startDMA();                    
    }         
    delay(20);
  }
  else if (callibrationActive()) {
    uint16_t bClick = emu_DebounceLocalKeys();
    handleCallibration(bClick);
  } 
  else {
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)    
#else
    handleVirtualkeyboard();
#endif    
    if ( (!virtualkeyboardIsActive()) || (vgaMode) ) {  
      emu_Step();   
      uint16_t bClick = emu_DebounceLocalKeys();
      emu_Input(bClick);      
    }
  }  
}


#ifdef HAS_SND

#include "AudioPlaySystem.h"

AudioPlaySystem mymixer;
#ifndef HAS_T4_VGA
#include <Audio.h>
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)    
//AudioOutputMQS  mqs;
//AudioConnection patchCord9(mymixer, 0, mqs, 1);
AudioOutputI2S  i2s1;
AudioConnection patchCord8(mymixer, 0, i2s1, 0);
AudioConnection patchCord9(mymixer, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;
#else
AudioOutputAnalog dac1;
AudioConnection   patchCord1(mymixer, dac1);
#endif
#endif

void emu_sndInit() {
  Serial.println("sound init");  
#ifdef HAS_T4_VGA
  tft.begin_audio(256, mymixer.snd_Mixer);  
 // sgtl5000_1.enable();
 // sgtl5000_1.volume(0.6);
#endif  
  mymixer.start();
}

void emu_sndPlaySound(int chan, int volume, int freq)
{
  if (chan < 6) {
    mymixer.sound(chan, freq, volume); 
  }
  /*
  Serial.print(chan);
  Serial.print(":" );  
  Serial.print(volume);  
  Serial.print(":" );  
  Serial.println(freq); 
  */ 
}

void emu_sndPlayBuzz(int size, int val) {
  mymixer.buzz(size,val); 
  //Serial.print((val==1)?1:0); 
  //Serial.print(":"); 
  //Serial.println(size); 
}
#endif
