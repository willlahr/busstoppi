
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

unsigned char *shared_memory_setup(int size)
{
    unsigned char *result = NULL;
    
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
	// turn on lines one at a time
    
    
    
    unsigned char *outbuff = shared_memory_setup(ROWS * LINES * BYTES_PER_LINE);
    int a;
  
    
    while (1)
    {
        int row, on_row, byte = 0;
        for(on_row = 0; on_row < 7; on_row++)
        {
            printf("Testing Row %i\n", on_row);
            for(row = 0; row < 7; row++)
            {
                int offset = BYTES_PER_LINE * row;

                for(byte =0; byte<BYTES_PER_LINE; byte++)
                {
                    *(outbuff+offset+byte) = (row==on_row)?0xff:0x00;
                }
                           }
        
            sleep(2);

        }
    }
 
    
  //  for(a = 0; a< ROWS * LINES * BYTES_PER_LINE; a++) {
  //     *(outbuff+a) = (char)a;
  //  }
    
    return 0;
}

