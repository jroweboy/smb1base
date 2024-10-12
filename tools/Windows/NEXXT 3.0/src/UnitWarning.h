//---------------------------------------------------------------------------

#ifndef UnitWarningH
#define UnitWarningH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormWarning : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TCheckBox *CheckBox1;
	TButton *Button1;
	TStaticText *StaticText1;
	TButton *Button2;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TFormWarning(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormWarning *FormWarning;
//---------------------------------------------------------------------------
#endif
