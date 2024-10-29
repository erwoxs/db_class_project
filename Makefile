# Compilateur à utiliser
CC = gcc
# Nom de l'exécutable final
EXEC = db_project
# Les fichiers sources
SRC = src/main.c src/btree.c src/repl.c
# Flags de compilation (ajout du -o ici)
CFLAGS = -Wall  -g 


# Target par défaut : compile le programme
all: $(EXEC)

# Règle pour construire l'exécutable
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)


# Section pour exécuter le programme après la compilation
run: $(EXEC)
	@echo "Execution : "
	./$(EXEC)

# Nettoyage des fichiers compilés
clean:
	rm -f $(EXEC)
	rm -f *.o

# Indique que ces cibles ne correspondent pas à des fichiers
.PHONY: all run clean
