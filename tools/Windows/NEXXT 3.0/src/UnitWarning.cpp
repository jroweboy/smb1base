//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitWarning.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormWarning *FormWarning;
//---------------------------------------------------------------------------
__fastcall TFormWarning::TFormWarning(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormWarning::FormClose(TObject *Sender, TCloseAction &Action)
{
	//if (ModalResult == mrNone) {
   //		ModalResult = mrCancel;
   //	}
}
//---------------------------------------------------------------------------

