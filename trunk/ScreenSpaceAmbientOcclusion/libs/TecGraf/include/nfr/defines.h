/* *************************************************************
*
* Class:  defines.h
* Created: 6/4/2006 - 15:12
* Author: malf
* $Id $
*
* Description: 
*   Definições gerais para o nfr.
*
* *************************************************************/


#ifndef __NFR_DEFINES_H__
#define __NFR_DEFINES_H__

#ifdef _WIN32
# ifdef NFR_DLL
#   define NFR_API __declspec(dllexport)
# else
#  define NFR_API
# endif
#else
# define NFR_API
#endif

#define NFR_QUAD_RESEQ_MAX 32
#define NFR_ELE_NUM_NODES_MAX 32

typedef enum {
  NFR_NO_ERROR = 0,
  NFR_ERROR = 1, 
  NFR_CANCELED = 2,
};

typedef enum {
  NFR_ELE_ID=0,
  NFR_ELE_MATERIAL_ID,
  NFR_ELE_THICKNESS_ID,
  NFR_ELE_INTEGRATION_ID,
  NFR_ELE_NODE_ID
} NfrElementFieldId;

typedef enum {
  NFR_ELE_T3=0,
  NFR_ELE_T6,
  NFR_ELE_Q4,
  NFR_ELE_Q8,
  NFR_ELE_Q9,
  NFR_ELE_DKT,
  NFR_ELE_DKQ,
  NFR_ELE_QUAD,
  NFR_ELE_INTERFACE,
  NFR_ELE_INFINITE,
  NFR_ELE_INF3DS,
  NFR_ELE_INT3DS,
  NFR_ELE_TETR4,
  NFR_ELE_TETR10,
  NFR_ELE_WEDGE6,
  NFR_ELE_WEDGE15,
  NFR_ELE_BRICK8,
  NFR_ELE_BRICK20,
  NFR_ELE_BRICKS,
  NFR_ELE_TYPE_MAX, // Maximum element type id.
} NfrElementType;

typedef enum {
  NFR_QUAD_GAUSS_TRIANGLE=0,
  NFR_QUAD_GAUSS_QUADRILATERAL,
  NFR_QUAD_GAUSS_TETRAHEDRON,
  NFR_QUAD_GAUSS_CUBE,
  NFR_QUAD_GAUSS_WEDGE,
  NFR_QUAD_TYPE_MAX, // Maximum quadrature type id.
} NfrQuadratureType;

#endif