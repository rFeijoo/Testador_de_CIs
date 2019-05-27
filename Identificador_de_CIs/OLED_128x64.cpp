/**********************************************************************************************/
/*                                                                                            */
/*                                                                                            */
/*        OLED_128x64.cpp                              Author(s): Bismark C. & Rafael F.      */
/*                                                     Email(s) : bismarkcotrim@hotmail.com   */
/*                                                                goldcard99@hotmail.com      */
/*                                                     Address  : DF, Brasil, 72444-240       */
/*        Created: 2019/05/21 13:54:29 by rFeijo                                              */
/*        Updated: 2019/05/27 16:00:05 by rFeijo                                              */
/*                                                                All Rights Reserved         */
/**********************************************************************************************/

// Includes
  #include "OLED_128x64.h"
  #include "Library.h"
  #include <Wire.h>

// LCD Common Class :: Print Integers
  void LCD_Common::printInt(uint16_t value, int8_t padding)
  {
    uint16_t den = 10000;
    
    for (int8_t i = 5; i > 0; i--)
    {
      byte v = (byte)(value / den);
      value -= v * den;
      den   /= 10;
      if (v == 0 && padding && den)
      {
        if (padding >= i)
            writeDigit((m_flags & FLAG_PAD_ZERO) ? 0 : -1);
        continue;
      }
      padding = 0;
      writeDigit(v);
    }
  }

// LCD Common Class :: Print Long Integers
  void LCD_Common::printLong(uint32_t value, int8_t padding)
  {
    uint32_t den = 1000000000;
    
    for (int8_t i = 10; i > 0; i--)
    {
      byte v = (byte)(value / den);
      value -= v * den;
      den   /= 10;
      if (v == 0 && padding && den)
      {
        if (padding >= i)
            writeDigit((m_flags & FLAG_PAD_ZERO) ? 0 : -1);
        continue;
      }
      padding = 0;
      writeDigit(v);
    }
  }

// LCD SSD1306 Class :: Set Cursor at Breakpoint
  void LCD_SSD1306::setCursor(byte column, byte line)
  {
      m_col = column;
      m_row = line;
      ssd1306_command(0xB0 + m_row);          // Set page address
      ssd1306_command(m_col & 0xf);           // Set lower column address
      ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address
  }

// LCD SSD1306 Class :: Write on I2C BUS
  size_t LCD_SSD1306::write(uint8_t c)
  {
    if (c == '\n')
    {
      setCursor(0, m_row + ((m_font == FONT_SIZE_SMALL) ? 1 : 2));
      return 1;
    }
    else if (c == '\r')
    {
      m_col = 0;
      return 1;
    }
    if (m_font == FONT_SIZE_SMALL)
    {
      Wire.beginTransmission(_i2caddr);
      Wire.write(0x40);
      if (c > 0x20 && c < 0x7f)
      {
        c -= 0x21;
        for (byte i = 0; i < 5; i++)
        {
          byte d = pgm_read_byte(&font5x8[c][i]);
          Wire.write(d);
          if (m_flags & FLAG_PIXEL_DOUBLE_H)
            Wire.write(d);
        }
        Wire.write(0);
      }
      else
      {
        for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 11 : 6; i > 0; i--)
        {
          Wire.write(0);
        }
      }
      Wire.endTransmission();
      m_col += (m_flags & FLAG_PIXEL_DOUBLE_H) ? 11 : 6;
      if (m_col >= 128)
      {
        m_col = 0;
        m_row ++;
      }
    }
    else
    {
      if (c > 0x20 && c < 0x7f)
      {
        c -= 0x21;
  
        ssd1306_command(0xB0 + m_row);          // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address
  
        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte i = 0; i <= 14; i += 2)
        {
          byte d = pgm_read_byte(&font8x16_terminal[c][i]);
          Wire.write(d);
          if (m_flags & FLAG_PIXEL_DOUBLE_H)
            Wire.write(d);
        }
        Wire.endTransmission();
  
        ssd1306_command(0xB0 + m_row + 1);      // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address
  
        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte i = 1; i <= 15; i += 2)
        {
          byte d = pgm_read_byte(&font8x16_terminal[c][i]);
          Wire.write(d);
          if (m_flags & FLAG_PIXEL_DOUBLE_H)
            Wire.write(d);
        }
        Wire.endTransmission();
      }
      else
      {
        ssd1306_command(0xB0 + m_row);          // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 16 : 8; i > 0; i--)
        {
          Wire.write(0);
        }
        Wire.endTransmission();

        ssd1306_command(0xB0 + m_row + 1);      // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 16 : 8; i > 0; i--)
        {
          Wire.write(0);
        }
        Wire.endTransmission();
      }
      m_col += (m_flags & FLAG_PIXEL_DOUBLE_H) ? 17 : 9;
      if (m_col >= 128)
      {
        m_col = 0;
        m_row += 2;
      }
    }
    return 1;
  }

