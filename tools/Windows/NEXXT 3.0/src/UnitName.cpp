//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitName.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormName *FormName;
extern bool bKeyEscape;
//---------------------------------------------------------------------------
__fastcall TFormName::TFormName(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormName::EditNameKeyPress(TObject *Sender, char &Key)
{
	if(Key==VK_RETURN||Key==VK_ESCAPE)
	{
		if(Key==VK_ESCAPE) bKeyEscape=true; else bKeyEscape=false;
		Key=0;
		Close();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormName::FormShow(TObject *Sender)
{
	//FormName->EditName->Text=RemoveExt(ExtractFileName(SaveDialogSession->FileName));
	//EditName->Text = FormMain->SaveDialogSession->FileName;
}
//---------------------------------------------------------------------------

