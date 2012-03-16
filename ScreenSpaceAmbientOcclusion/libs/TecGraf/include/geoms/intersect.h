/**
 * General intersection tests.
 * author: Cesar Palomo <cpalomo@tecgraf.puc-rio.br>
 * date:   1-Dec-2009
 */
#ifndef GEOMS_INTERSECT_H
#define GEOMS_INTERSECT_H

#define SUPPORT_RADIAL_MODEL 0
#define COLLECT_CUT_STATISTICS 0
#define TEST_CUT_PERFORMANCE 0
#if TEST_CUT_PERFORMANCE
// salva uma propriedade onde 0 significa que o elemento nao foi testado
// e 1 que o elemento foi testado.
// atencao - soh funciona com modelos nao refinados
# define DEBUG_TESTED_ELEMS 0
#endif

#define GEOMS_EPSILON_ZERO 1.0e-20
#define GEOMS_TOL_PLANE_DIST 1.0e-7
#define GEOMS_TOL_MIN_MAX_DIST 2.0e-4
#define GEOMS_T_INSIDE_POLY -1.f
#define GEOMS_PLANE_NO_INTERS -1

#include "defines.h"
#include "hierarchy.h"
#include "plane.h"
#include "point2d.h"
#include "polygon2d.h"
#include "ray.h"
#include "segment.h"
#include "sphere.h"
#include "triangle.h"
#include <alg/vector.h>
#include <ds/array.h>
#if DEBUG_TESTED_ELEMS
# include <ds/hash.h>
#endif
#include <ds/queue.h>
#include <utl/debug.h>
#include <utl/sort.h>
#include <vrbv/boxfactory.h>
#include <vrbv/geominfocallback.h>
#include <vrbv/scenedata.h>
#include <vrbv/treebuilder.h>

#include <math.h>

// forward declarations
class IOStream;

namespace geoms {

template <class POLYMODELCLASS> 
class GEOMS_API Intersect
{
public:
  /**
   * Structure to store a vertice resulting from a cut.
   */
  typedef struct {
    //bool from_edge;
    //union {
      //int edge_id;
      //int face_id;
    //};
    AlgVector coords;
    //float params[2];  // one used for edge, two for face
  } CutVertex;

  /**
   * Structure to store a polygon resulting from a cut
   */
  typedef struct {
    CutVertex* vertices;
    int vert_count;
    int poly_index;                     // SoH esse eh usado hoje
  } CutFace;

  /**
   * Structure to store parametric coordinates resulting from a cut
   * with a line segment.
   */
  struct CutSegmentInterval
  {
    float t_min;      // (0..1), or GEOMS_T_INSIDE_POLY
    float t_max;      // (0..1), or GEOMS_T_INSIDE_POLY
    int poly_id;
  };

  struct CutResult
  {
    // results for cut with line segment
    int intervals_count;              /**< Number of elements cut by line segment.         */
    CutSegmentInterval** intervals;   /**< Vector with intervals in (0,1) for cut results. */
    // results for cut with plane
    int faces_count;                  /**< Number of faces generated by cut.   */
    CutFace* faces;                   /**< Vector with faces generated by cut. */
#if COLLECT_CUT_STATISTICS
    // TODO remove these members after performance tests
    int elem_tested_count;              /**< # of elements tested against cut plane        */
    int elem_not_tested_count;          /**< # of elements NOT tested against cut plane    */
    int hierarchy_nodes_tested_count;   /**< # of hierarchy nodes tested against cut plane */
#endif
  };

public:
  /**
   * Default constructor.
   * @param poly_model object containing model information 
   *        (vertices/edges/faces etc).
   * NOTE: POLYMODELCLASS must have a copy constructor.
   */
  Intersect (const POLYMODELCLASS& poly_model);
  /**
   * Destructor.
   */
  virtual ~Intersect ();
  /**
   * Serialize to stream.
   */
  bool Serialize (IOStream* s);
  /**
   * Deserialize from stream.
   */
  bool Deserialize (IOStream* s);
  static float GetSerializationVersion ();
  /**
   * Initialize inner structures based on poly_model
   * provided in the constructor.
   */
  void Setup ();
  /**
   * Cut poly_model with provided plane, and store resulting faces in result.
   */
  void ComputeCut (DsArray<CutFace>& result, const vrbv::Plane& plane);
  /**
   * Set scale and scale center.
   */
  void SetScale (AlgVector& scale, AlgVector& center);
  /**
   * Return polymodel used for computations.
   */
  POLYMODELCLASS* GetPolyModel ()
  {
    return &m_poly_model;
  }
  // TODO remove this method
  Hierarchy* GetHierarchy ()
  {
    return m_hierarchy;
  }

