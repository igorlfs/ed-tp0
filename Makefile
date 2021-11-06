#---------------------------------------------------------------------
# Opções	: make all - compila programa principal
#			: make clean - remove objetos e executável
#			: make test - compila e executa arquivo de teste
#---------------------------------------------------------------------

CC = g++
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
OBJS = $(OBJ)/matop.o $(OBJ)/mat.o $(OBJ)/memlog.o
HDRS = $(INC)/mat.h $(INC)/memlog.h $(INC)/msgassert.h
CFLAGS = -Wall -c -I$(INC) -g

EXE = $(BIN)/matop
EXE_TEST = $(BIN)/test

all: $(EXE)

$(BIN)/matop: $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LIBS)

$(OBJ)/matop.o: $(HDRS) $(SRC)/matop.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/matop.o $(SRC)/matop.cpp

$(OBJ)/mat.o: $(HDRS) $(SRC)/mat.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/mat.o $(SRC)/mat.cpp

$(OBJ)/memlog.o: $(HDRS) $(SRC)/memlog.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/memlog.o $(SRC)/memlog.cpp

test: $(EXE_TEST)

$(BIN)/test: $(OBJ)/tst.o $(OBJ)/mat.o
	$(CC) -o $(EXE_TEST) $(OBJ)/tst.o $(OBJ)/mat.o $(OBJ)/memlog.o $(LIBS) -lgtest; ./$(EXE_TEST)

$(OBJ)/tst.o: $(INC)/mat.h $(SRC)/tst.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/tst.o $(SRC)/tst.cpp

	
clean:
	rm -r $(BIN)/* $(OBJ)/*
