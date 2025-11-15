CC = gcc
CFLAGS = -Wall -std=c99
EXEC = sistema_ventas
OBJ_FILES = main.o usuarios.o productos.o

$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(EXEC)

main.o: main.c usuarios.h productos.h
	$(CC) $(CFLAGS) -c main.c

usuarios.o: usuarios.c usuarios.h
	$(CC) $(CFLAGS) -c usuarios.c

productos.o: productos.c productos.h
	$(CC) $(CFLAGS) -c productos.c

.PHONY: clean
clean:
	rm -f $(EXEC) $(OBJ_FILES)