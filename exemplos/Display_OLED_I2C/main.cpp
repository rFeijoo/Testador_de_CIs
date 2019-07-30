/*
 * P1.6: SCL
 * P1.7: SDA
 */

// Includes
  #include <msp430g2553.h>
  #include "SSD1306.h"
  #include "ASCII.h"
  #include "I2C.h"

// Instances
  SSD1306 ft;
  I2C Wire;

  volatile signed int x = 128, y = 0, x1 = 128, y1 = 0, x2 = 128, y2 = 127;

  void main(void)
  {
    WDTCTL  = WDTPW + WDTHOLD;              // Stop Watchdog Timer

    BCSCTL1 = CALBC1_1MHZ;                  // Set master clock to 1MHz
    DCOCTL  = CALDCO_1MHZ;                  // Set master clock to 1MHz

    P1OUT &= 0x00;                          // Reset outputs
    P1DIR |= 0xFF;                          // P1 port as output
    P1DIR &= BIT0;                          // P1.0 as output

    ft.delay(1000);                         // 1s delay

    P1REN |= BIT3;                          // Enable pull-up/down
    P1OUT |= BIT3;                          // Pull Up mode for P1.3

    P1IES |= BIT3;                          // P1.3 Hi/Lo edge
    P1IE  |= BIT3;                          // P1.3 interrupt enabled

    P1IFG = 0x00;                           // Clear interrupt flag

    Wire.begin();                           // Configure I2C bus
    ft.OLED_begin();                        // Configure OLED Display

    P1OUT ^= BIT0;                          // LED ON

    while (1)
    {
      // CLEAR DISPLAY
      ft.fill_display(lcd_width, lcd_height, 0x00);
      ft.delay(500);

      __bis_SR_register(LPM4_bits + GIE);     // LPM4 mode + Global Interrupt enable

      // SCENE 00 - TYPER INTRO ---------------------------------------
      x = 0;
      ft.scene_00_typer_help(x, 0);
      ft.scene_00_typer_help(x, 4);
      ft.delay(time_ms);
      ft.write_h_string(4, 0, str_scene00_1, 0);
      ft.delay(time_ms);
      ft.write_h_string(17, 0, str_scene00_1, 0);

      ft.delay(2500);
      ft.fill_display(lcd_width, lcd_height, 0x00);
      ft.delay(2000);

      x = lcd_width;
      while (x != -80)
      {
        ft.draw_string_sin(x--, 44, str_scene00_2, 3);
        ft.delay(20);
        ft.clean_area(0, lcd_width, 5, 8);
      }
      ft.delay(750);

      // SCENE 01 - CITY-SCROLLER -------------------------------------
      x = lcd_width;
      y = 0;
      x1= lcd_width;
      y1= 0;
      x2= lcd_width;
      y2= lcd_width - 1;

      while (x >= -400)
      {
        if (x < 1)
        {
          ft.draw_string(x1, y, str_scene01_0);

          if(y1)
            y--;
          else
            y++;

          if(y == 7)
            y1 = 1;

          if(!y)
            y1 = 0;

          x1 -= 2;
        }
        ft.draw_bitmap(x2--, 16, cityb_w, cityb_h, cityb);
        if ((x2 + (signed int)cityb_w) < lcd_width)
          ft.draw_bitmap(y2--, 16, cityb_w, cityb_h, cityb);
        x--;
        ft.delay(15);
        ft.clean_area(0, lcd_width, 0, 1);
      }

      // SCENE 02 - BOUNCE DAT BMP ------------------------------------
      ft.bouncy_bmp(85, 64, 10, baabis_w, baabis_h, baabis);
      ft.delay(2500);

      ft.write_h_string(12, 0, str_scene02_0, 0);
      ft.delay(1000);

      // SCENE 03 - BMP SCROLL OUT + SINE SCROLLER --------------------
      x = 85;
      x1= lcd_width;
      x2= 0;

      while (x != -60)
      {
        ft.draw_bitmap(x--, 0, baabis_w, baabis_h, baabis);
        if(x < 40)
          ft.draw_string_sin_fixed(x1--, 24, str_scene03_0, x2++, 2);

        ft.delay(50);
        ft.clean_area(x1, lcd_width, 3, 5);
      }
      ft.delay(800);
      ft.fill_display(lcd_width, lcd_height, 0x00);

      // SCENE 04 - FIXED SINE STRING + NORMAL SCROLLER ---------------
      x = lcd_width;
      x1= lcd_width;
      y = -(6 * (sizeof(str_scene04_0) - 1));

      while (x > -145)
      {
        if (x < 108)
          ft.draw_string_sin(x1--, 24, str_scene04_1, 2);
        ft.draw_string(x--, 0, str_scene04_0);
        ft.draw_string(y++, 56, str_scene04_0);


        ft.delay(25);
        ft.clean_area(0, lcd_width, 2, 5);
      }
      ft.delay(750);

      // SCENE 05 - OUTRO SINE SCROLLER -------------------------------
      ft.bouncy_bmp(1, 64, 5, smiley1b_w, smiley1b_h, smiley1b);
      ft.draw_bitmap(1, 32, smiley1b_w, smiley1b_h, smiley1b);
      x = lcd_width;
      x1= lcd_width;
      x2= -(6 * (sizeof(str_scene05_0) - 1));
      y = 0;

      while (x1 >= x2)
      {
        if(x >= 40)
        {
          ft.draw_string(x, 32, str_scene05_1);
          ft.draw_string(x, 40, str_scene05_2);
          ft.draw_string(x, 48, str_scene05_3);
          ft.draw_string(x, 56, str_scene05_4);
          x--;
        }
        ft.draw_string_sin_fixed(x1--, 0, str_scene05_0, y++, 2);
        ft.delay(17);
        ft.clean_area(0, lcd_width, 0, 3);
      }
      ft.delay(1000);

      ft.fill_display(lcd_width, lcd_height, 0x00);
      ft.draw_string(24, 24, str_scene05_5);

      ft.delay(2500);

      // END CASE
      P1OUT ^= BIT0;                        // LED ON
    }
}

// Port1 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT ^= BIT0;                          // LED OFF

    LPM4_EXIT;                              // Disable Low Power Mode

    P1IFG = 0x00;                           // P1.3 flag cleared
}
