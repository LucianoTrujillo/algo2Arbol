CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes
OUTPUT=abb
DEPENDENCIES=*.c
ENTRY=abb_minipruebas.c

all: $(OUTPUT) mem_test

$(OUTPUT): $(DEPENDENCIES) $(ENTRY)
	gcc *.c $(CFLAGS) -o $(OUTPUT)

mem_test: $(OUTPUT)
	valgrind $(VFLAGS) ./$(OUTPUT)

debug: $(OUTPUT)
	gdb -tui $(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)