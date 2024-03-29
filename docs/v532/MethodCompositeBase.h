// @(#)root/tmva $Id: MethodCompositeBase.h 40005 2011-06-27 15:29:10Z stelzer $   
// Author: Andreas Hoecker, Joerg Stelzer, Helge Voss, Kai Voss,Or Cohen 

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : MethodCompositeBase                                                   *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Virtual base class for all MVA method                                     *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker <Andreas.Hocker@cern.ch> - CERN, Switzerland              *
 *      Joerg Stelzer   <Joerg.Stelzer@cern.ch>  - CERN, Switzerland              *
 *      Helge Voss      <Helge.Voss@cern.ch>     - MPI-K Heidelberg, Germany      *
 *      Kai Voss        <Kai.Voss@cern.ch>       - U. of Victoria, Canada         *
 *      Or Cohen        <orcohenor@gmail.com>    - Weizmann Inst., Israel         *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         * 
 *      U. of Victoria, Canada                                                    * 
 *      MPI-K Heidelberg, Germany                                                 * 
 *      LAPP, Annecy, France                                                      *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_MethodCompositeBase
#define ROOT_TMVA_MethodCompositeBase

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// MethodCompositeBase                                                  //
//                                                                      //
// Virtual base class for combining several TMVA method                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <iosfwd>
#include <vector>

#ifndef ROOT_TMVA_MethodBase
#include "TMVA/MethodBase.h"
#endif

namespace TMVA {
   class IMethod;

   class MethodCompositeBase : public MethodBase {

   public :
      MethodCompositeBase( const TString& jobName,
                           Types::EMVA methodType,
                           const TString& methodTitle,
                           DataSetInfo& theData,
                           const TString& theOption = "",
                           TDirectory* theTargetDir = NULL );


      MethodCompositeBase( Types::EMVA methodType,
                           DataSetInfo& dsi,
                           const TString& weightFile,
                           TDirectory* theBaseDir = 0 );

      using MethodBase::ReadWeightsFromStream;

      // write weights to file
      void AddWeightsXMLTo( void* parent ) const;
      void ReadWeightsFromXML( void* wghtnode );

      // calculate the MVA value combining all classifiers according to thier fMethodWeight
      Double_t GetMvaValue( Double_t* err = 0, Double_t* errUpper = 0 );

      using MethodBase::GetMvaValue;

      // read weights from file
      void ReadWeightsFromStream( istream& istr );

      // performs classifier training
      virtual void Train() = 0;

      // create ranking
      virtual const Ranking* CreateRanking() = 0;

      virtual ~MethodCompositeBase( void );

      UInt_t GetMethodIndex() { return fMethodIndex; }

      IMethod* GetLastMethod() { return fMethods.back(); }

      IMethod* GetPreviousMethod() { return (fMethodIndex>0)?fMethods[fMethodIndex-1]:0; }

      IMethod* GetCurrentMethod()
      { return (fMethodIndex>0)?fMethods[fMethodIndex]:0; }

   protected:

      void DeclareOptions() = 0;
      void ProcessOptions() = 0;

      IMethod* GetMethod( const TString& title ) const;  // accessor by name

      IMethod* GetMethod( const Int_t index ) const;  // accessor by index in vector

      //the index of the classifier currently boosted
      Int_t          fMethodIndex;

      std::vector<IMethod*>      fMethods;          // vector of all classifiers

      //the weight of every classifier used in the GetMVA method
      std::vector<Double_t>      fMethodWeight;

      ClassDef(MethodCompositeBase,0)

   };
}

#endif

