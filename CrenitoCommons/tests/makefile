TARGET = unit-tests

CRENITO-COMMONS-BIN = ../src/build

LIBS = -lpthread -lcommons -lcspecs
CC =  gcc 
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET) 
all: default 

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) 
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS) 
	$(CC) -I"../src" $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS) 

$(TARGET): $(OBJECTS)
	$(CC)  $(OBJECTS) $(CFLAGS) -L"$(CRENITO-COMMONS-BIN)" -lCrenitoCommons $(LIBS) -o $@ 

clean: 
	-rm -f *.o
	-rm -f $(TARGET)

debug: CC += -DDEBUG -g
debug: all