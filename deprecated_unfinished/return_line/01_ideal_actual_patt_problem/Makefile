CC = gcc
RES = 
OBJ = main.o testPatt.o getLine.o printData.o $(RES)
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

testPatt.o: testPatt.c
	$(CC) -c testPatt.c -o testPatt.o $(CFLAGS)

getLine.o: getLine.c
	$(CC) -c getLine.c -o getLine.o $(CFLAGS)

printData.o: printData.c
	$(CC) -c printData.c -o printData.o $(CFLAGS)

