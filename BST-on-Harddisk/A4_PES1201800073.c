#include "assignment_4.h"
#include <stdio.h>
#include <stdlib.h>

FILE* init_tree(const char* filename)
{
    FILE *fp;
    fp = fopen(filename,"r+b");     //If file already exists opening in read mode
    if(fp == NULL)
    {
        fp = fopen(filename,"w+b");     //If files doesnt exist creating the file and inserting the tree head node inside
        fseek(fp,0,SEEK_SET);           //by initializing default grounded values
        tree_t* temp = (tree_t*)malloc(sizeof(tree_t));
        temp->free_head = -1;
        temp->root = -1;
        fwrite(temp,sizeof(tree_t),1,fp);
        free(temp);
    }
    return fp;
}
void close_tree(FILE *fp)
{
    // Closing the file to make sure 
    fclose(fp);
}

// The inorder_Rec function was created because in the display_inorder function there was no offset, relative position of
// the root
void inorder_rec(int root,FILE* fp)
{
    if(root == -1)
        return;
    node_t temp_node;   //Declaring a temp that acts like root in this call
    fseek(fp,root,SEEK_SET);
    fread(&temp_node,sizeof(node_t),1,fp);
    //Standard recursive calls for inorder 
    inorder_rec(temp_node.left_offset,fp);
    printf("%d ",temp_node.key);
    inorder_rec(temp_node.right_offset,fp);
    return;
}

void display_inorder(FILE *fp)
{
    // In this function there is no offset that could be used to make a temporary root to make a recursive call
    // For that reason we we read the details from the first header node and pass the roots offset as
    // the function parameters
    tree_t temp;
    fseek(fp,0,SEEK_SET);
    fread(&temp,sizeof(tree_t),1,fp);
    int index = temp.root;
    inorder_rec(index,fp);
    printf("\n");
}

void preorder_rec(int root,FILE* fp)
{
    // Similarly defining a function for preorder traversal to use the offset parameter inorder to make recursive calls
    // with roots of different subtrees
    if(root == -1)
        return;
    node_t temp_node;
    fseek(fp,root,SEEK_SET);
    fread(&temp_node,sizeof(node_t),1,fp);
    //Standard recursive call for preorder 
    printf("%d ",temp_node.key);
    preorder_rec(temp_node.left_offset,fp);
    preorder_rec(temp_node.right_offset,fp);
    return;
}

void display_preorder(FILE *fp)
{
    // Function that reads the details from the header node and calls/sends it to the starting of the recursive calls
    tree_t temp;
    fseek(fp,0,SEEK_SET);
    fread(&temp,sizeof(tree_t),1,fp);
    int index = temp.root;
    preorder_rec(index,fp);
    printf("\n");
    return;
}

// Standard createnode function that is used only in the place of inserting a newnode
// Intializes values with grounded state, that is setting left offset and right offset to -1
node_t* createnode(int key)
{
    node_t* temp =(node_t*)malloc(sizeof(node_t));
    temp->key = key;
    temp->left_offset = -1;
    temp->right_offset = -1;
    return temp;
}

