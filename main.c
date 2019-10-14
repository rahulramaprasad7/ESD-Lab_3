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
   uint16_t totalCharacters;
   uint16_t freeCharacters;
   struct buff* next;
};

void append(struct buff** head_ref, uint16_t size)
{
	/*  allocate node */
	struct buff* new_node = (struct buff*) malloc(sizeof(struct buff));
    new_node->characters = (uint16_t*)malloc(size * sizeof(uint16_t));
	struct buff *last = *head_ref; /* used in step 5*/

	/*  This new node is going to be the last node, so make next
		of it as NULL*/
	new_node->next = NULL;

	/*  Else traverse till the last node */
	while (last->next != NULL)
		last = last->next;

	/*  Change the next of last node */
	last->next = new_node;
	printf_tiny("Address of appended buffer\n\r");
	printf("%p %p", new_node, new_node->characters);
	printf_tiny("\n\r");
}

void deleteNode(struct buff **head_ref, int position)
{
   // If linked list is empty
   if (*head_ref == NULL)
      return;

   // Store head node
   struct buff* temp = *head_ref;

    // If head needs to be removed
    if (position == 0)
    {
        *head_ref = temp->next;   // Change head
        free(temp);               // free old head
        *head_ref = NULL;
        return;
    }

    // Find previous node of the node to be deleted
    for (int i=0; temp!=NULL && i<position-1; i++)
         temp = temp->next;

    // If position is more than number of ndoes
    if (temp == NULL || temp->next == NULL)
         return;

    // Node temp->next is the node to be deleted
    // Store pointer to the next of node to be deleted
    struct buff *next = temp->next->next;

    // Unlink the node from linked list
    free(temp->next);  // Free memory
    temp->next = next;  // Unlink the deleted node from list
    printf_tiny("Address after deletion buffer\n\r");
    printf("%p %p", temp, temp->characters);
	printf_tiny("\n\r");
	printf("%p %p", temp->next, temp->next->characters);
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

    printf_tiny("Address of first buffer\n\r");
    printf("%p %p", buffer_0, buffer_0->characters);
    printf_tiny("\n\r");
    printf_tiny("Address of second buffer\n\r");
    printf("%p %p", buffer_1, buffer_1->characters);
    printf_tiny("\n\r");
    //printf_tiny("%d\n\r",sizeof(struct buff));

    append(&buffer_0, size);
    append(&buffer_0, size);
    deleteNode(&buffer_0, 2);
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

