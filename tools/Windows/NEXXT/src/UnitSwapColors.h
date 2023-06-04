//---------------------------------------------------------------------------

#ifndef UnitSwapColorsH
#define UnitSwapColorsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormSwapColors : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TRadioButton *RadioButton4K;
	TRadioButton *RadioButton8K;
	TRadioButton *RadioButtonSelection;
	TButton *ButtonSwap;
	TButton *ButtonCancel;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TButton *ButtonReset;
	TButton *ButtonDarker;
	TButton *ButtonBrighter;
	TRadioButton *RadioPatternNone;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TButton *Button5;
	TGroupBox *GroupBox4;
	TButton *Button6;
	TCheckBox *CheckBox1;
	TGroupBox *GroupBox5;
	TButton *ButtonCol0;
	TRadioButton *RadioPalCurrent;
	TRadioButton *RadioPalAll;
	TRadioButton *RadioPalOne;
	TRadioButton *RadioPalNone;
	TButton *ButtonWhichSubpal;
	TButton *Button7;
	TButton *Button8;
	void __fastcall ButtonSwapClick(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall RadioButton4KClick(TObject *Sender);
	void __fastcall CheckBoxPalClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall ButtonResetClick(TObject *Sender);
	void __fastcall ButtonDarkerClick(TObject *Sender);
	void __fastcall ButtonBrighterClick(TObject *Sender);
	void __fastcall CheckBoxPalMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall RadioPalCurrentMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall RadioPalCurrentMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ButtonResetMouseEnter(TObject *Sender);
	void __fastcall ButtonResetMouseLeave(TObject *Sender);
	void __fastcall Button3MouseEnter(TObject *Sender);
	void __fastcall ButtonWhichSubpalMouseEnter(TObject *Sender);
	void __fastcall ButtonCol0MouseEnter(TObject *Sender);
	void __fastcall ButtonDarkerMouseEnter(TObject *Sender);
	void __fastcall ButtonBrighterMouseEnter(TObject *Sender);
	void __fastcall Button7MouseEnter(TObject *Sender);
	void __fastcall Button8MouseEnter(TObject *Sender);
	void __fastcall Button5MouseEnter(TObject *Sender);
	void __fastcall Button6MouseEnter(TObject *Sender);
	void __fastcall ButtonCancelMouseEnter(TObject *Sender);
	void __fastcall ButtonSwapMouseEnter(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSwapColors(TComponent* Owner);
	void __fastcall PreviewSwap();

	int Map[4];
	bool Selection;
	bool WholeCHR;
	bool RemapPalette;
	bool Swap;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSwapColors *FormSwapColors;
//---------------------------------------------------------------------------
#endif
