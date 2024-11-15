#include <stdio.h>
#include <stdlib.h>
#include "dpersistence.h"
#include "btree.h"  // Inclure btree.h pour utiliser la structure Table et Row

// Fonction pour sauvegarder récursivement chaque nœud de l'arbre dans le fichier
void save_node(Node* node, FILE* file) {
    if (node == NULL) {
        int null_marker = -1;
        fwrite(&null_marker, sizeof(int), 1, file);  // Écrit un marqueur pour NULL
        return;
    }

    // Sauvegarde les données du nœud
    fwrite(&node->data, sizeof(Row), 1, file);

    // Sauvegarde récursive des sous-arbres gauche et droit
    save_node(node->left, file);
    save_node(node->right, file);
}

// Fonction pour charger récursivement chaque nœud de l'arbre depuis le fichier
Node* load_node(FILE* file) {
    int check;
    
    // Lire pour vérifier si c'est un marqueur de fin de nœud
    if (fread(&check, sizeof(int), 1, file) != 1) {
        printf("Erreur de lecture du fichier.\n");
        return NULL;
    }
    
    if (check == -1) {  // Condition pour détecter un nœud NULL
        return NULL;
    }

    // Remet la position pour relire `check` comme un Row cette fois
    fseek(file, -sizeof(int), SEEK_CUR);

    // Alloue un nouveau nœud
    Node* node = malloc(sizeof(Node));
    if (!node) {
        printf("Erreur d'allocation mémoire pour le nœud.\n");
        return NULL;
    }

    // Charge les données du nœud
    fread(&node->data, sizeof(Row), 1, file);

    // Charge récursivement les sous-arbres gauche et droit
    node->left = load_node(file);
    node->right = load_node(file);

    return node;
}

// Fonction pour sauvegarder la table complète dans un fichier
void save_table_to_file(Table* table, const char* fileData) {
    FILE* file = fopen(fileData, "wb");
    if (!file) {
        printf("Aucune données dans le fichier de sauvegarde, insérer d'abord des valeurs.\n");
        return;
    }

    // Sauvegarde du nombre de lignes
    fwrite(&table->num_rows, sizeof(int), 1, file);

    // Sauvegarde récursive de l'arbre binaire de recherche (la racine)
    save_node(table->root, file);

    fclose(file);
}

// Fonction pour charger la table complète depuis un fichier
Table* load_table_from_file(const char* fileData) {
    FILE* file = fopen(fileData, "rb");
    if (!file) {
        printf("Aucune données dans le fichier de sauvegarde, insérer d'abord des valeurs.\n");
        return NULL;
    }

    Table* table = malloc(sizeof(Table));
    if (!table) {
        printf("Erreur d'allocation mémoire.\n");
        fclose(file);
        return NULL;
    }

    // Charge le nombre de lignes depuis le fichier
    fread(&table->num_rows, sizeof(int), 1, file);

    // Charge l'arbre binaire de recherche depuis le fichier (à partir de la racine)
    table->root = load_node(file);

    fclose(file);
    return table;
}
