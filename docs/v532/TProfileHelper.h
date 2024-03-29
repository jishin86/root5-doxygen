// @(#)root/hist:$Id: TProfileHelper.h 41443 2011-10-18 09:09:59Z moneta $
// Author: David Gonzalez Maline   18/01/2008

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TProfileHelper
#define ROOT_TProfileHelper


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TProfileHelper                                                       //
//                                                                      //
// Profile helper class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TH1.h"
#include "TError.h"
#include "TCollection.h"
#include "THashList.h"
#include "TMath.h"

class TProfileHelper {

public:
   template <typename T>
   static void Add(T* p, const TH1 *h1,  const TH1 *h2, Double_t c1, Double_t c2=1);

   template <typename T>
   static Double_t GetBinEffectiveEntries(T* p, Int_t bin);

   template <typename T>
   static Long64_t Merge(T* p, TCollection *list);

   template <typename T>
   static T* RebinAxis(T* p, Double_t x, TAxis *axis);

   template <typename T>
   static void Scale(T* p, Double_t c1, Option_t * option);

   template <typename T> 
   static void Sumw2(T* p);

   template <typename T>
   static void LabelsDeflate(T* p, Option_t *);

   template <typename T>
   static void LabelsInflate(T* p, Option_t *);

   template <typename T>
   static Double_t GetBinError(T* p, Int_t bin);
};

template <typename T>
void TProfileHelper::Add(T* p, const TH1 *h1,  const TH1 *h2, Double_t c1, Double_t c2)
{
   // Performs the operation: this = c1*h1 + c2*h2

   T *p1 = (T*)h1;
   T *p2 = (T*)h2;

   // delete buffer if it is there since it will become invalid
   if (p->fBuffer) p->BufferEmpty(1);

// Check profile compatibility
   Int_t nx = p->GetNbinsX();
   Int_t ny = p->GetNbinsY();
   Int_t nz = p->GetNbinsZ();

   if ( nx != p1->GetNbinsX() ||  nx != p2->GetNbinsX() ||
        ny != p1->GetNbinsY() ||  ny != p2->GetNbinsY() ||
        nz != p1->GetNbinsZ() ||  nz != p2->GetNbinsZ() ) {
      Error("TProfileHelper::Add","Attempt to add profiles with different number of bins");
      return;
   }

// Add statistics
   Double_t ac1 = TMath::Abs(c1);
   Double_t ac2 = TMath::Abs(c2);
   p->fEntries = ac1*p1->GetEntries() + ac2*p2->GetEntries();
   Double_t s0[TH1::kNstat], s1[TH1::kNstat], s2[TH1::kNstat];
   Int_t i;
   for (i=0;i<TH1::kNstat;i++) {s0[i] = s1[i] = s2[i] = 0;}
   p->GetStats(s0);
   p1->GetStats(s1);
   p2->GetStats(s2);
   for (i=0;i<TH1::kNstat;i++) {
      if (i == 1) s0[i] = c1*c1*s1[i] + c2*c2*s2[i]; 
      else        s0[i] = ac1*s1[i] + ac2*s2[i];
   }
   p->PutStats(s0);

// Make the loop over the bins to calculate the Addition
   Int_t bin;
   Double_t *cu1 = p1->GetW();    Double_t *cu2 = p2->GetW();
   Double_t *er1 = p1->GetW2();   Double_t *er2 = p2->GetW2();
   Double_t *en1 = p1->GetB();    Double_t *en2 = p2->GetB();
   Double_t *ew1 = p1->GetB2();   Double_t *ew2 = p2->GetB2();
   // create sumw2 per bin if not set 
   if (p->fBinSumw2.fN == 0 && (p1->fBinSumw2.fN != 0 || p2->fBinSumw2.fN != 0) ) p->Sumw2();
   // if p1 has not the sum of weight squared/bin stored use just the sum of weights  
   if (ew1 == 0) ew1 = en1;
   if (ew2 == 0) ew2 = en2; 
   for (bin =0;bin< p->fN;bin++) {
      p->fArray[bin]             = c1*cu1[bin] + c2*cu2[bin];
      p->fSumw2.fArray[bin]      = ac1*er1[bin] + ac2*er2[bin];
      p->fBinEntries.fArray[bin] = ac1*en1[bin] + ac2*en2[bin];
      if (p->fBinSumw2.fN ) p->fBinSumw2.fArray[bin]  = ac1*ac1*ew1[bin] + ac2*ac2*ew2[bin];
   }
}

