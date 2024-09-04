CC = gcc
RES = 
OBJ = main.o line_patt_generator.o $(RES)
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

line_patt_generator.o: line_patt_generator.c
	$(CC) -c line_patt_generator.c -o line_patt_generator.o $(CFLAGS)

