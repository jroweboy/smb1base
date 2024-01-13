//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitSetSize.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSetSize *FormSetSize;
//---------------------------------------------------------------------------
__fastcall TFormSetSize::TFormSetSize(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSetSize::ButtonOKClick(TObject *Sender)
{
	Confirm=true;

	if(RadioButtonNormal->Checked)
	{
		NewWidth =32;
		NewHeight=30;
	}
	else
	{
		NewWidth =UpDownWidth ->Position;
		NewHeight=UpDownHeight->Position;
	}

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSetSize::ButtonCancelClick(TObject *Sender)
{
	Confirm=false;

	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TFormSetSize::FormShow(TObject *Sender)
{
	UpDownWidth ->Max=NAME_MAX_WIDTH;
    UpDownHeight->Max=NAME_MAX_HEIGHT;
	UpDownWidth ->Position=NewWidth;
	UpDownHeight->Position=NewHeight;
}
//---------------------------------------------------------------------------


void __fastcall TFormSetSize::EditWidthKeyPress(TObject *Sender, char &Key)
{
	if(!((Key>='0'&&Key<='9')||Key==VK_BACK||Key==VK_DELETE)) Key=0;	
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::EditWidthExit(TObject *Sender)
{
	int n;

	if(!TryStrToInt(EditWidth->Text,n)) n=0;

	if(n<4) n=4;

	if(n>NAME_MAX_WIDTH) n=NAME_MAX_WIDTH;

	//n=(n+3)/4*4;

	EditWidth->Text=IntToStr(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::EditHeightExit(TObject *Sender)
{
	int n;

	if(!TryStrToInt(EditHeight->Text,n)) n=0;

	if(n<4) n=4;

	if(n>NAME_MAX_HEIGHT) n=NAME_MAX_HEIGHT;

	//if (n!=30) n=(n+3)/4*4;
	//if (n!=30) n=(n+1)/2*2;


	EditHeight->Text=IntToStr(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::RadioButtonNormalClick(TObject *Sender)
{
	UpDownWidth ->Enabled=RadioButtonUser->Checked;
	UpDownHeight->Enabled=RadioButtonUser->Checked;
	EditWidth   ->Enabled=RadioButtonUser->Checked;
	EditHeight  ->Enabled=RadioButtonUser->Checked;
	BtnWdtInc   ->Enabled=RadioButtonUser->Checked;
	BtnWdtDec   ->Enabled=RadioButtonUser->Checked;
	BtnHgtInc   ->Enabled=RadioButtonUser->Checked;
	BtnHgtDec   ->Enabled=RadioButtonUser->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::FormCreate(TObject *Sender)
{
	NewWidth=256;
	NewHeight=256;

	RadioButtonNormalClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::EditWidthClick(TObject *Sender)
{
	((TEdit*)Sender)->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::BtnWdtIncClick(TObject *Sender)
{
	int n = StrToInt(EditWidth->Text);

	if(n<=31) n=32; else n=n+32;
	if(n>NAME_MAX_WIDTH) n=NAME_MAX_WIDTH;

	EditWidth->Text=IntToStr(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::BtnWdtDecClick(TObject *Sender)
{
	int n = StrToInt(EditWidth->Text);

	if(n<=32) n=4; else n=n-32;
	if(n>NAME_MAX_WIDTH) n=NAME_MAX_WIDTH;

	EditWidth->Text=IntToStr(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::BtnHgtIncClick(TObject *Sender)
{
	int n = StrToInt(EditHeight->Text);

	if(n<=29) n=30; else n=n+30;
	if(n>NAME_MAX_HEIGHT) n=NAME_MAX_HEIGHT;

	EditHeight->Text=IntToStr(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::BtnHgtDecClick(TObject *Sender)
{
	int n = StrToInt(EditHeight->Text);

	if(n<=30) n=4; else n=n-30;
	if(n>NAME_MAX_HEIGHT) n=NAME_MAX_HEIGHT;

	EditHeight->Text=IntToStr(n);
}
//---------------------------------------------------------------------------

