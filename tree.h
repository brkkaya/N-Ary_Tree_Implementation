// !!!IMPORTANT!!!
// This assignment was written in Ubuntu 19.04.
// gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0 -> is the version of the gcc.
// Besides, the code files works very well in ubuntu and Moodle VPL,
// but the files didn't work in Windows. I request to consider the conditions, in the ungiven test cases.

//This file is the decleration of N-Ary Tree.
//I create enum for node types, struct for tree nodes and the declaration of the functions.

//The enum of nodetype. With that enum we easily use the node type in nodes in tree.
typedef enum nodetype
{
    Switch,
    Router,
    Internet,
    Server,
    Firewall,
    Ws,
    Terminal,
    Imac,
    Laptop,
    Printer
} nodetype;


//Struct of node. it include a nodetype type as every node must have a type.
//I use pointer of pointer child to keep as many as node pointer in it when it we malloc it.
//int children is used to keep how many children does have.
//int id is id for every node. we make operation by using id variable.

struct node
{
    nodetype type;
    struct node **child;
    int children;
    int id;
};

//declaration of every function which has been needed.
struct node *insertNode(struct node **root, nodetype x, int *id, int rootid);
enum nodetype decideType(char *string);
void display_tree(struct node **root,FILE *outFile);
char *convertType(enum nodetype type);
struct node *change_sub_tree(struct node **node, int loc, int dest);
struct node *get(struct node **node, int id);
struct node *delete_node(struct node **node, int id);
int counter(struct node **node, nodetype type, int *count);
int counter2(struct node **node, nodetype type, int id);
int maxDepth(struct node **node);
void levelorder(struct node **node,int level,FILE *outFileB);
struct node *add_root(struct node **node,nodetype type,int *id);