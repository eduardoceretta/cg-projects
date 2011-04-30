/************************************************************************
 *
 * Class:  rezone
 * Created: 21/2/2006
 * Author: MALF 
 *----------------------------------------------------------------------
 * Description: 
 *
 ************************************************************************/

#ifndef __NFR_REZONE_H__
#define __NFR_REZONE_H__

#include "../import.h"

/************************************************************************
* nfr_rezoneTag
* 
*   Importa tag %REEZONE para o arquivo binário. 
*   Essa tag é válida somente para o primeiro RESULT.CASE. Cada 
* REZONE.CASE representa um STEP do primeiro RESULT.CASE, a partir 
* do qual os elementos indicados serão "removidos" da malha.
*   - Assume que os ids dos nós e elementos estão no intervalo [1,num_elem].
*   - Assume que as tags %NODE e %ELEMENT já foram importadas.
*
* @return NFR_NO_ERROR ou NFR_ERROR.
* @warning Uso interno.
************************************************************************/
int nfr_rezoneTag(NfrImport* f, char* head, char* tail, NfrFileOffset* offset);

/************************************************************************
* nfr_rezoneTagClose
*   Salva os dados de rezone para o arquivo binário.
************************************************************************/
int nfr_rezoneTagClose(NfrImport* f, NfrFileOffset* offset);

/************************************************************************
*	nfr_getRezoneArray
*   Retorna um vetor onde cada posição contém o caso de rezone do elemento.
*
*   v[eleId-1] = rezoneId
*   
*   Se o elemento não tem rezone, rezoneId é zero.
* @param data Vetor para onde os dados serão lidos.
* @return Se OK: 1, senão: 0.
************************************************************************/
NFR_API int nfr_getRezoneArray(NfrFile* f, int* data);

#endif

