TARGET = ../Broker

CRENITO-COMMONS-DIR = ../../CrenitoCommons
CRENITO-COMMONS-BIN = $(CRENITO-COMMONS-DIR)/src/build


LIBS = -lpthread -lcommons
CC = gcc 
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET) 
all: default 

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) 
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $< -o $@ 

.PRECIOUS: $(TARGET) $(OBJECTS) 

$(TARGET): $(OBJECTS)
	$(CC)  $(OBJECTS) $(CFLAGS) -L"$(CRENITO-COMMONS-BIN)" -lCrenitoCommons $(LIBS) -o $@ 

clean: 
	-rm -f *.o
	-rm -f $(TARGET)