template <typename T>
Double_t TProfileHelper::GetBinEffectiveEntries(T* p, Int_t bin) 
{
//            Return bin effective entries for a weighted filled Profile histogram. 
//            In case of an unweighted profile, it is equivalent to the number of entries per bin   
//            The effective entries is defined as the square of the sum of the weights divided by the 
//            sum of the weights square. 
//            TProfile::Sumw2() must be called before filling the profile with weights. 
//            Only by calling this method the  sum of the square of the weights per bin is stored. 
//  

   if (p->fBuffer) p->BufferEmpty();

   if (bin < 0 || bin >= p->fNcells) return 0;
   double sumOfWeights = p->fBinEntries.fArray[bin];
   if ( p->fBinSumw2.fN == 0 || p->fBinSumw2.fN != p->fNcells) { 
      // this can happen  when reading an old file 
      p->fBinSumw2.Set(0);
      return sumOfWeights;
   }
   double sumOfWeightsSquare = p->fBinSumw2.fArray[bin]; 
   return ( sumOfWeightsSquare > 0 ?  sumOfWeights * sumOfWeights /   sumOfWeightsSquare : 0 ); 
}

template <typename T>
Long64_t TProfileHelper::Merge(T* p, TCollection *li) {
   //Merge all histograms in the collection in this histogram.
   //This function computes the min/max for the axes,
   //compute a new number of bins, if necessary,
   //add bin contents, errors and statistics.
   //If overflows are present and limits are different the function will fail.
   //The function returns the total number of entries in the result histogram
   //if the merge is successfull, -1 otherwise.
   //
   //IMPORTANT remark. The 2 axis x and y may have different number
   //of bins and different limits, BUT the largest bin width must be
   //a multiple of the smallest bin width and the upper limit must also
   //be a multiple of the bin width.

   if (!li) return 0;
   if (li->IsEmpty()) return (Int_t) p->GetEntries();

   TList inlist;
   inlist.AddAll(li);

   TAxis newXAxis;
   TAxis newYAxis;
   TAxis newZAxis;
   Bool_t initialLimitsFound = kFALSE;
   Bool_t allSameLimits = kTRUE;
   Bool_t allHaveLimits = kTRUE;

   TIter next(&inlist);
   T* h = p;

   do {

      // skip empty histograms 
      if (h->fTsumw == 0 && h->GetEntries() == 0) continue;

      Bool_t hasLimits = h->GetXaxis()->GetXmin() < h->GetXaxis()->GetXmax();
      allHaveLimits = allHaveLimits && hasLimits;

      if (hasLimits) {
         h->BufferEmpty();
         if (!initialLimitsFound) {
            initialLimitsFound = kTRUE;
            newXAxis.Set(h->GetXaxis()->GetNbins(), h->GetXaxis()->GetXmin(),
                     h->GetXaxis()->GetXmax());
            if ( p->GetDimension() >= 2 )
            newYAxis.Set(h->GetYaxis()->GetNbins(), h->GetYaxis()->GetXmin(),
                     h->GetYaxis()->GetXmax());
            if ( p->GetDimension() >= 3 )
            newZAxis.Set(h->GetZaxis()->GetNbins(), h->GetZaxis()->GetXmin(),
                     h->GetZaxis()->GetXmax());
         }
         else {
            // check first if histograms have same bins 
            if (!p->SameLimitsAndNBins(newXAxis, *(h->GetXaxis())) || 
                !p->SameLimitsAndNBins(newYAxis, *(h->GetYaxis())) || 
                !p->SameLimitsAndNBins(newZAxis, *(h->GetZaxis())) ) { 

               allSameLimits = kFALSE;
               // recompute in this case the optimal limits
               // The condition to works is that the histogram have same bin with 
               // and one common bin edge
               
               if (!p->RecomputeAxisLimits(newXAxis, *(h->GetXaxis()))) {
                  Error("TProfileHelper::Merge", "Cannot merge profiles %d dim - limits are inconsistent:\n "
                     "first: (%d, %f, %f), second: (%d, %f, %f)",p->GetDimension(),
                        newXAxis.GetNbins(), newXAxis.GetXmin(), newXAxis.GetXmax(),
                        h->GetXaxis()->GetNbins(), h->GetXaxis()->GetXmin(),
                        h->GetXaxis()->GetXmax());
                  return -1;
               }
               if (p->GetDimension() >= 2 && !p->RecomputeAxisLimits(newYAxis, *(h->GetYaxis()))) {
                  Error("TProfileHelper::Merge", "Cannot merge profiles %d dim - limits are inconsistent:\n "
                        "first: (%d, %f, %f), second: (%d, %f, %f)",p->GetDimension(),
                        newYAxis.GetNbins(), newYAxis.GetXmin(), newYAxis.GetXmax(),
                        h->GetYaxis()->GetNbins(), h->GetYaxis()->GetXmin(),
                        h->GetYaxis()->GetXmax());
                  return -1;
               }
               if (p->GetDimension() >= 3 && !p->RecomputeAxisLimits(newZAxis, *(h->GetZaxis()))) {
                  Error("TProfileHelper::Merge", "Cannot merge profiles %d dim - limits are inconsistent:\n "
                        "first: (%d, %f, %f), second: (%d, %f, %f)",p->GetDimension(),
                        newZAxis.GetNbins(), newZAxis.GetXmin(), newZAxis.GetXmax(),
                        h->GetZaxis()->GetNbins(), h->GetZaxis()->GetXmin(),
                        h->GetZaxis()->GetXmax());
                  return -1;
               }
            }
         }
      }
   }  while ( ( h = dynamic_cast<T*> ( next() ) ) != NULL );
   if (!h && (*next) ) {
      Error("TProfileHelper::Merge","Attempt to merge object of class: %s to a %s",
            (*next)->ClassName(),p->ClassName());
      return -1;
   }

   next.Reset();

   // In the case of histogram with different limits
   // newX(Y)Axis will now have the new found limits
   // but one needs first to clone this histogram to perform the merge
   // The clone is not needed when all histograms have the same limits
   T * hclone = 0;
   if (!allSameLimits) { 
      // We don't want to add the clone to gDirectory,
      // so remove our kMustCleanup bit temporarily
      Bool_t mustCleanup = p->TestBit(kMustCleanup);
      if (mustCleanup) p->ResetBit(kMustCleanup);
      hclone = (T*)p->IsA()->New();
      R__ASSERT(hclone);
      hclone->SetDirectory(0);
      p->Copy(*hclone);
      if (mustCleanup) p->SetBit(kMustCleanup);
      p->BufferEmpty(1);         // To remove buffer.
      p->Reset();                // BufferEmpty sets limits so we can't use it later.
      p->SetEntries(0);
      inlist.AddFirst(hclone);
   }

   if (!allSameLimits && initialLimitsFound) {
      Int_t b[] = { newXAxis.GetNbins(), newYAxis.GetNbins(), newZAxis.GetNbins() };
      Double_t v[] = { newXAxis.GetXmin(), newXAxis.GetXmax(),
                       newYAxis.GetXmin(), newYAxis.GetXmax(),
                       newZAxis.GetXmin(), newZAxis.GetXmax() };
      p->SetBins(b, v);
   }

   if (!allHaveLimits) {
      // fill this histogram with all the data from buffers of histograms without limits
      while ( (h = dynamic_cast<T*> (next()) ) ) {
         if (h->GetXaxis()->GetXmin() >= h->GetXaxis()->GetXmax() && h->fBuffer) {
             // no limits
            Int_t nbentries = (Int_t)h->fBuffer[0];
            Double_t v[5];
            for (Int_t i = 0; i < nbentries; i++)
               if ( p->GetDimension() == 3 ) {
                  v[0] = h->fBuffer[5*i + 2];
                  v[1] = h->fBuffer[5*i + 3];
                  v[2] = h->fBuffer[5*i + 4];
                  v[3] = h->fBuffer[5*i + 5];
                  v[4] = h->fBuffer[5*i + 1];
                  p->Fill(v);
               } else if ( p->GetDimension() == 2 ) {
                  v[0] = h->fBuffer[4*i + 2];
                  v[1] = h->fBuffer[4*i + 3];
                  v[2] = h->fBuffer[4*i + 4];
                  v[3] = h->fBuffer[4*i + 1];
                  v[4] = 0;
                  p->Fill(v);
               }
               else if ( p->GetDimension() == 1 ) {
                  v[0] = h->fBuffer[3*i + 2];
                  v[1] = h->fBuffer[3*i + 3];
                  v[2] = h->fBuffer[3*i + 1];
                  v[3] = v[4] = 0;
                  p->Fill(v);
               }
         }
      }
      if (!initialLimitsFound) {
         if (hclone) { 
            inlist.Remove(hclone);
            delete hclone; 
         }
         return (Int_t) p->GetEntries();  // all histograms have been processed
      }
      next.Reset();
   }

   //merge bin contents and errors
   Double_t stats[TH1::kNstat], totstats[TH1::kNstat];
   for (Int_t i=0;i<TH1::kNstat;i++) {totstats[i] = stats[i] = 0;}
   p->GetStats(totstats);
   Double_t nentries = p->GetEntries();
   Bool_t canRebin=p->TestBit(TH1::kCanRebin);
   p->ResetBit(TH1::kCanRebin); // reset, otherwise setting the under/overflow will rebin

   while ( (h=static_cast<T*>(next())) ) {
      // process only if the histogram has limits; otherwise it was processed before

      if (h->GetXaxis()->GetXmin() < h->GetXaxis()->GetXmax()) {
         // import statistics
         h->GetStats(stats);
         for (Int_t i = 0; i < TH1::kNstat; i++)
            totstats[i] += stats[i];
         nentries += h->GetEntries();

         for ( Int_t hbin = 0; hbin < h->fN; ++hbin ) {
            if ((!allSameLimits) && (h->IsBinUnderflow(hbin) || h->IsBinOverflow(hbin)) ) {
               if (h->GetW()[hbin] != 0) {
                  Error("TProfileHelper::Merge", "Cannot merge profiles - they have"
                        " different limits and undeflows/overflows are present."
                        " The initial profile is now broken!");
                  return -1;
               }
            }
            Int_t hbinx, hbiny, hbinz;
            h->GetBinXYZ(hbin, hbinx, hbiny, hbinz);
            // find global bin number in p given the x,y,z axis bin numbers in h
            // we can use FindBin on p axes because kCanRebin bit of p has been rreset before 
            Int_t pbin = p->GetBin( p->fXaxis.FindBin( h->GetXaxis()->GetBinCenter(hbinx) ),
                                    p->fYaxis.FindBin( h->GetYaxis()->GetBinCenter(hbiny) ),
                                    p->fZaxis.FindBin( h->GetZaxis()->GetBinCenter(hbinz) ) );


            p->fArray[pbin]             += h->GetW()[hbin];
            p->fSumw2.fArray[pbin]      += h->GetW2()[hbin];
            p->fBinEntries.fArray[pbin] += h->GetB()[hbin];
            if (p->fBinSumw2.fN) { 
               if ( h->GetB2() ) p->fBinSumw2.fArray[pbin] += h->GetB2()[hbin];
               else p->fBinSumw2.fArray[pbin] += h->GetB()[hbin];
            }
         }
      }
   }
   if (canRebin) p->SetBit(TH1::kCanRebin);

   //copy merged stats
   p->PutStats(totstats);
   p->SetEntries(nentries);
   if (hclone) { 
      inlist.Remove(hclone);
      delete hclone;
   }
   return (Long64_t)nentries;
}

