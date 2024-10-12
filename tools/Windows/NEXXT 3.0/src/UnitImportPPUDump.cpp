//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitImportPPUDump.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormImportPPUDump *FormImportPPUDump;
//---------------------------------------------------------------------------
__fastcall TFormImportPPUDump::TFormImportPPUDump(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormImportPPUDump::Button1Click(TObject *Sender)
{
	OK=true;
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TFormImportPPUDump::Button2Click(TObject *Sender)
{
	OK=false;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormImportPPUDump::FormCreate(TObject *Sender)
{
	OK=false;	
}
//---------------------------------------------------------------------------
