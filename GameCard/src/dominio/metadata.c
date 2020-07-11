#include "../gamecard.h"

uint32_t config_metadata_get_block_size(){
	pthread_mutex_lock(&mutexConfigMetadata);
	uint32_t BlockSize = config_get_int_value(config_metadata, "BLOCK_SIZE");
	pthread_mutex_unlock(&mutexConfigMetadata);

	return BlockSize;
}

uint32_t config_metadata_get_blocks(){
	pthread_mutex_lock(&mutexConfigMetadata);
	uint32_t Blocks = config_get_int_value(config_metadata, "BLOCKS");
	pthread_mutex_unlock(&mutexConfigMetadata);

	return Blocks;
}