  // Intersection tests
  /**
   * Ray/convex polyhedra intersection test.
   * @return whether ray intersects the polyhedra.
   */
  static bool Check (const geoms::Ray& r,
                     const DsArray<geoms::Plane>& plane_face,
                     const DsArray<DsArray<float>*>& coords_face);

  /**
   * Ray/convex polyhedra intersection computation.
   * @return whether ray intersects the polyhedra, 
   * and inward(t1) and outward(t2) intersection parameters.
   */
  static bool Compute (const geoms::Ray& r,
                       const DsArray<geoms::Plane>& plane_face,
                       const DsArray<DsArray<float>*>& coords_face,
                       float* t1,
                       float* t2);

  /**
   * 2D point/2D polygon intersection test.
   * @return whether point is inside polygon.
   */
  static bool Check (const geoms::Point2D& p,
                     const geoms::Polygon2D& polygon);

  /**
   * Ray/box intersection test.
   * @return whether intersection exists.
   */
  static bool Check (const geoms::Ray& r,
                     const vrbv::Box& bb);

  /**
   * Ray/plane intersection test.
   * @return whether intersection exists.
   */
  static bool Check (const geoms::Ray& r,
                     const geoms::Plane& plane);

  /**
   * Ray/plane intersection computation.
   * @param t_result intersection parameter t at ray
   * @param ray_enters_plane true when dot(plane's normal, r) < 0.
   * @return whether intersection exists.
   */
  static bool Compute (const geoms::Ray& r,
                       const geoms::Plane& plane,
                       float* t_result,
                       bool* ray_enters_plane);

  /**
   * Segment/polymodel intersection test.
   * @return whether intersection exists.
   */
  bool Check (const geoms::Segment& s);

  /**
   * Segment/polymodel intersection computation.
   * Results can be accessed through GetResult()
   * If 'clean_prev_results' is true, cleans the previous Compute result.
   * Else, maintains the previous result and includes this result.
   * @return whether intersection exists.
   */
  bool Compute (const geoms::Segment& s, bool clean_prev_results = true);

  /**
   * Project a 2D point and a polygon to 2D plane (xy, xz or yz) that
   * maximizes the projected area.
   * @param point_2D projected point in 2D (return value)
   * @param polygon_2D projected polygon in 2D (return value)
   * @param point_3D point in 3D to be projected
   * @param plane_normal polygon's plane normal
   * @param coords polygon's vertices in 3D
   */
  static void ProjectTo2D (Point2D& point_2D, 
                           Polygon2D& polygon_2D, 
                           const AlgVector& point_3D,
                           const AlgVector& plane_normal, 
                           const DsArray<float>& coords);

  /**
   * Project points_count points in 3D, with coordinates coords,
   * to plane.
   */
  static void Project3DPointsToPlane (DsArray<float>* coords,
                                      int points_count,
                                      const geoms::Plane& plane);


