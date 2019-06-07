/**********************************************************************************************/
/*                                                                                            */
/*                                                                                            */
/*        Identificador_de_CIs.ino                     Author(s): Bismark C. & Rafael F.      */
/*                                                     Email(s) : bismarkcotrim@hotmail.com   */
/*                                                                goldcard99@hotmail.com      */
/*                                                     Address  : DF, Brasil, 72444-240       */
/*        Created: 2019/05/21 13:54:29 by rFeijo                                              */
/*        Updated: 2019/05/31 04:38:38 by rFeijo                                              */
/*                                                                All Rights Reserved         */
/**********************************************************************************************/

// Includes
  #include <msp430g2553.h>
  #include "OLED_128x64.h"
  #include "Library.h"
  #include <Wire.h>

// Pre-Processing & Instances
  LCD_SSD1306 oled;

// Setup Function
  void setup(void)
  {
    Serial.begin(9600);                     // Start Serial Console
    while (!Serial)                         // Wait until Serial starts
      ;
    
    oled.begin();                           // Start OLED module

    P1OUT  &= 0x00;                         // Reset outputs
    P1DIR   = BIT0;                         // Set P1.0 as outputs
    
    P1REN |= BIT3;                          // Enable pullup/down for P1.3
    P1OUT |= BIT3;                          // Pull Up mode for P1.3

    P1IE  |= BIT3;                          // P1.3 interrupt enabled
    P1IES |=~BIT3;                          // P1.3 Hi/Lo edge

    P1IFG |= 0x00;                          // Clear interrupt flag
    
    Serial.print("** CI Identifier **\n\n");// Print start message on Serial Console
    P1OUT |= BIT0;                          // Turn LED ON
  }

// Loop Function
  void loop(void)
  {
    if (P1OUT == 0x08)
    {
      oled.printLogo(logo, 48, 48);           // Print header

      // Logical test will be included here
    
      P1OUT ^= BIT0;                          // Toggle LED             
    }
  }

// Port1 Interrupt Service Routine
  #pragma vector = PORT1_VECTOR
  __interrupt void Port_1(void)
  {
    P1OUT ^= BIT0;                          // Toggle LED
  
    P1IFG = 0x00;                           // Interrupt Flag cleared
  }
