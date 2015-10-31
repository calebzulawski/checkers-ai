CC=g++
CFLAGS=-std=c++11 -c -Wall -Wfatal-errors
LDFLAGS=-pthread
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=checkers-ai

all: executable

prof: CFLAGS += -pg
prof: LDFLAGS += -pg
prof: debug

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
	rm gmon.out