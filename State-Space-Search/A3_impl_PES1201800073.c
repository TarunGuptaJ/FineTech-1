#include<stdio.h>
#include "A3_header_PES1201800073.h"
#include<Stdlib.h>

// The function to get the maximum capacity of the jugs
void get_upper_limit(state_t* ptr_state,int Amax,int Bmax,int Cmax)
{
    ptr_state->A = Amax;
    ptr_state->B = Bmax;
    ptr_state->C = Cmax;
    ptr_state->fun_ind = -1;    //Made -1 so that it can be distinguished
}

// A set_state function for assigning the values for state
void set_state(state_t* ptr_state,int A_val,int B_val,int C_val)
{
    ptr_state->A = A_val;
    ptr_state->B = B_val;
    ptr_state->C = C_val;
    ptr_state->fun_ind = 0;
}

// Display functions for displaying the state
void display_state(const state_t* ptr_state)
{
    printf("%d %d %d\n",
        ptr_state->A,
        ptr_state->B,
        ptr_state->C
    );
}

// Below are six functions each defined for each transition
// The transitions being A->B,A->C,B->A,B->C,C->A,C->B
// The functions taken in three arguments that is source, destination and max_capacity of the jugs
// In my implementation, I have not used a is_valid function and I have implemented it in such a way that a wrong transition cannot
// take place. For example if we were to pour a 8 ltr to a 5 ltr jug I return 0 at that point  
int transAB(const state_t* src,state_t* dst,const state_t* constraints)
{
    int spaceB = constraints->B - src->B;   //Checking the space left in the jug to be poured
    if(spaceB == 0 || src->A == 0)  //If space available in the jug to be poured is zero or there is no content in the src jug, return 0 as it cannot be a valid transition
    {
        return 0;
    }
    else if(src->A >= spaceB)   //If there is more content in the src jug than the space
    {
        dst->A = src->A - spaceB;   //There will be content in the jug (from which we pour)
        dst->B = constraints->B;    //The final jug will be filled to its max capacity
    }
    else if(src->A < spaceB)    //If the space available is more than the content from the src jug
    {
        dst->A = 0;     //The jug becomes empty
        dst->B = src->B + src->A;   //The destination is added with its already available content
    }
    dst->C = src->C;    //This jug is untouched in this transition
    dst->fun_ind = 0;   //Function index always has to be zero because this is the function that has to be next applied
    return 1;   //Returning 1 because it is a valid transition if it reached till here

}

// The next 5 transition function are same as the above one and hence I have not included comments for the next 5 transition functions
int transAC(const state_t* src,state_t* dst,const state_t* constraints)
{
    int spaceC = constraints->C - src->C;
    if(spaceC == 0 || src->A == 0)
    {
        return 0;
    }
    else if(src->A >= spaceC)
    {
        dst->A = src->A - spaceC;
        dst->C = constraints->C;
    }
    else if(src->A < spaceC)
    {
        dst->A = 0;
        dst->C = src->C + src->A;
    }
    dst->B = src->B;
    dst->fun_ind = 0;
    return 1;
    
}

int transBA(const state_t* src,state_t* dst,const state_t* constraints)
{
    int spaceA = constraints->A - src->A;
    if(spaceA == 0 || src->B == 0)
    {
        return 0;
    }
    else if(src->B >= spaceA)
    {
        dst->B = src->B - spaceA;
        dst->A = constraints->A;
    }
    else if(src->B < spaceA)
    {
        dst->B = 0;
        dst->A = src->A + src->B;
    }
    dst->C = src->C;
    dst->fun_ind = 0;
    return 1;
}

int transBC(const state_t* src,state_t* dst,const state_t* constraints)
{
    int spaceC = constraints->C - src->C;
    if(spaceC == 0 || src->B == 0)
    {
        return 0;
    }
    else if(src->B >= spaceC)
    {
        dst->B = src->B - spaceC;
        dst->C = constraints->C;
    }
    else if(src->B < spaceC)
    {
        dst->B = 0;
        dst->C = src->C + src->B;
    }
    dst->A = src->A;
    dst->fun_ind = 0;
    return 1;
}

