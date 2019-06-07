// Includes & Pre-Processing
  #include "Wire.h"                           // I2C library

  #define MCP23017_addr 0x20                  // MCP23017 I2C module address

// Setup Function
  void setup(void)
  {
     Wire.begin();                            // Start I2C BUS
     
     Wire.beginTransmission(MCP23017_addr);   // Call MCP2307 module on bus 
     Wire.write(0x00);                        // IODIRA register
     Wire.write(0x00);                        // Set all Port A as outputs
     Wire.endTransmission();                  // End communication

     Wire.beginTransmission(MCP23017_addr);   // Call MCP2307 module on bus 
     Wire.write(0x01);                        // IODIRB register
     Wire.write(0x00);                        // Set all Port B as outputs
     Wire.endTransmission();                  // End communication
  }

// Loop Function
  void loop(void)
  {
     Wire.beginTransmission(MCP23017_addr);   // Call MCP2307 module on bus 
     Wire.write(0x12);                        // Address Bank A
     Wire.write((byte)0xAA);                  // Value to write on Bank A
     Wire.endTransmission();                  // End communication
     
     delay(500);

     Wire.beginTransmission(MCP23017_addr);   // Call MCP2307 module on bus 
     Wire.write(0x13);                        // Address Bank B
     Wire.write((byte)0xFF);                  // Value to write on Bank B
     Wire.endTransmission();                  // End communication
     
     delay(1000);

     Wire.beginTransmission(MCP23017_addr);   // Call MCP2307 module on bus 
     Wire.write(0x12);                        // Address Bank A
     Wire.write((byte)0x55);                  // Value to write on Bank A
     Wire.endTransmission();                  // End communication
    
    delay(500);
    
     Wire.beginTransmission(MCP23017_addr);   // Call MCP2307 module on bus 
     Wire.write(0x13);                        // Address Bank B
     Wire.write((byte)0x00);                  // Value to write on Bank B
     Wire.endTransmission();                  // End communication

     delay(1000);
  }
