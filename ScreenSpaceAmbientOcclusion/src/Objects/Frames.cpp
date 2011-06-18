/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  Frames control utility class
 */

#include <iostream>
#include <string.h>
#include "Objects\Frames.h"

Frames :: Frames()
{
   t1 = clock();
   cont_frames = 0;
   fps_old = 80;
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

   if(cont_frames==FRAMES)
   {
      t1 = t2;
      fps = FRAMES/tempo;
      cont_frames=0;
      fps_old = fps;

      return fps;
   }
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

