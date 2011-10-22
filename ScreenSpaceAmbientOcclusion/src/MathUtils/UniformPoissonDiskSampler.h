/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2011  
 *
 *  Uniform Poisson Sampler Generator
 *
 * Adapted from java source by Herman Tulleken
 * http://www.luma.co.za/labs/2008/02/27/poisson-disk-sampling/

 * The algorithm is from the "Fast Poisson Disk Sampling in Arbitrary Dimensions" paper by Robert Bridson
 * http://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
 */
#ifndef _UNIFORM_POISSON_SAMPLER_H_
#define _UNIFORM_POISSON_SAMPLER_H_

#include <iostream>

#include "MathUtils/MathDefines.h"
#include "MathUtils/Vector3.h"

#include <vector>

using namespace std;

class UniformPoissonDiskSampler
{
  int m_defaultPointsPerIteration;

  float m_squareRootTwo;

  Vector3 m_topLeft, m_lowerRight, m_center;
  Vector3 m_dimensions;
  float *m_rejectionSqDistance;
  float m_minimumDistance;
  float m_cellSize;
  int m_gridWidth, m_gridHeight;


  Vector3*** m_grid;
  vector<Vector3> m_activePoints, m_points;

public:
  UniformPoissonDiskSampler();
  ~UniformPoissonDiskSampler();

  vector<Vector3> sampleCircle(Vector3 center, float radius, float minimumDistance);
  vector<Vector3> sampleCircle(Vector3 center, float radius, float minimumDistance, int pointsPerIteration);

  vector<Vector3> sampleRectangle(Vector3 topLeft, Vector3 lowerRight, float minimumDistance);
  vector<Vector3> sampleRectangle(Vector3 topLeft, Vector3 lowerRight, float minimumDistance, int pointsPerIteration);

private:
  void clear();

  vector<Vector3> sample(Vector3 topLeft, Vector3 lowerRight, float *rejectionDistance, float minimumDistance, int pointsPerIteration);

  void addFirstPoint();

  bool addNextPoint(Vector3 point);

  Vector3 generateRandomAround(Vector3 center, float minimumDistance);

  Vector3 denormalize(Vector3 point, Vector3 origin, double cellSize);
};


#endif

