// !!!IMPORTANT!!!
// This assignment was written in Ubuntu 19.04.
// gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0 -> is the version of the gcc.
// Besides, the code files works very well in ubuntu and Moodle VPL,
// but the files didn't work in Windows. I request to consider the conditions, in the ungiven test cases.

//This file is the implementation of N-Ary Tree.
//We include some header files to use in make operations on constructed tree.
//we include "tree.h" file which is my implementation.
//I use the declared functions to fill their inside. 

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "tree.h"


//This function is get only a pointer of pointer node type. With that node,
//We recursively find the Depth of the given node.
//The given node can be the root or a node of the subtree of root.
//If the node pointer is null, it returns simply 0. Which is the base case.
//Else we called the method itself and if the returned integer is bigger than the maxdepth, maxdepth become that integer.
//then it return maxdepth +1 at the end.
int maxDepth(struct node **node)
{
    if (!*node)
        return 0;

    int maxdepth = 0;
    for (size_t i = 0; i < (*node)->children; i++)
    {
        int y = maxDepth(&(*node)->child[i]);
        if (y > maxdepth)
            maxdepth = y;
    }
    return maxdepth + 1;
}


//The functions print the node(s) informations according to given level integer variable.
//In the base case if the point of node is null it simply return nothing.
//If the given level is equal to 1 we write the into given outFileB information of that node.
//If given level is bigger than 1, I use for loop to go every child. 
//In the loop I recursively call the function itself with child node and level - 1.
   
void levelorder(struct node **node, int level,FILE *outFileB)
{
    if (*node == NULL)
        return;
    if (level == 1){
        fprintf(outFileB, "%s[%d]\n", convertType((*node)->type), (*node)->id);
        // printf("%s[%d]\n", convertType((*node)->type), (*node)->id);

    }
    else if (level > 1)
    {
        for (int i = 0; i < (*node)->children; i++)
        {
            levelorder(&(*node)->child[i], level - 1,outFileB);
        }
    }
}
//This function is used to convert enum node type to string.
//It used while writing output in file. 
char *convertType(enum nodetype type)
{
    switch (type)
    {
    case 0:
        return "SWITCH";
        break;
    case 1:
        return "ROUTER";
        break;
    case 2:
        return "INTERNET";
        break;
    case 3:
        return "SERVER";
        break;
    case 4:
        return "FIREWALL";
        break;
    case 5:
        return "WS";
        break;
    case 6:
        return "TERMINAL";
        break;
    case 7:
        return "IMAC";
        break;
    case 8:
        return "LAPTOP";
        break;
    case 9:
        return "PRINTER";
        break;
    }

    return "none";
}

//The function is used to find a node inside the tree with given id variable.
//The base case if the point of node is null, returns null.
//If the point of the node id is equal to given id, it returns that node.
//Else, we check the node childs by using a for loop called the function itself.
//If the called function is not null, we understand that we find the node with given id,
//and we returns that node.
struct node *get(struct node **node, int id)
{
    if (!*node)
        return NULL;
    struct node *found = NULL;

    if ((*node)->id == id)
    {
        found = *node;
        return found;
    }

    for (int i = 0; i < (*node)->children; i++)
    {
        found = get(&(*node)->child[i], id);
        if (found)
            return found;
    }
    return found;
}

//This function is the recursive part of the finding how many given type of node exist in given pointer of pointer node.
//There is no base case in that function, we check every child node with a for loop.
//If the point of node has child at that position, we chech it's type, If its type equal to given type variable.
//count is increase by one. This is the pass by reference way to count.
//Then we recall the function by giving the child node.
//With that we can go every node under the given pointer of pointer node. 
int counter(struct node **node, nodetype type, int *count)
{

    for (size_t i = 0; i < (*node)->children; i++)
    {
        if ((*node)->child[i])
        {
            
            if ((*node)->child[i]->type == type)
            {
                (*count)++;
            }
            counter(&(*node)->child[i], type, count);
        }
    }

    return *count;
}


