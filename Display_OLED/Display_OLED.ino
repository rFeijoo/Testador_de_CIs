// Includes
  #include <Arduino.h>
  #include <Wire.h>
  #include "images.h"
  #include "MicroLCD.h"

// Pre-Processing & Instances
  LCD_SSD1306 lcd;

// Setup Function
  void setup(void)
  {
    lcd.begin();
  }

// Loop Function
  void loop(void)
  {
    lcd.clear();
    lcd.setCursor(40, 1);
    lcd.draw(logo, 48, 48);
    delay(1000);
  
    lcd.clear();
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.setCursor(10, 0);
    lcd.println("Hello World!");
    lcd.setFontSize(FONT_SIZE_SMALL);
    lcd.setCursor(20,5);
    lcd.println("Testador de CIs");
    delay(1000);
    
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.setFontSize(FONT_SIZE_SMALL);
//  lcd.printLong(12345678);
//  delay(1000);
//
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.setFontSize(FONT_SIZE_MEDIUM);
//  lcd.printLong(12345678);
//  delay(1000);
//
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.setFontSize(FONT_SIZE_LARGE);
//  lcd.printLong(12345678);
//  delay(1000);

//  //lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.setFontSize(FONT_SIZE_XLARGE);
//  lcd.printLong(12345678);
//  lcd.setCursor(0, 5);
//  lcd.printLong(92371793);
//  delay(1000);
//  lcd.setCursor(0, 0);
//  lcd.setFontSize(FONT_SIZE_LARGE);
//  lcd.println("A");
//  delay(1000);
  }
