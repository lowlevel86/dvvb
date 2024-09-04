CC = gcc
OBJ = main.o
LIBS = -lm -lpthread
BIN = prog
CFLAGS = -Wall
RM = rm -f

#gdb flags: -g -O0 -v -da -Q
#gdb prog
#run arguments
#where
#list

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

