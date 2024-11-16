#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include "repl.h"
#include "btree.h"   //Fichier header pour l'arbre binaire
#include "dpersistence.h"

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

//Fonction pour convertir une commande saisie en majuscule en minuscule
void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++) //Parcours chaque caractère de la chaine str
    { 
        if (str[i] >= 'A' && str[i] <= 'Z') { //Vérifie si une letrtre est une majuscule compris entre A et Z
            str[i] = str[i] + ('a' - 'A'); //Si une lettre est une majuscule, elle sera converti en minuscule
        }
    }
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
    to_lowercase(input_buffer->buffer);//Convertir la commande en minuscules
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) //Vérifie si la commande est un update en comparant la chaine
    { 

        statement->type = STATEMENT_INSERT; //Indique que le type de commande est insert 
        int id;
        char name[255];

        
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s", &id, name); //Extrait les données entré par l'utilisateur pour les stocker
        
        if (args_assigned < 2) {
            printf("Syntaxe de la commande INSERT invalide. Utilisation : insert <id> <name>\n");
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }

        // Stocke les valeurs extraites dans statement->row
        statement->row.id = id;
        strncpy(statement->row.name, name, sizeof(statement->row.name) - 1);
        statement->row.name[sizeof(statement->row.name) - 1] = '\0'; // Assure que c'est bien terminé

        return PREPARE_SUCCESS;
    }
    if (strncmp(input_buffer->buffer, "select",6) == 0) //Vérifie si la commande est insert en comparant la chaine
    {
        statement->type = STATEMENT_SELECT; //Indique que le type de commande est insert
        //Vérifie si l'id est fourni après un select
        int id; //stocke l'identifiant qui sera extrait de la commande SELECT
            if (sscanf(input_buffer->buffer, "select %d", &id) == 1) {
            statement->row.id = id;  // Si un ID est spécifié
        } else {
            statement->row.id = -1;  // Sinon, définis à -1 pour indiquer "toutes les lignes"
        }
        return PREPARE_SUCCESS;

    }else if (strncmp(input_buffer->buffer,"update",6)==0) { //Vérifie si la commande est insert en comparant la chaine

        int id;
        char name[255];

        //Extrait les arguments de la commande update
        int args_assigned = sscanf(input_buffer->buffer, "update %d %s", &id, name); //Extrait les données entré par l'utilisateur pour les stocker
    
        if (args_assigned<2)
        {
            printf("Syntaxe de la commande UPDATE invalide. Utilisation : update <id> <name>\n");
            return PREPARE_UNRECOGNIZED_STATEMENT;
        }

        // Stocker les valeurs extraites dans statement->row pour pouvoir les afficher ou modifier
        statement->type = STATEMENT_UPDATE;
        statement->row.id = id;
        strncpy(statement->row.name, name, sizeof(statement->row.name) - 1);
        statement->row.name[sizeof(statement->row.name) - 1] = '\0';

        return PREPARE_SUCCESS;

    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

// Fonction pour exécuter une instruction
void execute_statement(Statement* statement) {
    switch (statement->type) { //Vérifie le type d'insutrction contenue dans statement-type
        case (STATEMENT_INSERT):
            // Créer une nouvelle ligne à partir des données d'insertion
            Row row = statement->row;
            // Insérer la ligne dans l'arbre
            table_insert(table, row);
            printf("Row inséré avec succès : id=%d, name=%s\n", row.id, row.name);
            save_table_to_file(table, "table_data.dat"); //Sauvegarde après insertion
            break;
        case (STATEMENT_SELECT):
            //Si un id est fourni, la ligne sera affiché avec cet id
            if(statement->row.id >=0)
            {
                Row* row = table_select(table,statement->row.id);
                if (row) {
                    printf("Row trouvé : id=%d, name=%s\n", row->id,row->name);
                }else
                {
                    printf("Aucune ligne trouvée avec l'ID %d.\n", statement->row.id);
                }
            }else
            {
                // Afficher toutes les lignes
                for (int id = 0; id <= 100; id++)  //Parcours tous les ID possible pour afficher toutes les lignes présentes dans la table
                {
                    Row* row = table_select(table, id);
                    if (row)
                    {
                        printf("Row trouvé : id=%d, name=%s\n", row->id, row->name);
                    }
                }
            }            
            break;
        case (STATEMENT_UPDATE):
            //Appel de la fonction table_update pour effectuer la mise à jour
            if (table_update(table,statement->row.id,statement->row.name)) {
                printf("Row mise à jour avec succès : id=%d, name=%s\n", statement->row.id, statement->row.name);
                save_table_to_file(table, "table_data.dat"); // Sauvegarder après mise à jour
            } else {
                printf("Aucune ligne trouvée avec l'ID %d pour mise à jour.\n", statement->row.id);
            }
            break;
            
    }
}

// Fonction principale REPL
void repl(void) {
    InputBuffer* input_buffer = new_input_buffer();
    
    table = load_table_from_file("table_data.dat");  // Charger la table depuis le fichier
    if (!table) {
        table = new_table();  // Si le fichier n'existe pas, créer une nouvelle table
    }
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
