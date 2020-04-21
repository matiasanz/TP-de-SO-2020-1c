/*
 ============================================================================
 Name        : GameBoy.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <crenito-commons/prueba.h>


int main(void) {
	cr_saludar();
	char* string = string_new();
	string_append(&string, "! Hello World GameBoy!!!");

	puts(string); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
//int main(int argc, char **argv, char** envp) {
//    char** env;
//    for (env = envp; *env != 0; env++) {
//        char* thisEnv = *env;
//        printf("%s\n", thisEnv);
//
//    }
//
//    return EXIT_SUCCESS;
//}
