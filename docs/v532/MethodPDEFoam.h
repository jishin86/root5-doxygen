// @(#)root/tmva $Id: MethodPDEFoam.h 40005 2011-06-27 15:29:10Z stelzer $
// Author: Tancredi Carli, Dominik Dannheim, Alexander Voigt

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate Data analysis       *
 * Package: TMVA                                                                  *
 * Class  : MethodPDEFoam                                                         *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      The PDEFoam method is an extension of the PDERS method, which divides     *
 *      the multi-dimensional phase space in a finite number of hyper-rectangles  *
 *      (cells) of constant event density. This "foam" of cells is filled with    *
 *      averaged probability-density information sampled from a training event    *
 *      sample.                                                                   *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Tancredi Carli   - CERN, Switzerland                                      *
 *      Dominik Dannheim - CERN, Switzerland                                      *
 *      Peter Speckmayer <peter.speckmayer@cern.ch>  - CERN, Switzerland          *
 *      Alexander Voigt  - TU Dresden, Germany                                    *
 *                                                                                *
 * Original author of the TFoam implementation:                                   *
 *      S. Jadach - Institute of Nuclear Physics, Cracow, Poland                  *
 *                                                                                *
 * Copyright (c) 2008, 2010:                                                      *
 *      CERN, Switzerland                                                         *
 *      MPI-K Heidelberg, Germany                                                 *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_MethodPDEFoam
#define ROOT_TMVA_MethodPDEFoam

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// MethodPDEFoam                                                            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TRandom3
#include "TRandom3.h"
#endif

#ifndef ROOT_TMVA_MethodBase
#include "TMVA/MethodBase.h"
#endif

#ifndef ROOT_TMVA_PDEFoam
#include "TMVA/PDEFoam.h"
#endif

#ifndef ROOT_TMVA_PDEFoamDecisionTree
#include "TMVA/PDEFoamDecisionTree.h"
#endif
#ifndef ROOT_TMVA_PDEFoamEvent
#include "TMVA/PDEFoamEvent.h"
#endif
#ifndef ROOT_TMVA_PDEFoamDiscriminant
#include "TMVA/PDEFoamDiscriminant.h"
#endif
#ifndef ROOT_TMVA_PDEFoamTarget
#include "TMVA/PDEFoamTarget.h"
#endif
#ifndef ROOT_TMVA_PDEFoamMultiTarget
#include "TMVA/PDEFoamMultiTarget.h"
#endif

#ifndef ROOT_TMVA_PDEFoamDensityBase
#include "TMVA/PDEFoamDensityBase.h"
#endif
#ifndef ROOT_TMVA_PDEFoamTargetDensity
#include "TMVA/PDEFoamTargetDensity.h"
#endif
#ifndef ROOT_TMVA_PDEFoamEventDensity
#include "TMVA/PDEFoamEventDensity.h"
#endif
#ifndef ROOT_TMVA_PDEFoamDiscriminantDensity
#include "TMVA/PDEFoamDiscriminantDensity.h"
#endif
#ifndef ROOT_TMVA_PDEFoamDecisionTreeDensity
#include "TMVA/PDEFoamDecisionTreeDensity.h"
#endif

#ifndef ROOT_TMVA_PDEFoamKernelBase
#include "TMVA/PDEFoamKernelBase.h"
#endif
#ifndef ROOT_TMVA_PDEFoamKernelTrivial
#include "TMVA/PDEFoamKernelTrivial.h"
#endif
#ifndef ROOT_TMVA_PDEFoamKernelLinN
#include "TMVA/PDEFoamKernelLinN.h"
#endif
#ifndef ROOT_TMVA_PDEFoamKernelGauss
#include "TMVA/PDEFoamKernelGauss.h"
#endif

namespace TMVA {

   class MethodPDEFoam : public MethodBase {

   public:

      // kernel types
      typedef enum EKernel { kNone=0, kGaus=1, kLinN=2 } EKernel;

      MethodPDEFoam( const TString& jobName,
                     const TString& methodTitle,
                     DataSetInfo& dsi,
                     const TString& theOption = "PDEFoam",
                     TDirectory* theTargetDir = 0 );

      MethodPDEFoam( DataSetInfo& dsi,
                     const TString& theWeightFile,
                     TDirectory* theTargetDir = NULL );

      virtual ~MethodPDEFoam( void );

      virtual Bool_t HasAnalysisType( Types::EAnalysisType type, UInt_t numberClasses, UInt_t numberTargets );

      // training methods
      void Train( void );
      void TrainMonoTargetRegression( void );    // Regression output: one value
      void TrainMultiTargetRegression( void );   // Regression output: any number of values
      void TrainSeparatedClassification( void ); // Classification: one foam for Sig, one for Bg
      void TrainUnifiedClassification( void );   // Classification: one foam for Signal and Bg
      void TrainMultiClassification();           // Classification: one foam for every class

