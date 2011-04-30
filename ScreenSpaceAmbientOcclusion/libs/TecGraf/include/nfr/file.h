
#ifndef __NFR_FILE_H__
#define __NFR_FILE_H__

#include "defines.h"
#include "error.h"
#include "progress.h"

typedef struct NfrFile NfrFile;

/************************************************************************
* nfr_createFile
*   Cria uma nova refer�ncia para um arquivo bin�rio.
************************************************************************/
NFR_API NfrFile* nfr_createFile(const char* binfile, const char* luafile);

/************************************************************************
* nfr_destroyFile
*   Destr�i uma refer�ncia para um arquivo bin�rio.
************************************************************************/
NFR_API void nfr_destroyFile(NfrFile* f);

/************************************************************************
* nfr_open
*   Abre um arquivo binario. Retorna 1 se ok, ou 0 caso contr�rio.
************************************************************************/
NFR_API int nfr_openFile(NfrFile* f);

/************************************************************************
* Fecha um arquivo binario.
************************************************************************/
NFR_API void nfr_closeFile(NfrFile* f);

/************************************************************************
* nfr_getFileErrorHandler
*   Retorna o tratador de erros associado ao arquivo bin�rio.
************************************************************************/
NFR_API NfrError* nfr_getFileErrorHandler(NfrFile* f);

#endif
