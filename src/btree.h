// btree.h
#ifndef BTREE_H
#define BTREE_H
#include <stdbool.h>

//Définir une structure row(ligne) qui contient les données
typedef struct {
    int id;                //id
    char name[255];        // name
    char email[255];       //email
} Row;


// Définir une structure de noeud pour chaque noeud de l'abre binaire
typedef struct Node {
    Row data;              //Row data
    struct Node* left;     //Pointeur vers le noeud gauche
    struct Node* right;    //Pointeur vers le noeud droit
} Node;

//Définir une structure de table contenant la racine de l'arbre binaire
typedef struct Table {
    Node* root;            //Racine de l'arbre binaire pour accéder à tous les autres nœuds de l'arbre
} Table;

//Prototypes de fonctions pour manipuler l'arbre binaire
Table* new_table();  //Crée une nouvelle instance de la table
void table_insert(Table* table, Row data); //Prend deux paramètres pour insérer une ligne et ajouter cette ligne dans l'arborescence binaire
Row* table_select(Table* table, int id); //Prend deux paramètres pour parcourir les données
void free_table(Table* table); 

#endif
