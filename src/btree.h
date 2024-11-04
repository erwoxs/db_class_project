// btree.h
#ifndef BTREE_H
#define BTREE_H
#include <stdbool.h>
#include "repl.h"

// Définir une structure de noeud pour chaque noeud de l'arbre binaire
typedef struct Node {
    Row data;              // Row data
    struct Node* left;     // Pointeur vers le noeud gauche
    struct Node* right;    // Pointeur vers le noeud droit
} Node;

// Définir une structure de table contenant la racine de l'arbre binaire
typedef struct Table {
    Node* root;            // Racine de l'arbre binaire pour accéder à tous les autres nœuds de l'arbre
} Table;

// Prototypes de fonctions pour manipuler l'arbre binaire
Table* new_table();                  // Crée une nouvelle instance de la table
void table_insert(Table* table, Row data); // Insère une ligne dans l'arbre binaire
Row* table_select(Table* table, int id);   // Recherche une ligne par id
void free_table(Table* table);             // Libère la mémoire allouée pour chaque noeud

#endif

