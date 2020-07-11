#include "../gamecard.h"

void guardarLinea(char* path,char* nuevalinea,int len){
	FILE* f_block;

	f_block=fopen(path,"rb+");

	fseek(f_block,0,SEEK_END);

	fwrite(nuevalinea,sizeof(char),len,f_block);

	fclose(f_block);

}
char* crearLinea(t_mensaje_new_pokemon* unMsjNewPoke ){

	    char* unalinea=string_from_format("%u-%u=%u\n", unMsjNewPoke->pokemon.posicion.pos_x
	    											  , unMsjNewPoke->pokemon.posicion.pos_y
	    											  , unMsjNewPoke->cantidad);
		return unalinea;
}

void sobrescribirLineas(char* path,char* nuevalinea,int len){
	FILE* f_block;

	f_block=fopen(path,"wb+");

	fseek(f_block,0,SEEK_SET);

	fwrite(nuevalinea,sizeof(char),len,f_block);

	fclose(f_block);

}

//****************************************************************

int bloque_disponible(t_bitarray* bitmap,int totalBloques){

	int i=0;
	while(i<totalBloques && bitarray_test_bit(bitmap,i)){
		i++;
	}

	return i;
}

int size_bloque(char* path){
	FILE* f_block;
	f_block=fopen(path,"rb");
	fseek(f_block, 0, SEEK_END);
	int file_size = ftell(f_block);
	fseek(f_block, 0, SEEK_SET);

	fclose(f_block);

	return file_size;
}

//ejemplo ("unaCadena",["1","4","5","8",NULL])
bool contiene_string_en_bloques(char* string, char**bloques){

	char* cadenaCompleta=string_new();
	FILE* archivo;
	int file_size;
	char* cadena;
	bool result;
	char* bin_block;


	int cantbloques=cant_elemetos_array(bloques);

	if(cantbloques>0){
		for(int x=0;x<cantbloques;x++){

				bin_block = string_new();
				string_append(&bin_block,paths_estructuras[BLOCKS]);
				string_append(&bin_block,bloques[x]);
				string_append(&bin_block,".bin");


				archivo=fopen(bin_block,"rb");

				fseek(archivo, 0, SEEK_END);
				file_size = ftell(archivo);
				fseek(archivo, 0, SEEK_SET);

				cadena=malloc(file_size);

				fread(cadena,sizeof(char),file_size,archivo);

				string_append(&cadenaCompleta,cadena);

				fclose(archivo);
				free(bin_block);
				free(cadena);
		}

		result=string_contains(cadenaCompleta,string);

		free(cadenaCompleta);

		return result;

	}

	free(cadenaCompleta);
	return false;
}

int espacioDisponibleEnBloque(char* path){
	return config_get_int_value(config_metadata,"BLOCK_SIZE")-size_bloque(path);
}

int bloquesNecesarios(char* lineaNueva,int maxSizeBloque){
	return (int)((string_length(lineaNueva)+(maxSizeBloque-1))/(maxSizeBloque));
}


//posString:: "x-y", ejem "1-2",["1","4","5","8",NULL]
bool contienePosicionEnBloques(char* string, char**bloques){

	bool result;

	int cantbloques=cant_elemetos_array(bloques);

	if(cantbloques>0){
		char* lineasDeBloques=contenido_de_Bloques_con_mmap(bloques);

		//----------compruebo si esta------
		char** arrayLineasDelPokmeon=string_split(lineasDeBloques,"\n");


		//a la posicion que me pasen le agrego un "=", para facilitar la busqueda
		char* stringPosAdaptado=string_new();
		string_append(&stringPosAdaptado,string);
		string_append(&stringPosAdaptado,"=");

		result=false;

		for(int i=0;arrayLineasDelPokmeon[i]!=NULL;i++){

			if(string_starts_with(arrayLineasDelPokmeon[i],stringPosAdaptado)){
				result=true;
			};
			free(arrayLineasDelPokmeon[i]);
		}


		free(stringPosAdaptado);
		free(arrayLineasDelPokmeon);
		free(lineasDeBloques);

		return result;

	}

	return false;
}

//DEPRECATED
char* contenidoDeBloques(char** bloques){
	char* cadenaCompleta=string_new();
	FILE* archivo;
	int file_size;
	char* cadena;
	char* bin_block;

	int cantbloques=cant_elemetos_array(bloques);

	if(cantbloques>0){
		for(int x=0;x<cantbloques;x++){

				bin_block = string_new();
				string_append(&bin_block,paths_estructuras[BLOCKS]);
				string_append(&bin_block,bloques[x]);
				string_append(&bin_block,".bin");


				archivo=fopen(bin_block,"rb");

				fseek(archivo, 0, SEEK_END);
				file_size = ftell(archivo);
				fseek(archivo, 0, SEEK_SET);

				cadena=malloc(file_size);

				fread(cadena,sizeof(char),file_size,archivo);

				string_append(&cadenaCompleta,cadena);

				fclose(archivo);
				free(bin_block);
				free(cadena);
		}
	}

	return cadenaCompleta;
}

char* contenido_de_Bloques_con_mmap(char** bloques){
	char* cadenaCompleta=string_new();

	FILE* archivo;
	int file_size;
	char* cadena;
	char* bin_block;

	int cantbloques=cant_elemetos_array(bloques);

	if(cantbloques>0){
		for(int x=0;x<cantbloques;x++){

		bin_block = string_new();
		string_append(&bin_block,paths_estructuras[BLOCKS]);
		string_append(&bin_block,bloques[x]);
		string_append(&bin_block,".bin");

		archivo=fopen(bin_block,"rb+");

		fseek(archivo, 0, SEEK_END);
		file_size = ftell(archivo);
		fseek(archivo, 0, SEEK_SET);

		cadena=(char*)mmap(NULL,sizeof(char)*file_size,PROT_READ | PROT_WRITE | PROT_EXEC,MAP_SHARED,fileno(archivo),0);

		string_append(&cadenaCompleta,cadena);


		munmap(cadena,file_size);
		fclose(archivo);
		free(bin_block);

		}
	}

	return cadenaCompleta;
}
