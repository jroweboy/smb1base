//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitNewInstance.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormNewInstance *FormNewInstance;
//---------------------------------------------------------------------------
__fastcall TFormNewInstance::TFormNewInstance(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
