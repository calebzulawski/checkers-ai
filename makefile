CC=g++
CFLAGS=-std=c++11 -c -Wall
LDFLAGS=-pthread
SOURCES=wrappers.cpp ai.cpp board.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=checkers-ai

all: executable

debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: executable

executable: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm checkers-ai