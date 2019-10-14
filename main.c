#include <mcs51/8051.h>
#include <mcs51/at89c51ed2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define HEAP_SIZE 4000
__xdata char __sdcc_heap[HEAP_SIZE];
const unsigned int __sdcc_heap_size = HEAP_SIZE;

struct buff
{
   uint16_t size;
   uint16_t *characters;
   uint16_t *storageCharacters;
   uint16_t freeCharacters;
   struct buff* next;
};

void append(struct buff** head_ref, uint16_t size)
{
	/* 1. allocate node */
	struct buff* new_node = (struct buff*) malloc(sizeof(struct buff));
    new_node->characters = (uint16_t*)malloc(size * sizeof(uint16_t));
	struct buff *last = *head_ref; /* used in step 5*/

	/* 3. This new node is going to be the last node, so make next
		of it as NULL*/
	new_node->next = NULL;

	/* 5. Else traverse till the last node */
	while (last->next != NULL)
		last = last->next;

	/* 6. Change the next of last node */
	last->next = new_node;
	printf("%p %p", new_node, new_node->characters);
	printf_tiny("\n\r");
}

void main()
{
    AUXR |= (!XRS2) | XRS1 | XRS0;
    int ch, i = 0;
    struct buff* buffer_0 = NULL;
    struct buff* buffer_1 = NULL;

    uint16_t size = 400;

    buffer_0 = (struct buff*)malloc(sizeof(struct buff));
    buffer_0->characters = (uint16_t*)malloc(size * sizeof(uint16_t));

    buffer_1 = (struct buff*)malloc(sizeof(struct buff));
    buffer_1->characters = (uint16_t*)malloc(size * sizeof(uint16_t));

    buffer_0->next = buffer_1;
    buffer_1->next = NULL;

    printf("%p %p", buffer_0, buffer_0->characters);
    printf_tiny("\n\r");
    printf("%p %p", buffer_1, buffer_1->characters);
    printf_tiny("\n\r");
    //printf_tiny("%d\n\r",sizeof(struct buff));

    append(&buffer_0, size);
    append(&buffer_0, size);
/*    struct buff* buffer_2 = NULL;
    buffer_1->next = buffer_2;
    buffer_2->next = NULL;

    buffer_2 = (struct buff*)malloc(sizeof(struct buff));
    buffer_2->characters = (uint16_t*)malloc(size * sizeof(uint16_t));

    printf("%p %p %p %p", buffer_0, buffer_0->characters, buffer_1, buffer_1->characters);
    printf_tiny("\n\r");

    free(buffer_1);
    buffer_0->next = buffer_2;
    printf("%p %p %p %p %p %p", buffer_0, buffer_0->characters, buffer_1, buffer_1->characters, buffer_2, buffer_2->characters);
*/


    printf_tiny("Enter the characters\n\r");
    while (ch = getchar() != '\r')
        buffer_0->characters[i++] = ch;
    buffer_0->characters[i] = '\0';
    printf_tiny("Done\n\r");
    i = 0;
    while (buffer_0->characters[i++] != '\0')
        printf_tiny("%d\n\r", buffer_0->characters[i]);
    /*printf_tiny("Enter the size of Buffer\n\r");
    char str[16];
    int i=0;
    int ch;
    while((ch = getchar()) != '\n')
        str[i++] = ch;
    str[i] = '\0';
    i = 0;
    while(str[i] != '\n')
        putchar(str[i]);*/
    while (1)
    {
        ch=getchar();
        putchar(ch);
    }
}
int putchar (int c)
{
	while ((SCON & 0x02) == 0);    // wait for TX ready, spin on TI
	SBUF = c;  	// load serial port with transmit value
	TI = 0;  	// clear TI flag
	return 0;
}

int getchar ()
{
    while ((SCON & 0x01) == 0);  // wait for character to be received, spin on RI
	RI = 0;			// clear RI flag
	return SBUF;  	// return character from SBUF
}
int putstr (char *s)
{
	int i = 0;
	while (*s){			// output characters until NULL found
		putchar(*s++);
		i++;
	}
	//putchar('\n');
	return i+1;
}

