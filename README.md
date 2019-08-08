# Testador_de_CIs
Projeto de um Testador de Circuitos Integrados, capaz de identificar portas lógicas da família TTL, série 74 para o microcontrolador MSP430G.

# Versão
  * V1.0 - Display OLED (124x68 - SSD1306 - 0.96") funcionando via I2C bus, apenas no primeiro loop. BUG: interrupção trava após primeira ocorrência;
  * V1.1 - Display OLED funcionando via I2C bus, com ativação por interrupção em P1.3;
  * V1.2 - Melhorias na função string_typer(), da biblioteca SSD1306.h;
  * V1.3 - Criação da função result(), para biblioteca SSD1306.h, contendo as respostas para CIs NOT, AND2, OR2, NAND2, XOR2, NOR2, XNOR2 e caso ERRO;
  * V2.0 - Implementação do módulo MCP23017, expansor IO I2C;
  * V2.1 - Criação das funções lógicas para identificação dos CIs: NOT, AND, OR, NAND, NOR, XOR, XNOR e ERROR;
  * V2.2 - Apresentação do resultado no display OLED;
  * V3.0 - [TODO]Implementar arquitetura para leitura através do módulo I2C MCP23017;
  * V3.1 - [TODO]Implementar lógica para definição do CI;

# Bibliografia
  - https://www.electronicwings.com/ti-launchpad/oled-graphic-display-interfacing-with-msp-exp430g2-ti-launchpad
  - https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
  - https://artofcircuits.com/product/ssd1306-white-0-96-128x64-oled-display-i2c-interface
  - http://www.simplyembedded.org/tutorials/msp430-i2c-basics/
