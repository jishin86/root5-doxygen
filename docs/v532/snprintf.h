/* @(#)root/clib:$Id: snprintf.h 36336 2010-10-13 13:42:06Z rdm $ */
/* Author: Fons Rademakers  10/12/2000 */

/*
   Write formatted text to buffer 'string', using format string 'format'.
   Returns number of characters written, or -1 if truncated.
   Format string is understood as defined in ANSI C.
*/

#ifndef ROOT_snprintf
#define ROOT_snprintf

#ifndef ROOT_RConfig
#include "RConfig.h"
#endif

#ifndef __CINT__

#ifdef NEED_SNPRINTF

#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#pragma warning( push )
#pragma warning (disable: 4273)
#endif

#ifndef DONTNEED_VSNPRINTF
int vsnprintf(char *string, size_t length, const char *format, va_list args);
#endif
int snprintf(char *string, size_t length, const char *format, ...);

#ifdef WIN32
#pragma warning( pop )
#endif

#ifdef __cplusplus
}
#endif

#endif /* NEED_SNPRINTF */

#else

int snprintf(char *string, size_t length, const char *format, ...);

#endif /* __CINT__ */

#endif /* ROOT_snprintf */
