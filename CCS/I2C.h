#ifndef I2C_H_
  #define I2C_H_

// I2C Class
  class I2C
  {
    public:
        void begin(void);
        void beginTransmission(volatile int slave_address);
        void write(unsigned char data, unsigned int txControl);
        void endTransmission(void);
        void transmit(unsigned int slave_address, unsigned char data[], unsigned int len);
  };

#endif /* I2C_H_ */
