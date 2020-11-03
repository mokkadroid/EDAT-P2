CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ansi -g
LDLIBS = -lodbc

OBJ=./obj
SRC=./src
INC=./include

PROG_NAME=Main



all: $(OBJ)/menus.o $(OBJ)/querys.o $(OBJ)/odbc.o $(OBJ)/main.o
	@echo Compilando...
	gcc -o $(PROG_NAME) $(OBJ)/menus.o $(OBJ)/querys.o $(OBJ)/odbc.o $(OBJ)/main.o $(LDLIBS)
	@echo Creado ejecutable \"$(PROG_NAME)\"
	@echo
	@echo Ejecutando...
	@echo
	./$(PROG_NAME)
	@echo
	@echo Ejecucion finalizada
	@echo



$(OBJ)/main.o: $(OBJ)/menus.o $(OBJ)/odbc.o $(SRC)/main.c
	gcc $(FLAGS) -I$(INC) -c $(SRC)/main.c -o $(OBJ)/main.o $(LDLIBS)

$(OBJ)/menus.o: $(SRC)/menus.c $(INC)/menus.h $(OBJ)/querys.o
	gcc $(FLAGS) -I$(INC) -c $(SRC)/menus.c -o $(OBJ)/menus.o $(LDLIBS)

$(OBJ)/querys.o: $(SRC)/querys.c $(INC)/querys.h
	gcc $(FLAGS) -I$(INC) -c $(SRC)/querys.c -o $(OBJ)/querys.o $(LDLIBS)

$(OBJ)/odbc.o: $(SRC)/odbc.c $(INC)/odbc.h
	gcc $(FLAGS) -I$(INC) -c $(SRC)/odbc.c -o $(OBJ)/odbc.o $(LDLIBS)












clean :
	rm -f $(OBJ)/*.o ./$(PROG_NAME)