template <typename T>
T* TProfileHelper::RebinAxis(T* p, Double_t x, TAxis *axis)
{
// Profile histogram is resized along axis such that x is in the axis range.
// The new axis limits are recomputed by doubling iteratively
// the current axis range until the specified value x is within the limits.
// The algorithm makes a copy of the histogram, then loops on all bins
// of the old histogram to fill the rebinned histogram.
// Takes into account errors (Sumw2) if any.
// The bit kCanRebin must be set before invoking this function.
//  Ex:  h->SetBit(TH1::kCanRebin);


   if (!p->TestBit(TH1::kCanRebin)) return 0;
   if (axis->GetXmin() >= axis->GetXmax()) return 0;
   if (axis->GetNbins() <= 0) return 0;

   Double_t xmin, xmax;
   if (!p->FindNewAxisLimits(axis, x, xmin, xmax))
      return 0;

   //save a copy of this histogram
   T *hold = (T*)p->Clone();
   hold->SetDirectory(0);
   //set new axis limits
   axis->SetLimits(xmin,xmax);
   if (p->fBinSumw2.fN) hold->Sumw2();

   Int_t  nbinsx = p->fXaxis.GetNbins();
   Int_t  nbinsy = p->fYaxis.GetNbins();
   Int_t  nbinsz = p->fZaxis.GetNbins();

   //now loop on all bins and refill
   p->Reset("ICE"); //reset only Integral, contents and Errors

   Double_t bx,by,bz;
   Int_t ix, iy, iz, binx, biny, binz;
   for (binz=1;binz<=nbinsz;binz++) {
      bz  = hold->GetZaxis()->GetBinCenter(binz);
      iz  = p->fZaxis.FindFixBin(bz);
      for (biny=1;biny<=nbinsy;biny++) {
         by  = hold->GetYaxis()->GetBinCenter(biny);
         iy  = p->fYaxis.FindFixBin(by);
         for (binx=1;binx<=nbinsx;binx++) {
            bx = hold->GetXaxis()->GetBinCenter(binx);
            ix  = p->fXaxis.FindFixBin(bx);

            Int_t sourceBin = hold->GetBin(binx,biny,binz);
            Int_t destinationBin = p->GetBin(ix,iy,iz);
            p->AddBinContent(destinationBin, hold->fArray[sourceBin]);
            p->fBinEntries.fArray[destinationBin] += hold->fBinEntries.fArray[sourceBin];
            p->fSumw2.fArray[destinationBin] += hold->fSumw2.fArray[sourceBin];
            if (p->fBinSumw2.fN) p->fBinSumw2.fArray[destinationBin] += hold->fBinSumw2.fArray[sourceBin];
         }
      }
   }
   return hold;
}

