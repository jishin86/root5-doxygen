// @(#)root/roostats:$Id: ProfileLikelihoodCalculator.h 39391 2011-05-26 09:51:59Z moneta $
// Author: Kyle Cranmer, Lorenzo Moneta, Gregory Schott, Wouter Verkerke
/*************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOSTATS_ProfileLikelihoodCalculator
#define ROOSTATS_ProfileLikelihoodCalculator

#ifndef ROOSTATS_CombinedCalculator
#include "RooStats/CombinedCalculator.h"
#endif

#include "RooStats/LikelihoodInterval.h"

namespace RooStats {
   
   class LikelihoodInterval; 

 class ProfileLikelihoodCalculator : public CombinedCalculator {

   public:

      // default constructor (needed for I/O)
      ProfileLikelihoodCalculator();

      /// constructor from data, from a full model pdf describing both parameter of interest and nuisance parameters 
      /// and from the set specifying the parameter of interest (POI).
      /// There is no need to specify the nuisance parameters since they are all other parameters of the model. 
      /// When using the calculator for performing an hypothesis test one needs to provide also a snapshot (a copy) 
      /// defining the null parameters and their value. There is no need to pass the alternate parameters. These  
      /// will be obtained by the value maximazing the likelihood function
      ProfileLikelihoodCalculator(RooAbsData& data, RooAbsPdf& pdf, const RooArgSet& paramsOfInterest, 
                                  Double_t size = 0.05, const RooArgSet* nullParams = 0 );


      /// constructor from data and a model configuration
      /// If the ModelConfig defines a prior pdf for any of the parameters those will be included as constrained terms in the 
      /// likelihood function 
      ProfileLikelihoodCalculator(RooAbsData& data, ModelConfig & model, Double_t size = 0.05);


      virtual ~ProfileLikelihoodCalculator();
    
      /// Return a likelihood interval. A global fit to the likelihood is performed and 
      /// the interval is constructed using the the profile likelihood ratio function of the POI
      virtual LikelihoodInterval* GetInterval() const ; 

      /// Return the hypothesis test result obtained from the likelihood ratio of the 
      /// maximum likelihood value with the null parameters fixed to their values, with respect keeping all parameters 
      /// floating (global maximum likelihood value). 
      virtual HypoTestResult* GetHypoTest() const;   
    
      

   protected:

      // clear internal fit result
      void DoReset() const; 

      // perform a global fit 
      void DoGlobalFit() const; 

      ClassDef(ProfileLikelihoodCalculator,1) // A concrete implementation of CombinedCalculator that uses the ProfileLikelihood ratio.

      mutable RooFitResult * fFitResult;  // internal  result of gloabl fit 
      
   };
}
#endif
