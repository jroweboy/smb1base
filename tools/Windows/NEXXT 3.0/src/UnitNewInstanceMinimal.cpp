//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitNewInstanceMinimal.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormNewInstanceMinimal *FormNewInstanceMinimal;

extern int nameTableWidth;
extern int nameTableHeight;

int tmpW;
int tmpH;
bool bCreate=false;
//---------------------------------------------------------------------------
__fastcall TFormNewInstanceMinimal::TFormNewInstanceMinimal(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormNewInstanceMinimal::SizeEnableDisable(void)
{
	bool bLock =  (chkInheritMap->Checked || RadioClone->Checked);

	if(bLock){
		tmpH=UpDownHeight->Position;
		tmpW=UpDownWidth->Position;
        UpDownHeight->Position=nameTableHeight;
		UpDownWidth->Position=nameTableWidth;
	}
	else{
		UpDownHeight->Position=tmpH;
		UpDownWidth->Position=tmpW;
	}


	UpDownWidth->Enabled=bLock? false:true;
	UpDownHeight->Enabled=bLock? false:true;
	EditWidth->Enabled=bLock? false:true;
	EditHeight->Enabled=bLock? false:true;
	BtnWdtInc->Enabled=bLock? false:true;
	BtnWdtDec->Enabled=bLock? false:true;
	BtnHgtInc->Enabled=bLock? false:true;
	BtnHgtDec->Enabled=bLock? false:true;
	BtnThisSession->Enabled=bLock? false:true;
	Btn32x30->Enabled=bLock? false:true;


}
//---------------------------------------------------------------------------
void __fastcall TFormNewInstanceMinimal::FormShow(TObject *Sender)
{
	UpDownWidth->Position=nameTableWidth;
	UpDownHeight->Position=nameTableHeight;
	tmpH=UpDownHeight->Position;   
	tmpW=UpDownWidth->Position;
	SizeEnableDisable();
}
//---------------------------------------------------------------------------
void __fastcall TFormNewInstanceMinimal::BtnWdtIncClick(TObject *Sender)
{
	int n = UpDownWidth->Position;

	if(n<=31) n=32; else n=n+32;
	if(n>NAME_MAX_WIDTH) n=NAME_MAX_WIDTH;

	UpDownWidth->Position=n;
}
//---------------------------------------------------------------------------
void __fastcall TFormNewInstanceMinimal::BtnWdtDecClick(TObject *Sender)
{
	int n = UpDownWidth->Position;

	if(n<=32) n=4; else n=n-32;
	if(n>NAME_MAX_WIDTH) n=NAME_MAX_WIDTH;

	UpDownWidth->Position=n;
}
//---------------------------------------------------------------------------
void __fastcall TFormNewInstanceMinimal::BtnHgtIncClick(TObject *Sender)
{
	int n = UpDownHeight->Position;

	if(n<=29) n=30; else n=n+30;
	if(n>NAME_MAX_HEIGHT) n=NAME_MAX_HEIGHT;

	UpDownHeight->Position=n;
}
//---------------------------------------------------------------------------
void __fastcall TFormNewInstanceMinimal::BtnHgtDecClick(TObject *Sender)
{
	int n = UpDownHeight->Position;

	if(n<=30) n=4; else n=n-30;
	if(n>NAME_MAX_HEIGHT) n=NAME_MAX_HEIGHT;

	UpDownHeight->Position=n;
}
//---------------------------------------------------------------------------
void __fastcall TFormNewInstanceMinimal::BtnThisSessionClick(TObject *Sender)
{
   	UpDownWidth->Position=nameTableWidth;
	UpDownHeight->Position=nameTableHeight;
}
//---------------------------------------------------------------------------

void __fastcall TFormNewInstanceMinimal::Btn32x30Click(TObject *Sender)
{
	UpDownWidth->Position=32;
	UpDownHeight->Position=30;
}
//---------------------------------------------------------------------------

void __fastcall TFormNewInstanceMinimal::chkInheritMapClick(TObject *Sender)
{
	SizeEnableDisable();
}
//---------------------------------------------------------------------------

void __fastcall TFormNewInstanceMinimal::RadioCloneClick(TObject *Sender)
{
	SizeEnableDisable();	
}
//---------------------------------------------------------------------------

void __fastcall TFormNewInstanceMinimal::RadioNewClick(TObject *Sender)
{
	SizeEnableDisable();	
}
//---------------------------------------------------------------------------

