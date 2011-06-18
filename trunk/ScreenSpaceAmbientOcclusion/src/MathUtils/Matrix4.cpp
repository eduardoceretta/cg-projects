/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jul 2008  
 *
 *  Analytic LineXColumn Matrix4x4 math.
 */

#include "Matrix4.h"
#include <math.h>
#include <iostream>

Matrix4 :: Matrix4()
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j]=0;
}

Matrix4 :: Matrix4(double m[4][4])
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j]=m[i][j];
}

Matrix4 :: Matrix4(float m[4][4])
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j]=(double)m[i][j];
}

Matrix4 :: Matrix4(float *m)
{
  for(int i=0;i<16;++i)
    ((double*)(&m_mat[0]))[i]=(double)m[i];
}

Matrix4 :: Matrix4(int m[4][4])
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j]=(double)m[i][j];

}

Matrix4 :: Matrix4(const Matrix4& m)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j] = m.m_mat[i][j];
}

void Matrix4 :: setLine(const double l[4], int line)
{
   if(line<4)
      for(int j=0;j<4;++j)
         m_mat[line][j] = l[j];
}

void Matrix4 :: setColumn(const double c[4], int column)
{
   if(column<4)
      for(int i=0;i<4;++i)
         m_mat[i][column] = c[i];
}

void Matrix4 :: setValue(const double val, int line, int column)
{
   m_mat[line][column] = val;
}

double Matrix4 :: getValue(int line, int column)
{
   unsigned long nan[2]={0xffffffff, 0x7fffffff};
   double dNan = *(double *)nan;
   if(line < 4 && column <4)
      return m_mat[line][column];
   else return dNan;
}

void Matrix4 :: setIdentity()
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         if(i==j)
            m_mat[i][j]=1;
         else
            m_mat[i][j]=0;
}

void Matrix4 :: setRotationMatrix(double ang, Vector3 ax)
{
   double mAng = DEG_TO_RAD(ang);
   m_mat[0][0] = (1-cos(mAng))*ax.x+cos(mAng);
   m_mat[0][1] = (1-cos(mAng))*ax.x*ax.y+sin(mAng)*ax.z;
   m_mat[0][2] = (1-cos(mAng))*ax.x*ax.z+sin(mAng)*ax.y;
   m_mat[0][3] = 0;

   m_mat[1][0] = (1-cos(mAng))*ax.x*ax.y+sin(mAng)*ax.z;
   m_mat[1][1] = (1-cos(mAng))*ax.y*ax.y+cos(mAng);
   m_mat[1][2] = (1-cos(mAng))*ax.y*ax.z-sin(mAng)*ax.x;
   m_mat[1][3] = 0;

   m_mat[2][0] = (1-cos(mAng))*ax.x*ax.z-sin(mAng)*ax.y;
   m_mat[2][1] = (1-cos(mAng))*ax.y*ax.z+sin(mAng)*ax.x;
   m_mat[2][2] = (1-cos(mAng))*ax.z*ax.z+cos(mAng);
   m_mat[2][3] = 0;

   m_mat[3][0] = 0;
   m_mat[3][1] = 0;
   m_mat[3][2] = 0;
   m_mat[3][3] = 1;
}




void Matrix4 :: operator= (const Matrix4& m)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j] = m.m_mat[i][j];
}

void Matrix4 :: operator+= (const Matrix4& m)
{
  for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j] += m.m_mat[i][j];
}

void Matrix4 :: operator-= (const Matrix4& m)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j] -= m.m_mat[i][j];
}

void Matrix4 :: operator*= (const int val)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j] *= (double)val;
}

void Matrix4 :: operator*= (const float val)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j] *= (double)val;

}

void Matrix4 :: operator*= (const double val)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j] *= val;

}


void Matrix4 :: operator*= (const Matrix4& m)
{
   double tmpMat[4][4];
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         tmpMat[i][j] = (m_mat[i][0]*m.m_mat[0][j] + m_mat[i][1]*m.m_mat[1][j] + m_mat[i][2]*m.m_mat[2][j] + m_mat[i][3]*m.m_mat[3][j]);

   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         m_mat[i][j] = tmpMat[i][j];
}


