//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitScanlineWarnings.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormScanlineWarnings *FormScanlineWarnings;
//---------------------------------------------------------------------------
__fastcall TFormScanlineWarnings::TFormScanlineWarnings(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

extern int  iWarnMsprCyan;
extern bool bWarnMsprCyan;
extern bool	bWarnMsprYellow;
extern bool	bWarnMsprOrange;
extern bool bWarnMsprRed;

bool programmaticUpDown=false;



void __fastcall TFormScanlineWarnings::btnYellowMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Yellow warning: Just 1 of this metasprite fits at scanline on a standard NES PPU.";
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::btnOrangeMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Orange warning: Metasprite is at scanline limit (8) on a standard NES PPU.";
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::btnRedMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Red warning: This metasprite alone causes flicker/cancellation on a standard NES PPU.";
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::FormCreate(TObject *Sender)
{

	Label1->Caption=IntToStr(iWarnMsprCyan);
	programmaticUpDown=true;
	UpDown1->Position=iWarnMsprCyan;
	programmaticUpDown=false;
	btnCyan->Down=bWarnMsprCyan;
	btnYellow->Down=bWarnMsprYellow;
	btnOrange->Down=bWarnMsprOrange;
	btnRed->Down=bWarnMsprRed;
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::UpDown1Changing(TObject *Sender,
	  bool &AllowChange)
{
	iWarnMsprCyan=UpDown1->Position;
	Label1->Caption=IntToStr(iWarnMsprCyan);
	if(!programmaticUpDown) RunUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::btnCyanMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Custom (cyan) warning: set a number of sprites.\nIf the metasprite exceeds that number, a cyan warning mark is placed on that scanline.";
}
//---------------------------------------------------------------------------
void __fastcall TFormScanlineWarnings::RunUpdate(void)
{
   iWarnMsprCyan=UpDown1->Position;
   bWarnMsprCyan=btnCyan->Down;
   bWarnMsprYellow=btnYellow->Down;
   bWarnMsprOrange=btnOrange->Down;
   bWarnMsprRed=btnRed->Down;
   FormMain->UpdateMetaSprite(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::btnCyanClick(TObject *Sender)
{
	RunUpdate();	
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::FormKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	FormMain->FormKeyDown(Sender, Key, Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::FormKeyPress(TObject *Sender,
      char &Key)
{
	FormMain->FormKeyPress(Sender, Key);
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::FormKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	FormMain->FormKeyUp(Sender, Key, Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormScanlineWarnings::TrkTopChange(TObject *Sender)
{
	FormMain->UpdateNameTable(-1,-1,true);
}
//---------------------------------------------------------------------------

