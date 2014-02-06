
#include <bcm2835.h>
#include <stdio.h>

#define COLUMN_MAX 25
#define ROWS 7
#define LINES 3
#define BYTES_PER_LINE 4



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
    
	uint8_t outbuff[ROWS][BYTES_PER_LINE * LINES]; // one LED line long
    
	uint8_t inbuff[BYTES_PER_LINE * 3]; // doesn't read anything - isn't even connected to anything
    int n=0;
    int column = 0; // which column to light up, 0 - 25 for single board
    int count = 0;
    
    uint8_t pins[] = {RPI_V2_GPIO_P1_03 , RPI_V2_GPIO_P1_05 , RPI_V2_GPIO_P1_07 ,
        RPI_V2_GPIO_P1_11, RPI_V2_GPIO_P1_13 , RPI_V2_GPIO_P1_15 , RPI_V2_GPIO_P1_16 };
    
    uint8_t cs_pins[] = {RPI_V2_GPIO_P1_22 , RPI_V2_GPIO_P1_24 , RPI_V2_GPIO_P1_26};
    
    int pin =0;
    for(pin = 0; pin < sizeof(pins); pin++) {
        bcm2835_gpio_fsel(pins[pin], BCM2835_GPIO_FSEL_OUTP);
    }
    
    for(pin = 0;  pin< sizeof(cs_pins); pin++)
    {
        bcm2835_gpio_fsel(cs_pins[pin], BCM2835_GPIO_FSEL_OUTP);
    }
    
    while(1)
    {
        for(column=0;column<COLUMN_MAX;column++)
        {
            
            unsigned char column_byte = (column/8) ;
            unsigned char column_bit = column% 8 ;
            unsigned char buffer_byte;
            // this block makes up some data to display. (A horizontal bar goind from left to right)
            for(buffer_byte =0; buffer_byte< (BYTES_PER_LINE * LINES); buffer_byte++)
            {
                //       printf("column_byte %i, buffer_byte %i, column_bit %i \n", column_byte, buffer_byte, column_bit );
                if(column_byte == (buffer_byte % BYTES_PER_LINE))
                {
                    int row  = 0;
                    for(pin=0; row<ROWS; row++)
                    {
  //                      outbuff[row][buffer_byte]=0x80 >> column_bit;
                        outbuff[row][buffer_byte]=0xFF;
                    }
                } else {
                    int row = 0;
                    for(row=0; row<ROWS; row++)
                    {
//                        outbuff[row][buffer_byte]=0x00;
                        outbuff[row][buffer_byte]=0xFF;
                        
                    }
                }
                
            }
            
            
            // printf("Column %i %i %i %x %x %x %x\n", column, column_byte, column_bit, outbuff[0],outbuff[1],outbuff[2],outbuff[3]);
            
            
            
            for(count = 0; count < 100; count++)
            {
                int row;
                for(row =0; row<ROWS; row++)
                {
                    int line = 0;
                    for(line = 0; line < 3; line++)
                    {
                        
                        // pull cs low
                        bcm2835_gpio_clr(cs_pins[line]);
                        bcm2835_spi_transfernb(outbuff[line] + (line * BYTES_PER_LINE), inbuff, (BYTES_PER_LINE - 1) ); // one LED line is
                        bcm2835_gpio_set(cs_pins[line]);
                    }
                    // turn line on for a bit
                    bcm2835_gpio_set(pins[row]);
                    // delay
                    bcm2835_delayMicroseconds(200);
                    // turn line off
                    bcm2835_gpio_clr(pins[row]);
                    
                }
            }
        }
    }


bcm2835_spi_end();
bcm2835_close();
return 0;
}

