// @(#)root/mathcore:$Id: TKDTreeBinning.h 39937 2011-06-24 11:21:29Z moneta $
// Authors: B. Rabacal   11/2010

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2010 , LCG ROOT MathLib Team                         *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

// Header file for class TKDTreeBinning
//
//

#include <map>
#include <utility>

#ifndef ROOT_TKDTree
#include "TKDTree.h"
#endif

namespace ROOT {
   namespace Fit {
      class BinData;
   }
}

class TKDTreeBinning : public TObject {
private: 
   Double_t** fData; // The data from which a KDTree partition is computed for binning
   std::vector<Double_t> fBinMinEdges; // The minimum values for the bins' edges for each dimension
   std::vector<Double_t> fBinMaxEdges; // The maximum values for the bins' edges for each dimension
   TKDTreeID* fDataBins; // The binning inner structure.
   UInt_t fNBins; // The number of bins
   UInt_t fDim; // The data dimension
   UInt_t fDataSize; // The data size
   std::vector<std::pair<Double_t, Double_t> > fDataThresholds; // Minimum and maximum data values.
   std::vector<std::vector<std::pair<Bool_t, Bool_t> > > fCheckedBinEdges; // Auxiliary structure for readjusting the bin edges. Flags if the bin edge was processed in  the algorithm
   std::vector<std::map<Double_t, std::vector<UInt_t> > > fCommonBinEdges; // Auxiliary structure for readjusting the bin edges. Keeps the common bin boundaries
   Bool_t fIsSorted; // Flags if the bin edges are sorted densitywise
   Bool_t fIsSortedAsc; // Flags if the bin edges are sorted densitywise in ascending order
   std::vector<UInt_t> fBinsContent; // Holds the contents of the bins
   struct CompareAsc; // Predicate for ascending sort
   friend struct CompareAsc;
   struct CompareDesc; // Predicate for descending sort
   friend struct CompareDesc;
   TKDTreeBinning(TKDTreeBinning& bins);           // Disallowed copy constructor
   TKDTreeBinning operator=(TKDTreeBinning& bins); // Disallowed assign operator
   void SetData(Double_t* data);
   void SetTreeData();
   void SetBinsEdges();
   void SetBinMinMaxEdges(Double_t* binEdges);
   void SetCommonBinEdges(Double_t* binEdges);
   void SetBinsContent();
   void ReadjustMinBinEdges(Double_t* binEdges);
   void ReadjustMaxBinEdges(Double_t* binEdges);

public:

   // flag bits
   enum {
      kAdjustBinEdges     = BIT(14)  // adjust bin edges to avoid overlapping with data
   };


   TKDTreeBinning(UInt_t dataSize, UInt_t dataDim, Double_t* data, UInt_t nBins = 100, bool adjustBinEdges = false);
   ~TKDTreeBinning();
   void SetNBins(UInt_t bins);
   void SortBinsByDensity(Bool_t sortAsc = kTRUE);
   const Double_t* GetBinsMinEdges() const;
   const Double_t* GetBinsMaxEdges() const;
   std::pair<const Double_t*, const Double_t*> GetBinsEdges() const;
   std::pair<const Double_t*, const Double_t*> GetBinEdges(UInt_t bin) const;
   const Double_t* GetBinMinEdges(UInt_t bin) const;
   const Double_t* GetBinMaxEdges(UInt_t bin) const;
   UInt_t GetNBins() const;
   UInt_t GetDim() const;
   UInt_t GetBinContent(UInt_t bin) const;
   TKDTreeID* GetTree() const;
   const Double_t* GetDimData(UInt_t dim) const;
   Double_t GetDataMin(UInt_t dim) const;
   Double_t GetDataMax(UInt_t dim) const;
   Double_t GetBinDensity(UInt_t bin) const;
   Double_t GetBinVolume(UInt_t bin) const;
   const Double_t* GetOneDimBinEdges() const;
   const Double_t* GetBinCenter(UInt_t bin) const;
   const Double_t* GetBinWidth(UInt_t bin) const;
   UInt_t GetBinMaxDensity() const;
   UInt_t GetBinMinDensity() const;
   void FillBinData(ROOT::Fit::BinData & data) const;

   ClassDef(TKDTreeBinning, 1)

};
