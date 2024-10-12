//---------------------------------------------------------------------------

#ifndef UnitSwapBanksH
#define UnitSwapBanksH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormSwapBanks : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox1;
	TListBox *ListBox2;
	TCheckBox *chkInclLabel;
	TButton *Button2;
	TImage *Image1;
	TImage *Image2;
	TCheckBox *chkInclProps;
	TButton *btnSwap;
	TButton *btnClone;
	TButton *btnMove;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ListBox2Click(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Image2MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall Image2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnSwapClick(TObject *Sender);
	void __fastcall btnCloneClick(TObject *Sender);
	void __fastcall btnMoveClick(TObject *Sender);
	void __fastcall Image1MouseEnter(TObject *Sender);
	void __fastcall Image1MouseLeave(TObject *Sender);
	void __fastcall Image2MouseEnter(TObject *Sender);
	void __fastcall Image2MouseLeave(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSwapBanks(TComponent* Owner);
	void __fastcall  UpdateLists(bool keepSel);
	void __fastcall	 FullUpdate(bool keepSel);
	void __fastcall  UpdateCanvas1(void);
	void __fastcall  UpdateCanvas2(void);
	void __fastcall  Swap4k(int mode);
	void __fastcall  SwapSelection(int mode);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSwapBanks *FormSwapBanks;
//---------------------------------------------------------------------------
#endif
