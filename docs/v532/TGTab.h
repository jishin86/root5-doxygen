// @(#)root/gui:$Id: TGTab.h 26180 2008-11-13 15:51:56Z bellenot $
// Author: Fons Rademakers   13/01/98

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGTab
#define ROOT_TGTab


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TGTab, TGTabElement, TGTabLayout                                     //
//                                                                      //
// A tab widget contains a set of composite frames each with a little   //
// tab with a name (like a set of folders with tabs).                   //
//                                                                      //
// The TGTab is user callable. The TGTabElement and TGTabLayout are     //
// is a service classes of the tab widget.                              //
//                                                                      //
// Clicking on a tab will bring the associated composite frame to the   //
// front and generate the following event:                              //
// kC_COMMAND, kCM_TAB, tab id, 0.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGWidget
#include "TGWidget.h"
#endif

class TList;
class TGTabElement;
class TGTab;
class TGPicture;

class TGTabLayout : public TGLayoutManager {

protected:
   TGTab    *fMain;      // container frame
   TList    *fList;      // list of frames to arrange

private:
   TGTabLayout(const TGTabLayout&);             // not implemented
   TGTabLayout& operator=(const TGTabLayout&);  // not implemented

public:
   TGTabLayout(TGTab *main);

   virtual void Layout();
   virtual TGDimension GetDefaultSize() const;
   virtual void SavePrimitive(ostream &out, Option_t *option = "");

   ClassDef(TGTabLayout,0)  // Layout manager for TGTab widget
};



class TGTab : public TGCompositeFrame, public TGWidget {

protected:
   Int_t               fCurrent;        // index of current tab
   UInt_t              fTabh;           // tab height
   TGCompositeFrame   *fContainer;      // main container
   TList              *fRemoved;        // list of removed tabs
   FontStruct_t        fFontStruct;     // font
   GContext_t          fNormGC;         // drawing context

   void ChangeTab(Int_t tabIndex, Bool_t emit=kTRUE);

   static const TGFont *fgDefaultFont;
   static const TGGC   *fgDefaultGC;

private:
   TGTab(const TGTab&);             // not implemented
   TGTab& operator=(const TGTab&);  // not implemented

public:
   static FontStruct_t  GetDefaultFontStruct();
   static const TGGC   &GetDefaultGC();

   TGTab(const TGWindow *p = 0, UInt_t w = 1, UInt_t h = 1,
         GContext_t norm = GetDefaultGC()(),
         FontStruct_t font = GetDefaultFontStruct(),
         UInt_t options = kChildFrame,
         Pixel_t back = GetDefaultFrameBackground());
   virtual ~TGTab();

   virtual TGCompositeFrame *AddTab(TGString *text);
   virtual TGCompositeFrame *AddTab(const char *text);
   virtual void              AddTab(const char *text, TGCompositeFrame *cf);
   virtual void              AddTab(TGString *text, TGCompositeFrame *cf);

   virtual void              NewTab(const char *text = "tab");   // *MENU*icon=bld_newtab.png*
   virtual void              RemoveTab(Int_t tabIndex = -1,
                                       Bool_t storeRemoved = kTRUE); // *MENU*icon=bld_removetab.png*
   virtual Bool_t            SetTab(Int_t tabIndex, Bool_t emit = kTRUE);
   virtual Bool_t            SetTab(const char *name, Bool_t emit = kTRUE);
   virtual void              DrawBorder() { }

   TGCompositeFrame *GetContainer() const { return fContainer; }
   Int_t             GetCurrent() const { return fCurrent; }
   TGCompositeFrame *GetTabContainer(Int_t tabIndex) const;
   TGCompositeFrame *GetTabContainer(const char *name) const;
   TGTabElement     *GetTabTab(Int_t tabIndex) const;
   TGTabElement     *GetTabTab(const char *name) const;
   TGCompositeFrame *GetCurrentContainer() const { return GetTabContainer(fCurrent); }
   TGTabElement     *GetCurrentTab() const { return GetTabTab(fCurrent); }
   UInt_t            GetTabHeight() const { return fTabh; }
   Int_t             GetNumberOfTabs() const;
   virtual void      SetEnabled(Int_t tabIndex, Bool_t on = kTRUE);  //*MENU*
   virtual void      SetText(const char *text = "tab");              //*MENU*icon=bld_rename.png*
   Bool_t            IsEnabled(Int_t tabIndex) const;

   virtual void      SavePrimitive(ostream &out, Option_t *option = "");

   virtual void CloseTab(Int_t id) { Emit("CloseTab(Int_t)", id); }  //*SIGNAL*
   virtual void Removed(Int_t id) { Emit("Removed(Int_t)", id); }    //*SIGNAL*
   virtual void Selected(Int_t id) { Emit("Selected(Int_t)", id); }  //*SIGNAL*
   virtual TGLayoutManager *GetLayoutManager() const;

   ClassDef(TGTab,0)  // Tab widget
};



class TGTabElement : public TGFrame {

protected:
   TGString        *fText;            // text on tab
   const TGPicture *fClosePic;        // "close tab" icon
   const TGPicture *fClosePicD;       // "close tab" icon (disabled)
   GContext_t       fNormGC;          // graphics context for drawing tab
   FontStruct_t     fFontStruct;      // font used for tab
   UInt_t           fTWidth;          // width of tab text
   UInt_t           fTHeight;         // height of tab text
   Bool_t           fEnabled;         // enabled or disabled
   Bool_t           fShowClose;       // show or hide close icon
   Bool_t           fActive;          // true if active (in front)

private:
   TGTabElement(const TGTabElement&);             // not implemented
   TGTabElement& operator=(const TGTabElement&);  // not implemented

public:
   TGTabElement(const TGWindow *p = 0, TGString *text = 0, UInt_t w = 1, UInt_t h = 1,
                GContext_t norm = TGTab::GetDefaultGC()(),
                FontStruct_t font = TGTab::GetDefaultFontStruct(),
                UInt_t options = kRaisedFrame,
                Pixel_t back = GetDefaultFrameBackground());
   virtual ~TGTabElement();

   virtual void        DrawBorder();
   virtual TGDimension GetDefaultSize() const;
   const TGString     *GetText() const { return fText; }
   const char         *GetString() const { return fText->GetString(); }
   virtual Bool_t      HandleButton(Event_t *event);
   void                SetText(TGString *text);
   virtual void        SetEnabled(Bool_t on = kTRUE) { fEnabled = on; }
   Bool_t              IsEnabled() const { return fEnabled; }
   virtual void        SetEditDisabled(UInt_t) {}
   virtual void        ShowClose(Bool_t on = kTRUE);
   Bool_t              IsCloseShown() const { return fShowClose; }
   virtual void        SetActive(Bool_t on = kTRUE) { fActive = on; }
   Bool_t              IsActive() const { return fActive; }

   ClassDef(TGTabElement,0)  // Little tab on tab widget
};

#endif
