/**********************************************************************************************/
/*                                                                                            */
/*                                                                                            */
/*        SSD1306.cpp                                  Author(s): Bismark Cotrim & Rafael F.  */
/*                                                     Email(s) : bismarkcotrim@hotmail.com   */
/*                                                                goldcard99@hotmail.com      */
/*                                                     Address  : DF, Brasil, 72444-240       */
/*        Created: 2019/05/21 13:54:29 by rFeijo                                              */
/*        Updated: 2019/05/27 10:35:44 by rFeijo                                              */
/*                                                                All Rights Reserved         */
/**********************************************************************************************/

// Includes
  #include "SSD1306.h"
  #include <Wire.h>

// LCD SSD1306 Class :: Configure Pins
  SSD1306::SSD1306(int8_t SCLK, int8_t DC, int8_t RST, int8_t CS)
  {
    sclk = SCLK;
    rst  = RST;
    cs   = CS;
    dc   = DC;
  }

// LCD SSD1306 Class :: Set RESET Pin
  SSD1306::SSD1306(int8_t reset)
  {
    sclk = dc = cs = -1;
    rst = reset;
  }

// LCD SDD1306 Class :: Start Communication
  void SSD1306::begin(uint8_t vccstate, uint8_t i2caddr)
  {
    _i2caddr = i2caddr;
    
    Wire.begin();

    pinMode(rst, OUTPUT);
    digitalWrite(rst, HIGH);
    
    delay(10);
    
    digitalWrite(rst, LOW);

    delay(10);
    
    digitalWrite(rst, HIGH);

    #if defined SSD1306_128_64                              // Init configuration sequence for 128x64 OLED module
    ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    ssd1306_command(0x80);                                  // 0x80
    ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    ssd1306_command(0x3F);                                  // 0x3F
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    ssd1306_command(0x0);                                   // No offset
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
    ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
    if (vccstate == SSD1306_EXTERNALVCC)
      { ssd1306_command(0x10); }
    else
      { ssd1306_command(0x14); }
    ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
    ssd1306_command(0x00);                                  // 0x0 act like ks0108
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
    ssd1306_command(0x12);
    ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
    if (vccstate == SSD1306_EXTERNALVCC)
      { ssd1306_command(0x9F); }
    else
      { ssd1306_command(0xCF); }
    ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
    if (vccstate == SSD1306_EXTERNALVCC)
      { ssd1306_command(0x22); }
    else
      { ssd1306_command(0xF1); }
    ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
    #endif

    ssd1306_command(SSD1306_DISPLAYON);                     // Turn OLED on

    delay(10);

    ssd1306_command(SSD1306_SETLOWCOLUMN  | 0x0);           // Low col = 0
    ssd1306_command(SSD1306_SETHIGHCOLUMN | 0x0);           // Hi  col = 0
    ssd1306_command(SSD1306_SETSTARTLINE  | 0x0);           // Line #0

    for (byte i = 0; i < SSD1306_LCDHEIGHT / 8; i++)
    {
      for(byte j = 0; j < 8; j++)
      {
        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte k = 0; k < SSD1306_LCDWIDTH / 8; k++)
            Wire.write(0);
        Wire.endTransmission();
      }
    }
  }

// LCD SSD1306 Class :: Invert Display Contrast
  void SSD1306::invertDisplay(uint8_t i)
  {
    if (i)
      ssd1306_command(SSD1306_INVERTDISPLAY);
    else
      ssd1306_command(SSD1306_NORMALDISPLAY);
  }

// LCD SSD1306 Class :: Send Command to Module
  void SSD1306::ssd1306_command(uint8_t c)
  {
    uint8_t control = 0x00;                         // Co = 0, D/C = 0
    
    Wire.beginTransmission(_i2caddr);
    Wire.write(control);
    Wire.write(c);
    Wire.endTransmission();
  }

// LCD SSD1306 Class :: Scroll Right
// Hint: the display is 16 rows tall. To scroll the whole display, run "oled.scrollright(0x00, 0x0F)"
  void SSD1306::startscrollright(uint8_t start, uint8_t stop)
  {
    ssd1306_command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
    ssd1306_command(0X00);
    ssd1306_command(start);
    ssd1306_command(0X00);
    ssd1306_command(stop);
    ssd1306_command(0X01);
    ssd1306_command(0XFF);
    ssd1306_command(SSD1306_ACTIVATE_SCROLL);
  }

