// @(#)root/tmva $Id: TActivationRadial.h 40005 2011-06-27 15:29:10Z stelzer $
// Author: Matt Jachowski

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : TMVA::TActivationRadial                                               *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Radial basis activation function for TNeuron                              *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Matt Jachowski  <jachowski@stanford.edu> - Stanford University, USA       *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/ 

#ifndef ROOT_TMVA_TActivationRadial
#define ROOT_TMVA_TActivationRadial

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TActivationRadial                                                    //
//                                                                      //
// Radial basis activation function for TNeuron                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TFormula
#include "TFormula.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif

#ifndef ROOT_TMVA_TActivation
#include "TMVA/TActivation.h"
#endif

namespace TMVA {
  
   class TActivationRadial : public TActivation {
    
   public:

      TActivationRadial();
      ~TActivationRadial();

      // evaluate the activation function
      Double_t Eval(Double_t arg);

      // evaluate the derivative of the activation function
      Double_t EvalDerivative(Double_t arg);

      // minimum of the range of the activation function
      Double_t GetMin() { return 0; }

      // maximum of the range of the activation function
      Double_t GetMax() { return 1; }

      // expression for the activation function
      TString GetExpression();

      // writer of function code
      virtual void MakeFunction(std::ostream& fout, const TString& fncName);

   private:

      TFormula* fEqn;                // equation of radial basis function
      TFormula* fEqnDerivative;      // equation of derivative

      ClassDef(TActivationRadial,0)  // Radial basis activation function for TNeuron
   };

} // namespace TMVA

#endif
