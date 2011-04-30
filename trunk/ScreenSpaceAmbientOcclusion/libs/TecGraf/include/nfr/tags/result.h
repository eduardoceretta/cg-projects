/**************************************************************
 **************************************************************/

#ifndef __NFR_RESULT_H__
#define __NFR_RESULT_H__

#include "../import.h"

typedef enum {
  NFR_RES_NODAL_DISPLACEMENT = 0,

  /* Scalar */
  NFR_RES_NODAL_SCALAR,
  NFR_RES_ELE_NODAL_SCALAR,
  NFR_RES_ELE_GAUSS_SCALAR,

  /* Vector */
  NFR_RES_NODAL_VECTOR,
  NFR_RES_ELE_NODAL_VECTOR,
  NFR_RES_ELE_GAUSS_VECTOR,

  /* Tensor */
  NFR_RES_NODAL_TENSOR,
  NFR_RES_ELE_NODAL_TENSOR,
  NFR_RES_ELE_GAUSS_TENSOR,

  /* Principal tensor */
  NFR_RES_NODAL_PRI_TENSOR,
  NFR_RES_ELE_NODAL_PRI_TENSOR,
  NFR_RES_ELE_GAUSS_PRI_TENSOR,

  NFR_RES_TYPE_MAX,
} NfrResultType;


/* %RESULT */
NFR_API int nfr_getResNumCases(NfrFile* f);
NFR_API void nfr_getResCaseIds(NfrFile* f, int* data);
NFR_API int nfr_getResCaseLabelsSize(NfrFile* f);
NFR_API void nfr_getResCaseLabels(NfrFile* f, char* labels);

/* %RESULT.CASE */
NFR_API int nfr_getResNumSteps(NfrFile* f, int caseId);
NFR_API void nfr_getResStepIds(NfrFile* f, int caseId, int* data);
NFR_API int nfr_getResStepLabelsSize(NfrFile* f, int caseId);
NFR_API void nfr_getResStepLabels(NfrFile* f, int caseId, char* labels);


/* %RESULT.CASE.STEP */
/************************************************************************
* @param caseId Identificador do caso
* @param stepId Identificador do passo
* @return Retorna o step time                                                                
************************************************************************/
NFR_API float nfr_getResStepTime(NfrFile* f, int caseId, int stepId);

/************************************************************************
* @param caseId Identificador do caso
* @param stepId Identificador do passo
* @return Retorna o step factor                                                              
************************************************************************/
NFR_API float nfr_getResStepFactor(NfrFile* f, int caseId, int stepId);

/************************************************************************
* nfr_getResNumFields
*   Retorna o número de campos de resultado do tipo dado.
************************************************************************/
NFR_API int nfr_getResNumFields(NfrFile* f, int caseId, int stepId, int resType);

/************************************************************************
* nfr_getResFieldLabelsSize
*   Retorna o tamanho total do vetor dos títulos dos campos de 
* resultado do tipo dado.
************************************************************************/
NFR_API int nfr_getResFieldLabelsSize(NfrFile* f, int caseId, int stepId, int resType);

/************************************************************************
* nfr_getResFieldLabels
*   Retorna o vetor dos títulos dos campos de resultado do tipo dado.
* O vetor é uma seqüência de strings separados pelo caracter '\0':
*   [label1\0label2\0label3\0]
* @param labels Vetor para onde os dados serão lidos.
************************************************************************/
NFR_API void nfr_getResFieldLabels(NfrFile* f, int caseId, int stepId, int resType, char* labels);

/************************************************************************
*	nfr_getResFieldDataSize
*   Retorna o numero total de valores do campo de resultados.
* Por exemplo, se o campo de resultados é do tipo NFR_RES_ELE_NODAL_VECTOR,
* o valor retornado é: num_elements * num_nodes_per_element * 3
************************************************************************/
NFR_API int nfr_getResFieldDataSize(NfrFile* f, int caseId, int stepId, int resType, int fieldId);

/************************************************************************
*	nfr_getResEleNodalNumNodesArray
*   Retorna um vetor onde cada posição contém o tamanho do resultado
* escalar do elemento (número de nós):
*   v[eleId-1] = numNodes
* @param data Vetor para onde os dados serão lidos.
************************************************************************/
NFR_API void nfr_getResEleNodalNumNodesArray(NfrFile* f, int* data);

/************************************************************************
 * nfr_getResEleNodalOffsetArray
 *
 * Retorna o indice dos offsets de resultado EleNodal
 *
 ************************************************************************/
