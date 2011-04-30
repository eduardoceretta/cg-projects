/************************************************************************
 *
 * Class:  header
 * Created: 7/2/2006
 * Author: MALF 
 *----------------------------------------------------------------------
 * Description: 
 *
 ************************************************************************/

#ifndef __NFR_HEADER_H__
#define __NFR_HEADER_H__

typedef enum {
	NFR_HEADER = 0,
	NFR_HEADER_AUTHOR,
	NFR_HEADER_DATE,
	NFR_HEADER_VERSION,
	NFR_HEADER_TITLE,
	NFR_HEADER_ANALYSIS,
  NFR_HEADER_ANALYSIS_MAXIMUM_STEPS,
  NFR_HEADER_ANALYSIS_TIME_STEPS,
  NFR_HEADER_ANALYSIS_TOLERANCE,
  NFR_HEADER_ANALYSIS_MAXIMUM_ITERATIONS,
  NFR_HEADER_ANALYSIS_PRINT_STEPS,
  NFR_HEADER_PRINT_ELEMENT_NODAL_SCALAR,
  NFR_HEADER_PRINT_ELEMENT_GAUSS_SCALAR,
  NFR_HEADER_ANALYSIS_ALGORITHM,
  NFR_HEADER_ANALYSIS_ERROR,
  NFR_HEADER_ERROR_ESTIMATOR,
  NFR_HEADER_ERROR_PERCENTAGE,
  NFR_HEADER_SMOOTH_PROCEDURE,
  NFR_HEADER_SMOOTH_BETA_FACTOR,
  NFR_HEADER_UNITS,
  NFR_HEADER_TYPE_MAX
} NfrHeaderType;

void nfr_headerTag (NfrImport* f, char* head, char* tail, NfrFileOffset* offset);

#endif

