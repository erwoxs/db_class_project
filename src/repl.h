#ifndef REPL_H
#define REPL_H

#include <stdbool.h> //pour bool
#include <stddef.h> //pour size_t



// Définir une structure row (ligne) qui contient les données à insérer
typedef struct {
    int id;                // id
    char name[255];        // name
    char email[255];       // email
} Row;



//Énumérations pour les commandes
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult; //commandes en mode repl 


//Déclaration de l'énumération pour les résultats de commande préparée
typedef enum { 
    PREPARE_SUCCESS, 
    PREPARE_UNRECOGNIZED_STATEMENT 
} PrepareResult;    //pour vérifier le succès ou l'échec de la préparation de la commande.


//Déclaration des types de statements
typedef enum { 
    STATEMENT_INSERT, 
    STATEMENT_SELECT 
} StatementType;   //pour définir le type de commande

//Déclaration de la structure Statement pour représenter une instruction
typedef struct{
    StatementType type;
    Row row; //Inclure la ligne dans l'instruction
}Statement;


//Déclaration de la structure InputBuffer pour gérer l'entrée utilisateur
typedef struct {
    char* buffer;
    size_t buffer_length;
    size_t input_length;
} InputBuffer;  //représente le buffer qui contient l'entrée utilisateur.


//Prototypes de fonctions
InputBuffer* new_input_buffer();
void print_prompt();
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);
MetaCommandResult do_meta_command(InputBuffer* input_buffer);
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
void execute_statement(Statement* statement);
void repl(void);

#endif // REPL_H