NFR_API void nfr_getResEleNodalOffsetArray(NfrFile* f, int* data);

/************************************************************************
* nfr_getResEleGaussOffsetArray
*
* Retorna o indice dos offsets de resultado EleNodal
*
************************************************************************/
NFR_API void nfr_getResEleGaussOffsetArray(NfrFile* f, int* data);

/************************************************************************
*	nfr_getResEleGaussNumNodesArray
*   Retorna um vetor onde cada posição contém o tamanho do resultado
* escalar de gauss do elemento (número de pontos de gauss):
*   v[eleId-1] = numNodes
* @param data Vetor para onde os dados serão lidos.
************************************************************************/
NFR_API void nfr_getResEleGaussNumNodesArray(NfrFile* f, int* data);

//TODO
//void nfr_getResFieldMinMax(NfrFile* f, float* vmin, float* vmax);

/************************************************************************
*   Funcao que busca os dados referentes a tag: 
*   %RESULT.
*   Ela retorna por referencia no vetor data os valores do arquivo binario
*  referente a uma coluna identificada pelo fieldId.
*
* @param caseId Identificador do caso
* @param stepId Identificador do passo
* @param resType Tipo de resultado.
* @param fieldId Identificador de qual coluna sera' retornada
* @param data Vetor data para qual os dados sao salvos
************************************************************************/
NFR_API void nfr_getResFieldData(NfrFile* f, int caseId, int stepId, int resType, int fieldId, float* data);

/************************************************************************
* Retorna o resultado NODAL.* ou DISPLACEMENT do nó dado.
*
* @param caseId Identificador do caso.
* @param stepId Identificador do passo.
* @param resType Tipo de resultado.
* @param fieldId Identificador de qual coluna sera' retornada.
* @param nodeId Identificador do nó desejado.
* @param data Vetor para o qual os dados são salvos.
************************************************************************/
NFR_API void nfr_getResNodalDataAt(NfrFile* f, int caseId, int stepId, int resType, int fieldId, int nodeId, float* data);

/************************************************************************
* Retorna o resultado ELEMENT.NODAL.* do elemento dado.
*
* @param caseId Identificador do caso.
* @param stepId Identificador do passo.
* @param resType Tipo de resultado.
* @param fieldId Identificador de qual coluna sera' retornada.
* @param eleId Identificador do elemento desejado.
* @param data Vetor para o qual os dados são salvos.
************************************************************************/
NFR_API void nfr_getResEleNodalDataAt(NfrFile* f, int caseId, int stepId, int resType, int fieldId, int eleId, float* data);

/************************************************************************
* Retorna o resultado ELEMENT.GAUSS.* do elemento dado.
*
* @param caseId Identificador do caso.
* @param stepId Identificador do passo.
* @param resType Tipo de resultado.
* @param fieldId Identificador de qual coluna sera' retornada.
* @param eleId Identificador do elemento desejado.
* @param data Vetor para o qual os dados são salvos.
************************************************************************/
NFR_API void nfr_getResEleGaussDataAt(NfrFile* f, int caseId, int stepId, int resType, int fieldId, int eleId, float* data);


/*************************************************************************
 *
 * Uso interno
 *
 *************************************************************************/
/************************************************************************
*   Importa tag %RESULT para o arquivo binário. 
*   - Assume que os ids dos nós e elementos estão no intervalo [1,num_elem].
*   - Assume que as tags %NODE e %ELEMENT já foram importadas.
*   - Os dados um resultado devem existir para todos os nós ou elementos da malha.
*   - Os dados dos resultados são adicionados ordenados pelo id. A  
* ordenação é feita, pois os resultados de cada 'step' podem vir em ordens
* diferentes. A ordenação evita o armazenamento de um vetor paralelo de ids (e de
* posições dos elementos, no caso de ELEMENT.NODAL e ELEMENT.GAUSS).
*   - Apenas os vetores de cada 'field' são importados. Não são adicionados
* vetores de ids.
*
* @return NFR_NO_ERROR ou NFR_ERROR.
* @warning Uso interno.
************************************************************************/
int nfr_resultTag(NfrImport* f, char* head, char* tail, NfrFileOffset* offset);

/************************************************************************
* Salva os dados ainda pendentes de resultados para o arquivo binário e
* libera parte da memória ocupada. Deve ser chamada antes do final
* da importação.
* WARNING Internal use
************************************************************************/
int nfr_resultTagClose(NfrImport* f, NfrFileOffset* offset);

#endif
