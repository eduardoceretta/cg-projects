/**************************************************************
 **************************************************************/

#ifndef __NFR_ELEMENT_H__
#define __NFR_ELEMENT_H__

#include "../defines.h"
#include "../import.h"

/************************************************************************
*	nfr_getNumElements
* Retorna o n�mero total de elementos. 
************************************************************************/
NFR_API int nfr_getNumElements(NfrFile* f);

/************************************************************************
*	nfr_getNumElementsOfType
* Retorna o n�mero de elementos de um determinado tipo.
************************************************************************/
NFR_API int nfr_getNumElementsOfType(NfrFile* f, int eleType);

/************************************************************************
*	nfr_getEleResequence
* Retorna em reseqOut a reordena��o dos n�s do tipo do elemento.
************************************************************************/
NFR_API int nfr_getEleResequence(NfrFile* f, int eleType, int* reseqOut);

/************************************************************************
* nfr_getEleDataSize
*
* @param L Lua state
* @param eleType Tipo do elemento.
* @param fieldId Identificador de qual coluna sera' retornada (NfrElementFieldId).
* @return Retorna N�mero de posi��es do vetor de dados (num_elementos*num_fields).
*
************************************************************************/
NFR_API int nfr_getEleDataSize(NfrFile* f, int eleType, int fieldId);

/************************************************************************
* nfr_getEleData
* @param eleType Tipo do elemento.
* @param fieldId Identificador de qual coluna sera' retornada (NfrElementFieldId).
* @param data Vetor data para qual os dados sao salvos
* @param size Tamanho do vetor (em Y)
* @return Se OK: 1. Senao: 0.
************************************************************************/
NFR_API int nfr_getEleData(NfrFile* f, int eleType, int fieldId, int* data);

NFR_API int nfr_getEleNumNodes(int eleTypeId);


/*************************************************************************
 *
 * Uso interno
 *
 *************************************************************************/

/************************************************************************
*   Importa tag %ELEMENT para o arquivo bin�rio. 
*   - Assume que os ids dos elementos est�o no intervalo [1,num_elem].
*   - Os elementos s�o adicionados na mesma ordem do arquivo neutro.
*   - %ELEMENT.<tipo>: Adiciona um vetor de ids e outro de incid�ncias.
* O n�mero total de elementos deve ser igual a num_nodes.
*   - A tag %ELEMENT.<tipo>.RESEQUENCE � tratada na importa��o e as
* incid�ncias s�o adicionadas j� convertidas.
*   - Assume que todos os labels %ELEMENT. v�m antes dos labels %RESULT.
*   - Assume que a tag %QUADRATURE vem antes de %ELEMENT.
*
* @return NFR_NO_ERROR ou NFR_ERROR.
* WARNING Internal use
************************************************************************/
int nfr_elementTag(NfrImport* f, char * head, char * tail, NfrFileOffset* offset);

/************************************************************************
* Salva os dados ainda pendentes de elementos para o arquivo bin�rio e
* libera parte da mem�ria ocupada.
* WARNING Internal use
************************************************************************/
int nfr_elementTagClose(NfrImport* f, NfrFileOffset* offset);

#endif
