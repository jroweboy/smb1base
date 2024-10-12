//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitAttributeCheckerOptions.h"
#include "UnitMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAttrChecker *FormAttrChecker;
bool trackBarAuto=false;

extern bool cueUpdateNametable;

extern int faderAttrCheckerOpacity[];
extern int faderAttrCheckerSeparation[];
extern int upDownAttrCheckerRotation[];

extern int iAttrCheckerMethod;
extern int iRotateCheckerSeparation;

//---------------------------------------------------------------------------
__fastcall TFormAttrChecker::TFormAttrChecker(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAttrChecker::SpeedButton1Click(TObject *Sender)
{

	int tag = (int)((TSpeedButton*)Sender)->Tag;
	/*
	if(tag==2) FormAttrChecker->Height=FormOriginalHeight;
	else if(tag==3) FormAttrChecker->Height=FormOriginalHeight;
	else if(tag==4) FormAttrChecker->Height=FormOriginalHeight;
	else FormAttrChecker->Height=93;
    */
	iAttrCheckerMethod=(int)tag;
	trackBarAuto=true;
	TrackBar1->Position	=faderAttrCheckerOpacity[tag];
	TrackBar2->Position	=faderAttrCheckerSeparation[tag];

	trackBarAuto=false;

	iRotateCheckerSeparation=upDownAttrCheckerRotation[iAttrCheckerMethod];
	Label5->Caption=IntToStr(iRotateCheckerSeparation);

	cueUpdateNametable=true;

}
//---------------------------------------------------------------------------
void __fastcall TFormAttrChecker::FormCreate(TObject *Sender)
{
	ResetPresets(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormAttrChecker::TrackBar1Change(TObject *Sender)
{
	if(trackBarAuto) return; //prevent false trigs.
	faderAttrCheckerOpacity[iAttrCheckerMethod]=TrackBar1->Position;
	TrackBar1->TabStop = false;
    cueUpdateNametable=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormAttrChecker::UpDown1Changing(TObject *Sender,
	  bool &AllowChange)
{
	if(trackBarAuto)  {AllowChange=false; return;}

	//FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormAttrChecker::TrackBar2Change(TObject *Sender)
{
	//if(trackBarAuto) return; //prevent false trigs.
	faderAttrCheckerSeparation[iAttrCheckerMethod]=TrackBar2->Position;
	TrackBar2->TabStop = false;
    cueUpdateNametable=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormAttrChecker::SpeedButton6Click(TObject *Sender)
{
	iRotateCheckerSeparation++;
	if(iRotateCheckerSeparation>2) iRotateCheckerSeparation=0;
	else if(iRotateCheckerSeparation<0) iRotateCheckerSeparation=2;

	Label5->Caption=IntToStr(iRotateCheckerSeparation);

	upDownAttrCheckerRotation[iAttrCheckerMethod]=iRotateCheckerSeparation;
	 cueUpdateNametable=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormAttrChecker::SpeedButton7Click(TObject *Sender)
{
	iRotateCheckerSeparation--;
	if(iRotateCheckerSeparation>2) iRotateCheckerSeparation=0;
	else if(iRotateCheckerSeparation<0) iRotateCheckerSeparation=2;

	Label5->Caption=IntToStr(iRotateCheckerSeparation);

	upDownAttrCheckerRotation[iAttrCheckerMethod]=iRotateCheckerSeparation;
	cueUpdateNametable=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormAttrChecker::ResetPresets(bool doRefresh)
{
	faderAttrCheckerOpacity[0]=80;
	faderAttrCheckerSeparation[0]=30;
	upDownAttrCheckerRotation[0]=0;

	faderAttrCheckerOpacity[1]=80;
	faderAttrCheckerSeparation[1]=30;
	upDownAttrCheckerRotation[1]=0;

	faderAttrCheckerOpacity[2]=55;
	faderAttrCheckerSeparation[2]=25;
	upDownAttrCheckerRotation[2]=0;

	faderAttrCheckerOpacity[3]=30;
	faderAttrCheckerSeparation[3]=30;
	upDownAttrCheckerRotation[3]=0;

	faderAttrCheckerOpacity[4]=55;
	faderAttrCheckerSeparation[4]=25;
	upDownAttrCheckerRotation[4]=0;

    UpdateUI();
	cueUpdateNametable=doRefresh;

}

void __fastcall TFormAttrChecker::UpdateUI(void)
{
	int tmp=iAttrCheckerMethod;
	trackBarAuto=true;
	TrackBar1->Position	=faderAttrCheckerOpacity[tmp];
	TrackBar2->Position	=faderAttrCheckerSeparation[tmp];

	trackBarAuto=false;

	iRotateCheckerSeparation=upDownAttrCheckerRotation[tmp];
	Label5->Caption=IntToStr(iRotateCheckerSeparation);

}
void __fastcall TFormAttrChecker::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssRight)||Shift.Contains(ssCtrl))
	{
    	TPoint p = Mouse->CursorPos;
		int x= p.x;
		int y= p.y;
		PopupMenu1->Popup(x,y);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAttrChecker::Resetpresets1Click(TObject *Sender)
{
	ResetPresets(true);	
}
//---------------------------------------------------------------------------

