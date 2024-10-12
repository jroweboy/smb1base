//---------------------------------------------------------------------------

#ifndef UnitScanlineWarningsH
#define UnitScanlineWarningsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormScanlineWarnings : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TSpeedButton *btnCyan;
	TLabel *Label1;
	TSpeedButton *btnRed;
	TSpeedButton *btnYellow;
	TSpeedButton *btnOrange;
	TUpDown *UpDown1;
	TGroupBox *GroupBox2;
	TTrackBar *TrkTop;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TTrackBar *TrkBottom;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	void __fastcall btnYellowMouseEnter(TObject *Sender);
	void __fastcall btnOrangeMouseEnter(TObject *Sender);
	void __fastcall btnRedMouseEnter(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall UpDown1Changing(TObject *Sender, bool &AllowChange);
	void __fastcall btnCyanMouseEnter(TObject *Sender);
	void __fastcall btnCyanClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TrkTopChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormScanlineWarnings(TComponent* Owner);
    void __fastcall RunUpdate(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormScanlineWarnings *FormScanlineWarnings;
//---------------------------------------------------------------------------
#endif
