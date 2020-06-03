///*
// * sync_log.c
// *
// *  Created on: 2 jun. 2020
// *      Author: utnso
// */
//
//#include "sync_log.h"
//
//void log_sync(t_log_sync* logger, t_log_level log_level, logger, char* message,	...) {
//
//	char* mensaje;
//
//	va_list arguments;
//	va_start(arguments, message);
//	mensaje = string_from_vformat(message, arguments);
//	va_end(arguments);
//
//	switch (log_level) {
//	case LOG_LEVEL_DEBUG:
//
//		pthread_mutex_lock(&logger->mutex);
//		log_info(logger->lgr, mensaje);
//		pthread_mutex_unlock(&logger->mutex);
//
//		break;
//	case LOG_LEVEL_ERROR:
//
//		pthread_mutex_lock(&logger->mutex);
//		log_error(logger->lgr, mensaje);
//		pthread_mutex_unlock(&logger->mutex);
//
//		break;
//	case LOG_LEVEL_INFO:
//
//		pthread_mutex_lock(&logger->mutex);
//		log_info(logger->lgr, mensaje);
//		pthread_mutex_unlock(&logger->mutex);
//
//		break;
//	case LOG_LEVEL_TRACE:
//
//		pthread_mutex_lock(&logger->mutex);
//		log_trace(logger->lgr, mensaje);
//		pthread_mutex_unlock(&logger->mutex);
//
//		break;
//	case LOG_LEVEL_WARNING:
//
//		pthread_mutex_lock(&logger->mutex);
//		log_warning(logger->lgr, mensaje);
//		pthread_mutex_unlock(&logger->mutex);
//
//		break;
//	default:
//		break;
//	}
//
//	free(message);
//}
//
//void log_sync_destroy(t_log_sync* logger) {
//
//	log_destroy(logger->lgr);
//	free(logger);
//}
//
//t_log_sync* log_sync_create(char* file, char *program_name,
//bool is_active_console, t_log_level level) {
//
//	t_log_sync* logger = malloc(sizeof(t_log_sync));
//	logger->lgr = log_create(file, program_name, is_active_console, level);
//	pthread_mutex_init(&logger->mutex, NULL);
//
//	return logger;
//}
