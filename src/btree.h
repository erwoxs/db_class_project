#ifndef BTREE_H
#define BTREE_H
#include <stdbool.h>
#include "repl.h"

//Définir une structure de noeud dans l'arbre binaire
typedef struct Node {
    Row data;              //Données du noeud en ligne
    struct Node* left;     //Pointeur vers le noeud enfant gauche
    struct Node* right;    //Pointeur vers le noeud enfant droit
} Node;

//Définir une structure de table de la base de données
typedef struct Table {
    int num_rows;
    Row* rows;
    Node* root;  //pointeur vers la racine de l'arbre binaire
    
} Table;

//Prototypes de fonctions pour manipuler l'arbre binaire
Table* new_table();                  //Crée une nouvelle instance de la table
void table_insert(Table* table, Row data); //Insère une ligne dans l'arbre binaire
Row* table_select(Table* table, int id);   //Recherche une ligne par id dans l'arbre binaire
Row* table_update(Table* table, int id, const char* name); //Modifie la ligne en utilisant l'id pour modifier le nom
void free_table(Table* table);             //Libère la mémoire allouée pour chaque noeud

#endif