template <typename T>
void TProfileHelper::Scale(T* p, Double_t c1, Option_t *)
{
   Double_t ac1 = TMath::Abs(c1);

   // Make the loop over the bins to calculate the Addition
   Int_t bin;
   Double_t *cu1 = p->GetW();
   Double_t *er1 = p->GetW2();
   Double_t *en1 = p->GetB();
   for (bin=0;bin<p->fN;bin++) {
      p->fArray[bin]             = c1*cu1[bin];
      p->fSumw2.fArray[bin]      = ac1*ac1*er1[bin];
      p->fBinEntries.fArray[bin] = en1[bin];
   }
}

template <typename T>
void TProfileHelper::Sumw2(T* p)
{
   // Create structure to store sum of squares of weights per bin  *-*-*-*-*-*-*-*
   //   This is needed to compute  the correct statistical quantities  
   //    of a profile filled with weights 
   //  
   //
   //  This function is automatically called when the histogram is created
   //  if the static function TH1::SetDefaultSumw2 has been called before.

   if ( p->fBinSumw2.fN == p->fNcells) {
      if (!p->fgDefaultSumw2) 
         Warning("Sumw2","Sum of squares of profile bin weights structure already created");
      return;
   }

   p->fBinSumw2.Set(p->fNcells);

   // by default fill with the sum of weights which are stored in fBinEntries
   for (Int_t bin=0; bin<p->fNcells; bin++) {
      p->fBinSumw2.fArray[bin] = p->fBinEntries.fArray[bin]; 
   }
}

