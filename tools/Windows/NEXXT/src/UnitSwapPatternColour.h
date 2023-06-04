//---------------------------------------------------------------------------

#ifndef UnitSwapPatternColourH
#define UnitSwapPatternColourH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormSwapPatternColour : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox2;
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
	TButton *Button5;
	TButton *Button6;
	TCheckBox *CheckBox1;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TButton *Button11;
	TButton *Button12;
	void __fastcall ButtonSwapClick(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall ButtonResetClick(TObject *Sender);
	void __fastcall ButtonDarkerClick(TObject *Sender);
	void __fastcall ButtonBrighterClick(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall RadioButton4KClick(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSwapPatternColour(TComponent* Owner);
	void __fastcall PreviewSwap();
	int Map[4];
	bool Selection;
	bool WholeCHR;
	bool RemapPalette;
	bool Swap;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormSwapPatternColour *FormSwapPatternColour;
//---------------------------------------------------------------------------
#endif
