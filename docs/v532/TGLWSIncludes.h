// @(#)root/gl:$Id: TGLWSIncludes.h 36706 2010-11-16 18:48:09Z matevz $
// Authors:  Timur and Matevz, May 2008

/*************************************************************************
 * Copyright (C) 1995-2004, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

// Window-system specific GL includes.
// Inclusion should only be necessary in in low-level system files.

#ifndef ROOT_TGLWSIncludes

#include "TGLIncludes.h"

#if defined(WIN32)
#  include <GL/wglew.h>
#else
#  if defined(__APPLE__)
#    define GLEW_APPLE_GLX
#  endif
#  include <GL/glxew.h>
#endif

#endif