// Function to insert into the BST
void insert_key(int key, FILE *fp)
{
    // Loading the head of the file, the tree node
    fseek(fp,0,SEEK_SET);
    tree_t temp;
    fread(&temp,sizeof(tree_t),1,fp);
    node_t* newnode = createnode(key);
    //This condition is for inserting into a new file, that is no insertions/deletions have happened (empty BST)
    if(temp.root == -1 &&  temp.free_head == -1)    
    {
        fseek(fp,0,SEEK_END);
        temp.root = ftell(fp);
        fwrite(newnode,sizeof(node_t),1,fp);
        
        fseek(fp,0,SEEK_SET);
        fwrite(&temp,sizeof(tree_t),1,fp);
    }
    else if(temp.root == -1 && temp.free_head !=-1)
    {
        //This condition is for inserting into the BST when all the nodes have been deleted (Empty BST but not new file)
        // Only in this condition and the previous one we need to write back the root
        int p_off = temp.free_head;
        node_t q;
        fseek(fp,p_off,SEEK_SET);
        fread(&q,sizeof(node_t),1,fp);
        temp.free_head = q.left_offset;
        temp.root = p_off;
        fseek(fp,p_off,SEEK_SET);
        fwrite(newnode,sizeof(node_t),1,fp);

        fseek(fp,0,SEEK_SET);
        fwrite(&temp,sizeof(tree_t),1,fp);
    }
    else
    {
        // This condition is the general condition to insert into BST, that is inserting into a non empty BST
        // In this we dont need to write back the tree node

        //This part of the code is to find the parent of the node that is to be inserted
        int prev = 0;
        int present = temp.root;
        node_t temp_node;
        while(present!=-1)
        {
            fseek(fp,present,SEEK_SET);
            fread(&temp_node,sizeof(node_t),1,fp);
            prev = present;
            if(key == temp_node.key)    //This condition is to exit the function if the node has already been inserted
            {
                // printf("cant insert %d\n",key);
                return;
            }
            else if(key>temp_node.key)
                present = temp_node.right_offset;
            else if(key<temp_node.key)
                present = temp_node.left_offset;
            
        }
        
        // The previous part was to find the position of the key wrt the BST
        // Now we have to check conditions to see where we have to write it to in the file  
        int index = -1;
        if(temp.free_head == -1)    
        {
            //If freehead == -1 we just have to insert it at the end
            fseek(fp,0,SEEK_END);
            index = ftell(fp);
            fwrite(newnode,sizeof(node_t),1,fp);
        }
        else
        {
            // If free head isnt -1 that means some deletion has taken place we have to go to that offset and insert it
            // and I have used my left offset for keeping track so I reassign it and have to write back all editings 
            // back to the file
            fseek(fp,temp.free_head,SEEK_SET);
            index = ftell(fp);

            node_t free_update_node;
            fread(&free_update_node,sizeof(node_t),1,fp);
            temp.free_head = free_update_node.left_offset;

            // printf("keyy %d\n",free_update_node.key);

            fseek(fp,index,SEEK_SET);
            fwrite(newnode,sizeof(node_t),1,fp);
            
            fseek(fp,0,SEEK_SET);
            fwrite(&temp,sizeof(tree_t),1,fp);
        }
        
        //After inserting we would have updated the parents left or right offset so we need to write it back in the file
        fseek(fp,prev,SEEK_SET);
        fread(&temp_node,sizeof(node_t),1,fp);
        //The below condition is to check if we inserted it to the right / left offset of the parent
        if(temp_node.key > key)
            temp_node.left_offset = index;
        else
            temp_node.right_offset = index;
        fseek(fp,prev,SEEK_SET);
        fwrite(&temp_node,sizeof(node_t),1,fp);
    }
}

// This function was used as a helper function that returns the index(offset) of the parent, given root's offset and node key value
// This is utilized in the delete function, returns the parent offset
int parent(int key,int offset,FILE* fp)
{
    // Loading the offset that is the root," The naming offset is misleading " 
    node_t temp_node;
    int prev = -1;
    int present = offset;
    while(present!=-1)
    {
        // Checks conditions and checks the right Subtree if the key value is greater
        // checks the left subtree if the key value is lesser
        fseek(fp,present,SEEK_SET);
        fread(&temp_node,sizeof(node_t),1,fp);
        if(key == temp_node.key)
            break;
        else if(key>temp_node.key)
        {
            prev = present;
            present = temp_node.right_offset;
        }
        else if(key<temp_node.key)
        {
            prev = present;
            present = temp_node.left_offset;
        }
    }
    return prev;
}

// This is a helper function utilized in delete which is used to find the inorder successor that is one right offset
// And iterate till the left most edge that is iterating through the left offset returns the inorder successor offset
int insucc(int offset,FILE* fp)
{
    node_t temp_node;
    fseek(fp,offset,SEEK_SET);
    fread(&temp_node,sizeof(node_t),1,fp);

    int index = temp_node.right_offset;     // Moving one right in the BST  
    fseek(fp,index,SEEK_SET);
    fread(&temp_node,sizeof(node_t),1,fp);

    //Moving left while left offset is not equal to -1 that is leftmost child
    while(temp_node.left_offset!=-1)
    {
        index = temp_node.left_offset;
        fseek(fp,index,SEEK_SET);
        fread(&temp_node,sizeof(node_t),1,fp);
    }
    return index;
}

