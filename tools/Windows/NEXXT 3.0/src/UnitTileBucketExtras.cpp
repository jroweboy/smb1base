//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitTileBucketExtras.h"
#include "UnitBucketToolbox.h"
#include "UnitBrush.h"
#include "UnitCHREditor.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTileBucketExtras *FormTileBucketExtras;
//---------------------------------------------------------------------------
__fastcall TFormTileBucketExtras::TFormTileBucketExtras(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormTileBucketExtras::FormKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	 FormMain->FormKeyDown(Sender, Key, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TFormTileBucketExtras::FormKeyPress(TObject *Sender,
      char &Key)
{
	FormMain->FormKeyPress(Sender, Key);
}
//---------------------------------------------------------------------------
void __fastcall TFormTileBucketExtras::FormKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	FormMain->FormKeyUp(Sender, Key, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TFormTileBucketExtras::TrkReachChange(TObject *Sender)
{
	//Label1->Caption=IntToStr(TrkReach->Position);
	GroupBox6->Caption="Flood cap: "+IntToStr(TrkReach->Position);
}
//---------------------------------------------------------------------------

void __fastcall TFormTileBucketExtras::FormCreate(TObject *Sender)
{
	if(FormTileBucketExtras->Position==poDesigned)
	{
			FormTileBucketExtras->Left=(Screen->Width-FormMain->Width)/2.3+FormMain->Width - FormTileBucketExtras->Width;
			FormTileBucketExtras->Top=(Screen->Height-FormMain->Height)/4 + FormMain->Height;

	}
	GroupBox6->Caption="Flood cap: "+IntToStr(TrkReach->Position);
}
//---------------------------------------------------------------------------

void __fastcall TFormTileBucketExtras::CheckAlignToSelMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="When checked, if there is a box selection on the tileset and a box selection on the screen/map,\nthe pattern repeat will align to the screen/map selection rather than to the canvas.";
}
//---------------------------------------------------------------------------


void __fastcall TFormTileBucketExtras::CheckSubpaletteAwareMouseEnter(
      TObject *Sender)
{
	 FormMain->LabelStats->Caption="When checked, tile bucket treats 2 tiles of different palette attribute as unique.\nOn by default on startup.";
}
//---------------------------------------------------------------------------

void __fastcall TFormTileBucketExtras::btnSetMaxReachMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Turns flood cap on. It means the flood stops after a desired amount of tiles in all valid flood directions.";
}
//---------------------------------------------------------------------------

void __fastcall TFormTileBucketExtras::chkAutoCustomMouseEnter(
	  TObject *Sender)
{
	FormMain->LabelStats->Caption="When checked, if flood reach cap is turned on, tile bucket defaults to use custom flood directions.\nCustom flood directions are designed in the regular tile bucket toolbox [F6].";
}
//---------------------------------------------------------------------------

