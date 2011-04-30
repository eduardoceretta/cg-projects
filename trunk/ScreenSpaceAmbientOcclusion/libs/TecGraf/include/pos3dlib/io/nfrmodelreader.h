/***************************************************************
 ***************************************************************/

#ifndef P3D_NFR_MODEL_READER_H
#define P3D_NFR_MODEL_READER_H

#include <ds/hash.h>
#include <nfr/nfr.h>

#include <pos3dlib/defines.h>
#include "modelreader.h"
#include "nfrresult.h"

/*******************************************************************************
  class P3DNfrModelReader    

  Binary neutral file reader and importer.
  The steps to read a neutral file are as follows:
  1 - If necessary, import the text file (.pos) to binary representation
    (see Import()).
  2 - Set input file (see SetFilename()).
  3 - Open the binary files to start reading data (see Open()).
  4 - Read data into the model (see Read()).
  5 - Keep the file opened while using the model and accessing result data.
  6 - After finishing using the model, close the input file (see Close()).
*******************************************************************************/
class POS3DLIB_API P3DNfrModelReader : public P3DModelReader
{
public:
  /**
      Creates a new binary neutral file reader for the given model.
    Before using this object, a file must be opened.
  */
  P3DNfrModelReader(P3DModel* model);

  virtual ~P3DNfrModelReader();

  /**
      Gets the model associated to this reader.
  */
  P3DModel* GetModel() { return m_model; }

  /**
      Imports text neutral file (.pos) to binary representation.
  */
  int Import(const char* posfile, const char* binfile, const char* luafile);

  /**
      Sets the name of the input binary file used to read model data.
  */
  void SetFilename(const char* binfile, const char* luafile);

  int GetNumOfNodes();

  int GetNumOfElements();

  /**
      Opens input file.
    @warning A file must be opened before calling the access methods. Some
    data, as results, are read  is read on demand, so the file must be open during 
  */
  virtual int Open();

  /**
      Reads the contents of the input file to the model. Some data
  */
  virtual int Read();

  virtual int Close();

  virtual bool IsOpen();

  /**
    Gets the number of result cases in the input file.
  */
  int GetNumOfResultCases();

  int GetResultCaseId(int casei);

  /**
    Gets the number of result steps of a result case.
  */
  int GetNumOfResultSteps(int casei);

  int GetResultStepId(int casei, int stepi);

  char* GetResultStepName(int casei, int stepi);

  /**
    Sets whether a result step will be enabled. If disabled,
    the step will not be read from the neutral file.

    This method can only be invoked before reading the model (see Read()).

    Default: true
  */
  void SetResultStepEnabled(int casei, int stepi, bool flag);

  int ReadResultData(P3DNfrResult* res_data, int* size, float** data);

  int ReadNodalHistory(P3DNfrResult* res_data, int node_id, float* data);

  int ReadElemNodalHistory(P3DNfrResult* res_data, int elem_id, float* data);

  int ReadElemGaussHistory(P3DNfrResult* res_data, int elem_id, float* data);

private:
  struct ResultStep
  {
    int id;
    char* label;
    bool enabled;

    ResultStep ()
    : id(0), label(NULL), enabled(true)
    {
    }
    ~ResultStep ()
    {
      free(label);
    }
    void SetLabel(const char* s)
    {
      free(label); 
      label = NULL;
      if (s)
        label = strdup(s);
    }
  };

  struct ResultCase 
  {
    int id;
    char* label;
    DsArray<ResultStep*> steps;

    ResultCase ()
    : id(0), label(NULL)
    {
    }
    ~ResultCase ()
    {
      free(label);
      for (int i = 0; i < steps.Size(); i++)
        delete steps[i];
    }
    void SetLabel(const char* s)
    {
      free(label); 
      label = NULL;
      if (s)
        label = strdup(s);
    }
  };

private:
  int ReadBasicInfo();
  int GetTopsShapeElemType(int nfr_elem_type, int* node_ids);
  TopConcreteElement* CreateTopsPrototype(int tops_elem_type);
  void InitNfrToTopsIncMap();
  void InitTopsToNfrIncMap();

  int ReadNodes();  
  int ReadMaterials();
  int ReadQuadratures();
  int InsertQuadratures();
  int CreateElemQuadrature(int elem_type, int quad_id);
  int InsertElem(int type, int id, int* inc_map, int nnodes, int* node_ids);
  int GetTopsResequence(int elem_type, int nfr_elem_type, int* nfr_reseq, int* reseq);
  P3DShape* CreateShape(int elem_type);
  int ReadElements();
  int ReadRezone();
  int ReadElemSizeArrays();
  int ReadResultFields(P3DResultCase* res_case, P3DResultStep* res_step, int nfr_type_id);
  int ReadResultDisplacement(P3DResultCase* res_case, P3DResultStep* res_step);
  bool ReadResultFieldNames(int case_id,
                            int step_id,
                            int nfr_type_id,
                            DsArray<char>* o_names);
  int ReadResultTypes(P3DResultCase* res_case, P3DResultStep* res_step);
  int ReadResultSteps(int casei, P3DResultCase* res_case);
  int ReadResultCases();
  int ConvertResultElemNodalData(P3DNfrResult* res_data, int size, float* data);
  int ConvertResultElemNodal(P3DNfrResult* res_data, int elem_id, float* elem_data);

private:
  P3DModel* m_model;
  char m_binfile[512];
  char m_luafile[512];
  NfrFile* m_file;
  bool m_is_open;

  int* m_nfr_elem_type; // NfrElemType for each element. m_nfr_elem_type[id-1] = nfr_type.

  int m_nquadratures; // Number of quadratures.
  P3DGauss** m_quadratures; // Quadratures read from file.

  /**
      Incidence conversion map. For a local node id in Nfr, returns the equivalent
    id in TopS.
  */
  static int m_nfr_tops_inc[NFR_ELE_TYPE_MAX][TopModel::ELEMENT_MAX][NFR_ELE_NUM_NODES_MAX];
  static bool m_nfr_tops_inc_init;

  /**
      Incidence conversion map. For a local node id in TopS, returns the equivalent
    id in Nfr.
  */
  static int m_tops_nfr_inc[NFR_ELE_TYPE_MAX][TopModel::ELEMENT_MAX][TOP_MAX_NUM_N];
  static bool m_tops_nfr_inc_init;

  int m_num_nodes, m_num_elem, m_num_rescases;

  DsArray<ResultCase*> m_resultcases;
};

#endif