//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitLineDetails.h"
#include "UnitMain.h"
#include "UnitCHREditor.h"
#include "UnitBrush.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLineDetails *FormLineDetails;

extern bool prefStartShowLines;

extern bool lineRosterEnable[];
extern int lineToolRoster;
extern int lineDashLen;

extern int lineOffX;
extern int lineOffY;

extern bool bTrkLengthManual;
//line tool presets
extern int iLinePresetIndex;

extern bool bLinePreset_styleDots[];
extern bool bLinePreset_styleDashes[];
extern bool iLinePreset_styleRepeat[];

extern bool bLinePreset_taperIn[];
extern bool bLinePreset_taperOut[];
extern bool bLinePreset_taperFromMid[];
extern bool bLinePreset_taperEnabled[];

extern bool bLinePreset_modeQuick[];
extern bool bLinePreset_modeCoat[];

extern bool bLinePreset_rosterReset[];
extern bool bLinePreset_rosterAutosize[];
extern bool bLinePreset_rosterEnableLine[];
extern bool bLinePreset_rosterEnableCurve[];
extern bool bLinePreset_rosterEnableKnee[];
extern bool bLinePreset_rosterEnableRight[];

extern bool bLinePreset_rosterEnableRect[];
extern bool bLinePreset_rosterEnableEllipse[];
extern bool bLinePreset_rosterEnableHyperbola[];
extern bool bLinePreset_rosterEnableHyperline[];
extern bool bLinePreset_rosterEnableHypercave[];
extern int iLinePreset_rosterIndex[];

extern bool bLinePreset_adjustAutoreset[];




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
	lineRosterEnable[4]= FormLineDetails->btnRosterRectangle->Down;
	lineRosterEnable[5]= FormLineDetails->btnRosterEllipse->Down;
	lineRosterEnable[6]= FormLineDetails->btnRosterHyperbola->Down;
	lineRosterEnable[7]= FormLineDetails->btnRosterHyperline->Down;
	lineRosterEnable[8]= FormLineDetails->btnRosterHypercave->Down;

}

