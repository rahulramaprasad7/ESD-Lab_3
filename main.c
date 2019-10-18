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
   uint16_t bufferNo;
   char *characters;
   uint16_t characterCount;
   uint16_t freeCharacters;
   struct buff* next;
};

struct buff* buffer_0 = NULL;
struct buff* buffer_1 = NULL;
struct buff* new_node = NULL;

void printBuffers (struct buff** head_ref)
{
    struct buff* temp = *head_ref;
    int k = 0, j = 0;
    while(temp!= NULL)
    {
        printf_tiny("****Information regarding buffer_%d****\n\r", k);
        printf("Starting address of buffer_%d = %p\n\r", k, &temp->characters[0]);
        printf("Ending address of buffer_%d = %p\n\r", k, &(temp->characters[temp->size]));
        printf_tiny("The size of this buffer = %d", temp->size);
        if( k == 0)
        {
            printf_tiny("Number of total characters = %d\n\r", temp->characterCount);
            printf_tiny("Number of free characters = %d\n\r", temp->freeCharacters);
        }
//        if( k == 0)
//        {
//            printf_tiny("The contents of the buffer_%d are\n\r", k);
//            do
//            {
//                putchar(temp->characters[j]);
//                printf(" %p\n\r",&temp->characters[j++]);
//            }while (temp->characters[j] != '\0');
//        }
        k++;
        temp = temp->next;
    }

}

void printBuffer0(struct buff** head_ref)
{
    uint8_t i, j = 0;
    struct buff* buff_0 = *head_ref;
    printf_tiny("\n\r");
    for (i = 0; i< buff_0->size; i += 16)
    {
        printf("%p: ",&buff_0->characters[i]);
        for (j = i; j < (i + 16) ;j++)
            printf("%X ", buff_0->characters[j]);
        printf_tiny("\n\r");
    }
}
void append(struct buff** head_ref)
{
    uint16_t i =0;
    char x[10];

	while (1)
    {
        new_node = malloc(sizeof(struct buff));
        printf_tiny("Enter a buffer size between 30 and 300 to add\n\r");
        gets(x);
        new_node->size = atoi(x);
        if (!(new_node->size >= 30 && new_node->size <=300))
        {
            do
            {
                printf_tiny("Enter a valid buffer size which lies b/w 30 and 300\n\r");
                gets(x);
                new_node->size = atoi(x);
            }while (!(new_node->size >= 30 && new_node->size <=300));
        }

        new_node->characters = malloc(new_node->size * sizeof(char));
        if (new_node == NULL || new_node->characters == NULL)
        {
            free(new_node->characters);
            free(new_node);
            new_node->characters = NULL;
            &new_node == NULL;
            if (new_node->size < 30)
                printf_tiny("Available heap size is less than 30, buffer can't be allocated\n\r");
            printf_tiny("Extra Buffer allocation failed, not enough space on the heap\n\r");
            continue;
        }
        break;
    }

	struct buff *last = *head_ref;

	new_node->next = NULL;

	if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }

	while (last->next != NULL)
    {
        last = last->next;
        i++;
    }

	last->next = new_node;
	new_node->bufferNo = i;
//	printf_tiny("Address of appended buffer\n\r");
//	printf("%p %p", new_node, new_node->characters);
//	printf_tiny("\n\r");
}

