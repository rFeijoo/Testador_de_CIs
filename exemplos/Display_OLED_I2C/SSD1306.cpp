// Includes
  #include <msp430g2553.h>
  #include "SSD1306.h"
  #include "ASCII.h"
  #include "I2C.h"

// Instances
  I2C Wire;

/****************************************************************************************************************************/
void SSD1306::delay(volatile unsigned int t)
  {
    TACTL  = TASSEL_2 + ID_0 + MC_1;    // MCLK (1MHz) + 1MHz/1 + Up Mode
    TACCR0 = 1000 - 1;                  // 1MHz / 1000 = 1KHz (1ms)
    TACTL |= TACLR;                     // Clear counter

    while (t--)
    {
      while ((TACTL & TAIFG) == 0)
        ;
      TACTL &= ~TAIFG;
    }
    TACTL = MC_0;
  }

/****************************************************************************************************************************/
  void SSD1306::set_cursor(unsigned char x, unsigned char y)
  {
    Wire.beginTransmission(OLED_addr);
    Wire.write(0x00, 1);
    Wire.write(0x0F & x, 0);            // Set lower nibble of the column start address
    Wire.endTransmission();

    delay(10);                          // 10ms delay

    Wire.beginTransmission(OLED_addr);
    Wire.write(0x00, 1);
    Wire.write(0x10 + (x >> 4), 0);      // Set higher nibble of the column start address
    Wire.endTransmission();

    delay(10);                          // 10ms delay

    Wire.beginTransmission(OLED_addr);
    Wire.write(0x00, 1);
    Wire.write(0xB0 + y, 0);
    Wire.endTransmission();

    delay(10);                          // 10ms delay
  }

/****************************************************************************************************************************/
  void SSD1306::fill_display(unsigned char width, unsigned char height, unsigned char byte)
  {
    height /= 8;

    while (height--)
    {
      set_cursor(0, height);
      Wire.beginTransmission(OLED_addr);
      Wire.write(0x40, 1);
      while (width--)
          Wire.write(byte, 0);
      Wire.endTransmission();
      width = lcd_width;
    }
  }

/****************************************************************************************************************************/
  void SSD1306::OLED_begin(void)
  {
    unsigned char byte = sizeof(init_cmd_array);

    delay(100);

    Wire.beginTransmission(OLED_addr);
    Wire.write(0x00, 1);
    while (byte)
    {
      Wire.write(init_cmd_array[sizeof(init_cmd_array) - byte], 0);
      byte--;
    }
    Wire.endTransmission();

    fill_display(lcd_width, lcd_height, 0x00);
  }

/****************************************************************************************************************************/
  void SSD1306::clean_area(unsigned char x_start, unsigned char x_end, unsigned char y_start, unsigned char y_end)
  {
    byte = x_start;

    while (y_start <= y_end)
    {
      set_cursor(x_start, y_start++);
      Wire.beginTransmission(OLED_addr);
      Wire.write(0x40, 1);
      while (x_start++ <= x_end)
        Wire.write(0, 0);
      Wire.endTransmission();
      x_start = byte;
    }
  }

/****************************************************************************************************************************/
  void SSD1306::draw_string_sin_fixed(signed int x, signed int y, const char *string, unsigned int x_s, unsigned char div)
  {
    while ((*string != 0) && (x < lcd_width))
    {
      if (x >= -(signed int)f_width - 1)
        draw_bitmap(x, y + get_sin(x_s) / div, f_width, 8, ascii_table[*string]);
      *string++;
      x   += f_width + space_char;
      x_s += f_width + space_char;
    }
  }

/****************************************************************************************************************************/
  void SSD1306::draw_string_sin(signed int x, signed int y, const char *string, unsigned char div)
  {
    while((*string != 0) && (x < lcd_width))
    {
      if(x >= -(signed int)f_width - 1)   // Part of char inside display
        draw_bitmap(x, y + get_sin(x) / div, f_width, 8, ascii_table[*string]);
      *string++;
      x += f_width + space_char;
    }
  }

/****************************************************************************************************************************/
  unsigned int SSD1306::get_sin(signed int x)
  {
    if (x < 0)
      return sine_array[s_size - (get_abs(x) % s_size)];
    else
      return sine_array[get_abs(x) % s_size];
  }

/****************************************************************************************************************************/
  unsigned int SSD1306::get_abs(signed int x)
  {
    if (x < 0)
      x *= -1;

    return x;
  }

