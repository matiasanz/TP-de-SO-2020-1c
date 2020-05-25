TARGET = ../Team 

CRENITO-COMMONS-DIR = ../../CrenitoCommons
CRENITO-COMMONS-BIN = $(CRENITO-COMMONS-DIR)/src/build

LIBS = -lpthread -lcommons # bibliotecas incluidas
CC = gcc 				   # compilador
CFLAGS = -g -Wall          # -g (debug) y -Wall (muestra los warnings)

# Crear tareas del makefile: {make sin argumentos -> ejecuta la tarea por default;
							# make unaTarea       -> ejecuta "unaTarea"} ejs: make all, make clean

.PHONY: default all clean
# .PHONY dice a make: si(nombre(archivo) == nombre(regla)) ejecutar la regla; sino por defecto trata de tomar archivo

default: $(TARGET) # regla por defecto
all: default # Si ejecutan all, que ejecute la default

create-dirs:
	mkdir -p ../log

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) #genera los .o
HEADERS = $(wildcard *.h) # wildcard implica todos los .h

%.o: %.c $(HEADERS) # usar todos los .c y todos los .h
	$(CC) -I"../../CrenitoCommons/src" $(CFLAGS) -c $< -o $@

# Includes, dispuestos en orden
#	$(CC) -I"dominio/estructuras auxiliares/listas" $(CFLAGS) -c $< -o $@ #ampliacion de las commons
#	$(CC) -I"dominio" $(CFLAGS) -c $< -o $@	#tiene header global
#	$(CC) -I"dominio/estructuras principales" $(CFLAGS) -c $< -o $@ #estructuras basicas tales como entrenador, pokemon, posicion. Perfectamente funcionan solas.
#	$(CC) -I"dominio/estructuras auxiliares" $(CFLAGS) -c $< -o $@ #necesitan a las principales
#	$(CC) -I"hilos del team" $(CFLAGS) -c $< -o $@ #ampliacion del programa principal

.PRECIOUS: $(TARGET) $(OBJECTS)

 # Usamos compilador para linkear los objects a las bibliotecas y tener binario
$(TARGET): create-dirs $(OBJECTS)
	$(CC)  $(OBJECTS) $(CFLAGS) -L"$(CRENITO-COMMONS-BIN)" -lCrenitoCommons $(LIBS) -o $@ 

clean: # tarea para eliminar todos los .o y el binario
	-rm -f *.o
	-rm -f $(TARGET)