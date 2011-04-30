/* *************************************************************
 *
 * Class:   result.h
 * Created: 25/5/2006 - 12:09
 * Author:  malf
 * $Id: result.h,v 1.7 2011/04/20 13:55:11 rodesp Exp $
 *
 * Description: 
 *
 * *************************************************************/

#ifndef P3D_RESULT_H
#define P3D_RESULT_H

#include <ds/array.h>
#include <pos3dlib/defines.h>

class P3DModel;
class P3DResultCase;
class P3DResultStep;
class P3DResultField;
class P3DResult;

/**********************************************************
    In Pos3D, result information associated to a P3DModel object is
  organized according to the given hierarchy:

    Result Case
      +-- Result Step
          +-- Result Type (DISPLACEMENT, NODAL_SCALAR, etc.)
                +-- Result Field ('DX', 'DY', etc.)
                      +-- Result Data (P3DResult).

    Each result case (P3DResultCase) defines a case of simulation 
  an contains a number of result steps (P3DResultStep). Each step has 
  a number of fields of the given type (field and type are encapsulated 
  in P3DResultField), which, in turn, has an associated result data
  container (P3DResult).
***********************************************************/

/***********************************************************
  class P3DResult

    Abstract class used as a container of field result data.
  It is responsible for retrieving the result data which is
  requested by the application from a data base.
    The requested data may be explicitly stored in the main memory or
  may need to be fetched from elsewhere. Thus, in the general
  case, the Load()/Unload() methods must be called before/after accessing 
  the data (except where stated otherwise).
***********************************************************/
// Maximum result dimension, in number of components (1:scalar, 3: vector, etc.)
#define P3D_RESULT_MAX_DIMENSION 16

// Maximum number of gauss results per element
#define P3D_RESULT_MAX_ELEM_GAUSS 27

// Maximum number of result values (number of floats, ints, etc.) 
// for an element. It is guaranteed to be at least the size of 
// MAX(dimension*num_nodes, dimension*num_gauss)
#define P3D_RESULT_MAX_ELEM_DATA_SIZE (P3D_RESULT_MAX_DIMENSION*32)

class POS3DLIB_API P3DResult 
{
public:
  /**
      Defines where the result data are positioned.
  */
  enum Location {
    NONE = 0,
    NODES,  /**< Results for each node of the model (global). */
    ELEMENT_NODES, /**< Results for each node of each element of the model (local). */
    ELEMENT_GAUSS, /**< Results for each gauss point of each element of the model (local). */
    ELEMENT /**< Results for each element of the model (global). */
  };

  enum DataType {
    DATA_TYPE_NONE = 0,
    DATA_TYPE_SCALAR,
    DATA_TYPE_VECTOR,
    DATA_TYPE_TENSOR,
    DATA_TYPE_DISPLACEMENT,
    DATA_TYPE_MATERIAL
  };

  P3DResult (Location location = NODES);

  /**
   * Adds a new reference to this object. The object will not be deleted until
   * all the references to it are released (see Release()).
   *
   * One initial reference is always added during the object's construction.
   */
  void AddRef ();

  /**
   * Releases a reference to this object (see AddRef()).
   * The object is deleted when no remaining references to it exist.
   */
  void Release ();

  void SetField (P3DResultField* res_field)
  {
    m_field = res_field;
  }

  /**
      Gets field and type information of this result.
  */
  P3DResultField* GetField ()
  {
   return m_field;
  }

  void SetStep (P3DResultStep* res_step)
  {
    m_step = res_step;
  }

  /**
      Gets step information of this result.
  */
  P3DResultStep* GetStep ()
  {
    return m_step;
  }

  void SetCase (P3DResultCase* res_case)
  {
    m_case = res_case;
  }

  /**
      Gets case information of this result.
  */
  P3DResultCase* GetCase ()
  {
    return m_case;
  }

  void SetLocation (Location location)
  {
    m_location = location;
  }

  /**
    Gets the position of this result.
  */
  Location GetLocation ()
  {
    return m_location;
  }

  /**
      Gets the limits of the values of this result.

    @param vmin Output array of size equal to the dimension of the data. 
    @param vmax Output array of size equal to the dimension of the data. 

    @see GetDimension
  */
  virtual void GetLimits (float* vmin, float* vmax) = 0;

  /** 
      Gets the absolute size (in number of floats) of the data array. 
  */
  virtual int GetSize () = 0;

  /** 
      Gets the data array. 
  */ 
  virtual float* GetData () = 0;

  /** 
     Gets data at the given id. 
     If result position is NODES then id = node_id, else id = elem_id.

     This method does not requires that result is loaded.
  */
  void GetDataAt (int id, float* data);

  /** 
      Gets the data dimension: 1: scalar, 3: vector. 
  */ 
  virtual int GetDimension () = 0;

  /** 
      Load result data to the main memory. Must be called once per
    user before using the data.
  */
  virtual int  Load () = 0;

  /**
      Unloads result data. Must be called after using the data in order
    to release the resources allocated by Load() method.
  */
  virtual int  Unload () = 0;

  /**
    Gets the result at the given id for all the time steps of this result case.
    Parameters:
      id For nodal results, it's a node id. Otherwise it is an element id.
      data Array where the history data will be returned. It must be of size:
        - For NODES: dimension*num_steps
        - For ELEMENT_NODES: dimension*elem_size*num_steps. The result is 
          returned at each element node.
        - For ELEMENT_GAUSS: UNDEFINED!!!
        - For ELEMENT: dimension*num_steps
      num_steps is obtained from P3DResultCase::GetNumSteps()
      elem_size is from P3DModel::GetElemNodalSizeArray()
  */
  virtual void GetHistory (int id, float* data) = 0;

  /** 
    Same as GetHistory, but is convenient for tolua. 
  */
  void LuaGetHistory (int id, int size, float* data) 
  { 
    GetHistory(id, data); 
  }

  virtual void Print () 
  {
  }

protected:
  virtual ~P3DResult ();

private:
  P3DResultField* m_field;
  P3DResultStep* m_step;
  P3DResultCase* m_case;
  Location m_location;
  int m_refcount;
};

#endif