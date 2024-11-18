#ifndef REPL_H
#define REPL_H
#include <stdbool.h> 
#include <stddef.h> 



//Définir une structure row qui contient les données à insérer ou afficher
typedef struct {
    int id;                //id
    char name[255];        //name
} Row;



//Enumération pour les commandes comme .exit saisit par l'utilisateur
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;


//Enumération pour les résultats de commande préparée
typedef enum { 
    PREPARE_SUCCESS, 
    PREPARE_UNRECOGNIZED_STATEMENT 
} PrepareResult;   


//Enumération des types de statements
typedef enum { 
    STATEMENT_INSERT, 
    STATEMENT_SELECT,
    STATEMENT_UPDATE,
} StatementType;   

//Déclaration de la structure Statement pour représenter une instruction
typedef struct{
    StatementType type;
    Row row; 
}Statement;


//Déclaration de la structure InputBuffer pour gérer l'entrée utilisateur
typedef struct {
    char* buffer; //stocke l'entée utilisateur
    size_t buffer_length; //taille de l'entrée en mémoire
    size_t input_length; //longueur de l'entrée 
} InputBuffer;


//Prototypes de fonctions
InputBuffer* new_input_buffer(); //Crée et initalise un tampon d'entrée
void print_prompt(); //Affiche l'invite de commande 
void read_input(InputBuffer* input_buffer); //Lit l'entrée utilisateur et la stocke dans input_buffer
void close_input_buffer(InputBuffer* input_buffer); //Libère la mémoire allouée à input_buffer avce free()
MetaCommandResult do_meta_command(InputBuffer* input_buffer); //Lit input_buffer pour identifier si l'utilisateur a entré une commande méta.
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement); //Analyse l'entrée utilisateur pour voir si elle correspond à une instruction 
void execute_statement(Statement* statement); //Execute l'instruction en fonction de son type.
void repl(void); //Démarre la boucle repl

#endif // REPL_H
