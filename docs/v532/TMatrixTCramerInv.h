// @(#)root/base:$Id: TMatrixTCramerInv.h 20882 2007-11-19 11:31:26Z rdm $
// Authors: Fons Rademakers, Eddy Offermann  Jan 2004

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMatrixTCramerInv
#define ROOT_TMatrixTCramerInv

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMatrixTCramerInv                                                    //
//                                                                      //
// Encapsulate templates of Cramer Inversion routines.                  //
//                                                                      //
// The 4x4, 5x5 and 6x6 are adapted from routines written by            //
// Mark Fischler and Steven Haywood as part of the CLHEP package        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#ifndef ROOT_TMatrixT
#include "TMatrixT.h"
#endif

namespace TMatrixTCramerInv {

   template<class Element> Bool_t Inv2x2(TMatrixT<Element> &m,Double_t *determ);
   template<class Element> Bool_t Inv3x3(TMatrixT<Element> &m,Double_t *determ);
   template<class Element> Bool_t Inv4x4(TMatrixT<Element> &m,Double_t *determ);
   template<class Element> Bool_t Inv5x5(TMatrixT<Element> &m,Double_t *determ);
   template<class Element> Bool_t Inv6x6(TMatrixT<Element> &m,Double_t *determ);

}

#endif
