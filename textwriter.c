
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

#include "font.h"
#define COLUMN_MAX 145
#define ROWS 7
#define LINES 3
#define BYTES_PER_LINE 19

void pixel_on(int x, int y, int line, unsigned char *ledmem)
{

    
    int x_byte = (x / 8);
    int bit_number = x % 8;
    unsigned char bit_mask = 0x80 >> bit_number;
    int offset = (line * (BYTES_PER_LINE * ROWS)) + (BYTES_PER_LINE * y) + x_byte;
    *(ledmem + offset) |= bit_mask;
  //  printf("Turning on %i %i line: %i byte: %i mask: %x\n", x , y, line, x_byte, bit_mask);
    
}


void pixel_off(int x, int y, int line, unsigned char *ledmem)
{
    
    int x_byte = (x / 8);
    int bit_number = x % 8;
    unsigned char bit_mask = 0x80 >> bit_number;
    bit_mask ^= 0xff;
    int offset = (line * (BYTES_PER_LINE * ROWS)) + (BYTES_PER_LINE * y) + x_byte;
    *(ledmem + offset) &= bit_mask;
  //  printf("Turning off %i %i line: %i byte: %i mask: %x\n", x , y, line, x_byte, bit_mask);
    
}

unsigned char *shared_memory_setup(int size)
{
    unsigned char *result = NULL;
    
    /* Open the shared memory. */
    int descriptor = shm_open("ledsign",  O_RDWR, 0);
    
    /* Size up the shared memory. */
    ftruncate(descriptor, size);
    result = mmap(NULL, size,
                  PROT_WRITE | PROT_READ, MAP_SHARED,
                  descriptor, 0 );
    return result;
    
}

void write_character_at(int x,int line,char character, unsigned char *ledmem) {
    int x_offset;
    int y_offset;
    
    int fontoffset = (character-0x20);
    fontoffset *= 5;
 
    
    for(y_offset=0; y_offset<7; y_offset++)
    {
              for(x_offset=0; x_offset<5; x_offset++)
        {
            unsigned char fontmask = 0x01 << y_offset;
            int fontbyte_offset = fontoffset + x_offset;
            unsigned char fontbyte = font5x7[fontbyte_offset];

           // printf("Reading byte %i: %x with mask %x\n",fontbyte_offset, fontbyte, fontmask);
            
            if( (font5x7[fontbyte_offset] & fontmask) != 0x00 ) {
                pixel_on(x+x_offset,(line * LINES)+y_offset,0,ledmem);
            } else {
               pixel_off(x+x_offset,(line * LINES)+y_offset,0,ledmem);
                
            }
        }
    }
}

void write_string(char *string, unsigned char *ledmem) {
    int index = 0;
    for(index=0; index<strlen(string); index++){
        write_character_at(6*index,0,string[index], ledmem);
    }
    
}

int main(int argc, char **argv) {
	// write some text to the display
    
    if(argc < 2) {
        printf("Usage: %s <string to write>\n",argv[0]);
        exit(1);
        
    }
    
    
    
    
    unsigned char *outbuff = shared_memory_setup(ROWS * LINES * BYTES_PER_LINE);
    write_string(argv[1], outbuff);
    
    

    
  //  for(a = 0; a< ROWS * LINES * BYTES_PER_LINE; a++) {
  //     *(outbuff+a) = (char)a;
  //  }
    
    return 0;
}

