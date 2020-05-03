/*
 ============================================================================
 Name        : Team.c
 Author      : 
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "team.h"


int main(void) {

	printf("hola soy el team");

	if(inicializar()==-1){
		exit_team();
		return EXIT_FAILURE;
	};

	return EXIT_SUCCESS;
}

int inicializar(){

	config=config_create(CONFIG_PATH);

	logger=log_create(config_get_string_value(config,"LOG_FILE"),TEAM_STRING,true,LOG_LEVEL_INFO);

	subscribpcion_colas();

	return 1;
}

int subscribpcion_colas(){

	//TO DO//

	return 1;
}



void exit_team(){

	log_destroy(logger);
	config_destroy(config);

}
