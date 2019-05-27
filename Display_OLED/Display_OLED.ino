/**********************************************************************************************/
/*                                                                                            */
/*                                                                                            */
/*        Identificador_de_CIs.ino                     Author(s): Bismark Cotrim & Rafael F.  */
/*                                                     Email(s) : bismarkcotrim@hotmail.com   */
/*                                                                goldcard99@hotmail.com      */
/*                                                     Address  : DF, Brasil, 72444-240       */
/*        Created: 2019/05/21 13:54:29 by rFeijo                                              */
/*        Updated: 2019/05/27 13:16:45 by rFeijo                                              */
/*                                                                All Rights Reserved         */
/**********************************************************************************************/

// Includes
  #include "OLED_128x64.h"
  #include "Library.h"
  #include <Wire.h>

// Pre-Processing & Instances
  LCD_SSD1306 oled;

// Print Header on OLED Display
  void ft_printLogo(void)
  {
    oled.clear();
    
    oled.setCursor(40, 0);
    oled.draw(logo, 48, 48);
    
    delay(200);
    
    oled.setFontSize(FONT_SIZE_SMALL);
    oled.setCursor(20, 7);
    oled.println("Testador de CIs");
    
    delay(800);
  }

// Setup Function
  void setup(void)
  {
    oled.begin();
  }

// Loop Function
  void loop(void)
  {
    ft_printLogo();
    
    delay(500);

    oled.printResult(4);
    oled.printResult(8);
    oled.printResult(32);
    oled.printResult(0);
    oled.printResult(86);
    oled.printResult(2);
    oled.printResult(266);
    oled.printResult(10);
  }
