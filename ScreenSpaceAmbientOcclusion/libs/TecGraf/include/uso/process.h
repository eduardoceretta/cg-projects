#ifndef USO_PROCESS_H
#define USO_PROCESS_H

#include "defines.h"
#include <stdio.h>

typedef void (*USOProgramOutputCB)(const char* line, bool is_err, bool is_new, void* data);

USO_API int uso_execute (bool wait, bool hide, int argc, char** argv, const char *starting_directory);
USO_API int USOExecuteAndMonitorOutput (const char*        command,
                                        USOProgramOutputCB outputfunc,
                                        void*              outputfuncdata=NULL);

#endif

