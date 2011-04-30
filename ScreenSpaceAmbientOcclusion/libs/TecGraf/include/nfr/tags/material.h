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
*   Importa tag %MATERIAL para o arquivo bin�rio. 
*   - Assume que os ids dos materiais est�o no intervalo [1,num_mat].
*   - As coordenadas dos n�s s�o adicionadas na mesma ordem do arquivo neutro.
*
* @return NFR_NO_ERROR ou NFR_ERROR.
* @warning Uso interno.
************************************************************************/
int nfr_materialTag(NfrImport* f, char * head, char * tail, NfrFileOffset *offset);

/************************************************************************
* nfr_getNumMaterials
*
* @param f FILE
* @return Se OK: n�mero de materiais. Sen�o: 0.
************************************************************************/
NFR_API int nfr_getNumMaterials(NfrFile* f);

/************************************************************************
* nfr_getNumMaterialLabels
*
*   Retorna o n�mero de labels de materiais, que pode ser diferente do n�mero
* de materiais.
************************************************************************/
NFR_API int nfr_getNumMaterialLabels(NfrFile* f);

/************************************************************************
* nfr_getMaterialLabelsSize
*
* @return Se OK: Numero de total de bytes (incluindo \0). Sen�o: 0.
************************************************************************/
NFR_API int nfr_getMaterialLabelsSize(NfrFile* f);

/************************************************************************
* nfr_getMaterialLabelIds
*
*   Preenche o vetor de ids com os ids dos materiais.
* @param f FILE
* @param ids Vetor para onde os dados ser�o lidos, de tamanho igual ao 
* n�mero de labels.
* @return Se OK: 1, sen�o: 0.
************************************************************************/
NFR_API int nfr_getMaterialLabelIds(NfrFile* f, int* ids);

/************************************************************************
* nfr_getMaterialLabels
*
* @param f FILE
* @param coords Vetor para onde os dados ser�o lidos, de tamanho igual ao 
* retornado por nfr_getMaterialLabelsSize.
* @return Se OK: 1. Sen�o: 0.
************************************************************************/
NFR_API int nfr_getMaterialLabels(NfrFile* f, char* labels);

#endif

