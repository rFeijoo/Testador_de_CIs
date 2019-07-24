// Includes
  #include <msp430g2553.h>
  #include "I2C.cpp"
  #include "I2C.h"

#define MCP23017_addr 0x20

// Main Loop
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT timer

    P1OUT &= 0x00;                          // Reset outputs
    P1DIR  = BIT0;                          // Set P1.0 as outputs

    P1REN |= BIT3;                          // Enable pullup/down for P1.3
    P1OUT |= BIT3;                          // Pull Up mode for P1.3

    P1IE  |= BIT3;                          // P1.3 interrupt enabled
    P1IES |=~BIT3;                          // P1.3 Hi/Lo edge

    P1IFG |= 0x00;                          // Clear interrupt flag

    I2C_begin();                            // Configure I2C BUS

    // Configure MCP23017
    I2C_beginTransmission(MCP23017_addr);   // Call MCP2307 module on bus
    I2C_write(0x00, 1);                     // IODIRA register
    I2C_write(0x00, 0);                     // Set all Port A as outputs
    I2C_endTransmission();                  // End communication

    I2C_beginTransmission(MCP23017_addr);   // Call MCP2307 module on bus
    I2C_write(0x01, 1);                     // IODIRB register
    I2C_write(0x00, 0);                     // Set all Port B as outputs
    I2C_endTransmission();                  // End communication

    P1OUT |= BIT0;                          // Turn LED ON

    while (1)
    {
      _BIS_SR(LPM4_bits + GIE);             // Low Power Mode + Global Interrupt Enable

      I2C_beginTransmission(MCP23017_addr); // Call MCP2307 module on bus
      I2C_write(0x12, 1);                   // Address Bank A
      I2C_write(0xAA, 0);                   // Set output ports
      I2C_endTransmission();                // End communication

      delay(1000);                          // 1s delay

      I2C_beginTransmission(MCP23017_addr); // Call MCP2307 module on bus
      I2C_write(0x12, 1);                   // Address Bank A
      I2C_write(0x55, 0);                   // Set output ports
      I2C_endTransmission();                // End communication

      P1OUT ^= BIT0;                        // Turn LED ON
    }
}

// Port1 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    P1OUT ^= BIT0;                          // Turn LED OFF

    LPM4_EXIT;                              // Disable Low Power Mode

    P1IFG = 0x00;                           // P1.3 flag cleared
}
