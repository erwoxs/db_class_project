#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include "repl.h"
#include "btree.h"   //Fichier header pour l'arbre binaire


//Déclare une variable globale pour la table
Table *table;

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
            // TODO: Implémentez la commande ici
            break;
        case (STATEMENT_SELECT):
            // TODO: Implémentez la commande ici
            break;
    }
}

// Fonction principale REPL
void repl(void) {
    InputBuffer* input_buffer = new_input_buffer();
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
}
