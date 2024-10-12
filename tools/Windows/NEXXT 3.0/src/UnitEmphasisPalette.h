//---------------------------------------------------------------------------

#ifndef UnitEmphasisPaletteH
#define UnitEmphasisPaletteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormEmphasisPalette : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TCheckBox *chkB0;
	TCheckBox *chkG0;
	TCheckBox *chkR0;
	TImage *ImageEmphPalette;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TGroupBox *GroupBox2;
	TCheckBox *chkB1;
	TCheckBox *chkG1;
	TCheckBox *chkR1;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TGroupBox *GroupBox3;
	TCheckBox *chkB2;
	TCheckBox *chkG2;
	TCheckBox *chkR2;
	TSpeedButton *SpeedButton5;
	TSpeedButton *SpeedButton6;
	TGroupBox *GroupBox4;
	TCheckBox *chkB3;
	TCheckBox *chkG3;
	TCheckBox *chkR3;
	TSpeedButton *SpeedButton7;
	TSpeedButton *SpeedButton8;
	TGroupBox *GroupBox5;
	TCheckBox *chkB4;
	TCheckBox *chkG4;
	TCheckBox *chkR4;
	TSpeedButton *SpeedButton9;
	TSpeedButton *SpeedButton10;
	TGroupBox *GroupBox6;
	TCheckBox *chkB5;
	TCheckBox *chkG5;
	TCheckBox *chkR5;
	TSpeedButton *SpeedButton11;
	TSpeedButton *SpeedButton12;
	TGroupBox *GroupBox7;
	TCheckBox *chkB6;
	TCheckBox *chkG6;
	TCheckBox *chkR6;
	TSpeedButton *SpeedButton13;
	TSpeedButton *SpeedButton14;
	TGroupBox *GroupBox8;
	TCheckBox *chkB7;
	TCheckBox *chkG7;
	TCheckBox *chkR7;
	TSpeedButton *SpeedButton15;
	TSpeedButton *SpeedButton16;
	TSpeedButton *SpeedButton17;
	TSpeedButton *SpeedButton18;
	TSpeedButton *SpeedButton19;
	TMaskEdit *MaskEdit1;
	TPaintBox *PaintBox1;
	TSpeedButton *SpeedButton20;
	TSpeedButton *SpeedButton21;
	TSaveDialog *SaveDialogPal;
	TSaveDialog *SaveDialogBitmap;
	TSaveDialog *SaveDialogFullPal;
	TSaveDialog *SaveDialogFullBitmap;
	TSpeedButton *SpeedButton22;
	TPopupMenu *PopupMenu1;
	TMenuItem *PutCarrayonclipboard1;
	TSpeedButton *SpeedButton23;
	TSpeedButton *SpeedButton24;
	TSpeedButton *SpeedButton25;
	TSpeedButton *SpeedButton26;
	TSpeedButton *SpeedButton27;
	TSpeedButton *SpeedButton28;
	TSpeedButton *SpeedButton29;
	TMenuItem *toclipboardasjavac1;
	TSpeedButton *SpeedButton30;
	TPopupMenu *PopupMenuMethod;
	TMenuItem *NESPPU1;
	TMenuItem *RGBPPUPlaychoiceetc1;
	TMenuItem *N1;
	TMenuItem *Nintendulator1;
	TMenuItem *Mesen1;
	TMenuItem *emulatorspecific1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall PaintBox1Paint(TObject *Sender);
	void __fastcall ImageEmphPaletteClick(TObject *Sender);
	void __fastcall ImageEmphPaletteMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall SpeedButton20Click(TObject *Sender);
	void __fastcall SpeedButton21Click(TObject *Sender);
	void __fastcall chkB0MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall chkG0MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall chkR0MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall SpeedButton18Click(TObject *Sender);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall SpeedButton19Click(TObject *Sender);
	void __fastcall SpeedButton22Click(TObject *Sender);
	void __fastcall PutCarrayonclipboard1Click(TObject *Sender);
	void __fastcall NESPPU1Click(TObject *Sender);
	void __fastcall RGBPPUPlaychoiceetc1Click(TObject *Sender);
	void __fastcall SpeedButton30Click(TObject *Sender);
	void __fastcall Nintendulator1Click(TObject *Sender);
	void __fastcall Mesen1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormEmphasisPalette(TComponent* Owner);
	void __fastcall MakePalette(void);
	void __fastcall DrawCol(int x,int y,int size,int c);
	void __fastcall Save192b(int tag);
	void __fastcall Save1536b(void);
	void __fastcall Save192b_BMP(int tag);
	void __fastcall Save1536b_BMP(void);

	void __fastcall HexToClip(void);
    void __fastcall ClipToHexEdit(void);
	void __fastcall SetPPUmaskArray(void);
	void __fastcall UpdateCheckboxes(void);
	void __fastcall UpdateCanvas(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEmphasisPalette *FormEmphasisPalette;
//---------------------------------------------------------------------------
#endif
