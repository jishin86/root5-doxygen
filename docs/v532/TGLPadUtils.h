// @(#)root/gl:$Id: TGLPadUtils.h 36532 2010-11-08 10:36:48Z couet $
// Author:  Timur Pocheptsov  06/05/2009

/*************************************************************************
 * Copyright (C) 1995-2009, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGLPadUtils
#define ROOT_TGLPadUtils

#include <vector>
#include <list>

#ifndef ROOT_RStipples
#include "RStipples.h"
#endif
#ifndef ROOT_TPoint
#include "TPoint.h"
#endif
#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

class TGLPadPainter;//For friend declarations.

/*

All code here and in corresponding *.cxx file is only
for TGLPadPainter. So, it can be limited or wrong
for something else, but it's OK for TGLPadPainter.

*/

namespace Rgl {
namespace Pad {
/*
Auxiliary class to converts ROOT's polygon stipples from
RStipples.h into GL's stipples and hold them in a fStipples array.
*/
class PolygonStippleSet {
   friend class ::TGLPadPainter;
   friend class FillAttribSet;
private:
   std::vector<unsigned char> fStipples;
   
   static const UInt_t fgBitSwap[];
   static UInt_t SwapBits(UInt_t bits);
      
   enum EGeometry {
      kRowSize = 4,//For gl, stipple is a 32x32 pixel pattern. So, 4 GLubyte objects form a single line of a stipple.
      kNRows = 32,
      kStippleSize = kNRows * kRowSize//4 * 32 == 32 lines.   
   };
   
   enum EBitMasks {
      kLow4   = 0xf,
      kUp4    = 0xf0,
      k16Bits = 0xff
   };
public:
   PolygonStippleSet();
};

/*
RAII class to enable/disable selected stipple.
*/
class FillAttribSet {
   UInt_t fStipple;
public:
   FillAttribSet(const PolygonStippleSet & set, Bool_t ignoreStipple);
   ~FillAttribSet();
};

/*
"ROOT like" line stipples.
*/

extern const UShort_t gLineStipples[];
extern const UInt_t gMaxStipple;

/*
Set/unset line attributes.
*/
class LineAttribSet {
private:
   Bool_t fSmooth;
   UInt_t fStipple;
   Bool_t fSetWidth;
public:
   LineAttribSet(Bool_t smooth, UInt_t stipple, Double_t maxWidth, Bool_t setWidth);
   ~LineAttribSet();
};

/*
Marker painter. Most markers can be painted by standlone functions.
For circles, it can be usefull to precalculate the marker geometry
and use it for poly-markers.
*/
/*
Marker painter. Most markers can be painted by standlone functions.
For circles, it can be usefull to precalculate the marker geometry
and use it for poly-markers.
*/
class MarkerPainter {
private:
   //Different TArrMarker styles.
   mutable TPoint fStar[8];
   mutable TPoint fCross[4];
   
   mutable std::vector<TPoint> fCircle;
   
   enum {
      kSmallCirclePts = 80,
      kLargeCirclePts = 150
   };
   
public:
   //Each function draw n markers.
   void DrawDot(UInt_t n, const TPoint *xy)const;
   void DrawPlus(UInt_t n, const TPoint *xy)const;
   void DrawStar(UInt_t n, const TPoint *xy)const;
   void DrawX(UInt_t n, const TPoint *xy)const;
   void DrawFullDotSmall(UInt_t n, const TPoint *xy)const;
   void DrawFullDotMedium(UInt_t n, const TPoint *xy)const;
   
   void DrawCircle(UInt_t n, const TPoint *xy)const;
   void DrawFullDotLarge(UInt_t n, const TPoint *xy)const;
   
   void DrawFullSquare(UInt_t n, const TPoint *xy)const;
   void DrawFullTrianlgeUp(UInt_t n, const TPoint *xy)const;
   void DrawFullTrianlgeDown(UInt_t n, const TPoint *xy)const;
   void DrawDiamond(UInt_t n, const TPoint *xy)const;
   void DrawCross(UInt_t n, const TPoint *xy)const;
   void DrawFullStar(UInt_t n, const TPoint *xy)const;
   void DrawOpenStar(UInt_t n, const TPoint *xy)const;

};

//
// OpenGL's tesselator calls callback functions glBegin(MODE), glVertex3(v), glEnd(),
// where v can be new vertex (or existing) and MODE is a type of mesh patch.
// MeshPatch_t is a class to save such a tesselation
// (instead of using glVertex and glBegin to draw.
//
struct MeshPatch_t {
   MeshPatch_t(Int_t type) : fPatchType(type)
   {}

   Int_t                 fPatchType; //GL_QUADS, GL_QUAD_STRIP, etc.
   std::vector<Double_t> fPatch;     //vertices.
};

typedef std::list<MeshPatch_t> Tesselation_t;

class Tesselator {


public:
   Tesselator(Bool_t dump = kFALSE);

   ~Tesselator();
   
   void *GetTess()const
   {
      return fTess;
   }

   static void SetDump(Tesselation_t *t)
   {
      fVs = t;
   }

   static Tesselation_t *GetDump()
   {
      return fVs;
   }

private:

   void *fTess;

   static Tesselation_t *fVs;//the current tesselator's dump.
};

/*
In future, this should be an interface to per-pad FBO.
Currently, in only save sizes and coordinates (?)
*/
class OffScreenDevice {
   friend class ::TGLPadPainter;
public:
   OffScreenDevice(UInt_t w, UInt_t h, UInt_t x, UInt_t y, Bool_t top);
   
private:
   UInt_t fW;
   UInt_t fH;
   UInt_t fX;
   UInt_t fY;
   Bool_t fTop;
};

void ExtractRGB(Color_t colorIndex, Float_t * rgb);

class GLLimits {
public:
   GLLimits();

   Double_t GetMaxLineWidth()const;
   Double_t GetMaxPointSize()const;
private:
   mutable Double_t fMaxLineWidth;
   mutable Double_t fMaxPointSize;
};

}//namespace Pad
}//namespace Rgl

#endif
