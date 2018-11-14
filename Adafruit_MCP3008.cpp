/* modified to use BCM2835 on Raspberry Pi by Marco Duarte*/

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
#include "Adafruit_MCP3008.h"

// Initialize for hardware SPI
bool Adafruit_MCP3008::begin(uint8_t spiModule, uint8_t cs) {

  this->cs = cs;
  this->spiModule = spiModule;
  
  if(this->spiModule == MCP3008_SPI_AUX)
  {
    uint16_t clockDivider = bcm2835_aux_spi_CalcClockDivider(100000);
    bcm2835_aux_spi_setClockDivider(clockDivider);
  }
  else if(this->spiModule == MCP3008_SPI_0)
  {
    if(this->cs != BCM2835_SPI_CS0 && this->cs != BCM2835_SPI_CS1)
    {
      printf("Choose BCM2835_SPI_CS0 or BCM2835_SPI_CS1\n");
      return false;
    }
    
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4096);
    bcm2835_spi_chipSelect(this->cs);
    bcm2835_spi_setChipSelectPolarity(this->cs, LOW);
  }
  else
  {
    printf("Choose MCP3008_SPI_AUX or MCP3008_SPI_0\n");
    return false;
  }
  
  return true;
}

// Read single ended ADC channel.
int Adafruit_MCP3008::readADC(uint8_t channel) {
  if ((channel < 0) || (channel > 7)) return -1;
  return SPIxADC(channel, false);
}

// Read differential ADC channel.
//      0: Return channel 0 minus channel 1
//      1: Return channel 1 minus channel 0
//      2: Return channel 2 minus channel 3
//      3: Return channel 3 minus channel 2
//      4: Return channel 4 minus channel 5
//      5: Return channel 5 minus channel 4
//      6: Return channel 6 minus channel 7
//      7: Return channel 7 minus channel 6
int Adafruit_MCP3008::readADCDifference(uint8_t differential) {
  if ((differential < 0) || (differential > 7)) return -1;
  return SPIxADC(differential, true);
}

// SPI transfer for ADC read
int Adafruit_MCP3008::SPIxADC(uint8_t channel, bool differential) {
  char command, sgldiff;

  bcm2835_spi_chipSelect(this->cs);
  if (differential) {
    sgldiff = 0;
  } else {
    sgldiff = 1;
  }

  command = ((0x01 << 7) |                 // start bit
             (sgldiff << 6) |              // single or differential
             ((channel & 0x07) << 3) );    // channel number

  char buff[3] = {command, 0x00, 0x00};

  if(spiModule == MCP3008_SPI_AUX)
    bcm2835_aux_spi_transfern(buff, sizeof(buff));
  else
    bcm2835_spi_transfern(buff, sizeof(buff));

  return 0x3FF & ((buff[0] & 0x01) << 9 |
                  (buff[1] & 0xFF) << 1 |
                  (buff[2] & 0x80) >> 7 );
}
