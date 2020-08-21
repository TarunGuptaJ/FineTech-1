#include<stdio.h>
#include "assignment_2.h"
#include<math.h>
#include<stdlib.h>

//Below is the structure of node that will be present in the adjacency list
typedef struct node
{
    int data;
    struct node* next;
}node;

//Function to create a node for the adjacency list
node* createnode(int num)
{
    node* temp = (node*)malloc(sizeof(node));
    temp->data = num;
    temp->next = NULL;
    return temp;
}

//Function that takes input a node address and it frees all the nodes
//starting from the given until the end of the corresponding linked list
void free_nodes(node* temp1)
{
    while(temp1!=NULL)  //Loop condition
    {
        node* temp = temp1;
        temp1 = temp1->next;
        free(temp);
    }
}

//Function that takes input the starting node address and prints the contents
//of the linked list 
void print_list(node* temp1)
{
    while(temp1!=NULL)  //Loop condition
    {
        printf("%d ",temp1->data);
        temp1=temp1->next;
    }
}

// Function to print current state of adjacency list 
// void print_struct(node** temp1,int length)
// {
//     for(int i = 0;i<ceil(log2(length)) + 1;++i)
//     {
//         printf("%d :",i);
//         print_list(temp1[i]);
//         printf("\n");
//     }
// }

void find_second_greatest(int *numbers, int length)
{
    if(length == 1) //Checking for exit condition if there is no second greatest element possible
    {
        return;
    }
    int n = ceil(log2(length)) + 1;//Length of the array of structure pointers required for creating the adjacency list 
    node** array_structs = (node**)malloc(sizeof(node)*n);//Allocating memory for the array of structure pointers 
    for(int i = 0;i<n;++i)
        array_structs[i] = NULL;//Initializing the values with NULL
    node* compete;
    
    for(int i = 0;i<length;++i)//Iterating through the entire array of given numbers
    {
        node* temp = createnode(numbers[i]);
        int iter = 0;
        if(array_structs[iter] == NULL)//Checking the condition if there is no element in the first place the element can be directly placed there
        {
            array_structs[iter] = temp;
        }
        else
        {
            compete = (node*)temp;//Assigning the newly created node to a temp variable because the competitions maybe cascaded
            while(iter<n && array_structs[iter]!=NULL)//The looping conditions for the occurences of competitions
            {
                //print_list is the function that is being called for printing the linked list data structure
                print_list(array_structs[iter]);
                printf("x ");
                print_list(compete);
                printf("= ");
                
                //Checking the condition for determining the winner of the competition
                //Everytime the losing element(the top most) is added as the second element of the the winning linked list
                //and then it is added back / assigned back to the the compete variable
                if(array_structs[iter]->data < compete->data)
                {
                    //In this block the compete top variable wins and the losing element is added as the second element
                    //and the rest are freed
                    free_nodes(array_structs[iter]->next);
                    array_structs[iter]->next = NULL;
                    array_structs[iter]->next = compete->next;
                    compete->next = array_structs[iter];
                    array_structs[iter] = NULL;
                }
                else if(array_structs[iter]->data > compete->data)
                {
                    //Whereas in this block the compete variable loses and the top most variable is added as the second
                    //element of the other linked list and the rest are freed accordingly and it is assigned back to the compete variable
                    free_nodes(compete->next);
                    compete->next = NULL;
                    compete->next = array_structs[iter]->next;
                    array_structs[iter]->next = compete;
                    compete = array_structs[iter];
                    array_structs[iter] = NULL;
                }
                print_list(compete);
                printf("\n");
                iter++;
                

            }
            array_structs[iter] = compete;
            
        }
    }
    //The below set of code is for taking care of arrays of lengths that are not exact powers of 2

    int iter = 0;
    while(array_structs[iter]==NULL)    //Finding the first non null linked available in the adjacency list
    {   
        iter++;
    }
    compete = (node*)array_structs[iter++];
    while(iter<n)   //changing of the loop condition from the above segment because NULL linked lists may occur in between
    {               //not null linked lists of the adjacency lists
        if(array_structs[iter]==NULL)
            iter++;
            
        else
        {
            print_list(array_structs[iter]);
            printf("x ");
            print_list(compete);
            printf("= ");
            //The same set of comparisons and working happens the result of the cascaded competitions is always written back to the
            //compete variable
            if(array_structs[iter]->data < compete->data)
            {
                free_nodes(array_structs[iter]->next);
                array_structs[iter]->next = NULL;
                array_structs[iter]->next = compete->next;
                compete->next = array_structs[iter];
                array_structs[iter] = NULL;
            }
            else if(array_structs[iter]->data > compete->data)
            {
                free_nodes(compete->next);
                compete->next = NULL;
                compete->next = array_structs[iter]->next;
                array_structs[iter]->next = compete;
                compete = array_structs[iter];
                array_structs[iter] = NULL;
            }
            print_list(compete);
            printf("\n");
            iter++;
        }
    }
    //This last segment of the code is used for finding the maximum element in the final competition winning linked list
    //disregarding the first element as that is the maximum element
    int max = compete->next->data;
    compete = (node*)(compete->next->next);
    while(compete!=NULL)
    {
        if(max<compete->data)
        {
            max = compete->data;
        }
        compete=(node*)compete->next;
    }
    printf("\n%d ",max);
}