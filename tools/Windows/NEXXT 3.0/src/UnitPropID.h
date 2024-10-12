//---------------------------------------------------------------------------

#ifndef UnitPropIDH
#define UnitPropIDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormPropID : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox1;
	TEdit *Edit1;
	TLabel *Label1;
	TTrackBar *TrackR;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TTrackBar *TrackG;
	TTrackBar *TrackB;
	TGroupBox *GroupBox1;
	TSpeedButton *btnSave;
	TSpeedButton *btnLoad;
	TGroupBox *GroupBox2;
	TSpeedButton *btnHold;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TSpeedButton *btnHov;
	TSpeedButton *btnSel;
	TSpeedButton *btnAll;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ListBox1DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall TrackRChange(TObject *Sender);
	void __fastcall TrackGChange(TObject *Sender);
	void __fastcall TrackBChange(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall Edit1Click(TObject *Sender);
	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit1Enter(TObject *Sender);
	void __fastcall Edit1Exit(TObject *Sender);
	void __fastcall ListBox1MouseEnter(TObject *Sender);
	void __fastcall ListBox1MouseLeave(TObject *Sender);
	void __fastcall Label1MouseEnter(TObject *Sender);
	void __fastcall Label2MouseEnter(TObject *Sender);
	void __fastcall btnHoldMouseEnter(TObject *Sender);
	void __fastcall btnTilesMouseEnter(TObject *Sender);
	void __fastcall btnMetasMouseEnter(TObject *Sender);
	void __fastcall btnSaveMouseEnter(TObject *Sender);
	void __fastcall btnLoadMouseEnter(TObject *Sender);
	void __fastcall btnLoadClick(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ListBox1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall btnHovClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormPropID(TComponent* Owner);
	void __fastcall MakeList(void);
    void __fastcall UpdateUI(void);
	void __fastcall SetCHRprops(unsigned char id);
	void __fastcall SetMTprops(unsigned char id);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPropID *FormPropID;
//---------------------------------------------------------------------------
#endif
