#ifndef SSD1306_H_
  #define SSD1306_H_

  #define OLED_addr     0x3C                  // OLED I2C address

  #define lcd_width     128
  #define lcd_height    64
  #define lcd_height_b  8

  #define f_width       5
  #define space_char    1                     // Space between chars
  #define max_f_size    8                     // Max. font cache size for resize function

  unsigned const char time_ms = 50;

  unsigned char byte, bit_num;                // Global buffer and bit index
  unsigned int h_index, w_index;              // Global width_index and height_index

  const char init_cmd_array[]={0xAE,          // DISPLAY OFF
                               0xD5,          // SET OSC FREQUENY
                               0x80,          // divide ratio = 1 (bit 3-0), OSC (bit 7-4)
                               0xA8,          // SET MUX RATIO
                               0x3F,          // 64MUX
                               0xD3,          // SET DISPLAY OFFSET
                               0x00,          // offset = 0
                               0x40,          // set display start line, start line = 0
                               0x8D,          // ENABLE CHARGE PUMP REGULATOR
                               0x14,          //
                               0x20,          // SET MEMORY ADDRESSING MODE
                               0x02,          // horizontal addressing mode
                               0xA1,          // set segment re-map, column address 127 is mapped to SEG0
                               0xC8,          // set COM/Output scan direction, remapped mode (COM[N-1] to COM0)
                               0xDA,          // SET COM PINS HARDWARE CONFIGURATION
                               0x12,          // alternative COM pin configuration
                               0x81,          // SET CONTRAST CONTROL
                               0xCF,          //
                               0xD9,          // SET PRE CHARGE PERIOD
                               0xF1,          //
                               0xDB,          // SET V_COMH DESELECT LEVEL
                               0x40,          //
                               0xA4,          // DISABLE ENTIRE DISPLAY ON
                               0xA6,          // NORMAL MODE (A7 for inverse display)
                               0xAF};         // DISPLAY ON

  const char sine_array[]={15,16,17,18,19,20,21,22,23,24,25,26,27,28,28,29,29,30,30,30,30,30,
                           29,29,28,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,
                           8,7,6,5,4,3,3,2,2,1,1,0,0,0,0,0,1,1,2,2,3,3,4,5,6,7,8,9,10,11,12,13,14};

  const char s_size = sizeof(sine_array);

  // SSD1306 Class
  class SSD1306
  {
    public:
        void delay(volatile unsigned int t);
        void begin(void);
        void bouncy_bmp(signed int x, unsigned int f_height, unsigned int time_ms, unsigned int b_width, unsigned int b_height,const char *bmap);
        void fill_display(unsigned char width, unsigned char height, unsigned char byte);
        void set_cursor(unsigned char x, unsigned char y);
        void clean_area(unsigned char x_start, unsigned char x_end, unsigned char y_start, unsigned char y_end);
        void draw_string_sin_fixed(signed int x, signed int y, const char *string, unsigned int x_s, unsigned char div);
        void draw_string_sin(signed int x, signed int y, const char *string, unsigned char div);
        void draw_string(signed int x, signed int y, const char *string);
        unsigned int get_sin(signed int x);
        unsigned int get_abs(signed int x);
        void draw_bitmap(signed int x, signed int y, unsigned int b_width, unsigned int b_height, const char *bmap);
        void convert_font_size(unsigned char x, unsigned char y, unsigned char character, unsigned char f_size);
        void write_string(unsigned char x, unsigned char y, const char *string, unsigned int f_size);
        void write_h_string(unsigned char x, unsigned char y, const char *string, unsigned char f_size);
        void send_data_array(const char *d_array, unsigned char size);
        void write_char(unsigned char x, unsigned char y, unsigned char character, unsigned char f_size);
        void string_typer(unsigned char x, unsigned char y, const char *string, unsigned char f_size, unsigned int ms);
  };

#endif /* SSD1306_H_ */

