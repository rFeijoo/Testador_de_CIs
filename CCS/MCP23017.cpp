// Includes
  #include <msp430g2553.h>
  #include "MCP23017.h"
  #include "SSD1306.h"
  #include "I2C.h"

// Instances
  SSD1306   oled;
  I2C       Wire;

/****************************************************************************************************************************/
  void GPIO::configureIO(unsigned int PADIR, unsigned int PBDIR)
  {
    Wire.beginTransmission(MCP23017_addr);       // Call MCP2307 module on BUS
    Wire.write(0x00, 1);                         // IODIRA register
    Wire.write(PADIR, 0);                        // Set register A IO direction
    Wire.endTransmission();                      // End communication

    Wire.beginTransmission(MCP23017_addr);       // Call MCP2307 module on bus
    Wire.write(0x01, 1);                         // IODIRB register
    Wire.write(PBDIR, 0);                        // Set register B IO direction
    Wire.endTransmission();                      // End communication
  }

/****************************************************************************************************************************/
  void GPIO::regA_setOutputs(char data)
  {
    Wire.beginTransmission(MCP23017_addr);       // Call MCP2307 module on BUS
    Wire.write(0x12, 1);                         // Address bank A
    Wire.write(data, 0);                         // Set output ports
    Wire.endTransmission();                      // End communication
  }

/****************************************************************************************************************************/
  void GPIO::regB_setOutputs(char data)
  {
    Wire.beginTransmission(MCP23017_addr);       // Call MCP2307 module on BUS
    Wire.write(0x13, 1);                         // Address bank B
    Wire.write(data, 0);                         // Set output ports
    Wire.endTransmission();                      // End communication
  }

/****************************************************************************************************************************/
  int GPIO::notCase(void)
  {
    configureIO(0xEA, 0xEA);                     // PADIR = 11101010 & PBDIR = 11101010, where 0's output (Big Endian)

    // Send 0 -> Receive 1
    regA_setOutputs(0x00);
    regB_setOutputs(0x00);
    oled.delay(5);
      // read inputs

    // Send 1 -> Receive 0
    regA_setOutputs(0x15);
    regB_setOutputs(0x15);
    oled.delay(5);
      // read inputs

    return (0);
  }

/****************************************************************************************************************************/
  int GPIO::andCase(void)
  {
    configureIO(0xE4, 0xE4);                     // PADIR = 11001001 & PBDIR = 11001001, where 0's output (Big Endian)

    // Send 11 -> Receive 1
    regA_setOutputs(0x1B);
    regB_setOutputs(0x1B);
    oled.delay(5);
      // read inputs

    // Send 10 -> Receive 0
    regA_setOutputs(0x09);
    regB_setOutputs(0x09);
    oled.delay(5);
      // read inputs

    // Send 01 -> Receive 0
    regA_setOutputs(0x12);
    regB_setOutputs(0x12);
    oled.delay(5);
      // read inputs

    // Send 00 -> Receive 0
    regA_setOutputs(0x00);
    regB_setOutputs(0x00);
    oled.delay(5);
      // read inputs

    return (0);
  }

/****************************************************************************************************************************/
  int GPIO::orCase(void)
  {
    configureIO(0xE4, 0xE4);                     // PADIR = 11001001 & PBDIR = 11001001, where 0's output (Big Endian)

    // Send 00 -> Receive 0
    regA_setOutputs(0x00);
    regB_setOutputs(0x00);
    oled.delay(5);
      // read inputs

    // Send 10 -> Receive 1
    regA_setOutputs(0x09);
    regB_setOutputs(0x09);
    oled.delay(5);
      // read inputs

    // Send 01 -> Receive 1
    regA_setOutputs(0x12);
    regB_setOutputs(0x12);
    oled.delay(5);
      // read inputs

    // Send 11 -> Receive 1
    regA_setOutputs(0x1B);
    regB_setOutputs(0x1B);
    oled.delay(5);
      // read inputs

    return (0);
  }

/****************************************************************************************************************************/
  int GPIO::nandCase(void)
  {
    configureIO(0xE4, 0xE4);                     // PADIR = 11001001 & PBDIR = 11001001, where 0's output (Big Endian)

    // Send 11 -> Receive 0
    regA_setOutputs(0x1B);
    regB_setOutputs(0x1B);
    oled.delay(5);
      // read inputs

    // Send 10 -> Receive 1
    regA_setOutputs(0x09);
    regB_setOutputs(0x09);
    oled.delay(5);
      // read inputs

    // Send 01 -> Receive 1
    regA_setOutputs(0x12);
    regB_setOutputs(0x12);
    oled.delay(5);
      // read inputs

    // Send 00 -> Receive 1
    regA_setOutputs(0x00);
    regB_setOutputs(0x00);
    oled.delay(5);
      // read inputs

    return (0);
  }

/****************************************************************************************************************************/
  int GPIO::norCase(void)
  {
    configureIO(0xC9, 0xC9);                     // PADIR = 11001001 & PBDIR = 11001001, where 0's output (Big Endian)

    // Send 11 -> Receive 1
    regA_setOutputs(0x36);
    regB_setOutputs(0x36);
    oled.delay(5);
      // read inputs

    // Send 10 -> Receive 0
    regA_setOutputs(0x12);
    regB_setOutputs(0x12);
    oled.delay(5);
      // read inputs

    // Send 01 -> Receive 0
    regA_setOutputs(0x24);
    regB_setOutputs(0x24);
    oled.delay(5);
      // read inputs

    // Send 00 -> Receive 0
    regA_setOutputs(0x00);
    regB_setOutputs(0x00);
    oled.delay(5);
      // read inputs

    return (0);
  }

/****************************************************************************************************************************/
  int GPIO::xorCase(void)
  {
    configureIO(0xE4, 0xE4);                     // PADIR = 11001001 & PBDIR = 11001001, where 0's output (Big Endian)

    // Send 11 -> Receive 0
    regA_setOutputs(0x1B);
    regB_setOutputs(0x1B);
    oled.delay(5);
      // read inputs

    // Send 10 -> Receive 1
    regA_setOutputs(0x09);
    regB_setOutputs(0x09);
    oled.delay(5);
      // read inputs

    // Send 01 -> Receive 1
    regA_setOutputs(0x12);
    regB_setOutputs(0x12);
    oled.delay(5);
      // read inputs

    // Send 00 -> Receive 0
    regA_setOutputs(0x00);
    regB_setOutputs(0x00);
    oled.delay(5);
      // read inputs

    return (0);
  }

/****************************************************************************************************************************/
  int GPIO::xnorCase(void)
  {
    configureIO(0xCC, 0xCC);                     // PADIR = 11001100 & PBDIR = 11001100, where 0's output (Big Endian)

    // Send 11 -> Receive 1
    regA_setOutputs(0x33);
    regB_setOutputs(0x33);
    oled.delay(5);
      // read inputs

    // Send 10 -> Receive 0
    regA_setOutputs(0x21);
    regB_setOutputs(0x21);
    oled.delay(5);
      // read inputs

    // Send 01 -> Receive 0
    regA_setOutputs(0x12);
    regB_setOutputs(0x12);
    oled.delay(5);
      // read inputs

    // Send 00 -> Receive 1
    regA_setOutputs(0x00);
    regB_setOutputs(0x00);
    oled.delay(5);
      // read inputs

    return (0);
  }
