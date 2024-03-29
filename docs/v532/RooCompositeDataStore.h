/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooCompositeDataStore.h 42179 2011-11-22 22:03:01Z wouter $
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
#ifndef ROO_COMPOSITE_DATA_STORE
#define ROO_COMPOSITE_DATA_STORE

#include "RooAbsDataStore.h" 
#include "TString.h"
#include <map>
#include <string>

class RooAbsArg ;
class RooArgList ;
class RooFormulaVar ;
class RooArgSet ;
class RooCategory ;


class RooCompositeDataStore : public RooAbsDataStore {
public:

  RooCompositeDataStore() ; 

  // Ctors from DataStore
  RooCompositeDataStore(const char* name, const char* title, const RooArgSet& vars, RooCategory& indexCat, std::map<std::string,RooAbsDataStore*> inputData) ;

  // Empty ctor
  virtual RooAbsDataStore* clone(const char* newname=0) const { return new RooCompositeDataStore(*this,newname) ; }
  virtual RooAbsDataStore* clone(const RooArgSet& vars, const char* newname=0) const { return new RooCompositeDataStore(*this,vars,newname) ; }

  RooCompositeDataStore(const RooCompositeDataStore& other, const char* newname=0) ;
  RooCompositeDataStore(const RooCompositeDataStore& other, const RooArgSet& vars, const char* newname=0) ;
  virtual ~RooCompositeDataStore() ;

  virtual void dump() ;

  // Write current row
  virtual Int_t fill() ;

  virtual Double_t sumEntries() const ;

  // Retrieve a row
  using RooAbsDataStore::get ;
  virtual const RooArgSet* get(Int_t index) const ;
  virtual Double_t weight() const ;
  virtual Double_t weight(Int_t index) const ;
  virtual Double_t weightError(RooAbsData::ErrorType etype=RooAbsData::Poisson) const ;
  virtual void weightError(Double_t& lo, Double_t& hi, RooAbsData::ErrorType etype=RooAbsData::Poisson) const ; 
  virtual Bool_t isWeighted() const ;

  // Change observable name
  virtual Bool_t changeObservableName(const char* from, const char* to) ;
  
  // Add one or more columns
  virtual RooAbsArg* addColumn(RooAbsArg& var, Bool_t adjustRange=kTRUE) ;
  virtual RooArgSet* addColumns(const RooArgList& varList) ;

  // Merge column-wise
  RooAbsDataStore* merge(const RooArgSet& allvars, std::list<RooAbsDataStore*> dstoreList) ;

  RooCategory* index() { return _indexCat ; }

  // Add rows 
  virtual void append(RooAbsDataStore& other) ;

  // General & bookkeeping methods
  virtual Bool_t valid() const ;
  virtual Int_t numEntries() const ;
  virtual void reset() ;

  // Buffer redirection routines used in inside RooAbsOptTestStatistics
  virtual void attachBuffers(const RooArgSet& extObs) ; 
  virtual void resetBuffers() ;
   
  // Constant term  optimizer interface
  virtual void cacheArgs(const RooAbsArg* owner, RooArgSet& varSet, const RooArgSet* nset=0) ;
  virtual const RooAbsArg* cacheOwner() { return 0 ; }
  virtual void setArgStatus(const RooArgSet& set, Bool_t active) ;
  virtual void resetCache() ;

  void loadValues(const RooAbsDataStore *tds, const RooFormulaVar* select=0, const char* rangeName=0, Int_t nStart=0, Int_t nStop=2000000000) ;
  
 protected:

  void attachCache(const RooAbsArg* newOwner, const RooArgSet& cachedVars) ;

  std::map<Int_t,RooAbsDataStore*> _dataMap ;
  RooCategory* _indexCat ;
  mutable RooAbsDataStore* _curStore ; //! Datastore associated with current event
  mutable Int_t _curIndex ; //! Index associated with current event
  Bool_t _ownComps ; //! 

  ClassDef(RooCompositeDataStore,1) // Composite Data Storage class
};


#endif
