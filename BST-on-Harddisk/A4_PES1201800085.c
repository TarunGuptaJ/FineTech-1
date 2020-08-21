#include "assignment_4.h"
// Creates the tree file if it doesn't exist
// Returns a pointer to the file with the header set
FILE* init_tree(const char* filename)
{
    FILE *fp = fopen(filename,"r+b");
    if(!fp)
    {
        fp = fopen(filename,"w+b");
        tree_t header;
        header.free_head = -1;
        header.root = -1;
        fwrite(&header,sizeof(tree_t),1,fp);
        fseek(fp,0,SEEK_SET);
    }
    return fp;
}

// Close the file pointed by fp
void close_tree(FILE *fp)
{
    fclose(fp);
}

// Helper Function to implement inorder recursively
static void inorder(FILE *fp)
{
    node_t curr;
    fread(&curr,sizeof(node_t),1,fp);
    // Prints left child as long as it exists
    if(curr.left_offset != -1)
    {
        fseek(fp,curr.left_offset,SEEK_SET);
        inorder(fp);
    }
    printf("%d ",curr.key);
    // Prints right child as long as it exists
    if(curr.right_offset != -1)
    {
        fseek(fp,curr.right_offset,SEEK_SET);
        inorder(fp);
    }
}
// Calls inorder function if the tree is not empty
// Prints ending \n
void display_inorder(FILE *fp)
{
    tree_t header;
    fread(&header,sizeof(tree_t),1,fp);
    if(header.root == -1)
    {
        fseek(fp,0,SEEK_SET);
        return;
    }
    else
    {
        fseek(fp,header.root,SEEK_SET);
        inorder(fp);
        printf("\n");
    }
    fseek(fp,0,SEEK_SET);
}
// Helper Function to implement preorder recursively
static void preorder(FILE *fp)
{
    node_t curr;
    fread(&curr,sizeof(node_t),1,fp);
    printf("%d ",curr.key);
    // Prints left child as long as it exists
    if(curr.left_offset != -1)
    {
        fseek(fp,curr.left_offset,SEEK_SET);
        preorder(fp);
    }
    // Prints right child as long as it exists
    if(curr.right_offset != -1)
    {
        fseek(fp,curr.right_offset,SEEK_SET);
        preorder(fp);
    }
}
// Calls preorder function if the tree is not empty
// Prints ending \n
void display_preorder(FILE *fp)
{
    tree_t header;
    fread(&header,sizeof(tree_t),1,fp);
    if(header.root == -1)
    {
        fseek(fp,0,SEEK_SET);
        return;
    }
    else
    {
        fseek(fp,header.root,SEEK_SET);
        preorder(fp);
        printf("\n");
    }
    fseek(fp,0,SEEK_SET);
}
// insert the key into the tree in the file
// if the key already exists just return
void insert_key(int key, FILE *fp)
{
    tree_t header;
    
    // Creates the new node with initial values
    node_t new_node;
    new_node.key = key;
    new_node.left_offset = -1;
    new_node.right_offset = -1;

    // Gets the data from the header and finds the root offset
    fseek(fp,0,SEEK_SET);
    fread(&header,sizeof(tree_t),1,fp);
    int curr_offset = header.root;

    // Empty Tree
    if(curr_offset == -1)
    {
        // Seeks to the end of the file first
        fseek(fp,0,SEEK_END);
        int insert_offset = ftell(fp);

        // Checks if any previous deletions have occurred
        if(header.free_head != -1)
        {
            int temp = -1;
            insert_offset = header.free_head;   // Finds the position of the previous deletion
            fseek(fp,insert_offset,SEEK_SET);
            fread(&temp,sizeof(int),1,fp);      // Next position in the list is stored as an integer in the previous offsets location
            header.free_head = temp;            // Update free head with the next value in the list
        }

        // Changes the root nodes position and updates the header
        header.root = insert_offset;        
        fseek(fp,0,SEEK_SET);
        fwrite(&header,sizeof(tree_t),1,fp);
        fseek(fp,insert_offset,SEEK_SET);
        fwrite(&new_node,sizeof(node_t),1,fp);  // Writes the new node
    }
    // Non-empty Tree
    else
    {
        // Sets up values to iterate through the tree
        int parent_offset = -1;
        node_t curr_node;
        node_t parent;
        while(curr_offset != -1)
        {
            // Seeks to the position of the current node
            fseek(fp,curr_offset,SEEK_SET);
            fread(&curr_node,sizeof(node_t),1,fp);

            // Updates parent value 
            parent_offset = curr_offset;

            // Moves current node to the left or right
            if(new_node.key > curr_node.key)
            {
                curr_offset = curr_node.right_offset;
            }
            else if(new_node.key < curr_node.key)
            {
                curr_offset = curr_node.left_offset;
            }

            // If the node already exists set file pointer to the beginning and return
            else
            {
                fseek(fp,0,SEEK_SET);
                return;
            }
        }
        // Seeks to the end of the file for insertion
        fseek(fp,0,SEEK_END);
        int insert_offset = ftell(fp);

        // Checks for previous deletions
        if(header.free_head != -1)
        {
            // Moves to free location and reads the offset of the next free location
            int temp = -1;
            insert_offset = header.free_head;
            fseek(fp,insert_offset,SEEK_SET);
            fread(&temp,sizeof(int),1,fp);

            // Updates the header with the new free location
            header.free_head = temp;
            fseek(fp,0,SEEK_SET);
            fwrite(&header,sizeof(tree_t),1,fp);
        }
        // Reads the parent node
        fseek(fp,parent_offset,SEEK_SET);
        fread(&parent,sizeof(node_t),1,fp);

        // Checks if new node should be to the left or right of the parent
        if(new_node.key < parent.key)
        {
            parent.left_offset = insert_offset;
        }
        else if(new_node.key > parent.key)
        {
            parent.right_offset = insert_offset;
        }

        // Writes the parent and new node to the file
        fseek(fp,parent_offset,SEEK_SET);
        fwrite(&parent,sizeof(node_t),1,fp);
        fseek(fp,insert_offset,SEEK_SET);
        fwrite(&new_node,sizeof(node_t),1,fp);
    }
    // Sets file pointer back to the beginning of the file
    fseek(fp,0,SEEK_SET);    
}
// delete the key from the tree in the file
// the key may or may not exist
void delete_key(int key, FILE *fp)
{
    // Declaration of values for tree traversal
    tree_t header;
    node_t parent;
    node_t curr_node;
    int parent_offset = -1;

    // Reading header info from the file
    fseek(fp,0,SEEK_SET);
    fread(&header,sizeof(tree_t),1,fp);

    // Checking if tree is empty
    if(header.root == -1)
    {
        fseek(fp,0,SEEK_SET);
        return;
    }
    // Reading the root node
    int curr_offset = header.root;
    fseek(fp,curr_offset,SEEK_SET);
    fread(&curr_node,sizeof(node_t),1,fp);

    // Iterating through the nodes until the node to be deleted is found
    while(curr_node.key != key && curr_offset != -1)
    {
        parent_offset = curr_offset;
        if(curr_node.key > key)
        {
            curr_offset = curr_node.left_offset;
        }
        else if(curr_node.key < key)
        {
            curr_offset = curr_node.right_offset;
        }
        fseek(fp,curr_offset,SEEK_SET);
        fread(&curr_node,sizeof(node_t),1,fp);
    }
    // Element Not Found
    if(curr_offset == -1)
    {
        fseek(fp,0,SEEK_SET);
        return;
    }
    // Sets the free offset to add to the free list
    int free_offset = curr_offset;

    // Leaf Node to be deleted
    if(curr_node.right_offset == -1 && curr_node.left_offset == -1)
    {
        // Checks if root node is to be deleted
        if(parent_offset == -1)
        {
            header.root = -1;
        }
        // Updates parent if the node is not the root node
        else
        {
            fseek(fp,parent_offset,SEEK_SET);
            fread(&parent,sizeof(node_t),1,fp);
            if(parent.left_offset == curr_offset)
            {
                parent.left_offset = -1;
            }
            else
            {
                parent.right_offset = -1;
            }
            fseek(fp,parent_offset,SEEK_SET);
            fwrite(&parent,sizeof(node_t),1,fp);
        }
    }
    // Node with two children is to be deleted
    else if (curr_node.left_offset != -1 && curr_node.right_offset != -1)
    {
        // psucc : Parent of Inorder successor
        // succ : Inorder successor
        node_t psucc, succ;
        int psucc_offset = curr_offset;
        int succ_offset = curr_node.right_offset;

        // Finding inorder successor of node to be deleted
        fseek(fp,succ_offset,SEEK_SET);
        fread(&succ,sizeof(node_t),1,fp);
        while(succ.left_offset != -1)
        {
            psucc_offset = succ_offset;
            succ_offset = succ.left_offset;
            fseek(fp,succ_offset,SEEK_SET);
            fread(&succ,sizeof(node_t),1,fp);
        }
        // Copying value of inorder successor to the current node
        curr_node.key = succ.key;

        // Reading parent of inorder successor
        fseek(fp,psucc_offset,SEEK_SET);
        fread(&psucc,sizeof(node_t),1,fp);

        // Updating free offset to inorder successors offset for free list
        free_offset = succ_offset;

        // Checking if the current element is the parent of the inorder successor
        if(psucc_offset == curr_offset)
        {
            curr_node.right_offset = succ.right_offset;
        }
        // Updating parent of successor
        else
        {
            psucc.left_offset = succ.right_offset;
            fseek(fp,psucc_offset,SEEK_SET);
            fwrite(&psucc,sizeof(node_t),1,fp);
        }
        // Updating the current node
        fseek(fp,curr_offset,SEEK_SET);
        fwrite(&curr_node,sizeof(node_t),1,fp);
    }
    // Node with one child is to be deleted
    else
    {
        // Initializing value of replacement of parent node
        int replacement = -1;

        // Setting replacement value to the left or right child
        if(curr_node.left_offset == -1)
        {
            replacement = curr_node.right_offset;
        }
        else
        {
            replacement = curr_node.left_offset;
        }

        // Checking if node to be deleted is root node
        if(parent_offset == -1)
        {
            header.root = replacement;
        }
        // Updating parent if node is not root
        else
        {
            fseek(fp,parent_offset,SEEK_SET);
            fread(&parent,sizeof(node_t),1,fp);
            if(parent.left_offset == curr_offset)
            {
                parent.left_offset = replacement;
            }
            else
            {
                parent.right_offset = replacement;
            }
            fseek(fp,parent_offset,SEEK_SET);
            fwrite(&parent,sizeof(node_t),1,fp);
        }
    }

    // Adding the free offset to the free list
    fseek(fp,free_offset,SEEK_SET);
    fwrite(&header.free_head,sizeof(int),1,fp);
    
    // Updating and writing the header to the file
    header.free_head = free_offset;
    fseek(fp,0,SEEK_SET);
    fwrite(&header,sizeof(tree_t),1,fp);

    // Resetting file pointer position
    fseek(fp,0,SEEK_SET);
}
