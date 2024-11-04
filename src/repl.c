#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include "repl.h"
#include "btree.h"   //Fichier header pour l'arbre binaire


//Déclare une variable globale pour la table
Table *table; //Stocke les données

// Fonction pour créer un nouveau buffer d'entrée
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

// Fonction pour afficher l'invite
void print_prompt() { 
    printf("db > "); 
}

// Fonction pour lire l'entrée utilisateur
void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    // Ignorer le saut de ligne
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0; // Remplacer '\n' par '\0'
}

// Fonction pour fermer le buffer d'entrée
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

// Fonction pour traiter les commandes de métadonnées
MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    return META_COMMAND_UNRECOGNIZED_COMMAND;
}

// Fonction pour préparer une instruction
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        //Déclarer les champs pour id, name, email
        int id;
        char name[255];
        char email[255];

        //Exemple de format d'entrée : "insert 1 Bob bob@exemple.com"
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", &id, name, email);
        
        if (args_assigned < 3) {
            printf("Syntaxe de la commande INSERT invalide. Utilisation : insert <id> <name> <email>\n");
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }

        // Assigner les valeurs à l'instruction
        statement->row.id = id;
        strncpy(statement->row.name, name, sizeof(statement->row.name) - 1);
        statement->row.name[sizeof(statement->row.name) - 1] = '\0'; // Assurer que c'est bien terminé
        strncpy(statement->row.email, email, sizeof(statement->row.email) - 1);
        statement->row.email[sizeof(statement->row.email) - 1] = '\0'; // Assurer que c'est bien terminé

        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

// Fonction pour exécuter une instruction
void execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            // Créer une nouvelle ligne à partir des données d'insertion
            Row row = statement->row;
            // Insérer la ligne dans l'arbre
            table_insert(table, row);
            printf("Row inséré avec succès : id=%d, name=%s, email=%s\n", row.id, row.name, row.email);
            break;
        case (STATEMENT_SELECT):
            // Afficher toutes les lignes
            for (int id = 0; id <= 100; id++) { // Vous pouvez ajuster la plage comme bon vous semble
                Row* row = table_select(table, id);
                if (row) {
                    printf("Row trouvé : id=%d, name=%s, email=%s\n", row->id, row->name, row->email);
                }
            }
            break;
    }
}

// Fonction principale REPL
void repl(void) {
    InputBuffer* input_buffer = new_input_buffer();
    table = new_table(); //Initialise la table pour contenir les données
    while (true) {
        print_prompt();
        read_input(input_buffer);
        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                printf("recognized statement\n");
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }
        execute_statement(&statement);
        printf("Executed.\n");
    }

    free_table(table); //Libére la table à la fin
    close_input_buffer(input_buffer);
}
