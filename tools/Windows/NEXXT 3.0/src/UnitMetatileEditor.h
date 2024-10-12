//---------------------------------------------------------------------------

#ifndef UnitMetatileEditorH
#define UnitMetatileEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormMetatileTool : public TForm
{
__published:	// IDE-managed Components
	TImage *Image2;
	TGroupBox *GroupBox1;
	TImage *Image1;
	TSpeedButton *btnMap;
	TGroupBox *GroupBox2;
	TSpeedButton *Insert1;
	TSpeedButton *Remove1;
	TSpeedButton *Duplicate1;
	TSpeedButton *Up1;
	TSpeedButton *Down1;
	TSpeedButton *Clear1;
	TSpeedButton *Copy1;
	TSpeedButton *Paste1;
	TSpeedButton *Rename1;
	TTimer *DrawTimer;
	TTimer *OpenByFileAssociationMakeListTimer;
	TGroupBox *GroupBox3;
	TLabel *Label1;
	TSpeedButton *Append1;
	TSpeedButton *Sort1;
	TSpeedButton *btnMetaClear;
	TSpeedButton *btnAttr;
	TPageControl *PageControl1;
	TTabSheet *TabSheet2x2;
	TTabSheet *TabSheet4x4;
	TTabSheet *TabSheet8x8;
	TListBox *ListBox2x2;
	TListBox *ListBox4x4;
    TListBox *ListBox8x8;
	TSpeedButton *btnTiles;
	TSpeedButton *SBSetViewA;
	TSpeedButton *SBSetViewB;
	TSpeedButton *SBSetView64;
	TSpeedButton *SBSetViewC;
	TSpeedButton *SBSetViewD;
	TLabel *Label2;
	TSpeedButton *btnProps;
	TLabel *Label3;
	TLabel *Label4;
	TSpeedButton *btnUseAttr;
	TSpeedButton *btnClonestamp;
	TSpeedButton *btnCloneSnap;
	TSpeedButton *SpeedButton12;
	TLabel *Label5;
	TTimer *ListTimer;
	TLabel *LabelPos;
	TSpeedButton *Rebuild1;
	TCheckBox *chkReserve1st;
	TTimer *TimerAsync;
	TPopupMenu *PopupMenuSetOrClear;
	TMenuItem *tilenames1;
	TMenuItem *paletteattributes1;
	TMenuItem *collisionproperties1;
	TMenuItem *rmNameInMeta;
	TMenuItem *rmNameOnSheet;
	TMenuItem *rmNamesInList;
	TMenuItem *rnNamesAll;
	TMenuItem *rmAttrOnSheet;
	TMenuItem *rmAttrInMeta;
	TMenuItem *rmAttrThisList;
	TMenuItem *rmAttrAll;
	TMenuItem *everything2;
	TMenuItem *rmPropsInMeta;
	TMenuItem *onthissheet1;
	TMenuItem *rmPropsThisList;
	TMenuItem *rmPropsAll;
	TMenuItem *rmEveryMeta;
	TMenuItem *rmEveryOnSheet;
	TMenuItem *rmEveryInList;
	TMenuItem *rmEveryAll;
	TSpeedButton *btnMetaSet;
	TPopupMenu *PopupMenuMore;
	TMenuItem *Associateothernssmetatileliststothissession1;
	TPopupMenu *PopupMenuAppend;
	TMenuItem *Fromnametablemap1;
	TMenuItem *Fromfile1;
	TOpenDialog *OpenDialogAppend;
	TCheckBox *chkAlignScr;
	TLabel *LabelTilePal;
	TLabel *LabelProps;
	TMenuItem *N1;
	TMenuItem *Removeunused1;
	TMenuItem *N2;
	TMenuItem *Findfirstmapmatch1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Image2MouseEnter(TObject *Sender);
	void __fastcall Image2MouseLeave(TObject *Sender);
	void __fastcall Image1MouseEnter(TObject *Sender);
	void __fastcall Image1MouseLeave(TObject *Sender);
	void __fastcall Insert1Click(TObject *Sender);
	void __fastcall Rename1Click(TObject *Sender);
	void __fastcall Clear1Click(TObject *Sender);
	void __fastcall Remove1Click(TObject *Sender);
	void __fastcall Down1Click(TObject *Sender);
	void __fastcall Up1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall DrawTimerTimer(TObject *Sender);
	void __fastcall SBSetView64Click(TObject *Sender);
	void __fastcall SBSetViewAClick(TObject *Sender);
	void __fastcall SBSetViewBClick(TObject *Sender);
	void __fastcall SBSetViewCClick(TObject *Sender);
	void __fastcall SBSetViewDClick(TObject *Sender);
	void __fastcall ListTimerTimer(TObject *Sender);
	void __fastcall Image2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Image2MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y);
	void __fastcall ListBox2x2Click(TObject *Sender);
	void __fastcall ListBox4x4Click(TObject *Sender);
	void __fastcall ListBox8x8Click(TObject *Sender);
	void __fastcall Rebuild1Click(TObject *Sender);
	void __fastcall SpeedButton12Click(TObject *Sender);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall PageControl1Changing(TObject *Sender, bool &AllowChange);
	void __fastcall Image2DragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall Image2DragOver(TObject *Sender, TObject *Source, int X, int Y,
          TDragState State, bool &Accept);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ListBox2x2DragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
	void __fastcall ListBox4x4DragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
	void __fastcall ListBox8x8DragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
	void __fastcall Image2EndDrag(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall TimerAsyncTimer(TObject *Sender);
	void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall rmEveryInListClick(TObject *Sender);
	void __fastcall rmEveryAllClick(TObject *Sender);
	void __fastcall rmPropsAllClick(TObject *Sender);
	void __fastcall rmPropsThisListClick(TObject *Sender);
	void __fastcall rmAttrAllClick(TObject *Sender);
	void __fastcall rmAttrThisListClick(TObject *Sender);
	void __fastcall rnNamesAllClick(TObject *Sender);
	void __fastcall rmNamesInListClick(TObject *Sender);
	void __fastcall rmEveryOnSheetClick(TObject *Sender);
	void __fastcall btnMetaClearClick(TObject *Sender);
	void __fastcall onthissheet1Click(TObject *Sender);
	void __fastcall rmAttrOnSheetClick(TObject *Sender);
	void __fastcall rmNameOnSheetClick(TObject *Sender);
	void __fastcall rmEveryMetaClick(TObject *Sender);
	void __fastcall rmPropsInMetaClick(TObject *Sender);
	void __fastcall rmAttrInMetaClick(TObject *Sender);
	void __fastcall rmNameInMetaClick(TObject *Sender);
	void __fastcall SpeedButton5Click(TObject *Sender);
	void __fastcall Append1Click(TObject *Sender);
	void __fastcall Fromnametablemap1Click(TObject *Sender);
	void __fastcall Fromfile1Click(TObject *Sender);
	void __fastcall OpenDialogAppendSelectionChange(TObject *Sender);
	void __fastcall Copy1Click(TObject *Sender);
	void __fastcall PageControl1MouseEnter(TObject *Sender);
	void __fastcall PageControl1MouseLeave(TObject *Sender);
	void __fastcall Copy1MouseEnter(TObject *Sender);
	void __fastcall Paste1MouseEnter(TObject *Sender);
	void __fastcall Rename1MouseEnter(TObject *Sender);
	void __fastcall Up1MouseEnter(TObject *Sender);
	void __fastcall Down1MouseEnter(TObject *Sender);
	void __fastcall Duplicate1MouseEnter(TObject *Sender);
	void __fastcall Remove1MouseEnter(TObject *Sender);
	void __fastcall Clear1MouseEnter(TObject *Sender);
	void __fastcall Insert1MouseEnter(TObject *Sender);
	void __fastcall btnMapMouseEnter(TObject *Sender);
	void __fastcall btnPropsMouseEnter(TObject *Sender);
	void __fastcall btnAttrMouseEnter(TObject *Sender);
	void __fastcall btnTilesMouseEnter(TObject *Sender);
	void __fastcall btnUseAttrMouseEnter(TObject *Sender);
	void __fastcall btnClonestampMouseEnter(TObject *Sender);
	void __fastcall btnCloneSnapMouseEnter(TObject *Sender);
	void __fastcall SpeedButton12MouseEnter(TObject *Sender);
	void __fastcall SBSetViewAMouseEnter(TObject *Sender);
	void __fastcall SBSetViewBMouseEnter(TObject *Sender);
	void __fastcall SBSetViewCMouseEnter(TObject *Sender);
	void __fastcall SBSetViewDMouseEnter(TObject *Sender);
	void __fastcall SBSetView64MouseEnter(TObject *Sender);
	void __fastcall Rebuild1MouseEnter(TObject *Sender);
	void __fastcall Append1MouseEnter(TObject *Sender);
	void __fastcall Sort1MouseEnter(TObject *Sender);
	void __fastcall Find1MouseEnter(TObject *Sender);
	void __fastcall btnMetaSetMouseEnter(TObject *Sender);
	void __fastcall btnMetaClearMouseEnter(TObject *Sender);
	void __fastcall chkAlignScrMouseEnter(TObject *Sender);
	void __fastcall chkReserve1stMouseEnter(TObject *Sender);
	void __fastcall Paste1Click(TObject *Sender);
	void __fastcall Removeunused1Click(TObject *Sender);
	void __fastcall Sort1Click(TObject *Sender);
	void __fastcall Associateothernssmetatileliststothissession1Click(
          TObject *Sender);
	void __fastcall Findfirstmapmatch1Click(TObject *Sender);
	void __fastcall btnClonestampClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	
private:	// User declarations
public:		// User declarations
	__fastcall TFormMetatileTool(TComponent* Owner);
    void __fastcall BuildMetas(int whichType, int reserveOff);
	void __fastcall MakeList(bool bSelectTop, bool bInit);
    void __fastcall Draw(void);
    void __fastcall AppendFromNametable(void);
	void __fastcall UpdateUI(bool cue);
    bool __fastcall AppendMetatilesFromFile(AnsiString filename, bool mt2,bool mt4,bool mt8);
    void __fastcall GenerateAppendMetatileTitle(bool mt2,bool mt4,bool mt8);
	void __fastcall EvaluateOverlap(int whichType, int reserveOff);
	void __fastcall MergeUniquesFromBuffer(int whichType, int reserveOff);
	void __fastcall PrepMapEditBuffer(int d, int meta_id);
	void __fastcall AutoEditMap(int d, int tx, int ty);
	int  __fastcall AutoSelectMap(int d);      //also returns total matches in int.
	void __fastcall FindMapMatch(void);
    int  __fastcall GetMTMap_matches(int d);
	void __fastcall DisplayMT_usage(int idX, int idY);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormMetatileTool *FormMetatileTool;
//---------------------------------------------------------------------------
#endif
