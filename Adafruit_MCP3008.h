/* modified to use BCM2835 on Raspberry Pi by Marco Duarte */

/***************************************************
  This is an Arduino Library for the MCP3008 - 8-Channel 10-Bit ADC
  With SPI Interface.
  ----> http://www.adafruit.com/products/856

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Copyright (c) 2017, Adafruit Industries (adafruit.com)
  MIT License (https://opensource.org/licenses/MIT)
****************************************************/
#ifndef Adafruit_MCP3008_h
#define Adafruit_MCP3008_h

#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>

#define MCP3008_SPI_MAX_5V  3600000
#define MCP3008_SPI_MAX_3V  1350000
#define MCP3008_SPI_MAX     MCP3008_SPI_MAX_3V
#define MCP3008_SPI_ORDER   MSBFIRST
#define MCP3008_SPI_MODE    SPI_MODE0
#define MCP3008_SPI_0        0
#define MCP3008_SPI_AUX      1 //SPI1

class Adafruit_MCP3008 {
  public:
    bool begin(uint8_t spiModule=MCP3008_SPI_AUX, uint8_t cs=0);
    int readADC(uint8_t channel);
    int readADCDifference(uint8_t differential);
  private:
    uint8_t spiModule;
    uint8_t cs;
    int SPIxADC(uint8_t channel, bool differential);
};

#endif
