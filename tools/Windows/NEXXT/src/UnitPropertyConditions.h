//---------------------------------------------------------------------------

#ifndef UnitPropertyConditionsH
#define UnitPropertyConditionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormPropConditions : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TCheckBox *chk0;
	TCheckBox *chk1;
	TCheckBox *chk2;
	TCheckBox *chk3;
	TCheckBox *chkTiles;
	TCheckBox *chkMap;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall chk0Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall chkTilesClick(TObject *Sender);
	void __fastcall chkMapClick(TObject *Sender);
	void __fastcall chkMapMouseEnter(TObject *Sender);
	void __fastcall chkMapMouseLeave(TObject *Sender);
	void __fastcall chkTilesMouseEnter(TObject *Sender);
	void __fastcall chk0MouseEnter(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormPropConditions(TComponent* Owner);
	void __fastcall SetConditions(void);


};
//---------------------------------------------------------------------------
extern PACKAGE TFormPropConditions *FormPropConditions;
//---------------------------------------------------------------------------
#endif