template <typename T>
void TProfileHelper::LabelsDeflate(T* p, Option_t *ax)
{
   // Reduce the number of bins for this axis to the number of bins having a label.
   // Works only for the given axis passed in the option
   // The method will remove only the extra bins existing after the last "labeled" bin.
   // Note that if there are "un-labeled" bins present between "labeled" bins they will not be removed 


   TAxis *axis = p->GetXaxis();
   if (ax[0] == 'y' || ax[0] == 'Y') axis = p->GetYaxis();
   if (ax[0] == 'z' || ax[0] == 'Z') axis = p->GetZaxis();
   if (!axis) {
      Error("TProfileHelper::LabelsDeflate","Invalid axis option %s",ax);
      return;
   }
   if (!axis->GetLabels()) return;

   // find bin with last labels 
   // bin number is object ID in list of labels 
   // therefore max bin number is number of bins of the deflated histograms
   TIter next(axis->GetLabels());
   TObject *obj;
   Int_t nbins = 0;
   while ((obj = next())) {
      Int_t ibin = obj->GetUniqueID();
      if (ibin > nbins) nbins = ibin; 
   }
   if (nbins < 1) nbins = 1;
   T *hold = (T*)p->IsA()->New();;
   hold->SetDirectory(0);
   p->Copy(*hold);


   Double_t xmin = axis->GetXmin();
   Double_t xmax = axis->GetBinUpEdge(nbins);
   axis->SetRange(0,0);
   // set the new bins and range
   axis->Set(nbins,xmin,xmax);
   p->SetBinsLength(-1); // reset the number of cells
   p->fBinEntries.Set(p->fN);
   p->fSumw2.Set(p->fN);
   if (p->fBinSumw2.fN)  p->fBinSumw2.Set(p->fN);

   // reset the content
   p->Reset("ICE");

   //now loop on all bins and refill
   Int_t bin,binx,biny,binz;
   for (bin =0; bin < hold->fN; ++bin)
   {
      hold->GetBinXYZ(bin, binx, biny, binz);
      Int_t ibin = p->GetBin(binx, biny, binz);
      p->fArray[ibin] += hold->fArray[bin];
      p->fBinEntries.fArray[ibin] += hold->fBinEntries.fArray[bin];
      p->fSumw2.fArray[ibin] += hold->fSumw2.fArray[bin];
      if (p->fBinSumw2.fN) p->fBinSumw2.fArray[ibin] += hold->fBinSumw2.fArray[bin];
   }

   delete hold;
}

