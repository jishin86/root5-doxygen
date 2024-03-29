/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooAbsPdf.h,v 1.90 2007/07/21 21:32:52 wouter Exp $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/
#ifndef ROO_ABS_PDF
#define ROO_ABS_PDF

#include "RooAbsReal.h"
//#include "RooRealIntegral.h"
#include "RooNameSet.h"
#include "RooObjCacheManager.h"
#include "RooCmdArg.h"

class RooDataSet;
class RooDataHist ;
class RooArgSet ;
class RooRealProxy ;
class RooAbsGenContext ;
class RooFitResult ;
class RooExtendPdf ;
class RooCategory ;
class TPaveText;
class TH1F;
class TH2F;
class TList ;
class RooLinkedList ;
class RooNumGenConfig ;
class RooRealIntegral ;

class RooAbsPdf : public RooAbsReal {
public:

  // Constructors, assignment etc
  RooAbsPdf() ;
  RooAbsPdf(const char *name, const char *title=0) ;
  RooAbsPdf(const char *name, const char *title, Double_t minVal, Double_t maxVal) ;
  // RooAbsPdf(const RooAbsPdf& other, const char* name=0);
  virtual ~RooAbsPdf();

  // Toy MC generation
  RooDataSet *generate(const RooArgSet &whatVars, Int_t nEvents, const RooCmdArg& arg1,
                       const RooCmdArg& arg2=RooCmdArg::none(), const RooCmdArg& arg3=RooCmdArg::none(),
                       const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none()) ;
  RooDataSet *generate(const RooArgSet &whatVars,  
                       const RooCmdArg& arg1=RooCmdArg::none(),const RooCmdArg& arg2=RooCmdArg::none(),
                       const RooCmdArg& arg3=RooCmdArg::none(),const RooCmdArg& arg4=RooCmdArg::none(),
                       const RooCmdArg& arg5=RooCmdArg::none(),const RooCmdArg& arg6=RooCmdArg::none()) ;
  RooDataSet *generate(const RooArgSet &whatVars, Int_t nEvents = 0, Bool_t verbose=kFALSE, Bool_t autoBinned=kTRUE, 
		       const char* binnedTag="", Bool_t expectedData=kFALSE) const;
  RooDataSet *generate(const RooArgSet &whatVars, const RooDataSet &prototype, Int_t nEvents= 0,
		       Bool_t verbose=kFALSE, Bool_t randProtoOrder=kFALSE, Bool_t resampleProto=kFALSE) const;


  class GenSpec {
  public:
    virtual ~GenSpec() ;
    GenSpec() { _genContext = 0 ; _protoData = 0 ; _init = kFALSE ; }
  private:
    GenSpec(RooAbsGenContext* context, const RooArgSet& whatVars, RooDataSet* protoData, Int_t nGen, Bool_t extended, 
	    Bool_t randProto, Bool_t resampleProto, TString dsetName, Bool_t init=kFALSE) ;
    GenSpec(const GenSpec& other) ;

    friend class RooAbsPdf ;
    RooAbsGenContext* _genContext ;
    RooArgSet _whatVars ;
    RooDataSet* _protoData ;
    Int_t _nGen ;
    Bool_t _extended ;
    Bool_t _randProto ;
    Bool_t _resampleProto ;
    TString _dsetName ;    
    Bool_t _init ;
    ClassDef(GenSpec,0) // Generation specification
  } ;

  GenSpec* prepareMultiGen(const RooArgSet &whatVars,  
			   const RooCmdArg& arg1=RooCmdArg::none(),const RooCmdArg& arg2=RooCmdArg::none(),
			   const RooCmdArg& arg3=RooCmdArg::none(),const RooCmdArg& arg4=RooCmdArg::none(),
			   const RooCmdArg& arg5=RooCmdArg::none(),const RooCmdArg& arg6=RooCmdArg::none()) ;
  RooDataSet* generate(GenSpec&) const ;
  

  virtual RooDataHist *generateBinned(const RooArgSet &whatVars, Double_t nEvents, const RooCmdArg& arg1,
			      const RooCmdArg& arg2=RooCmdArg::none(), const RooCmdArg& arg3=RooCmdArg::none(),
			      const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none()) ;
  virtual RooDataHist *generateBinned(const RooArgSet &whatVars,  
			      const RooCmdArg& arg1=RooCmdArg::none(),const RooCmdArg& arg2=RooCmdArg::none(),
			      const RooCmdArg& arg3=RooCmdArg::none(),const RooCmdArg& arg4=RooCmdArg::none(),
			      const RooCmdArg& arg5=RooCmdArg::none(),const RooCmdArg& arg6=RooCmdArg::none()) ;
  virtual RooDataHist *generateBinned(const RooArgSet &whatVars, Double_t nEvents, Bool_t expectedData=kFALSE, Bool_t extended=kFALSE) const;

