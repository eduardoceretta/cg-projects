#ifndef TextFile_H
#define TextFile_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GL/glew.h"

char *textFileRead(char *fn);
int textFileWrite(char *fn, char *s);
bool printInfoLog(GLhandleARB obj);

#endif