  // TODO remove legacy code below
  bool checkIntersection(float *planeLimits, 
                         float* planeEquation,
                         float direction[3],
                         bool isInfinitePlane);
  bool CheckIntersection (int edge_count,
                          const float* edges,
                          int face_count,
                          const float* faces,
                          const int* faces_v_count,
                          float *planeLimits,
                          float* planeEquation,
                          float direction[3],
                          bool is_infinite_plane);
  bool CheckIntersection (int poly_index,
                          float *planeLimits,
                          float* planeEquation,
                          float direction[3],
                          bool is_infinite_plane);
#if DEBUG_TESTED_ELEMS
  /**
   * Specifies a hash table that will be filled with
   * the ids of the elements that were tested for intersection
   * via checkIntersection.
   * Useful for performance debugging.
   */
  void SetDebugTestedElements (DsHash<int, bool>* tested_elems)
  {
    m_tested_elems = tested_elems;
  }
#endif
  CutResult& GetResult ()
  {
    return m_last_result;
  }
  void FreeResult ()
  {
    free(m_last_result.faces);
    m_last_result.faces = NULL;
    m_last_result.faces_count = 0;
    for (int i = 0; i < m_last_result.intervals_count; i++)
      delete m_last_result.intervals[i];
    free(m_last_result.intervals);
    m_last_result.intervals = NULL;
    m_last_result.intervals_count = 0;
#if COLLECT_CUT_STATISTICS
    m_last_result.elem_tested_count = 0;
    m_last_result.elem_not_tested_count = 0;
    m_last_result.hierarchy_nodes_tested_count = 0;
#endif
  }
private:
  /**
   *  Test whether segment's extremities are in the
   * same half-space (half-spaces determined by normal direction).
   */
  static bool AreExtremitiesInSameHalfSpace (const geoms::Ray& r,
                                             const geoms::Plane& plane);
  /**
   * From Real-time rendering p. 582: when 
   * solving plane equation for t, if |n_p*d| < epsilon, ray 
   * is considered parallel.
   */
  static bool IsParallel (const geoms::Ray& r,
                          const geoms::Plane& plane,
                          float *n_dot_d);
  /**
   *  Test whether t lays inside ray's limits.
   */
  static bool IsInsideRayLimits (const geoms::Ray& r,
                                 const geoms::Plane& plane,
                                 float n_dot_d,
                                 float* t);
  /**
   * Compare function used to sort SegmentInterval's in ascending order.
   */
  static int SegmentIntervalCompare (CutSegmentInterval* s_i_1,
                                     CutSegmentInterval* s_i_2);
  /**
   * Create planes for all faces of a poly_index.
   * Arrays plane_face and coords_face contain the planes
   * and its nodes' coordinates, respectively.
   */
  void CreatePlanes (int poly_index,
                     DsArray<geoms::Plane>& plane_face,
                     DsArray<DsArray<float>*>& coords_face);
  void ComputeLeafIntersection (vrbv::Node* node,
                                float* plane_limits, 
                                float* plane_equation,
                                float direction[3],
                                bool is_infinite_plane); 
  void ComputeIntersection (vrbv::Node* node,
                            float* plane_limits, 
                            float* plane_equation,
                            float direction[3],
                            bool is_infinite_plane);
  bool HasIntersection (vrbv::Node* node,
                        float* plane_limits, 
                        float* plane_equation,
                        float direction[3],
                        bool is_infinite_plane);
  // Intersection tests

  /**
   * Ray/poly intersection test.
   * @return whether intersection exists.
   */
  bool Check (const geoms::Ray& r,
              int poly_index);

  /**
   * Ray/poly intersection computation.
   * @param r ray
   * @param poly_index poly index
   * @param t_1 inward interpolation param 1 (in range 0..r.limit)
   * @param t_2 outward interpolation param 2 (in range 0..r.limit)
   * NOTE *t_1 <= *t_2
   * @return whether intersection exists.
   */
  bool Compute (const geoms::Ray& r,
                int poly_index,
                float* t_1,
                float* t_2);
 
