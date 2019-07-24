# Testador_de_CIs
Projeto de um Testador de Circuitos Integrados, capaz de identificar portas lógicas da família TTL, série 74 para o microcontrolador MSP430G.

# Versão
  V1.0 - Display OLED (124x68 - SSD1306 - 0.96") funcionando via I2C bus, apenas no primeiro loop. BUG: interrupção trava após primeira ocorrência;
  V1.1 - Display OLED funcionando via I2C bus, com ativação por interrupção em P1.3;
  V1.2 - Melhorias na função string_typer(), da biblioteca SSD1306.h;
  V1.3 - Criação da função result(), para biblioteca SSD1306.h, contendo as respostas para CIs NOT, AND2, OR2, NAND2, XOR2, NOR2, XNOR2 e caso ERRO;

# Bibliografia
  - https://www.electronicwings.com/ti-launchpad/oled-graphic-display-interfacing-with-msp-exp430g2-ti-launchpad
  - https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
  - https://artofcircuits.com/product/ssd1306-white-0-96-128x64-oled-display-i2c-interface
  - http://www.simplyembedded.org/tutorials/msp430-i2c-basics/
