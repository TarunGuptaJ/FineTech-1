#include<stdio.h>
#include<stdlib.h>
#include "assignment_1.h"

int size; // Defining to utilize the extern variable
char* p;

typedef struct book
{
    int flag;   //To check if the block is allocated/free
    int total;  //To store how much memory it stores
    int prevBook;   //Offset in the total memory, index it has to check for previous book 
    int nextBook;   //Offset to reach the next book
}book;

//Corresponding allocation is done to initialize the memory to be managed by a book and setting variables of the book
//appropriately
void allocate(int n)
{
    size = n;
    p = (char*)malloc(size*sizeof(char));
    book* first = (book*)p;
    first->flag = 0;
    first->total = n - sizeof(book);
    first->prevBook = -1;
    first->nextBook = -1;
}

void* mymalloc(int size1)
{
    int iter = 0;
    int min = size; //minimum value set for reference if there is no space available, the overall size of the memory
    int minimized = iter;
    //The while loop to iterate through the entire memory and find the best fit that is minimal wastage
    while(iter<size)
    {
        book* tem = (book*)(p+iter);
        // printf("%d %d\n",((book*)(temp+iter))->flag, ((book*)(temp+iter))->total);
        if(
            (tem)->flag == 0 &&
            (tem)->total>=size1 && 
            ((tem)->total) - size1 < min
        )
        {
            // printf("Entered\n");
            // min = ((book*)(temp+iter))->total - size1;
            min = tem->total - size1;
            minimized = iter;
        }
        iter = iter + sizeof(book) + (tem)->total;
    }

    //If the min variable remained size that means there was no place for the requested memory in that case it returns NULL.
    if(min == size)
        return NULL;

    book* tem = (book*)(p+minimized);
    
    //This if loop is entered if there is space for the requested memory but no place for a book
    if((tem)->total >= size1 && tem->total < (size1 + sizeof(book)))
    {
        // printf("No place for book\n");
        (tem)->flag = 1;
        return (p+minimized);
    }

    //This if loop is entered when there is enough space for the requested memory and there is enough space to accomodate a book
    if((tem)->total > size1 && tem->total > (size1 + sizeof(book)))
    {
        book* newBook = (book*)(p + minimized + sizeof(book) + size1);
        newBook->flag = 0;
        newBook->total = (tem)->total - sizeof(book) - size1;
        newBook->nextBook = -1;
        newBook->prevBook = sizeof(book) + size1;
        (tem)->flag = 1;
        (tem)->total = size1;

        (tem)->nextBook = sizeof(book) + (tem)->total;
        return (p+minimized);
    }

}

void print_book()
{
    // int iter = 0;
    // // book* temp = (book*)p;
    // int size_of_book = 0;
    // book* tem = (book*)(p+iter);
    // while(tem->nextBook != -1)
    // {
    //     tem = (book*)(p+iter);
    //     size_of_book+=sizeof(book);
    //     iter+=tem->nextBook;
    // }
    // if(size_of_book == 0)
    // {
    //     printf("%d\n",sizeof(book));
    //     return;
    // }
    // printf("%d\n",size_of_book);
    printf("%d\n",sizeof(book));
}

void myfree(void *b)
{
    if(b==NULL)
        return;
    int pos = (char*)b - p; //Obtaining position of the book in the memory 
    book* temfree = (book*)(p+pos);
    int tgf = temfree->prevBook;
    temfree->flag = 0;
    
    //Checking if the next book exists if yes obtaining the book and if free merging the 2 free spaces and making them be
    //refered to by one block
    if(temfree->nextBook!=-1)
    {
         book* temRight = (book*)(p+pos+temfree->nextBook);
         if(temRight->flag == 0)
         {
            temfree->total = temfree->total + sizeof(book) + temRight->total;
            // printf("Updated :%d\n",temfree->total);
            if(temRight->nextBook==-1)
                temfree->nextBook=-1;
            else
            {
                temfree->nextBook = temfree->nextBook + temRight->nextBook;
                book* changePrev = (book*)(p+pos+temfree->nextBook);
                changePrev->prevBook = temfree->total + sizeof(book);
                // printf("Updated Nextbook: %d\nPrevious :%d\n",pos+temfree->nextBook,pos+temfree->nextBook-changePrev->prevBook);
            }
            
        }
    }
    //Checking if previous exists and if yes obtaining the block and merging the 2 blocks and making appropriate changes in the 
    //bookkeeping structure
    if(temfree->prevBook!=-1)
    {
        book* temLeft = (book*)(p+pos-tgf);
        if(temLeft->flag == 0)
        {
            temLeft->total = temLeft->total + sizeof(book) + temfree->total;
            // printf("Total %d flag %d\n",temLeft->total,temLeft->flag);
            if(temfree->nextBook==-1)
                temLeft->nextBook = -1;
            else
            {
                temLeft->nextBook = sizeof(book) + temLeft->total;
                book* changePrev = (book*)(p+pos+temfree->nextBook);
                changePrev->prevBook = temfree->prevBook + sizeof(book) + temfree->total;
            }
            
        }
    }

}

void display_mem_map()
{
    int iter = 0;
    //Iterating through the memory by jumping to the location of the next books and correspondingly printing
    //free or located based on the value of the flag in the bookkeeping strucure.
    while(iter<=size)
    {
        printf("%d\t%d\t%s\n",iter,sizeof(book),"book");
        book* tem = (book*)(p+iter);
        if((tem)->flag == 0)
            printf("%d\t%d\t%s\n",iter+sizeof(book),(tem)->total,"free");
        else if((tem)->flag == 1)
            printf("%d\t%d\t%s\n",iter+sizeof(book),(tem)->total,"allocated");
        iter = iter + sizeof(book) + (tem)->total;
        if(iter >= size)
            break;//This break statement was necessary as the print statment took place before updation of tem
    }
    // print_book();
}