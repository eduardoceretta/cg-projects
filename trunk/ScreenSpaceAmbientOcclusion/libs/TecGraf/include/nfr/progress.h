/* *************************************************************
*
* Class:  progress.h
* Created: 9/2/2007
* Author: rodesp
* $Id: progress.h,v 1.2 2008/02/11 20:18:07 rodesp Exp $
*
* Description: 
*   Módulo de notificação de progresso da importação.
*
* *************************************************************/

#ifndef __NFR_PROGRESS_H__
#define __NFR_PROGRESS_H__

#include "defines.h"

typedef struct NfrProgress NfrProgress;

typedef int (*NfrTotalProgressFunc) (double ratio, void* data);
typedef int (*NfrPartialProgressLabelFunc) (const char* msg, void* data);
typedef int (*NfrPartialProgressFunc) (double ratio, void* data);

/************************************************************************
*
************************************************************************/
NFR_API NfrProgress* nfr_createProgress();
NFR_API void nfr_destroyProgress(NfrProgress* self);

/************************************************************************
*
************************************************************************/
NFR_API void nfr_setTotalProgressFunc(NfrProgress* self, NfrTotalProgressFunc cb, void* data);
NFR_API void nfr_setPartialProgressLabelFunc(NfrProgress* self, NfrPartialProgressLabelFunc cb, void* data);
NFR_API void nfr_setPartialProgressFunc(NfrProgress* self, NfrPartialProgressFunc cb, void* data);

/************************************************************************
* 
************************************************************************/
NFR_API int nfr_reportPartialProgressLabelf(NfrProgress* self, const char* format, ...);
NFR_API int nfr_reportPartialProgress(NfrProgress* self, double ratio);
NFR_API int nfr_reportTotalProgress(NfrProgress* self, double ratio);

NFR_API int nfr_getProgressIsRunning(NfrProgress* self);
NFR_API void nfr_stopProgress(NfrProgress* self);


#endif