  virtual RooDataSet* generateSimGlobal(const RooArgSet& whatVars, Int_t nEvents) ;

  virtual RooPlot* plotOn(RooPlot* frame, 
			  const RooCmdArg& arg1=RooCmdArg::none(), const RooCmdArg& arg2=RooCmdArg::none(),
			  const RooCmdArg& arg3=RooCmdArg::none(), const RooCmdArg& arg4=RooCmdArg::none(),
			  const RooCmdArg& arg5=RooCmdArg::none(), const RooCmdArg& arg6=RooCmdArg::none(),
			  const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none(),
			  const RooCmdArg& arg9=RooCmdArg::none(), const RooCmdArg& arg10=RooCmdArg::none()
              ) const {
    return RooAbsReal::plotOn(frame,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10) ;
  }


  virtual RooPlot* paramOn(RooPlot* frame, 
                           const RooCmdArg& arg1=RooCmdArg::none(), const RooCmdArg& arg2=RooCmdArg::none(), 
                           const RooCmdArg& arg3=RooCmdArg::none(), const RooCmdArg& arg4=RooCmdArg::none(), 
                           const RooCmdArg& arg5=RooCmdArg::none(), const RooCmdArg& arg6=RooCmdArg::none(), 
                           const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none()) ;

  virtual RooPlot* paramOn(RooPlot* frame, const RooAbsData* data, const char *label= "", Int_t sigDigits = 2,
			   Option_t *options = "NELU", Double_t xmin=0.50,
			   Double_t xmax= 0.99,Double_t ymax=0.95) ;

