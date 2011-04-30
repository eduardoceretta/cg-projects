/* *************************************************************
*
* Class:   geodesic.h
* Created: 16/04/2007 - 18:15
* Author:  malf
*
* Description: 
*
* *************************************************************/

#ifndef P3D_GEODESIC_H
#define P3D_GEODESIC_H


#include <ds/array.h>
#include <alg/vector.h>
#include <topsview/geometry/triangulator.h>
#include <topsview/geometry/lineset.h>
#include <pos3dlib/defines.h>
#include "model/model.h"
#include <float.h>
#include "polyline.h"


class POS3DLIB_API P3DGeodesicPolyline
{
private:
  struct GeodesicData
  {
    TopNode node;
    float dist;
    float scalar;
    GeodesicData() {}
    GeodesicData(float dist, float s) 
      : node(TOP_INVALID_HANDLE), dist(dist), scalar(s) 
    {}
    GeodesicData(TopNode node, float dist, float s)
      : node(node), dist(dist), scalar(s) 
    {}
  };

  P3DPolyline m_points;
  DsArray<GeodesicData> m_data;

public:
  P3DGeodesicPolyline() 
  {
  }

  P3DGeodesicPolyline (const P3DGeodesicPolyline& o)
  : m_points(o.m_points)
  {
    for (int i = 0; i < o.m_data.Size(); i++)
      m_data.Append(o.m_data.GetRef(i));
  }

  ~P3DGeodesicPolyline() 
  {
  }

  P3DGeodesicPolyline* Copy()
  {
    return new P3DGeodesicPolyline(*this);
  }

  int GetNumOfPoints() const 
  { 
    return m_points.GetNumPoints(); 
  }

  void Clear() 
  { 
    m_points.Clear(); 
    m_data.Empty();
  }

//TODO rename method
  void Add(AlgVector p, float dist, float s)
  { 
    m_points.Append(p); 
    m_data.Append(GeodesicData(dist, s)); 
  }
//TODO rename method
  void Add(TopNode node, AlgVector p, float dist, float s)
  { 
    m_points.Append(p);
    m_data.Append(GeodesicData(node, dist, s)); 
  }

  void Set(int i, AlgVector p, float dist, float s)
  {
    m_points.Set(i, p);
    m_data.Set(i, GeodesicData(dist, s)); 
  }

  void Set(int i, TopNode node, AlgVector p, float dist, float s)
  {
    m_points.Set(i, p);
    m_data.Set(i, GeodesicData(node, dist, s)); 
  }

  const P3DPolyline* GetPolyline () 
  {
    return &m_points;
  }

  TopNode GetNode(int i)
  {
    return m_data.GetRef(i).node;
  }

  AlgVector GetPosition(int i)
  {
    return m_points.Get(i);
  }

  float GetDistance(int i)
  {
    return m_data.GetRef(i).dist;
  }

  float GetScalar(int i)
  {
    return m_data.GetRef(i).scalar;
  }

  void Remove(int i)
  {
    m_points.Remove(i);
    m_data.Remove(i);
  }

  void Invert()
  {
    m_points.Invert();
    m_data.Invert();
  }

  //TODO: remover daqui.
  void Draw();
};


/**
    Algorithm for computing the shortest path between vertices 
  along the edges on the model's boundary.

  Usage:
    P3DGeodesic g(model, result);
    // Input parameters.
    for (int i = 0; i < ngeom; i++)
      g.AddLineSet(geom[i]);
    // Execute algoritm.
    g.Compute(nvertices, vertices);
    // Get a reference to the computed result.
    P3DGeodesicPolyline* p = g.GetPolyline();

  Assumptions:
    - Element ids are compact and in the interval [1, nelem].
    - Node ids are compact and in the interval [1, nnodes].
    - Result field is scalar.

  TODO: 
    - Support for REZONE.
*/
class POS3DLIB_API P3DGeodesic
{
public:
  class HeapCompareFunc;

  /**
      Gets the vertex-use nearest to the point p on the given face.
  */
  static TopVertexUse GetNearestFaceVertex(TopModel* m, TopFacetUse fu, AlgVector p)
  {
    return GetNearestVertexAux<TopFacetUse>(m, fu, p);
  }

  /**
      Gets the vertex-use nearest to the point p on the given face.
  */
  static TopVertexUse GetNearestFace2DVertex(TopModel* m, TopElement fu, AlgVector p)
  {
    return GetNearestVertexAux<TopElement>(m, fu, p);
  }

