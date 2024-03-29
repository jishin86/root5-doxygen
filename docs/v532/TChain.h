// @(#)root/tree:$Id: TChain.h 40993 2011-09-23 18:54:14Z pcanal $
// Author: Rene Brun   03/02/97

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TChain
#define ROOT_TChain


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TChain                                                               //
//                                                                      //
// A chain of TTrees.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TTree
#include "TTree.h"
#endif

class TFile;
class TBrowser;
class TCut;
class TEntryList;
class TEventList;
class TCollection;

class TChain : public TTree {

protected:
   Int_t        fTreeOffsetLen;    //  Current size of fTreeOffset array
   Int_t        fNtrees;           //  Number of trees
   Int_t        fTreeNumber;       //! Current Tree number in fTreeOffset table
   Long64_t    *fTreeOffset;       //[fTreeOffsetLen] Array of variables
   Bool_t       fCanDeleteRefs;    //! If true, TProcessIDs are deleted when closing a file
   TTree       *fTree;             //! Pointer to current tree (Note: We do *not* own this tree.)
   TFile       *fFile;             //! Pointer to current file (We own the file).
   TObjArray   *fFiles;            //-> List of file names containing the trees (TChainElement, owned)
   TList       *fStatus;           //-> List of active/inactive branches (TChainElement, owned)
   TChain      *fProofChain;       //! chain proxy when going to be processed by PROOF

private:
   TChain(const TChain&);            // not implemented
   TChain& operator=(const TChain&); // not implemented

protected:
   void ReleaseChainProof();

public:
   // TChain constants
   enum {
      kGlobalWeight   = BIT(15),
      kAutoDelete     = BIT(16),
      kProofUptodate  = BIT(17),
      kProofLite      = BIT(18),
      kBigNumber      = 1234567890
   };

public:
   TChain();
   TChain(const char* name, const char* title = "");
   virtual ~TChain();

