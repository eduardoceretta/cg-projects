/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  PLY Model Loader. 
 *  Imports a text mesh file defined by the Stanford University.
 *  Based on fabraham@tecgraf.puc-rio.br em Oct 2004 implementation of SGPLYLoader
 */

#include <ply.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "MeshLoaders/PlyMeshFile.h"
#include "defines.h"

PlyMeshFile::PlyMeshFile(void):MeshFileBase()
{
  m_fileType = ".ply";
}

PlyMeshFile::~PlyMeshFile(void)
{
}

void PlyMeshFile::readFile( string fileName, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  m_vbo = NULL;
  m_pos = pos;
  m_scale = scale;
  m_fileName = fileName;
  calcTriangles();
}


void PlyMeshFile::calcVBO()
{
  m_vbo = new GLVertexBufferObject();
  m_vbo->setVBOBuffer( GL_VERTEX_ARRAY, GL_FLOAT, m_numVertices, m_vertices);
  m_vbo->setVBOBuffer( GL_NORMAL_ARRAY, GL_FLOAT, m_numVertices, m_normals);
  m_vbo->setVBOIndexBuffer(GL_UNSIGNED_INT, m_numTriangles*3, m_indexes);
  m_vbo->calcVBO();
  if(m_writeBinaryFile)
    writeBinaryFile(m_fileName);
}

typedef struct Vertex {
  float x,y,z;
  float nx,ny,nz;
  unsigned char red,green,blue;
  float s0,t0,r0;
  float s1,t1,r1;
  float s2,t2,r2;
  float s3,t3,r3;
  float s4,t4,r4;
  float s5,t5,r5;
  float s6,t6,r6;
  float s7,t7,r7;
} Vertex;

typedef struct Face {
  unsigned char nverts;
  unsigned int *verts;
} Face;


static PlyProperty s_vert_props[] = {
  {"x", Float32, Float32, offsetof(Vertex,x), 0, 0, 0, 0},
  {"y", Float32, Float32, offsetof(Vertex,y), 0, 0, 0, 0},
  {"z", Float32, Float32, offsetof(Vertex,z), 0, 0, 0, 0},
  {"nx", Float32, Float32, offsetof(Vertex,nx), 0, 0, 0, 0},
  {"ny", Float32, Float32, offsetof(Vertex,ny), 0, 0, 0, 0},
  {"nz", Float32, Float32, offsetof(Vertex,nz), 0, 0, 0, 0},
  {"red",   Uint8,   Uint8,   offsetof(Vertex,red),   0, 0, 0, 0},
  {"green", Uint8,   Uint8,   offsetof(Vertex,green), 0, 0, 0, 0},
  {"blue",  Uint8,   Uint8,   offsetof(Vertex,blue),  0, 0, 0, 0},
  {"s0", Float32, Float32, offsetof(Vertex,s0), 0, 0, 0, 0},
  {"t0", Float32, Float32, offsetof(Vertex,t0), 0, 0, 0, 0},
  {"r0", Float32, Float32, offsetof(Vertex,r0), 0, 0, 0, 0},
  {"s1", Float32, Float32, offsetof(Vertex,s1), 0, 0, 0, 0},
  {"t1", Float32, Float32, offsetof(Vertex,t1), 0, 0, 0, 0},
  {"r1", Float32, Float32, offsetof(Vertex,r1), 0, 0, 0, 0},
  {"s2", Float32, Float32, offsetof(Vertex,s2), 0, 0, 0, 0},
  {"t2", Float32, Float32, offsetof(Vertex,t2), 0, 0, 0, 0},
  {"r2", Float32, Float32, offsetof(Vertex,r2), 0, 0, 0, 0},
  {"s3", Float32, Float32, offsetof(Vertex,s3), 0, 0, 0, 0},
  {"t3", Float32, Float32, offsetof(Vertex,t3), 0, 0, 0, 0},
  {"r3", Float32, Float32, offsetof(Vertex,r3), 0, 0, 0, 0},
  {"s4", Float32, Float32, offsetof(Vertex,s4), 0, 0, 0, 0},
  {"t4", Float32, Float32, offsetof(Vertex,t4), 0, 0, 0, 0},
  {"r4", Float32, Float32, offsetof(Vertex,r4), 0, 0, 0, 0},
  {"s5", Float32, Float32, offsetof(Vertex,s5), 0, 0, 0, 0},
  {"t5", Float32, Float32, offsetof(Vertex,t5), 0, 0, 0, 0},
  {"r5", Float32, Float32, offsetof(Vertex,r5), 0, 0, 0, 0},
  {"s6", Float32, Float32, offsetof(Vertex,s6), 0, 0, 0, 0},
  {"t6", Float32, Float32, offsetof(Vertex,t6), 0, 0, 0, 0},
  {"r6", Float32, Float32, offsetof(Vertex,r6), 0, 0, 0, 0},
  {"s7", Float32, Float32, offsetof(Vertex,s7), 0, 0, 0, 0},
  {"t7", Float32, Float32, offsetof(Vertex,t7), 0, 0, 0, 0},
  {"r7", Float32, Float32, offsetof(Vertex,r7), 0, 0, 0, 0},
};

