CC = g++
C = -c
O = -o
VERSION = --std=c++11
FLAGS = -Wall -g
SOURCES = main.cpp keccak.cpp utilities.cpp
OBJECTS = $(SOURCES:.cpp=.o)
OUTPUT = shake128
REMOVE = shake128 *.o

$(OUTPUT): $(OBJECTS)
	$(CC) $(FLAGS) $(VERSION) $(OBJECTS) -o $(OUTPUT)
main.o: main.cpp keccak.cpp
	$(CC) $(C) $(FLAGS) $(VERSION) main.cpp
keccak.o: keccak.cpp keccak.h
	$(CC) $(C) $(FLAGS) $(VERSION) keccak.cpp
utilities.o: utilities.cpp
	$(CC) $(C) $(FLAGS) $(VERSION) utilities.cpp
.PHONY:
clean:
	@rm -rf $(REMOVE)
