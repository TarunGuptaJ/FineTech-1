#include "assignment_2.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// int main()
// {
//     int n = 0;
//     scanf("%d",&n);
//     for(int i = 0; i < n; ++i)
//     {
//         int length = 0;
//         scanf("%d",&length);
//         int *arr = (int *)calloc(length,sizeof(int));
//         for(int j = 0; j < length; ++j)
//             scanf("%d",&arr[j]);
//         find_second_greatest(arr,length);
//         free(arr);
//         printf("\n");
//     }
//     return 0;
// }

int main()
{
    // int a = ceil(log2(7));
    // printf("%d",a);
    // Check for 9,10,11
    // Check for 6,7
    // Matrix requirement size
    // Run code with -lm in the end for linking math
    
    int length = 0;
    scanf("%d",&length);
    int* array1 = (int*)calloc(length,sizeof(int));
    for(int i = 0;i<length;i++)
    {
        scanf("%d",&array1[i]);
    }
    printf("\n");
    find_second_greatest(array1,length);
    
    
}
