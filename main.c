// !!!IMPORTANT!!!
// This assignment was written in Ubuntu 19.04.
// gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0 -> is the version of the gcc.
// Besides, the code files works very well in ubuntu and Moodle VPL,
// but the files didn't work in Windows 10. I request to consider the conditions, in the ungiven test cases.

//In this file I implement the main method as required. Which can read inputs from text file and writes.
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "tree.c"


//This method is written for getting the number inside brackets.
//it gets a char string then it uses a for loop. It give us the begin and end index to retrive the number from the string.
//then its copy the part of the string to another char pointer.
//then it returns the integer format of the string. 
int getID(char *string)
{
    int begin = 0, end = 0;
    for (size_t i = 0; i < strlen(string); i++)
    {
        if (string[i] == '[')
        {
            begin = i + 1;
        }
        if (string[i] == ']')
        {
            end = i - 1;
        }
    }
   
    char *ptr = malloc(sizeof(end - begin) + 2);
    for (size_t i = 0; i < end - begin + 2; i++)
    {
        ptr[i] = string[begin+i];
    }
    ptr[strlen(ptr) - 1] = '\0';
    return atoi(ptr);
}


//Main method of the assignment. If the command-line argument is not equal 5 is stop running.
//if the files couldn't opened it stops working again.
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Not enough input to work!!\n");
        return 0;
    }
    FILE *treecommands = fopen(argv[1], "r");
    FILE *treecommandsB = fopen(argv[3], "r");
    FILE *out = fopen(argv[2], "w");
    FILE *outB = fopen(argv[4], "w");
    if (!treecommands || !treecommandsB || !out || !outB)
    {
        printf("The files couldn't be opened\n");
        return 0;
    }

    struct node *root = NULL;

    //these variables declared to used when we get the input from input files.
    
    int id = 1, rootid = 0;
    char buffer[255];
    char *type;
    char *pch;
    //As the pdf document says the first commands will be always to insertion to root. Thus, we skip the first line.
    
    fgets(buffer, 255, treecommands);
    //then we create root node of tree. by using add_root method. 
    root = add_root(&root, Switch, &id);


    //We uses a loop to gets every input from the input file.
    while (fgets(buffer, 255, treecommands))
    {

        //we strtok by word by word by using " \t" is skips the white spaces and tabs.
        pch = strtok(buffer, " \t");
        //if the first input is equal insert.
        if (strcmp(pch, "INSERT") == 0)
        {
            //we take the type
            pch = strtok(NULL, " \t");
            type = pch;
            //then try to find rootid if there is a space between rootid and root type
            pch = strtok(NULL, " \t");
            rootid = getID(pch);

            if (!rootid)
            {
                //we strtok again and eventually get the root id.
                pch = strtok(NULL, " \t");
                rootid = getID(pch);
            }
                //then we make insertion by using input file.
            insertNode(&root, decideType(type), &id, rootid);
        }
        //else the first input is equal print.
        else if (strcmp(pch, "PRINT") == 0)
        {
            //we try to get root id then ,we get that root by using get method then, use the node in display_tree method.
            pch = strtok(NULL, " \t");
            rootid = getID(pch);
            if (!rootid)
            {
                pch = strtok(NULL, " \t");
                rootid = getID(pch);
            }
            struct node *hold = get(&root, rootid);
            display_tree(&hold,out);
        }
        //else the first input is equal delete.
        else if (strcmp(pch, "DELETE") == 0)
        {
            //we try to get root id, then by giving root node and rootid the deletion operation is handled by delete_node method.
            pch = strtok(NULL, " \t");
            rootid = getID(pch);
            if (!rootid)
            {
                pch = strtok(NULL, " \t");
                rootid = getID(pch);
            }
            delete_node(&root, rootid);
        }
    }
    printf("\n\n");
    //Bonus part input file is handled by this while method.
    while (fgets(buffer, 255, treecommandsB))
    {
        pch = strtok(buffer, " \t");
        //if the first input is equal to move.
        if (strcmp(pch, "MOVE") == 0)
        {
            //we try to get node ids by using same way in the first loop.
            //when we get the ids we use root node subid and the new rootid and the swap operation is handled by change_sub_tree method.
            pch = strtok(NULL, " \t");
            int subid =getID(pch);
            if(!subid){
                pch = strtok(NULL, " \t");
                subid =getID(pch);
            }
            pch = strtok(NULL, " \t");
            rootid =getID(pch);
            if (!rootid)
            {
                pch = strtok(NULL, " \t");
                rootid = getID(pch);
            }
            change_sub_tree(&root,subid,rootid);
        }
        //if the first input is equal to Print level
        else if (strcmp(pch, "PRINT_LEVEL") == 0)
        {   
            //As same as we try to get node id
            pch = strtok(NULL, " \t");
            rootid = getID(pch);
            if (!rootid)
            {
                pch = strtok(NULL, " \t");
                rootid = getID(pch);
            }
            //we hold the node with given id.
            struct node *hold = get(&root, rootid);
            //then we found that node depth by using maxDepth method.
            //after that a for loop prints the nodes by their level until to maxDepth.
            int c = maxDepth(&hold);
            for (int i = 1; i <= c; i++)
                levelorder(&hold, i,outB);
        }
        //else the first input is equal to count 
        else if (strcmp(pch, "COUNT") == 0)
        {
            //we take the node type which will be search.
            pch = strtok(NULL, " \t");
            type =pch;
            //we try to take node id.
            pch = strtok(NULL, " \t");
            rootid = getID(pch);
            if (!rootid)
            {
                pch = strtok(NULL, " \t");
                rootid = getID(pch);
            }
            //we use the type and rootid from the text file and we call counter2 method to count how many node has in that subtree.
            //We use it inside the fprintf method to write into output file.
            fprintf(outB, "%d\n", counter2(&root, decideType(type), rootid));
            
        }
    }


    //level order traversal
    // int c = maxDepth(&s);
    // for (int i = 1; i <= c; i++)
    //     levelorder(&s, i);
}