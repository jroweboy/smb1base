//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "UnitMain.h"
#include "UnitPropertyConditions.h"
#include "UnitCHRbit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPropConditions *FormPropConditions;
extern bool propCondition[4][8];
extern bool propShowCondMap;
extern bool propShowCondTile;
extern char propConditional[];
extern int conditionClicked;
//---------------------------------------------------------------------------
__fastcall TFormPropConditions::TFormPropConditions(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormPropConditions::SetConditions()
{
    GroupBox1->Caption="Cond. (bit "+IntToStr(conditionClicked)+")";

	chk0->Checked=propCondition[0][conditionClicked];
	chk1->Checked=propCondition[1][conditionClicked];
	chk2->Checked=propCondition[2][conditionClicked];
	chk3->Checked=propCondition[3][conditionClicked];
}

void __fastcall TFormPropConditions::FormShow(TObject *Sender)
{
	SetConditions();


}
//---------------------------------------------------------------------------
void __fastcall TFormPropConditions::chk0Click(TObject *Sender)
{
    FormMain->SetUndo();
	int t=((TCheckBox*)Sender)->Tag;
	propCondition[t][conditionClicked]=((TCheckBox*)Sender)->Checked;

	if(FormCHRbit->btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(FormCHRbit->btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
}
//---------------------------------------------------------------------------
void __fastcall TFormPropConditions::FormCreate(TObject *Sender)
{
	for(int i=0; i<8; i++) {
		propCondition[0][i] = true;
		propCondition[1][i] = true;
		propCondition[2][i] = false;
		propCondition[3][i] = false;

	}
}
//---------------------------------------------------------------------------
void __fastcall TFormPropConditions::chkTilesClick(TObject *Sender)
{
	if(FormCHRbit->btnShowCHR->Down) FormMain->UpdateTiles(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormPropConditions::chkMapClick(TObject *Sender)
{
	if(FormCHRbit->btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);	
}
//---------------------------------------------------------------------------

void __fastcall TFormPropConditions::chkMapMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Conditional rules apply to the highlights on the screen/map.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropConditions::chkMapMouseLeave(TObject *Sender)
{
	FormMain->LabelStats->Caption="---";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropConditions::chkTilesMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Conditional rules apply to highlights on the tileset.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropConditions::chk0MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Sets the requirements of the condition applying to this property bit.\nConditions only take take effect if the conditional button is set to 'yes' or 'all'.\nIf 'no', the requirements are ignored.";
}
//---------------------------------------------------------------------------

