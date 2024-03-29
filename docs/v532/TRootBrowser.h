// @(#)root/gui:$Id: TRootBrowser.h 37996 2011-02-07 15:33:10Z rdm $
// Author: Bertrand Bellenot   26/09/2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TRootBrowser
#define ROOT_TRootBrowser

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif

#ifndef ROOT_TBrowserImp
#include "TBrowserImp.h"
#endif

class TGLayoutHints;
class TGTab;
class TGMenuBar;
class TGPopupMenu;
class TGStatusBar;
class TGVSplitter;
class TGHSplitter;

//_____________________________________________________________________________
//
// TBrowserPlugin
//
// Helper class used to manage plugins (command or macro to be executed).
//_____________________________________________________________________________

class TBrowserPlugin : public TNamed
{
public:
   Int_t    fTab;             // Tab number
   Int_t    fSubTab;          // Tab element number
   TString  fCommand;         // Command to be executed

   TBrowserPlugin(const char *name, const char *cmd = "", Int_t tab = 1,
                  Int_t sub = -1) : TNamed(name, cmd), fTab(tab),
      fSubTab(sub), fCommand(cmd) { }
   virtual ~TBrowserPlugin() {}

   void     SetTab(Int_t tab) { fTab = tab; }
   void     SetSubTab(Int_t sub) { fSubTab = sub; }
   void     SetCommand(const char *cmd) { fCommand = cmd; }

   ClassDef(TBrowserPlugin, 0)  // basic plugin description class
};

class TRootBrowser : public TGMainFrame, public TBrowserImp {
private:
   TRootBrowser(const TRootBrowser&); // Not implemented
   TRootBrowser& operator=(const TRootBrowser&); // Not implemented

protected:

   TGLayoutHints     *fLH0, *fLH1, *fLH2, *fLH3;   // Layout hints, part 1
   TGLayoutHints     *fLH4, *fLH5, *fLH6, *fLH7;   // Layout hints, part 2
   TGTab             *fTabLeft;                    // Left Tab
   TGTab             *fTabRight;                   // Right Tab
   TGTab             *fTabBottom;                  // Bottom Tab
   TGTab             *fEditTab;                    // Tab in "Edit" mode
   Int_t              fEditPos;                    // Id of tab in "Edit" mode
   Int_t              fEditSubPos;                 // Id of subtab in "Edit" mode
   TGVerticalFrame   *fVf;                         // Vertical frame
   TGHorizontalFrame *fHf;                         // Horizontal frame
   TGHorizontalFrame *fH1;                         // Horizontal frame
   TGHorizontalFrame *fH2;                         // Horizontal frame
   TGVerticalFrame   *fV1;                         // Vertical frame
   TGVerticalFrame   *fV2;                         // Vertical frame
   TGVSplitter       *fVSplitter;                  // Vertical splitter
   TGHSplitter       *fHSplitter;                  // Horizontal splitter
   TGCompositeFrame  *fEditFrame;                  // Frame in "Edit" mode
   TGHorizontalFrame *fTopMenuFrame;               // Top menu frame
   TGHorizontalFrame *fPreMenuFrame;               // First (owned) menu frame
   TGHorizontalFrame *fMenuFrame;                  // Shared menu frame
   TGHorizontalFrame *fToolbarFrame;               // Toolbar frame
   TGMenuBar         *fMenuBar;                    // Main (owned) menu bar
   TGPopupMenu       *fMenuFile;                   // "File" popup menu
   TGPopupMenu       *fMenuExecPlugin;             // "Exec Plugin" popup menu
   TGPopupMenu       *fMenuHelp;                   // "Browser Help" popup menu
   TGCompositeFrame  *fActMenuBar;                 // Actual (active) menu bar
   TBrowserImp       *fActBrowser;                 // Actual (active) browser imp
   TList              fBrowsers;                   // List of (sub)browsers
   TList              fPlugins;                    // List of plugins
   TGStatusBar       *fStatusBar;                  // Status bar
   Int_t              fNbInitPlugins;              // Number of initial plugins (from .rootrc)
   Int_t              fNbTab[3];                   // Number of tab elements (for each Tab)
   Int_t              fCrTab[3];                   // Actual (active) tab elements (for each Tab)
   Int_t              fPid;                        // Current process id
   Bool_t             fShowCloseTab;               // kTRUE to show close icon on tab elements

public:
   enum EInsertPosition {
      kLeft, kRight, kBottom
   };

