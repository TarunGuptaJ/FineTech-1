#ifndef A3_H
#define A3_H
// Defining the state structures and functionality
struct state
{
    int A;
    int B;
    int C;
    int fun_ind;
};
typedef struct state state_t;

// Functions for displaying the states and setting the states, that is assigning the corresponding values 
void set_state(state_t* ptr_state,int A_val,int B_val,int C_val);
void display_state(const state_t* ptr_state);

// Functions that will be used in the callback array that is the transition functions, for example pouring from jug A to jug B 
// Will call transAB, Sending three arguments for knowing the capacity of the jugs (Constraints)
int transAB(const state_t* src,state_t* dst,const state_t* constraints);
int transAC(const state_t* src,state_t* dst,const state_t* constraints);
int transBA(const state_t* src,state_t* dst,const state_t* constraints);
int transBC(const state_t* src,state_t* dst,const state_t* constraints);
int transCA(const state_t* src,state_t* dst,const state_t* constraints);
int transCB(const state_t* src,state_t* dst,const state_t* constraints);

// The function is responsible for utilizing one of the states for holding the max capacity of the jugs
// Making one separate state for holding maximum values
void get_upper_limit(state_t* ptr_state,int Amax,int Bmax,int Cmax);
int are_same(const state_t* ptr1,const state_t* ptr2);

// Defining the doubly linked list structures and functionality
// Structure for the node that holds the state and is used for doubly linked list working
struct node
{
	state_t st;
	struct node* prev;
	struct node* next;
};
typedef struct node node_t;

// The equivalent head structure that stores the head and tail
struct list
{
	node_t* head;
	node_t* tail;
};

// The following functions are for using the doubly linked list for accomplishing backtracking
typedef struct list list_t;
void init_list(list_t *ptr_list);
void add_at_end(list_t *ptr_list, const state_t* ptr_state);
void remove_at_end(list_t *ptr_list);
int is_repeated(list_t *ptr_list, const state_t* ptr_state);
void disp_list(const list_t *ptr_list);

#endif 