/****************************************************************************************************************************/
  void SSD1306::draw_bitmap(signed int x, signed int y, unsigned int b_width, unsigned int b_height, const char *bmap)
  {
    unsigned int width_max = 0, x_off = 0, y_off = 0;

    h_index = 0;
    w_index = 0;

    byte = 0;

    if ((y + (signed int)b_height) < 0)         // Outside display
      return;
    if ((x + (signed int)b_width) < 0)
      return;
    if (y >= lcd_height)
      return;
    if (x >= lcd_width)
      return;

    if((x + (signed int)b_width) >= lcd_width)  // Width overflow check
    {
      if(x < 0)
        width_max = (x * -1) + lcd_width;       // w_index will be set to x * (-1)
      else
        width_max = lcd_width - x;
    }
    else
      width_max = b_width;

    if (x < 0)
    {
      w_index = x * -1;                         // Set bmap x - offset
      x_off = w_index;
      x = 0;                                    // Start at display position x = 0 with x - offset
    }

    if (b_height % 8)
      b_height += 8;

    b_height /= 8;

    if (y < 0)
    {
      y *= -1;
      y_off = y / 8;
      b_height -= y_off;
      while (h_index < b_height)
      {
        set_cursor(x, h_index);
        Wire.beginTransmission(OLED_addr);
        Wire.write(0x40, 1);
        while(w_index < width_max)
        {
          byte = bmap[w_index + ((h_index + y_off) * b_width)]>>(y % 8);
          if((h_index + 1) != b_height)
            byte |= bmap[w_index + ((h_index + y_off + 1) * b_width)]<<(8 - (y % 8));
          Wire.write(byte, 0);
          w_index++;
          byte = 0;
        }
        Wire.endTransmission();
        h_index++;
        w_index = x_off;
        if(h_index >= lcd_height_b)
          return;
      }
    }
    else
    {
      y_off = y / 8;
      while (h_index <= b_height)
      {
        set_cursor(x, y_off + h_index);
        Wire.beginTransmission(OLED_addr);
        Wire.write(0x40, 1);
        while (w_index < width_max)
        {
          if (h_index != b_height)
            byte = bmap[w_index + (h_index * b_width)]<<(y % 8);
          if(h_index)
            byte |= (bmap[w_index + ((h_index - 1) * b_width)]>>(8 - (y % 8)));
          Wire.write(byte, 0);
          w_index++;
          byte = 0;
        }
        Wire.endTransmission();
        h_index++;
        w_index = x_off;
        if((h_index + y_off) >= lcd_height_b)
          return;
        if(h_index == b_height && !(y % 8))
          return;
        }
    }
  }

