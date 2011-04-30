// Stream utilities
// fabraham@tecgraf.puc-rio.br
// May 2010
#ifndef IO_STREAM_UTILS_H
#define IO_STREAM_UTILS_H

#include <stdio.h>

class IOStream;

/**
 * Writes a version tag to stream 'stream'.
 * Returns true if successful, false otherwise.
 */
bool IOWriteVersion (IOStream* stream, float version);
/**
 * Checks a version tag from stream 'stream'.
 * Returns true if successful and version matches, false otherwise.
 * In case of failure and o_read_version != NULL, the read version
 * is returned. If failed reading version, *o_read_version will be FLT_MAX.
 */
bool IOCheckVersion (IOStream* stream, float expected_version, float* o_read_version=NULL);

/**
 * Writes a string to stream 'stream'.
 * Returns true if successful, false otherwise.
 */
bool IOWriteString (IOStream* stream, const char* str);

/**
 * Reads a string from stream 'stream'.
 * @str must have enough room to read the string from stream.
 * Returns true if successful, false otherwise.
 */
bool IOReadString (IOStream* stream, char* str, size_t str_len_max);

/**
 * Reads a string from stream 'stream'.
 * Returns true if successful, false otherwise.
 */
bool IOReadString (IOStream* stream, char** str);

#endif

