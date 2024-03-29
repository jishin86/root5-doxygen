// @(#)root/winnt:$Id: TWin32SplashThread.h 20882 2007-11-19 11:31:26Z rdm $
// Author: Bertrand Bellenot 14/01/04

/*************************************************************************
 * Copyright (C) 1995-2001, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TWin32SplashThread
#define ROOT_TWin32SplashThread

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

///////////////////////////////////////////////////////////////////////////////
class TWin32SplashThread {
public:
   void     *fHandle;   // splash thread handle

   TWin32SplashThread(Bool_t extended);
   ~TWin32SplashThread();
};

R__EXTERN TWin32SplashThread *gSplash;

#endif
