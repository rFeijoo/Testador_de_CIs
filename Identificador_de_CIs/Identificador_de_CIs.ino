/**********************************************************************************************/
/*                                                                                            */
/*                                                                                            */
/*        Identificador_de_CIs.ino                     Author(s): Bismark C. & Rafael F.      */
/*                                                     Email(s) : bismarkcotrim@hotmail.com   */
/*                                                                goldcard99@hotmail.com      */
/*                                                     Address  : DF, Brasil, 72444-240       */
/*        Created: 2019/05/21 13:54:29 by rFeijo                                              */
/*        Updated: 2019/05/27 16:00:05 by rFeijo                                              */
/*                                                                All Rights Reserved         */
/**********************************************************************************************/

// Includes
  #include <msp430g2553.h>
  #include "OLED_128x64.h"
  #include "Library.h"
  #include <Wire.h>

// Pre-Processing & Instances
  LCD_SSD1306 oled;

// Logical Test Function
  void ft_do_logical_test(void)
  {
    uint8_t i = 0, result = 0, aux_P1IN = 0, aux_P2IN = 0;

    while (ci_id[i][0] != 0xFF)                                   // Check for end case
    {
      P1DIR = ci_id[i][2];                                        // Set P1DIR
      P2DIR = ci_id[i][3];                                        // Set P2DIR

      int count = 0, aux = ci_id[i][0];
      while (truth_table[count][0] != aux)                        // Add offset on test_case matrix
        count++;

      for (int j=count; j<(ci_id[i][1] + count); j++)             // Do as many tests as ci_id[i][1] specify
      {
        P1OUT |= truth_table[j][1];                               // Set P1OUT test case
        P2OUT |= truth_table[j][2];                               // Set P2OUT test case

        delay(10);                                                // Delay to set IO's properly

        if ((ci_id[i][0] == 0x04) && (j == 1))                    // NOT test case exception
        {
          aux_P1IN = ~(P1IN);
          aux_P2IN = ~(P2IN);
        }
        if ((P1IN & truth_table[j][3]) || (P2IN & truth_table[j][4]) || ((ci_id[i][0] == 0x04) && (j == 1) && (((aux_P1IN & truth_table[j][3])) || (aux_P2IN & truth_table[j][4]))))
          result++;
  
        P1OUT &= BIT3;                                            // Reset P1OUT (BTN pullup)
        P2OUT &= 0x00;                                            // Reset P2OUT
      }

      if (result == ci_id[i][1])                                  // Check if CI successfully passed all tests
      {
        oled.printResult(ci_id[i][0]);                            // Print result on OLED display
        break;                                                    // Break while loop
      }
      else if (ci_id[i + 1][0] == 0xFF)                           // Check if there's more test cases
        oled.printResult(10);                                     // Print ERROR if there's not
      i++;
    }
  }

// Setup Function
  void setup(void)
  {
    Serial.begin(9600);                     // Start Serial Console
    while (!Serial)                         // Wait until Serial starts
      ;
    
    oled.begin();                           // Start OLED module

    P1OUT  &= 0x00;                         // Reset outputs
    P1DIR   = BIT0 + BIT6;                  // Set P1.0 & P1.6 as outputs
    
    P1REN |= BIT3;                          // Enable pullup/down for P1.3
    P1OUT |= BIT3;                          // Pull Up mode for P1.3

    P1IE  |= BIT3;                          // P1.3 interrupt enabled
    P1IES |= BIT3;                          // P1.3 Hi/Lo edge

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
    
      ft_do_logical_test();                   // Start logical test for identification
    
      P1OUT ^= BIT0;                          // Toggle LED             
    }
  }

// Port1 Interrupt Service Routine
  #pragma vector = PORT1_VECTOR
  __interrupt void Port_1(void)
  {
    P1OUT ^= BIT0;                          // Toggle LED
    
    Serial.print("ISR: PORT1\n");           // Print ISR identification message on Serial Console
  
    P1IFG = 0x00;                           // Interrupt Flag cleared
  }
