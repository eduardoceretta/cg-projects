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

#include <cmath>

#include "MathUtils/UniformPoissonDiskSampler.h"

UniformPoissonDiskSampler::UniformPoissonDiskSampler() 
:m_defaultPointsPerIteration(30)
,m_squareRootTwo((float) sqrt(2.0f))
,m_rejectionSqDistance(NULL)
,m_grid(NULL)
{

}



UniformPoissonDiskSampler::~UniformPoissonDiskSampler()
{
  clear();
}

vector<Vector3> UniformPoissonDiskSampler::sampleCircle( Vector3 center, float radius, float minimumDistance )
{
  return sampleCircle(center, radius, minimumDistance, m_defaultPointsPerIteration);
}

vector<Vector3> UniformPoissonDiskSampler::sampleCircle( Vector3 center, float radius, float minimumDistance, int pointsPerIteration )
{
  return sample(center - Vector3(radius, radius, 0.0f), center + Vector3(radius, radius, 0.0f), &radius, minimumDistance, pointsPerIteration);
}

vector<Vector3> UniformPoissonDiskSampler::sampleRectangle( Vector3 topLeft, Vector3 lowerRight, float minimumDistance )
{
  return sampleRectangle(topLeft, lowerRight, minimumDistance, m_defaultPointsPerIteration);
}

vector<Vector3> UniformPoissonDiskSampler::sampleRectangle( Vector3 topLeft, Vector3 lowerRight, float minimumDistance, int pointsPerIteration )
{
  return sample(topLeft, lowerRight, NULL, minimumDistance, pointsPerIteration);
}

vector<Vector3> UniformPoissonDiskSampler::sample( Vector3 topLeft, Vector3 lowerRight, float *rejectionDistance, float minimumDistance, int pointsPerIteration )
{
  clear();

  m_topLeft = topLeft;
  m_lowerRight = lowerRight;
  m_dimensions = lowerRight - topLeft;
  m_center = (topLeft + lowerRight)*.5f;
  m_cellSize = minimumDistance / m_squareRootTwo;
  m_minimumDistance = minimumDistance;
  m_rejectionSqDistance = rejectionDistance == NULL ? NULL : new float((*rejectionDistance) * (*rejectionDistance));
  m_gridWidth = (int) (m_dimensions.x /  m_cellSize) + 1;
  m_gridHeight = (int) (m_dimensions.y / m_cellSize) + 1;

  m_grid = new Vector3**[m_gridWidth];
  for(int i = 0; i < m_gridWidth; ++i)
  {
    m_grid[i] = new Vector3*[m_gridHeight];
    for(int j = 0; j < m_gridHeight; ++j)
      m_grid[i][j] = NULL;
  }

  addFirstPoint();

  while (m_activePoints.size() != 0)
  {
    int listIndex = rand()%int(m_activePoints.size());

    Vector3 point = m_activePoints[listIndex];
    bool found = false;

    for (int k = 0; k < pointsPerIteration; k++)
      found = found || addNextPoint(point);

    if (!found)
      m_activePoints.erase(m_activePoints.begin() + listIndex);
  }

  return m_points;
}

void UniformPoissonDiskSampler::addFirstPoint()
{
  bool added = false;
  while (!added)
  {
    float d = float(rand()%10000)/10000;
    float xr = m_topLeft.x + m_dimensions.x * d;

    d = float(rand()%10000)/10000;
    float yr = m_topLeft.y + m_dimensions.y * d;

    Vector3 p = Vector3(xr, yr, 0.0f);
    float dist = ~(p - m_center);
    if (m_rejectionSqDistance != NULL && dist*dist > *(m_rejectionSqDistance))
      continue;
    added = true;

    Vector3 index = denormalize(p, m_topLeft, m_cellSize);

    m_grid[(int) index.x][(int) index.y] = new Vector3(p);

    m_activePoints.push_back(p);
    m_points.push_back(p);
  }
}

bool UniformPoissonDiskSampler::addNextPoint(Vector3 point)
{
  bool found = false;
  Vector3 q = generateRandomAround(point, m_minimumDistance);

  float dist = ~(q - m_center);
  if (q.x >= m_topLeft.x && q.x < m_lowerRight.x && 
    q.y > m_topLeft.y && q.y < m_lowerRight.y &&
    (m_rejectionSqDistance == NULL || dist*dist <= *(m_rejectionSqDistance)))
  {
    Vector3 qIndex = denormalize(q, m_topLeft, m_cellSize);
    bool tooClose = false;

    for (int i = (int)max(0, int(qIndex.x) - 2); i < min(m_gridWidth, int(qIndex.x) + 3) && !tooClose; i++)
      for (int j = (int)max(0, int(qIndex.y) - 2); j < min(m_gridHeight, int(qIndex.y) + 3) && !tooClose; j++)
        if (m_grid[i][j] && ~(q - *(m_grid[i][j])) < m_minimumDistance)
          tooClose = true;

    if (!tooClose)
    {
      found = true;
      m_activePoints.push_back(q);
      m_points.push_back(q);
      m_grid[(int)qIndex.x][(int)qIndex.y] = new Vector3(q);
    }
  }
  return found;
}

Vector3 UniformPoissonDiskSampler::generateRandomAround( Vector3 center, float minimumDistance )
{
  float d = float(rand()%10000)/10000;
  float radius = minimumDistance + minimumDistance * d;

  d = float(rand()%10000)/10000;
  float angle = 2.0f * PI * d;

  float newX = radius * sin(angle);
  float newY = radius * cos(angle);

  return Vector3((float) (center.x + newX), (float) (center.y + newY), 0.0f);
}

Vector3 UniformPoissonDiskSampler::denormalize( Vector3 point, Vector3 origin, double cellSize )
{
  return Vector3((int) ((point.x - origin.x) / cellSize), (int) ((point.y - origin.y) / cellSize), 0);
}

void UniformPoissonDiskSampler::clear()
{
  if(m_rejectionSqDistance)
  {
    delete m_rejectionSqDistance;
    m_rejectionSqDistance = NULL;
  }

  if(m_grid)
  {
    for(int i = 0; i < m_gridWidth; ++i)
    {
      for(int j = 0; j < m_gridHeight; ++j)
        if(m_grid[i][j])
          delete m_grid[i][j];
      delete[] m_grid[i];
    }
    delete[] m_grid;
    m_grid = NULL;
  }

  m_activePoints.clear();
  m_points.clear();
}

