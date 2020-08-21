#include <stdio.h>
#include "assignment_4.h"

int main()
{
    FILE* fp5 = init_tree("tree.dat");

    int a;
    int n = 1300;
    for(int i = 0;i<n;++i)
    {
        scanf("%d",&a);
        insert_key(a,fp5);
    }
    for(int i = 0;i<n;++i)
    {
        scanf("%d",&a);
        delete_key(a,fp5);
    }
    for(int i = 0;i<n;++i)
    {
        scanf("%d",&a);
        insert_key(a,fp5);
    }
    // display_inorder(fp5); 
    for(int i = 0;i<n;++i)
    {
        // printf("delete %d insert %d\n",i,n-i);
        delete_key(i,fp5);
        insert_key(n-i,fp5);
    }
    for(int i = 0;i<n;++i)
    {
        // printf("insert %d\n",i);
        insert_key(i,fp5);
        // display_inorder(fp5);
        // getchar();
    }
    // for(int i = -2000;i<2000;++i)
    // {
    //     delete_key(i,fp5);
    // }
    // insert_key(25,fp5);
    // insert_key(15,fp5);
    // insert_key(50,fp5);

    // insert_key(10,fp5);
    // insert_key(22,fp5);
    // insert_key(35,fp5);

    // insert_key(70,fp5);
    // insert_key(4,fp5);
    // insert_key(12,fp5);

    // insert_key(18,fp5);
    // insert_key(24,fp5);
    // insert_key(31,fp5);

    // insert_key(44,fp5);
    // insert_key(66,fp5);
    // insert_key(90,fp5); 
       
    // // Deleting root
    // // delete_key(24,fp5);
    // insert_key(26,fp5);
    


    // display_inorder(fp5);
    display_inorder(fp5);
    display_preorder(fp5);
    // tree_t temp;
    // fseek(fp5,0,SEEK_SET);
    // fread(&temp,sizeof(tree_t),1,fp5);
    // printf("root %d freehead %d\n",temp.root,temp.free_head);
    // for(int i = 0;i<20;i++)
    // {
    //     node_t temp1;
    //     printf("index %d ",ftell(fp5));
    //     fread(&temp1,sizeof(node_t),1,fp5);
    //     printf("key %d left offset %d right offset %d\n",temp1.key,temp1.left_offset,temp1.right_offset);

    // }
    // fclose(fp5);
}   