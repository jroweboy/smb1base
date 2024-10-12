//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitSetSize.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSetSize *FormSetSize;
extern int nameTableWidth;
extern int nameTableHeight;
//---------------------------------------------------------------------------
__fastcall TFormSetSize::TFormSetSize(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSetSize::ButtonOKClick(TObject *Sender)
{
	Confirm=true;
	/*
	if(RadioButtonNormal->Checked)
	{
		NewWidth =32;
		NewHeight=30;
	}
	else
	{        */
	   //	NewWidth =UpDownWidth ->Position;
	   //	NewHeight=UpDownHeight->Position;
	//}

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
	UpDownWidth ->Position=nameTableWidth;
	UpDownHeight->Position=nameTableHeight;
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



void __fastcall TFormSetSize::EditWidthClick(TObject *Sender)
{
	((TEdit*)Sender)->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::BtnWdtIncClick(TObject *Sender)
{
	int n = UpDownWidth->Position;

	if(n<=31) n=32; else n=n+32;
	if(n>NAME_MAX_WIDTH) n=NAME_MAX_WIDTH;

	UpDownWidth->Position=n;

}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::BtnWdtDecClick(TObject *Sender)
{
	 int n = UpDownWidth->Position;

	if(n<=32) n=4; else n=n-32;
	if(n>NAME_MAX_WIDTH) n=NAME_MAX_WIDTH;

	UpDownWidth->Position=n;

}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::BtnHgtIncClick(TObject *Sender)
{
	int n = UpDownHeight->Position;

	if(n<=29) n=30; else n=n+30;
	if(n>NAME_MAX_HEIGHT) n=NAME_MAX_HEIGHT;

	UpDownHeight->Position=n;
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::BtnHgtDecClick(TObject *Sender)
{
	int n = UpDownHeight->Position;

	if(n<=30) n=4; else n=n-30;
	if(n>NAME_MAX_HEIGHT) n=NAME_MAX_HEIGHT;

	UpDownHeight->Position=n;
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::BtnThisSessionClick(TObject *Sender)
{
	UpDownWidth->Position=nameTableWidth;
	UpDownHeight->Position=nameTableHeight;
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::Btn32x30Click(TObject *Sender)
{
	UpDownWidth->Position=32;
	UpDownHeight->Position=30;
}
//---------------------------------------------------------------------------

void __fastcall TFormSetSize::SpeedButton1Click(TObject *Sender)
{
	UpDownWidth->Position=64;
	UpDownHeight->Position=60;
}
//---------------------------------------------------------------------------