template <typename T>
void TProfileHelper::LabelsInflate(T* p, Option_t *ax)
{
// Double the number of bins for axis.
// Refill histogram
// This function is called by TAxis::FindBin(const char *label)
// Works only for the given axis


   TAxis *axis = p->GetXaxis();
   if (ax[0] == 'y' || ax[0] == 'Y') axis = p->GetYaxis();
   T *hold = (T*)p->IsA()->New();;
   hold->SetDirectory(0);
   p->Copy(*hold);


   Int_t  nbxold = p->fXaxis.GetNbins();
   Int_t  nbyold = p->fYaxis.GetNbins();
   Int_t  nbins  = axis->GetNbins();
   Double_t xmin = axis->GetXmin();
   Double_t xmax = axis->GetXmax();
   xmax = xmin + 2*(xmax-xmin);
   axis->SetRange(0,0);
   // double the number of bins
   nbins *= 2;
   axis->Set(nbins,xmin,xmax);
   // reset the array of content according to the axis
   p->SetBinsLength(-1);  
   Int_t ncells = p->fN;
   p->fBinEntries.Set(ncells);
   p->fSumw2.Set(ncells);
   if (p->fBinSumw2.fN)  p->fBinSumw2.Set(ncells);

   //now loop on all bins and refill
   for (Int_t ibin =0; ibin < p->fN; ibin++)
   {
      Int_t binx, biny, binz;
      p->GetBinXYZ(ibin, binx, biny, binz);
      if (binx <= nbxold && biny <= nbyold) {
         Int_t bin = hold->GetBin(binx, biny, binz);
         p->fArray[ibin] = hold->fArray[bin];
         p->fBinEntries.fArray[ibin] = hold->fBinEntries.fArray[bin];
         p->fSumw2.fArray[ibin] = hold->fSumw2.fArray[bin];
         if (p->fBinSumw2.fN) p->fBinSumw2.fArray[ibin] = hold->fBinSumw2.fArray[bin];
      } else {
         p->fArray[ibin] = 0;
         p->fBinEntries.fArray[ibin] = 0;
         p->fSumw2.fArray[ibin] = 0;
         if (p->fBinSumw2.fN) p->fBinSumw2.fArray[ibin] = 0;
      }
   }
   delete hold;
}

