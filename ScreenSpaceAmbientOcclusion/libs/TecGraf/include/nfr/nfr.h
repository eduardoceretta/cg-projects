/************************************************************************
 *
 * Class:  nfr
 * Created: 15/2/2006
 * Author: MALF 
 *
 * Description: 
 *   Interface da biblioteca nfr.
 ************************************************************************/

#ifndef __NFR_H__
#define __NFR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "defines.h"
#include "error.h"
#include "progress.h"
#include "import.h"
#include "check.h"
#include "fileutils.h"
#include "tags/beam.h"
#include "tags/element.h"
#include "tags/function.h"
#include "tags/header.h"
#include "tags/integration.h"
#include "tags/load.h"
#include "tags/material.h"
#include "tags/nodal.h"
#include "tags/node.h"
#include "tags/quadrature.h"
#include "tags/remark.h"
#include "tags/result.h"
#include "tags/rezone.h"
#include "tags/section.h"
#include "tags/thickness.h"

//#define DEBUG_MODE_VALUES
//#define DEBUG_MODE_TAGS

#ifdef __cplusplus
}
#endif

#endif