   virtual Int_t     Add(TChain* chain);
   virtual Int_t     Add(const char* name, Long64_t nentries = kBigNumber);
   virtual Int_t     AddFile(const char* name, Long64_t nentries = kBigNumber, const char* tname = "");
   virtual Int_t     AddFileInfoList(TCollection* list, Long64_t nfiles = kBigNumber);
   virtual TFriendElement *AddFriend(const char* chainname, const char* dummy = "");
   virtual TFriendElement *AddFriend(const char* chainname, TFile* dummy);
   virtual TFriendElement *AddFriend(TTree* chain, const char* alias = "", Bool_t warn = kFALSE);
   virtual void      Browse(TBrowser*);
   virtual void      CanDeleteRefs(Bool_t flag = kTRUE);
   virtual void      CreatePackets();
   virtual void      DirectoryAutoAdd(TDirectory *);
   virtual Long64_t  Draw(const char* varexp, const TCut& selection, Option_t* option = "", Long64_t nentries = kBigNumber, Long64_t firstentry = 0);
   virtual Long64_t  Draw(const char* varexp, const char* selection, Option_t* option = "", Long64_t nentries = kBigNumber, Long64_t firstentry = 0); // *MENU*
   virtual void      Draw(Option_t* opt) { Draw(opt, "", "", 1000000000, 0); }
   virtual Int_t     Fill() { MayNotUse("Fill()"); return -1; }
   virtual TBranch  *FindBranch(const char* name);
   virtual TLeaf    *FindLeaf(const char* name);
   virtual TBranch  *GetBranch(const char* name);
   virtual Bool_t    GetBranchStatus(const char* branchname) const;
   virtual Long64_t  GetChainEntryNumber(Long64_t entry) const;
   virtual TClusterIterator GetClusterIterator(Long64_t firstentry);
           Int_t     GetNtrees() const { return fNtrees; }
   virtual Long64_t  GetEntries() const;
   virtual Long64_t  GetEntries(const char *sel) { return TTree::GetEntries(sel); }
   virtual Int_t     GetEntry(Long64_t entry=0, Int_t getall=0);
   virtual Long64_t  GetEntryNumber(Long64_t entry) const;
   virtual Int_t     GetEntryWithIndex(Int_t major, Int_t minor=0);
   TFile            *GetFile() const;
   virtual TLeaf    *GetLeaf(const char* branchname, const char* leafname);
   virtual TLeaf    *GetLeaf(const char* name);
   virtual TObjArray *GetListOfBranches();
   //                Warning, GetListOfFiles returns the list of TChainElements (not the list of files)
   //                see TChain::AddFile to see how to get the corresponding TFile objects
   TObjArray        *GetListOfFiles() const {return fFiles;}
   virtual TObjArray *GetListOfLeaves();
   virtual const char *GetAlias(const char *aliasName) const;
   virtual Double_t  GetMaximum(const char *columname);
   virtual Double_t  GetMinimum(const char *columname);
   virtual Int_t     GetNbranches();
   virtual Long64_t  GetReadEntry() const;
   TList            *GetStatus() const { return fStatus; }
   virtual TTree    *GetTree() const { return fTree; }
   virtual Int_t     GetTreeNumber() const { return fTreeNumber; }
           Long64_t *GetTreeOffset() const { return fTreeOffset; }
           Int_t     GetTreeOffsetLen() const { return fTreeOffsetLen; }
   virtual Double_t  GetWeight() const;
   virtual Int_t     LoadBaskets(Long64_t maxmemory);
   virtual Long64_t  LoadTree(Long64_t entry);
           void      Lookup(Bool_t force = kFALSE);
   virtual void      Loop(Option_t *option="", Long64_t nentries=kBigNumber, Long64_t firstentry=0); // *MENU*
   virtual void      ls(Option_t *option="") const;
   virtual Long64_t  Merge(const char *name, Option_t *option = "");
   virtual Long64_t  Merge(TCollection *list, Option_t *option = "");
   virtual Long64_t  Merge(TCollection *list, TFileMergeInfo *info);
   virtual Long64_t  Merge(TFile *file, Int_t basketsize, Option_t *option="");
   virtual void      Print(Option_t *option="") const;
   virtual Long64_t  Process(const char *filename, Option_t *option="", Long64_t nentries=kBigNumber, Long64_t firstentry=0); // *MENU*
#if defined(__CINT__)
#if defined(R__MANUAL_DICT)
   virtual Long64_t  Process(void* selector, Option_t* option = "", Long64_t nentries = kBigNumber, Long64_t firstentry = 0);
#endif
#else
   virtual Long64_t  Process(TSelector* selector, Option_t* option = "", Long64_t nentries = kBigNumber, Long64_t firstentry = 0);
#endif
   virtual void      RecursiveRemove(TObject *obj);
   virtual void      Reset(Option_t *option="");
   virtual void      ResetAfterMerge(TFileMergeInfo *);
   virtual void      ResetBranchAddress(TBranch *);
   virtual void      ResetBranchAddresses();
   virtual Long64_t  Scan(const char *varexp="", const char *selection="", Option_t *option="", Long64_t nentries=1000000000, Long64_t firstentry=0); // *MENU*
   virtual void      SetAutoDelete(Bool_t autodel=kTRUE);
   virtual Int_t     SetBranchAddress(const char *bname,void *add, TBranch **ptr = 0);
   virtual Int_t     SetBranchAddress(const char *bname,void *add, TBranch **ptr, TClass *realClass, EDataType datatype, Bool_t isptr);
   virtual Int_t     SetBranchAddress(const char *bname,void *add, TClass *realClass, EDataType datatype, Bool_t isptr);

   virtual void      SetBranchStatus(const char *bname, Bool_t status=1, UInt_t *found=0);
   virtual void      SetDirectory(TDirectory *dir);
   virtual void      SetEntryList(TEntryList *elist, Option_t *opt="");
   virtual void      SetEntryListFile(const char *filename="", Option_t *opt="");
   virtual void      SetEventList(TEventList *evlist);
   virtual void      SetMakeClass(Int_t make) { TTree::SetMakeClass(make); if (fTree) fTree->SetMakeClass(make);}
   virtual void      SetPacketSize(Int_t size = 100);
   virtual void      SetProof(Bool_t on = kTRUE, Bool_t refresh = kFALSE, Bool_t gettreeheader = kFALSE);
   virtual void      SetWeight(Double_t w=1, Option_t *option="");
   virtual void      UseCache(Int_t maxCacheSize = 10, Int_t pageSize = 0);

   ClassDef(TChain,5)  //A chain of TTrees
};

#endif // ROOT_TChain
