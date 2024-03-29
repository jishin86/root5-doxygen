// @(#)root/tmva $Id: MethodKNN.h 40005 2011-06-27 15:29:10Z stelzer $
// Author: Rustem Ospanov

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : MethodKNN                                                             *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Analysis of k-nearest neighbor                                            *
 *                                                                                *
 * Author:                                                                        *
 *      Rustem Ospanov <rustem@fnal.gov> - U. of Texas at Austin, USA             *
 *                                                                                *
 * Copyright (c) 2007:                                                            *
 *      CERN, Switzerland                                                         * 
 *      MPI-K Heidelberg, Germany                                                 * 
 *      U. of Texas at Austin, USA                                                *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_MethodKNN
#define ROOT_TMVA_MethodKNN

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// MethodKNN                                                            //
//                                                                      //
// Analysis of k-nearest neighbor                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <map>

// Local
#ifndef ROOT_TMVA_MethodBase
#include "TMVA/MethodBase.h"
#endif
#ifndef ROOT_TMVA_ModulekNN
#include "TMVA/ModulekNN.h"
#endif

// SVD and linear discriminat code
#ifndef ROOT_TMVA_LDA
#include "TMVA/LDA.h"
#endif

namespace TMVA
{   
   namespace kNN
   {
      class ModulekNN;
   }

   class MethodKNN : public MethodBase
   {
   public:

      MethodKNN(const TString& jobName, 
                const TString& methodTitle, 
                DataSetInfo& theData,
                const TString& theOption = "KNN",
                TDirectory* theTargetDir = NULL);

      MethodKNN(DataSetInfo& theData, 
                const TString& theWeightFile,  
                TDirectory* theTargetDir = NULL);
      
      virtual ~MethodKNN( void );
    
      virtual Bool_t HasAnalysisType( Types::EAnalysisType type, UInt_t numberClasses, UInt_t numberTargets );

      void Train( void );

      Double_t GetMvaValue( Double_t* err = 0, Double_t* errUpper = 0 );
      const std::vector<Float_t>& GetRegressionValues();

      using MethodBase::ReadWeightsFromStream;

      void WriteWeightsToStream(TFile& rf) const;
      void AddWeightsXMLTo( void* parent ) const;
      void ReadWeightsFromXML( void* wghtnode );

      void ReadWeightsFromStream(std::istream& istr);
      void ReadWeightsFromStream(TFile &rf);

      const Ranking* CreateRanking();

   protected:

      // make ROOT-independent C++ class for classifier response (classifier-specific implementation)
      void MakeClassSpecific( std::ostream&, const TString& ) const;

      // get help message text
      void GetHelpMessage() const;

   private:

      // the option handling methods
      void DeclareOptions();
      void ProcessOptions();
      void DeclareCompatibilityOptions();

      // default initialisation called by all constructors
      void Init( void );

      // create kd-tree (binary tree) structure
      void MakeKNN( void );

      // polynomial and Gaussian kernel weight function
      Double_t PolnKernel(Double_t value) const;
      Double_t GausKernel(const kNN::Event &event_knn, const kNN::Event &event, const std::vector<Double_t> &svec) const;

      Double_t getKernelRadius(const kNN::List &rlist) const;
      const std::vector<Double_t> getRMS(const kNN::List &rlist, const kNN::Event &event_knn) const;
      
      double getLDAValue(const kNN::List &rlist, const kNN::Event &event_knn);

   private:

      // number of events (sumOfWeights)
      Double_t fSumOfWeightsS;        // sum-of-weights for signal training events
      Double_t fSumOfWeightsB;        // sum-of-weights for background training events      

      kNN::ModulekNN *fModule;        //! module where all work is done

      Int_t fnkNN;            // number of k-nearest neighbors 
      Int_t fBalanceDepth;    // number of binary tree levels used for balancing tree

      Float_t fScaleFrac;     // fraction of events used to compute variable width
      Float_t fSigmaFact;     // scale factor for Gaussian sigma in Gaus. kernel

      TString fKernel;        // ="Gaus","Poln" - kernel type for smoothing

      Bool_t fTrim;           // set equal number of signal and background events
      Bool_t fUseKernel;      // use polynomial kernel weight function
      Bool_t fUseWeight;      // use weights to count kNN
      Bool_t fUseLDA;         // use local linear discriminat analysis to compute MVA

      kNN::EventVec fEvent;   //! (untouched) events used for learning

      LDA fLDA;               //! Experimental feature for local knn analysis

      // for backward compatibility
      Int_t fTreeOptDepth;    // number of binary tree levels used for optimization

      ClassDef(MethodKNN,0) // k Nearest Neighbour classifier
   };

} // namespace TMVA

#endif // MethodKNN
