#include <mcs51/8051.h>
#include <mcs51/at89c51ed2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DEBUG                                      //Define DEBUG to enable DEBUG mode

#define HEAP_SIZE 4000
__xdata char __sdcc_heap[HEAP_SIZE];
const unsigned int __sdcc_heap_size = HEAP_SIZE;   //Set the Heap size to 4000 bytes

uint16_t lastCount;
#ifdef DEBUG
#define DEBUGPORT(x) dataout(x)                     //Write debug codes when enabled
#else
#define DEBUGPORT(x)                                //Don't do anything when debug is disabled
#endif

void dataout(char x)                                //Function to write debug codes
{
    ACC = x;                                        //Store the debug code in Accumulator
    __asm                                           //Start the assemble routine
    MOV DPTR, #0xFFFF                               //Set the address of write to ffff
    MOVX @DPTR, A                                   //Write to address ffff
    __endasm;                                       //End the assembly routine
}

struct buff
{
   uint16_t size;             //Stores the buffer size
   uint16_t bufferNo;		  //Stores the buffer number
   char *characters;		  //Stores the buffer
   uint16_t characterCount;   //Stores total character count
   uint16_t storedCharacters; //Stores he count of stored characters
   uint16_t freeCharacters;   //Stores the number of free spaces in the buffer
   struct buff* next;         //Points to the next buffer
};

struct buff* buffer_0 = NULL;
struct buff* buffer_1 = NULL;
struct buff* new_node = NULL;

void printBuffers (struct buff** head_ref)
{
    DEBUGPORT(1);                   //Call the DEBUG
    struct buff* temp = *head_ref;  //Create a temporary reference
    int k = 0, j = 0;
    while(temp!= NULL)              //Check until last node is NULL
    {
        printf_tiny("-----------------------------------------------------------------------\n\r");
        printf_tiny("****Information regarding buffer_%d****\n\r", temp->bufferNo);
        printf_tiny("-----------------------------------------------------------------------\n\r");
        printf("\n\rStarting address of buffer_%d = %p\n\r", temp->bufferNo, &temp->characters[0]);
        printf("Ending address of buffer_%d = %p\n\r", temp->bufferNo, &(temp->characters[temp->size]));
        printf_tiny("The size of this buffer = %d\n\r", temp->size);
        printf_tiny("Number of total characters stored in the buffer= %d\n\r", temp->storedCharacters);
        printf_tiny("Number of free characters = %d\n\r", temp->freeCharacters);

        temp = temp->next;          //Point to next node to print next buffer
    }

}

void questionMark()
{
    DEBUGPORT(2);                //Call DEBUG
    uint16_t i, j = 0;
    printBuffers(&buffer_0);     //Print the contents of the heap
    printf_tiny("\n\rNumber of total characters since last ? = %d\n\r", ((buffer_0->characterCount) - lastCount)); //Calculate the number of free space in the buffer
    printf_tiny("\n\rContents of Buffer_0 are \n\r");
    for (i = 0; i < buffer_0->characterCount; i += 64)      //Print 64 ASCII characters in each line
    {
        for (j = i; j < (i+64) && j < buffer_0->characterCount; j++)    //Print the contents of buffer in ASCII
        {
            printf_tiny("%c ", buffer_0->characters[j]);
            buffer_0->characters[j] = 0;                                //Empty the buffer contents one by one
        }
        printf_tiny("\n\r");
    }
    lastCount = buffer_0->characterCount;                               //Store the character count since last call
}

void printBuffer0(struct buff** head_ref)
{
    DEBUGPORT(3);                   //Call DEBUG
    uint16_t i, j = 0;
    struct buff* buff_0 = *head_ref;    //Create a temporary reference to buffer_0
    printf_tiny("\n\r");
    for (i = 0; i< buff_0->size; i += 16) //Print 16 characters per line
    {
        printf("%p: ",&buff_0->characters[i]);  //Print the starting address for the line
        for (j = i; j < (i + 16) ;j++)
            printf("%2X ", buff_0->characters[j]); //Print the characters in HEX
        printf_tiny("\n\r");
    }
    return;
}
void append(struct buff** head_ref)
{
    DEBUGPORT(4);       //Call DEBUG
    uint16_t i =0;
    char x[10];         //Create a temporary array to store input size

	while (1)           //Loop until the buffer is allocated
    {
        new_node = malloc(sizeof(struct buff));  //Allocate meta data for the buffer
        printf_tiny("\n\rEnter a buffer size between 30 and 300 to add\n\r");
        gets(x);                                 //Get the input as string
        new_node->size = atoi(x);                //Converting string to ASCII
        if (!(new_node->size >= 30 && new_node->size <=300))  //Check the size conditions
        {
            do
            {
                printf_tiny("Enter a valid buffer size which lies b/w 30 and 300\n\r");
                gets(x);
                new_node->size = atoi(x);
            }while (!(new_node->size >= 30 && new_node->size <=300));    //Loop until the right size is input
        }

        new_node->characters = malloc(new_node->size * sizeof(char));    //Allocate the buffer
        if (new_node == NULL || new_node->characters == NULL)            //Check if the buffer is allocated
        {
            free(new_node->characters);                                  //Free the buffer
            free(new_node);                                              //Free the meta-data for the buffer
            new_node->characters = NULL;                                 //Set the buffer pointer to null
            &new_node == NULL;                                           //Set the buffer meta-data pointer to null
            if (new_node->size < 30)
                printf_tiny("Available heap size is less than 30, buffer can't be allocated\n\r");
            printf_tiny("Extra Buffer allocation failed, not enough space on the heap\n\r");
            continue;           //Keep looping until the buffer is allocated successfully
        }
        break;                  //Break out of the loop if buffer is allocated
    }

	struct buff *last = *head_ref;  //Create a temporary node

	new_node->next = NULL;          //Set the next node to null as it is appending in the end

	if (*head_ref == NULL)          //If there is no head node
    {
       *head_ref = new_node;        //Make the created buffer as head
       return;
    }

	while (last->next != NULL)     //Traverse to the last buffer in the list
    {
        last = last->next;
        i++;
    }

	last->next = new_node;              //Link the new buffer to the list
	new_node->bufferNo = i + 1;         //Assign buffer number
	new_node->storedCharacters = 0;     //Set it to 0 as appended buffers have no data
	new_node->freeCharacters = new_node->size;   //Set the freecharacter counter to buffer size
	printf_tiny("Successfully allocated buffer_%d\n\r", new_node->bufferNo);
}

