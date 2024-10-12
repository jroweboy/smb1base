//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitUnsavedChanges.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormUnsavedChanges *FormUnsavedChanges;
//---------------------------------------------------------------------------
__fastcall TFormUnsavedChanges::TFormUnsavedChanges(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