  // Built-in generator support
  virtual Int_t getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK=kTRUE) const;
  virtual void initGenerator(Int_t code) ;
  virtual void generateEvent(Int_t code);  
  virtual Bool_t isDirectGenSafe(const RooAbsArg& arg) const ; 

  // Configuration of MC generators used for this pdf
  const RooNumGenConfig* getGeneratorConfig() const ;
  static RooNumGenConfig* defaultGeneratorConfig()  ;
  RooNumGenConfig* specialGeneratorConfig() const ;
  RooNumGenConfig* specialGeneratorConfig(Bool_t createOnTheFly) ;
  void setGeneratorConfig() ;
  void setGeneratorConfig(const RooNumGenConfig& config) ;

  // -log(L) fits to binned and unbinned data
  virtual RooFitResult* fitTo(RooAbsData& data, const RooCmdArg& arg1=RooCmdArg::none(),  const RooCmdArg& arg2=RooCmdArg::none(),  
                              const RooCmdArg& arg3=RooCmdArg::none(),  const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),  
                              const RooCmdArg& arg6=RooCmdArg::none(),  const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none()) ;
  virtual RooFitResult* fitTo(RooAbsData& data, const RooLinkedList& cmdList) ;

  virtual RooAbsReal* createNLL(RooAbsData& data, const RooLinkedList& cmdList) ;
  virtual RooAbsReal* createNLL(RooAbsData& data, const RooCmdArg& arg1=RooCmdArg::none(),  const RooCmdArg& arg2=RooCmdArg::none(),  
				const RooCmdArg& arg3=RooCmdArg::none(),  const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),  
				const RooCmdArg& arg6=RooCmdArg::none(),  const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none()) ;

  // Chi^2 fits to histograms
  using RooAbsReal::chi2FitTo ;
  using RooAbsReal::createChi2 ;
  virtual RooFitResult* chi2FitTo(RooDataHist& data, const RooLinkedList& cmdList) ;
  virtual RooAbsReal* createChi2(RooDataHist& data, const RooCmdArg& arg1=RooCmdArg::none(),  const RooCmdArg& arg2=RooCmdArg::none(),  
				 const RooCmdArg& arg3=RooCmdArg::none(),  const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),  
				 const RooCmdArg& arg6=RooCmdArg::none(),  const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none()) ;

  // Chi^2 fits to X-Y datasets
  virtual RooAbsReal* createChi2(RooDataSet& data, const RooLinkedList& cmdList) ;
  




  // Constraint management
  virtual RooArgSet* getConstraints(const RooArgSet& /*observables*/, RooArgSet& /*constrainedParams*/, Bool_t /*stripDisconnected*/) const { 
    // Interface to retrieve constraint terms on this pdf. Default implementation returns null
    return 0 ; 
  }
  virtual RooArgSet* getAllConstraints(const RooArgSet& observables, RooArgSet& constrainedParams, Bool_t stripDisconnected=kTRUE) const ;
  
  // Project p.d.f into lower dimensional p.d.f
  virtual RooAbsPdf* createProjection(const RooArgSet& iset) ;  

  // Create cumulative density function from p.d.f
  RooAbsReal* createCdf(const RooArgSet& iset, const RooArgSet& nset=RooArgSet()) ;
  RooAbsReal* createCdf(const RooArgSet& iset, const RooCmdArg& arg1, const RooCmdArg& arg2=RooCmdArg::none(),
			const RooCmdArg& arg3=RooCmdArg::none(), const RooCmdArg& arg4=RooCmdArg::none(), 
			const RooCmdArg& arg5=RooCmdArg::none(), const RooCmdArg& arg6=RooCmdArg::none(), 
			const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none()) ;
  RooAbsReal* createScanCdf(const RooArgSet& iset, const RooArgSet& nset, Int_t numScanBins, Int_t intOrder) ;

  // Function evaluation support
  virtual Bool_t traceEvalHook(Double_t value) const ;  
  virtual Double_t getValV(const RooArgSet* set=0) const ;
  virtual Double_t getLogVal(const RooArgSet* set=0) const ;

  void setNormValueCaching(Int_t minNumIntDim, Int_t ipOrder=2) ;
  Int_t minDimNormValueCaching() const { return _minDimNormValueCache ; }
  Int_t intOrderNormValueCaching() const { return _valueCacheIntOrder ; }
  

  Double_t getNorm(const RooArgSet& nset) const { 
    // Get p.d.f normalization term needed for observables 'nset'
    return getNorm(&nset) ; 
  }
  virtual Double_t getNorm(const RooArgSet* set=0) const ;

  virtual void resetErrorCounters(Int_t resetValue=10) ;
  void setTraceCounter(Int_t value, Bool_t allNodes=kFALSE) ;
  Bool_t traceEvalPdf(Double_t value) const ;

  Double_t analyticalIntegralWN(Int_t code, const RooArgSet* normSet, const char* rangeName=0) const ;

  virtual Bool_t selfNormalized() const { 
    // If true, p.d.f is taken as self-normalized and no attempt is made to add a normalization term
    // This default implementation return false
    return kFALSE ; 
  }

  // Support for extended maximum likelihood, switched off by default
  enum ExtendMode { CanNotBeExtended, CanBeExtended, MustBeExtended } ;
  virtual ExtendMode extendMode() const { 
    // Returns ability of p.d.f to provided extended likelihood terms. Possible
    // answers are CanNotBeExtended, CanBeExtended or MustBeExtended. This
    // default implementation always return CanNotBeExtended
    return CanNotBeExtended ; 
  } 
  inline Bool_t canBeExtended() const { 
    // If true p.d.f can provide extended likelihood term
    return (extendMode() != CanNotBeExtended) ; 
  }
  inline Bool_t mustBeExtended() const { 
    // If true p.d.f must extended likelihood term
    return (extendMode() == MustBeExtended) ; 
  }
  virtual Double_t expectedEvents(const RooArgSet* nset) const ; 
  virtual Double_t expectedEvents(const RooArgSet& nset) const { 
    // Return expecteded number of p.d.fs to be used in calculated of extended likelihood
    return expectedEvents(&nset) ; 
  }

  // Printing interface (human readable)
  virtual void printValue(ostream& os) const ;
  virtual void printMultiline(ostream& os, Int_t contents, Bool_t verbose=kFALSE, TString indent="") const ;

  static void verboseEval(Int_t stat) ;
  static int verboseEval() ;

  virtual Double_t extendedTerm(Double_t observedEvents, const RooArgSet* nset=0) const ;

  static void clearEvalError() ;
  static Bool_t evalError() ;

  void setNormRange(const char* rangeName) ;
  const char* normRange() const { 
    return _normRange.Length()>0 ? _normRange.Data() : 0 ; 
  }
  void setNormRangeOverride(const char* rangeName) ;

  
protected:   

public:
  virtual const RooAbsReal* getNormObj(const RooArgSet* set, const RooArgSet* iset, const TNamed* rangeName=0) const ;
