// @(#)root/cont:$Id: TVirtualCollectionIterators.h 40839 2011-09-10 15:11:01Z pcanal $
// Author: Philippe Canal 20/08/2010

/*************************************************************************
 * Copyright (C) 1995-2003, Rene Brun, Fons Rademakers and al.           *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TVirtualCollectionIterators
#define ROOT_TVirtualCollectionIterators

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TVirtualCollectionIterators                                          //
//                                                                      //
// Small helper class to generically acquire and release iterators      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TVirtualCollectionProxy
#include "TVirtualCollectionProxy.h"
#endif
#ifndef ROOT_TError
#include "TError.h"
#endif

class TVirtualCollectionIterators
{
private:
   TVirtualCollectionIterators(); // Intentionally unimplemented.
   TVirtualCollectionIterators(const TVirtualCollectionIterators&); // Intentionally unimplemented.
   
public:
   // Note when the collection is a vector, fBegin and fEnd points to
   // the start and end of the memory content rather than to the address
   // of iterators (saving one dereference when being used).
   
   typedef TVirtualCollectionProxy::CreateIterators_t CreateIterators_t;
   typedef TVirtualCollectionProxy::DeleteTwoIterators_t DeleteTwoIterators_t;
   
   char  fBeginBuffer[TVirtualCollectionProxy::fgIteratorArenaSize];
   char  fEndBuffer[TVirtualCollectionProxy::fgIteratorArenaSize];
   void *fBegin; // Pointer to the starting iterator (collection->begin())
   void *fEnd;   // Pointer to the ending iterator (collection->end())
   CreateIterators_t    fCreateIterators;
   DeleteTwoIterators_t fDeleteTwoIterators;
   
   TVirtualCollectionIterators(TVirtualCollectionProxy *proxy, bool read = kTRUE) : fBegin( &(fBeginBuffer[0]) ), fEnd(&(fEndBuffer[0])), fCreateIterators(0), fDeleteTwoIterators(0) 
   {
      //         memset(fBeginBuffer,0,TVirtualCollectionProxy::fgIteratorArenaSize);
      //         memset(fEndBuffer,0,TVirtualCollectionProxy::fgIteratorArenaSize);
      if (proxy) {
         fCreateIterators = proxy->GetFunctionCreateIterators(read);
         fDeleteTwoIterators = proxy->GetFunctionDeleteTwoIterators(read);
      } else {
         ::Fatal("TIterators::TIterators","Created with out a collection proxy!\n");
      }
   }
   TVirtualCollectionIterators(CreateIterators_t creator, DeleteTwoIterators_t destruct) : fBegin( &(fBeginBuffer[0]) ), fEnd(&(fEndBuffer[0])), fCreateIterators(creator), fDeleteTwoIterators(destruct) 
   {
   }
   
   inline void CreateIterators(void *collection)
   {
      // Initialize the fBegin and fEnd iterators.
      
      fCreateIterators(collection, &fBegin, &fEnd);
   }
   
   inline ~TVirtualCollectionIterators() 
   {
      if (fBegin != &(fBeginBuffer[0])) {
         // assert(end != endbuf);
         fDeleteTwoIterators(fBegin,fEnd);
      }      
   }
};

class TVirtualCollectionPtrIterators
{
public:
   typedef TVirtualCollectionProxy::Next_t Next_t;
   typedef TVirtualCollectionProxy::CopyIterator_t Copy_t;
   typedef TVirtualCollectionProxy::CreateIterators_t CreateIterators_t;
   typedef TVirtualCollectionProxy::DeleteIterator_t Delete_t;
   typedef TVirtualCollectionProxy::DeleteTwoIterators_t DeleteTwoIterators_t;
   
private:
   TVirtualCollectionPtrIterators(); // Intentionally unimplemented.
   TVirtualCollectionPtrIterators(const TVirtualCollectionPtrIterators&); // Intentionally unimplemented.
   
   CreateIterators_t    fCreateIterators;
   DeleteTwoIterators_t fDeleteTwoIterators;
   
   Bool_t fAllocated;

   char  fRawBeginBuffer[TVirtualCollectionProxy::fgIteratorArenaSize];
   char  fRawEndBuffer[TVirtualCollectionProxy::fgIteratorArenaSize];

   struct TInternalIterator {
      TInternalIterator() : fCopy(0),fDelete(0),fNext(0),fIter(0) {}
      TInternalIterator(const TInternalIterator &source) : fCopy(source.fCopy),fDelete(source.fDelete),fNext(source.fNext),fIter(0) {}

      Copy_t    fCopy;
      Delete_t  fDelete;
      Next_t    fNext;

      void     *fIter;
   };
   
   TInternalIterator fBeginBuffer;
   TInternalIterator fEndBuffer;

public:
   // Note when the collection is a vector, fBegin and fEnd points to
   // the start and end of the memory content rather than to the address
   // of iterators (saving one dereference when being used).
   
   void *fBegin; // Pointer to the starting iterator (collection->begin())
   void *fEnd;   // Pointer to the ending iterator (collection->end())
   
   TVirtualCollectionPtrIterators(TVirtualCollectionProxy *proxy) : fCreateIterators(0), fDeleteTwoIterators(0), fAllocated(kFALSE),
                                                                    fBegin( &(fRawBeginBuffer[0]) ),
                                                                    fEnd( &(fRawEndBuffer[0]) )
   {
      //         memset(fBeginBuffer,0,TVirtualCollectionProxy::fgIteratorArenaSize);
      //         memset(fEndBuffer,0,TVirtualCollectionProxy::fgIteratorArenaSize);
      if (proxy) {
         fCreateIterators = proxy->GetFunctionCreateIterators();
         fDeleteTwoIterators = proxy->GetFunctionDeleteTwoIterators();
         
         fEndBuffer.fCopy = fBeginBuffer.fCopy = proxy->GetFunctionCopyIterator();
         fEndBuffer.fNext = fBeginBuffer.fNext = proxy->GetFunctionNext();
         fEndBuffer.fDelete = fBeginBuffer.fDelete = proxy->GetFunctionDeleteIterator();
      } else {
         ::Fatal("TIterators::TIterators","Created with out a collection proxy!\n");
      }
   }
   
   inline void CreateIterators(void *collection)
   {
      // Initialize the fBegin and fEnd iterators.
      
      fBegin = &(fRawBeginBuffer[0]);
      fEnd = &(fRawEndBuffer[0]);
      fCreateIterators(collection, &fBegin, &fEnd);
      if (fBegin != &(fRawBeginBuffer[0])) {
         // The iterator where too large to buffer in the  buffer
         fAllocated = kTRUE;
      }
      fBeginBuffer.fIter = fBegin;
      fEndBuffer.fIter = fEnd;
      fBegin = &fBeginBuffer;
      fEnd = &fEndBuffer;
   }
   
   inline ~TVirtualCollectionPtrIterators() 
   {
      if (fAllocated) {
         // assert(end != endbuf);
         fDeleteTwoIterators(fBeginBuffer.fIter,fEndBuffer.fIter);
      }
   }
   
   static void *Next(void *iter, const void *end) 
   {
      TInternalIterator *internal_iter = (TInternalIterator*) iter;
      TInternalIterator *internal_end = (TInternalIterator*) end;
      
      void **ptr = (void**)internal_iter->fNext(internal_iter->fIter,internal_end->fIter);
      if(ptr) return *ptr;
      else return 0;
   }
   
   static void DeleteIterator(void *iter)
   {
      TInternalIterator *internal_iter = (TInternalIterator*) iter;
      if (internal_iter->fDelete) {
         internal_iter->fDelete(internal_iter->fIter);
      }
   }
   
   static void *CopyIterator(void *dest, const void *source)
   {
      TInternalIterator *internal_source = (TInternalIterator*)source;
      TInternalIterator *internal_dest = new TInternalIterator(*internal_source);
      
      void *newiter = internal_source->fCopy(dest,internal_source->fIter);
      if (newiter == dest) {
         internal_dest->fDelete = 0;
      }
      internal_dest->fIter = newiter;
      return internal_dest;
   }
};

// Specialization of TVirtualCollectionIterators when we know the collection
// to be a vector (hence there is nothing to delete at the end).
struct TVirtualVectorIterators
{
private:
   TVirtualVectorIterators(const TVirtualVectorIterators&); // Intentionally unimplemented.
   
public:
   // Note when the collection is a vector, fBegin and fEnd points to
   // the start and end of the memory content rather than to the address
   // of iterators (saving one dereference when being used).
   
   typedef TVirtualCollectionProxy::CreateIterators_t CreateIterators_t;

   void *fBegin; // Pointer to the starting iterator (collection->begin())
   void *fEnd;   // Pointer to the ending iterator (collection->end())
   
   TVirtualVectorIterators(TVirtualCollectionProxy * /* proxy */) : fBegin(0), fEnd(0)
   {
      // fCreateIterators = proxy->GetFunctionCreateIterators();
   }
   
   TVirtualVectorIterators(CreateIterators_t /* creator */) : fBegin(0), fEnd(0)
   {
      // fCreateIterators = creator;
   }
   
   TVirtualVectorIterators() : fBegin(0), fEnd(0) 
   {
      // Default constructor.
   }
   
   inline void CreateIterators(void *collection)
   {
      // Initialize the fBegin and fEnd iterators.
      
      // We can safely assume that the std::vector layout does not really depend on
      // the content!
      std::vector<char> *vec = (std::vector<char>*)collection;
      if (vec->empty()) {
         fBegin = 0;
         fEnd = 0;
         return;
      }
      fBegin= &(*vec->begin());
#ifdef R__VISUAL_CPLUSPLUS
      fEnd = &(*(vec->end()-1)) + 1; // On windows we can not dererence the end iterator at all.
#else
      // coverity[past_the_end] Safe on other platforms
      fEnd = &(*vec->end());
#endif
      //fCreateIterators(collection, &fBegin, &fEnd);
   }
};

#endif // ROOT_TVirtualCollectionIterators