Matrix4 Matrix4 :: operator + (const Matrix4& m) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.m_mat[i][j] = m_mat[i][j]+ m.m_mat[i][j];
   return aux;
}

Matrix4 Matrix4 :: operator - (const Matrix4& m) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.m_mat[i][j] =m_mat[i][j] - m.m_mat[i][j];
   return aux;

}

Matrix4 Matrix4 :: operator* (const Matrix4& m) const
{
   double tmpMat[4][4];
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         tmpMat[i][j] = (m_mat[i][0]*m.m_mat[0][j] + m_mat[i][1]*m.m_mat[1][j] + m_mat[i][2]*m.m_mat[2][j] + m_mat[i][3]*m.m_mat[3][j]);

   Matrix4 aux(tmpMat);
   return aux;
}

Matrix4 Matrix4 :: operator* (const int val) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.m_mat[i][j] = m_mat[i][j] *(double)val;
   return aux;
}

Matrix4 Matrix4 :: operator* (const float val) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.m_mat[i][j] = m_mat[i][j] *(double)val;
   return aux;
}

Matrix4 Matrix4 :: operator* (const double val) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.m_mat[i][j] = m_mat[i][j] *val;
   return aux;
}

Vector3 Matrix4 :: operator* (const Vector3& v) const
{
   Vector3 aux(m_mat[0][0]*v.x + m_mat[0][1]*v.y + m_mat[0][2]*v.z  + m_mat[0][3],
               m_mat[1][0]*v.x + m_mat[1][1]*v.y + m_mat[1][2]*v.z  + m_mat[1][3],
               m_mat[2][0]*v.x + m_mat[2][1]*v.y + m_mat[2][2]*v.z  + m_mat[2][3]  );
   return aux;
}


