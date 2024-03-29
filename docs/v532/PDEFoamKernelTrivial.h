// @(#)root/tmva $Id: PDEFoamKernelTrivial.h 40005 2011-06-27 15:29:10Z stelzer $
// Author: Dominik Dannheim, Alexander Voigt

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Classes: PDEFoamKernelTrivial                                                  *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Trivial PDEFoam kernel                                                    *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      S. Jadach        - Institute of Nuclear Physics, Cracow, Poland           *
 *      Tancredi Carli   - CERN, Switzerland                                      *
 *      Dominik Dannheim - CERN, Switzerland                                      *
 *      Alexander Voigt  - TU Dresden, Germany                                    *
 *                                                                                *
 * Copyright (c) 2010:                                                            *
 *      CERN, Switzerland                                                         *
 *      MPI-K Heidelberg, Germany                                                 *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_PDEFoamKernelTrivial
#define ROOT_TMVA_PDEFoamKernelTrivial

#ifndef ROOT_TMVA_PDEFoam
#include "TMVA/PDEFoam.h"
#endif
#ifndef ROOT_TMVA_PDEFoamKernelBase
#include "TMVA/PDEFoamKernelBase.h"
#endif

namespace TMVA
{

   class PDEFoamKernelTrivial : public PDEFoamKernelBase
   {

   public:
      PDEFoamKernelTrivial();                 // Constructor
      PDEFoamKernelTrivial(const PDEFoamKernelTrivial&); // Copy Constructor
      virtual ~PDEFoamKernelTrivial() {};     // Destructor

      // kernel estimator
      virtual Float_t Estimate(PDEFoam*, std::vector<Float_t>&, ECellValue);

      ClassDef(PDEFoamKernelTrivial, 1) // trivial PDEFoam kernel estimator
   }; // end of PDEFoamKernelTrivial
}  // namespace TMVA

#endif
