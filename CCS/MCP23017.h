#ifndef MCP23017_H_
  #define MCP23017_H_

  #define MCP23017_addr 0x20                  // MCP23017 I2C address

  // MCP23017 Class
  class GPIO
  {
    public:
        void configureIO(unsigned int PADIR, unsigned int PBDIR);
        void regA_setOutputs(char data);
        void regB_setOutputs(char data);
        int  notCase(void);
        int  andCase(void);
        int  orCase(void);
        int  nandCase(void);
        int  norCase(void);
        int  xorCase(void);
        int  xnorCase(void);
  };

#endif /* MCP23017_H_ */