// LCD SSD1306 Class :: Scroll Left
// Hint: the display is 16 rows tall. To scroll the whole display, run "oled.scrollright(0x00, 0x0F)"
  void SSD1306::startscrollleft(uint8_t start, uint8_t stop)
  {
    ssd1306_command(SSD1306_LEFT_HORIZONTAL_SCROLL);
    ssd1306_command(0X00);
    ssd1306_command(start);
    ssd1306_command(0X00);
    ssd1306_command(stop);
    ssd1306_command(0X01);
    ssd1306_command(0XFF);
    ssd1306_command(SSD1306_ACTIVATE_SCROLL);
  }

// LCD SSD1306 Class :: Diagonal Scroll Right
// Hint: the display is 16 rows tall. To scroll the whole display, run "oled.scrollright(0x00, 0x0F)"
  void SSD1306::startscrolldiagright(uint8_t start, uint8_t stop)
  {
    ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
    ssd1306_command(0X00);
    ssd1306_command(SSD1306_LCDHEIGHT);
    ssd1306_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
    ssd1306_command(0X00);
    ssd1306_command(start);
    ssd1306_command(0X00);
    ssd1306_command(stop);
    ssd1306_command(0X01);
    ssd1306_command(SSD1306_ACTIVATE_SCROLL);
  }

// LCD SSD1306 Class :: Diagonal Scroll Left
// Hint: the display is 16 rows tall. To scroll the whole display, run "oled.scrollright(0x00, 0x0F)"
  void SSD1306::startscrolldiagleft(uint8_t start, uint8_t stop)
  {
    ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
    ssd1306_command(0X00);
    ssd1306_command(SSD1306_LCDHEIGHT);
    ssd1306_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
    ssd1306_command(0X00);
    ssd1306_command(start);
    ssd1306_command(0X00);
    ssd1306_command(stop);
    ssd1306_command(0X01);
    ssd1306_command(SSD1306_ACTIVATE_SCROLL);
  }

// LCD SSD1306 Class :: Stop Scrolling
  void SSD1306::stopscroll(void)
  {
    ssd1306_command(SSD1306_DEACTIVATE_SCROLL);
  }

// LCD SSD1306 Class :: Send data
  void SSD1306::ssd1306_data(uint8_t c)
  {
      uint8_t control = 0x40;                   // Co = 0, D/C = 1
      
      Wire.beginTransmission(_i2caddr);
      Wire.write(control);
      Wire.write(c);
      Wire.endTransmission();
  }

// LCD SSD1306 Class :: Fill Display in
  void SSD1306::fill(unsigned char dat)
  {
    unsigned char i,j;

    ssd1306_command(0x00);                // Set lower column address
    ssd1306_command(0x10);                // Set higher column address
    ssd1306_command(0xB0);                // Set page address

    for (byte i=0; i<(SSD1306_LCDHEIGHT/8); i++)
    {
      // Send a bunch of data in one xmission
      ssd1306_command(0xB0 + i);          // Set page address
      ssd1306_command(0);                 // Set lower column address
      ssd1306_command(0x10);              // Set higher column address
      
      for(byte j = 0; j < 8; j++)
      {
        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte k = 0; k < 16; k++)
          Wire.write(dat);
        Wire.endTransmission();
      }
    }
  }

// LCD SSD1306 Class :: Draw Matrix 8x8
  void SSD1306::draw8x8(byte* buffer, uint8_t x, uint8_t y)
  {
    // Send a bunch of data in one xmission
    ssd1306_command(0xB0 + y);            // Set page address
    ssd1306_command(x & 0xf);             // Set lower column address
    ssd1306_command(0x10 | (x >> 4));     // Set higher column address

    Wire.beginTransmission(_i2caddr);
    Wire.write(0x40);
    Wire.write(buffer, 8);
    Wire.endTransmission();
  }