      using MethodBase::ReadWeightsFromStream;

      // write weights to stream
      void AddWeightsXMLTo( void* parent ) const;

      // read weights from stream
      void ReadWeightsFromStream( std::istream & i );
      void ReadWeightsFromXML   ( void* wghtnode );

      // write/read pure foams to/from file
      void WriteFoamsToFile() const;
      void ReadFoamsFromFile();
      PDEFoam* ReadClonedFoamFromFile(TFile*, const TString&);

      // calculate the MVA value
      Double_t GetMvaValue( Double_t* err = 0, Double_t* errUpper = 0 );

      // calculate multiclass MVA values
      const std::vector<Float_t>& GetMulticlassValues();

      // regression procedure
      virtual const std::vector<Float_t>& GetRegressionValues();

      // reset the method
      virtual void Reset();

      // ranking of input variables
      const Ranking* CreateRanking();

      // get number of cuts in every dimension, starting at cell
      void GetNCuts(PDEFoamCell *cell, std::vector<UInt_t> &nCuts);

      // helper functions to convert enum types to UInt_t and back
      EKernel GetKernel( void ) { return fKernel; }
      UInt_t KernelToUInt(EKernel ker) const { return UInt_t(ker); }
      EKernel UIntToKernel(UInt_t iker);
      UInt_t TargetSelectionToUInt(ETargetSelection ts) const { return UInt_t(ts); }
      ETargetSelection UIntToTargetSelection(UInt_t its);

   protected:

      // make ROOT-independent C++ class for classifier response (classifier-specific implementation)
      void MakeClassSpecific( std::ostream&, const TString& ) const;

      // get help message text
      void GetHelpMessage() const;

      // calculate Xmin and Xmax for Foam
      void CalcXminXmax();

      // Set Xmin, Xmax in foam with index 'foam_index'
      void SetXminXmax(TMVA::PDEFoam*);

      // create foam and set foam options
      PDEFoam* InitFoam(TString, EFoamType, UInt_t cls=0);

      // create pdefoam kernel
      PDEFoamKernelBase* CreatePDEFoamKernel();

      // delete all trained foams
      void DeleteFoams();

      // fill variable names into foam
      void FillVariableNamesToFoam() const;

   private:

      // the option handling methods
      void DeclareOptions();
      void DeclareCompatibilityOptions();
      void ProcessOptions();

      // nice output
      void PrintCoefficients( void );

      // Square function (fastest implementation)
      template<typename T> T Sqr(T x) const { return x*x; }

      // options to be used
      Bool_t        fSigBgSeparated;  // Separate Sig and Bg, or not
      Float_t       fFrac;            // Fraction used for calc of Xmin, Xmax
      Float_t       fDiscrErrCut;     // cut on discrimant error
      Float_t       fVolFrac;         // volume fraction (used for density calculation during buildup)
      Int_t         fnCells;          // Number of Cells  (1000)
      Int_t         fnActiveCells;    // Number of active cells
      Int_t         fnSampl;          // Number of MC events per cell in build-up (1000)
      Int_t         fnBin;            // Number of bins in build-up (100)
      Int_t         fEvPerBin;        // Maximum events (equiv.) per bin in buid-up (1000)

      Bool_t        fCompress;        // compress foam output file
      Bool_t        fMultiTargetRegression; // do regression on multible targets
      UInt_t        fNmin;            // minimal number of events in cell necessary to split cell"
      Bool_t        fCutNmin;         // Keep for bw compatibility: Grabbing cell with maximal RMS to split next (TFoam default)
      UInt_t        fMaxDepth;        // maximum depth of cell tree

      TString       fKernelStr;       // Kernel for GetMvaValue() (option string)
      EKernel       fKernel;          // Kernel for GetMvaValue()
      PDEFoamKernelBase *fKernelEstimator;// Kernel estimator
      TString       fTargetSelectionStr; // method of selecting the target (only mulit target regr.)
      ETargetSelection fTargetSelection; // method of selecting the target (only mulit target regr.)
      Bool_t        fFillFoamWithOrigWeights; // fill the foam with boost weights
      Bool_t        fUseYesNoCell;    // return -1 or 1 for bg or signal like event
      TString       fDTLogic;         // use DT algorithm to split cells
      EDTSeparation fDTSeparation;    // enum which specifies the separation to use for the DT logic
      Bool_t        fPeekMax;         // BACKWARDS COMPATIBILITY: peek up cell with max. driver integral for split

      std::vector<Float_t> fXmin, fXmax; // range for histograms and foams

      std::vector<PDEFoam*> fFoam;    // grown PDEFoams

      // default initialisation called by all constructors
      void Init( void );

      ClassDef(MethodPDEFoam,0) // Multi-dimensional probability density estimator using TFoam (PDE-Foam)
   };

} // namespace TMVA

#endif // MethodPDEFoam_H
