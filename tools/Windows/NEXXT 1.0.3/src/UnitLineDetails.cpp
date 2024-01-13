//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitLineDetails.h"
#include "UnitMain.h"
#include "UnitCHREditor.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLineDetails *FormLineDetails;

extern bool lineRosterEnable[];
extern int lineDashLen;

extern int lineOffX;
extern int lineOffY;

//---------------------------------------------------------------------------
__fastcall TFormLineDetails::TFormLineDetails(TComponent* Owner)
	: TForm(Owner)
{
}

void UpdateRoster(void)
{
	lineRosterEnable[0]= FormLineDetails->btnRosterLine->Down;
	lineRosterEnable[1]= FormLineDetails->btnRosterCurve->Down;
	lineRosterEnable[2]= FormLineDetails->btnRosterKnee->Down;
	lineRosterEnable[3]= FormLineDetails->btnRosterAngle->Down;

}

//---------------------------------------------------------------------------
void __fastcall TFormLineDetails::btnRosterLineClick(TObject *Sender)
{
	if(    btnRosterLine->Down	==false
		&& btnRosterCurve->Down	==false
		&& btnRosterKnee->Down	==false
		&& btnRosterAngle->Down	==false ){ ((TSpeedButton*)Sender)->Down=true;}


	 UpdateRoster();




}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterLineMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			btnRosterLine->Down=true;    //bc click applies after, this one is inverted
			btnRosterCurve->Down=true;
			btnRosterKnee->Down=true;
			btnRosterAngle->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		btnRosterLine->Down=false;
		btnRosterCurve->Down=false;
		btnRosterKnee->Down=false;
		btnRosterAngle->Down=false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::FormCreate(TObject *Sender)
{
	UpdateRoster();
	int i = TrkDash->Position;
	int t1 = 10;    //thresholds
	int t2 = 15;

	if (i>t1) i = (i-t1) *2 +t1-1;
	if (i>t2) i = (i-t2) *4 +t2;
	LabelDashLen->Caption = IntToStr(i +1);
	lineDashLen = i;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::TrkDashChange(TObject *Sender)
{
	 int i = TrkDash->Position;
	 int t1 = 10;    //thresholds
	 int t2 = 15;

	 if (i>t1) i = (i-t1) *2 +t1-1;
	 if (i>t2) i = (i-t2) *4 +t2;
	 LabelDashLen->Caption = IntToStr(i +1);
     lineDashLen = i;
}
//---------------------------------------------------------------------------


void __fastcall TFormLineDetails::btnTaperInClick(TObject *Sender)
{
	if((btnTaperIn->Down && btnTaperOut->Down)) btnTaper2->Flat=true;
	else btnTaper2->Flat=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	FormCHREditor->FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	FormCHREditor->FormKeyUp(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::SpeedButton3Click(TObject *Sender)
{
	lineOffX=0;
	lineOffY=0;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnTaperInMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Taper in: the brush line gets tapered, thinning to a point at x0y0";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnTaperInMouseLeave(TObject *Sender)
{
	FormMain->LabelStats->Caption="---";	
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnTaperOutMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Taper in: the brush line gets tapered, thinning to a point at x2y2";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnTaper2MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="When engaged, taper in/out will have effect from midpoint-out,/n as opposed to over the whole line./nThis mode is automatically engaged if both taper in and taper out are set.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::CheckEnableBrushMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Brush line effects on/off.\nUseful for when you prefer to toggle between normal brush line and a particular combination of brush line effects.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnSmearMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Coat mode: When engaged, each move of the line commits to the asset.\nUseful creating rollerpaint-like motions. Combinable with [move].\nCan cover surfaces with a surprising amount of control.\nAlso handy together with x0y0 adjustments.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnMoveMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Move mode. Has two functions:\n1)Engaged mid-drag, you´ll be able to adjust the relative position of your line.\n2)If engaged during a new click, it lets you retouch a line placement nondestructively.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::SpeedButton3MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Centers x0y0 offset to its original position.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::CheckResetLineNudgeMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Normally checked. Uncheck if you want to reuse the x0y0 offset between multiple lines.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnDotsMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Places a dot where there is no modulo on the repeat length.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnDashesMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Places a dash where the is a modulo on the repeat leght.\nIf the repeat length is 2, that means a dot on even pixels.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::Label1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="'Repeat length' of the total dot + dash pattern.";
}
//---------------------------------------------------------------------------

