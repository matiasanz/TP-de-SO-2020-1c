#include"crenito-logger.h"

// Singleton mentiroso --> este sería el método "getInscance()"
// Revisar esto con los ayudantes!

t_log* get_crnito_logger() {

	if (crenito_logger != NULL)
		return crenito_logger;

	return log_create("./log/crenito_commons.log", "crenito_commons", 1,
			LOG_LEVEL_INFO);
}

