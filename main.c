
#include <bcm2835.h>
#include <stdio.h>


int main(int argc, char **argv) {
	if (!bcm2835_init()) {
		printf("oops, could not init bcm2835\n");
		return 1;
	}
    
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);    // ~ 1 MHz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    
	uint8_t mosi[19]; // one LED line long
    
	uint8_t miso[19];
    int n=0;
    while(1)
    {
        int a;
        for(a =0; a<19; a++)
        {
            mosi[a] = a+(19*n);
        }
        n++;
       
        printf("SPI data sent\n");
        bcm2835_spi_transfernb(mosi, miso, 19 ); // one LED line is
    }
    
    bcm2835_spi_end();
    bcm2835_close();
    return 0;
}

