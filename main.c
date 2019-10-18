#include <mcs51/8051.h>
#include <mcs51/at89c51ed2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define HEAP_SIZE 4000
__xdata char __sdcc_heap[HEAP_SIZE];
const unsigned int __sdcc_heap_size = HEAP_SIZE;

uint16_t lastCount = 0;

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
        printf_tiny("\n\r****Information regarding buffer_%d****\n\r", k);
        printf("\n\rStarting address of buffer_%d = %p\n\r", k, &temp->characters[0]);
        printf("Ending address of buffer_%d = %p\n\r", k, &(temp->characters[temp->size]));
        printf_tiny("The size of this buffer = %d\n\r", temp->size);
        if( k == 0)
        {
            printf_tiny("Number of total characters = %d\n\r", temp->characterCount);
            printf_tiny("Number of free characters = %d\n\r", temp->freeCharacters);
        }
        k++;
        temp = temp->next;
    }

}

void questionMark()
{
    uint16_t i, j = 0;
    printBuffers(&buffer_0);
    printf_tiny("Number of total characters since last ? = %d\n\r", ((buffer_0->characterCount) - lastCount));
    printf_tiny("Contents of Buffer_0 are \n\r");
    for (i = 0; i < buffer_0->characterCount; i += 64)
    {
        for (j = i; j < (i+64) && j < buffer_0->characterCount; j++)
        {
            printf_tiny("%c ", buffer_0->characters[j]);
            buffer_0->characters[j] = 0;
        }
        printf_tiny("\n\r");
    }
    lastCount = buffer_0->characterCount;
}

void printBuffer0(struct buff** head_ref)
{
    uint16_t i, j = 0;
    struct buff* buff_0 = *head_ref;
    printf_tiny("\n\r");
    for (i = 0; i< buff_0->size; i += 16)
    {
        printf("%p: ",&buff_0->characters[i]);
        for (j = i; j < (i + 16) ;j++)
            printf("%2X ", buff_0->characters[j]);
        printf_tiny("\n\r");
    }
    return;
}
void append(struct buff** head_ref)
{
    uint16_t i =0;
    char x[10];

	while (1)
    {
        new_node = malloc(sizeof(struct buff));
        printf_tiny("\n\rEnter a buffer size between 30 and 300 to add\n\r");
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
	new_node->bufferNo = i + 1;
	printf_tiny("Successfully allocated buffer_%d\n\r", new_node->bufferNo);
}

void deleteNode(struct buff **head_ref)
{
    char x[10];
    uint16_t i = 0;;
    printf_tiny("Enter the buffer number to be deleted\n\r");
    gets(x);
    uint16_t position = atoi(x);

    if (*head_ref == NULL)
        return;

    struct buff* temp = *head_ref;

    if (position == 0)
    {
        printf_tiny("Buffer_0 cannot be deleted using this command, please type the next character\n\r");
        return;
    }
    for (i = 1; temp != NULL && i < position; i++)
        temp = temp->next;

    if (temp == NULL || temp->next == NULL)
        return;

    struct buff *next = temp->next->next;

    free(temp->next->characters);
    free(temp->next);

    printf_tiny("\n\r Successfully deleted buffer_%d\n\r", temp->next->bufferNo);

    temp->next->characters = NULL;
    temp->next = NULL;

    temp->next = next;
    temp->next->bufferNo = next->bufferNo;
}

void deleteAll(struct buff **head_ref)
{
    struct buff* current = *head_ref;
    struct buff* next;
    while (current != NULL)
    {
        next = current->next;
        free(current->characters);
        free(current);
        current->characters = NULL;
        current = NULL;
        current = next;
    }
    *head_ref = NULL;
    printf("\n\r Deleted all buffers, starting the program again\n\r");
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
        buffer_1->size = buffer_0->size;
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
        ch = getchar();
        (buffer_0->characterCount)++;
        buffer_0->freeCharacters = buffer_0->size - buffer_0->characterCount;
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
                i++;
            }
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
            questionMark();
            i = 0;
            continue;
        }
        else if (ch == '@')
        {
            putchar(ch);
            printf_tiny("\n\r");
            deleteAll(&buffer_0);
            reallocate();
            printf_tiny("Enter the characters\n\r");
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


