//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitLossyDetails.h"
#include "UnitMain.h"
#include "UnitImportBitmap.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLossyDetails *FormLossyDetails;
//---------------------------------------------------------------------------
__fastcall TFormLossyDetails::TFormLossyDetails(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLossyDetails::RadioFreqClick(TObject *Sender)
{
	FormImportBMP->PreviewImport();
}
//---------------------------------------------------------------------------

