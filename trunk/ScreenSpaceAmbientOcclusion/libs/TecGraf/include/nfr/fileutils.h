/************************************************************************
 * Uso interno
 *
 * Created: 8/2/2006
 * Author: MALF 
 * Description: 
 *   Funções utilitárias para operações nos arquivos de entrada e saída.
 * 
 ************************************************************************/

#ifndef __NFR_FILE_UTILS_H__
#define __NFR_FILE_UTILS_H__

#include <stdlib.h>
#include <stdio.h>

#define NFR_FILE_LABEL_MAX_SIZE 80
#define NFR_FILE_LABEL_FORMAT " ' %80[^']'" 

//DataTypes
typedef enum {
  NFR_FILE_DATA_INT = 0,
  NFR_FILE_DATA_FLOAT,
  NFR_FILE_DATA_STRING,
  NFR_FILE_DATA_BYTE
} NfrFileDataType;

#ifdef _WIN32
  typedef __int64 NfrFileOffset;
#else
  typedef size_t NfrFileOffset;
#endif

FILE* nfrFileOpen (const char* filename, const char* mode);

void nfrFileClose (FILE* fp);

int nfrFileGotoOffset (FILE* fp, NfrFileOffset offset);

NfrFileOffset nfrFileGetCurrentOffset (FILE* fp);

int nfrFileGetSize (FILE* fp, NfrFileOffset* size);

int nfrFileReadTextOffset (FILE* fp, NfrFileOffset* offset);

int nfrFileWriteTextOffset (FILE* fp, NfrFileOffset offset);

/************************************************************************
* Lê um float do arquivo neutro ASCII.
************************************************************************/
float nfrFileReadTextFloat(FILE *f);

/************************************************************************
* Lê um inteiro do arquivo neutro ASCII.
************************************************************************/
int nfrFileReadTextInt(FILE *f);

/************************************************************************
* Lê um label do arquivo neutro ASCII, no formato "' string '".
* Remove espaços em branco antes e depois do string.
* Retorna strlen(label) or 0.
************************************************************************/
int nfrFileReadTextLabel(FILE* fr, char* buffer);

/************************************************************************
* Escreve um label no arquivo binário.
************************************************************************/
int nfrFileWriteBinLabel(FILE* fb, const char* buffer);

/************************************************************************
* Le um label do arquivo binário.
* Retorna strlen(label) or 0.
************************************************************************/
int nfrFileReadBinLabel(FILE* fb, char* buffer);

/************************************************************************
* Le um int do arquivo binário.
* Retorna OK: 1 or 0.
************************************************************************/
int nfrFileReadBinInt(FILE* fb);

/************************************************************************
* Le um vetor de valores do tipo "int" do arquivo binário.
* Retorna OK: 1 or 0.
************************************************************************/
int nfrFileReadBinInts(FILE* fb, size_t count, int* values);

/************************************************************************
* Escreve um int no arquivo binário.
* Retorna OK: 1 or 0.
************************************************************************/
int nfrFileWriteBinInt(FILE* fp, int value);

/************************************************************************
* Escreve um vetor de valores do tipo "int" para um arquivo binário.
* Retorna OK: 1 or 0.
************************************************************************/
int nfrFileWriteBinInts(FILE* fp, size_t count, const int* values);

/************************************************************************
* Le um float do arquivo binário.
* Retorna OK: 1 or 0.
************************************************************************/
float nfrFileReadBinFloat(FILE* fb);

/************************************************************************
* Le um vetor de valores do tipo "float" do arquivo binário.
* Retorna OK: 1 or 0.
************************************************************************/
int nfrFileReadBinFloats(FILE* fb, size_t count, float* values);

/************************************************************************
* Escreve um float no arquivo binário.
* Retorna OK: 1 or 0.
************************************************************************/
int nfrFileWriteBinFloat(FILE* fb, float value);

/************************************************************************
* Escreve um vetor de valores do tipo "float" para um arquivo binário.
* Retorna OK: 1 or 0.
************************************************************************/
int nfrFileWriteBinFloats(FILE* fp, size_t count, const float* values);

/************************************************************************
* nfrFileReadBinData 
*
* @param fb Arquivo binario de entrada.
* @param offset Local de inicio de leitura.
* @param datatype Tipo de dados a serem lidos.
* @param count Quantidade de dados a serem lidas.
* @return volta o numero de bytes de uma string lida
************************************************************************/
size_t nfrFileReadBinData(FILE *fb, 
                          NfrFileOffset offset, 
                          NfrFileDataType datatype, 
                          size_t count, 
                          void* data);

/************************************************************************/
/* HORIZONTAL DATA
 ************************************************************************/  
void nfrFileReadTextWriteBinFloat(FILE *fr, FILE *fb, int num);

void nfrFileReadTextWriteBinInt(FILE *fr, FILE *fb, int num);

#endif
