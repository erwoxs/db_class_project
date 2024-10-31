// btree.h
#ifndef BTREE_H
#define BTREE_H
#include <stdbool.h>

//Define a Row structure that contains the data
typedef struct {
    int id;                //id
    char name[255];        // name
    char email[255];       //email
} Row;


// Define a Node structure for each node in the binary tree
typedef struct Node {
    Row data;              //Row data
    struct Node* left;     //Pointer to left node
    struct Node* right;    //Pointer to right node
} Node;

//Define a Table structure that contains the root of binary tree
typedef struct Table {
    Node* root;            //Root of binary tree to access all other nodes in the tree
} Table;

//Prototypes of functions to manipulate the binary tree
Table* new_table();  //Create a new instance of Table
void table_insert(Table* table, Row data); //Takes two parameters to insert a row and add that row into the binary tree
Row* table_select(Table* table, int id);
void free_table(Table* table);

#endif