//This is the non-recursive part for the counting how many given type of node has in subtree of the given pointer of pointer node.
//Firstly, we need to find which wanted node by using given id variable.
//If there is no node in with that id in the tree we return 0.
//Else, if finded node type is equal to searched type equals we start counting by one.(Because, I write counter function only for counting subtree.)
//Thus, I starts counting by one.
//Else it starts to counting by zero.
//Then it calls and returns the counter function and that will give the number of nodes which is the same type with given type variable  
int counter2(struct node **node, nodetype type, int id)
{

    struct node *subtree = get(node, id);

    if (!subtree)
        return 0;
    int c;
    if(subtree->type ==type)
     c = 1;
    else c =0;
    return counter(&subtree, type, &c);
}

//recursive method to delete node within the tree by given id.
//In the base case if the pointed node is null return NULL.
//if we found the node by id. We preserve itself and subtree by using another node pointer variable.
//But, the pointed node is set to NULL, Thus, the node and its subtree is deleted from the tree, 
//but we keep the node and its subtree for using somewhere else.
//else, we check every child by using a for loop. Inside of it, we call the function itself and it assigned to a temporary node variable.
//If the temporary node is not NULL we returns that node, and the function job is finished.  
struct node *delete_node(struct node **node, int id)
{
    if (!*node)
        return NULL;
    struct node *tmp = NULL;

    if ((*node)->id == id)
    {
        tmp = (*node);
        // tmp->parent = NULL;
        (*node) = NULL;
        return tmp;
    }
    for (int i = 0; i < (*node)->children; i++)
    {

        tmp = delete_node(&(*node)->child[i], id);
        if (tmp)
        {
            return tmp;
        }
    }
    return tmp;
}


//The method changes the subtree.
//Firstly, we try to find destination node by using get method.
//then if the get method returns NULL, we dont go any further and the change_sub_tree method is also return NULL and
// finishes make other operation to avoid crashes.
//Then, we delete the subtree from the tree.
//If the deletion will fail, the function stop.
//If everything fine at this point, we add the returned subtree from delete_node method.

//Adding a child:
//we hold the destPtr childs at a pointer of pointer node. 
//(Because, we need to malloc child to add another children and the malloc operation will delete the occurence of nodes.)
//we keep the how many children does haw that node and we increase after.
//the we malloc the child pointer of pointer to increase size to fit another tree in seperated memory.(We malloc the child with increased children number.)
//Then, we put back the childs which holded before by a pointer of pointer by using a for loop but we used kept number of children number before increase.
//After that by using another for loop but we used increased children number, we try to find any empty place to add the new child node.
//(It checks left to right if there is no empty space in between childs we add the new child at left-most free place.)
//Then it returns new root of that subtree.
struct node *change_sub_tree(struct node **node, int loc, int dest)
{
    struct node *locPtr = NULL;
    struct node *destPtr = NULL;

    destPtr = get(node, dest);
    if (!destPtr)
        return NULL;

    locPtr = delete_node(node, loc);
    if (!locPtr)
        return NULL;

    struct node **toKeep = (destPtr)->child;
    int size = (destPtr)->children++;
    (destPtr)->child = malloc(sizeof(struct node *) * ((destPtr)->children));

    for (int i = 0; i < size; i++)
    {
        (destPtr)->child[i] = (toKeep[i]);
    }

    for (int i = 0; i < destPtr->children; i++)
    {
        if (!destPtr->child[i])
        {
            destPtr->child[i] = locPtr;
            break;
        }
    }

    return destPtr;
}


//The recursive method is used to print the the by given pointer of pointer root node.
//in the base case if the pointed place of the root is null we return.
//if the pointed place of the root is not null,we print that node information into outFile ,
//then we goes every child node by calling the method itself inside of a for loop.

void display_tree(struct node **root,FILE *outFile)
{
    if (*root == NULL)
    {
        return;
    }

    if ((*root))
    {
        fprintf(outFile, "%s[%d]\n", convertType((*root)->type), (*root)->id);
        printf("%s[%d]\n", convertType((*root)->type), (*root)->id);

        for (size_t i = 0; i < (*root)->children; i++)
        {
            display_tree(&(*root)->child[i],outFile);
        }
    }

}

