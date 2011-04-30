/************************************************************************
 *
 * Class:  material
 * Created: 7/2/2006
 * Author: MALF 
 *----------------------------------------------------------------------
 * Description: 
 *
 ************************************************************************/

#ifndef __NFR_MATERIAL_H__
#define __NFR_MATERIAL_H__

/************************************************************************
* nfr_materialTag
* 
*   Importa tag %MATERIAL para o arquivo binário. 
*   - Assume que os ids dos materiais estão no intervalo [1,num_mat].
*   - As coordenadas dos nós são adicionadas na mesma ordem do arquivo neutro.
*
* @return NFR_NO_ERROR ou NFR_ERROR.
* @warning Uso interno.
************************************************************************/
int nfr_materialTag(NfrImport* f, char * head, char * tail, NfrFileOffset *offset);

/************************************************************************
* nfr_getNumMaterials
*
* @param f FILE
* @return Se OK: número de materiais. Senão: 0.
************************************************************************/
NFR_API int nfr_getNumMaterials(NfrFile* f);

/************************************************************************
* nfr_getNumMaterialLabels
*
*   Retorna o número de labels de materiais, que pode ser diferente do número
* de materiais.
************************************************************************/
NFR_API int nfr_getNumMaterialLabels(NfrFile* f);

/************************************************************************
* nfr_getMaterialLabelsSize
*
* @return Se OK: Numero de total de bytes (incluindo \0). Senão: 0.
************************************************************************/
NFR_API int nfr_getMaterialLabelsSize(NfrFile* f);

/************************************************************************
* nfr_getMaterialLabelIds
*
*   Preenche o vetor de ids com os ids dos materiais.
* @param f FILE
* @param ids Vetor para onde os dados serão lidos, de tamanho igual ao 
* número de labels.
* @return Se OK: 1, senão: 0.
************************************************************************/
NFR_API int nfr_getMaterialLabelIds(NfrFile* f, int* ids);

/************************************************************************
* nfr_getMaterialLabels
*
* @param f FILE
* @param coords Vetor para onde os dados serão lidos, de tamanho igual ao 
* retornado por nfr_getMaterialLabelsSize.
* @return Se OK: 1. Senão: 0.
************************************************************************/
NFR_API int nfr_getMaterialLabels(NfrFile* f, char* labels);

#endif

