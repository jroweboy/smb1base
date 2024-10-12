//---------------------------------------------------------------------------

#ifndef UnitNewInstanceH
#define UnitNewInstanceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormNewInstance : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TRadioButton *RadioButton3;
	TRadioButton *RadioButton4;
	TRadioButton *RadioButton5;
	TRadioButton *RadioButton6;
	TCheckBox *CheckBox5;
	TGroupBox *GroupBox3;
	TRadioButton *RadioButton7;
	TRadioButton *RadioButton8;
	TRadioButton *RadioButton9;
	TRadioButton *RadioButton10;
	TRadioButton *RadioButton11;
	TRadioButton *RadioButton12;
	TGroupBox *GroupBox4;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox4;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TCheckBox *CheckBox6;
	TCheckBox *CheckBox7;
	TCheckBox *CheckBox8;
private:	// User declarations
public:		// User declarations
	__fastcall TFormNewInstance(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormNewInstance *FormNewInstance;
//---------------------------------------------------------------------------
#endif
