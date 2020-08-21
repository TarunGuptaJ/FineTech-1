#include<stdio.h>
#include<stdlib.h>
#include "A3_header_PES1201800073.h"

int main()
{
    int a,b,c;
    state_t constraints;    //Getting max capacity of the jugs
    scanf("%d %d %d",&a,&b,&c);
    get_upper_limit(&constraints,a,b,c);
    
    state_t start;      //Getting the start state
    scanf("%d %d %d",&a,&b,&c);
    set_state(&start,a,b,c);

    state_t goal;   //Getting the final/goal state
    scanf("%d %d %d",&a,&b,&c);
    set_state(&goal,a,b,c);

    //Constructing the array of functions that take 3 arguments
    int (*move[])(const state_t* src,state_t* dst,const state_t* constraints)={
        transAB,transAC,transBA,transBC,transCA,transCB
    };

    // Doubly linked list initializations
    list_t soln_list;
    init_list(&soln_list);
    add_at_end(&soln_list,&start);      //Adding start state to the doubly linked list (beginning)
    
    int soln = 0;   //Flag to check if a solution occured or not
    state_t temp;
    int fn_index;
    int count = 0;  //Counter for number of solutions
    // printf("\n");
    while(soln_list.tail!=NULL)
    {
        fn_index = soln_list.tail->st.fun_ind;  //Getting the function to be applied
        //The if condition is for checking if it is a valid move and gives a state that can be added,
        //It also checks if the state has already been added/reached before and if it already has it doesnt except 
        if(move[fn_index](&soln_list.tail->st,&temp,&constraints) && !is_repeated(&soln_list,&temp))
        {
            //If it satisfies both the conditions we add to the end of the list
            add_at_end(&soln_list,&temp);
            soln = are_same(&temp,&goal);   //To check if the goal has been reached and we return 1 if it has beem reached
        }
        else
        {
            //Now if it wasnt a valid move we check the below condition that is while the DLL is not empty and if the function index == 5 
            //We check this condition by pre incrementing and checking if it 6 ( that implies it was 5 when read)
            while(soln_list.tail!=NULL && ++soln_list.tail->st.fun_ind == 6)
            {
                //Now we remove the state
                remove_at_end(&soln_list);
            }
        }
        if(soln == 1)
        {
            //If the soln flag was set to one we display the order it was attained
            disp_list(&soln_list);
            ++count;    //Number of solutions incremented
            printf("\n");
            soln = 0;
            remove_at_end(&soln_list);
            //Safety removal check condition
            while(soln_list.tail!=NULL && ++soln_list.tail->st.fun_ind == 6 )
            {
                remove_at_end(&soln_list);
            }
        }
    }
    printf("%d\n",count);
}
