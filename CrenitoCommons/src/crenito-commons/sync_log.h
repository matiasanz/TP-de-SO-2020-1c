///*
// * sync_log.h
// *
// *  Created on: 2 jun. 2020
// *      Author: utnso
// */
//
//#include<commons/log.h>
//#include<pthread.h>
//#include<stdlib.h>
//#include <commons/string.h>
//#include <commons/temporal.h>
//#include "error.h"
//#include "string.h"
//#include <commons/txt.h>
//#include <commons/process.h>
//
//#include <stdlib.h>
//#include <stdbool.h>
//#include <stdarg.h>
//
//typedef struct {
//	t_log* lgr;
//	pthread_mutex_t mutex;
//} t_log_sync;
//
//t_log_sync* log_sync_create(char* file, char *program_name, bool is_active_console, t_log_level level);
//
//void log_sync_destroy(t_log_sync* logger);
//
//log_sync(t_log_sync* logger, t_log_level log_level, logger, char* message,	...);
