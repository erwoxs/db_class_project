#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

//Crée une nouvelle table avec un arbre vide
Table* new_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    table->root = NULL; // L'arbre commence sans aucun nœud
    return table;
}

//Fonction pour créer un nouveau noeud.
Node* create_node(Row data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Insère un nouveau Row dans l'arbre
void table_insert(Table* table, Row data) {
    if (table->root == NULL) {
        // Si l'arbre est vide, le Row devient la racine
        table->root = create_node(data);
        return;
    }

    // Insère en utilisant une référence vers le pointeur (Node**)
    Node* current = table->root; //Commence à la racine de l'arbre 
    Node* parent = NULL;  //Noeud parent pour l'insertion

    while (current != NULL) {
        parent = current;
        if (data.id < current->data.id) {
            current = current->left;
        } else if (data.id > current->data.id) {
            current = current->right;
        } else {
            // Si l'ID existe déjà, on ne fait rien
            printf("ID %d already exists in the table.\n", data.id);
            return;
        }
    }

    if (data.id < parent->data.id) {
        parent->left = create_node(data);
    } else {
        parent->right = create_node(data);
    }
}

// Recherche un Row par ID
Row* table_select(Table* table, int id) {
    Node* current = table->root;
    
    while (current != NULL) {
        if (id == current->data.id) {
            return &current->data;  //Si l'id est trouvé on retourne un pointeur vers les données
        } else if (id < current->data.id) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return NULL; // Si l'ID n'est pas trouvé dans l'arbre
}

// Fonction pour libérer tous les nœuds d'un arbre
void free_nodes(Node* node) {
    if (node == NULL) return;
    free_nodes(node->left);
    free_nodes(node->right);
    free(node);
}

// Libère la table en libérant tous les nœuds de l'arbre
void free_table(Table* table) {
    if (table->root != NULL) {
        free_nodes(table->root);
    }
    free(table);
}