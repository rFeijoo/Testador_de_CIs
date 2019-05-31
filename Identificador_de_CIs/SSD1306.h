/**********************************************************************************************/
/*                                                                                            */
/*                                                                                            */
/*        SSD1306.h                                    Author(s): Bismark C. & Rafael F.      */
/*                                                     Email(s) : bismarkcotrim@hotmail.com   */
/*                                                                goldcard99@hotmail.com      */
/*                                                     Address  : DF, Brasil, 72444-240       */
/*        Created: 2019/05/21 13:54:29 by rFeijo                                              */
/*        Updated: 2019/05/31 04:38:38 by rFeijo                                              */
/*                                                                All Rights Reserved         */
/**********************************************************************************************/

// Includes
  #include "Arduino.h"

// Pre-Processing & Defines
  #define SSD1306_I2C_ADDRESS   0x3C
  #define SSD1306_128_64

  #if defined SSD1306_128_64
    #define SSD1306_LCDWIDTH    128
    #define SSD1306_LCDHEIGHT   64
  #endif

  #define SSD1306_SETCONTRAST         0x81
  #define SSD1306_DISPLAYALLON_RESUME 0xA4
  #define SSD1306_DISPLAYALLON        0xA5
  #define SSD1306_NORMALDISPLAY       0xA6
  #define SSD1306_INVERTDISPLAY       0xA7
  #define SSD1306_DISPLAYOFF          0xAE
  #define SSD1306_DISPLAYON           0xAF

  #define SSD1306_SETDISPLAYOFFSET    0xD3
  #define SSD1306_SETCOMPINS          0xDA
  
  #define SSD1306_SETVCOMDETECT       0xDB
  
  #define SSD1306_SETDISPLAYCLOCKDIV  0xD5
  #define SSD1306_SETPRECHARGE        0xD9
  
  #define SSD1306_SETMULTIPLEX        0xA8
  
  #define SSD1306_SETLOWCOLUMN        0x00
  #define SSD1306_SETHIGHCOLUMN       0x10
  
  #define SSD1306_SETSTARTLINE        0x40
  
  #define SSD1306_MEMORYMODE          0x20
  
  #define SSD1306_COMSCANINC          0xC0
  #define SSD1306_COMSCANDEC          0xC8

  #define SSD1306_SEGREMAP            0xA0

  #define SSD1306_CHARGEPUMP          0x8D

  #define SSD1306_EXTERNALVCC         0x1
  #define SSD1306_SWITCHCAPVCC        0x2

  // Scrolling
  #define SSD1306_ACTIVATE_SCROLL                       0x2F
  #define SSD1306_DEACTIVATE_SCROLL                     0x2E
  #define SSD1306_SET_VERTICAL_SCROLL_AREA              0xA3
  #define SSD1306_RIGHT_HORIZONTAL_SCROLL               0x26
  #define SSD1306_LEFT_HORIZONTAL_SCROLL                0x27
  #define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL  0x29
  #define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL   0x2A

// SSD1306 Class
  class SSD1306
  {
    public:
      //SSD1306(int8_t RST = 4);
    
      void begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = SSD1306_I2C_ADDRESS);
      void ssd1306_command(uint8_t c);
      void ssd1306_data(uint8_t c);
    
      void invertDisplay(uint8_t i);
      void draw8x8(byte* buffer, byte x, byte y);
    
      void startscrollright(uint8_t start, uint8_t stop);
      void startscrollleft(uint8_t start, uint8_t stop);
    
      void startscrolldiagright(uint8_t start, uint8_t stop);
      void startscrolldiagleft(uint8_t start, uint8_t stop);
      
      void stopscroll(void);
    
      void fill(unsigned char dat);
    
    protected:
      uint8_t _i2caddr;
      
    private:
      int8_t sclk, dc, rst, cs;
  };
