#include "UniformGrid.h"
#include <math.h>

UniformGrid::UniformGrid(unsigned int p_numTriangles, std::vector<Mesh>* p_mesh, std::vector<Material>* p_material, Vector3 p_numVoxels)
{
	m_min = Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	m_max = Vector3(0, 0, 0);

	calculateBB(p_mesh, p_numVoxels);
	calculateGrid(p_numTriangles, p_mesh, p_material, p_numVoxels);
}


UniformGrid::~UniformGrid(){

	delete [] m_gridArray;
	delete [] m_triangleListArray;
	delete [] m_triangleVertexArray;
	delete [] m_triangleNormalsArray;
	delete [] m_triangleAmbientArray;
	delete [] m_triangleDiffuseArray;
	delete [] m_triangleSpecularArray;

}
	
void UniformGrid::calculateBB(std::vector<Mesh>* p_mesh, Vector3 p_numVoxels)
{

	std::vector<Mesh> :: iterator meshIt;
	for(meshIt = p_mesh->begin(); meshIt!=p_mesh->end(); ++meshIt){
		std::vector<Triangle>* trianglesArray = meshIt->getTriangles();
		for(int i=0; i<trianglesArray->size(); i++){
			Triangle currentTriangle = trianglesArray->at(i);

			setMinMax(currentTriangle.v1);
			setMinMax(currentTriangle.v2);
			setMinMax(currentTriangle.v3);
		}
	}

	m_numVoxels = Vector3(p_numVoxels);
	m_gridSize = Vector3(fabsf(m_max.x - m_min.x), fabsf(m_max.y - m_min.y), fabsf(m_max.z - m_min.z));
	m_voxelSize = Vector3(m_gridSize.x / p_numVoxels.x, m_gridSize.y / p_numVoxels.y, m_gridSize.z / p_numVoxels.z);
}

