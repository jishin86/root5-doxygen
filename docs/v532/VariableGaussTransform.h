// @(#)root/tmva $Id: VariableGaussTransform.h 40005 2011-06-27 15:29:10Z stelzer $
// Author: Andreas Hoecker, Joerg Stelzer, Helge Voss, Eckhard v. Toerne

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : VariableGaussTransform                                                *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Decorrelation of input variables                                          *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker <Andreas.Hocker@cern.ch> - CERN, Switzerland              *
 *      Joerg Stelzer   <Joerg.Stelzer@cern.ch>  - CERN, Switzerland              *
 *      Eckhard v. Toerne     <evt@uni-bonn.de>  - Uni Bonn, Germany              *
 *      Helge Voss      <Helge.Voss@cern.ch>     - MPI-K Heidelberg, Germany      *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         *
 *      U. of Victoria, Canada                                                    *
 *      MPI-K Heidelberg, Germany                                                 *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_VariableGaussTransform
#define ROOT_TMVA_VariableGaussTransform

#ifndef ROOT_TMVA_PDF
#include "TMVA/PDF.h"
#endif

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// VariableGaussTransform                                               //
//                                                                      //
// Gaussian transformation of input variables.                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <vector>

#ifndef ROOT_TH1
#include "TH1.h"
#endif
#ifndef ROOT_TGraph
#include "TGraph.h"
#endif
#ifndef ROOT_TSpline
#include "TSpline.h"
#endif
#ifndef ROOT_TDirectory
#include "TDirectory.h"
#endif
#ifndef ROOT_Event
#include "Event.h"
#endif

#ifndef ROOT_TMVA_VariableTransformBase
#include "TMVA/VariableTransformBase.h"
#endif

namespace TMVA {

   class TMVAGaussPair {

   public:

      TMVAGaussPair( Float_t f, Float_t w ): fF(f), fW(w) {}
      Bool_t  operator >  ( const TMVAGaussPair &p ) const { return fF >  p.fF; }
      Bool_t  operator <  ( const TMVAGaussPair &p ) const { return fF <  p.fF; }
      Bool_t  operator == ( const TMVAGaussPair &p ) const { return fF == p.fF; }
      Float_t GetValue() const { return fF; }
      Float_t GetWeight() const { return fW; }

   private:

      Float_t fF; // the float
      Float_t fW; // the event weight
   };


   class VariableGaussTransform : public VariableTransformBase {

   public:
  
      VariableGaussTransform( DataSetInfo& dsi, TString strcor=""  );
      virtual ~VariableGaussTransform( void );

      void   Initialize();
      Bool_t PrepareTransformation( const std::vector<Event*>& );

      virtual const Event* Transform(const Event* const, Int_t cls ) const;
      virtual const Event* InverseTransform(const Event* const, Int_t cls ) const;

      void WriteTransformationToStream ( std::ostream& ) const;
      void ReadTransformationFromStream( std::istream&, const TString& );
     
      virtual void AttachXMLTo(void* parent);
      virtual void ReadFromXML( void* trfnode );

      virtual void PrintTransformation( ostream & o );

      // writer of function code
      virtual void MakeFunction( std::ostream& fout, const TString& fncName, Int_t part, UInt_t trCounter, Int_t cls );
    
   private:

      Bool_t           fFlatNotGauss;
      Int_t            fPdfMinSmooth;
      Int_t            fPdfMaxSmooth;
      //      mutable Event*   fTransformedEvent;

      std::vector< std::vector< TH1F* > >      fCumulativeDist;   //! The Cummulative distributions 
      //std::vector< std::vector< TGraph* > >    fCumulativeGraph;  //! The Cummulative distributions 
      //std::vector< std::vector< TSpline3* > >  fCumulativeSpline; //! The Cummulative distributions 
      std::vector< std::vector< PDF*> >         fCumulativePDF;    //  The cumulative PDF

      void GetCumulativeDist( const std::vector<Event*>& );
      void CleanUpCumulativeArrays(TString opt = "ALL");

      // needed for backward compatibility
      UInt_t fElementsperbin;  // av number of events stored per bin in cum dist
      Double_t OldCumulant(Float_t x, TH1* h ) const;

      ClassDef(VariableGaussTransform,0) // Variable transformation: Gauss transformation
   };

} // namespace TMVA

#endif 
