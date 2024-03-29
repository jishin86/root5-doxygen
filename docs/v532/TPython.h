// @(#)root/pyroot:$Id: TPython.h 23159 2008-04-11 20:13:29Z wlav $
// Author: Wim Lavrijsen   April 2004

#ifndef ROOT_TPython
#define ROOT_TPython

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// TPython                                                                  //
//                                                                          //
// Access to the python interpreter and API onto PyROOT.                    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


// Bindings
#include "TPyReturn.h"

// ROOT
#ifndef ROOT_TObject
#include "TObject.h"
#endif


class TPython {

private:
   static Bool_t Initialize();

public:
// load a python script as if it were a macro
   static void LoadMacro( const char* name );

// execute a python stand-alone script, with argv CLI arguments
   static void ExecScript( const char* name, int argc = 0, const char** argv = 0 );

// execute a python statement (e.g. "import ROOT" )
   static Bool_t Exec( const char* cmd );

// evaluate a python expression (e.g. "1+1")
   static const TPyReturn Eval( const char* expr );

// bind a ROOT object with, at the python side, the name "label"
   static Bool_t Bind( TObject* object, const char* label );

// enter an interactive python session (exit with ^D)
   static void Prompt();

// type verifiers for ObjectProxy
   static Bool_t ObjectProxy_Check( PyObject* pyobject );
   static Bool_t ObjectProxy_CheckExact( PyObject* pyobject );

// object proxy to void* conversion
   static void* ObjectProxy_AsVoidPtr( PyObject* pyobject );

// void* to object proxy conversion, returns a new reference
   static PyObject* ObjectProxy_FromVoidPtr( void* addr, const char* classname );

   virtual ~TPython() { }
   ClassDef(TPython,0)   //Access to the python interpreter
};

#endif