// LCD SSD1306 Class :: Print Logo
  void LCD_SSD1306::printLogo(const PROGMEM byte* buffer, byte width, byte height)
  {
    clear();

    setCursor(40, 0);
    draw(buffer, width, height);
    delay(200);

    setFontSize(FONT_SIZE_SMALL);
    setCursor(20, 7);
    println("Testador de CIs");
    
    delay(800);
  }

// LCD SSD1306 Class :: Print Result
  void LCD_SSD1306::printResult(uint16_t r)
  {
    clear();

    setFontSize(FONT_SIZE_MEDIUM);
    switch (r)
    {
      case 4:
        setCursor(53, 2);
        println("NOT");
        setFontSize(FONT_SIZE_SMALL);
        setCursor(48, 5);
        println("74XX04");
        break;
      case 8:
        setCursor(48, 2);
        println("AND2");
        setFontSize(FONT_SIZE_SMALL);
        setCursor(48, 5);
        println("74XX08");
        break;
      case 32:
        setCursor(53, 2);
        println("OR2");
        setFontSize(FONT_SIZE_SMALL);
        setCursor(48, 5);
        println("74XX32");
        break;
      case 0:
        setCursor(44, 2);
        println("NAND2");
        setFontSize(FONT_SIZE_SMALL);
        setCursor(48, 5);
        println("74XX00");
        break;
      case 86:
        setCursor(48, 2);
        println("XOR2");
        setFontSize(FONT_SIZE_SMALL);
        setCursor(48, 5);
        println("74XX86");
        break;
      case 2:
        setCursor(48, 2);
        println("NOR2");
        setFontSize(FONT_SIZE_SMALL);
        setCursor(48, 5);
        println("74XX02");
        break;
      case 266:
        setCursor(44, 2);
        println("XNOR2");
        setFontSize(FONT_SIZE_SMALL);
        setCursor(45, 5);
        println("74XX266");
        break;
      default:
        setCursor(44, 2);
        println("ERROR");
        setFontSize(FONT_SIZE_SMALL);
        setCursor(10, 5);
        println("CI nao identificado");
        break;
    }
    delay(5000);
  }

// LCD SSD1306 Class :: Print Byte
  void LCD_SSD1306::writeDigit(byte n)
  {
    if (m_font == FONT_SIZE_SMALL)
    {
      Wire.beginTransmission(_i2caddr);
      Wire.write(0x40);
      if (n <= 9)
      {
        n += '0' - 0x21;
        for (byte i = 0; i < 5; i++)
          Wire.write(pgm_read_byte(&font5x8[n][i]));
        Wire.write(0);
      }
      else
      {
        for (byte i = 0; i < 6; i++)
          Wire.write(0);
      }
      Wire.endTransmission();
      m_col += 6;
    }
    else if (m_font == FONT_SIZE_MEDIUM)
    {
      write(n <= 9 ? ('0' + n) : ' ');
#ifndef MEMORY_SAVING
    }
    else if (m_font == FONT_SIZE_LARGE)
    {
      if (n <= 9)
      {
        byte i;
        ssd1306_command(0xB0 + m_row);          // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address
        
        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (i = 0; i < 16; i ++)
        {
          byte d = pgm_read_byte(&digits16x16[n][i]);
          Wire.write(d);
          if (m_flags & FLAG_PIXEL_DOUBLE_H)
            Wire.write(d);
        }
        Wire.endTransmission();

        ssd1306_command(0xB0 + m_row + 1);      // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (; i < 32; i ++)
        {
          byte d = pgm_read_byte(&digits16x16[n][i]);
          Wire.write(d);
          if (m_flags & FLAG_PIXEL_DOUBLE_H)
            Wire.write(d);
        }
        Wire.endTransmission();
      }
      else
      {
        ssd1306_command(0xB0 + m_row);          // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--)
          Wire.write(0);
        Wire.endTransmission();

        ssd1306_command(0xB0 + m_row + 1);      // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--)
          Wire.write(0);
        Wire.endTransmission();
      }
      m_col += (m_flags & FLAG_PIXEL_DOUBLE_H) ? 30 : 16;
#endif
    }
    else
    {
      if (n <= 9)
      {
        byte i;
        ssd1306_command(0xB0 + m_row);          // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (i = 0; i < 16; i ++)
        {
          byte d = pgm_read_byte(&digits16x24[n][i * 3]);
          Wire.write(d);
          if (m_flags & FLAG_PIXEL_DOUBLE_H)
            Wire.write(d);
        }
        Wire.endTransmission();

        ssd1306_command(0xB0 + m_row + 1);      // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (i = 0; i < 16; i ++)
        {
          byte d = pgm_read_byte(&digits16x24[n][i * 3 + 1]);
          Wire.write(d);
          if (m_flags & FLAG_PIXEL_DOUBLE_H)
            Wire.write(d);
        }
        Wire.endTransmission();

        ssd1306_command(0xB0 + m_row + 2);      // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (i = 0; i < 16; i ++)
        {
          byte d = pgm_read_byte(&digits16x24[n][i * 3 + 2]);
          Wire.write(d);
          if (m_flags & FLAG_PIXEL_DOUBLE_H)
            Wire.write(d);
        }
        Wire.endTransmission();
      }
      else
      {
        ssd1306_command(0xB0 + m_row);          // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--)
          Wire.write(0);
        Wire.endTransmission();

        ssd1306_command(0xB0 + m_row + 1);      // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--)
          Wire.write(0);
        Wire.endTransmission();

        ssd1306_command(0xB0 + m_row + 2);      // Set page address
        ssd1306_command(m_col & 0xf);           // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));   // Set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--)
          Wire.write(0);
        Wire.endTransmission();
      }
      m_col += (m_flags & FLAG_PIXEL_DOUBLE_H) ? 30 : 16;
    }
  }

