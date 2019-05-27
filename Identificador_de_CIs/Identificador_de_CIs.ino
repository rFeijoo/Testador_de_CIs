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
  #include "OLED_128x64.h"
  #include "Library.h"
  #include <Wire.h>

// Pre-Processing & Instances
  LCD_SSD1306 oled;

// Setup Function
  void setup(void)
  {
    oled.begin();                           // Configure OLED module
  }

// Loop Function
  void loop(void)
  {
    oled.printLogo(logo, 48, 48);           // Print header

    oled.printResult(4);                    // Print result for 74XX04 (NOT)
    oled.printResult(8);                    // Print result for 74XX08 (AND2)
    oled.printResult(32);                   // Print result for 74XX32 (OR2)
    oled.printResult(0);                    // Print result for 74XX00 (NAND2)
    oled.printResult(86);                   // Print result for 74XX86 (XOR2)
    oled.printResult(2);                    // Print result for 74XX02 (NOR2)
    oled.printResult(266);                  // Print result for 74XX266 (XNOR2)
    oled.printResult(10);                   // Print result for ERROR - CI not found
  }
