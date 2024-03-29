// @(#)root/tree:$Id: TSelector.h 38869 2011-04-15 11:29:04Z pcanal $
// Author: Rene Brun   05/02/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TSelector
#define ROOT_TSelector


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSelector                                                            //
//                                                                      //
// A utility class for Trees selections.                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef ROOT_TString
#include "TString.h"
#endif
#ifndef ROOT_TSelectorList
#include "TSelectorList.h"
#endif


class TTree;


class TSelector : public TObject {

public:
   enum EAbort { kContinue, kAbortProcess, kAbortFile };

protected:
   Long64_t       fStatus;  //selector status
   EAbort         fAbort;   //abort status
   TString        fOption;  //option given to TTree::Process
   TObject       *fObject;  //current object if processing object (vs. TTree)
   TList         *fInput;   //list of objects available during processing
   TSelectorList *fOutput;  //list of objects created during processing

private:
   TSelector(const TSelector&);            // not implemented
   TSelector& operator=(const TSelector&); // not implemented

public:
   TSelector();
   virtual            ~TSelector();

   virtual int         Version() const { return 0; }
   virtual void        Init(TTree *) { }
   virtual void        Begin(TTree *) { }
   virtual void        SlaveBegin(TTree *) { }
   virtual Bool_t      Notify() { return kTRUE; }
   virtual const char *GetOption() const { return fOption; }
   virtual Long64_t    GetStatus() const { return fStatus; }
   virtual Int_t       GetEntry(Long64_t /*entry*/, Int_t /*getall*/ = 0) { return 0; }
   virtual Bool_t      ProcessCut(Long64_t /*entry*/) { return kTRUE; }
   virtual void        ProcessFill(Long64_t /*entry*/) { }
   virtual Bool_t      Process(Long64_t /*entry*/) { return kFALSE; }
   virtual void        SetOption(const char *option) { fOption = option; }
   virtual void        SetObject(TObject *obj) { fObject = obj; }
   virtual void        SetInputList(TList *input) { fInput = input; }
   virtual void        SetStatus(Long64_t status) { fStatus = status; }
   virtual TList      *GetOutputList() const { return fOutput; }
   virtual void        SlaveTerminate() { }
   virtual void        Terminate() { }
   virtual void        Abort(const char *why, EAbort what = kAbortProcess);
   virtual EAbort      GetAbort() const { return fAbort; }
   virtual void        ResetAbort() { fAbort = kContinue; }

   static  TSelector  *GetSelector(const char *filename);
   static  Bool_t      IsStandardDraw(const char *selec);

   ClassDef(TSelector,0)  //A utility class for tree and object processing
};

#endif

