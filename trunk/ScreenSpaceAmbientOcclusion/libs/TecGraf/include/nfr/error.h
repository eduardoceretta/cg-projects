/* *************************************************************
*
* Class:  error.h
* Created: 9/2/2007
* Author: rodesp
* $Id: error.h,v 1.3 2008/02/11 20:18:07 rodesp Exp $
*
* Description: 
*   M�dulo de tratamento de erros.
*
* *************************************************************/

#ifndef __NFR_ERROR_H__
#define __NFR_ERROR_H__

/*
typedef enum {
  NFR_NO_ERROR = 0,
  NFR_ERROR = 1 
};
*/
#include "defines.h"

typedef struct NfrError NfrError;

typedef void (*NfrWarningFunc) (const char* msg, void* data);
typedef void (*NfrErrorFunc) (const char* msg, void* data);

/************************************************************************
* Cria um novo tratador de erros.
************************************************************************/
NFR_API NfrError* nfr_createError();
NFR_API void nfr_destroyError(NfrError* e);

/************************************************************************
* Altera fun��o chamada quando um aviso � reportado.
************************************************************************/
NFR_API void nfr_setWarningFunc(NfrError* e, NfrWarningFunc cb, void* data);

/************************************************************************
* Reporta um aviso.
************************************************************************/
NFR_API void nfr_reportWarning(NfrError* e, const char* msg);
NFR_API void nfr_reportWarningf(NfrError* e, const char* format, ...);

/************************************************************************
* Altera fun��o chamada quando um erro � reportado.
************************************************************************/
NFR_API void nfr_setErrorFunc(NfrError* e, NfrErrorFunc cb, void* data);

/************************************************************************
* Reporta um erro.
* @returns NFR_ERROR
************************************************************************/
NFR_API int nfr_reportError(NfrError* e, const char* msg);
NFR_API int nfr_reportErrorf(NfrError* e, const char* format, ...);

/************************************************************************
* Retorna se o flag de erro est� ligado.
************************************************************************/
NFR_API int nfr_hasError(NfrError* e);

/************************************************************************
* Retorna o string de erro, se o flag de erro estiver ligado.                                                                     
************************************************************************/
NFR_API const char* nfr_getError(NfrError* e);

/************************************************************************
* Limpa o flag de erro.                                                                     
************************************************************************/
NFR_API void nfr_clearError(NfrError* e);

#endif