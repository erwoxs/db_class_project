# Compilateur à utiliser
CC = gcc
# Nom de l'exécutable final
EXEC = db_project
#Nom de l'exécutable de test 
TEST_EXEC=db_test
# Les fichiers sources
SRC = src/main.c src/btree.c src/repl.c src/dpersistence.c

#Fichier test 
TEST_SRC = src/test.c  src/btree.c src/repl.c src/dpersistence.c
CFLAGS = -Wall  -g #-Wall active touts les avertissements -g : génère des informations de débogue plus simple


# Target par défaut : compile le programme
all: $(EXEC)

# Règle pour construire l'exécutable
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

# Règle pour construire l'exécutable de test
$(TEST_EXEC): $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_EXEC) $(TEST_SRC)

# Section pour exécuter le programme après la compilation
run: $(EXEC)
	@echo "Execution : "
	./$(EXEC)

# Règle pour exécuter Valgrind avec le programme principal
valgrind: $(EXEC)
	@echo "Execution du programme principal avec Valgrind :"
#Commande qui exécute le programme principal avec Valgrind, pour détecter des erreurs de gestion mémoire
#--leak-check=full -> Vérifie toutes les fuites de mémoire et de les afficher
#--track-origins=yes -> Option pour retrouver l'origine des erreurs liées à l'utilisation de mémoire non initialisée.
	valgrind --leak-check=full --track-origins=yes ./$(EXEC)   
	


# Section pour exécuter les tests
test: $(TEST_EXEC)
	@echo "Execution des tests :"
	./$(TEST_EXEC)



# Nettoyage des fichiers compilés
clean:
	rm -f $(EXEC) $(TEST_EXEC)
	rm -f *.o

# Indique que ces cibles ne correspondent pas à des fichiers
.PHONY: all run clean test
