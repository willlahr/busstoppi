
#include <bcm2835.h>
#include <stdio.h>

#define COLUMN_MAX 25


int main(int argc, char **argv) {
	if (!bcm2835_init()) {
		printf("oops, could not init bcm2835\n");
		return 1;
	}
    
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32768);    // ~ 1 MHz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    
	uint8_t mosi[4]; // one LED line long
    
	uint8_t miso[4];
    int n=0;
    int column = 0; // which column to light up, 0 - 25 for single board
    int count = 0;
   
    
    while(1)
    {
        for(column=0;column<COLUMN_MAX;column++)
        {
            
            unsigned char column_byte = (column/8) ;
            unsigned char column_bit = column% 8 ;
            
            printf("Column %i %i %i\n", column, column_byte, column_bit);
        for(count = 0; count < 100; count++)
        {
            
            int a;
            for(a =0; a<4; a++)
            {
//                if(column_byte == (a/8))
//                {
//                    unsigned char byte_with_one_bit_set = 0x07 << column_bit;
 //                   mosi[a] = byte_with_one_bit_set;
 //               } else {
                    mosi[a] = 0x08;
           //     }
            }
            n++;
            
            bcm2835_spi_transfernb(mosi, miso, 4 ); // one LED line is
                // delay after transaction is finished
            bcm2835_delay(1);
            }
        }
    }
    
    
    
    bcm2835_spi_end();
    bcm2835_close();
    return 0;
}

