#ifndef UniformGrid_H
#define UniformGrid_H

#include <GL/freeglut.h>
#include <vector>
#include <limits>

#include "GraphBasis/Vector3.h"
#include "Objects/Triangle.h"
#include "Objects/Mesh.h"
#include "Objects/Material.h"

class UniformGrid {

public:
	UniformGrid(unsigned int p_numTriangles, std::vector<Mesh>* p_triangles, std::vector<Material>* p_material, Vector3 p_sizeVoxel);
	~UniformGrid();

	void render();


  GLfloat* getGridArray();
  GLfloat* getTriangleListArray();
  GLfloat* getTriangleVertexArray();
  GLfloat* getTriangleNormalsArray();
  GLfloat* getTriangleAmbientArray();
  GLfloat* getTriangleDiffuseArray();
  GLfloat* getTriangleSpecularArray();

  Vector3 getBBMin();
  Vector3 getBBMax();

  int getGridArrayAbsoluteSize();
  int getTriangleListArrayAbsoluteSize();
  int getTriangleVertexArrayAbsoluteSize();
  int getTriangleNormalsArrayAbsoluteSize();
  int getTriangleAmbientArrayAbsoluteSize();
  int getTriangleDiffuseArrayAbsoluteSize();
  int getTriangleSpecularArrayAbsoluteSize();


  int getGridArraySize();
  int getTriangleListArraySize();
  int getTriangleVertexArraySize();
  int getTriangleNormalsArraySize();
  int getTriangleAmbientArraySize();
  int getTriangleDiffuseArraySize();
  int getTriangleSpecularArraySize();

  GLuint getGridTextureId();
  Vector3 getVoxelSize();
  Vector3 getNumVoxels();

private:
	void calculateBB(std::vector<Mesh>* p_triangles, Vector3 p_numVoxels);
	void calculateGrid(unsigned int p_numTriangles, std::vector<Mesh>* p_triangles, std::vector<Material>* p_material, Vector3 p_numVoxels);
	Vector3 getVertexGridIndex(Vector3 vertex);
	unsigned int getNumTriangles();
	unsigned int getVoxelAt(Vector3 index);
	void setMinMax(Vector3 vertex);
	void initialize(void** vector, int size);

	Vector3 m_min;
	Vector3 m_max;
	Vector3 m_gridSize;
	Vector3 m_numVoxels;
	Vector3 m_voxelSize;
	GLfloat* m_gridArray;
	GLfloat* m_triangleListArray;
	GLfloat* m_triangleVertexArray;
  GLfloat* m_triangleNormalsArray;
	GLfloat* m_triangleAmbientArray;
	GLfloat* m_triangleDiffuseArray;
	GLfloat* m_triangleSpecularArray;

	int m_gridArraySize;
	int m_triangleVertexArraySize;
	int m_triangleListArraySize;
	int m_triangleNormalsArraySize;
  int m_triangleAmbientArraySize;
	int m_triangleDiffuseArraySize;
	int m_triangleSpecularArraySize;
};


#endif