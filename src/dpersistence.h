#ifndef DPERSISTENCE_H
#define DPERSISTENCE_H
#include "btree.h"


//Déclaration des fonctions de persistance
void save_table_to_file(Table* table, const char* fileData);
Table* load_table_from_file(const char* fileData);

#endif  