int transCA(const state_t* src,state_t* dst,const state_t* constraints)
{
    int spaceA = constraints->A - src->A;
    if(spaceA == 0 || src->C == 0)
    {
        return 0;
    }
    else if(src->C >= spaceA)
    {
        dst->C = src->C - spaceA;
        dst->A = constraints->A;
    }
    else if(src->C < spaceA)
    {
        dst->C = 0;
        dst->A = src->A + src->C;
    }
    dst->B = src->B;
    dst->fun_ind = 0;
    return 1;

}

int transCB(const state_t* src,state_t* dst,const state_t* constraints)
{
    int spaceB = constraints->B - src->B;
    if(spaceB == 0 || src->C == 0)
    {
        return 0;
    }
    else if(src->C >= spaceB)
    {
        dst->C = src->C - spaceB;
        dst->B = constraints->B;
    }
    else if(src->C < spaceB)
    {
        dst->C = 0;
        dst->B = src->B + src->C;
    }
    dst->A = src->A;
    dst->fun_ind = 0;
    return 1;
}

// To compare if 2 states are same the function index need not be checked because the content of the jugs matter 
// and not the function that was applied in order to reach that state
int are_same(const state_t* ptr1,const state_t* ptr2)
{
    return ptr1->A == ptr2->A && ptr1->B == ptr2->B && ptr1->C == ptr2->C;
}

// Doubly linked list implementation
// Function for initiating the list
void init_list(list_t *ptr_list)
{
	ptr_list->head = NULL;
	ptr_list->tail = NULL;
}

// Function to add a state at the end of the doubly linked list
void add_at_end(list_t *ptr_list, const state_t* ptr_state)
{
	node_t* temp = (node_t*)malloc(sizeof(node_t));
	temp->st = *ptr_state;
	// Checking if the dooubly linked list is empty
    // And making appropriate changes to head and tail
	if(ptr_list->head == NULL)
	{
		temp->prev = ptr_list->tail;
		ptr_list->head = temp;
		ptr_list->tail = temp;
		temp->next = NULL;
	}
	else    // The else condition takes care if the doubly linked is not empty, and makes appropriate changes to the tail and last node
	{
		temp->prev = ptr_list->tail;
		ptr_list->tail->next = temp;
		ptr_list->tail = temp;
		temp->next = NULL;
	}

}
void remove_at_end(list_t *ptr_list)
{
	// If it is empty we cannot remove
	if(ptr_list->head == NULL)
	{
		return;
	}
	else if(ptr_list->head == ptr_list->tail)   // If the DLL contains only one node the conditions are different as we need to change head and tail
	{
		free(ptr_list->head);
		ptr_list->head = ptr_list->tail = NULL;
	}
	else    // If the DLL contains more than one node we need not change head
	{
		node_t *temp = ptr_list->tail;
		ptr_list->tail = temp->prev;
		ptr_list->tail->next = NULL;
		free(temp);
	}
	
}

// A function to check if the state had already been reached before
int is_repeated(list_t *ptr_list, const state_t* ptr_state)
{
	node_t* temp = ptr_list->head;
	while(temp)     //For iterating through the DLL
	{
		if(are_same(&temp->st, ptr_state))  //If it is found anywhere in between return 1 to signify that it has been repeated
			return 1;
		temp = temp->next;      //To move to the next node
	}
	return 0;   //Reaches this condition only if it wasnt repeated
}

// A function to iterate through the DLL and print all the states by calling the display_state function 
void disp_list(const list_t *ptr_list)
{
	node_t* temp = ptr_list->head;
	while(temp)     //For iterating through the DLL
	{
		display_state(&temp->st);
		temp = temp->next;      //To move to the next node
	}
}

