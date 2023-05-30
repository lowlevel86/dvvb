CC = gcc
OBJ = main.o print_line.o build_line.o
LIBS = -lm -lpthread
BIN = prog
CFLAGS = -Wall
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

print_line.o: print_line.c
	$(CC) -c print_line.c -o print_line.o $(CFLAGS)

build_line.o: build_line.c
	$(CC) -c build_line.c -o build_line.o $(CFLAGS)

