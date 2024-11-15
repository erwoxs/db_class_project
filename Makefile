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
