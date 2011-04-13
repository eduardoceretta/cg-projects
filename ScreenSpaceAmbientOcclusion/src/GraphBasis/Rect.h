/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho Rect.h
   Contem a definição da classe Rect
\**********************************************************/
#pragma once

using namespace std;

struct Rect
{
   float ix,iy;
   float ex,ey;
   float width,height;
   Rect()
   {
      ix=iy=ex=ey=width=height=0;
   }

   Rect(float iX,float  iY,float eX,float eY)
   {
      ix = iX;
      iy = iY;
      ex = eX;
      ey = eY;
      width=ex-ix;
      height=ey-iy;
   }

   void set(float iX,float iY,float eX,float eY)
   {
      ix = iX;
      iy = iY;
      ex = eX;
      ey = eY;
      width = ex-ix;
      height = ey-iy;
   }


   bool contains(int x, int y)
   {
      float maiorX=ix,menorX=ex;
      if(ix<ex)
      {
         maiorX=ex;
         menorX=ix;
      }
      float maiorY=iy,menorY=ey;
      if(iy<ey)
      {
         maiorY=ey;
         menorY=iy;
      }
      if(x > maiorX || x < menorX || y > maiorY || y < menorY)
         return false;
      return true;
   }
};

