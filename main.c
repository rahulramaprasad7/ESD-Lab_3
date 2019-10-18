#include <mcs51/8051.h>
#include <mcs51/at89c51ed2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define HEAP_SIZE 4000
__xdata char __sdcc_heap[HEAP_SIZE];         
const unsigned int __sdcc_heap_size = HEAP_SIZE;   //Set the Heap size to 4000 bytes

uint16_t lastCount = 0;

struct buff
{
   uint16_t size;             //Stores the buffer size
   uint16_t bufferNo;		  //Stores the buffer number
   char *characters;		  //Stores the buffer
   uint16_t characterCount;   //Stores total character count
   uint16_t freeCharacters;   //Stores the number of free spaces in the buffer
   struct buff* next;         //Points to the next buffer
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
        buffer_0 = malloc(sizeof(struct buff));          //Allocate memory for meta-data for buffer_0
        printf_tiny("Enter a buffer size\n\r");          
        gets(x);										 //Input the size of buffer
        buffer_0->size = atoi(x);						 //Convert string to int
        if (!(buffer_0->size >= 32 && buffer_0->size <=3200 && (buffer_0->size % 16 == 0)))      //Check for buffer size conditions imposed by the question
        {
            do
            {
                printf_tiny("Enter a valid buffer size which lies b/w 32 and 3200 and is divisible by 16\n\r");
                gets(x);
                buffer_0->size = atoi(x);
            }while (!(buffer_0->size >= 32 && buffer_0->size <=3200 && (buffer_0->size % 16 == 0)));    //Keep checking until the right input is given
        }

        buffer_0->characters = malloc(buffer_0->size * sizeof(char));     //Allocate buffer_0 once the input size is acceptable

        buffer_1 = malloc(sizeof(struct buff));							  //Allocate buffer_1 meta-data
        buffer_1->size = buffer_0->size;								  //Assign same size for buffer_1 as buffer_0
        buffer_1->characters = malloc(buffer_0->size * sizeof(char));      //Allocate memory for buffer_1
        if (buffer_1->characters == NULL || buffer_0->characters == NULL)   //Check if any of the buffers are not allocated
        {
            free(buffer_1->characters);										//Free the buffer_1
            free(buffer_1);													//Free Buffer_1 meta-data
            buffer_1->characters = NULL;									//Set the buffer pointer to NULL 
            &buffer_1 == NULL;
            free(buffer_0->characters);										//Free the buffer_0
			free(buffer_0);                                                 //Free buffer_0 meta-data
            buffer_0->characters = NULL;                                    //Set the buffer pointer to NULL
            &buffer_0 == NULL;
            printf_tiny("Buffer allocation failed, not enough space on the heap\n\r");
            continue;														//Continue allocating until it meets the requirements of heap size
        }
        break;																//Break out of the loop if buffer_0 and buffer_1 are successfully allocated
    }
    buffer_0->next = buffer_1;												//Point buffer_0 to next node buffer_1
    buffer_1->next = NULL;													//Point the next node of buffer_1 to NULL for time being
    buffer_0->bufferNo = 0;													//Assign buffer number
    buffer_1->bufferNo = 1;													//Assign buffer number
}

_sdcc_external_startup()
{
    AUXR &= (!XRS2);				//Clear the XRS2 bit
    AUXR |= XRS1 | XRS0;            //Set XRS1 and XRS0 bit to allocate 1024 bytes for XRAM
    return 0;
}

void main()
{
    uint16_t ch, i = 0, j = 0;
    reallocate();								//Allocate buffer_0 and buffer_1 successfully
    if (buffer_1->characters == NULL)			
    {
        reallocate();
    }

    buffer_0->characterCount = 0;				//Initialise character count of buffer_0
    printf_tiny("Enter the characters\n\r");    //Start inputting characters serially
    do
    {
        ch = getchar();                         								//Store each character
        (buffer_0->characterCount)++;           							    //Incremenet character count after each input 
        buffer_0->freeCharacters = buffer_0->size - buffer_0->characterCount;   //Calculate free space
        if (ch >= 97 && ch <= 122)												//Store the lowercase characters in buffer_0
        {
            if (i >= buffer_0->size)											//If buffer_0 limit is reached exho the characters
            {
                putchar(ch);
            }
            else																//If buffer limit is not reached store the lower characters
            {
                buffer_0->characters[i] = ch;
                putchar(ch);
                i++;
            }
        }
        else if (ch == '+')														//Execute functions to add additional buffer
        {
            putchar(ch);
            append(&buffer_0);
            continue;
        }
        else if (ch == '-')														//Execute functions to delete buffers
        {
            putchar(ch);
            deleteNode(&buffer_0);
            continue;
        }
        else if (ch == '?')														//Display the current bufefr details and empty out buffer_0
        {
            putchar(ch);
            questionMark();
            i = 0;															    //To start writing into buffer_0 again
            continue;
        }
        else if (ch == '@')														//Execute functions to delete all bufefrs and start the program again
        {
            putchar(ch);
            printf_tiny("\n\r");
            deleteAll(&buffer_0);
            reallocate();										                //Start executing program again 
            printf_tiny("Enter the characters\n\r");
            continue;
        }
        else if (ch == '=')                                                     //Print the buffer_0 contents in the specified format
        {
            putchar(ch);
            printBuffer0(&buffer_0);
            continue;
        }
        else																	//Echo other characters
        {
            putchar(ch);
            continue;
        }

    }while (ch != '\r');                                                        //Continue reading serial inputs until "Enter" key is pressed
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


