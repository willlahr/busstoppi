
#ifndef NO_HARDWARE
#include <bcm2835.h>
#endif

#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <stdlib.h>



#define COLUMN_MAX 145
#define ROWS 7
#define LINES 3
#define BYTES_PER_LINE 19



void spi_setup() {
#ifdef NO_HARDWARE
    printf("Not really doing spi setup, no hardware available\n");
#else

    if (!bcm2835_init()) {
		printf("oops, could not init bcm2835\n");
        abort();
	}
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);    // ~ 1 MHz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    
#endif
}

uint8_t *shared_memory_setup(size_t size)
{
    uint8_t *result = NULL;
 
    /* Open the shared memory. */
    int descriptor = shm_open("ledsign", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    /* Size up the shared memory. */
    ftruncate(descriptor, size);
    result = mmap(NULL, size,
                  PROT_WRITE | PROT_READ, MAP_SHARED,
                  descriptor, 0 );
    return result;
    
}


int main(int argc, char **argv) {
	
     spi_setup();
    // stop our memory from getting paged out, this is to stop our task being scheduled out whilst doing something critical
    mlockall(MCL_CURRENT | MCL_FUTURE);
    
    uint8_t *outbuff = shared_memory_setup(ROWS * LINES * BYTES_PER_LINE);
    
    uint8_t inbuff[BYTES_PER_LINE * 3]; // doesn't read anything - isn't even connected to anything
    
    int n=0;
    int column = 0; // which column to light up, 0 - 25 for single board
    int count = 0;
    
    
    uint8_t pins[7] =
#ifdef NO_HARDWARE
    {};
#else
    {      RPI_V2_GPIO_P1_11, RPI_V2_GPIO_P1_13 , RPI_V2_GPIO_P1_15 , RPI_V2_GPIO_P1_16 ,RPI_V2_GPIO_P1_07 , RPI_V2_GPIO_P1_05, RPI_V2_GPIO_P1_03};
#endif
    
    uint8_t cs_pins[3] =
#ifdef NO_HARDWARE
    {};
#else
    {RPI_V2_GPIO_P1_22 , RPI_V2_GPIO_P1_24 , RPI_V2_GPIO_P1_26};
#endif

#ifdef NO_HARDWARE
    printf("Not setting up pins - no hardware\n");
#else
    int pin =0;
    for(pin = 0; pin < sizeof(pins); pin++) {
        bcm2835_gpio_fsel(pins[pin], BCM2835_GPIO_FSEL_OUTP);
    }
    
    for(pin = 0;  pin< sizeof(cs_pins); pin++)
    {
        bcm2835_gpio_fsel(cs_pins[pin], BCM2835_GPIO_FSEL_OUTP);
    }
#endif
    
    while(1)
    {
        int row;
        for(row =0; row<ROWS; row++)
        {
            
            int line = 0;
            for(line = 0; line < 3; line++)
            {
                int line_offset = (line * ROWS * BYTES_PER_LINE);
                int row_offset= (row * BYTES_PER_LINE);
                int offset = line_offset + row_offset;
#ifdef NO_HARDWARE
                printf("CS Low for line %i\n",line);
                printf("SPI Send %i bytes starting at offset %i\n", (BYTES_PER_LINE)- 1 , offset );
        
                printf("CS High for line %i\n",line);
                
#else
                // pull cs low
                bcm2835_gpio_clr(cs_pins[line]);
                
                bcm2835_spi_transfernb( (outbuff  + offset) , inbuff, (BYTES_PER_LINE)- 1 );

#endif

 
                bcm2835_gpio_set(cs_pins[line]);
            }
            struct sched_param sp;
            memset(&sp, 0, sizeof(sp));
            sp.sched_priority = sched_get_priority_max(SCHED_FIFO);

#ifdef NO_HARDWARE
            printf("Turning row %i on\n",row);
            
#else
            sched_setscheduler(0, SCHED_FIFO, &sp);
            // turn line on for a bit
            bcm2835_gpio_set(pins[row]);
            //           // delay
            bcm2835_delayMicroseconds(120);
            // turn line off
            bcm2835_gpio_clr(pins[row]);
            memset(&sp, 0, sizeof(sp));
            sp.sched_priority = sched_get_priority_max(SCHED_OTHER);
            sched_setscheduler(0, SCHED_OTHER, &sp);
#endif

        }
    }



//bcm2835_spi_end();
//bcm2835_close();
return 0;
}

