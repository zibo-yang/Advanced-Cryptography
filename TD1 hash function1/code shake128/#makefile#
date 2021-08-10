CC = g++
C = -c
O = -o
VERSION = --std=c++11
FLAGS = -Wall -g
SOURCES = main.cpp keccak.cpp 
OBJECTS = $(SOURCES:.cpp=.o)
OUTPUT = shake128
REMOVE = shake128 *.o

$(OUTPUT): $(OBJECTS)
	$(CC) $(FLAGS) $(VERSION) $(OBJECTS) -o $(OUTPUT)
main.o: main.cpp keccak.h
	$(CC) $(C) $(FLAGS) $(VERSION) main.cpp
keccak.o: keccak.cpp keccak.h
	$(CC) $(C) $(FLAGS) $(VERSION) keccak.cpp

.PHONY:
clean:
	@rm -rf $(REMOVE)
