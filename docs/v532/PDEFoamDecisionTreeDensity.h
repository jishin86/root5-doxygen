// @(#)root/tmva $Id: PDEFoamDecisionTreeDensity.h 40005 2011-06-27 15:29:10Z stelzer $
// Author: Alexander Voigt

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Classes: PDEFoamDecisionTreeDensity                                            *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Class PDEFoamDecisionTreeDensity is a class representing                  *
 *      n-dimensional real positive integrand function                            *
 *      The main function is Density() which provides the event density at a      *
 *      given point during the foam build-up (sampling).                          *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Tancredi Carli   - CERN, Switzerland                                      *
 *      Dominik Dannheim - CERN, Switzerland                                      *
 *      S. Jadach        - Institute of Nuclear Physics, Cracow, Poland           *
 *      Alexander Voigt  - TU Dresden, Germany                                    *
 *      Peter Speckmayer - CERN, Switzerland                                      *
 *                                                                                *
 * Copyright (c) 2010:                                                            *
 *      CERN, Switzerland                                                         *
 *      MPI-K Heidelberg, Germany                                                 *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_PDEFoamDecisionTreeDensity
#define ROOT_TMVA_PDEFoamDecisionTreeDensity

#include <vector>

#ifndef ROOT_TH2
#include "TH2.h"
#endif

#ifndef ROOT_TMVA_PDEFoamDensityBase
#include "TMVA/PDEFoamDensityBase.h"
#endif
#ifndef ROOT_TMVA_Volume
#include "TMVA/Volume.h"
#endif

namespace TMVA
{

   // class definition of underlying event density
   class PDEFoamDecisionTreeDensity : public PDEFoamDensityBase
   {

   protected:
      UInt_t fClass; // signal class

   public:
      PDEFoamDecisionTreeDensity();
      PDEFoamDecisionTreeDensity(std::vector<Double_t> box, UInt_t cls);
      PDEFoamDecisionTreeDensity(const PDEFoamDecisionTreeDensity&);
      virtual ~PDEFoamDecisionTreeDensity() {};

      // returns allways 0
      virtual Double_t Density(std::vector<Double_t> &Xarg, Double_t &event_density);

      // fill histograms with events found in volume
      virtual void FillHistograms(TMVA::Volume&, std::vector<TH1D*>&, std::vector<TH1D*>&, std::vector<TH1D*>&, std::vector<TH1D*>&);

      ClassDef(PDEFoamDecisionTreeDensity, 1) // Class for decision tree like PDEFoam density
   };  //end of PDEFoamDecisionTreeDensity

}  // namespace TMVA

#endif
