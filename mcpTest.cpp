#include <bcm2835.h>
#include <unistd.h>
#include "Adafruit_MCP3008.h"

int main()
{
  Adafruit_MCP3008 adc;
  
  if (!bcm2835_init())
  {
    printf("bcm2835_init failed. Are you running as root??\n");
    return 1;
  }
  
  //change tp bcm2835_spi_begin() if using SPI0
  if (!bcm2835_aux_spi_begin())
  {
    printf("bcm2835_spi_begin failed. Are you running as root??\n");
    return 1;
  }
  
  //using SPI1 and CS2, uncomment for SPI0 and CS0
  if(!adc.begin(/*MCP3008_SPI_0, BCM2835_SPI_CS0*/))
    return 1;
  
  while(1)
  {
	  //read channel 0
    int val = adc.readADC(0);
    printf("Channel 0 = %d\n", val);
	sleep(1);
  }
}