void Matrix4::Inverse()
{
  float Adj[16];    // Cofactors and adjoint matrix
  float Det2x2[12]; // 2x2 sub matrix determinants

  double * matt = (double*)(&m_mat[0]);

  float m0  = matt[0];
  float m1  = matt[1];
  float m2  = matt[2];
  float m3  = matt[3];
  float m4  = matt[4];
  float m5  = matt[5];
  float m6  = matt[6];
  float m7  = matt[7];
  float m8  = matt[8];
  float m9  = matt[9];
  float m10 = matt[10];
  float m11 = matt[11];
  float m12 = matt[12];
  float m13 = matt[13];
  float m14 = matt[14];
  float m15 = matt[15];

  // Auxiliary determinants of 2x2 sub-mattrices

  Det2x2[0]  = m10 * m15 - m11 * m14;
  Det2x2[1]  = m9  * m15 - m11 * m13;
  Det2x2[2]  = m9  * m14 - m10 * m13;
  Det2x2[3]  = m8  * m15 - m11 * m12;
  Det2x2[4]  = m8  * m14 - m10 * m12;
  Det2x2[5]  = m8  * m13 - m9  * m12;

  // First 4 cofactors

  Adj[0]  =   m5 * Det2x2[0] - m6 * Det2x2[1]  + m7 * Det2x2[2];
  Adj[1]  = -(m4 * Det2x2[0] - m6 * Det2x2[3]  + m7 * Det2x2[4]);
  Adj[2]  =   m4 * Det2x2[1] - m5 * Det2x2[3]  + m7 * Det2x2[5];
  Adj[3]  = -(m4 * Det2x2[2] - m5 * Det2x2[4]  + m6 * Det2x2[5]);

  // Calculation and verification of the 4x4 mattrix determinant

  float Det4x4 = m0 * Adj[0]
  + m1 * Adj[1]
  + m2 * Adj[2]
  + m3 * Adj[3];

  if (Det4x4 == 0.0f)     // The mattrix is not invertible! Return a mattrix filled with zeros
  {
    matt[0] = 0.0f; matt[4] = 0.0f; matt[8]  = 0.0f; matt[12] = 0.0f;
    matt[1] = 0.0f; matt[5] = 0.0f; matt[9]  = 0.0f; matt[13] = 0.0f;
    matt[2] = 0.0f; matt[6] = 0.0f; matt[10] = 0.0f; matt[14] = 0.0f;
    matt[3] = 0.0f; matt[7] = 0.0f; matt[11] = 0.0f; matt[15] = 0.0f;
    //return false;
  }

  // Other auxiliary determinants of 2x2 sub-mattrices

  Det2x2[6]  = m2  * m7  - m3  * m6;
  Det2x2[7]  = m1  * m7  - m3  * m5;
  Det2x2[8]  = m1  * m6  - m2  * m5;
  Det2x2[9]  = m0  * m7  - m3  * m4;
  Det2x2[10] = m0  * m6  - m2  * m4;
  Det2x2[11] = m0  * m5  - m1  * m4;

  // Other 12 cofactors

  Adj[4]  = -(m1 * Det2x2[0] - m2 * Det2x2[1]  + m3 * Det2x2[2]);
  Adj[5]  =   m0 * Det2x2[0] - m2 * Det2x2[3]  + m3 * Det2x2[4];
  Adj[6]  = -(m0 * Det2x2[1] - m1 * Det2x2[3]  + m3 * Det2x2[5]);
  Adj[7]  =   m0 * Det2x2[2] - m1 * Det2x2[4]  + m2 * Det2x2[5];

  Adj[8]  =   m13* Det2x2[6] - m14* Det2x2[7]  + m15* Det2x2[8];
  Adj[9]  = -(m12* Det2x2[6] - m14* Det2x2[9]  + m15* Det2x2[10]);
  Adj[10] =   m12* Det2x2[7] - m13* Det2x2[9]  + m15* Det2x2[11];
  Adj[11] = -(m12* Det2x2[8] - m13* Det2x2[10] + m14* Det2x2[11]);
  Adj[12] = -(m9 * Det2x2[6] - m10* Det2x2[7]  + m11* Det2x2[8]);
  Adj[13] =   m8 * Det2x2[6] - m10* Det2x2[9]  + m11* Det2x2[10];
  Adj[14] = -(m8 * Det2x2[7] - m9 * Det2x2[9]  + m11* Det2x2[11]);
  Adj[15] =   m8 * Det2x2[8] - m9 * Det2x2[10] + m10* Det2x2[11];

  // Finding the transposed adjoint mattrix, which is the inverse

  float IDet = (1.0f / Det4x4);

  matt[0] = IDet * Adj[0];  matt[4] = IDet * Adj[1];  matt[8]  = IDet * Adj[2];  matt[12] = IDet * Adj[3];
  matt[1] = IDet * Adj[4];  matt[5] = IDet * Adj[5];  matt[9]  = IDet * Adj[6];  matt[13] = IDet * Adj[7];
  matt[2] = IDet * Adj[8];  matt[6] = IDet * Adj[9];  matt[10] = IDet * Adj[10]; matt[14] = IDet * Adj[11];
  matt[3] = IDet * Adj[12]; matt[7] = IDet * Adj[13]; matt[11] = IDet * Adj[14]; matt[15] = IDet * Adj[15];
}

void swap(double * v, int pos1, int pos2)
{
  double tmp = v[pos1];
  v[pos1]=v[pos2];
  v[pos2]=tmp;
}

void Matrix4::Transpose()
{
  swap((double*)m_mat,1,4);
  swap((double*)m_mat,2,8);
  swap((double*)m_mat,3,12);
  swap((double*)m_mat,6,9);
  swap((double*)m_mat,7,13);
  swap((double*)m_mat,11,14);
}



std::ostream &operator<<(std::ostream & os, Matrix4  & m)
{
   for(int i=0;i<4;++i)
   {
      os << "| ";
      for(int j=0;j<4;++j)
         os << m.getValue(i,j) << " ";

      os << "|" << std::endl;
   }

   return os;
}