  int PolyAgainstPlane (int poly_index, 
                        float* planeLimits, 
                        float* planeEquation, 
                        float direction[3],
                        float* totalIntersectionPoints,
                        bool is_infinite_plane);
  int PolyAgainstPlane (int face_count,
                        const float* faces,
                        const int* faces_v_count,
                        int edge_count,
                        const float* edges, 
                        float* planeLimits, 
                        float* planeEquation, 
                        float direction[3],
                        float* totalIntersectionPoints,
                        bool is_infinite_plane);
#if SUPPORT_RADIAL_MODEL
  int getRadialElementIntersection(TopElement el, 
                                   float* planeLimits, 
                                   float* planeEquation, 
                                   float** totalIntersectionPoints);
  int getRadialIntersections(TopElement el, 
                             const TopFacetUse& fu, 
                             float* planeLimits, 
                             float* planeEquation, 
                             float** point);
#endif
  bool HasIntersectionWithFace (const float v0[3], 
                                const float v1[3], 
                                const float v2[3], 
                                const float v3[3], 
                                float* plane_limits, 
                                float direction[3],
                                float* point, 
                                bool is_infinite_plane);
  bool HasIntersection (const float* face, 
                        int face_v_count,
                        float *planeLimits, 
                        float direction[3], 
                        float* point, 
                        bool is_infinite_plane);
  bool HasIntersectionWithEdge (const float v0[3], 
                                const float v1[3],
                                float* plane_limits, 
                                float* plane_equation, 
                                float direction[3],
                                float* point,
                                bool is_infinite_plane);
  // TODO rename for sth more instructive
  bool HasIntersection (int poly_index, 
                        float *planeLimits, 
                        float* planeEquation,
                        float direction[3],
                        bool is_infinite_plane);
  bool HasIntersection (int face_count,
                        const float* faces,
                        const int* faces_v_count,
                        int edge_count,
                        const float* edges,
                        float *planeLimits, 
                        float* planeEquation, 
                        float direction[3], 
                        bool is_infinite_plane);
  // Callback used for hierarchy construction
  static int GetBlockVerticesCB (float** coords, int block_i, void* data);

private:
  struct GeomInfoCBData
  {
    POLYMODELCLASS* poly_model;
    DsArray<int>** block_polys; 
  };
private:
  POLYMODELCLASS m_poly_model;
  Hierarchy* m_hierarchy;
  DsArray<CutFace> m_faces_ds_array; /**< points created by cut with plane */
#if DEBUG_TESTED_ELEMS
  DsHash<int, bool>* m_tested_elems; /**< ids of tested elements */
#endif
public:
  CutResult m_last_result;
};

/**
 * Creates a plane fitting 3D vertices points.
 * @param plane returned fitting plane
 * @param v_count number of 3D vertices
 * @param coords array with 3D vertices coords
 */
void CreateFittingPlane (geoms::Plane& plane, 
                         int v_count,
                         const float* coords);

/**
 * Tests whether a point is inside all half spaces determined by a
 * group of planes.
 */
bool IsInsideAllHalfSpaces (const AlgVector& point,
                            const DsArray<geoms::Plane>& plane_face);

/**
 * Dot product between 3D points v1 and v2.
 */
float Dot (float* v1, float* v2);

/**
 * Cross product between 3D points v1 and v2 returned in point v.
 */
void Cross (float *v1, float *v2, float *v);

/**
 * Return vector normal, and also normalizes vector.
 */
float Normalize (float* vector);

/**
 * Return in projectedPoint the projection of a point into a plane
 * given by planeEquation (in Hessian form).
 */
void ProjectPointToPlane (float* point, 
                          float* planeEquation, 
                          float* projectedPoint);

/**
 * Calculate intersection between line and plane,
 * returning result in intersectionPoint.
 */
bool LinePlaneIntersect (float* origin, 
                         float* direction, 
                         float* planeEquation, 
                         float* intersectionPoint);

/**
 * Triangularize a number of points after projecting them on a plane
 * given by planeEquation.
 */
int Triangularize (int npoints, float* points, float* planeEquation);

/**
 * Test whether a 3D point in inside a polygon determined by a set of points.
 */
bool IsInsidePolygon (float* point, int npoints, float* points);

#include "intersect.inl"

} // namespace geoms

#endif // GEOMS_INTERSECT_H