// @(#)root/pyroot:$Id: Utility.h 38982 2011-04-20 23:00:56Z wlav $
// Author: Wim Lavrijsen, Apr 2004

#ifndef PYROOT_UTILITY_H
#define PYROOT_UTILITY_H

// ROOT
#include "DllImport.h"

// CINT
namespace Cint {
   class G__ClassInfo;
}

// Standard
#include <map>
#include <string>


namespace PyROOT {

   class PyCallable;

   typedef PyDictEntry* (*DictLookup_t) ( PyDictObject*, PyObject*, Long_t );
   R__EXTERN DictLookup_t gDictLookupOrg;
   R__EXTERN Bool_t gDictLookupActive;

// general place holder for function parameters
   union TParameter {
      Long_t      fl;
      ULong_t    ful;
      Long64_t   fll;
      ULong64_t full;
      Double_t    fd;
      void*       fv;
   };

// additional converter functions
   ULong_t PyLongOrInt_AsULong( PyObject* pyobject );
   ULong64_t PyLongOrInt_AsULong64( PyObject* pyobject );

   namespace Utility {

   // convenience functions for adding methods to classes
      Bool_t AddToClass( PyObject* pyclass, const char* label, PyCFunction cfunc,
                         int flags = METH_VARARGS );
      Bool_t AddToClass( PyObject* pyclass, const char* label, const char* func );
      Bool_t AddToClass( PyObject* pyclass, const char* label, PyCallable* pyfunc );

      Bool_t AddUsingToClass( PyObject* pyclass, const char* method );

   // helpers for dynamically constructing binary operators
      Bool_t AddBinaryOperator( PyObject* left, PyObject* right, const char* op, const char* label = "" );
      Bool_t AddBinaryOperator( PyObject* pyclass, const char* op, const char* label = "" );
      Bool_t AddBinaryOperator( PyObject* pyclass, const std::string& lcname, const std::string& rcname,
                                const char* op, const char* label = "" );

   // helper for template classes and methods
      Bool_t BuildTemplateName( PyObject*& pyname, PyObject* args, int argoff );

   // initialize proxy type objects
      Bool_t InitProxy( PyObject* module, PyTypeObject* pytype, const char* name );

   // retrieve the memory buffer from pyobject, return buflength, tc (optional) is python
   // array.array type code, size is type size, buf will point to buffer, and if check is
   // true, some heuristics will be applied to check buffer compatibility with the type
      int GetBuffer( PyObject* pyobject, char tc, int size, void*& buf, Bool_t check = kTRUE );

   // memory handling
      enum EMemoryPolicy { kHeuristics = 1, kStrict = 2 };
      R__EXTERN EMemoryPolicy gMemoryPolicy;
      Bool_t SetMemoryPolicy( EMemoryPolicy e );

   // signal safety
      enum ESignalPolicy { kFast = 1, kSafe = 2 };
      R__EXTERN ESignalPolicy gSignalPolicy;
      Bool_t SetSignalPolicy( ESignalPolicy e );

   // data/return types
      const int kPtrMask = 0x10000000;

      enum EDataType {
         kBool        = 0x00000001,
         kChar        = 0x00000002,
         kShort       = 0x00000004,
         kInt         = 0x00000008,
         kUInt        = 0x00000010,
         kLong        = 0x00000020,
         kULong       = 0x00000040,
         kFloat       = 0x00000080,
         kDouble      = 0x00000100,
         kVoid        = 0x00000200,
         kOther       = 0x00000400,
         kLongLong    = 0x00000800,
         kEnum        = 0x00001000,
         kSTLString   = 0x00002000,
         kMacro       = 0x00004000,
         kString      = kPtrMask | kChar,
         kDoublePtr   = kPtrMask | kDouble,
         kFloatPtr    = kPtrMask | kFloat,
         kLongPtr     = kPtrMask | kLong,
         kIntPtr      = kPtrMask | kInt,
         kVoidPtr     = kPtrMask | kVoid
      };

   // data/operator mappings
      std::string MapOperatorName( const std::string& name, Bool_t bTakesParames );

   // meta information
      EDataType EffectiveType( const std::string& name );
      const std::string Compound( const std::string& name );
      const std::string ClassName( PyObject* pyobj );
      const std::string ResolveTypedef( const std::string& name );

   // CINT integration
      void ErrMsgCallback( char* msg );
      void ErrMsgHandler( int level, Bool_t abort, const char* location, const char* msg );

   // install a python function with CINT; returns identifier
      Long_t InstallMethod( Cint::G__ClassInfo* scope, PyObject* callback,
         const std::string& mtName, const char* rtype, const char* signature,
         void* func, Int_t npar = 0, Long_t extra = 0 );

      PyObject* GetInstalledMethod( int tagnum, Long_t* extra = 0 );

   // for threading: save call to PyErr_Occurred()
      PyObject* PyErr_Occurred_WithGIL();

   } // namespace Utility

} // namespace PyROOT

#endif // !PYROOT_UTILITY_H