  static TopVertexUse GetVuTmp(int elid, int lid) 
  {
    return TopVertexUse((unsigned) elid, lid);
  }

  /**
      Creates a new instance of the algorithm to computes geodesics.
    See class documentation for usage.
    Parameters:
      model P3DModel.
      result Smooth scalar result field data: NODES or ELEMENT_NODES.
  */
  P3DGeodesic(P3DModel* model, P3DResult* res);

  /**
      Destroys this instance of the algorithm.
  */
  ~P3DGeodesic();

  /**
      Input parameter.
      Adds a lineset containing boundary edges, which are the ones allowed 
    to be visited during the geodesic computation.
  */
  void AddLineSet(TpvLineSet* g) 
  { 
    m_linesets.Append(g); 
  } 

  /**
      Computes the shortest path between the first and the last given vertices,
    passing through the intermediate vertices.
      For each segment between two consecutive vertices, the shortest path
    constrained to selected edges on the mesh's boundary is computed and 
    append to the previous one.
  */
  bool Compute(int num_vertices, TopVertexUse* vertices);

  /**
      Output.
      Gets the computed geodesic.
  */
  P3DGeodesicPolyline* GetPolyline() { return &m_polyline; }

  /**
      Output.
      Gets the polyline of the input vertices with computed 
    distances and scalars at those points.
  */
  P3DGeodesicPolyline* GetPolylineVertices() { return &m_polyline_vertices; }

private:
  struct VertexData
  {
    bool    valid;
    float   g;
    float   h;
    bool    visited;
    TopNode prev;
    VertexData() : valid(false), g(FLT_MAX), h(FLT_MAX) {}
    VertexData(float g, float h) : valid(true), g(g), h(h), visited(false), prev(TopNode()) {}
  };

  class EdgeBitArray
  {
  private:
    TopModel* m_model;
    int m_nelem;
    int* m_v;

  public:
    EdgeBitArray(TopModel* model)
    {
      m_model = model;
      m_nelem = model->GetNElem();
      assert(8*sizeof(int) >= TOP_MAX_NUM_E);
      m_v = new int[m_nelem];
      for (int i = 0; i < m_nelem; i++)
        m_v[i] = 0;
    }
    ~EdgeBitArray()
    {
      delete [] m_v;
    }
    void Set(const TopEdgeUse& eu)
    {
      int elid = m_model->GetId(m_model->GetElem(eu));
      assert(elid >= 1 && elid <= m_nelem);
      m_v[elid - 1] |= (1 << m_model->GetLocalId(eu));
    }
    int Get(const TopEdgeUse& eu)
    {
      int elid = m_model->GetId(m_model->GetElem(eu));
      assert(elid >= 1 && elid <= m_nelem);
      return m_v[elid - 1] & (1 << m_model->GetLocalId(eu));
    }
  };

  struct HeapEntry
  {
    TopNode     node;
    VertexData* data;
    float       g;
    HeapEntry() {}
    HeapEntry(TopNode node, VertexData* data, float g) : node(node), data(data), g(g) {}
  };

  P3DModel* m_model;
  TopModel* m_mesh;
  P3DResult* m_result;
  DsArray<TpvLineSet*> m_linesets;
  P3DGeodesicPolyline m_polyline;
  P3DGeodesicPolyline m_polyline_vertices;

  template <class T>
  static TopVertexUse GetNearestVertexAux(TopModel* m, T fu, AlgVector p)
  {
    int min_i = 0;
    float min_dist = FLT_MAX;
    for (int i = 0; i < m->GetNVertices(fu); i++)
    {
      float x, y, z;
      m->GetPosition(m->GetVertexNode(fu, i), &x, &y, &z);
      float dist = sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y) + (z - p.z)*(z - p.z));
      if (dist < min_dist) 
      {
        min_i = i;
        min_dist = dist;
      }
    }
    return m->GetVertexUse(fu, min_i);
  }

  float GetNodeScalarField(int node_id, bool is_cohesive);

  void InitValidEdges(EdgeBitArray* valid_edges);

  void InitVertexData(VertexData* vertex_data,
    TopNode source_node, TopNode target_node);

  P3DGeodesicPolyline* CreateSegmentPolyline(
    TopModel* mesh, bool is_cohesive, VertexData* vertex_data, TopNode n);

  P3DGeodesicPolyline* ComputeSegment(EdgeBitArray& valid_edges, bool is_cohesive,
    TopVertexUse vus, TopVertexUse vut);
};

#endif
