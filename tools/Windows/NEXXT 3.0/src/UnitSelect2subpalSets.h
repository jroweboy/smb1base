//---------------------------------------------------------------------------

#ifndef UnitSelect2subpalSetsH
#define UnitSelect2subpalSetsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSelect2subpalSets : public TForm
{
__published:	// IDE-managed Components
	TButton *btnOK;
	TButton *BtnCancel;
	TGroupBox *GroupBox1;
	TSpeedButton *btn1a;
	TSpeedButton *btn1b;
	TSpeedButton *btn1c;
	TSpeedButton *btn1d;
	TGroupBox *GroupBox2;
	TSpeedButton *btn2a;
	TSpeedButton *btn2b;
	TSpeedButton *btn2c;
	TSpeedButton *btn2d;
	void __fastcall btnOKClick(TObject *Sender);
	void __fastcall BtnCancelClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	bool bProceed;
	__fastcall TFormSelect2subpalSets(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSelect2subpalSets *FormSelect2subpalSets;
//---------------------------------------------------------------------------
#endif