   TRootBrowser(TBrowser *b = 0, const char *name = "ROOT Browser", UInt_t width = 800, UInt_t height = 500, Option_t *opt="", Bool_t initshow=kTRUE);
   TRootBrowser(TBrowser *b, const char *name, Int_t x, Int_t y, UInt_t width, UInt_t height, Option_t *opt="", Bool_t initshow=kTRUE);
   virtual ~TRootBrowser();

   void              InitPlugins(Option_t *opt="");

   void              CreateBrowser(const char *name);
   void              CloneBrowser();
   void              CloseWindow();
   void              CloseTab(Int_t id);
   virtual void      CloseTabs();
   void              DoTab(Int_t id);
   TGFrame          *GetActFrame() const { return (TGFrame *)fEditFrame; }
   TGFrame          *GetToolbarFrame() const { return (TGFrame *)fToolbarFrame; }
   TGStatusBar      *GetStatusBar() const { return fStatusBar; }
   TGTab            *GetTabLeft() const { return fTabLeft; }
   TGTab            *GetTabRight() const { return fTabRight; }
   TGTab            *GetTabBottom() const { return fTabBottom; }
   TGTab            *GetTab(Int_t pos) const;
   void              SetTab(Int_t pos = kRight, Int_t subpos = -1);
   void              SetTabTitle(const char *title, Int_t pos = kRight, Int_t subpos = -1);
   void              HandleMenu(Int_t id);
   void              RecursiveReparent(TGPopupMenu *popup);
   void              RemoveTab(Int_t pos, Int_t subpos);
   void              SetActBrowser(TBrowserImp *b) { fActBrowser = b; }
   void              ShowMenu(TGCompositeFrame *menu);
   virtual void      StartEmbedding(Int_t pos = kRight, Int_t subpos = -1);
   virtual void      StopEmbedding(const char *name = 0) { StopEmbedding(name, 0); }
   void              StopEmbedding(const char *name, TGLayoutHints *layout);
   void              SwitchMenus(TGCompositeFrame *from);

   virtual void      BrowseObj(TObject *obj);             //*SIGNAL*
   virtual void      ExecuteDefaultAction(TObject *obj);  //*SIGNAL*
   virtual void      DoubleClicked(TObject *obj);         //*SIGNAL*
   virtual void      Checked(TObject *obj, Bool_t check); //*SIGNAL*

   virtual void      Add(TObject *obj, const char *name = 0, Int_t check = -1);
   virtual void      RecursiveRemove(TObject *obj);
   virtual void      Refresh(Bool_t force = kFALSE);
   virtual void      Show() { MapRaised(); }
   Option_t         *GetDrawOption() const;
   TGMainFrame      *GetMainFrame() const { return (TGMainFrame *)this; }

   virtual Long_t    ExecPlugin(const char *name = 0, const char *fname = 0,
                                const char *cmd = 0, Int_t pos = kRight,
                                Int_t subpos = -1);
   virtual void      SetStatusText(const char *txt, Int_t col);
   virtual Bool_t    HandleKey(Event_t *event);

   virtual void      ShowCloseTab(Bool_t show) { fShowCloseTab = show; }
   virtual Bool_t    IsCloseTabShown() const { return fShowCloseTab; }

   // overridden from TGMainFrame
   virtual void      ReallyDelete();

   static TBrowserImp *NewBrowser(TBrowser *b = 0, const char *title = "ROOT Browser", UInt_t width = 800, UInt_t height = 500, Option_t *opt="");
   static TBrowserImp *NewBrowser(TBrowser *b, const char *title, Int_t x, Int_t y, UInt_t width, UInt_t height, Option_t *opt="");

   ClassDef(TRootBrowser, 0) // New ROOT Browser
};

#endif