static PlyProperty s_face_props[] = {
  {"vertex_indices", Uint32, Uint32, offsetof(Face,verts),1, Uint8, Uint8, offsetof(Face,nverts)},
};


static void compute_normals (GLfloat * vList, GLfloat * nList, Face **flist, int nverts, int nfaces, bool flip_sign)
{
  int i,j;
  Face *face;
  Vertex *vert;
  unsigned int *verts;
  float x,y,z;
  float x0,y0,z0;
  float x1,y1,z1;
  float len;
  float recip;
  // zero out all normal information at vertices
  for (i = 0; i < nverts; i++)
  {
    nList[i*3] = nList[i*3 + 1] = nList[i*3 + 2] = 0;
  }
  // find normal of each face and add it to each vertex adjacent to the face
  for (i = 0; i < nfaces; i++)
  {
    face = flist[i];
    verts = face->verts;
    // determine vectors parallel to two edges of face
    x0 = vList[verts[face->nverts-1]*3]     - vList[verts[0]*3];
    y0 = vList[verts[face->nverts-1]*3 + 1] - vList[verts[0]*3 + 1];
    z0 = vList[verts[face->nverts-1]*3 + 2] - vList[verts[0]*3 + 2];
    x1 = vList[verts[1]*3]     - vList[verts[0]*3];
    y1 = vList[verts[1]*3 + 1] - vList[verts[0]*3 + 1];
    z1 = vList[verts[1]*3 + 2] - vList[verts[0]*3 + 2];
    // find cross-product between these vectors
    x = y0 * z1 - z0 * y1;
    y = z0 * x1 - x0 * z1;
    z = x0 * y1 - y0 * x1;
    // normalize this vector
    len = x*x + y*y + z*z;
    if (len == 0)
      x = y = z = 0;
    else
    {
      recip = 1.0 / float(sqrt (len));
      x *= recip;
      y *= recip;
      z *= recip;
    }
    // add this normal to each vertex that is adjacent to face
    for (j = 0; j < face->nverts; j++)
    {
      nList[verts[j]*3]     += x;
      nList[verts[j]*3 + 1] += y;
      nList[verts[j]*3 + 2] += z;
    }
  }
  // normalize all the normals at the vertices
  for (i = 0; i < nverts; i++)
  {

    GLfloat * n = &nList[i*3];
    len = n[0] * n[0] + n[1] * n[1] + n[2] * n[2];
    if (len == 0)
    {
      n[0] = n[1] = n[2] = 0;
    }
    else
    {
      if (flip_sign)
        recip = -1.0 / sqrt(len);
      else
        recip = 1.0 / sqrt(len);
      n[0] *= recip;
      n[1] *= recip;
      n[2] *= recip;
    }
  }
}