void UniformGrid::calculateGrid(unsigned int p_numTriangles, std::vector<Mesh>* p_mesh, std::vector<Material>* p_material, Vector3 p_numVoxels)
{
	std::vector<Triangle*>* aux_grid = new std::vector<Triangle*>[(int)(p_numVoxels.x * p_numVoxels.y * p_numVoxels.z)];
	unsigned int size = 0;
	for(unsigned int i=0; i<p_mesh->size(); i++){
		std::vector<Triangle>* trianglesArray = p_mesh->at(i).getTriangles();
		for(unsigned int i=0; i<trianglesArray->size(); i++){
			Triangle* currentTriangle = &(trianglesArray->at(i));

			Vector3 voxelIndex_vertex1 = getVertexGridIndex(currentTriangle->v1);
			Vector3 voxelIndex_vertex2 = getVertexGridIndex(currentTriangle->v2);
			Vector3 voxelIndex_vertex3 = getVertexGridIndex(currentTriangle->v3);

			aux_grid[getVoxelAt(voxelIndex_vertex1)].push_back(currentTriangle);
			size++;
			if((voxelIndex_vertex2 == voxelIndex_vertex1) == false){
				aux_grid[getVoxelAt(voxelIndex_vertex2)].push_back(currentTriangle);
				size++;
			}
			if((voxelIndex_vertex3 == voxelIndex_vertex2) == false && (voxelIndex_vertex3 == voxelIndex_vertex1) == false){
				aux_grid[getVoxelAt(voxelIndex_vertex3)].push_back(currentTriangle);
				size++;
			}
		}
	}
  

	m_gridArraySize = (int)(p_numVoxels.x * p_numVoxels.y * p_numVoxels.z) * 4;
	m_gridArray = new GLfloat[m_gridArraySize];
	memset(m_gridArray, -1, sizeof(GLfloat) * m_gridArraySize);

  size += p_numVoxels.x * p_numVoxels.y * p_numVoxels.z;
	m_triangleListArraySize = size;
	m_triangleListArray = new GLfloat[m_triangleListArraySize];
	memset(m_triangleListArray, -1, sizeof(GLfloat) * m_triangleListArraySize);

	m_triangleVertexArraySize = p_numTriangles * 3 * 3;
	m_triangleVertexArray = new GLfloat[m_triangleVertexArraySize];
	memset(m_triangleVertexArray, 0, sizeof(GLfloat) * m_triangleVertexArraySize);

	m_triangleNormalsArraySize = p_numTriangles * 3;
	m_triangleNormalsArray = new GLfloat[m_triangleNormalsArraySize];
	memset(m_triangleNormalsArray, 0, sizeof(GLfloat) * m_triangleNormalsArraySize);

	m_triangleAmbientArraySize = p_numTriangles * 3;
	m_triangleAmbientArray = new GLfloat[m_triangleAmbientArraySize];
	memset(m_triangleAmbientArray, 0, sizeof(GLfloat) * m_triangleAmbientArraySize);

	m_triangleDiffuseArraySize = p_numTriangles * 3;
	m_triangleDiffuseArray = new GLfloat[m_triangleDiffuseArraySize];
	memset(m_triangleDiffuseArray, 0, sizeof(GLfloat) * m_triangleDiffuseArraySize);

	m_triangleSpecularArraySize = p_numTriangles * 4;
	m_triangleSpecularArray = new GLfloat[m_triangleSpecularArraySize];
	memset(m_triangleSpecularArray, 0, sizeof(GLfloat) * m_triangleSpecularArraySize);

 
	unsigned int gridCount = 0;
	int cont = 0;
  int gridIndexCont = 0;
	for(int x=0; x<p_numVoxels.x; x++){
		for(int y=0; y<p_numVoxels.y; y++){
			for(int z=0; z<p_numVoxels.z; z++){
				for(int j=0; j<aux_grid[cont].size(); j++){
					unsigned int triangleIndex = aux_grid[cont].at(j)->getGlobalIndex();
					m_triangleListArray[gridCount+j] = aux_grid[cont].at(j)->getGlobalIndex();
					m_triangleVertexArray[gridCount+j] = aux_grid[cont].at(j)->v1.x;
					m_triangleVertexArray[gridCount+j+1] = aux_grid[cont].at(j)->v1.y;
					m_triangleVertexArray[gridCount+j+2] = aux_grid[cont].at(j)->v1.z;

					m_triangleVertexArray[gridCount+j+1] = aux_grid[cont].at(j)->v2.x;
					m_triangleVertexArray[gridCount+j+1+1] = aux_grid[cont].at(j)->v2.y;
					m_triangleVertexArray[gridCount+j+1+2] = aux_grid[cont].at(j)->v2.z;

					m_triangleVertexArray[triangleIndex] = aux_grid[cont].at(j)->v3.x;
					m_triangleVertexArray[triangleIndex+1] = aux_grid[cont].at(j)->v3.y;
					m_triangleVertexArray[triangleIndex+2] = aux_grid[cont].at(j)->v3.z;

					Vector3 n = (aux_grid[cont].at(j)->v2 - aux_grid[cont].at(j)->v1) ^ (aux_grid[cont].at(j)->v3 - aux_grid[cont].at(j)->v2);
					n = n.unitary();
					m_triangleNormalsArray[triangleIndex] = n.x;
					m_triangleNormalsArray[triangleIndex+1] = n.y;
					m_triangleNormalsArray[triangleIndex+2] = n.z;


					m_triangleSpecularArray[triangleIndex] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).mSpecular.r;
					m_triangleSpecularArray[triangleIndex+1] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).mSpecular.g;
					m_triangleSpecularArray[triangleIndex+2] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).mSpecular.b;
					m_triangleSpecularArray[triangleIndex+3] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).mSpecularExp;

					m_triangleDiffuseArray[triangleIndex] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).mDiffuse.r;
					m_triangleDiffuseArray[triangleIndex+1] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).mDiffuse.g;
					m_triangleDiffuseArray[triangleIndex+2] = p_material->at(aux_grid[cont].at(j)->getMaterialIndex()).mDiffuse.b;
				}
				m_gridArray[gridIndexCont++] = x;
				m_gridArray[gridIndexCont++] = y;
				m_gridArray[gridIndexCont++] = z;
				m_gridArray[gridIndexCont++] = aux_grid[cont].size();
        //m_gridArray[gridIndexCont++] = -1;
				
				gridCount += aux_grid[cont].size()+1;

				cont++;
			}
		}
	}
  for(int i=0; i<m_gridArraySize; i+=4)
  {
    printf("%f %f %f %f\n",m_gridArray[i],m_gridArray[i+1],m_gridArray[i+2],m_gridArray[i+3] );
  }


	delete [] aux_grid;
}

Vector3 UniformGrid::getVertexGridIndex(Vector3 vertex){

	Vector3 index;
	index.x = (int)(((vertex.x - m_min.x)/m_gridSize.x) * m_numVoxels.x);
	index.y = (int)(((vertex.y - m_min.y)/m_gridSize.y) * m_numVoxels.y);
	index.z = (int)(((vertex.z - m_min.z)/m_gridSize.z) * m_numVoxels.z);


	return index;
}

unsigned int UniformGrid::getVoxelAt(Vector3 index){

	return index.x*m_numVoxels.y*m_numVoxels.z + index.y*m_numVoxels.z + index.z;
}

void UniformGrid::setMinMax(Vector3 vertex){

	if(vertex.x < m_min.x)
		m_min.x = vertex.x;
	else if(vertex.x > m_max.x)
		m_max.x = vertex.x;

	if(vertex.y < m_min.y)
		m_min.y = vertex.y;
	else if(vertex.y > m_max.y)
		m_max.y = vertex.y;

	if(vertex.z < m_min.z)
		m_min.z = vertex.z;
	else if(vertex.z > m_max.z)
		m_max.z = vertex.z;

}

