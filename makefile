CC=g++
CFLAGS=-std=c++11 -c -Wall
LDFLAGS=
SOURCES=wrappers.cpp ai.cpp board.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=checkers-ai

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@