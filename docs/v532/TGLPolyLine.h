// @(#)root/gl:$Id: TGLPolyLine.h 21252 2007-12-07 01:39:32Z matevz $
// Author:  Timur Pocheptsov  03/08/2004
// NOTE: This code moved from obsoleted TGLSceneObject.h / .cxx - see these
// attic files for previous CVS history

/*************************************************************************
 * Copyright (C) 1995-2006, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGLPolyLine
#define ROOT_TGLPolyLine

#ifndef ROOT_TGLLogicalShape
#include "TGLLogicalShape.h"
#endif

#include <vector>

class TBuffer3D;

class TGLPolyLine : public TGLLogicalShape
{
private:
   std::vector<Double_t> fVertices;
   Double_t              fLineWidth;

public:
   TGLPolyLine(const TBuffer3D & buffer);

   virtual void DirectDraw(TGLRnrCtx & rnrCtx) const;

   ClassDef(TGLPolyLine,0) // a polyline logical shape
};

#endif