void delete_rec(int root,int key,FILE* fp)
{
    // Finding the parent of the given key, this wasnt accomplished by the parent function
    // because I had to return more than one parameter and hence written iteratively in here, same logic as int parent function
    node_t temp_node;
    int prev = -1;
    int present = root;
    while(present!=-1)
    {
        fseek(fp,present,SEEK_SET);
        fread(&temp_node,sizeof(node_t),1,fp);
        if(key == temp_node.key)
            break;
        else if(key>temp_node.key)
        {
            prev = present;
            present = temp_node.right_offset;
        }
        else if(key<temp_node.key)
        {
            prev = present;
            present = temp_node.left_offset;
        }
    }
    // Set conditions such that if present == -1 that means the element doesnt exist so the delete doesnt take place
    if(present == -1)
    {
        return;
    }
    
    int flag = 1; //This flag is used at a later part of the code that decides what part needs to written back to the file
    node_t prev_node,present_node;
    fseek(fp,present,SEEK_SET);
    fread(&present_node,sizeof(node_t),1,fp);
    
    // I have split my delete function into 2 segments one is deleting the root node and one is deleting non root nodes
    // This redundancy was done only because different parts of the file had to be updated
    if(prev!=-1)
    {
        fseek(fp,prev,SEEK_SET);
        fread(&prev_node,sizeof(node_t),1,fp);

        //The following next 3 if/elif conditions are for deleting nodes that have one or no child
        if(present_node.left_offset == -1 && present_node.right_offset == -1)
        {
            if(present_node.key>prev_node.key)
                prev_node.right_offset = -1;
            else
                prev_node.left_offset = -1;
        }
        else if(present_node.left_offset == -1 && present_node.right_offset !=-1)
        {
            if(present_node.key>prev_node.key)
                prev_node.right_offset = present_node.right_offset;
            else
                prev_node.left_offset = present_node.right_offset;
        }
        else if(present_node.left_offset !=-1 && present_node.right_offset ==-1)
        {
            if(present_node.key>prev_node.key)
                prev_node.right_offset = present_node.left_offset;
            else
                prev_node.left_offset = present_node.left_offset;
        }
        else
        {
            //This segment of the code is to delete the nodes that have 2 children
            // Getting the parent of the node to be deleted
            node_t parent_node;
            int parent_index = parent(key,root,fp);
            fseek(fp,parent_index,SEEK_SET);
            fread(&parent_node,sizeof(node_t),1,fp);
            // printf("parent key %d\n",parent_node.key);

            //Getting the inorder successor of the node to be deleted because it has to replace the parents position in the BST
            node_t insucc_node;
            int insucc_index = insucc(present,fp);
            fseek(fp,insucc_index,SEEK_SET);
            fread(&insucc_node,sizeof(node_t),1,fp);
            // printf("insucc key %d\n",insucc_node.key);
            
            //Getting the parent of the inorder successor because changes have to made as that node is being deleted
            node_t psucc_node;
            int psucc_index = parent(insucc_node.key,root,fp);
            fseek(fp,psucc_index,SEEK_SET);
            fread(&psucc_node,sizeof(node_t),1,fp);
            // printf("psucc key %d\n",psucc_node.key);
            
            //This condition is to check if the inorder successor is the immediate right child of the root 
            // As it has a slight different change in parameters
            if(psucc_index == present)
            {
                if(parent_node.left_offset == present)
                    parent_node.left_offset = insucc_index;
                else
                    parent_node.right_offset = insucc_index;

                insucc_node.left_offset = present_node.left_offset;
                   
            }
            else
            {
                // This part is to reassign the insucc's parents offsets, this is done first as the insucc will never have 2 children
                // Then the insucc's offsets are set to the node that is being deleted, it also has to change the offsets of the parent
                // nodes of the node to be deleted
                psucc_node.left_offset = insucc_node.right_offset;

                if(parent_node.left_offset == present)
                    parent_node.left_offset = insucc_index;
                else
                    parent_node.right_offset = insucc_index;
                

                insucc_node.left_offset = present_node.left_offset;
                insucc_node.right_offset = present_node.right_offset;

                fseek(fp,psucc_index,SEEK_SET);
                fwrite(&psucc_node,sizeof(node_t),1,fp);
                
            }
            // Writing back all the updates
            fseek(fp,insucc_index,SEEK_SET);
            fwrite(&insucc_node,sizeof(node_t),1,fp);

            fseek(fp,parent_index,SEEK_SET);
            fwrite(&parent_node,sizeof(node_t),1,fp);
            flag = 0;
            // Now to reduce redundancy / inorder not to write back the previous node that hasnt been updated
            // The redundancy is prev and parent they are the same
        }
        if(flag)
        {
            fseek(fp,prev,SEEK_SET);
            fwrite(&prev_node,sizeof(node_t),1,fp);
        }
        // We write back along with updating the freehead
        tree_t temp_wb;
        fseek(fp,0,SEEK_SET);
        fread(&temp_wb,sizeof(tree_t),1,fp);

        node_t temp_wb_node;
        fseek(fp,present,SEEK_SET);
        fread(&temp_wb_node,sizeof(node_t),1,fp);

        temp_wb_node.left_offset = temp_wb.free_head;
        temp_wb.free_head = present;

        fseek(fp,present,SEEK_SET);
        fwrite(&temp_wb_node,sizeof(node_t),1,fp);

        fseek(fp,0,SEEK_SET);
        fwrite(&temp_wb,sizeof(tree_t),1,fp);
        return;
        
    }
    else if(prev == -1)
    {
        // Now this subsection is the same as the previous one with very minor changes as 
        // The node to be deleted is the root node
        tree_t temp_root_wb;
        fseek(fp,0,SEEK_SET);
        fread(&temp_root_wb,sizeof(tree_t),1,fp);
        if(present_node.left_offset == -1 && present_node.right_offset == -1)
            temp_root_wb.root = -1;
        else if(present_node.left_offset == -1 && present_node.right_offset != -1) 
            temp_root_wb.root = present_node.right_offset;
        else if(present_node.left_offset != -1 && present_node.right_offset == -1)
            temp_root_wb.root = present_node.left_offset;
        else
        {
            // The small change is that we dont need to change the node's parent as it is the root node
            node_t insucc_node;
            int insucc_index = insucc(present,fp);
            fseek(fp,insucc_index,SEEK_SET);
            fread(&insucc_node,sizeof(node_t),1,fp);
            // printf("insucc key %d\n",insucc_node.key);
            
            node_t psucc_node;
            int psucc_index = parent(insucc_node.key,root,fp);
            fseek(fp,psucc_index,SEEK_SET);
            fread(&psucc_node,sizeof(node_t),1,fp);
            // printf("psucc key %d\n",psucc_node.key);

            if(psucc_index == present)
            {
                temp_root_wb.root = insucc_index;

                insucc_node.left_offset = present_node.left_offset;
            }
            else
            {
                psucc_node.left_offset = insucc_node.right_offset;

                insucc_node.left_offset = present_node.left_offset;
                insucc_node.right_offset = present_node.right_offset;

                temp_root_wb.root = insucc_index;

                fseek(fp,psucc_index,SEEK_SET);
                fwrite(&psucc_node,sizeof(node_t),1,fp);
            }
            fseek(fp,insucc_index,SEEK_SET);
            fwrite(&insucc_node,sizeof(node_t),1,fp);
            
        }
        // Making writebacks with updating freehead
        node_t temp_root_node_wb;
        fseek(fp,present,SEEK_SET);
        fread(&temp_root_node_wb,sizeof(node_t),1,fp);

        temp_root_node_wb.left_offset = temp_root_wb.free_head;
        temp_root_wb.free_head = present;

        fseek(fp,present,SEEK_SET);
        fwrite(&temp_root_node_wb,sizeof(node_t),1,fp);

        fseek(fp,0,SEEK_SET);
        fwrite(&temp_root_wb,sizeof(tree_t),1,fp);
    }
}

void delete_key(int key, FILE *fp)
{
    // This was initially done with attempting recursive delete but changed implementation mid way, unrequired call
    fseek(fp,0,SEEK_SET);
    tree_t temp;
    fread(&temp,sizeof(tree_t),1,fp); 
    int root = temp.root;
    delete_rec(root,key,fp);
}