// LCD SSD1306 Class :: Draw Image
  void LCD_SSD1306::draw(const PROGMEM byte* buffer, byte width, byte height)
  {
      ssd1306_command(SSD1306_SETLOWCOLUMN  | 0x0);   // Low col = 0
      ssd1306_command(SSD1306_SETHIGHCOLUMN | 0x0);   // Hi  col = 0
      ssd1306_command(SSD1306_SETSTARTLINE  | 0x0);   // Line #0
  
      const PROGMEM byte *p = buffer;
      
      height >>= 3;
      width  >>= 3;
  
      for (byte i = 0; i < height; i++)
      {
        // Send a bunch of data in one xmission
        ssd1306_command(0xB0 + i + m_row);            // Set page address
        ssd1306_command(m_col & 0xf);                 // Set lower column address
        ssd1306_command(0x10 | (m_col >> 4));         // Set higher column address
        
        for(byte j = 0; j < 8; j++)
        {
          Wire.beginTransmission(_i2caddr);
          Wire.write(0x40);
          for (byte k = 0; k < width; k++, p++)
            Wire.write(pgm_read_byte(p));
          Wire.endTransmission();
        }
      }
      m_col += width;
  }

// LCD SSD1306 Class :: Clear Line
  void LCD_SSD1306::clearLine(byte line)
  {
    ssd1306_command(SSD1306_SETLOWCOLUMN | 0x0);    // Low col = 0
    ssd1306_command(SSD1306_SETHIGHCOLUMN | 0x0);   // Hi  col = 0
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);    // Line #0
  
    // Send a bunch of data in one xmission
    ssd1306_command(0xB0 + line);                   // Set page address
    ssd1306_command(0);                             // Set lower column address
    ssd1306_command(0x10);                          // Set higher column address
  
    for(byte j = 0; j < 8; j++)
    {
      Wire.beginTransmission(_i2caddr);
      Wire.write(0x40);
      for (byte k = 0; k < 16; k++)
      {
        Wire.write(0);
      }
      Wire.endTransmission();
    }
  }

// LCD SSD1306 Class :: Clear Display
  void LCD_SSD1306::clear(byte x, byte width, byte height)
  {
    ssd1306_command(SSD1306_SETLOWCOLUMN  | 0x0);   // Low col = 0
    ssd1306_command(SSD1306_SETHIGHCOLUMN | 0x0);   // Hi  col = 0
    ssd1306_command(SSD1306_SETSTARTLINE  | 0x0);   // Line #0
  
    width  >>= 3;
  
    for (byte i = 0; i < height; i++)
    {
      for(byte j = 0; j < 8; j++)
      {
        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        for (byte k = 0; k < width; k++)
            Wire.write(0);
        Wire.endTransmission();
      }
    }
    setCursor(0, 0);
  }

// LCD SSD1306 Class :: Set Contrast
  void LCD_SSD1306::setContrast(byte Contrast)
  {
    ssd1306_command(SSD1306_SETCONTRAST);
    ssd1306_command(Contrast);
  }