//---------------------------------------------------------------------------
void __fastcall TFormLineDetails::btnRosterLineClick(TObject *Sender)
{
	if(    btnRosterLine->Down		==false
		&& btnRosterCurve->Down		==false
		&& btnRosterKnee->Down		==false
		&& btnRosterAngle->Down		==false
		&& btnRosterRectangle->Down ==false
		&& btnRosterEllipse->Down	==false
		&& btnRosterHyperbola->Down ==false
		&& btnRosterHyperline->Down ==false
		&& btnRosterHypercave->Down ==false


	){ ((TSpeedButton*)Sender)->Down=true;}

    int i = iLinePresetIndex;

	bLinePreset_rosterEnableLine[i]=btnRosterLine->Down;
	bLinePreset_rosterEnableCurve[i]=btnRosterCurve->Down;
	bLinePreset_rosterEnableKnee[i]=btnRosterKnee->Down;
	bLinePreset_rosterEnableRight[i]=btnRosterAngle->Down;
	bLinePreset_rosterEnableRect[i]=btnRosterRectangle->Down;

	bLinePreset_rosterEnableEllipse[i]=btnRosterEllipse->Down;
	bLinePreset_rosterEnableHyperbola[i]=btnRosterHyperbola->Down;
	bLinePreset_rosterEnableHyperline[i]=btnRosterHyperline->Down;
	bLinePreset_rosterEnableHypercave[i]=btnRosterHypercave->Down;

    LinePresetSaveTimer->Enabled=true;
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

			btnRosterRectangle->Down=true;
			btnRosterEllipse->Down=true;

			btnRosterHyperbola->Down=true;
			btnRosterHyperline->Down=true;
			btnRosterHypercave->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		btnRosterLine->Down=false;
		btnRosterCurve->Down=false;
		btnRosterKnee->Down=false;
		btnRosterAngle->Down=false;

		btnRosterRectangle->Down=false;
		btnRosterEllipse->Down=false;

		btnRosterHyperbola->Down=false;
		btnRosterHyperline->Down=false;
		btnRosterHypercave->Down=false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::FormCreate(TObject *Sender)
{
	if(FormLineDetails->Position==poDesigned)
			{
			FormLineDetails->Left=(Screen->Width-FormMain->Width)/2.3 + FormMain->Width;
			FormLineDetails->Top=(Screen->Height-FormMain->Height)/4+FormCHREditor->Height+FormBrush->Height;

	}

    FormMain->LoadConfig();
	UpdateRoster();
	int i = TrkDash->Position;
	int t1 = 10;    //thresholds
	int t2 = 15;

	if (i>t1) i = (i-t1) *2 +t1-1;
	if (i>t2) i = (i-t2) *4 +t2;
	LabelDashLen->Caption = IntToStr(i +1);
	lineDashLen = i;

	if(prefStartShowLines==true) FormLineDetails->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::TrkDashChange(TObject *Sender)
{
	 if(bTrkLengthManual== true) return;

	 int i = TrkDash->Position;
	 int t1 = 10;    //thresholds
	 int t2 = 15;

	 if (i>t1) i = (i-t1) *2 +t1-1;
	 if (i>t2) i = (i-t2) *4 +t2;
	 LabelDashLen->Caption = IntToStr(i +1);
     lineDashLen = i;
	 iLinePreset_styleRepeat[iLinePresetIndex]=i;
	 LinePresetSaveTimer->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormLineDetails::btnTaperInClick(TObject *Sender)
{
	if((btnTaperIn->Down && btnTaperOut->Down)) btnTaperFromMid->Flat=true;
	else btnTaperFromMid->Flat=false;
	int i = iLinePresetIndex;
	bLinePreset_taperIn[i]= btnTaperIn->Down;
	bLinePreset_taperOut[i]= btnTaperOut->Down;
	LinePresetSaveTimer->Enabled=true;
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

void __fastcall TFormLineDetails::btnTaperFromMidMouseEnter(TObject *Sender)
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

void __fastcall TFormLineDetails::btnPresetAClick(TObject *Sender)
{
	TSpeedButton *speedButton = dynamic_cast<TSpeedButton*>(Sender);
	if (speedButton){
		iLinePresetIndex = speedButton->Tag;
	}
	GetPreset();

		switch(iLinePresetIndex)
	{
	case 1:
		FormMain->PresetA1->Checked=true;
		break;
	case 2:
		FormMain->PresetB1->Checked=true;
		break;
	case 3:
		FormMain->PresetC1->Checked=true;
		break;
	default:
		FormMain->PresetD1->Checked=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::GetPreset(void)
{
	int i = iLinePresetIndex;

	btnDots->Down=bLinePreset_styleDots[i];
	btnDashes->Down=bLinePreset_styleDashes[i];

	bTrkLengthManual=true;
	TrkDash->Position=iLinePreset_styleRepeat[i];
	bTrkLengthManual=false;

	btnTaperIn->Down				=bLinePreset_taperIn[i];
	btnTaperOut->Down				=bLinePreset_taperOut[i];
	btnTaperFromMid->Down			=bLinePreset_taperFromMid[i];
	CheckEnableBrush->Checked		=bLinePreset_taperEnabled[i];

	btnQuick->Down					=bLinePreset_modeQuick[i];
	btnSmear->Down					=bLinePreset_modeCoat[i];

	CheckResetLineNudge->Checked	=bLinePreset_adjustAutoreset[i];

	btnResetLine->Down				=bLinePreset_rosterReset[i];
	btnAutoSizeHyperY->Down			=bLinePreset_rosterAutosize[i];

	btnRosterLine->Down				=bLinePreset_rosterEnableLine[i];
	btnRosterCurve->Down			=bLinePreset_rosterEnableCurve[i];
	btnRosterKnee->Down				=bLinePreset_rosterEnableKnee[i];
	btnRosterAngle->Down			=bLinePreset_rosterEnableRight[i];
	btnRosterRectangle->Down		=bLinePreset_rosterEnableRect[i];

	btnRosterEllipse->Down			=bLinePreset_rosterEnableEllipse[i];
	btnRosterHyperbola->Down		=bLinePreset_rosterEnableHyperbola[i];
	btnRosterHyperline->Down		=bLinePreset_rosterEnableHyperline[i];
	btnRosterHypercave->Down		=bLinePreset_rosterEnableHypercave[i];

	lineToolRoster					=iLinePreset_rosterIndex[i];

	if((btnTaperIn->Down && btnTaperOut->Down)) btnTaperFromMid->Flat=true;
	else btnTaperFromMid->Flat=false;

}
void __fastcall TFormLineDetails::btnDotsClick(TObject *Sender)
{
	int i = iLinePresetIndex;
	bLinePreset_styleDots[i]=btnDots->Down;
	LinePresetSaveTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnDashesClick(TObject *Sender)
{
	int i = iLinePresetIndex;
	bLinePreset_styleDashes[i]=btnDashes->Down;
	LinePresetSaveTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnTaperFromMidClick(TObject *Sender)
{
   int i = iLinePresetIndex;
   bLinePreset_taperFromMid[i]=btnTaperFromMid->Down;
   LinePresetSaveTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnQuickClick(TObject *Sender)
{
   int i = iLinePresetIndex;
   bLinePreset_modeQuick[i]=btnQuick->Down;
   LinePresetSaveTimer->Enabled=true;
   FormMain->LineQuickmultiline1->Checked=btnQuick->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::CheckResetLineNudgeClick(TObject *Sender)
{
	int i = iLinePresetIndex;
	bLinePreset_adjustAutoreset[i]=CheckResetLineNudge->Checked;
	LinePresetSaveTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnResetLineClick(TObject *Sender)
{
   int i = iLinePresetIndex;
   bLinePreset_rosterReset[i]=btnResetLine->Down;
   LinePresetSaveTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnAutoSizeHyperYClick(TObject *Sender)
{
   int i = iLinePresetIndex;
   bLinePreset_rosterAutosize[i]=btnAutoSizeHyperY->Down;
   LinePresetSaveTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::LinePresetSaveTimerTimer(TObject *Sender)
{
    FormMain->SaveLineToolConfig();
	LinePresetSaveTimer->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnQuickMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="When on, automatically starts plotting the next line from the position you placed the previous.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnSmearClick(TObject *Sender)
{
   int i = iLinePresetIndex;
   bLinePreset_modeCoat[i]=btnSmear->Down;
   LinePresetSaveTimer->Enabled=true;
   FormMain->LineCoating1->Checked=btnSmear->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnPresetAMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Preset A [Ctrl+Alt+1]";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnPresetBMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Preset B [Ctrl+Alt+2]";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnPresetCMouseEnter(TObject *Sender)
{
	 FormMain->LabelStats->Caption="Preset C [Ctrl+Alt+3]";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnPresetDMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Preset D [Ctrl+Alt+4]";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnAutoSizeHyperYMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="When down, Hyperbolas, Hyperlines and Hyperbolic concaves\nautomatically resize the perpendicular dimension;\nmatching the distance between the 2 drawn points.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnResetLineMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="When down, each new line starts as a basic line between two points.\nIf up, the line tool remembers which line algorithm was last used.\nScroll to change algorithm.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::GroupBox3MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Scrollwheel roster. Click the various buttons to include or exclude the algorithms from the scrollwheel roster.\nThe different line algorithms are accessed while drawing by scrolling the mouse wheel.\nMost of them have inverse variations if scrolling down instead of up.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterLineMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Line: Draws a straight line between 2 points. While drawing;\n[Ctrl+Scroll]: Adjust the origin (y0) coordinate.\t[Ctrl+Alt+Scroll]: Adjust the origin (x0) coordinate.\n[Alt+Scroll]: Buckle the midpoint of the line.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterCurveMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Curve: Draws a 90 degree arc. While draing;\n[Ctrl+Scroll] to adjust curvature y-wise,\t[Ctrl+Alt+Scroll] to adjust curvature x-wise,\n[Alt+Scroll] to adjust both curvature coordinates." ;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterKneeMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Knee angle: Draws a curve-approximating knee-bent line. While drawing;\n[Ctrl+Scroll] to Adjust the mid (y1) coordinate.\t[Ctrl+Alt+Scroll] to adjust the mid (x1) coordinate.\n[Alt+Scroll: Buckle the midpoint.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterAngleMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Right angle: Draws a right angle between the points. While drawing;\n[Alt+Scroll] Adjusts line completion towards the juncture.\n[Ctrl+Scroll] adjusts the Y coordinate of the juncture.\t[Ctrl+Alt+Scroll]the X coordinate of the juncture.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterRectangleMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Rectangle: Draws a rectangle between the points. While drawing;\n[Alt]Forces likesidedness. [Alt+Scroll] Adjusts line completion towards the corners.\n[Ctrl+Scroll] folds/expands the Y coordinate of the corners.\t[Ctrl+Alt+Scroll] the X coordinate of the corners.\nTip: By folding, drawing hexagons, octagons, hourglass shapes and more are possible.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterEllipseMouseEnter(
	  TObject *Sender)
{
	FormMain->LabelStats->Caption="Ellipse: Draws an ellipse between the points. While drawing;\n[Alt]Forces likesidedness. [Alt+Scroll] Adjusts line completion.\n[Ctrl+Scroll] folds/expands the Y coordinate of the 'corners'.\t[Ctrl+Alt+Scroll] the X coordinate of the corners.\nTip: By folding/expanding and moving the plot around, various ornamentations and bubbleshapes are possible.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterHyperbolaMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Hyperbolic curve:\tThrows an 180 degree curve between the points.\nMay form an Arc or an S depending on relative positions. While drawing;\n[Ctrl+Scroll]: Adjust the midpoint (y1) coordinate.\t[Ctrl+Alt+Scroll]: Adjust the midpoint (x1) coordinate.\nMidpoint y1 is stored between lines, but x1 is reset." ;
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterHyperlineMouseEnter(
	  TObject *Sender)
{
	FormMain->LabelStats->Caption="Hyperbolic line: Similar behaviour to hyperbolic curve, but plots 2 straight lines.";
}
//---------------------------------------------------------------------------

void __fastcall TFormLineDetails::btnRosterHypercaveMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Hyperbolic Concave: Similar to hyperbolic curve, but plots 2 concave curves instead.";
}
//---------------------------------------------------------------------------

