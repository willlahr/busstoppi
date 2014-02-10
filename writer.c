
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
	// try to turn on one first 5x7 bit character at a time
    
    
    
    
    unsigned char *outbuff = shared_memory_setup(ROWS * LINES * BYTES_PER_LINE);
    int a;
    // just turn on first row
    for(a = 0; a< ROWS; a++) {
        offset = (BYTES_PER_LINE * a);
        *(outbuff+offset) = 0xF8;
        int b;
        for(b=0; b<5; b++)
        {
            offset++;
            *(outbuff+offset) = 0x00;
            
        }
        
    }
    

    
  //  for(a = 0; a< ROWS * LINES * BYTES_PER_LINE; a++) {
  //     *(outbuff+a) = (char)a;
  //  }
    
    return 0;
}

