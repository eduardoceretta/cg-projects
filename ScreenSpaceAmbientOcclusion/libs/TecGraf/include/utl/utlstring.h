// utlstring.h
// String utility methods
// Jan 2010
// vbarata@tecgraf.puc-rio.br

#ifndef utl_string_h
#define utl_string_h

#include "defines.h"

/**
 * String comparison taking NULL pointers into account.
 * NULL pointers are only equal to other NULL pointers.
 *
 * @param str1 First string
 * @param str2 Second string
 *
 * @return true if equal, false otherwise
 */
UTL_API bool utlStringEqual (const char* str1, const char* str2);

/**
 * String copy with memory (re)allocation.
 * Destination string pointer must be NULL or allocated with malloc().
 * Destination string is reallocated to match the size of the source string.
 * If source string is NULL, destination string will be freed and set to NULL.
 *
 * @param dst Destination string pointer
 * @param src Source string pointer
 *
 * @return true if successful, false otherwise
 */
UTL_API bool utlStringCopy (char*& dst, const char* src);

/**
 * String concatenation with memory allocation.
 * Source string pointers may be NULL or zero-length strings.
 * Returned string must be freed with free().
 *
 * @param count Number of strings to concatenate
 * @param ... Strings to concatenate
 *
 * @return New concatenated string allocated with malloc()
 */
UTL_API char* utlStringConcat (int count, ...);

/**
 * Concatenates two strings with memory allocation.
 * Source string pointers may be NULL or zero-length strings.
 * Returned string must be freed with free().
 *
 * @param s1 First string
 * @param s2 Second string
 *
 * @return New concatenated string allocated with malloc()
 */
UTL_API char* utlStringConcat (const char* s1, const char* s2);

#endif
