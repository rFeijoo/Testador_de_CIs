// Includes
  #include <msp430g2553.h>
  #include "I2C.h"

// Configure I2C BUS
  void I2C::begin(void)
  {
    UCB0CTL1 |= UCSWRST;                    // Enable SW reset
    UCB0CTL0  = UCMST + UCMODE_3 + UCSYNC;  // I2C Master, synchronous mode

    UCB0CTL1 = UCSSEL_2 + UCSWRST;          // Use SMCLK, keep SW reset
    UCB0BR0  = 10;                          // fSCL = SMCLK/10 = 100kHz

    UCB0BR1 = 0;

    P1SEL |= BIT6 + BIT7;                   // Assign I2C pins to USCI_B0
    P1SEL2|= BIT6 + BIT7;                   // Assign I2C pins to USCI_B0

    UCB0CTL1 &= ~UCSWRST;                   // Clear SW reset, resume operation
    //IE2 |= UCB0RXIE + UCB0TXIE;           //Enable RX and TX interrupt
  }

// Begin Transmission to slave via I2C BUS
  void I2C::beginTransmission(volatile int slave_address)
  {
    while(UCB0CTL1 & UCTXSTP)               // Ensure stop condition was sent
        ;

    UCB0I2CSA = slave_address;

    UCB0CTL1 |= UCTR;
    UCB0CTL1 |= UCTXSTT;                    // I2C TX, start condition
  }

// Begin Transmission to slave via I2C BUS
  void I2C::write(unsigned char data, unsigned int txControl)
  {
    UCB0TXBUF = data;

    if (txControl)
    {
      while(UCB0CTL1 & UCTXSTT)
        ;
    }
    else
    {
      while((IFG2 & UCB0TXIFG) == 0)
        ;
    }
  }

// End Transmission on I2C BUS
  void I2C::endTransmission(void)
  {
    UCB0CTL1 |= UCTXSTP;

    while(UCB0CTL1 & UCTXSTP)
      ;
  }

// Transmit data to slave via I2C BUS
  void I2C::transmit(unsigned int slave_address, unsigned char data[], unsigned int len)
  {
    volatile unsigned int i;

    while(UCB0CTL1 & UCTXSTP)               // Ensure stop condition was sent
        ;

    UCB0I2CSA = slave_address;

    UCB0CTL1 |= UCTR;
    UCB0CTL1 |= UCTXSTT;                    // I2C TX, start condition

    if(len == 1)
    {
        UCB0TXBUF = data[0];
        while(UCB0CTL1 & UCTXSTT)
            ;
        UCB0CTL1 |= UCTXSTP;
    }
    else
    {
        UCB0TXBUF = data[0];
        while(UCB0CTL1 & UCTXSTT)
            ;
        for(i=1; i<len; i++)
        {
            UCB0TXBUF = data[i];
            while((IFG2 & UCB0TXIFG) == 0)
                ;
        }
        UCB0CTL1 |= UCTXSTP;
    }
    while(UCB0CTL1 & UCTXSTP)
        ;
  }