void UniformGrid::render(){

	glBegin(GL_LINES);
		
		glVertex3f(m_min.x, m_min.y, m_min.z); glVertex3f(m_min.x, m_min.y, m_max.z);
		glVertex3f(m_min.x, m_min.y, m_min.z); glVertex3f(m_min.x, m_max.y, m_min.z);
		glVertex3f(m_min.x, m_min.y, m_min.z); glVertex3f(m_max.x, m_min.y, m_min.z);

		glVertex3f(m_max.x, m_max.y, m_max.z); glVertex3f(m_max.x, m_max.y, m_min.z);
		glVertex3f(m_max.x, m_max.y, m_max.z); glVertex3f(m_max.x, m_min.y, m_max.z);
		glVertex3f(m_max.x, m_max.y, m_max.z); glVertex3f(m_min.x, m_max.y, m_max.z);
		
		glVertex3f(m_max.x, m_min.y, m_max.z); glVertex3f(m_max.x, m_min.y, m_min.z);
		glVertex3f(m_min.x, m_max.y, m_max.z); glVertex3f(m_min.x, m_max.y, m_min.z);
		
		glVertex3f(m_max.x, m_min.y, m_max.z); glVertex3f(m_min.x, m_min.y, m_max.z);
		glVertex3f(m_min.x, m_max.y, m_min.z); glVertex3f(m_max.x, m_max.y, m_min.z);
		

		glVertex3f(m_min.x, m_min.y, m_max.z); glVertex3f(m_min.x, m_max.y, m_max.z);
		glVertex3f(m_max.x, m_min.y, m_min.z); glVertex3f(m_max.x, m_max.y, m_min.z);
	glEnd();

}

Vector3 UniformGrid::getBBMin(){
	return m_min;
}
Vector3 UniformGrid::getBBMax(){
	return m_max;
}


Vector3 UniformGrid::getVoxelSize(){
  return m_voxelSize;
}
GLfloat* UniformGrid::getGridArray()
{
  return m_gridArray;
}

GLfloat* UniformGrid::getTriangleListArray()
{
  return m_triangleListArray;
}

GLfloat* UniformGrid::getTriangleVertexArray()
{
  return m_triangleVertexArray;
}

GLfloat* UniformGrid::getTriangleNormalsArray()
{
  return m_triangleNormalsArray;
}

GLfloat* UniformGrid::getTriangleAmbientArray()
{
  return m_triangleAmbientArray;
}

GLfloat* UniformGrid::getTriangleDiffuseArray()
{
  return m_triangleDiffuseArray;
}

GLfloat* UniformGrid::getTriangleSpecularArray()
{
  return m_triangleSpecularArray;
}


int UniformGrid::getGridArraySize(){
	return (int)((float)(m_gridArraySize)/4.0);
}

int UniformGrid::getTriangleListArraySize(){
	return m_triangleListArraySize;
}

int UniformGrid::getTriangleVertexArraySize(){
	return (int)((float)(m_triangleVertexArraySize)/3.0);
}

int UniformGrid::getTriangleAmbientArraySize(){
  return (int)((float)(m_triangleAmbientArraySize)/3.0);
}


int UniformGrid::getTriangleDiffuseArraySize(){
	return (int)((float)(m_triangleDiffuseArraySize)/3.0);
}

int UniformGrid::getTriangleSpecularArraySize(){
	return (int)((float)(m_triangleSpecularArraySize)/4.0);
}


Vector3 UniformGrid::getNumVoxels(){
	return m_numVoxels;
}

int UniformGrid::getTriangleNormalsArraySize()
{
  return (int)((float)(m_triangleNormalsArraySize)/3.0);
}


int UniformGrid::getGridArrayAbsoluteSize()
{
  return m_gridArraySize;
}

int UniformGrid::getTriangleListArrayAbsoluteSize()
{
  return m_triangleListArraySize;
}

int UniformGrid::getTriangleVertexArrayAbsoluteSize()
{
  return m_triangleVertexArraySize;
}

int UniformGrid::getTriangleNormalsArrayAbsoluteSize()
{
  return m_triangleNormalsArraySize;
}

int UniformGrid::getTriangleAmbientArrayAbsoluteSize()
{
  return m_triangleAmbientArraySize;
}

int UniformGrid::getTriangleDiffuseArrayAbsoluteSize()
{
  return m_triangleDiffuseArraySize;
}

int UniformGrid::getTriangleSpecularArrayAbsoluteSize()
{
  return m_triangleSpecularArraySize;
} 
