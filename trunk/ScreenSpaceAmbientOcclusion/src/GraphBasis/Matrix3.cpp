/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo Matrix3
   Contem a implementaçao dos metodos da classe Matrix3
\**********************************************************/

#include "Matrix3.h"
#include <math.h>
#include <iostream>

Matrix3 :: Matrix3()
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j]=0;
}

Matrix3 :: Matrix3(double m[3][3])
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j]=m[i][j];
}

Matrix3 :: Matrix3(float m[3][3])
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j]=(double)m[i][j];
}

Matrix3 :: Matrix3(int m[3][3])
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j]=(double)m[i][j];

}

Matrix3 :: Matrix3(const Matrix3& m)
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j] = m.mat[i][j];
}

void Matrix3 :: setLine(const double l[3], int line)
{
   if(line<3)
      for(int j=0;j<3;++j)
         mat[line][j] = l[j];
}

void Matrix3 :: setLine(const Vector3 &v, int line)
{
   if(line<3)
   {
      mat[line][0] = v.x;
      mat[line][1] = v.y;
      mat[line][2] = v.z;
   }
}

void Matrix3 :: setColumn(const double c[3], int column)
{
   if(column<3)
      for(int i=0;i<3;++i)
         mat[i][column] = c[i];
}

void Matrix3 :: setColumn(const Vector3 &v, int column)
{
   if(column<3)
   {
      mat[0][column] = v.x;
      mat[1][column] = v.y;
      mat[2][column] = v.z;
   }
}

void Matrix3 :: setValue(const double val, int line, int column)
{
   mat[line][column] = val;
}

double Matrix3 :: getValue(int line, int column)
{
   unsigned long nan[2]={0xffffffff, 0x7fffffff};
   double dNan = *( double *)nan;
   if(line < 3 && column <3)
      return mat[line][column];
   else return dNan;
}

void Matrix3 :: setIdentity()
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         if(i==j)
            mat[i][j]=1;
         else
            mat[i][j]=0;
}



void Matrix3 :: operator= (const Matrix3& m)
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j] = m.mat[i][j];
}

void Matrix3 :: operator+= (const Matrix3& m)
{
  for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j] += m.mat[i][j];
}

void Matrix3 :: operator-= (const Matrix3& m)
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j] -= m.mat[i][j];
}

void Matrix3 :: operator*= (const int val)
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j] *= (double)val;
}

void Matrix3 :: operator*= (const float val)
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j] *= (double)val;

}

void Matrix3 :: operator*= (const double val)
{
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j] *= val;

}


void Matrix3 :: operator*= (const Matrix3& m)
{
   double tmpMat[3][3];
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         tmpMat[i][j] = (mat[i][0]*m.mat[0][j] + mat[i][1]*m.mat[1][j] + mat[i][2]*m.mat[2][j] );

   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         mat[i][j] = tmpMat[i][j];
}


Matrix3 Matrix3 :: operator + (const Matrix3& m) const
{
   Matrix3 aux;
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         aux.mat[i][j] = mat[i][j]+ m.mat[i][j];
   return aux;
}

Matrix3 Matrix3 :: operator - (const Matrix3& m) const
{
   Matrix3 aux;
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         aux.mat[i][j] =mat[i][j] - m.mat[i][j];
   return aux;

}

Matrix3 Matrix3 :: operator* (const Matrix3& m) const
{
   double tmpMat[3][3];
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         tmpMat[i][j] = (mat[i][0]*m.mat[0][j] + mat[i][1]*m.mat[1][j] + mat[i][2]*m.mat[2][j] );

   Matrix3 aux(tmpMat);
   return aux;
}

Matrix3 Matrix3 :: operator* (const int val) const
{
   Matrix3 aux;
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         aux.mat[i][j] = mat[i][j] *(double)val;
   return aux;
}

Matrix3 Matrix3 :: operator* (const float val) const
{
   Matrix3 aux;
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         aux.mat[i][j] = mat[i][j] *(double)val;
   return aux;
}

Matrix3 Matrix3 :: operator* (const double val) const
{
   Matrix3 aux;
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         aux.mat[i][j] = mat[i][j] *val;
   return aux;
}

Matrix3 Matrix3 :: operator~() const
{
   Matrix3 aux;
   for(int i=0;i<3;++i)
      for(int j=0;j<3;++j)
         aux.mat[i][j] = mat[j][i];
   return aux;
}

double Matrix3 :: det() const
{
   double det=0;
   for(int j=0;j<3;j++)
      det+=mat[0][j]*mat[1][(j+1)%3]*mat[2][(j+2)%3];
   for(int j=2;j>=0;j--)
      det-= (mat[0][j]*mat[1][(j-1>=0)?j-1:3+(j-1)]*mat[2][(j-2>=0)?j-2:3+(j-2)]);
   return det;
}


std::ostream &operator<<(std::ostream & os, Matrix3  & m)
{
   for(int i=0;i<3;++i)
   {
      os << "| ";
      for(int j=0;j<3;++j)
         os << m.getValue(i,j) << " ";

      os << "|" << std::endl;
   }

   return os;
}
