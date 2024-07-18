CC = gcc
OBJ = main.o line_patt_generator.o print_line.o return_line.o test_line.o build_line.o modeling_phase.o
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

print_line.o: print_line.c
	$(CC) -c print_line.c -o print_line.o $(CFLAGS)

return_line.o: return_line.c
	$(CC) -c return_line.c -o return_line.o $(CFLAGS)

test_line.o: test_line.c
	$(CC) -c test_line.c -o test_line.o $(CFLAGS)

build_line.o: build_line.c
	$(CC) -c build_line.c -o build_line.o $(CFLAGS)

modeling_phase.o: modeling_phase.c
	$(CC) -c modeling_phase.c -o modeling_phase.o $(CFLAGS)

