# Definimos la tarea que va a ejecutar make por defecto Y el nombre del binario final
TARGET = ../Team 

CRENITO-COMMONS-DIR = ../../CrenitoCommons
CRENITO-COMMONS-BIN = $(CRENITO-COMMONS-DIR)/src/build

LIBS = -lpthread -lcommons # bibliotecas incluidas
CC = gcc 				   # se define compilador
CFLAGS = -g -Wall          # Compilar con flags -g (debug) y -Wall (muestra los warnings)

# Crear tareas del makefile:
#{make sin argumentos ejecuta la tarea por default; make unaTarea ejecita "unaTarea"} ejemplo: make all o make clean

.PHONY: default all clean
# .PHONY dice a make: si(nombre(archivo) == nombre(regla)) ejecutar la regla; sino por defecto trata de tomar archivo

default: $(TARGET) # regla por defecto
all: default # Si ejecutan all, que ejecute la default

create-dirs:
	mkdir -p ../log

# Definamos un par de constantes mas, OBJECTS (que vamos a usar para generar los .o)
# y HEADERS, para incluir los headers que definamos.

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h) # wildcard implica todos los .h

%.o: %.c $(HEADERS) # Las variables que le pasamos a la tarea, le decimos que use todos los .c y todos los .h
	$(CC) -I"../../CrenitoCommons/src" $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

 # Usamos compilador para linkear los objects a las bibliotecas y tener binario
$(TARGET): create-dirs $(OBJECTS)
	$(CC)  $(OBJECTS) $(CFLAGS) -L"$(CRENITO-COMMONS-BIN)" -lCrenitoCommons $(LIBS) -o $@ 

clean: # tarea para eliminar todos los .o y el binario
	-rm -f *.o
	-rm -f $(TARGET)
	