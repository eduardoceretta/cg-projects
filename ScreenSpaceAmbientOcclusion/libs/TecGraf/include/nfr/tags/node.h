/************************************************************************
 ************************************************************************/

#ifndef __NFR_NODE_H__
#define __NFR_NODE_H__

#include "../import.h"

/************************************************************************
* nfr_nodeTag
* 
*   Importa tag %NODE para o arquivo binário. 
*   - Assume que os ids dos nós estão no intervalo [1,num_nodes].
*   - As coordenadas dos nós são adicionadas na mesma ordem do arquivo neutro.
*   - %NODE.COORD: Adiciona um vetor de ids e outro de coordenadas (x,y,z).
* O número de coordenadas lidas deve ser igual a num_nodes.
*
* @return NFR_NO_ERROR ou NFR_ERROR.
* @warning Uso interno.
************************************************************************/
int nfr_nodeTag(NfrImport* f, char * head, char * tail, NfrFileOffset *offset);

/************************************************************************
* nfr_getNumNodes
*
* @param f FILE
* @return Se OK: número de nós. Senão: 0.
************************************************************************/
NFR_API int nfr_getNumNodes(NfrFile* f);

/************************************************************************
* nfr_getNodeCoordIds
*
* @param f FILE
* @param ids Vetor para onde os dados serão lidos.
* @return Se OK: 1. Senão: 0.
************************************************************************/
NFR_API int nfr_getNodeCoordIds(NfrFile* f, int* ids);

/************************************************************************
* nfr_getNodeCoords
*
* @param f FILE
* @param coords Vetor para onde os dados serão lidos, de tamanho igual a 
* 3*nnodes.
* @return Se OK: 1. Senão: 0.
************************************************************************/
NFR_API int nfr_getNodeCoords(NfrFile* f, float* coords);

/************************************************************************
* nfr_getNumNodeLabels
*   Retorna o número de labels de nós, que pode ser diferente do número
* de nós.
************************************************************************/
NFR_API int nfr_getNumNodeLabels(NfrFile* f);

/************************************************************************
* nfr_getNodeLabelsSize
* @return Se OK: Numero de total de bytes (incluindo \0). Senão: 0.
************************************************************************/
NFR_API int nfr_getNodeLabelsSize(NfrFile* f);

/************************************************************************
* nfr_getNodeLabelIds
*   Preenche o vetor de ids com os ids dos nós.
* @param f FILE
* @param ids Vetor para onde os dados serão lidos, de tamanho igual ao 
* número de labels.
* @return Se OK: 1, senão: 0.
************************************************************************/
NFR_API int nfr_getNodeLabelIds(NfrFile* f, int* ids);

/************************************************************************
* nfr_getNodeLabels
* @param f FILE
* @param coords Vetor para onde os dados serão lidos, de tamanho igual ao 
* retornado por nfr_getNodeLabelsSize.
* @return Se OK: 1. Senão: 0.
************************************************************************/
NFR_API int nfr_getNodeLabels(NfrFile* f, char* labels);


/************************************************************************
* nfr_getNumNodeSupports
*   Retorna o número de suportes nodais, que pode ser diferente do número
* de nós.
************************************************************************/
NFR_API int nfr_getNumNodeSupports(NfrFile* f);

/************************************************************************
* nfr_getNodeSupportIds
* Preenche o vetor de ids com os ids dos suportes nodais.
* Parametros:
*   ids Vetor para onde os dados serão lidos, de tamanho igual a 
*   num_node_supports 
* Retorna: Se OK: 1, senão: 0.
************************************************************************/
NFR_API int nfr_getNodeSupportIds(NfrFile* f, int* ids);

/************************************************************************
* nfr_getNodeSupports
* Retorna os suportes nodais, no formato:
*   { {[dx] [dy] [dz] [rx] [ry] [rz]}, ... }  (0=free, 1=fixed)
* Parametros:
*   data Vetor data para qual os dados serão lidos, de tamanho igual a
*   6*num_node_supports
* Retorna: Se OK: 1. Senão: 0.
************************************************************************/
NFR_API int nfr_getNodeSupports(NfrFile* f, int* data);

#endif

