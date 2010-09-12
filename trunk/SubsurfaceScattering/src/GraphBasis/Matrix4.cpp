/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo Matrix4
   Contem a implementaçao dos metodos da classe Matrix4
\**********************************************************/

#include "Matrix4.h"
#include <math.h>
#include <iostream>

Matrix4 :: Matrix4()
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j]=0;
}

Matrix4 :: Matrix4(double m[4][4])
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j]=m[i][j];
}

Matrix4 :: Matrix4(float m[4][4])
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j]=(double)m[i][j];
}

Matrix4 :: Matrix4(int m[4][4])
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j]=(double)m[i][j];

}

Matrix4 :: Matrix4(const Matrix4& m)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j] = m.mat[i][j];
}

void Matrix4 :: setLine(const double l[4], int line)
{
   if(line<4)
      for(int j=0;j<4;++j)
         mat[line][j] = l[j];
}

void Matrix4 :: setColumn(const double c[4], int column)
{
   if(column<4)
      for(int i=0;i<4;++i)
         mat[i][column] = c[i];
}

void Matrix4 :: setValue(const double val, int line, int column)
{
   mat[line][column] = val;
}

double Matrix4 :: getValue(int line, int column)
{
   unsigned long nan[2]={0xffffffff, 0x7fffffff};
   double dNan = *( double *)nan;
   if(line < 4 && column <4)
      return mat[line][column];
   else return dNan;
}



void Matrix4 :: setIdentity()
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         if(i==j)
            mat[i][j]=1;
         else
            mat[i][j]=0;
}

void Matrix4 :: setRotationMatrix(double ang, Vector3 ax)
{
   double mAng = DEG_TO_RAD(ang);
   mat[0][0] = (1-cos(mAng))*ax.x+cos(mAng);
   mat[0][1] = (1-cos(mAng))*ax.x*ax.y+sin(mAng)*ax.z;
   mat[0][2] = (1-cos(mAng))*ax.x*ax.z+sin(mAng)*ax.y;
   mat[0][3] = 0;

   mat[1][0] = (1-cos(mAng))*ax.x*ax.y+sin(mAng)*ax.z;
   mat[1][1] = (1-cos(mAng))*ax.y*ax.y+cos(mAng);
   mat[1][2] = (1-cos(mAng))*ax.y*ax.z-sin(mAng)*ax.x;
   mat[1][3] = 0;

   mat[2][0] = (1-cos(mAng))*ax.x*ax.z-sin(mAng)*ax.y;
   mat[2][1] = (1-cos(mAng))*ax.y*ax.z+sin(mAng)*ax.x;
   mat[2][2] = (1-cos(mAng))*ax.z*ax.z+cos(mAng);
   mat[2][3] = 0;

   mat[3][0] = 0;
   mat[3][1] = 0;
   mat[3][2] = 0;
   mat[3][3] = 1;
}




void Matrix4 :: operator= (const Matrix4& m)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j] = m.mat[i][j];
}

void Matrix4 :: operator+= (const Matrix4& m)
{
  for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j] += m.mat[i][j];
}

void Matrix4 :: operator-= (const Matrix4& m)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j] -= m.mat[i][j];
}

void Matrix4 :: operator*= (const int val)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j] *= (double)val;
}

void Matrix4 :: operator*= (const float val)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j] *= (double)val;

}

void Matrix4 :: operator*= (const double val)
{
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j] *= val;

}


void Matrix4 :: operator*= (const Matrix4& m)
{
   double tmpMat[4][4];
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         tmpMat[i][j] = (mat[i][0]*m.mat[0][j] + mat[i][1]*m.mat[1][j] + mat[i][2]*m.mat[2][j] + mat[i][3]*m.mat[3][j]);

   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         mat[i][j] = tmpMat[i][j];
}


Matrix4 Matrix4 :: operator + (const Matrix4& m) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.mat[i][j] = mat[i][j]+ m.mat[i][j];
   return aux;
}

Matrix4 Matrix4 :: operator - (const Matrix4& m) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.mat[i][j] =mat[i][j] - m.mat[i][j];
   return aux;

}

Matrix4 Matrix4 :: operator* (const Matrix4& m) const
{
   double tmpMat[4][4];
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         tmpMat[i][j] = (mat[i][0]*m.mat[0][j] + mat[i][1]*m.mat[1][j] + mat[i][2]*m.mat[2][j] + mat[i][3]*m.mat[3][j]);

   Matrix4 aux(tmpMat);
   return aux;
}

Matrix4 Matrix4 :: operator* (const int val) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.mat[i][j] = mat[i][j] *(double)val;
   return aux;
}

Matrix4 Matrix4 :: operator* (const float val) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.mat[i][j] = mat[i][j] *(double)val;
   return aux;
}

Matrix4 Matrix4 :: operator* (const double val) const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.mat[i][j] = mat[i][j] *val;
   return aux;
}

Vector3 Matrix4 :: operator* (const Vector3& v) const
{
   Vector3 aux(mat[0][0]*v.x + mat[0][1]*v.y + mat[0][2]*v.z  + mat[0][3],
               mat[1][0]*v.x + mat[1][1]*v.y + mat[1][2]*v.z  + mat[1][3],
               mat[2][0]*v.x + mat[2][1]*v.y + mat[2][2]*v.z  + mat[2][3]  );
   return aux;
}


Matrix4 Matrix4 :: operator~() const
{
   Matrix4 aux;
   for(int i=0;i<4;++i)
      for(int j=0;j<4;++j)
         aux.mat[i][j] = mat[j][i];
   return aux;
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
