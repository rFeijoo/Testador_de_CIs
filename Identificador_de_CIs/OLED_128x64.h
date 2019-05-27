/**********************************************************************************************/
/*                                                                                            */
/*                                                                                            */
/*        OLED_128x64.h                                Author(s): Bismark C. & Rafael F.      */
/*                                                     Email(s) : bismarkcotrim@hotmail.com   */
/*                                                                goldcard99@hotmail.com      */
/*                                                     Address  : DF, Brasil, 72444-240       */
/*        Created: 2019/05/21 13:54:29 by rFeijo                                              */
/*        Updated: 2019/05/27 16:00:05 by rFeijo                                              */
/*                                                                All Rights Reserved         */
/**********************************************************************************************/

// Includes
  #include "SSD1306.h"

// Pre-Processing & Defines
  typedef enum {
      FONT_SIZE_SMALL = 0,
      FONT_SIZE_MEDIUM,
      FONT_SIZE_LARGE,
      FONT_SIZE_XLARGE
  } FONT_SIZE;

  #define FLAG_PAD_ZERO 1
  #define FLAG_PIXEL_DOUBLE_H 2
  #define FLAG_PIXEL_DOUBLE_V 4
  #define FLAG_PIXEL_DOUBLE (FLAG_PIXEL_DOUBLE_H | FLAG_PIXEL_DOUBLE_V)

// LCD Common Class
  class LCD_Common
  {
    public:
        LCD_Common():m_font(FONT_SIZE_SMALL),m_flags(0) {}
        void setFontSize(FONT_SIZE size) { m_font = size; }
        void setFlags(byte flags) { m_flags = flags; }
        void printInt(uint16_t value, int8_t padding = -1);
        void printLong(uint32_t value, int8_t padding = -1);
        virtual void backlight(bool on) {}
        virtual void draw(const PROGMEM byte* buffer, byte width, byte height) {}
    
    protected:
        virtual void writeDigit(byte n) {}
        byte m_font;
        byte m_flags;
  };

// LCD Null Class
  class LCD_Null : public LCD_Common, public Print
  {
    public:
        void clearLine(byte line) {}
        void clear() {}
        void begin() {}
        void setCursor(byte column, byte line) {}
        byte getLines() { return 0; }
        byte getCols() { return 0; }
        size_t write(uint8_t c) { return 0; }
  };

// LCD SSD1306 Class
  class LCD_SSD1306 : public LCD_Common, public SSD1306, public Print
  {
    public:
        void setCursor(byte column, byte line);
        void setContrast(byte Contrast);
        void printLogo(const PROGMEM byte* buffer, byte width, byte height);
        void draw(const PROGMEM byte* buffer, byte width, byte height);
        void printResult(uint16_t  r);
        void clear(byte x = 0, byte width = 128, byte height = 64);
        void clearLine(byte line);
        byte getLines() { return 21; }
        byte getCols() { return 8; }
        size_t write(uint8_t c);
    
    private:
        void writeDigit(byte n);
        byte m_col;
        byte m_row;
  };
