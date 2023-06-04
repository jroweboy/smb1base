//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "UnitImportBitmap.h"
#include "UnitMain.h"
#include "UnitLossyDetails.h"
#include "UnitNavigator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "UnitSwapColors"
#pragma resource "*.dfm"
TFormImportBMP *FormImportBMP;

 bool bNoAttr;
 bool bNoPal;
 bool bDensityThres;
 bool bMaxTiles;
 bool bLossy;
 bool bBestOffsets;
 int mode;
 int iMT;
 int iPT;

 extern bool cueUpdateNametable;
 extern bool cueUpdateMetasprite;

extern int nameTableWidth;
extern int buf_nameWidth;
extern int nameTableHeight;
extern int buf_nameHeight;

extern unsigned char chr[8192];
extern unsigned char chrBuf[8192];
extern unsigned char chrImportBuf [8192];

extern unsigned char nameTable[NAME_MAX_SIZE];
extern unsigned char attrTable[ATTR_MAX_SIZE];
extern unsigned char tmpNameTable[NAME_MAX_SIZE];
extern unsigned char tmpAttrTable[ATTR_MAX_SIZE];

extern int importBMP_tile_count;
extern int importBMP_tile_count_reduced;

extern unsigned char palBuf[4*16];
extern unsigned char bgPal[4*16];
extern unsigned char palImportBuf[4*16];
 enum {
	IMPORT_NORMAL=0,
	IMPORT_SQUEEZE,
	IMPORT_MATCH
};

//---------------------------------------------------------------------------
__fastcall TFormImportBMP::TFormImportBMP(TComponent* Owner)
	: TFormSwapColors(Owner)
{
}
//---------------------------------------------------------------------------

__fastcall TFormImportBMP::PreviewImport(void)
{
	//refresh
	memcpy (chr, chrBuf, 4096*2);
	memcpy (bgPal, palBuf, 4*16);
	memcpy (nameTable, tmpNameTable, NAME_MAX_SIZE);
	memcpy (attrTable, tmpAttrTable, ATTR_MAX_SIZE);
    nameTableWidth  = buf_nameWidth;
	nameTableHeight = buf_nameHeight;
	//enable/disable options depending on method
	CheckLossy->Enabled!=RadioMatched->Checked;


	//update arguments
	bNoAttr = FormImportBMP->CheckNoAttr->Checked;
	bNoPal = FormImportBMP->CheckNoPal->Checked;
	bDensityThres =FormImportBMP->CheckDensityThres->Checked;
	bLossy = FormImportBMP->CheckLossy->Checked;
	bBestOffsets = FormImportBMP->CheckBestOffsets->Checked;
	bMaxTiles = FormImportBMP->CheckMaxTiles->Checked;
	iMT = StrToInt(FormImportBMP->EditMaxTiles->Text);
	iPT = StrToInt(FormImportBMP->EditPxThres->Text);
	if(bLossy) mode=IMPORT_SQUEEZE;
	else mode=IMPORT_NORMAL;
	if(RadioMatched->Checked) mode=IMPORT_MATCH;

	//run import routine
	if(CheckBox1->Checked)
	{

		if(FormMain->ImportBMP(FormMain->OpenDialogImport->FileName,mode,bBestOffsets,bDensityThres?iPT:-1,bMaxTiles?iMT:256,bNoAttr,bNoPal))
		{
			memcpy (chrImportBuf, chr, 4096*2);
			memcpy (palImportBuf, bgPal, 4*16);

			if(importBMP_tile_count_reduced<importBMP_tile_count)
				Label1->Caption="# tiles: "+IntToStr(importBMP_tile_count_reduced)+"/"+IntToStr(importBMP_tile_count);
			else
				Label1->Caption="# tiles: "+IntToStr(importBMP_tile_count);
		}
		else //invalid bmp
		{
			//Close();
			FormImportBMP->CloseModal();
		}
		//call pass 2
		PreviewSwap();
	}
	else //refresh canvases w/o preview
	{
		FormMain->pal_validate();
		FormMain->UpdateTiles(true);
	   //	cueUpdateNametable=true;
		FormMain->DrawPalettes();
		cueUpdateMetasprite=true;
		FormMain->CorrectView();

	}
    FormNavigator->Draw(false,false);
	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->CorrectView();
	Repaint();            //FormSwapColors->



	return false;
}
void __fastcall TFormImportBMP::CheckBestOffsetsClick(TObject *Sender)
{
	PreviewImport();
}
//---------------------------------------------------------------------------


void __fastcall TFormImportBMP::FormShow(TObject *Sender)
{
  FormMain->BlockDrawing(false);
  PreviewImport();
}
//---------------------------------------------------------------------------

void __fastcall TFormImportBMP::CheckBox1Click(TObject *Sender)
{
	PreviewImport();
}
//---------------------------------------------------------------------------



void __fastcall TFormImportBMP::Button9Click(TObject *Sender)
{
	FormLossyDetails->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormImportBMP::BtnWdtIncClick(TObject *Sender)
{
	int n = StrToInt(EditMaxTiles->Text);

	if(n<=1) n=16; else n=n+16;

	EditMaxTiles->Text=IntToStr(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormImportBMP::SpeedButton1Click(TObject *Sender)
{
	int n = StrToInt(EditMaxTiles->Text);

	if(n<=16) n=1; else n=n-16;

	EditMaxTiles->Text=IntToStr(n);
}
//---------------------------------------------------------------------------