void PlyMeshFile::calcTriangles()
{
  int i, j, k;
  int elem_count;
  int nverts, nfaces = 0, numtris = 0;
  char *elem_name;
  PlyFile *ply;
  bool hasnormals = false, hasnormal[3] = {false,false,false}, hastexcoords[8] = {false,false,false,false,false,false,false,false}, hastexcoord[8][3];
  bool hasvertex = false, hasface = false;
  bool hascolors = false, hascolor[3] = {false,false,false};
  int vertexelem = -1;
  int faceelem = -1;
  int texelem[8][3];
  int normalelem[3];
  int colorelem[3];
  int texnelems[8];

  char vertexname[] = "vertex";
  char facename[] = "face";
  bool flipnormals = true;

  const char* texpropnames[8][3] = {
    { "s0", "t0", "r0" },
    { "s1", "t1", "r1" },
    { "s2", "t2", "r2" },
    { "s3", "t3", "r3" },
    { "s4", "t4", "r4" },
    { "s5", "t5", "r5" },
    { "s6", "t6", "r6" },
    { "s7", "t7", "r7" },
  };
  const char *error = NULL;

  Face face;
  Face *pface;

  
  GLfloat * vList = NULL;
  GLfloat * nList = NULL; 
  unsigned int * iList = NULL;

  Face **flist = NULL;
  // try to open file
  FILE *file = fopen(m_fileName.c_str(),"rb");
  if (!file)
  {
    error = "Could not open file for reading";
    goto error;
  }
  ply = read_ply(file);
  if (!ply)
  {
    error = "Error recognizing ply format";
    goto error;
  }
  // check if ply has vertex and face information
  for (i=0; i<ply->num_elem_types; i++)
  {
    elem_name = setup_element_read_ply(ply,i,&elem_count);
    if (equal_strings(vertexname,elem_name))
    {
      hasvertex = true;
      vertexelem = i;
    }
    else if (equal_strings(facename,elem_name))
    {
      hasface = true;
      faceelem = i;
    }
  }
  if (!hasvertex || (!hasface))
  {
    error = "Could not get vertex or face information from file";
    goto error;
  }
  // read vertex info
  setup_element_read_ply(ply, vertexelem, &elem_count);
  // create a vertex list to hold all the vertices
  
  nverts = elem_count;
  vList = new GLfloat[nverts*3];
  nList = new GLfloat[nverts*3];

  printf("Reading %d Vertices...\n", nverts);

  // set up for getting vertex positions
  setup_property_ply(ply,&s_vert_props[0]);
  setup_property_ply(ply,&s_vert_props[1]);
  setup_property_ply(ply,&s_vert_props[2]);
  for (i=0; i<8; i++)
    for (k=0; k<3; k++)
      hastexcoord[i][k] = false;
  // check if normals and texcoords are specified
  for (j=0; j<ply->elems[vertexelem]->nprops; j++)
  {
    PlyProperty *prop = ply->elems[vertexelem]->props[j];
    if (equal_strings("nx",prop->name))
    {
      hasnormal[0] = true;
      normalelem[0] = j;
    } 
    else if (equal_strings("ny",prop->name))
    {
      hasnormal[1] = true;
      normalelem[1] = j;
    } 
    else if (equal_strings("nz",prop->name))
    {
      hasnormal[2] = true;
      normalelem[2] = j;
    } 
    else if (equal_strings("red",prop->name))
    {
      hascolor[0] = true;
      colorelem[0] = j;
    }
    else if (equal_strings("green",prop->name))
    {
      hascolor[1] = true;
      colorelem[1] = j;
    }
    else if (equal_strings("blue",prop->name))
    {
      hascolor[2] = true;
      colorelem[2] = j;
    }
    else
      for (i=0; i<8; i++)
        for (k=0; k<3; k++)
          if (equal_strings(texpropnames[i][k],prop->name))
          {
            hastexcoord[i][k] = hastexcoords[i] = true;
            texelem[i][k] = j;
          }
  }
  if (hasnormal[0] && hasnormal[1] && hasnormal[2])
  {
    hasnormals = true;
    // set up for getting vertex normals
    setup_property_ply(ply,&s_vert_props[3]);
    setup_property_ply(ply,&s_vert_props[4]);
    setup_property_ply(ply,&s_vert_props[5]);
  }
  if (hascolor[0] && hascolor[1] && hascolor[2])
  {
    hascolors = true;
    // set up for getting vertex colors
    setup_property_ply(ply,&s_vert_props[6]);
    setup_property_ply(ply,&s_vert_props[7]);
    setup_property_ply(ply,&s_vert_props[8]);
  }
  for (i=0; i<8; i++)
    if (hastexcoords[i])
    {
      if (!hastexcoord[i][0] || (hastexcoord[i][2] && !hastexcoord[i][1]))
      {
        error = "File defines texture coordinates but does not define as (s) or (s+t) or (s+t+r)";
        goto error;
      }
      setup_property_ply(ply,&s_vert_props[9+i*3]);
      texnelems[i] = 1;
      if (hastexcoord[i][1])
      {
        setup_property_ply(ply,&s_vert_props[9+i*3+1]);
        texnelems[i]++;
      }
      if (hastexcoord[i][2])
      {
        setup_property_ply(ply,&s_vert_props[9+i*3+2]);
        texnelems[i]++;
      }
    }

    // grab vertex elements 
    for (j=0; j<nverts; j++)
    {
      Vertex v;
      get_element_ply(ply,(void*)&v);
      vList[j*3] = v.x * m_scale.x;
      vList[j*3+1] = v.y * m_scale.y;
      vList[j*3+2] = v.z * m_scale.z;

      nList[j*3] = v.nx;
      nList[j*3+1] = v.ny;
      nList[j*3+2] = v.nz;

      m_bb_min.x = min(vList[j*3]  , m_bb_min.x);
      m_bb_min.y = min(vList[j*3+1], m_bb_min.y);
      m_bb_min.z = min(vList[j*3+2], m_bb_min.z);

      m_bb_max.x = max(vList[j*3]  , m_bb_max.x);
      m_bb_max.y = max(vList[j*3+1], m_bb_max.y);
      m_bb_max.z = max(vList[j*3+2], m_bb_max.z);
    }

    if (hasface)
    {
      // read face info
      setup_element_read_ply(ply,faceelem,&elem_count);
      // create a list to hold all the face elements
      nfaces = elem_count;

      // set up for getting face elements
      setup_property_ply(ply,&s_face_props[0]);
      vector<unsigned int> triangles;
      // grab face elements
      if (!hasnormals)
        flist = (Face**)malloc(nfaces*sizeof(Face*));
      for (j=0; j<nfaces; j++)
      {
        if (hasnormals)
          pface = &face;
        else
          pface = flist[j] = (Face*)malloc(sizeof(Face));
        get_element_ply(ply,pface);
        int nfaceverts = pface->nverts;
        if (nfaceverts == 3) {
          triangles.push_back(pface->verts[0]);
          triangles.push_back(pface->verts[1]);
          triangles.push_back(pface->verts[2]);
        }
        else if (nfaceverts == 4) {
          triangles.push_back(pface->verts[0]);
          triangles.push_back(pface->verts[1]);
          triangles.push_back(pface->verts[2]);

          triangles.push_back(pface->verts[2]);
          triangles.push_back(pface->verts[3]);
          triangles.push_back(pface->verts[0]);
        }
        else
        {
          error = "Found a face that is not a triangle nor a quad";
          goto error;
        }
      }
      if (!hasnormals)
      {
        compute_normals(vList, nList, flist, nverts, nfaces, flipnormals);
        for (j=0; j<nfaces; j++)
          free(flist[j]);
        free(flist);
      }

      numtris = triangles.size() / 3;
      printf("Reading %d Triangles...\n", numtris);
      if (numtris > 0) {
        iList = new unsigned int[numtris*3];
        for (int i=0; i<numtris; ++i)
        {
          iList[i*3] = triangles[i*3];
          iList[i*3+1] = triangles[i*3+1];
          iList[i*3+2] = triangles[i*3+2];
        }
      }
      triangles.clear();
    }else 
    {
      error = "DOES NOT HAVE TRIANGLES";
      goto error;
    }

    close_ply(ply);

    cout << "BoundingBox Size:" << (m_bb_max - m_bb_min) ;
    cout << "BoundingBox Center:"<<((m_bb_max + m_bb_min)*.5) <<endl;

    m_numVertices = nverts;
    m_numTriangles = numtris;
    m_vertices = vList;
    m_normals = nList;
    m_indexes = iList;

    return;
error:
    printf("Error loading model from file \"%s\": error = %s\n",m_fileName.c_str(), error);
}
