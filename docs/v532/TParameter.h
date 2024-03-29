// @(#)root/base:$Id: TParameter.h 40272 2011-07-18 11:12:14Z rdm $
// Author: Maarten Ballintijn   21/06/2004

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TParameter
#define ROOT_TParameter


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TParameter<AParamType>                                               //
//                                                                      //
// Named parameter, streamable and storable.                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_Riostream
#include "Riostream.h"
#endif

#ifndef ROOT_TClass
#include "TClass.h"
#endif

#ifndef ROOT_TObject
#include "TObject.h"
#endif

#ifndef ROOT_TCollection
#include "TCollection.h"
#endif

#ifndef ROOT_TString
#include "TString.h"
#endif

#ifndef ROOT_TROOT
#include "TROOT.h"
#endif

template <class AParamType>
class TParameter : public TObject {

public:
   enum EStatusBits { kMultiply = BIT(16) }; // Use multiplication while merging

private:
   TString     fName;
   AParamType  fVal;

public:
   TParameter() { /* leave fVal uninitialized */ }
   TParameter(const char *name, const AParamType &val)
             : fName(name), fVal(val) { ResetBit(kMultiply); }
   TParameter(const char *name, const AParamType &val, char mergemode)
             : fName(name), fVal(val) { SetMergeMode(mergemode); }
   ~TParameter() { }

   const char       *GetName() const { return fName; }
   const AParamType &GetVal() const { return fVal; }
   void              SetVal(const AParamType &val) { fVal = val; }
   void              SetMergeMode(char mergemode = '+') { ResetBit(kMultiply);
                                                          if (mergemode == '*') SetBit(kMultiply); }

   virtual ULong_t  Hash() const { return fName.Hash(); }
   virtual Bool_t   IsSortable() const { return kTRUE; }
   virtual Int_t    Compare(const TObject *obj) const {
      // Compare two TParameter objects. Returns 0 when equal, -1 when this is
      // smaller and +1 when bigger (like strcmp).
      
      if (this == obj) return 0;
      return fName.CompareTo(obj->GetName());
   }

   virtual void ls(Option_t *) const {
      // Print this parameter content
      TROOT::IndentLevel();
      cout << "OBJ: " << IsA()->GetName() << "\t" << fName << " = " << fVal << endl;
   }

   virtual void Print(Option_t *) const {
      // Print this parameter content
      TROOT::IndentLevel();
      cout << IsA()->GetName() << "\t" << fName << " = " << fVal << endl;
   }

   virtual Int_t Merge(TCollection *in);
   
   ClassDef(TParameter,2)  //Named templated parameter type
};

template <class AParamType>
inline Int_t TParameter<AParamType>::Merge(TCollection *in) {
   // Merge objects in the list.
   // Returns the number of objects that were in the list.
   TIter nxo(in);
   Int_t n = 0;
   while (TObject *o = nxo()) {
      TParameter<AParamType> *c = dynamic_cast<TParameter<AParamType> *>(o);
      if (c) {
         if (TestBit(kMultiply))
            fVal *= c->GetVal();
         else
            fVal += c->GetVal();
         n++;
      }
   }
   
   return n;
}

// Specialization of Merge for Bool_t  
template <>
inline Int_t TParameter<Bool_t>::Merge(TCollection *in)
{
   // Merge bool objects in the list.
   // Returns the number of objects that were in the list.
   TIter nxo(in);
   Int_t n = 0;
   while (TObject *o = nxo()) {
      TParameter<Bool_t> *c = dynamic_cast<TParameter<Bool_t> *>(o);
      if (c) {
         if (TestBit(TParameter::kMultiply))
            fVal &= (Bool_t) c->GetVal();
         else
            fVal |= (Bool_t) c->GetVal();
         n++;
      }
   }
   
   return n;
}

#endif
