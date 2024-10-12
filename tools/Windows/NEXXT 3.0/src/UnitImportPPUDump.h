//---------------------------------------------------------------------------

#ifndef UnitImportPPUDumpH
#define UnitImportPPUDumpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormImportPPUDump : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TCheckBox *CheckCHR;
	TCheckBox *CheckName;
	TCheckBox *CheckSubpal;
	TGroupBox *GroupBox2;
	TRadioButton *RadioAll;
	TRadioButton *RadioHorz;
	TRadioButton *RadioVert;
	TRadioButton *Radio2000;
	TRadioButton *Radio2400;
	TRadioButton *Radio2800;
	TRadioButton *Radio2C00;
	TGroupBox *GroupBox3;
	TRadioButton *Radio8k;
	TRadioButton *Radio0000;
	TRadioButton *Radio1000;
	TGroupBox *GroupBox4;
	TRadioButton *RadioPalBoth;
	TRadioButton *RadioPalBG;
	TRadioButton *RadioPalSPR;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormImportPPUDump(TComponent* Owner);
	bool OK;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormImportPPUDump *FormImportPPUDump;
//---------------------------------------------------------------------------
#endif