/****************************************************************************************************************************/
/* Horizontal resize with cache (every bit in one byte will be resized to f_size, for example:
** f_size = 2, every pixel will have a new size of 2*2 pixel; f_size = 3, pixel-size = 3*3 pixel, etc.
** f_size = 1, double height font, only horizontal duplication of pixels (bits), pixel-size: 1*2 pixel
** the vertical duplication is running over a cache (every horizontal row (resized byte to f_size),
** will be write "f_size" - times to the display)*/
  void SSD1306::convert_font_size(unsigned char x, unsigned char y, unsigned char character, unsigned char f_size)
  {
    unsigned char x_char = 0, bit_num_b = 0, size = 0, px_size = 0, y_pos_new = 0, x_pos_new = 0;
    unsigned char cache[max_f_size],i = 0;

    byte = 0;
    bit_num = 0;

    if (f_size == 1)
      size = 2;
    else
      size = f_size;

    while (x_char < f_width)                                                // Test byte, starting at 0 to f_width (font width)
    {
      while (bit_num < 8)                                                   // Test bit 0..7 of current byte
      {
        Wire.beginTransmission(OLED_addr);
        Wire.write(0x40, 1);
        if (ascii_table[character][x_char] & (1 << bit_num))                // If bit=1 in byte...
        {
          while (px_size < size)                                            // Duplicate bits (f_size * f_size)
          {
            if ((bit_num_b > 7) && (px_size > 0))                           // Byte overflow, new byte
            {
              set_cursor(x + x_pos_new, y + y_pos_new++);                   // Set cursor (increment y - new position)
              Wire.write(byte, 0);                                          // Send byte
              bit_num_b = 0;                                                // Reset bit counter (buffer)
              cache[i++] = byte;                                            // Save byte in cache
              byte = 0;                                                     // Reset byte
            }
            byte |= (1 << bit_num_b);                                       // Set bit in byte
            px_size++;                                                      // Increment pixel duplicate counter
            bit_num_b++;                                                    // Increment bit position for buffer
          }
          px_size = 0;                                                      // Reset pixel duplicate counter
        }
        else
          bit_num_b += size;                                                // Bit = 0, calculate new bit position in byte
                                                                            // If bit = 0, remaining bits are 0, too
        if (bit_num_b > 7)                                                  // Byte overflow, new byte
        {
          set_cursor(x + x_pos_new, y + y_pos_new++);
          Wire.write(byte, 0);
          bit_num_b -= 8;
          cache[i++] =byte;
          byte = 0;
        }
        Wire.endTransmission();
        bit_num++;                                                          // Test next byte in array
      }

      y_pos_new = 0;                                                        // Reset y-offset
      x_pos_new++;                                                          // Increment x-position

      i = 0;                                                                // Reset cache counter

      if (f_size == 1)                                                      // Double height font (only for f_size = 1)
        size = 0;
      else                                                                  // First row is ready, only size - 1
        size--;

      while (size--)
      {
        Wire.beginTransmission(OLED_addr);
        Wire.write(0x40, 1);
        while (i < f_size)
        {
          set_cursor(x + x_pos_new, y + y_pos_new++);
          Wire.write(cache[i++], 0);                                        // Horizontal cache write
        }
        Wire.endTransmission();
        i = 0;
        y_pos_new = 0;
        x_pos_new++;
      }
      x_char++;                                                             // Increment byte position
      if (f_size == 1)                                                      // Size correction
        size = 2;
      else
        size = f_size;
      i = 0;                                                                // Reset cache counter
      bit_num = 0;
    }
  }

/****************************************************************************************************************************/
  void SSD1306::write_string(unsigned char x, unsigned char y, const char *string, unsigned char f_size)
  {
    x *= (f_width + space_char);

    set_cursor(x,y);

    while (*string != 0)
    {
      if(f_size)                                                            // Font converter enabled
      {
        convert_font_size(x, y, *string++, f_size);
        x += (f_size * f_width + space_char);                               // New x-position
      }
      else
      {
        send_data_array(ascii_table[*string++], f_width);
        y = space_char;                                                     // Space between chars
        Wire.beginTransmission(OLED_addr);
        Wire.write(0x40, 1);
        while (y--)
          Wire.write(0x00, 0);
        Wire.endTransmission();
      }
    }
  }

/****************************************************************************************************************************/
  void SSD1306::write_h_string(unsigned char x, unsigned char y, const char *string, unsigned char f_size)
  {
    while(*string != 0)
    {
      write_char(x, y, *string++, f_size);

      if (f_size == 1)
        y += f_size + 1;
      else if (f_size > 1)
        y += f_size;
      else
        y++;
    }
  }

/****************************************************************************************************************************/
  void SSD1306::write_char(unsigned char x, unsigned char y, unsigned char character, unsigned char f_size)
  {
    x *= (f_width + space_char);

    set_cursor(x, y);

    if (f_size)
      convert_font_size(x, y, character, f_size);
    else
      send_data_array(ascii_table[character], f_width);
  }

/****************************************************************************************************************************/
  void SSD1306::send_data_array(const char *d_array, unsigned char size)
  {
    Wire.beginTransmission(OLED_addr);
    Wire.write(0x40, 1);
    while (size--)
      Wire.write(*d_array++, 0);
    Wire.endTransmission();
  }

/****************************************************************************************************************************/
  void SSD1306::string_typer(unsigned char x, unsigned char y, const char *string, unsigned char f_size, unsigned int ms)
  {
    while (*string != 0)
    {
      write_char(x, y, *string++, f_size);
      if (f_size > 1)
        x += f_size;
      else
        x++;
      delay(ms);
    }
  }

/****************************************************************************************************************************/
  void SSD1306::scene_00_typer_help(unsigned char x, unsigned char y_pos)
  {
    while(x < 3)
    {
      string_typer(5 + (x * (sizeof(str_scene00_0) - 1)), y_pos, str_scene00_0, 0, time_ms);
      x++;
    }
    x = 0;
  }
