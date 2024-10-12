//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitBucketToolbox.h"
#include "UnitMain.h"
#include "UnitLineDetails.h"
#include "UnitCHREditor.h"
#include "UnitBrush.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBucketToolbox *FormBucketToolbox;

extern bool prefStartShowBucket;

extern bool holdStats;
extern unsigned char iBucketDirectionA;
extern unsigned char iBucketDirectionB;
//---------------------------------------------------------------------------
__fastcall TFormBucketToolbox::TFormBucketToolbox(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::SpeedButton15MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
       if(SpeedButton15->Down==false && SpeedButton16->Down==false)
	SpeedButton15->Down=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnC_nwMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
		if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			btnC_nw->Down=true;    //bc click applies after, this one is inverted
			btnC_n->Down=true;
			btnC_ne->Down=true;
			btnC_e->Down=true;
			btnC_se->Down=true;
			btnC_s->Down=true;
			btnC_sw->Down=true;
			btnC_w->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(btnC_nw->Down && (btnC_n->Down
							||btnC_ne->Down||btnC_e->Down
							||btnC_se->Down||btnC_s->Down
							||btnC_sw->Down||btnC_w->Down))
		{
			btnC_nw->Down=false; //force always down, except if others are already up

		}
		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;


	}
	if(Shift.Contains(ssShift))
	{

		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;
		if(!btnC_nw->Down)
		{
			btnC_nw->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnC_nMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
			if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			btnC_nw->Down=true;    //bc click applies after, this one is inverted
			btnC_n->Down=true;
			btnC_ne->Down=true;
			btnC_e->Down=true;
			btnC_se->Down=true;
			btnC_s->Down=true;
			btnC_sw->Down=true;
			btnC_w->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(btnC_n->Down && (  btnC_nw->Down
							||btnC_ne->Down||btnC_e->Down
							||btnC_se->Down||btnC_s->Down
							||btnC_sw->Down||btnC_w->Down))
		{
			btnC_n->Down=false; //force always down, except if others are already up

		}
		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;


	}
	if(Shift.Contains(ssShift))
	{

		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;
		if(!btnC_n->Down)
		{
			btnC_n->Down=true; //force always up
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnC_neMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
			if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			btnC_nw->Down=true;    //bc click applies after, this one is inverted
			btnC_n->Down=true;
			btnC_ne->Down=true;
			btnC_e->Down=true;
			btnC_se->Down=true;
			btnC_s->Down=true;
			btnC_sw->Down=true;
			btnC_w->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(btnC_ne->Down && (btnC_n->Down
							||btnC_nw->Down||btnC_e->Down
							||btnC_se->Down||btnC_s->Down
							||btnC_sw->Down||btnC_w->Down))
		{
			btnC_ne->Down=false; //force always down, except if others are already up

		}
		btnC_nw->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;


	}
	if(Shift.Contains(ssShift))
	{

		btnC_nw->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;
		if(!btnC_ne->Down)
		{
			btnC_ne->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnC_wMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
			if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			btnC_nw->Down=true;    //bc click applies after, this one is inverted
			btnC_n->Down=true;
			btnC_ne->Down=true;
			btnC_e->Down=true;
			btnC_se->Down=true;
			btnC_s->Down=true;
			btnC_sw->Down=true;
			btnC_w->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(btnC_w->Down && (btnC_nw->Down
							||btnC_ne->Down||btnC_e->Down
							||btnC_se->Down||btnC_s->Down
							||btnC_sw->Down||btnC_n->Down))
		{
			btnC_w->Down=false; //force always down, except if others are already up

		}
		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;

	}
	if(Shift.Contains(ssShift))
	{

		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;
		if(!btnC_w->Down)
		{
			btnC_w->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnC_eMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
       			if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			btnC_nw->Down=true;    //bc click applies after, this one is inverted
			btnC_n->Down=true;
			btnC_ne->Down=true;
			btnC_e->Down=true;
			btnC_se->Down=true;
			btnC_s->Down=true;
			btnC_sw->Down=true;
			btnC_w->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(btnC_e->Down && (btnC_nw->Down
							||btnC_ne->Down||btnC_e->Down
							||btnC_se->Down||btnC_s->Down
							||btnC_sw->Down||btnC_w->Down))
		{
			btnC_e->Down=false; //force always down, except if others are already up

		}
		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_w->Down=false;
		btnC_s->Down=false;

	}
	if(Shift.Contains(ssShift))
	{

		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;
		if(!btnC_e->Down)
		{
			btnC_e->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnC_swMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
				if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			btnC_nw->Down=true;    //bc click applies after, this one is inverted
			btnC_n->Down=true;
			btnC_ne->Down=true;
			btnC_e->Down=true;
			btnC_se->Down=true;
			btnC_s->Down=true;
			btnC_sw->Down=true;
			btnC_w->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(btnC_sw->Down && (btnC_nw->Down
							||btnC_ne->Down||btnC_e->Down
							||btnC_se->Down||btnC_s->Down
							||btnC_n->Down ||btnC_w->Down))
		{
			btnC_sw->Down=false; //force always down, except if others are already up

		}
		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_w->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;

	}
	if(Shift.Contains(ssShift))
	{

		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;
		if(!btnC_sw->Down)
		{
			btnC_sw->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnC_sMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			btnC_nw->Down=true;    //bc click applies after, this one is inverted
			btnC_n->Down=true;
			btnC_ne->Down=true;
			btnC_e->Down=true;
			btnC_se->Down=true;
			btnC_s->Down=true;
			btnC_sw->Down=true;
			btnC_w->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(btnC_s->Down && (btnC_nw->Down
							||btnC_ne->Down||btnC_e->Down
							||btnC_se->Down||btnC_n->Down
							||btnC_sw->Down||btnC_w->Down))
		{
			btnC_s->Down=false; //force always down, except if others are already up

		}
		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_w->Down=false;

	}
	if(Shift.Contains(ssShift))
	{

		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;
		if(!btnC_s->Down)
		{
			btnC_s->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnC_seMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			btnC_nw->Down=true;    //bc click applies after, this one is inverted
			btnC_n->Down=true;
			btnC_ne->Down=true;
			btnC_e->Down=true;
			btnC_se->Down=true;
			btnC_s->Down=true;
			btnC_sw->Down=true;
			btnC_w->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(btnC_se->Down && (btnC_nw->Down
							||btnC_ne->Down||btnC_e->Down
							||btnC_n->Down||btnC_s->Down
							||btnC_sw->Down||btnC_w->Down))
		{
			btnC_se->Down=false; //force always down, except if others are already up

		}
		btnC_ne->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
        btnC_w->Down=false;

	}
	if(Shift.Contains(ssShift))
	{

		btnC_ne->Down=false;
		btnC_se->Down=false;
		btnC_sw->Down=false;
		btnC_nw->Down=false;

		btnC_n->Down=false;
		btnC_e->Down=false;
		btnC_s->Down=false;
		btnC_w->Down=false;
		if(!btnC_se->Down)
		{
			btnC_se->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::SpeedButton5MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	btnC_ne->Down= btnC_ne->Down? false:true;
	btnC_se->Down= btnC_se->Down? false:true;
	btnC_sw->Down= btnC_sw->Down? false:true;
	btnC_nw->Down= btnC_nw->Down? false:true;

	btnC_n->Down= btnC_n->Down? false:true;
	btnC_e->Down= btnC_e->Down? false:true;
	btnC_s->Down= btnC_s->Down? false:true;
	btnC_w->Down= btnC_w->Down? false:true;
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::SpeedButton27MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bool tmp=btnC_nw->Down;
	btnC_nw->Down=btnC_n->Down;
	btnC_n->Down=btnC_ne->Down;
	btnC_ne->Down=btnC_e->Down;
	btnC_e->Down=btnC_se->Down;
	btnC_se->Down=btnC_s->Down;
	btnC_s->Down=btnC_sw->Down;
	btnC_sw->Down=btnC_w->Down;
	btnC_w->Down=tmp;
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::SpeedButton26MouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bool tmp=btnC_w->Down;
	btnC_w->Down=btnC_sw->Down;
	btnC_sw->Down=btnC_s->Down;
	btnC_s->Down=btnC_se->Down;
	btnC_se->Down=btnC_e->Down;
	btnC_e->Down=btnC_ne->Down;
	btnC_ne->Down=btnC_n->Down;
	btnC_n->Down=btnC_nw->Down;
	btnC_nw->Down=tmp;

}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::SpeedButton25MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bool tmp1=btnC_nw->Down;
	bool tmp2=btnC_n->Down;
	bool tmp3=btnC_ne->Down;

	btnC_nw->Down= btnC_sw->Down;
	btnC_n->Down= btnC_s->Down;
	btnC_ne->Down= btnC_se->Down;

	btnC_sw->Down= tmp1;
	btnC_s->Down= tmp2;
	btnC_se->Down= tmp3;

}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::SpeedButton24MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    bool tmp1=btnC_nw->Down;
	bool tmp2=btnC_w->Down;
	bool tmp3=btnC_sw->Down;

	btnC_nw->Down= btnC_ne->Down;
	btnC_w->Down= btnC_e->Down;
	btnC_sw->Down= btnC_se->Down;

	btnC_ne->Down= tmp1;
	btnC_e->Down= tmp2;
	btnC_se->Down= tmp3;
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnClassicMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	btnC_ne->Flat=false;
	btnC_se->Flat=false;
	btnC_sw->Flat=false;
	btnC_nw->Flat=false;
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::btnForgivingMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	btnC_ne->Flat=true;
	btnC_se->Flat=true;
	btnC_sw->Flat=true;
	btnC_nw->Flat=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::FormCreate(TObject *Sender)
{
	if(FormBucketToolbox->Position==poDesigned)
	{
			FormBucketToolbox->Left=(Screen->Width-FormMain->Width)/2.3+FormMain->Width+FormBrush->Width;
			FormBucketToolbox->Top=(Screen->Height-FormMain->Height)/4+FormCHREditor->Height;

	}
	GetBucketDirA();
	if(prefStartShowBucket==true) FormBucketToolbox->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::GetBucketDirA(void)
{
	btnC_nw->Down = (iBucketDirectionA & (1 << 0)) != 0;
	btnC_n->Down  = (iBucketDirectionA & (1 << 1)) != 0;
	btnC_ne->Down = (iBucketDirectionA & (1 << 2)) != 0;
	btnC_w->Down  = (iBucketDirectionA & (1 << 3)) != 0;
	btnC_e->Down  = (iBucketDirectionA & (1 << 4)) != 0;
	btnC_sw->Down = (iBucketDirectionA & (1 << 5)) != 0;
	btnC_s->Down  = (iBucketDirectionA & (1 << 6)) != 0;
	btnC_se->Down = (iBucketDirectionA & (1 << 7)) != 0;
}

//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::GetBucketDirB(void)
{
	btnC_nw->Down = (iBucketDirectionB & (1 << 0)) != 0;
	btnC_n->Down  = (iBucketDirectionB & (1 << 1)) != 0;
	btnC_ne->Down = (iBucketDirectionB & (1 << 2)) != 0;
	btnC_w->Down  = (iBucketDirectionB & (1 << 3)) != 0;
	btnC_e->Down  = (iBucketDirectionB & (1 << 4)) != 0;
	btnC_sw->Down = (iBucketDirectionB & (1 << 5)) != 0;
	btnC_s->Down  = (iBucketDirectionB & (1 << 6)) != 0;
	btnC_se->Down = (iBucketDirectionB & (1 << 7)) != 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::PutBucketDirA(void)
{
	iBucketDirectionA=0;

	iBucketDirectionA |= (btnC_nw->Down ? (1 << 0) : 0);
	iBucketDirectionA |= (btnC_n->Down ? (1 << 1) : 0);
	iBucketDirectionA |= (btnC_ne->Down ? (1 << 2) : 0);
	iBucketDirectionA |= (btnC_w->Down ? (1 << 3) : 0);
	iBucketDirectionA |= (btnC_e->Down ? (1 << 4) : 0);
	iBucketDirectionA |= (btnC_sw->Down ? (1 << 5) : 0);
	iBucketDirectionA |= (btnC_s->Down ? (1 << 6) : 0);
	iBucketDirectionA |= (btnC_se->Down ? (1 << 7) : 0);
}

//---------------------------------------------------------------------------
void __fastcall TFormBucketToolbox::PutBucketDirB(void)
{
	iBucketDirectionB=0;

	iBucketDirectionB |= (btnC_nw->Down ? (1 << 0) : 0);
	iBucketDirectionB |= (btnC_n->Down ? (1 << 1) : 0);
	iBucketDirectionB |= (btnC_ne->Down ? (1 << 2) : 0);
	iBucketDirectionB |= (btnC_w->Down ? (1 << 3) : 0);
	iBucketDirectionB |= (btnC_e->Down ? (1 << 4) : 0);
	iBucketDirectionB |= (btnC_sw->Down ? (1 << 5) : 0);
	iBucketDirectionB |= (btnC_s->Down ? (1 << 6) : 0);
	iBucketDirectionB |= (btnC_se->Down ? (1 << 7) : 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::PutBucketDirections(void)
{
	if(btnSwitchDir->Caption=="A")PutBucketDirA();
	else PutBucketDirB();
}

//---------------------------------------------------------------------------


void __fastcall TFormBucketToolbox::btnSwitchDirMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(btnSwitchDir->Caption=="A"){
	GetBucketDirB();
	btnSwitchDir->Caption="B";
   }
   else{
	GetBucketDirA();
	btnSwitchDir->Caption="A";

   }
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::RotateCW(void)
{
	bool tmp=btnC_w->Down;
	btnC_w->Down=btnC_sw->Down;
	btnC_sw->Down=btnC_s->Down;
	btnC_s->Down=btnC_se->Down;
	btnC_se->Down=btnC_e->Down;
	btnC_e->Down=btnC_ne->Down;
	btnC_ne->Down=btnC_n->Down;
	btnC_n->Down=btnC_nw->Down;
	btnC_nw->Down=tmp;
}

void __fastcall TFormBucketToolbox::RotateCCW(void)
{
	bool tmp=btnC_nw->Down;
	btnC_nw->Down=btnC_n->Down;
	btnC_n->Down=btnC_ne->Down;
	btnC_ne->Down=btnC_e->Down;
	btnC_e->Down=btnC_se->Down;
	btnC_se->Down=btnC_s->Down;
	btnC_s->Down=btnC_sw->Down;
	btnC_sw->Down=btnC_w->Down;
	btnC_w->Down=tmp;
}
void __fastcall TFormBucketToolbox::btnClassicMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Classic bucket mode:\nbinds classic bucket to the [ctrl] key when painting.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnForgivingMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Gap Aware bucket mode:\nto the [ctrl] key when painting.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btn4wayMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="4-way flood fill:\nThe default, most common mode.\nSuits most cases fine.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btn8wayMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="8-way flood fill:\nSlips through corners between 2 diagonal pixels; like a bishop in chess.\nCommonly used to fill checkerboard dithers, or together with jaggy line art styles.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnCustomwayMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Custom flood fill: Lets you tweak which directions the flood fill should collision-test;\nallowing for many useful shapes to be filled in; lines, halves, quadrants and more.\nUse the custom flood editor to tweak to taste.\nTip:\t you can rotate directions with the scroll wheel while filling in this mode. Have fun!";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnFieldPenMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Field colour: Pen\nDefault, typical behaviour for a normal bucket.\nFills the field with the current pen colour.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::SpeedButton10MouseEnter(
	  TObject *Sender)
{
	FormMain->LabelStats->Caption="Field colour: Border\nChooses field colour based on what colour surrounds the field the most.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnSidesMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Gap detection: Laterals\nWhen enabled, the flood checks the sides of the currently evaluated pixel to dermine if the pixel is in a gap.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnSemisMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Gap detection: Semi-diagonals\nWhen enabled, the flood checks the semi-diagonals of the evaluated pixel to dermine if the pixel is in a gap.\nA semi-diagonal is when there's one lateral edge on one side, and a diagonal edge on the other.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnDiagsMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Gap detection: Diagonals\nWhen enabled, the flood checks the diagonals of the currently evaluated pixel to dermine if the pixel is in a gap.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnFieldsMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Fill fields:\nWhen down, the clicked on Field itself is filled while Gaps are left unfilled.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnGapsMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Fill fields:\nWhen down, the Gaps surrounding the field are filled, while the field itself is left unfilled.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnBothMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Fill both:\nWhen down, both the clicked-on Field and surrounding Gaps are filled.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnSmartAllMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Gap Aware 4-way flood fill:\nDefault. Suits most cases fine.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnSmartCustomMouseEnter(
	  TObject *Sender)
{
   FormMain->LabelStats->Caption="Gap Aware Custom flood fill:\tWorks almost identically to the Classic Custom flood fill;\nexcept the Gap Aware algorithm doesn't utilize diagonal flood directions.\nInstead, when a diagonal button is checked in the custom flood editor, its neighboring 2 buttons are assumed active,\nresulting in a filled quadrant.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnGapPenMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Border colour: Pen\nWhen down, gaps and fields are filled-in with the same colour (pen colour).";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::SpeedButton12MouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Field colour: Border\nWhen down, gaps are filled in with the detected majority colour of the borders surrounding the clicked on field.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnForceBufMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Buffer:\nUp: Dragging the cursor while filling smears the fill over all hovered/affected areas.\nDown:Dragging only affects the currently hovered over area.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::SpeedButton5MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Invert:\nClick to invert the state of all directional flood buttons.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::SpeedButton27MouseEnter(
	  TObject *Sender)
{
   FormMain->LabelStats->Caption="Rotate CCW:\nRotates the state of the directional flood buttons 45 degrees counter-clockwise.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::SpeedButton26MouseEnter(
	  TObject *Sender)
{
   FormMain->LabelStats->Caption="Rotate CW:\nRotates the state of the directional flood buttons 45 degrees clockwise.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::SpeedButton24MouseEnter(
	  TObject *Sender)
{
  FormMain->LabelStats->Caption="Flip Horz:\nFlips the state of the directional flood buttons horizontally.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::SpeedButton25MouseEnter(
	  TObject *Sender)
{
	FormMain->LabelStats->Caption="Flip Vert:\nFlips the state of the directional flood buttons vertically.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnSwitchDirMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Switch set A/B:\nToggles between using main (A) and spare (B) sets of directional flood buttons.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnC_nwMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Toggle NW:\nToggles the state of the northwest directional flood on/off.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnC_nMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Toggle N:\nToggles the state of the north directional flood on/off.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnC_neMouseEnter(TObject *Sender)
{
FormMain->LabelStats->Caption="Toggle NE:\nToggles the state of the northeast directional flood on/off.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnC_wMouseEnter(TObject *Sender)
{
FormMain->LabelStats->Caption="Toggle W:\nToggles the state of the west directional flood on/off.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnC_eMouseEnter(TObject *Sender)
{
FormMain->LabelStats->Caption="Toggle E:\nToggles the state of the E directional flood on/off.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnC_swMouseEnter(TObject *Sender)
{
FormMain->LabelStats->Caption="Toggle SW:\nToggles the state of the southwest directional flood on/off.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnC_sMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Toggle S:\nToggles the state of the south directional flood on/off.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnC_seMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Toggle SE:\nToggles the state of the southeast directional flood on/off.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::ToggleFillMode(void)
{
	if(btnClassic->Down==true) {
		btnForgiving->Down=true;
		FormMain->LabelStats->Caption="Bucket mode: Gap Aware.\n\n[Ctrl-click] to use.\t[Shift-F6] opens bucket toolbox.";
	}
	else{
		btnClassic->Down=true;
		FormMain->LabelStats->Caption="Bucket mode: Classic.\n\n[Ctrl-click] to use.\t[Shift-F6] opens bucket toolbox.";

	}
    FormMain->StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::ToggleFloodMode(void)
{
	if(btnClassic->Down==true)
	{
		if(btn4way->Down==true){
			btnCustomway->Down=true;
			FormMain->LabelStats->Caption="Bucket flood direction: Custom.\n\n[Ctrl-click] to use.\t[Shift-F6] opens bucket toolbox.";
		}
		else if(btnCustomway->Down==true){
			btn8way->Down=true;
			FormMain->LabelStats->Caption="Bucket flood direction: 8-way.\n\n[Ctrl-click] to use.\t[Shift-F6] opens bucket toolbox.";

		}
		else{
			btn4way->Down=true;
			FormMain->LabelStats->Caption="Bucket flood direction: 4-way.\n\n[Ctrl-click] to use.\t[Shift-F6] opens bucket toolbox.";
		}
	}
	else{
		if(btnSmartAll->Down==true){
			btnSmartCustom->Down=true;
			FormMain->LabelStats->Caption="Bucket flood direction: Custom.\n\n[Ctrl-click] to use.\t[Shift-F6] opens bucket toolbox.";
		}
		else{
			btnSmartAll->Down=true;
			FormMain->LabelStats->Caption="Bucket flood direction: 4-way.\n\n[Ctrl-click] to use.\t[Shift-F6] opens bucket toolbox.";
		}
	}
    FormMain->StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBucketToolbox::btnSidesClick(TObject *Sender)
{
	if(btnSides->Down==false && btnSemis->Down==false && btnDiags->Down==false)
	btnSides->Down=true;
}
//---------------------------------------------------------------------------

