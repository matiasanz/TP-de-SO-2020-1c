/*
 ============================================================================
 Name        : GameCard.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>

int main(void) {
	char* string = string_new();
	string_append(&string, "!!!!Hello World GameCard!!!");

	puts(string); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
