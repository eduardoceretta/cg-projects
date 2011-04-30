#ifndef UTL_H
#define UTL_H

#include <utl/debug.h>
#include <utl/writer.h>
#include <utl/writers/luafilewriter.h>

/**
 * Formats and throws an error message.
 * Syntax works exactly like *printf.
 */
void UtlThrowError (const char* format, ...) throw(const char*);

#endif

