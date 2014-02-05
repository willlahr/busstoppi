
#include <bcm2835.h>
#include <stdio.h>

#define COLUMN_MAX 65
#define LINES 7
#define BYTES_PER_LINE 9



int main(int argc, char **argv) {
	if (!bcm2835_init()) {
		printf("oops, could not init bcm2835\n");
		return 1;
	}
    
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);    // ~ 1 MHz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    
	uint8_t outbuff[BYTES_PER_LINE][LINES]; // one LED line long
    
	uint8_t inbuff[BYTES_PER_LINE][LINES];
    int n=0;
    int column = 0; // which column to light up, 0 - 25 for single board
    int count = 0;
    uint8_t pins = {RPI_V2_GPIO_P1_03 , RPI_V2_GPIO_P1_03 , RPI_V2_GPIO_P1_03 , RPI_V2_GPIO_P1_03
    RPI_V2_GPIO_P1_03 , RPI_V2_GPIO_P1_03 , RPI_V2_GPIO_P1_03 };
    
    while(1)
    {
        for(column=0;column<COLUMN_MAX;column++)
        {
            
            unsigned char column_byte = (column/8) ;
            unsigned char column_bit = column% 8 ;
            unsigned char buffer_byte;
            for(buffer_byte =0; buffer_byte< BYTES_PER_LINE; buffer_byte++)
            {
         //       printf("column_byte %i, buffer_byte %i, column_bit %i \n", column_byte, buffer_byte, column_bit );
                if(column_byte == buffer_byte)
                {
                    
                    outbuff[buffer_byte]=0x80 >> column_bit;
                } else {
                    outbuff[buffer_byte]=0x00;
                }
                
            }
        
        
       // printf("Column %i %i %i %x %x %x %x\n", column, column_byte, column_bit, outbuff[0],outbuff[1],outbuff[2],outbuff[3]);
        
        
        
            for(count = 0; count < 100; count++)
            {
                int line;
                for(line =0; line<LINES; line++)
                {
                    bcm2835_spi_transfernb(outbuff, inbuff, (BYTES_PER_LINE - 1) ); // one LED line is
                    // turn line on for a bit
                    bcm2835_gpio_set(pins[line]);
                    // delay
                    bcm2835_delayMicroseconds(100);
                    // turn line off
                    bcm2835_gpio_clear(pins[line]);
                    
                }
            }
        }
    }


bcm2835_spi_end();
bcm2835_close();
return 0;
}

