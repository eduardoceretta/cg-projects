/* *************************************************************
*
* Class:  check.h
* Created: 6/4/2006 - 15:12
* Author: malf
*
* Description: 
*   Módulo de checagem de erros.
*
* *************************************************************/

#ifndef __NFR_H__
#define __NFR_H__

#include "import.h"

/************************************************************************
*	nfr_checkBlockMore
*   Teste para verificar se um bloco de dados dentro de uma TAG% qualquer
* tem MAIS valores do que o esperado.
*   Retorna o estado de erro (true or False).
************************************************************************/
NFR_API int nfr_checkBlockMore(NfrImport* f, const char* tagString);
NFR_API int nfr_checkBlockLess(NfrImport* f, const char* tagString);

#endif