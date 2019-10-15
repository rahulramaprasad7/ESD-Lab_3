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
   char *characters;
   uint16_t characterCount;
   uint16_t freeCharacters;
   struct buff* next;
};

void printBuffers(struct buff** head_ref)
{
    struct buff* temp = *head_ref;
    int i = 0, j = 0;
    while(temp->next != NULL)
    {
        printf("****Information regarding buffer_%d****\n\r", i);
        printf("Starting address of buffer_%d = %p\n\r", i, temp);
        printf("Ending address of buffer_%d = %p\n\r", i, (temp->next - 1));
        printf_tiny("Number of total characters = %d", temp->characterCount);
        printf_tiny("Number of free characters = %d", temp->freeCharacters);
        if( i == 0)
        {
            printf_tiny("The contents of the buffer_%d are\n\r", i);
            do
            {
                putchar(temp->characters[j]);
                printf(" %p\n\r",&temp->characters[j++]);
            }while (temp->characters[j] != '\0');
        }
    }
}

void append(struct buff** head_ref, uint16_t size)
{
	struct buff* new_node = (struct buff*) malloc(sizeof(struct buff));
    new_node->characters = (uint16_t*)malloc(size * sizeof(uint16_t));
	struct buff *last = *head_ref;

	new_node->next = NULL;

	while (last->next != NULL)
		last = last->next;

	last->next = new_node;
	printf_tiny("Address of appended buffer\n\r");
	printf("%p %p", new_node, new_node->characters);
	printf_tiny("\n\r");
}

void deleteNode(struct buff **head_ref, int position)
{
   if (*head_ref == NULL)
      return;

   struct buff* temp = *head_ref;

    if (position == 0)
    {
        *head_ref = temp->next;   // Change head
        free(temp);               // free old head
        return;
    }

    for (int i=0; temp!=NULL && i<position-1; i++)
         temp = temp->next;

    if (temp == NULL || temp->next == NULL)
         return;

    struct buff *next = temp->next->next;

    free(temp->next);  // Free memory
    free(temp->next->characters);
    temp->next = next;  // Unlink the deleted node from list
    printf_tiny("Address after deletion buffer\n\r");
    printf("%p %p", temp, temp->characters);
	printf_tiny("\n\r");
	printf("%p %p", temp->next, temp->next->characters);
	printf_tiny("\n\r");
}

/*int* mgets()
{
	int *ptr;
	int *x = ptr;
	int rd, i = 0;
    do
    {
        rd = getchar();
        ptr[i] = rd;
        printf("%d ", ptr[i++]);
    }while (rd != '\r');

	*ptr = '\0';
	return x;
}
*/
void main()
{
    AUXR &= (!XRS2);
    AUXR |= XRS1 | XRS0;
    uint16_t ch, i = 0, j = 0;
    struct buff* buffer_0 = NULL;
    struct buff* buffer_1 = NULL;
    char *x;
    printf_tiny("Enter a buffer size\n\r");
    gets(x);
    buffer_0->size = atoi(x);
    if (!(buffer_0->size >= 32 && buffer_0->size <=3200 && (buffer_0->size % 16 == 0)))
    {
        do
        {
            printf_tiny("Enter a valid buffer size\n\r");
            gets(x);
            buffer_0->size = atoi(x);
        }while (!(buffer_0->size >= 32 && buffer_0->size <=3200 && (buffer_0->size % 16 == 0)));
    }
    printf_tiny("%d\n\r", buffer_0->size);
    buffer_0 = malloc(sizeof(struct buff));
    buffer_0->characters = malloc(buffer_0->size * sizeof(char));

    buffer_1 = malloc(sizeof(struct buff));
    buffer_1->characters = malloc(buffer_0->size * sizeof(char));

    buffer_0->next = buffer_1;
    buffer_1->next = NULL;

    printf_tiny("Address of first buffer\n\r");
    printf("%p %p %p %p", buffer_0, buffer_0->characters, &buffer_0->characters[0], &buffer_0->characters[1]);
    printf_tiny("\n\r");
    printf_tiny("Address of second buffer\n\r");
    printf("%p %p %p %p", buffer_1, buffer_1->characters, &buffer_1->characters[0], &buffer_1->characters[1]);
    printf_tiny("\n\r");

    append(&buffer_0, buffer_0->size);
    append(&buffer_0, buffer_0->size);
    deleteNode(&buffer_0, 2);
    buffer_0->characterCount = 0;
    printf_tiny("Enter the characters\n\r");
    do
    {
        if( i == buffer_0->size)
            break;
        ch = getchar();
        buffer_0->characterCount++;
        if (ch >= 97 && ch <= 122)
        {
            buffer_0->characters[i] = ch;
            putchar(ch);
            i++;
        }
        else
        {
            putchar(ch);
            continue;
        }
    }while (ch != '\r');

    buffer_0->freeCharacters = buffer_0->characterCount - i;
    buffer_0->characters[i] = '\0';
    printf_tiny("Done\n\r");
    do
    {
        putchar(buffer_0->characters[j]);
        printf("%p\n\r",&buffer_0->characters[j++]);
    }while (buffer_0->characters[j] != '\0');
    printf_tiny("Total=%d Free=%d", buffer_0->characterCount, buffer_0->freeCharacters);
    /*while (1)
    {
        ch=getchar();
        putchar(ch);
    }*/
    return;
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



