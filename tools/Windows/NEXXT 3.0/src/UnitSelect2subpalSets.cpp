//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSelect2subpalSets.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSelect2subpalSets *FormSelect2subpalSets;

//---------------------------------------------------------------------------
__fastcall TFormSelect2subpalSets::TFormSelect2subpalSets(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSelect2subpalSets::btnOKClick(TObject *Sender)
{
	bProceed=true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSelect2subpalSets::BtnCancelClick(TObject *Sender)
{
	bProceed=false;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSelect2subpalSets::FormShow(TObject *Sender)
{
	bProceed=false;	
}
//---------------------------------------------------------------------------

