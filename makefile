CC=g++
CFLAGS=-Wall -pedantic -std=c++14
#LDFLAGS=
EXEC=test

all: main

main: parseXml.o prot.o main.o
	@echo -e "\n[=== Executable ===]"
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

%.o: %.cpp
	@echo -e "\n[=== $< -> $@ ===]"
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean javel

clean:
	rm -r *.o

javel: clean
	rm -r $(EXEC)
