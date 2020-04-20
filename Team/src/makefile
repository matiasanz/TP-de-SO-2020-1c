TARGET = ../bin/Team
LIBS = -lpthread -lcommons 
CC = gcc 
CFLAGS = -g -Wall

.PHONY: default all clean

create-dir:
	mkdir -p ../bin
	
default: $(TARGET) 
all: default 

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) 
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $< -o $@ 

.PRECIOUS: $(TARGET) $(OBJECTS) 

$(TARGET): create-dir $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@ 

clean: 
	-rm -f *.o
	-rm -f $(TARGET)