#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "repl.h" // Inclure le fichier d'en-tête repl.h
#include "dpersistence.h"
#include "btree.h"

int main(int argc, char* argv[], char* envp[]){
  
  // Charge la table depuis le fichier "table_data.dat" si elle existe
  Table* table = load_table_from_file("table_data.dat");
  if (table == NULL) {
    // Si le fichier n'existe pas, crée une nouvelle table
    table = new_table();
  }else 
  {
    printf("Table chargée depuis le fichier.\n");
  }


  repl();
  //Sauvegarde la table avant de quitter
  save_table_to_file(table, "table_data.dat");
  printf("Table sauvegardée dans le fichier.\n");

  // Libère la mémoire de la table
  free(table);
  return 0;
  
}
