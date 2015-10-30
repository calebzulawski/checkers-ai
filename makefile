CC=g++
CFLAGS=-std=c++11 -c -Wall -Wfatal-errors
LDFLAGS=-pthread
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=checkers-ai

all: executable

debug: CFLAGS += -g -pg
debug: LDFLAGS += -g -pg
debug: executable

executable: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm checkers-ai