template <typename T>
Double_t TProfileHelper::GetBinError(T* p, Int_t bin)
{

   if (p->fBuffer) p->BufferEmpty();

   if (bin < 0 || bin >= p->fNcells) return 0;
   Double_t cont = p->fArray[bin];
   Double_t sum  = p->fBinEntries.fArray[bin];
   Double_t err2 = p->fSumw2.fArray[bin];
   Double_t neff = p->GetBinEffectiveEntries(bin);
   if (sum == 0) return 0;
   Double_t contsum = cont/sum;
   Double_t eprim2  = TMath::Abs(err2/sum - contsum*contsum);
   Double_t eprim   = TMath::Sqrt(eprim2);
   Double_t test = 1;
   if (err2 != 0 && neff < 5) test = eprim2*sum/err2;
   //Int_t cellLimit = (p->GetDimension() == 3)?1000404:10404;
   if (p->fgApproximate && p->fNcells <=1000404 && (test < 1.e-4 || eprim2 < 1e-6)) { //3.04
      Double_t scont, ssum, serr2;
      scont = ssum = serr2 = 0;
      for (Int_t i=1;i<p->fNcells;i++) {
         if (p->fSumw2.fArray[i] <= 0) continue; //added in 3.10/02
         scont += p->fArray[i];
         ssum  += p->fBinEntries.fArray[i];
         serr2 += p->fSumw2.fArray[i];
      }
      Double_t scontsum = scont/ssum;
      Double_t seprim2  = TMath::Abs(serr2/ssum - scontsum*scontsum);
      eprim           = 2*TMath::Sqrt(seprim2);
      sum = ssum;
   }
   sum = TMath::Abs(sum);
   if (p->fErrorMode == kERRORMEAN) return eprim/TMath::Sqrt(neff);
   else if (p->fErrorMode == kERRORSPREAD) return eprim;
   else if (p->fErrorMode == kERRORSPREADI) {
      if (eprim != 0) return eprim/TMath::Sqrt(neff);
      return 1/TMath::Sqrt(12*neff);
   }
   else if (p->fErrorMode == kERRORSPREADG) {
      // it is supposed the values y are gaussian distributed y +/- dy
      return 1./TMath::Sqrt(sum);
   }
   else return eprim;
}

#endif