void deleteNode(struct buff **head_ref)
{
    DEBUGPORT(5);         //Call DEBUG
    char x[10];
    uint16_t i = 0;
    printf_tiny("Enter the buffer number to be deleted\n\r");
    gets(x);
    uint16_t position = atoi(x);

    if (*head_ref == NULL)        //Check if the list is empty
    {
        printf_tiny("There are no buffers to be deleted\n\r");
        return;
    }

    struct buff* temp = *head_ref;  //Create a temporary node
    struct buff* prev = NULL;       //Create a temporary node to store previous buffer

    if (position == 0)              //Check if the user is trying to delete buffer_0 and prevent it from happening
    {
        printf_tiny("Buffer_0 cannot be deleted using this command, please type the next character\n\r");
        return;
    }
    while (temp != NULL && temp->bufferNo != position)  //Traverse the list until the position matches the buffer number to be deleted
    {
        prev = temp;        //Store current buffer in previous
        temp = temp->next;  //Increment the current buffer to point to next buffer
    }

    if (temp == NULL)       //Check if the user is trying to delete a non-existent buffer
    {
        printf_tiny("Please enter a valid buffer number to delete \n\r");
        return;
    }
    printf_tiny("\n\rSuccessfully deleted buffer_%d\n\r", temp->bufferNo);
    prev->next = temp->next;       //Relink the buffers after deleting a buffer

    free(temp);                    //Free the buffer

    temp->characters = NULL;       //Point the buffer to NULL
    temp = NULL;                   //Point the metadata of the buffer to NULL
}

void deleteAll(struct buff **head_ref)
{
    DEBUGPORT(6);      //Call DEBUG
    struct buff* current = *head_ref; //Create a temporary node to store current buffer
    struct buff* next;                //Create a next node to point to next buffer
    while (current != NULL)           //Loop until all the buffers are deleted
    {
        next = current->next;          //next points to the next buffer
        free(current->characters);     //Free current buffer
        free(current);                 //Free current metadat
        current->characters = NULL;    //Point the buffer pointer to NULL
        current = NULL;                //Point the metadata pointer to NULL
        current = next;                //Increment current to store the next buffer
    }
    *head_ref = NULL;                  //Declare head as NULL after deleting all buffers
    printf("\n\r Deleted all buffers, starting the program again\n\r");
}

void reallocate()
{
    DEBUGPORT(7);
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
    printf_tiny("Successfully allocated the buffers\n\r");
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
    lastCount = 0;                              //Initialize the lastcharacter count to 0 in the beginning
    DEBUGPORT(8);                               //Call DEBUG
    uint16_t ch, i = 0, j = 0;
    reallocate();								//Allocate buffer_0 and buffer_1 successfully

    buffer_0->storedCharacters = 0;             //Initialise storedcharacter count to 0
    buffer_0->characterCount = 0;				//Initialise character count of buffer_0
    printf_tiny("Enter the characters\n\r");    //Start inputting characters serially
    do
    {
        ch = getchar();                         								//Store each character
        (buffer_0->characterCount)++;           							    //Increment character count after each input
        buffer_0->freeCharacters = buffer_0->size - buffer_0->storedCharacters; //Calculate free space
        if (ch >= 97 && ch <= 122)												//Store the lowercase characters in buffer_0
        {
            if (buffer_0->storedCharacters >= buffer_0->size)					//If buffer_0 limit is reached exho the characters
            {
                putchar(ch);
            }
            else																//If buffer limit is not reached store the lower characters
            {
                buffer_0->characters[buffer_0->storedCharacters] = ch;
                putchar(ch);
                (buffer_0->storedCharacters)++;
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
        else if (ch == '?')														//Display the current buffer details and empty out buffer_0
        {
            putchar(ch);
            questionMark();
            buffer_0->storedCharacters = 0;										//To start writing into buffer_0 again
            continue;
        }
        else if (ch == '@')														//Execute functions to delete all buffers and start the program again
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
    }while (1);                                                        //Continue reading serial inputs
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
    DEBUGPORT(9);                //Call DEBUG
    while ((SCON & 0x01) == 0);  // wait for character to be received, spin on RI
	RI = 0;			// clear RI flag
	return SBUF;  	// return character from SBUF
}



