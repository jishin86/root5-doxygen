// @(#)root/roostats:$Id: UpperLimitMCSModule.h 39391 2011-05-26 09:51:59Z moneta $
// Author: Kyle Cranmer, Lorenzo Moneta, Gregory Schott, Wouter Verkerke, Nils Ruthmann
/*************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


#ifndef UPPER_LIMIT_MCS_MODULE
#define UPPER_LIMIT_MCS_MODULE

#include "RooAbsMCStudyModule.h"
#include <string>

class RooArgSet; 
class RooDataSet; 
class RooRealVar; 
class RooAbsPdf; 

namespace RooStats {

   class ProfileLikelihoodCalculator; 


class UpperLimitMCSModule : public RooAbsMCStudyModule {
public:


   UpperLimitMCSModule(const RooArgSet* poi, Double_t CL=0.95) ;
   UpperLimitMCSModule(const UpperLimitMCSModule& other) ;
   virtual ~UpperLimitMCSModule() ;

   Bool_t initializeInstance() ; 

   Bool_t initializeRun(Int_t /*numSamples*/) ; 
   RooDataSet* finalizeRun() ;

   //Bool_t processAfterFit(Int_t /*sampleNum*/)  ;
   Bool_t processBetweenGenAndFit(Int_t /*sampleNum*/) ;
	
private:

   std::string _parName ;  // Name of Nsignal parameter
   RooStats::ProfileLikelihoodCalculator* _plc;
   RooRealVar* _ul ;

   const RooArgSet* _poi;  // parameters of interest
   RooDataSet* _data ;     // Summary dataset to store results
   Double_t _cl;
   RooAbsPdf* _model;

   ClassDef(UpperLimitMCSModule,0) // MCStudy module to calculate upperlimit of a given poi
};

}

#endif

