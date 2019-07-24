#ifndef I2C_H_
  #define I2C_H_

  void I2C_begin(void);
  void I2C_beginTransmission(volatile int slave_address);
  void I2C_write(unsigned char data, unsigned int txControl);
  void I2C_endTransmission(void);
  void I2C_transmit(unsigned int slave_address, unsigned char data[], unsigned int len);
  void delay(volatile unsigned int t);

#endif /* I2C_H_ */
