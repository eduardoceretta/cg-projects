/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
                  Matricula:2610166
\**********************************************************/
/**********************************************************\
                  Cabecalho Frames.h
   referencia: Implementação Frames.H de Cesar Tadeu Pozzer
   Contem a definição da classe Frames
\**********************************************************/
#ifndef _FRAMES_H_
#define _FRAMES_H_

#include <stdio.h>
#include <time.h>

#define FRAMES 50

class Frames
{
   clock_t t1, t2;
   long    cont_frames;
   float   fps, fps_old;
   float   timeFromLastFrame;
   bool paused;
   public:

   Frames();

   void render();

   void pauseClock();

   float getFrames();
   inline float getTimeFromLastFrame();
   inline float accessTimeFromLastFrame();
   void resetClock();

   long getFrameNum();
};


inline float Frames :: getTimeFromLastFrame()
{
   if(paused)
      return 0.0;
   t2 = clock();
   timeFromLastFrame  = (double)(t2 - t1) / CLOCKS_PER_SEC;
   t1 = t2;

   cont_frames++;

   return timeFromLastFrame;
}




#endif
