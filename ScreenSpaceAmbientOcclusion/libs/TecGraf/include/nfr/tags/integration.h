/************************************************************************
 *
 * Class:  integration
 * Created: 7/2/2006
 * Author: MALF 
 *----------------------------------------------------------------------
 * Description: 
 *
 ************************************************************************/

#ifndef __NFR_INTEGRATION_H__
#define __NFR_INTEGRATION_H__

#include "../import.h"

/************************************************************************
* nfr_integrationTag
* 
*   Importa tag %INTEGRATION para o arquivo binário. 
*   - Assume que "thickness_id" está no intervalo [1, num_thickness].
*
* @return NFR_NO_ERROR ou NFR_ERROR.
* @warning Uso interno.
************************************************************************/
int nfr_integrationTag(NfrImport* f, char* head, char* tail, NfrFileOffset* offset);

/************************************************************************
* nfr_getNumIntegrationOrders
*
* @param f FILE
* @return Se OK: número de "element integration orders". Senão: 0.
************************************************************************/
NFR_API int nfr_getNumIntegrationOrders(NfrFile* f);

/************************************************************************
* nfr_getIntegrationOrderIds
*
* @param f FILE
* @param data Vetor para onde os dados serão lidos.
* @return Se OK: 1. Senão: 0.
************************************************************************/
NFR_API int nfr_getIntegrationOrderIds(NfrFile* f, int* data);

/************************************************************************
* nfr_getIntegrationOrders
*
* @param f FILE
* @param data Vetor para onde os dados serão lidos, de tamanho igual a 
* 6*num_integration_orders.
* @return Se OK: 1. Senão: 0.
************************************************************************/
NFR_API int nfr_getIntegrationOrders(NfrFile* f, int* data);

#endif

