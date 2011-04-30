/************************************************************************
* Class:  import
* Created: 7/2/2006
* Author: MALF 
*----------------------------------------------------------------------
* Description: 
* Estrutura de dados usados na importa��o do neutralfile.
* @warning Uso interno.
************************************************************************/


#ifndef __NFR_IMPORT_H__
#define __NFR_IMPORT_H__

#include "defines.h"
#include "error.h"
#include "progress.h"

typedef struct NfrImport NfrImport;

/************************************************************************
* nfr_createImport
*   Cria um novo importador para converter arquivos neutros em formato
* texto (.pos) para bin�rio.
*	  Recebe o arquivo 'NEUTRAL FILE' de entrada no formato de texto e o
* converte para o formato bin�rio, criando a sua estrutura de organizacao
* atraves de uma tabela lua que e' salva em um arquivo de saida txt.
*
* @param infile Arquivo 'NEUTRAL FILE' de entrada (.pos).
* @param binfile Arquivo binario de saida.
* @param luafile Tabela lua de saida.
************************************************************************/
NFR_API NfrImport* nfr_createImport(const char* infile, const char* binfile, const char* luafile);

/************************************************************************
* nfr_destroyImport
*   Destr�i um importador de arquivos neutros.
************************************************************************/
NFR_API void nfr_destroyImport(NfrImport* f);

/************************************************************************
* nfr_convertInput
*   Importa arquivo texto (.pos) para bin�rio.
* @returns Se ok: NFR_NO_ERROR, sen�o NFR_ERROR.
************************************************************************/
NFR_API int nfr_convertInput(NfrImport* f);

/************************************************************************
* nfr_getImportErrorHandler
*   Retorna o tratador de erros associado ao importador.
************************************************************************/
NFR_API NfrError* nfr_getImportErrorHandler(NfrImport* f);

/************************************************************************
* nfr_getImportProgressHandler
*   Retorna o tratador do progresso da importa��o associado ao importador.
************************************************************************/
NFR_API NfrProgress* nfr_getImportProgressHandler(NfrImport* f);

/************************************************************************
* nfr_getImportProgressRatio
*   Retorna a fra��o [0,1] da posi��o atual da importa��o em rela��o 
* � total.
************************************************************************/
NFR_API double nfr_getImportProgressRatio(NfrImport* f);

#include "file.h"

#endif
