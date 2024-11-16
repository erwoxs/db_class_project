#include <stdio.h>
#include <string.h>
#include "repl.h"
#include "btree.h"
#include <stdlib.h>
#include "test.h"

extern Table* table;  //Accède à la variable globale table pour faire des tests sur la table


void test_insert_row() {
    printf("Test d'insertion de ligne:\n");

    //Instruction à exécuter
    Statement statement; 
    statement.type = STATEMENT_INSERT;
    
    //Insertion de la première ligne
    statement.row.id = 1;
    strcpy(statement.row.name, "test3"); //Copie le nom dans la ligne
    execute_statement(&statement); //Appel de la fonction pour éxecuter l'insertion de la ligne dans la table
    

    //Insertion de la deuxième ligne
    statement.row.id = 2;
    strcpy(statement.row.name, "test4");
    execute_statement(&statement);

    printf("Test d'insertion réussi\n\n");

}

void test_select_all() {
    printf("Test de sélection de toutes les lignes:\n");

    Statement statement;
    statement.type = STATEMENT_SELECT;
    statement.row.id=-1; //Id défini à -1 pour selectionner toutes les lignes.
    execute_statement(&statement);  

    printf("Test de sélection de toutes les lignes réussi\n\n");
}

void test_select_by_id(int id) {
    printf("Test de sélection de ligne par ID:\n");

    Statement statement;
    statement.type = STATEMENT_SELECT;
    statement.row.id = id;

    execute_statement(&statement);

    printf("Test de sélection par ID réussi\n\n");
}

//Fonction de test pour la mise à jour
void test_update_row(int id, const char* new_name) 
{
    printf("Test de mise à jour de ligne ID=%d:\n", id);

    //Création de la déclaration de mise à jour
    Statement statement;
    statement.type = STATEMENT_UPDATE;
    statement.row.id = id;
    strcpy(statement.row.name, new_name);

    //Appel de la fonction pour mettre à jour la ligne
    execute_statement(&statement);

    //Vérification de la mise à jour
    Row* updated_row = table_select(table, id);
    if (updated_row) {
        printf("Ligne mise à jour : id=%d, name=%s\n", updated_row->id, updated_row->name);
        printf("Échec de la mise à jour : ligne non trouvée.\n");
    }

    printf("Test de mise à jour réussi\n\n");
}

int main() {
    //Initialisation de la table pour les tests
    table = new_table();
    if (table == NULL) {
        printf("Erreur : échec de l'allocation de la table.\n");
        return 1;
    }

    //Exécute les tests
    test_insert_row();
    test_select_all();  //Sélectionne toutes les lignes insérées
    test_select_by_id(1);  //Sélectionne la ligne avec ID = 1
    test_select_by_id(2);  //Sélectionne la ligne avec ID = 2

    //Test de mise à jour
    test_update_row(1, "test1");  //Mise à jour de la ligne avec ID 1
    test_update_row(2, "test2");  //Mise à jour de la ligne avec ID 2

    test_select_all(); //Sélectionne à nouveau toutes les lignes insérées et modifiées

    free(table);
    return 0;
}
