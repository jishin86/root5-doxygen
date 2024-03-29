// @(#)root/mathcore:$Id: TFitResultPtr.h 31262 2009-11-18 07:38:37Z brun $
// Author: David Gonzalez Maline Tue Nov 10 15:01:24 2009

/*************************************************************************
 * Copyright (C) 1995-2009, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TFitResultPtr
#define ROOT_TFitResultPtr

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TFitResultPtr                                                        //
//                                                                      //
// Provides an indirection to TFitResult class and with a semantics     //
// identical to a TFitResult pointer                                    //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

class TFitResult;

class TFitResultPtr {
public:

   TFitResultPtr(int status = -1): fStatus(status), fPointer(0) {};

   TFitResultPtr(TFitResult* p);

   TFitResultPtr(const TFitResultPtr& rhs); 

   operator int() const { return fStatus; }
   
   TFitResult& operator*() const;

   TFitResult* operator->() const;

   TFitResult* Get() const { return fPointer; }

   TFitResultPtr& operator= (const TFitResultPtr& rhs); 

   virtual ~TFitResultPtr();

private:
   
   int fStatus;          // fit status code
   TFitResult* fPointer; // Smart Pointer to TFitResult class  

   ClassDef(TFitResultPtr,1)  //indirection to TFitResult
};

#endif
