// @(#)root/eve:$Id: TEveJetConeGL.h 35221 2010-09-10 11:46:37Z matevz $
// Author: Matevz Tadel, Jochen Thaeder 2009

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TEveJetConeGL
#define ROOT_TEveJetConeGL

#include "TGLObject.h"
#include "TEveVector.h"

class TGLViewer;
class TGLScene;

class TEveJetCone;
class TEveJetConeProjected;

//------------------------------------------------------------------------------
// TEveJetCone
//------------------------------------------------------------------------------

class TEveJetConeGL : public TGLObject
{
private:
   TEveJetConeGL(const TEveJetConeGL&);            // Not implemented
   TEveJetConeGL& operator=(const TEveJetConeGL&); // Not implemented

protected:
   TEveJetCone                     *fC;  // Model object.
   mutable std::vector<TEveVector>  fP;

   virtual void CalculatePoints() const;

public:
   TEveJetConeGL();
   virtual ~TEveJetConeGL() {}

   virtual Bool_t SetModel(TObject* obj, const Option_t* opt=0);
   virtual void   SetBBox();

   virtual void   DLCacheClear();
   virtual void   Draw(TGLRnrCtx& rnrCtx) const;
   virtual void   DirectDraw(TGLRnrCtx & rnrCtx) const;

   ClassDef(TEveJetConeGL, 0); // GL renderer class for TEveJetCone.
};


//------------------------------------------------------------------------------
// TEveJetConeProjectedGL
//------------------------------------------------------------------------------

class TEveJetConeProjectedGL : public TEveJetConeGL
{
private:
   TEveJetConeProjectedGL(const TEveJetConeProjectedGL&);            // Not implemented
   TEveJetConeProjectedGL& operator=(const TEveJetConeProjectedGL&); // Not implemented

protected:
   TEveJetConeProjected  *fM;  // Model object.

   virtual void CalculatePoints() const;

   void RenderOutline() const;
   void RenderPolygon() const;

public:
   TEveJetConeProjectedGL();
   virtual ~TEveJetConeProjectedGL() {}

   virtual Bool_t SetModel(TObject* obj, const Option_t* opt=0);
   virtual void   SetBBox();

   virtual void   Draw(TGLRnrCtx& rnrCtx) const;
   virtual void   DirectDraw(TGLRnrCtx & rnrCtx) const;

   ClassDef(TEveJetConeProjectedGL, 0); // GL renderer class for TEveJetCone.
};

#endif
