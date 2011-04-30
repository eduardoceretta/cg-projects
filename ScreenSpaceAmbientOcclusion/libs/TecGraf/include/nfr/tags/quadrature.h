/************************************************************************
 *
 * Class:  quadrature
 * Created: 7/2/2006
 * Author: MALF 
 *----------------------------------------------------------------------
 * Description: 
 *
 ************************************************************************/

#ifndef __NFR_QUADRATURE_H__
#define __NFR_QUADRATURE_H__

#include "../import.h"

/************************************************************************
* nfr_quadratureTag
* 
*   Importa tag %QUADRATURE para o arquivo bin�rio. 
*   - Assume que os ids das quadraturas est�o no intervalo [1,num_quad].
*   - Os dados de RESEQUENCE s�o tratados na importa��o e n�o s�o adicionados
* ao arquivo bin�rio.
*
* @return NFR_NO_ERROR ou NFR_ERROR.
* @warning Uso interno.
************************************************************************/
int nfr_quadratureTag(NfrImport* f, char* head, char* tail, NfrFileOffset* offset, int integration);

/************************************************************************
* nfr_quadratureTagCloseData
*   Salva os dados ainda pendentes de quadraturas para o arquivo bin�rio e
* libera parte da mem�ria ocupada.
* WARNING Internal use
************************************************************************/
int nfr_quadratureTagClose(NfrImport* f, NfrFileOffset* offset);

/************************************************************************
*	nfr_getNumQuadratures
* Retorna o n�mero total de quadraturas. 
************************************************************************/
NFR_API int nfr_getNumQuadratures(NfrFile* f);

/************************************************************************
*	nfr_getNumQuadraturesOfType
* Retorna o n�mero de quadraturas de um determinado tipo.
************************************************************************/
NFR_API int nfr_getNumQuadraturesOfType(NfrFile* f, int quadType);

/************************************************************************
*	nfr_getQuadDim
* Retorna o n�mero de dimens�es da quadratura do tipo dado: 
*   1: r, 2: (r,s), 3: (r,s,t)
************************************************************************/
NFR_API int nfr_getQuadDim(int quadType);

/************************************************************************
*	nfr_getQuadIds
* Retorna os ids das quadraturas.
************************************************************************/
NFR_API int nfr_getQuadIds(NfrFile* f, int quadType, int* data);

/************************************************************************
*	nfr_getQuadOrders
* Retorna as ordens (r,s,t) das quadraturas.
************************************************************************/
NFR_API int nfr_getQuadOrders(NfrFile* f, int quadType, int* data);

#endif

