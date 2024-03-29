// @(#)root/roostats:$Id: MarkovChain.h 39391 2011-05-26 09:51:59Z moneta $
// Authors: Kevin Belasco        17/06/2009
// Authors: Kyle Cranmer         17/06/2009
/*************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOSTATS_MarkovChain
#define ROOSTATS_MarkovChain

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

#ifndef ROO_REAL_VAR
#include "RooRealVar.h"
#endif
#ifndef ROO_DATA_SET
#include "RooDataSet.h"
#endif
#ifndef ROO_DATA_HIST
#include "RooDataHist.h"
#endif
#ifndef ROOT_THnSparse
#include "THnSparse.h"
#endif
//#include "RooArgSet.h"
//#include "RooMsgService.h"
//#include "RooRealVar.h"

using namespace std;

namespace RooStats {

   class MarkovChain : public TNamed {

   public:
      MarkovChain();
      MarkovChain(RooArgSet& parameters);
      MarkovChain(const char* name, const char* title, RooArgSet& parameters);

      // safely add an entry to the chain
      virtual void Add(RooArgSet& entry, Double_t nllValue, Double_t weight = 1.0);
      // add an entry to the chain ONLY IF you have constructed with parameters
      // or called SetParameters
      virtual void AddFast(RooArgSet& entry, Double_t nllValue, Double_t weight = 1.0);
      // set which of your parameters this chain should store
      virtual void SetParameters(RooArgSet& parameters);
      // get the number of steps in the chain
      virtual Int_t Size() const { return fChain->numEntries(); }
      // get the entry at position i
      virtual const RooArgSet* Get(Int_t i) const { return fChain->get(i); }
      // get the entry at the current position
      virtual const RooArgSet* Get() const { return fChain->get(); }
      // get the weight of the current (last indexed) entry
      virtual Double_t Weight() const;
      // get the weight of entry at position i
      virtual Double_t Weight(Int_t i) const;
      // get the NLL value of entry at position i
      virtual Double_t NLL(Int_t i) const; 
      // get the NLL value of the current (last indexed) entry
      virtual Double_t NLL() const;

      // get this MarkovChain as a RooDataSet whose entries contain the values
      // of whichVars.  Call with whichVars = NULL (default) to get values of
      // all variables (including NLL value and weight);
      // Note: caller owns the returned data set
      virtual RooDataSet* GetAsDataSet(RooArgSet* whichVars = NULL) const;

      // Get a clone of the markov chain on which this interval is based
      // as a RooDataSet.  You own the returned RooDataSet*
      virtual RooDataSet* GetAsDataSet(const RooCmdArg& arg1,
                                       const RooCmdArg& arg2=RooCmdArg::none(), const RooCmdArg& arg3=RooCmdArg::none(),
                                       const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),
                                       const RooCmdArg& arg6=RooCmdArg::none(), const RooCmdArg& arg7=RooCmdArg::none(),
                                       const RooCmdArg& arg8=RooCmdArg::none() ) const; 

      virtual const RooDataSet* GetAsConstDataSet() const { return fChain; }

      // get this MarkovChain as a RooDataHist whose entries contain the values
      // of whichVars.  Call with whichVars = NULL (default) to get values of
      // all variables (including NLL value and weight);
      // Note: caller owns the returned data hist
      virtual RooDataHist* GetAsDataHist(RooArgSet* whichVars = NULL) const;

      // Get a clone of the markov chain on which this interval is based
      // as a RooDataHist.  You own the returned RooDataHist*
      virtual RooDataHist* GetAsDataHist(const RooCmdArg & arg1,
                                         const RooCmdArg& arg2=RooCmdArg::none(), const RooCmdArg& arg3=RooCmdArg::none(),
                                         const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),
                                         const RooCmdArg& arg6=RooCmdArg::none(), const RooCmdArg& arg7=RooCmdArg::none(),
                                         const RooCmdArg& arg8=RooCmdArg::none() ) const; 

      // Get a clone of the markov chain on which this interval is based
      // as a sparse histogram.  You own the returned THnSparse*
      virtual THnSparse* GetAsSparseHist(RooAbsCollection* whichVars = NULL) const;
      virtual THnSparse* GetAsSparseHist(RooAbsCollection& whichVars) const
      { return GetAsSparseHist(&whichVars); }

      // get a clone of the NLL variable
      virtual RooRealVar* GetNLLVar() const
      { return (RooRealVar*)fNLL->Clone(); }

      // get a clone of the weight variable
      virtual RooRealVar* GetWeightVar() const
      { return (RooRealVar*)fWeight->Clone(); }

      virtual ~MarkovChain()
      {
         delete fParameters;
         delete fDataEntry;
         delete fChain;
      }

   protected:
      RooArgSet * fParameters;
      RooArgSet * fDataEntry;
      RooDataSet* fChain;
      RooRealVar* fNLL;
      RooRealVar* fWeight;

      ClassDef(MarkovChain,1);
   };
}

#endif
