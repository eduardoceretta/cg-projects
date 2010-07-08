/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Arquivo Frames
   Contem a implementaçao dos metodos da classe Frames
\**********************************************************/

#include <iostream>
#include <string.h>
#include "GraphBasis/Frames.h"
#include "GraphBasis\GLFont.h"
Frames :: Frames()
{
   t1 = clock();
   cont_frames = 0;
   fps_old = 80; //valor inicial
   paused = false;
}

float Frames :: getFrames()
{
   if(paused)
      return 0.0;

   double tempo;

   t2 = clock();
   tempo  = (double)(t2 - t1) / CLOCKS_PER_SEC;
   cont_frames++;
   //apos FRAMES frames, calcula o fps medio.
   if(cont_frames==FRAMES)
   {
      t1 = t2;
      fps = FRAMES/tempo;
      cont_frames=0;
      fps_old = fps;

      //printf("\r %0.2lf frames per second  ", fps);
      return fps;
   }
   //se nao passaram 6 frames, retorna a taxa antiga
   return fps_old;
}

void Frames :: resetClock()
{
   t1 = clock();
   cont_frames = 0;
   fps_old = 80;
   paused = false;
}

long Frames :: getFrameNum()
{
   return cont_frames;
}

void Frames :: pauseClock()
{
   paused = true;
}

void Frames :: render()
{
   char frames[30];
   sprintf(frames, "FPS:%.3f",getFrames());
   string fram(frames);
   GLFont::initText();
      GLFont::print(20,20, fram);
   GLFont::endText();
}