protected:

  RooDataSet *generate(RooAbsGenContext& context, const RooArgSet& whatVars, const RooDataSet* prototype,
		       Int_t nEvents, Bool_t verbose, Bool_t randProtoOrder, Bool_t resampleProto, Bool_t skipInit=kFALSE, 
		       Bool_t extended=kFALSE) const ;

  // Implementation version
  virtual RooPlot* paramOn(RooPlot* frame, const RooArgSet& params, Bool_t showConstants=kFALSE,
                           const char *label= "", Int_t sigDigits = 2, Option_t *options = "NELU", Double_t xmin=0.65,
			   Double_t xmax= 0.99,Double_t ymax=0.95, const RooCmdArg* formatCmd=0) ;


  virtual RooPlot* plotOn(RooPlot* frame, RooLinkedList& cmdList) const ;
  void plotOnCompSelect(RooArgSet* selNodes) const ;

  virtual RooPlot *plotOn(RooPlot *frame, PlotOpt o) const;  

  friend class RooEffGenContext ;
  friend class RooAddGenContext ;
  friend class RooProdGenContext ;
  friend class RooSimGenContext ;
  friend class RooSimSplitGenContext ;
  friend class RooConvGenContext ;
  friend class RooSimultaneous ;
  friend class RooAddGenContextOrig ;
  friend class RooProdPdf ;
  friend class RooMCStudy ;

  Int_t* randomizeProtoOrder(Int_t nProto,Int_t nGen,Bool_t resample=kFALSE) const ;

  virtual RooAbsGenContext* binnedGenContext(const RooArgSet &vars, Bool_t verbose= kFALSE) const ;

  virtual RooAbsGenContext* genContext(const RooArgSet &vars, const RooDataSet *prototype=0, 
	                               const RooArgSet* auxProto=0, Bool_t verbose= kFALSE) const ;

  virtual RooAbsGenContext* autoGenContext(const RooArgSet &vars, const RooDataSet* prototype=0, const RooArgSet* auxProto=0, 
					   Bool_t verbose=kFALSE, Bool_t autoBinned=kTRUE, const char* binnedTag="") const ;


  friend class RooExtendPdf ;
  // This also forces the definition of a copy ctor in derived classes 
  RooAbsPdf(const RooAbsPdf& other, const char* name = 0);

  friend class RooRealIntegral ;
  static Int_t _verboseEval ;

  virtual Bool_t syncNormalization(const RooArgSet* dset, Bool_t adjustProxies=kTRUE) const ;

  friend class RooAbsAnaConvPdf ;
  mutable Double_t _rawValue ;
  mutable RooAbsReal* _norm   ;      //! Normalization integral (owned by _normMgr)
  mutable RooArgSet* _normSet ;      //! Normalization set with for above integral
  Int_t _minDimNormValueCache ;      // Minimum number of numerically integrated dimensions to activate normalization value caching
  Int_t _valueCacheIntOrder ;        // Interpolation order for numeric integral value cache

  class CacheElem : public RooAbsCacheElement {
  public:
    CacheElem(RooAbsReal& norm) : _norm(&norm) {} ;
    void operModeHook(RooAbsArg::OperMode) ;
    virtual ~CacheElem() ; 
    virtual RooArgList containedArgs(Action) { return RooArgList(*_norm) ; }
    RooAbsReal* _norm ;
  } ;
  mutable RooObjCacheManager _normMgr ; // The cache manager

  friend class CacheElem ; // Cache needs to be able to clear _norm pointer
  
  virtual Bool_t redirectServersHook(const RooAbsCollection&, Bool_t, Bool_t, Bool_t) { 
    // Hook function intercepting redirectServer calls. Discard current normalization
    // object if any server is redirected

    // Object is own by _normCacheManager that will delete object as soon as cache
    // is sterilized by server redirect
    _norm = 0 ;
    return kFALSE ; 
  } ;

  
  mutable Int_t _errorCount ;        // Number of errors remaining to print
  mutable Int_t _traceCount ;        // Number of traces remaining to print
  mutable Int_t _negCount ;          // Number of negative probablities remaining to print

  Bool_t _selectComp ;               // Component selection flag for RooAbsPdf::plotCompOn

  static void raiseEvalError() ;

  static Bool_t _evalError ;

  RooNumGenConfig* _specGeneratorConfig ; //! MC generator configuration specific for this object
  
  TString _normRange ; // Normalization range
  static TString _normRangeOverride ; 
  
  ClassDef(RooAbsPdf,3) // Abstract PDF with normalization support
};


#endif
