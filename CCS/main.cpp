/*
 * P1.6: SCL
 * P1.7: SDA
 */

// Includes
  #include <msp430g2553.h>
  #include "MCP23017.h"
  #include "SSD1306.h"
  #include "ASCII.h"
  #include "I2C.h"

// Instances
  SSD1306   oled;
  GPIO      gpio;
  I2C       Wire;

  void main(void)
  {
    WDTCTL  = WDTPW + WDTHOLD;              // Stop WDT timer

    BCSCTL1 = CALBC1_1MHZ;                  // Set master clock to 1MHz
    DCOCTL  = CALDCO_1MHZ;                  // Set master clock to 1MHz

    P1OUT &= 0x00;                          // Reset outputs
    P1DIR |= 0xFF;                          // P1 port as output
    P1DIR &= BIT0;                          // P1.0 as output

    oled.delay(1000);                       // 1s delay

    P1REN |= BIT3;                          // Enable pull-up/down
    P1OUT |= BIT3;                          // Pull Up mode for P1.3

    P1IES |= BIT3;                          // P1.3 Hi/Lo edge
    P1IE  |= BIT3;                          // P1.3 interrupt enabled

    P1IFG = 0x00;                           // Clear interrupt flag

    Wire.begin();                           // Configure I2C  BUS
    oled.begin();                           // Configure OLED Display

    oled.draw_bitmap(40, 8, 48, 48, logo);  // Print logo image
    oled.delay(2500);                       // 2,5s delay
    oled.fill_display(128, 64, 0x00);       // Clear display
    oled.string_typer(6, 4, logo_t, 0, 50); // Print logo text
    oled.delay(2000);                       // 2,5s delay

    P1OUT ^= BIT0;                          // LED ON

    while (1)
    {
      // CLEAR DISPLAY
      oled.fill_display(128, 64, 0x00);
      oled.delay(500);

      __bis_SR_register(LPM4_bits + GIE);   // LPM4 mode + Global Interrupt enable

      // WAKEUP DISPLAY
      oled.draw_bitmap(40, 8, 48, 48, logo);// Print logo on OLED
      oled.delay(1500);                     // 1,5s delay
      oled.fill_display(128, 64, 0x00);     // Clear display

      // LOGICAL TEST
      if (gpio.notCase())
      {
        oled.write_string(9, 3, "NOT", 0);
        oled.write_string(8, 5, "74X04", 0);
      }
      else if (gpio.andCase())
      {
        oled.write_string(9, 3, "AND", 0);
        oled.write_string(8, 5, "74X08", 0);
      }
      else if (gpio.orCase())
      {
        oled.write_string(10, 3, "OR", 0);
        oled.write_string(8, 5, "74XX32", 0);
      }
      else if (gpio.nandCase())
      {
        oled.write_string(9, 3, "NAND", 0);
        oled.write_string(8, 5, "74XX00", 0);
      }
      else if (gpio.norCase())
      {
        oled.write_string(9, 3, "NOR", 0);
        oled.write_string(8, 5, "74X02", 0);
      }
      else if (gpio.xorCase())
      {
        oled.write_string(9, 3, "XOR", 0);
        oled.write_string(8, 5, "74X86", 0);
      }
      else if (gpio.xnorCase())
      {
        oled.write_string(9, 3, "XNOR", 0);
        oled.write_string(8, 5, "74X266", 0);
      }
      else
      {
        oled.write_string(8, 3, "ERROR", 0);
        oled.write_string(4, 5, "IC not found!", 0);
      }

      oled.delay(3000);

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
