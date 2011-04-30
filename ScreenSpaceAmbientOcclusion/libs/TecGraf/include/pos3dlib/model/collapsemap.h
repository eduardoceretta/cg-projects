#ifndef P3D_SHAPE_COLLAPSE_MAP_H
#define P3D_SHAPE_COLLAPSE_MAP_H

#include <ds/hash.h>
#include <pos3dlib/defines.h>

class POS3DLIB_API P3DShapeCollapseMap
{
private:
  struct CollapseMapKey 
  {
    int* collapses;
    operator int () const 
    {
      int hash_code = 1;
      for (int i = 1; i <= 2*collapses[0]; i++)
        hash_code = 31*hash_code + collapses[i];
      return abs(hash_code);
    }
    bool operator == (const CollapseMapKey& k) const 
    {
      for (int i = 0; i <= 2*collapses[0]; i++)
        if (k.collapses[i] != collapses[i])
          return false;
      return true;
    }
    bool IsValid() { return collapses != NULL; }
    CollapseMapKey(int* collapses = NULL) : collapses(collapses) {}  
  };

  int m_ncollapses;
  int m_max_size;
  int* m_collapses;
  int* m_collapse_types;
  int* m_shape_to_topology;
  int* m_topology_to_shape;
  DsHash<CollapseMapKey, int> m_map;

public:
  /**
    Constructs an invalid collapsed map.
  */
  P3DShapeCollapseMap()
    : m_ncollapses(0), m_max_size(0), 
    m_collapses(NULL), m_collapse_types(NULL), 
    m_shape_to_topology(NULL), m_topology_to_shape(NULL)
  {}

  ~P3DShapeCollapseMap() {}

  /**
    Initializes collapse map (must be invoked at most once after the 
    object instantiation).

    @param ncollapses Number of collapses.
    @param max_collapse_size Maximum size of each entry of the collapsed 
    element table.
    @param collapses Collapsed element table 
      int collapses[ncollapses][max_collapse_size] = {
        {num_collapses, c_0, c_to_0, c_1, c_to_1, ...},
        ...
      };
    @param shape_to_topology Shape to collapsed topology nodal incidence map.
    @param topology_to_shape Collapsed topology to shape nodal incidence map.
  */
  void Init(int ncollapses, int max_collapse_size, 
    int* collapses, int* collapse_types, int* shape_to_topology, int* topology_to_shape)
  {
    m_ncollapses = ncollapses;
    m_max_size = max_collapse_size;
    m_collapses = collapses;
    m_collapse_types = collapse_types;
    m_shape_to_topology = shape_to_topology;
    m_topology_to_shape = topology_to_shape;
    for (int i = 0; i < ncollapses; i++)
      m_map.Set(CollapseMapKey(&collapses[i*max_collapse_size]), i + 1);
  }

  bool IsValid() const
  {
    return m_ncollapses > 0;
  }

  int FindCollapseId(int* collapses)
  {
    return m_map.Get(CollapseMapKey(collapses)) - 1;
  }

  int GetTopologyType(int collapse_id)
  {
    return m_collapse_types[collapse_id];
  }

  /**
    Gets the map which transforms the local node ids of the shape into the local 
    node ids of the topology.
  */
  int* GetShapeToTopologyIncMap(int collapse_id)
  {
    return &m_shape_to_topology[collapse_id*m_max_size];
  }

  /**
    Gets the map which transforms the local node ids of the topology into the local 
    node ids of the shape.
  */
  int* GetTopologyToShapeIncMap(int collapse_id)
  {
    return &m_topology_to_shape[collapse_id*m_max_size];
  }
};

#endif