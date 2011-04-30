/**
  TopS - Topological data structure for mesh representation.
  
  TopS is a software developed by Tecgraf/PUC-Rio & UIUC.
  It is requested that the TopS users provide the appropriate
  credits and references to the work.
  
  References:
  1 - W. Celes, G.H. Paulino, R. Espinha, "A compact adjacency-based
  topological data structure for finite element mesh representation",
  IJNME, 2005 (in press)
  2 - W. Celes, G.H. Paulino, R. Espinha, "Efficient Handling of Implicit
  Entities in Reduced Mesh Representations", JCISE, Special Issue on
  Mesh-Based Geometric Data Processing, 2005 (in press)
  
  Aug 2005
*/

#ifndef _TOP_GRID_BUILDER_H
#define _TOP_GRID_BUILDER_H

class TopModel;

#include "../defines.h"

class TOPS_API TopGridBuilderOutput
{
public: 
  virtual ~TopGridBuilderOutput() {}

  virtual bool Start() { return true; }
  virtual bool Finish() { return true; }
  virtual void SetElemType(int type) {}
  virtual void SetNNodes(int nnodes) {}
  virtual void SetNElems(int nelems) {}
  virtual int InsertNode(double x = 0.0, double y = 0.0, double z = 0.0) = 0;
  virtual int InsertElem(int type, int* nids) = 0;   
};

class TOPS_API TopGridBuilderModelOutput : public TopGridBuilderOutput
{
public: 
  TopGridBuilderModelOutput(TopModel* model);
  virtual ~TopGridBuilderModelOutput();

  virtual bool Start();
  virtual bool Finish();
  virtual int InsertNode(double x, double y, double z);
  virtual int InsertElem(int type, int* nids);   

  void SetUpdateAdj(bool flag);

private:
  TopModel* m_model;
  bool m_update_adj;
};


class TOPS_API TopGridBuilder
{
private:
  static bool CreateGridT3(TopGridBuilderOutput* model, int nx, int ny, 
    double xmin, double ymin, double xmax, double ymax);
  static bool CreateGridT6(TopGridBuilderOutput* model, int nx, int ny, 
    double xmin, double ymin, double xmax, double ymax);
  static bool CreateGridQ4(TopGridBuilderOutput* model, int nx, int ny,
    double xmin, double ymin, double xmax, double ymax);
  static bool CreateGridQ8(TopGridBuilderOutput* model, int nx, int ny,
    double xmin, double ymin, double xmax, double ymax);
  static bool CreateGridHEX8(TopGridBuilderOutput* model, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);
  static bool CreateGridHEX8Reservoir(TopGridBuilderOutput* model, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);
  static bool CreateGridHEX20(TopGridBuilderOutput* model, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);
  static bool CreateGridTET4(TopGridBuilderOutput* model, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);
  static bool CreateGridTET10(TopGridBuilderOutput* model, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);

  static bool CreateGridDiscT3(TopGridBuilderOutput* model, int nx, int ny, 
    double xmin, double ymin, double xmax, double ymax);
  static bool CreateGridDiscT6(TopGridBuilderOutput* model, int nx, int ny, 
    double xmin, double ymin, double xmax, double ymax);
  static bool CreateGridDiscQ4(TopGridBuilderOutput* model, int nx, int ny,
    double xmin, double ymin, double xmax, double ymax);
  static bool CreateGridDiscQ8(TopGridBuilderOutput* model, int nx, int ny,
    double xmin, double ymin, double xmax, double ymax);
  static bool CreateGridDiscHEX8(TopGridBuilderOutput* model, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);
  static bool CreateGridDiscHEX20(TopGridBuilderOutput* model, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);
  static bool CreateGridDiscTET4(TopGridBuilderOutput* model, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);
  static bool CreateGridDiscTET10(TopGridBuilderOutput* model, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);

public:

  /**
      Creates a square grid of the given element type into the given empty model.
      For Q4, Q8, HEX8, HEX20 and HEX8_RESERVOIR element types, a regular grid
    is created.
      For T3 and T6 element types, a 4-8 mesh is created.
      For TET4 and TET10 element types, a mesh of HEX elements each decomposed 
    into 6 TET elements is created.
      For two-dimensional elements, the z coordinate parameters are ignored.
      If the grid cannot be created, returns false.
  */
  /** deprecated */
  static bool BuildGrid(TopModel* m, int el_type, int nx, int ny, int nz, 
    double xmin, double ymin, double zmin, 
    double xmax, double ymax, double zmax,
    bool update_adj = true);

  static bool BuildGrid(TopGridBuilderOutput* m, int el_type, int nx, int ny, int nz,
    double xmin, double ymin, double zmin, double xmax, double ymax, double zmax,
    bool update_adj = true);

};


#endif