void deleteNode(struct buff **head_ref)
{
   char x[10];
   printf_tiny("Enter the buffer number to be deleted\n\r");
        gets(x);
    uint16_t position = atoi(x);
    // If linked list is empty
   if (*head_ref == NULL)
      return;

   // Store head node
   struct buff* temp = *head_ref;

    // If head needs to be removed
    if (position == 0)
    {
        free(temp->characters);
        free(temp);
        //temp = NULL;
        //temp->characters = NULL;
        printf("Yo\n\r");
        printf_tiny("Address after deletion buffer\n\r");
        printf("%p %p\n\r", temp, temp->characters);
        *head_ref = NULL;   // Change head
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
    free(temp->next->characters);
    free(temp->next);  // Free memory
    temp->next->characters = NULL;
    temp->next = NULL;
    temp->next = next;  // Unlink the deleted node from list
    printf_tiny("Address after deletion buffer\n\r");
    printf("%p %p", temp, temp->characters);
    printf_tiny("\n\r");
    //printf("%p %p", temp->next, temp->next->characters);
    //printf_tiny("\n\r");
}

void reallocate()
{
    char x[10];
    while(1)
    {
        buffer_0 = malloc(sizeof(struct buff));
        printf_tiny("Enter a buffer size\n\r");
        gets(x);
        buffer_0->size = atoi(x);
        buffer_1->size = buffer_0->size;
        if (!(buffer_0->size >= 32 && buffer_0->size <=3200 && (buffer_0->size % 16 == 0)))
        {
            do
            {
                printf_tiny("Enter a valid buffer size which lies b/w 32 and 3200 and is divisible by 16\n\r");
                gets(x);
                buffer_0->size = atoi(x);
            }while (!(buffer_0->size >= 32 && buffer_0->size <=3200 && (buffer_0->size % 16 == 0)));
        }

        buffer_0->characters = malloc(buffer_0->size * sizeof(char));

        buffer_1 = malloc(sizeof(struct buff));
        buffer_1->characters = malloc(buffer_0->size * sizeof(char));
        if (buffer_1->characters == NULL || buffer_0->characters == NULL)
        {
            free(buffer_1->characters);
            free(buffer_1);
            buffer_1->characters = NULL;
            &buffer_1 == NULL;
            free(buffer_0->characters);
            free(buffer_0);
            buffer_0->characters = NULL;
            &buffer_0 == NULL;
            printf_tiny("Buffer allocation failed, not enough space on the heap\n\r");
            continue;
        }
        break;
    }
    buffer_0->next = buffer_1;
    buffer_1->next = NULL;
    buffer_0->bufferNo = 0;
    buffer_1->bufferNo = 1;
}

_sdcc_external_startup()
{
    AUXR &= (!XRS2);
    AUXR |= XRS1 | XRS0;
    return 0;
}

void main()
{
    uint16_t ch, i = 0, j = 0;
    reallocate();
    if (buffer_1->characters == NULL)
    {
        reallocate();
    }

    buffer_0->characterCount = 0;
    printf_tiny("Enter the characters\n\r");
    do
    {
        if( i == buffer_0->size)
        {
            break;
        }
        ch = getchar();
        buffer_0->characterCount++;
        if (ch >= 97 && ch <= 122)
        {
            if (i >= buffer_0->size)
            {
                putchar(ch);
            }
            else
            {
                buffer_0->characters[i] = ch;
                putchar(ch);
            }
            i++;
        }
        else if (ch == '+')
        {
            putchar(ch);
            append(&buffer_0);
            continue;
        }
        else if (ch == '-')
        {
            putchar(ch);
            deleteNode(&buffer_0);
            continue;
        }
        else if (ch == '?')
        {
            putchar(ch);
            continue;
        }
        else if (ch == '@')
        {
            putchar(ch);
            continue;
        }
        else if (ch == '=')
        {
            putchar(ch);
            printBuffer0(&buffer_0);
            continue;
        }
        else
        {
            putchar(ch);
            continue;
        }

    }while (ch != '\r');

    buffer_0->freeCharacters = buffer_0->characterCount - i;
    buffer_0->characters[i] = '\0';
//    do
//    {
//        putchar(buffer_0->characters[j]);
//        printf("%p\n\r",&buffer_0->characters[j++]);
//    }while (buffer_0->characters[j] != '\0');
//    printf_tiny("Total=%d Free=%d\n\r", buffer_0->characterCount, buffer_0->freeCharacters);
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