//This method is used in the beginning on the main method to add a node to root.
//It simply malloc a space for a node then malloc a size for further child and assigned node type to given type and return new node. 
struct node *add_root(struct node **node, nodetype type, int *id)
{

    struct node *newNode = malloc(sizeof(struct node));
    newNode->children = 0;
    // printf("%d -s-s %d\n",newNode->children,*id);
    (newNode->child) = malloc(sizeof(struct node *));
    newNode->id = (*id)++;
    newNode->type = type;
    // newNode->parent = NULL;
    return newNode;
}

//The insertion method. The method takes a pointer of pointer node to insert new nodes under itself.
//The base case is if the place of the root is null, return NULL.
//Else, if we found the root node which the new node will be added as a child at that node by rootid variable,
//Adding child:
// //we hold the root childs at a pointer of pointer node.
//(Because, we need to malloc child to add another children and the malloc operation will delete the occurence of nodes.)
//we keep the how many children does haw that node and we increase after.
//the we malloc the child pointer of pointer to increase size to fit another node in seperated memory.(We malloc the child with increased children number.)
//Then, we put back the childs which holded before by a pointer of pointer by using a for loop but we used kept number of children number before increase.
//After that by using another for loop but we used increased children number, we try to find any empty place to add the new child node.
//We create a new node by using method variables. 
//(It checks left to right if there is no empty space in between childs we add the new child at left-most free place.)
//Then it returns new root of that subtree.
struct node *insertNode(struct node **root, nodetype type, int *id, int rootid)
{

    if ((*root) == NULL)
    {
        return NULL;
    }
    if ((*root)->id == rootid)
    {
        struct node **toKeep = (*root)->child;
        int size = (*root)->children++;
        (*root)->child = malloc(sizeof(struct node *) * ((*root)->children));

        for (int i = 0; i < size; i++)
        {
            // printf("%d hii ->%d ->-> %d\n", *id, (*root)->children, (*root)->id);
            (*root)->child[i] = (toKeep[i]);
        }

        int i;
        for (i = 0; i < (*root)->children; i++)
        {
            if ((*root)->child[i] == NULL)
            {
                struct node *newNode = malloc(sizeof(struct node));
                newNode->children = 0;
                // printf("%d -s-s %d\n",newNode->children,*id);
                (newNode->child) = malloc(sizeof(struct node *));
                newNode->id = (*id)++;
                newNode->type = type;
                // newNode->parent = (*root);
                (*root)->child[i] = newNode;
                // printf("%d sd%s\n", (*root)->child[i]->id, convertType((*root)->type));
                break;
            }
        }
        return (*root)->child[i];
    }
    else
    {
        for (size_t i = 0; i < (*root)->children; i++)
        {
            if ((*root)->child[i])
            {
                insertNode(&(*root)->child[i], type, id, rootid);
            }
        }
        return *root;
    }
}

//The method is used when inputs reading from the text file.
//To decide which type of node will be used.
enum nodetype decideType(char *string)
{
    if (strcmp(string, "SWITCH") == 0)
    {
        return Switch;
    }
    else if (strcmp(string, "ROUTER") == 0)
    {
        return Router;
    }
    else if (strcmp(string, "INTERNET") == 0)
    {
        return Internet;
    }
    else if (strcmp(string, "SERVER") == 0)
    {
        return Server;
    }
    else if (strcmp(string, "FIREWALL") == 0)
    {
        return Firewall;
    }
    else if (strcmp(string, "WS") == 0)
    {
        return Ws;
    }
    else if (strcmp(string, "TERMINAL") == 0)
    {
        return Terminal;
    }
    else if (strcmp(string, "IMAC") == 0)
    {
        return Imac;
    }
    else if (strcmp(string, "LAPTOP") == 0)
    {
        return Laptop;
    }
    else if (strcmp(string, "PRINTER") == 0)
    {
        return Printer;
    }
    return -1;
}
