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
*   Importa tag %QUADRATURE para o arquivo binário. 
*   - Assume que os ids das quadraturas estão no intervalo [1,num_quad].
*   - Os dados de RESEQUENCE são tratados na importação e não são adicionados
* ao arquivo binário.
*
* @return NFR_NO_ERROR ou NFR_ERROR.
* @warning Uso interno.
************************************************************************/
int nfr_quadratureTag(NfrImport* f, char* head, char* tail, NfrFileOffset* offset, int integration);

/************************************************************************
* nfr_quadratureTagCloseData
*   Salva os dados ainda pendentes de quadraturas para o arquivo binário e
* libera parte da memória ocupada.
* WARNING Internal use
************************************************************************/
int nfr_quadratureTagClose(NfrImport* f, NfrFileOffset* offset);

/************************************************************************
*	nfr_getNumQuadratures
* Retorna o número total de quadraturas. 
************************************************************************/
NFR_API int nfr_getNumQuadratures(NfrFile* f);

/************************************************************************
*	nfr_getNumQuadraturesOfType
* Retorna o número de quadraturas de um determinado tipo.
************************************************************************/
NFR_API int nfr_getNumQuadraturesOfType(NfrFile* f, int quadType);

/************************************************************************
*	nfr_getQuadDim
* Retorna o número de dimensões da quadratura do tipo dado: 
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

