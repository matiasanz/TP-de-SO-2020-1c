/*
 * memoria_dump.h
 *
 *  Created on: 13 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_MEMORIA_UTILS_MEMORIA_DUMP_H_
#define SRC_MEMORIA_MEMORIA_UTILS_MEMORIA_DUMP_H_

#include "../particiones_dinamicas/particiones_dinamicas.h"
#include "../buddy_system/buddy_system.h"
#include "../memoria_modelo/particion.h"

t_log* dump_logger;
#define SEPARADOR_DUMP_CACHE "----------------------------------------------------------------------------------------------------------------"

void log_dump_cache(int signum);
void log_debug_dump_cache();

#endif /* SRC_MEMORIA_MEMORIA_UTILS_MEMORIA_DUMP_H_ */
