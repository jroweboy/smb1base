//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitBrush.h"
#include "UnitMain.h"
#include "UnitCHREditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBrush *FormBrush;
extern bool bBrushMask[4];
extern bool bBrushMaskBuffer[4];
extern bool bSnapToScreen;

inline const char * const BoolToString(bool b)
{
	return b?"true":"false";
}
//---------------------------------------------------------------------------
__fastcall TFormBrush::TFormBrush(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TFormBrush::FormCreate(TObject *Sender)
{
	if(FormBrush->Position==poDesigned)
			{
			FormBrush->Left=(Screen->Width+FormMain->Width)/2;
			FormBrush->Top=(Screen->Height-FormMain->Height)/2+FormCHREditor->Height;
			//FormCHREditor->Position=poDesigned;
			//FormCHREditor->Left=(curMainWinPos.left+FormMain->Width);
			//FormCHREditor->Top=(curMainWinPos.top);
			//curMainWinPos
			/*TPoint P;
			P = Mouse->CursorPos;
			FormCHREditor->Left=P.x;
			FormCHREditor->Top=P.y;*/
			}
}
//---------------------------------------------------------------------------
void __fastcall TFormBrush::DrawCell(int iX,int iY,int maskID)
{
	int x,y;
	int size=32;
	int cell=size-1;
	TRect r;

	x=iX*size;
	y=iY*size;

	if(bBrushMask[maskID]==true) PaintBoxMask->Canvas->Brush->Color=TColor(0x00FFFFFF);
	else        PaintBoxMask->Canvas->Brush->Color=TColor(0x00000000);
	r.left=x;
	r.top=y;
	r.right=x+cell;
	r.Bottom=y+cell;

	PaintBoxMask->Canvas->FillRect(r);

	}

//---------------------------------------------------------------------------
void __fastcall TFormBrush::Draw()
{
int i,x,y;
	i=0;
	for (y = 0; y < 2; y++) {
			for (x = 0; x < 2; x++) {
				DrawCell(x,y,	i);
				i++;
			}
	}

}

//---------------------------------------------------------------------------
void __fastcall TFormBrush::PaintBoxMaskPaint(TObject *Sender)
{
	Draw();
}
//---------------------------------------------------------------------------


void __fastcall TFormBrush::PaintBoxMaskMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   memcpy	(bBrushMaskBuffer,bBrushMask,4);

   if(Shift.Contains(ssLeft))
   {
		if ((X<32)&&(Y<32)) bBrushMask[0]^=true;
		if ((X>=32)&&(Y<32)) bBrushMask[1]^=true;
		if ((X<32)&&(Y>=32)) bBrushMask[2]^=true;
		if ((X>=32)&&(Y>=32)) bBrushMask[3]^=true;
   }
   //FormMain->LabelStats->Caption=BoolToString(bBrushMask[0]);
   Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	FormCHREditor->FormKeyDown(Sender,Key,Shift);	
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::FormActivate(TObject *Sender)
{
	FormBrush->ScreenSnap=bSnapToScreen;	
}
//---------------------------------------------------------------------------



void __fastcall TFormBrush::PaintBoxMaskMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if(Shift.Contains(ssLeft))
   {
		if ((X<32)&&(Y<32)) if(bBrushMask[0]==bBrushMaskBuffer[0])
			bBrushMask[0]^=true;

		if ((X>=32)&&(Y<32)) if(bBrushMask[1]==bBrushMaskBuffer[1])
			bBrushMask[1]^=true;

		if ((X<32)&&(Y>=32)) if(bBrushMask[2]==bBrushMaskBuffer[2])
			bBrushMask[2]^=true;

		if ((X>=32)&&(Y>=32)) if(bBrushMask[3]==bBrushMaskBuffer[3])
			bBrushMask[3]^=true;
   }
   //FormMain->LabelStats->Caption=BoolToString(bBrushMask[0]);
   BrushmaskTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::BrushmaskTimerTimer(TObject *Sender)
{
	BrushmaskTimer->Enabled=false;
	Draw();
}
//---------------------------------------------------------------------------

