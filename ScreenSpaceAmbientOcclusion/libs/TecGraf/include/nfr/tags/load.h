/************************************************************************
 ************************************************************************/

#ifndef __NFR_LOAD_H__
#define __NFR_LOAD_H__

typedef enum {
  NFR_LOAD_NODAL_FORCE = 0,
  NFR_LOAD_NODAL_DISPLACEMENT,
  NFR_LOAD_NODAL_TEMPERATURE,
  NFR_LOAD_NODAL_PRESCRIBED_TEMPERATURE,
  NFR_LOAD_NODAL_INITIAL_TEMPERATURE, 
  NFR_LOAD_LINE_FORCE_UNIFORM,
  NFR_LOAD_LINE_FORCE_VARIABLE, 
  NFR_LOAD_LINE_MOMENT_UNIFORM,
  NFR_LOAD_LINE_MOMENT_VARIABLE, 
  NFR_LOAD_DOMAIN_FORCE_UNIFORM, 
  NFR_LOAD_DOMAIN_INITIAL_STRESS_UNIFORM,
  NFR_LOAD_AREA_PRESSURE_UNIFORM,
  NFR_LOAD_AREA_PRESSURE_VARIABLE, 
  NFR_LOAD_BEAM_FORCE,
  NFR_LOAD_BEAM_UNIFORM,
  NFR_LOAD_BEAM_LINEAR,
  NFR_LOAD_LINE_CONVECTION_UNIFORM,
  NFR_LOAD_LINE_HEAT_FLUX_UNIFORM,
  NFR_LOAD_LINE_HEAT_FLUX_VARIABLE, 
  NFR_LOAD_AREA_TEMPERATURE,
  NFR_LOAD_DOMAIN_TEMPERATURE_UNIFORM,
  NFR_LOAD_AREA_HEAT_FLUX_UNIFORM,
  NFR_LOAD_AREA_HEAT_FLUX_VARIABLE, 
  NFR_LOAD_GRAVITY,

  NFR_LOAD_TYPE_MAX
} NfrLoadType;

/*************************************************************************
* %LOAD.CASE.NODAL.FORCE 
* [#_of_nodal_forces] 
* [node_id]    <Fx>    <Fy>   <Fz>   <Mx>   <My>   <Mz> 
**************************************************************************/
typedef struct {
  int nodeid;
  float fx, fy, fz;
  float mx, my, mz;
} NfrLoadNodalForce;


//TODO autalizar comentario!
/************************************************************************
* nfr_resultTag
* 
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
int nfr_loadTag(NfrImport* f, char* head, char* tail, NfrFileOffset* offset);

NFR_API int nfr_getLoadNumCases(NfrFile* f);
NFR_API void nfr_getLoadCaseIds(NfrFile* f, int* data);
NFR_API int nfr_getLoadCaseLabelsSize(NfrFile* f);
NFR_API void nfr_getLoadCaseLabels(NfrFile* f, char* labels);

NFR_API int nfr_getNumLoadsOfType(NfrFile* f, int caseId, int loadType);

/************************************************************************
* Retorna OK: != 0 ou 0
*************************************************************************/
NFR_API int nfr_getLoads(NfrFile* f, int caseId, int loadType, void* data);

#endif

