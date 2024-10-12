//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "UnitBrush.h"
#include "UnitMain.h"
#include "UnitCHREditor.h"
#include "BrushMaskData.h"
#include "UnitLineDetails.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBrush *FormBrush;

Graphics::TBitmap *brushMaskBuffer;
extern bool bBrushMask[25];
///extern int bBrushMaskBuffer[25];
extern bool bSnapToScreen;
extern bool prefStartShowBrushes;

extern int iBrushPresetIndex;
extern int iBrushCursorAlignment[];
extern float brush_x_anchor;
extern float brush_y_anchor;

extern int (*ptr_tableBrush[14])[16][16];
extern int iBrushSize[];

extern int iBrushSnapSize_x[];
extern int iBrushSnapSize_y[];

extern bool	bBufCtrl;
extern bool	bBufShift;
extern bool	bBufAlt;
bool bMaskPenColour;
bool bDontUpdatePaintModulo=false;
bool bForbidMaskPen=false;

int x_brushStatus=0;
int y_brushStatus=0;

extern bool bBufVK_3;
extern bool lineDrawing;
extern bool bSmudge;
 char maskRotateWorkspace[32][32];
 char maskRotateBuf[32][32];
 char CompareBufBitmask[32][32];
//used by free rotator
TPoint cursorBeginRotate;
TPoint cursorRotate;
TPoint maskCenterPoint;
double initialAngle_rad;

extern void transpose_bitmask(char* data, int d);
extern void rotate180_bitmask(char* data, int w, int h);
extern void shear_bitmask(char* data, float shear, int w, int h, bool bRot180, bool bTranspose,bool bRounding);

AnsiString RemoveExt(AnsiString name)
{
	return ChangeFileExt(name,"");
}



AnsiString GetExt(AnsiString name)
{
	name=ExtractFileName(name);

	return name.SubString(name.LastDelimiter(".")+1,name.Length()-name.LastDelimiter(".")).LowerCase();
}


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
	brushMaskBuffer = new Graphics::TBitmap();

	brushMaskBuffer->Width = PaintBoxMask->Width;
	brushMaskBuffer->Height = PaintBoxMask->Height;
	brushMaskBuffer->PixelFormat=pf24bit;

	if(FormBrush->Position==poDesigned)
			{
			FormBrush->Left=(Screen->Width-FormMain->Width)/2.3 + FormMain->Width;
			FormBrush->Top=(Screen->Height-FormMain->Height)/4+FormCHREditor->Height;

	}

	ToggleSpeedButtonByTag(GroupBox1, iBrushPresetIndex, 	 1, 1);
	ToggleSpeedButtonByTag(GroupBox2, iBrushCursorAlignment[iBrushPresetIndex], 1, 1);

	 SetBrushAnchor();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);

	if(prefStartShowBrushes==true) FormBrush->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormBrush::DrawCell(int iX,int iY,int maskID)
{
	int x,y;
	int d = PaintBoxMask->Width;
	int size=d/iBrushSize[iBrushPresetIndex];
	int cell=size-1;


	x=iX*size;
	y=iY*size;
		TRect r;
	//if(bBrushMask[maskID]==true) PaintBoxMask->Canvas->Brush->Color=TColor(0x00FFFFFF);


	int val = (*ptr_tableBrush[iBrushPresetIndex])[iX][iY];
	if(val==true) brushMaskBuffer->Canvas->Brush->Color=TColor(0x00FFFFFF);

	else        brushMaskBuffer->Canvas->Brush->Color=TColor(0x00000000);
	r.left=x;
	r.top=y;
	r.right=x+cell;
	r.Bottom=y+cell;

	brushMaskBuffer->Canvas->FillRect(r);

	}

//---------------------------------------------------------------------------
void __fastcall TFormBrush::Draw()
{
unsigned int i,x,y;
	//if(!bDontUpdatePaintModulo)
	UpdatePaintBoxModulo();
	//draw background.
	TRect r;
	brushMaskBuffer->Canvas->Brush->Color=FormBrush->Color;
	r.left=0;
	r.top=0;
	r.right=PaintBoxMask->Width;
	r.Bottom=PaintBoxMask->Height;
	brushMaskBuffer->Canvas->FillRect(r);

	//deaw mask cells
	i=0;
	for (y = 0; y < (unsigned)iBrushSize[iBrushPresetIndex]; y++) {
			for (x = 0; x < (unsigned)iBrushSize[iBrushPresetIndex]; x++) {
				DrawCell(x,y,	i);
				i++;
			}
	}

	PaintBoxMask->Canvas->Draw(0, 0, brushMaskBuffer);
}

//---------------------------------------------------------------------------
void __fastcall TFormBrush::PaintBoxMaskPaint(TObject *Sender)
{
	if(!bDontUpdatePaintModulo) Draw();
}
//---------------------------------------------------------------------------


void __fastcall TFormBrush::PaintBoxMaskMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	FormMain->SetUndo();
	UpdatePaintBoxModulo();
   //memcpy	(bBrushMaskBuffer,*ptr_tableBrush[iBrushIndirector[iBrushPresetIndex]],25*sizeof(int));
	int x,y;
	int dd = iBrushSize[iBrushPresetIndex];
	int d = PaintBoxMask->Width;
	int size=d/dd;
	x=X/size;
	y=Y/size;
   bForbidMaskPen=false;
   if(Shift.Contains(ssRight))
   {
		TRect clientRect = PaintBoxMask->ClientRect;

		// Calculate the center point of the client area
		int centerX = clientRect.Width() / 2 + clientRect.Left;
		int centerY = clientRect.Height() / 2 + clientRect.Top;

		cursorBeginRotate=Mouse->CursorPos;
		cursorRotate=cursorBeginRotate;
		double x1= centerX;
		double y1= centerY;
		double x2= X;
		double y2= Y;

		initialAngle_rad = atan2(y2 - y1, x2 - x1);

		// Normalize angle to the range [0, 2*pi)
		initialAngle_rad = fmod(initialAngle_rad, 2.0 * M_PI);
		/*
		if (initialAngle_rad < 0) {
			// Ensure the angle is positive
			initialAngle_rad += 2.0 * M_PI;
		}
		*/


		// Create a TPoint with the center coordinates
		TPoint tmpPoint(centerX, centerY);

		// Convert the client coordinates to screen coordinates using a temporary variable
		tmpPoint = PaintBoxMask->ClientToScreen(tmpPoint);
		//FormBrush->ClientToScreen()
        maskCenterPoint= tmpPoint;

		MakeRotBuffer();
		TimerRotate1->Enabled=true;
   }
   else if(Shift.Contains(ssLeft))
   {
		if(Shift.Contains(ssCtrl))
		{
		   FillBrushMask(x,y);
		}
		else{
			(*ptr_tableBrush[iBrushPresetIndex])[x][y] ^= true;
			bMaskPenColour=(*ptr_tableBrush[iBrushPresetIndex])[x][y];
		}
   }

   //FormMain->LabelStats->Caption=BoolToString(bBrushMask[0]);
   Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::FormKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	FormCHREditor->FormKeyDown(Sender,Key,Shift);
	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;


	if(Key==VK_OEM_3 || Key==VK_OEM_5) bBufVK_3=true;
	if(Key==VK_PAUSE)
	{
		FormCHREditor->AlphaBlend^=true;
		if (!Shift.Contains(ssShift))
		{
			FormMain->AlphaBlend^=true;
			//FormManageMetasprites->AlphaBlend^=true;
		}
	}
	bool b;
	if(Key==VK_F1) {FormMain->PageControlEditor->ActivePageIndex=0; b=true;}
	if(Key==VK_F2) {FormMain->PageControlEditor->ActivePageIndex=1; b=true;}
	if(Key==VK_F3) {FormMain->PageControlEditor->ActivePageIndex=2; b=true;}
	if (b) {FormMain->PageControlEditorChange(Sender);}




	if(!Shift.Contains(ssCtrl))
	{
		if(Key==VK_ESCAPE) {
			if( FormBrush->Active) {
				FormBrush->Close();
				if(FormCHREditor->Visible) FormCHREditor->Show();
			}
			else if(FormLineDetails->Active){
            	FormLineDetails->Close();
				if(FormCHREditor->Visible) FormCHREditor->Show();
				}
			else Close();
		}
		if(Key==VK_DELETE) FormMain->CopyCHR(false,true);

		if(!lineDrawing || Key==VK_LEFT|| Key==VK_RIGHT || Key==VK_UP || Key==VK_DOWN){
            //todo: change these to mask equivalents.
			if(Key=='A'||Key==VK_LEFT)  ShiftLeft();
			if(Key=='D'||Key==VK_RIGHT) ShiftRight();
			if(Key=='W'||Key==VK_UP)    ShiftUp();
			if(Key=='S'||Key==VK_DOWN)  ShiftDown();



		}
		if(!lineDrawing)
		{
			if(Key=='5') {FormCHREditor->Protect0->Down^=true; FormMain->Protect0->Down = FormCHREditor->Protect0->Down;}
			if(Key=='6') {FormCHREditor->Protect1->Down^=true; FormMain->Protect1->Down = FormCHREditor->Protect1->Down;}
			if(Key=='7') {FormCHREditor->Protect2->Down^=true; FormMain->Protect2->Down = FormCHREditor->Protect2->Down;}
			if(Key=='8') {FormCHREditor->Protect3->Down^=true; FormMain->Protect3->Down = FormCHREditor->Protect3->Down;}
			//if(Key=='V') MirrorVertical();    //this is an alias for T
			if(Key=='R') FormCHREditor->SpeedButtonRotateCWClick(Sender);//{Flip90(false); Rotate4tiles(false);}
			if(Key=='G') {FormCHREditor->btnSmudge->Down=true; FormMain->btnSmudge->Down=true; bSmudge=true;}
			if(Key=='T') FormLineDetails->btnSmear->Down^=true;
			if(Key=='K') FormLineDetails->btnQuick->Down^=true;
			//int scancode_oe= MapVirtualKey(VK_OEM_1, MAPVK_VK_TO_VSC);

		}


		//if(Key=='H') MirrorHorizontal();
		
		if(Key=='L') FormCHREditor->SpeedButtonRotateCCWClick(Sender);//{Flip90(true); Rotate4tiles(true);}

		if(Key=='B') {FormCHREditor->btnThick->Down^=true; FormMain->btnThick->Down = FormCHREditor->btnThick->Down;}
		if(Key=='N') {FormCHREditor->btnLine->Down^=true;   FormMain->btnLine->Down = FormCHREditor->btnLine->Down;}
		if(Key=='U') {FormCHREditor->btnQuant->Down^=true;  FormMain->btnQuant->Down = FormCHREditor->btnQuant->Down;}
		if(Key=='M') FormBrush->Close();

		if(Key=='I') FormCHREditor->ButtonBitmaskHi->Down^=true;
		if(Key=='O') FormCHREditor->ButtonBitmaskLo->Down^=true;
		
		if(Key=='P') FormCHREditor->SpeedButtonDoWrap->Down^=true;
		if(Key=='Q') FormMain->Show();
		if(Key=='X') FormMain->Toggletileset1Click(FormMain->Toggletileset1);

		if(Key=='E') {FormCHREditor->btn2x2mode->Down^=true;}

		if(Key==VK_OEM_4||Key==VK_OEM_COMMA) FormMain->SpeedButtonPrevMetaSpriteClick(Sender);// [
		if(Key==VK_OEM_6||Key==VK_OEM_PERIOD) FormMain->SpeedButtonNextMetaSpriteClick(Sender);// ]


		if(Key==(int)MapVirtualKey(0x27, 1)) FormBrush->ChangePreset(-1);
		if(Key==(int)MapVirtualKey(0x28, 1)) FormBrush->ChangePreset(+1);
		if(Key==(int)MapVirtualKey(0x2B, 1)) FormBrush->ChangePreset(+7);

		if(Key==VK_NUMPAD7) FormCHREditor->TileChange(-1,-1);
		if(Key==VK_NUMPAD8) FormCHREditor->TileChange( 0,-1);
		if(Key==VK_NUMPAD9) FormCHREditor->TileChange(+1,-1);

		if(Key==VK_NUMPAD4) FormCHREditor->TileChange(-1,0);
		if(Key==VK_NUMPAD5) FormMain->MCHREditorClick(Sender);
		if(Key==VK_NUMPAD6) FormCHREditor->TileChange(+1,0);

		if(Key==VK_NUMPAD1) FormCHREditor->TileChange(-1,+1);
		if(Key==VK_NUMPAD2) FormCHREditor->TileChange( 0,+1);
		if(Key==VK_NUMPAD3) FormCHREditor->TileChange(+1,+1);


	}

	if(FormMain->PageControlEditor->ActivePage==FormMain->TabSheetName)
	{
		/*
		if(Key==VK_NUMPAD8) FormMain->MovePaletteCursor(-16);
		if(Key==VK_NUMPAD4) FormMain->MovePaletteCursor(-1);
		if(Key==VK_NUMPAD6) FormMain->MovePaletteCursor(1);
		if(Key==VK_NUMPAD5||Key==VK_NUMPAD2) FormMain->MovePaletteCursor(16);

		if(Key==VK_NUMPAD7){bgPalCur=(bgPalCur-1)&3; FormMain->UpdateAll(); }
		if(Key==VK_NUMPAD9){bgPalCur=(bgPalCur+1)&3; FormMain->UpdateAll(); }
		if(Key==VK_NUMPAD1){palActive=(palActive-1)&3; FormMain->UpdateAll(); }

		if(Key==VK_NUMPAD3) {palActive=(palActive+1)&3; FormMain->UpdateAll(); }
		*/



         //todo: maybe make use comma, period?
		if(Shift.Contains(ssCtrl))
		{
			if(Key==VK_OEM_4) FormMain->ChangeNameTableFrame(-1);// [
			if(Key==VK_OEM_6) FormMain->ChangeNameTableFrame(1);// ]
		}
	}

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

	int dd = iBrushSize[iBrushPresetIndex];
	int d = PaintBoxMask->Width;
	int size=d/dd;
	int x=X/size;
	int y=Y/size;

	x_brushStatus=x;
	y_brushStatus=y;

	if(Shift.Contains(ssLeft) && (x>=0) && (x<dd) && (y>=0) && (y<dd))
	{
		 if(!bForbidMaskPen)(*ptr_tableBrush[iBrushPresetIndex])[x][y] = bMaskPenColour;
	}
    
	StatTimer->Enabled=true;
	BrushmaskTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::BrushmaskTimerTimer(TObject *Sender)
{
	BrushmaskTimer->Enabled=false;
	Draw();
}
//---------------------------------------------------------------------------
void  __fastcall TFormBrush::ToggleSpeedButtonByTag(TGroupBox* groupBox, int iTagToToggle, int iGroupIndex, int mode)
{
	//modes:
	//1   down
	//0   up
	//-1  toggle

	for (int i = 0; i < groupBox->ControlCount; i++)
	{
		TSpeedButton* speedButton = dynamic_cast<TSpeedButton*>(groupBox->Controls[i]);

		if (speedButton != NULL && speedButton->GroupIndex == iGroupIndex)
		{
			if (speedButton->Tag == iTagToToggle)
			{
				if(mode ==  1)speedButton->Down = true;
				if(mode ==  0)speedButton->Down = false;
				if(mode == -1)speedButton->Down = !speedButton->Down;

			}
		}
	}
}
void __fastcall TFormBrush::SpeedButton3Click(TObject *Sender)
{
	TSpeedButton *speedButton = dynamic_cast<TSpeedButton*>(Sender);
	if (speedButton){
		iBrushPresetIndex = speedButton->Tag;
	}
	SetBrushAnchor();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
    MakeRotBuffer();
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::FormDestroy(TObject *Sender)
{
	delete brushMaskBuffer;
}
//---------------------------------------------------------------------------
void __fastcall TFormBrush::UpdatePaintBoxModulo(void){

  int d = PaintBoxMask->Width;





  int dd=iBrushSize[iBrushPresetIndex];
  int cell=d/dd;

  int modulo=84%dd;
  int manual;
  if(dd==16 || dd==13) manual = cell+modulo;
  else manual = 0;
  PaintBoxMask->Width=84+modulo+manual;
  PaintBoxMask->Height=84+modulo+manual;
  brushMaskBuffer->Width=84+modulo+manual;
  brushMaskBuffer->Height=84+modulo+manual;
  bDontUpdatePaintModulo=true;
  unsigned int tmp = iBrushSize[iBrushPresetIndex]-modulo;
  if (tmp == (unsigned)iBrushSize[iBrushPresetIndex]) tmp=0;

  if(dd<9){
	PaintBoxMask->Left=5-(tmp/2);
	PaintBoxMask->Top=5-(tmp/2);
  }
   else if(dd==9){
	PaintBoxMask->Left=7;
	PaintBoxMask->Top=7;
  }
  else if(dd==10){
	PaintBoxMask->Left=8;
	PaintBoxMask->Top=8;
  }
  else if(dd==11){
	PaintBoxMask->Left=4;
	PaintBoxMask->Top=4;
  }
  else if(dd==12){
	PaintBoxMask->Left=6;
	PaintBoxMask->Top=6;
  }
  else if(dd==13){
	PaintBoxMask->Left=3;
	PaintBoxMask->Top=3;
  }
  else if(dd==14){
	PaintBoxMask->Left=7;
	PaintBoxMask->Top=7;
  }
  else if(dd==15){
	PaintBoxMask->Left=3;
  	PaintBoxMask->Top=3;
  }
  else if(dd==16){
	PaintBoxMask->Left=1;
	PaintBoxMask->Top=1;
  }

  bDontUpdatePaintModulo=false;
}
void __fastcall TFormBrush::SpeedButton30Click(TObject *Sender)
{

	iBrushSize[iBrushPresetIndex]++;
	if(iBrushSize[iBrushPresetIndex]>16) iBrushSize[iBrushPresetIndex]=2;
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);

	if (bBufCtrl)
	{
		if (bBufShift)
		{
			iBrushSnapSize_x[iBrushPresetIndex]++;
			iBrushSnapSize_y[iBrushPresetIndex]++;

			if(iBrushSnapSize_x[iBrushPresetIndex]>16) iBrushSnapSize_x[iBrushPresetIndex]=2;
			if(iBrushSnapSize_y[iBrushPresetIndex]>16) iBrushSnapSize_y[iBrushPresetIndex]=2;

		}
		else{
			iBrushSnapSize_x[iBrushPresetIndex]=iBrushSize[iBrushPresetIndex];
			iBrushSnapSize_y[iBrushPresetIndex]=iBrushSize[iBrushPresetIndex];

		}
		Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
		Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);

	}
    MakeRotBuffer();
	BrushmaskTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton28Click(TObject *Sender)
{
	iBrushSize[iBrushPresetIndex]--;
	if(iBrushSize[iBrushPresetIndex]<2) iBrushSize[iBrushPresetIndex]=16;
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);

	if (bBufCtrl)
	{
		if (bBufShift)
		{
			iBrushSnapSize_x[iBrushPresetIndex]--;
			iBrushSnapSize_y[iBrushPresetIndex]--;

			if(iBrushSnapSize_x[iBrushPresetIndex]>2) iBrushSnapSize_x[iBrushPresetIndex]=16;
			if(iBrushSnapSize_y[iBrushPresetIndex]>2) iBrushSnapSize_y[iBrushPresetIndex]=16;

		}
		else{
			iBrushSnapSize_x[iBrushPresetIndex]=iBrushSize[iBrushPresetIndex];
			iBrushSnapSize_y[iBrushPresetIndex]=iBrushSize[iBrushPresetIndex];

		}
		Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
		Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);

	}
    MakeRotBuffer();
	BrushmaskTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton31Click(TObject *Sender)
{
	int tag;
	TSpeedButton *speedButton = dynamic_cast<TSpeedButton*>(Sender);
	if (speedButton){
		tag = speedButton->Tag;
	}
	bool doX, doY;
	if(tag==0) {doX=true; doY=true;}
	if(tag==1) {doX=true; doY=false;}
	if(tag==2) {doX=false; doY=true;}

	if(doX) iBrushSnapSize_x[iBrushPresetIndex]++;
	if(doY) iBrushSnapSize_y[iBrushPresetIndex]++;

	if(iBrushSnapSize_x[iBrushPresetIndex]>16) iBrushSnapSize_x[iBrushPresetIndex]=1;
	if(iBrushSnapSize_y[iBrushPresetIndex]>16) iBrushSnapSize_y[iBrushPresetIndex]=1;

	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);

	if (bBufCtrl)
	{
		if (bBufShift)
		{
			iBrushSize[iBrushPresetIndex]++;
			if(iBrushSize[iBrushPresetIndex]>16) iBrushSize[iBrushPresetIndex]=2;
		}
		else{
			if(iBrushSnapSize_x[iBrushPresetIndex]>=iBrushSnapSize_y[iBrushPresetIndex])
				iBrushSize[iBrushPresetIndex]=iBrushSnapSize_x[iBrushPresetIndex];
			else  iBrushSize[iBrushPresetIndex]=iBrushSnapSize_y[iBrushPresetIndex];
		}
		Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	}
    MakeRotBuffer();
	BrushmaskTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton29Click(TObject *Sender)
{
	int tag;
	TSpeedButton *speedButton = dynamic_cast<TSpeedButton*>(Sender);
	if (speedButton){
		tag = speedButton->Tag;
	}
	bool doX, doY;
	if(tag==0) {doX=true; doY=true;}
	if(tag==1) {doX=true; doY=false;}
	if(tag==2) {doX=false; doY=true;}

	if(doX) iBrushSnapSize_x[iBrushPresetIndex]--;
	if(doY) iBrushSnapSize_y[iBrushPresetIndex]--;

	if(iBrushSnapSize_x[iBrushPresetIndex]<1) iBrushSnapSize_x[iBrushPresetIndex]=16;
	if(iBrushSnapSize_y[iBrushPresetIndex]<1) iBrushSnapSize_y[iBrushPresetIndex]=16;

	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);

	if (bBufCtrl)
	{
		if (bBufShift)
		{
			iBrushSize[iBrushPresetIndex]--;
			if(iBrushSize[iBrushPresetIndex]<2) iBrushSize[iBrushPresetIndex]=16;
		}
		else{
            if(iBrushSnapSize_x[iBrushPresetIndex]>=iBrushSnapSize_y[iBrushPresetIndex])
				iBrushSize[iBrushPresetIndex]=iBrushSnapSize_x[iBrushPresetIndex];
			else iBrushSize[iBrushPresetIndex]=iBrushSnapSize_y[iBrushPresetIndex];

		}
		Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	}
    MakeRotBuffer();
	BrushmaskTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	FormCHREditor->FormKeyUp(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton24MouseLeave(TObject *Sender)
{
		FormMain->LabelStats->Caption="---";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton24MouseEnter(TObject *Sender)
{
		FormMain->LabelStats->Caption="Flips the mask horizontally.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton25MouseEnter(TObject *Sender)
{
		FormMain->LabelStats->Caption="Flips the mask vertically.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton27MouseEnter(TObject *Sender)
{
		 FormMain->LabelStats->Caption="Rotates the mask counter-clockwise.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton26MouseEnter(TObject *Sender)
{
		 FormMain->LabelStats->Caption="Rotates the mask clockwise.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::GroupBox2MouseEnter(TObject *Sender)
{
		 FormMain->LabelStats->Caption="Picks the anchor from which the brush is drawn when the mouse clicks on the canvas.\nCenter is most typical.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton32MouseEnter(TObject *Sender)
{
		 FormMain->LabelStats->Caption="Loads a set (or stores a set to a user slot).\n Sets are organized in 7 brushes. When loading, you replace the brushes of either set A or B,\n depending on which is selected.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton30MouseEnter(TObject *Sender)
{
		 FormMain->LabelStats->Caption="Increase brush size current brush.\nCtrl-click increases, then matches 'snap size' to 'brush size'.\nCtrl+Shift-click increases both, but with independent values.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton28MouseEnter(TObject *Sender)
{
	 FormMain->LabelStats->Caption="Decrease brush size of current brush.\nCtrl-click decreases, then matches 'snap size' to 'brush size'.\nCtrl+Shift-click decreases both, but with independent values.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton31MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Increase snap size of current brush.\nQuantization is toggled on/off when tile editing with [U].\nCtrl-click decreases, then matches 'brush size' to 'snap size'.\nCtrl+Shift-click increases both, but with independent values.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton29MouseEnter(TObject *Sender)
{
		FormMain->LabelStats->Caption="Decrease snap size of current brush.\nQuantization is toggled on/off when tile editing with [U].\nCtrl-click decreases, then matches 'brush size' to 'snap size'.\nCtrl+Shift-click decreases both, but with independent values.";
}
//---------------------------------------------------------------------------


void __fastcall TFormBrush::SpeedButton3MouseEnter(TObject *Sender)
{
   TSpeedButton *hoveredBtn = dynamic_cast<TSpeedButton*>(Sender);

	if (hoveredBtn)
	{

		int tag = hoveredBtn->Tag;
		AnsiString cap = hoveredBtn->Caption;
		int brSize = iBrushSize[tag];
		int snSize_x = iBrushSnapSize_x[tag];
		int snSize_y = iBrushSnapSize_y[tag];

		FormMain->LabelStats->Caption="Brush "+cap+"\nSize: "+IntToStr(brSize)+"\tSnap: "+IntToStr(snSize_x)+","+IntToStr(snSize_y);
   }
}
//---------------------------------------------------------------------------


void __fastcall TFormBrush::StatTimerTimer(TObject *Sender)
{

	StatTimer->Enabled=false;
	AnsiString str="x,y: "+IntToStr(x_brushStatus)+","+IntToStr(y_brushStatus);

	int d=iBrushSize[iBrushPresetIndex];
	bool even = (d % 2== 0)? true:false;

	int x1 = x_brushStatus;
	int x2 = even? x1+1:x1;

	int y1 = y_brushStatus;
	int y2 = even? y1+1:y1;


	if(d>2){

		if(even)
		{
        	if((d/2==x1 || d/2==x2) && (d/2==y1 || d/2==y2)){
				AppendStr(str," [xy @ ~center]");
			}
			else if(d/2==x1 || d/2==x2) AppendStr(str," [x  @ ~center]");
			else if(d/2==y1 || d/2==y2) AppendStr(str," [y  @ ~center]");

		}
		else
		{
			if((d/2==x1 || d/2==x2) && (d/2==y1 || d/2==y2)){
				AppendStr(str," [xy @ center]");
			}
			else if(d/2==x1 || d/2==x2) AppendStr(str," [x  @ center]");
			else if(d/2==y1 || d/2==y2) AppendStr(str," [y  @ center]");
		}
	}
	///AppendStr(str,"d2: "+IntToStr(d/2));
	LabelXY->Caption=str;

}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::PaintBoxMaskMouseLeave(TObject *Sender)
{
   LabelXY->Caption="x,y: ";
}
//---------------------------------------------------------------------------




void __fastcall TFormBrush::SpeedButton32Click(TObject *Sender)
{
	bool set = iBrushPresetIndex<7? 0:7;
	Currentset2->Caption= set? "Save current set (B)...":"Save current set (A)...";

	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;
	PopupMenu1->Popup(x,y);
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton35Click(TObject *Sender)
{
	for(int y=0; y<16; y++){
		for(int x=0; x<16; x++){
			(*ptr_tableBrush[iBrushPresetIndex])[x][y] ^= true;
		}
	}
	MakeRotBuffer();
	BrushmaskTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton34Click(TObject *Sender)
{
   for(int y=0; y<16; y++){
		for(int x=0; x<16; x++){
			(*ptr_tableBrush[iBrushPresetIndex])[x][y] = false;
		}
	}
	MakeRotBuffer();
	BrushmaskTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton33Click(TObject *Sender)
{
   for(int y=0; y<16; y++){
		for(int x=0; x<16; x++){
			(*ptr_tableBrush[iBrushPresetIndex])[x][y] = true;
		}
	}
	MakeRotBuffer();
	BrushmaskTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton25Click(TObject *Sender)
{
    FormMain->SetUndo();
	size_t d= iBrushSize[iBrushPresetIndex];
	int** arr = new int*[d];
	for (size_t i = 0; i < d; ++i) arr[i] = new int[d];

	for(size_t y=0; y<d; y++){
		for(size_t x=0; x<d; x++){
			arr[x][d-1-y]=(*ptr_tableBrush[iBrushPresetIndex])[x][y];
		}
	}
	for(size_t y=0; y<d; y++){
		for(size_t x=0; x<d; x++){
			(*ptr_tableBrush[iBrushPresetIndex])[x][y]=arr[x][y];
		}
	}
	BrushmaskTimer->Enabled=true;

	for (size_t i = 0; i < d; ++i) {
		delete[] arr[i];
	}
	delete[] arr;
	MakeRotBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton24Click(TObject *Sender)
{
    FormMain->SetUndo();
	size_t d= iBrushSize[iBrushPresetIndex];
	int** arr = new int*[d];
	for (size_t i = 0; i < d; ++i) arr[i] = new int[d];

	for(size_t y=0; y<d; y++){
		for(size_t x=0; x<d; x++){
			arr[d-1-x][y]=(*ptr_tableBrush[iBrushPresetIndex])[x][y];
		}
	}
	for(unsigned int y=0; y<d; y++){
		for(unsigned int x=0; x<d; x++){
			(*ptr_tableBrush[iBrushPresetIndex])[x][y]=arr[x][y];
		}
	}
	BrushmaskTimer->Enabled=true;

	for (size_t i = 0; i < d; ++i) {
		delete[] arr[i];
	}
	delete[] arr;
	MakeRotBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton26Click(TObject *Sender)
{
    FormMain->SetUndo();
	size_t d= iBrushSize[iBrushPresetIndex];
	int** arr = new int*[d];
	for (size_t i = 0; i < d; ++i) arr[i] = new int[d];

	for(size_t y=0; y<d; y++){
		for(size_t x=0; x<d; x++){
			arr[d-1-y][x]=(*ptr_tableBrush[iBrushPresetIndex])[x][y];
		}
	}
	for(size_t y=0; y<d; y++){
		for(size_t x=0; x<d; x++){
			(*ptr_tableBrush[iBrushPresetIndex])[x][y]=arr[x][y];
		}
	}
	BrushmaskTimer->Enabled=true;

	for (size_t i = 0; i < d; ++i) {
		delete[] arr[i];
	}
	delete[] arr;
	MakeRotBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton27Click(TObject *Sender)
{
	FormMain->SetUndo();
	size_t d= iBrushSize[iBrushPresetIndex];
	int** arr = new int*[d];
	for (size_t i = 0; i < d; ++i) arr[i] = new int[d];

	for(size_t y=0; y<d; y++){
		for(size_t x=0; x<d; x++){
			arr[y][d-1-x]=(*ptr_tableBrush[iBrushPresetIndex])[x][y];
		}
	}
	for(size_t y=0; y<d; y++){
		for(size_t x=0; x<d; x++){
			(*ptr_tableBrush[iBrushPresetIndex])[x][y]=arr[x][y];
		}
	}
	BrushmaskTimer->Enabled=true;

	for (size_t i = 0; i < d; ++i) {
		delete[] arr[i];
	}
	delete[] arr;
	MakeRotBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton36Click(TObject *Sender)
{
	FormMain->SetUndo();

	size_t d= iBrushSize[iBrushPresetIndex];
	int** arr = new int*[d];
	for (size_t i = 0; i < d; ++i) arr[i] = new int[d];


	if(!bBufCtrl){
		for(size_t y=0; y<d/2; y++){
			for(size_t x=0; x<d; x++){
				arr[x][y]=(*ptr_tableBrush[iBrushPresetIndex])[x][y];
			}
		}
		for(size_t y=0; y<d/2; y++){
			for(size_t x=0; x<d; x++){
				(*ptr_tableBrush[iBrushPresetIndex])[x][d-1-y]=arr[x][y];
			}
		}
	}
	else if(bBufCtrl){
		for(size_t y=0; y<d/2; y++){
			for(size_t x=0; x<d; x++){
				arr[x][y]=(*ptr_tableBrush[iBrushPresetIndex])[x][d-1-y];
			}
		}
		for(size_t y=0; y<d/2; y++){
			for(size_t x=0; x<d; x++){
				(*ptr_tableBrush[iBrushPresetIndex])[x][y]=arr[x][y];
			}
		}
	}
	BrushmaskTimer->Enabled=true;

	for (size_t i = 0; i < d; ++i) {
		delete[] arr[i];
	}
	delete[] arr;
	MakeRotBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton37Click(TObject *Sender)
{
    FormMain->SetUndo();

	size_t d= iBrushSize[iBrushPresetIndex];
	int** arr = new int*[d];
	for (size_t i = 0; i < d; ++i) arr[i] = new int[d];


	if(!bBufCtrl){
		for(size_t y=0; y<d; y++){
			for(size_t x=0; x<d/2; x++){
				arr[x][y]=(*ptr_tableBrush[iBrushPresetIndex])[x][y];
			}
		}
		for(size_t y=0; y<d; y++){
			for(size_t x=0; x<d/2; x++){
				(*ptr_tableBrush[iBrushPresetIndex])[d-1-x][y]=arr[x][y];
			}
		}
	}
	else if(bBufCtrl){
		for(size_t y=0; y<d; y++){
			for(size_t x=0; x<d/2; x++){
				arr[x][y]=(*ptr_tableBrush[iBrushPresetIndex])[d-1-x][y];
			}
		}
		for(size_t y=0; y<d; y++){
			for(size_t x=0; x<d/2; x++){
				(*ptr_tableBrush[iBrushPresetIndex])[x][y]=arr[x][y];
			}
		}
	}
	BrushmaskTimer->Enabled=true;

	for (size_t i = 0; i < d; ++i) {
		delete[] arr[i];
	}
	delete[] arr;
	MakeRotBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton23Click(TObject *Sender)
{
	int tag=4;
	TSpeedButton *speedButton = dynamic_cast<TSpeedButton*>(Sender);
	if (speedButton){
		tag = speedButton->Tag;
	}
	if(bBufCtrl){
		for(int i=0;i<14;i++){
        	iBrushCursorAlignment[i]=tag;
		}
	}
	iBrushCursorAlignment[iBrushPresetIndex]=tag;
    SetBrushAnchor();

}
//---------------------------------------------------------------------------
void __fastcall TFormBrush::SetBrushAnchor(void)
{
  int d= iBrushSize[iBrushPresetIndex];
  int tag=iBrushCursorAlignment[iBrushPresetIndex];
  float half = (float)(d-1)/2.0;
  float full = (float)(d-1)/1.0;
  if(tag==0){ 	brush_x_anchor= 0;  		brush_y_anchor=0;}  	//top left
  if(tag==1){ 	brush_x_anchor= half;		brush_y_anchor=0;}  	//top center
  if(tag==2){ 	brush_x_anchor= full; 		brush_y_anchor=0;}  	//top right

  if(tag==3){ 	brush_x_anchor=0;			brush_y_anchor= half;}  	//center left
  if(tag==4){ 	brush_x_anchor= half; 		brush_y_anchor= half;}  	//center-center
  if(tag==5){ 	brush_x_anchor= full; 		brush_y_anchor= half;}  	//center right

  if(tag==6){ 	brush_x_anchor=0;			brush_y_anchor= full;}  	//bottom left
  if(tag==7){ 	brush_x_anchor= half;		brush_y_anchor= full;}  	//bottom center
  if(tag==8){ 	brush_x_anchor= full; 		brush_y_anchor= full;}  	//bottom right
}
//---------------------------------------------------------------------------


void __fastcall TFormBrush::SpeedButton39MouseEnter(TObject *Sender)
{
		FormMain->LabelStats->Caption="Decrease snap width of current brush.\nQuantization is toggled on/off when tile editing with [U].\nCtrl-click decreases, then matches 'brush size' to 'snap size'.\nCtrl+Shift-click decreases both, but with independent values.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton41MouseEnter(TObject *Sender)
{
		FormMain->LabelStats->Caption="Decrease snap height of current brush.\nQuantization is toggled on/off when tile editing with [U].\nCtrl-click decreases, then matches 'brush size' to 'snap size'.\nCtrl+Shift-click decreases both, but with independent values.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton38MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Increase snap width of current brush.\nQuantization is toggled on/off when tile editing with [U].\nCtrl-click decreases, then matches 'brush size' to 'snap size'.\nCtrl+Shift-click increases both, but with independent values.";

}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton40MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Increase snap height of current brush.\nQuantization is toggled on/off when tile editing with [U].\nCtrl-click decreases, then matches 'brush size' to 'snap size'.\nCtrl+Shift-click increases both, but with independent values.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton42Click(TObject *Sender)
{
	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;
	PopupMenu2->Popup(x,y);
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Fromsolidpixels1Click(TObject *Sender)
{
	int x,y,w,h;
	int tx,ty;
	int src,dst;
	char tileData[4*16];

	extern TRect nameSelection;
	extern TRect chrSelection;
	extern unsigned char nameTable[];
	extern unsigned char tileViewTable[];
	extern int nameTableWidth;
	extern int bankActive;
	extern unsigned char *chr;

	bool bIsNT=false;
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	if(nameSelection.left>=0 && nameSelection.top>=0){
		FormMain->GetSelection(nameSelection,x,y,w,h);
		bIsNT=true;
	}
	else{
		FormMain->GetSelection(chrSelection,x,y,w,h);

	}
	//cap selection to the brush max area.
	if(w>2)w=2;
	if(h>2)h=2;
	//Always put down a 16x16 area, but make 8 pixels repeat if that axis is short.
	//get tile data
	dst=0;
	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			for(int k=0;k<16;k++){
					tx= w<2? 0:j;
					ty= h<2? 0:i;
					if (bIsNT)  src=bankActive + nameTable[(y+ty)*nameTableWidth+(tx+x)]*16 +k;
					else 		src=bankActive + tileViewTable[(x+tx) + (y+ty)*16]*16 + k;

					tileData[dst++]=chr[src];

			}
		}
	}



	//pick interpretation method

	for(int i=0; i<4; i++){
		for(int br_y=0;br_y<8;br_y++){
			for(int br_x=0;br_x<8;br_x++){

				src= i*16 + br_y;
				if(tag==0){ //all solid pixels
					(*ptr_tableBrush[iBrushPresetIndex])[br_x +(i%2)*8][br_y + (i/2)*8]=
						((tileData[src]>>(7-br_x))&1)
						|((tileData[src+8]>>(7-br_x))&1);
				}
				if(tag==1){ //bitplane 0
					(*ptr_tableBrush[iBrushPresetIndex])[br_x +(i%2)*8][br_y + (i/2)*8]=
						((tileData[src]>>(7-br_x))&1);
				}
				if(tag==2){ //bitplane 2
					(*ptr_tableBrush[iBrushPresetIndex])[br_x +(i%2)*8][br_y + (i/2)*8]=
						((tileData[src+8]>>(7-br_x))&1);
				}
				if(tag==3){  //col 0
					(*ptr_tableBrush[iBrushPresetIndex])[br_x +(i%2)*8][br_y + (i/2)*8]=
						(~(tileData[src]>>(7-br_x))&1)
						&(~(tileData[src+8]>>(7-br_x))&1);
				}
				if(tag==4){ //col 1
					(*ptr_tableBrush[iBrushPresetIndex])[br_x +(i%2)*8][br_y + (i/2)*8]=
						((tileData[src]>>(7-br_x))&1)
						&(~(tileData[src+8]>>(7-br_x))&1);
				}
				if(tag==5){ //col 2
					(*ptr_tableBrush[iBrushPresetIndex])[br_x +(i%2)*8][br_y + (i/2)*8]=
						(~(tileData[src]>>(7-br_x))&1)
						&((tileData[src+8]>>(7-br_x))&1);
				}
				if(tag==6){ //col 3
					(*ptr_tableBrush[iBrushPresetIndex])[br_x +(i%2)*8][br_y + (i/2)*8]=
						((tileData[src]>>(7-br_x))&1)
						&((tileData[src+8]>>(7-br_x))&1);
				}



			}
		}
	}
	int newSize = w<2? 8:16;

	iBrushSize[iBrushPresetIndex]=newSize;
	iBrushSnapSize_x[iBrushPresetIndex]=newSize;
	iBrushSnapSize_y[iBrushPresetIndex]=newSize;
    MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

 void __fastcall TFormBrush::ChangePreset(int step)
{
	if(step<2){
		iBrushPresetIndex+=step;
		if(iBrushPresetIndex<0)iBrushPresetIndex=13;
		if(iBrushPresetIndex>13)iBrushPresetIndex=0;
	}
	else{      //toggle row
	   if(iBrushPresetIndex<7) iBrushPresetIndex+=step;
	   else					   iBrushPresetIndex-=step;
	}
	ToggleSpeedButtonByTag(GroupBox1, iBrushPresetIndex, 	 1, 1);
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	BrushmaskTimer->Enabled=true;
}
void __fastcall TFormBrush::Filledsquares281Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt = 0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);
		cnt++;
		for(int j=0;j<256;j++){
			(*ptr_tableBrush[i])[j/16][j%16] = 1;
		}
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Outlinedsquares281Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt = 0;
	int d;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);
		d=iBrushSize[i];
		cnt++;
		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				bool tmp = (j == 0 || j == d - 1 || k == 0 || k == d - 1);
				(*ptr_tableBrush[i])[j][k] = tmp;
			}
		}
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Circles281Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[j][k] = (*ptr_tB_circle_filled[cnt+tag])[k][j];
			}
		}
		cnt++;
	}
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Circlesoutlined281Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[j][k] = (*ptr_tB_circle_line[cnt+tag])[k][j];
			}
		}
		cnt++;
	}
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Circlesjaggy391Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[j][k] = (*ptr_tB_circle_jaggies[cnt+tag])[k][j];
			}
		}
		cnt++;
	}
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Diamondsoutlined391Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[j][k] = (*ptr_tB_diamond_line[cnt+tag])[k][j];
			}
		}
		cnt++;
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------


void __fastcall TFormBrush::Diamondsjaggy391Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[j][k] = (*ptr_tB_diamond_jaggies[cnt+tag])[k][j];
			}
		}
		cnt++;
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Diamondsfilled391Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[j][k] = (*ptr_tB_diamond_filled[cnt+tag])[k][j];
			}
		}
		cnt++;
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::N901Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[k][j] = (bool)tB_90_45_45_wedge_filled[j][k];
			}
		}
		cnt++;
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::N90wedgesoutlined281Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[j][k] = (*ptr_tB_wedge_90_45_45_line[cnt+tag])[k][j];
			}
		}
		cnt++;
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Roundedsquaresfilled4101Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt = 0;
	int d;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);
		d=iBrushSize[i];
		cnt++;
		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				bool tmp = (j == 0 || j == d-1 || k == 0 || k == d-1);
				tmp = (
					(j == 0 && k == 0)
					|| (j == 0 && k == d-1)
					|| (j == d-1 && k == 0)
					|| (j == d-1 && k == d-1))? false:tmp;
				(*ptr_tableBrush[i])[j][k] = tmp;
			}
		}
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Roundedsquares4101Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt = 0;
	int d;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);
		d=iBrushSize[i];
		cnt++;
		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				bool tmp = (j >= 0 && j <= d-1 && k >= 0 && k <= d-1);
				tmp = (
					(j == 0 && k == 0)
					|| (j == 0 && k == d-1)
					|| (j == d-1 && k == 0)
					|| (j == d-1 && k == d-1))? false:tmp;
				(*ptr_tableBrush[i])[j][k] = tmp;
			}
		}
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Roundedsquaresjaggy4101Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt = 0;
	int d;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(cnt+tag);
		iBrushSnapSize_x[i]=(cnt+tag);
		iBrushSnapSize_y[i]=(cnt+tag);
		d=iBrushSize[i];
		cnt++;
		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				bool tmp = (j == 0 || j == d-1 || k == 0 || k == d-1);
				tmp = (
					(j == 0 && k == 0)
					|| (j == 0 && k == d-1)
					|| (j == d-1 && k == 0)
					|| (j == d-1 && k == d-1))? false:tmp;
				tmp = (
					(j == 1 && k == 1)
					|| (j == 1 && k == d-2)
					|| (j == d-2 && k == 1)
					|| (j == d-2 && k == d-2))? true:tmp;
				(*ptr_tableBrush[i])[j][k] = tmp;
			}
		}
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Linesat7angles8x81Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(tag);
		iBrushSnapSize_x[i]=(tag);
		iBrushSnapSize_y[i]=(tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[j][k] = (*ptr_line_angle[cnt])[k][j];
			}
		}
		cnt++;
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::Linesat7anglesjaggy8x81Click(TObject *Sender)
{
	FormMain->SetUndo();
	int tag=0;
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if (menuItem){
		tag = menuItem->Tag;
	}

	int set = iBrushPresetIndex<7? 0:7;
	int end = set + 7;
	int cnt =0;
	for(int i=set;i<end;i++){
		iBrushSize[i]=(tag);
		iBrushSnapSize_x[i]=(tag);
		iBrushSnapSize_y[i]=(tag);

		for(int j=0;j<16;j++){
			for(int k=0;k<16;k++){
				(*ptr_tableBrush[i])[j][k] = (*ptr_line_jagged[cnt])[k][j];
			}
		}
		cnt++;
	}
	MakeRotBuffer();
	Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
	Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
	Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
	Draw();
}
//---------------------------------------------------------------------------


void __fastcall TFormBrush::Currentsinglebrush1Click(TObject *Sender)
{
    TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	int tag;
	if (menuItem){
		tag = menuItem->Tag;
	}

	//int set = iBrushPresetIndex<7? 0:7;
   AnsiString ext;

   if(tag==0){
		FormMain->SaveDialogBrushes->Title=Currentsinglebrush2->Caption;
		FormMain->SaveDialogBrushes->Filter="Single Brush|*.bm1|Any (*.*)|*.*";
		ext=".bm1";
   }
   if(tag==1){
		FormMain->SaveDialogBrushes->Title=Currentset2->Caption;
		FormMain->SaveDialogBrushes->Filter="Brush set (*.bm7)|*.bm7|Any (*.*)|*.*";
		ext=".bm7";
   }
   if(tag==2){
		FormMain->SaveDialogBrushes->Title=Doubleset2->Caption;
		FormMain->SaveDialogBrushes->Filter="Double Brush set|*.bm14|Any (*.*)|*.*";
		ext=".bm14";
   }

	FormMain->BlockDrawing(true);
	FormMain->SaveDialogBrushes->FileName=RemoveExt(FormMain->SaveDialogBrushes->FileName)+ext;

	if(FormMain->SaveDialogBrushes->Execute())
	{
		bool customExt=false;
		if(GetExt(FormMain->SaveDialogBrushes->FileName)==".bm1")ext=".bm1";
		else if(GetExt(FormMain->SaveDialogBrushes->FileName)==".bm7")ext=".bm7";
		else if(GetExt(FormMain->SaveDialogBrushes->FileName)==".bm14")ext=".bm14";
		else customExt=true;

		AnsiString name;
		if(!customExt) name = RemoveExt(FormMain->SaveDialogBrushes->FileName)+ext;
		else name = FormMain->SaveDialogBrushes->FileName;
		FormMain->SaveBrushes(name, ext, customExt);

	}
	FormMain->BlockDrawing(false);
}
//---------------------------------------------------------------------------

 void __fastcall TFormBrush::ShiftLeft(void)
{
	FormMain->SetUndo();
	int d= iBrushSize[iBrushPresetIndex];
	int tmp[16];

	for(int y=0;y<d;y++){
	   tmp[y] = (*ptr_tableBrush[iBrushPresetIndex])[0][y];
	}

	for(int x=0;x<d-1;x++){
		for(int y=0;y<d;y++){
			(*ptr_tableBrush[iBrushPresetIndex])[x][y] = (*ptr_tableBrush[iBrushPresetIndex])[x+1][y];
		}
	}

	for(int y=0;y<d;y++){
	   (*ptr_tableBrush[iBrushPresetIndex])[d-1][y] = tmp[y];
	}
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::ShiftRight(void)
{
	FormMain->SetUndo();
	int d = iBrushSize[iBrushPresetIndex];
	int tmp[16];

	for (int y=0;y<d;y++) {
		tmp[y] = (*ptr_tableBrush[iBrushPresetIndex])[d-1][y];
	}


	for (int x=d-1;x>0;x--) {
		for (int y=0;y<d;y++) {
			(*ptr_tableBrush[iBrushPresetIndex])[x][y] = (*ptr_tableBrush[iBrushPresetIndex])[x - 1][y];
		}
	}

	for (int y=0;y<d;y++) {
		(*ptr_tableBrush[iBrushPresetIndex])[0][y] = tmp[y];
	}

	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::ShiftUp(void)
{
	FormMain->SetUndo();
	int d= iBrushSize[iBrushPresetIndex];
	int tmp[16];

	for(int x=0;x<d;x++){
	   tmp[x] = (*ptr_tableBrush[iBrushPresetIndex])[x][0];
	}

	for(int x=0;x<d;x++){
		for(int y=0;y<d-1;y++){
			(*ptr_tableBrush[iBrushPresetIndex])[x][y] = (*ptr_tableBrush[iBrushPresetIndex])[x][y+1];
		}
	}

	for(int x=0;x<d;x++){
	   (*ptr_tableBrush[iBrushPresetIndex])[x][d-1] = tmp[x];
	}
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::ShiftDown(void)
{
    FormMain->SetUndo();
	int d = iBrushSize[iBrushPresetIndex];
	int tmp[16];

	for (int x=0;x<d;x++) {
		tmp[x] = (*ptr_tableBrush[iBrushPresetIndex])[x][d-1];
	}


	for (int x=0;x<d;x++) {
		for (int y=d-1;y>0;y--) {
			(*ptr_tableBrush[iBrushPresetIndex])[x][y] = (*ptr_tableBrush[iBrushPresetIndex])[x][y-1];
		}
	}

	for (int x = 0; x < d; x++) {
		(*ptr_tableBrush[iBrushPresetIndex])[x][0] = tmp[x];
	}

	Draw();
}
void __fastcall TFormBrush::SpeedButton44Click(TObject *Sender)
{
	if(!FormMain->OpenDialogBrushes->Execute()) return;

  FormMain->BlockDrawing(true);

  if(FormMain->OpenBrushes(FormMain->OpenDialogBrushes->FileName))
  {
		FormMain->OpenDialogBrushes->FileName=RemoveExt(FormMain->OpenDialogBrushes->FileName);
        ToggleSpeedButtonByTag(GroupBox1, iBrushPresetIndex, 	 1, 1);
		ToggleSpeedButtonByTag(GroupBox2, iBrushCursorAlignment[iBrushPresetIndex], 1, 1);

		SetBrushAnchor();
		Label1->Caption=IntToStr(iBrushSize[iBrushPresetIndex]);
		Label2->Caption="x"+IntToStr(iBrushSnapSize_x[iBrushPresetIndex]);
		Label3->Caption="y"+IntToStr(iBrushSnapSize_y[iBrushPresetIndex]);
        MakeRotBuffer();
		Draw();
  }

	FormMain->BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::SpeedButton43Click(TObject *Sender)
{
		bool set = iBrushPresetIndex<7? 0:7;
	Currentset2->Caption= set? "Save current set (B)...":"Save current set (A)...";

	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;
	PopupMenu3->Popup(x,y);	
}
//---------------------------------------------------------------------------




void __fastcall TFormBrush::MakeRotBuffer(void){
  
  int d = iBrushSize[iBrushPresetIndex];
		//clear buffer
		for(int x=0;x<32;x++){
			for(int y=0;y<32;y++){
				maskRotateBuf[y][x] = 0;
			}
		}
		int startPixel = (32 -d )/2 ;

		//populate buffer

		for(int x=0;x<d;x++){
			for(int y=0;y<d;y++){
				maskRotateBuf[y+startPixel][x+startPixel] = (*ptr_tableBrush[iBrushPresetIndex])[x][y];
			}
		}
}






void __fastcall TFormBrush::TimerRotate1Timer(TObject *Sender)
{
	//	bBufCtrl;
	//	bBufShift;
	//	bBufAlt;

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		cursorRotate = Mouse->CursorPos;

		//double x1= cursorBeginRotate.x;   //this should be the center point of canvas in screen coordinates.
		//double y1= cursorBeginRotate.y;
		double x1= maskCenterPoint.x;
		double y1= maskCenterPoint.y;

		double x2= cursorRotate.x;
		double y2= cursorRotate.y;

		double angle_rad=0.0;
		if (x2 - x1 != 0.0)
			{angle_rad = atan2(y2 - y1, x2 - x1);}
		// Normalize the angle to the range [0, 2*pi)
		angle_rad = fmod(angle_rad, 2.0 * M_PI);
		if (angle_rad < 0) {
			// Ensure the angle is positive
			angle_rad += 2.0 * M_PI;
		}

		double diff_rad = angle_rad-initialAngle_rad;
        // Normalize the angle to the range [0, 2*pi)
		diff_rad = fmod(diff_rad, 2.0 * M_PI);
		if (angle_rad < 0) {
			// Ensure the angle is positive
		  	angle_rad += 2.0 * M_PI;
		}


		double diff_deg = diff_rad * 180.0 / M_PI;

		int disp_deg= diff_deg;
		if (disp_deg >= 180) {disp_deg -= 360;}
		double disp_rad= diff_rad;
		if (disp_rad >= 1 * M_PI) {disp_rad -= 2*M_PI;}


		int octant = GetOctant(diff_rad);
		int hextant = GetHextant(diff_rad);
		int seg24	= GetSeg24(diff_rad);
		FormBrush->Caption="Octant: "+IntToStr((int)octant)+", Degrees: "+IntToStr((int)disp_deg)+" Radians: "+FormatFloat("#0.00", disp_rad);

		bBufCtrl=(GetAsyncKeyState(VK_CONTROL) & 0x8000);
		bBufShift=(GetAsyncKeyState(VK_SHIFT) & 0x8000);
		bBufAlt=(GetAsyncKeyState(VK_MENU) & 0x8000);



			int d = iBrushSize[iBrushPresetIndex];
			int cd=32;
			int sp = (32 -d )/2 ;
			switch(d){

			//specialcase a 3x3 brush for better consistency
			case 3:

				for(int x=0;x<d;x++){
					for(int y=0;y<d;y++){
						maskRotateWorkspace[y][x] = maskRotateBuf[y+sp][x+sp];
					}
				}
				Rotate3x3(octant);
				for(int x=0;x<d;x++){
					for(int y=0;y<d;y++){
						(*ptr_tableBrush[iBrushPresetIndex])[x][y] = maskRotateWorkspace[y][x];
					}
				}
				break;

			case 5:

				for(int x=0;x<d;x++){
					for(int y=0;y<d;y++){
						maskRotateWorkspace[y][x] = maskRotateBuf[y+sp][x+sp];
					}
				}
				Rotate5x5(octant,hextant);
				for(int x=0;x<d;x++){
					for(int y=0;y<d;y++){
						(*ptr_tableBrush[iBrushPresetIndex])[x][y] = maskRotateWorkspace[y][x];
					}
				}
				break;

			case 7:
                for(int x=0;x<d;x++){
					for(int y=0;y<d;y++){
						maskRotateWorkspace[y][x] = maskRotateBuf[y+sp][x+sp];
					}
				}
				Rotate7x7(octant,hextant,seg24);
				for(int x=0;x<d;x++){
					for(int y=0;y<d;y++){
						(*ptr_tableBrush[iBrushPresetIndex])[x][y] = maskRotateWorkspace[y][x];
					}
				}
				break;

			default:

			for(int x=0;x<32;x++){
				for(int y=0;y<32;y++){
				maskRotateWorkspace[y][x] = maskRotateBuf[y][x];
				}
			}

			bool bRounding=true;
			float fAngle = fmod(diff_rad,0.5f*M_PI);

			if(octant>1 && octant <6) TurnBitmask_180(maskRotateWorkspace[0], cd, cd);//bFlip180=true;
			if(octant==2 || octant==3 || octant==6 || octant==7) TurnBitmask_90(cd, true);

			if(!bBufShift) //octant shift mode
			{
				shear_bitmask(maskRotateWorkspace[0],-tan(fAngle/2),cd,cd,false,false,bRounding);
				shear_bitmask(maskRotateWorkspace[0],sin(fAngle),cd,cd,false,true,bRounding);
				shear_bitmask(maskRotateWorkspace[0],-tan(fAngle/2),cd,cd,false,false,bRounding);
			}
			//if(bBufCtrl)RetouchBitmask();
			if(!bBufCtrl)SoftenBitmask();

			int startPixel = (32-d)/2 ;
			for(int x=0;x<d;x++){
				for(int y=0;y<d;y++){
				(*ptr_tableBrush[iBrushPresetIndex])[x][y]
				= maskRotateWorkspace[y+startPixel][x+startPixel];
				}
			}
			}
			BrushmaskTimer->Enabled=true;

	}
	else {
		TimerRotate1->Enabled=false;
    	TimerRestoreCaption->Interval=500;
		TimerRestoreCaption->Enabled=true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBrush::Rotate3x3(int steps){
	if(steps==0) return;
	int tmp;
	for (int i = 0; i <steps; i++) {
		tmp=maskRotateWorkspace[0][0];

		maskRotateWorkspace[0][0]=maskRotateWorkspace[1][0];
		maskRotateWorkspace[1][0]=maskRotateWorkspace[2][0];
		maskRotateWorkspace[2][0]=maskRotateWorkspace[2][1];

		maskRotateWorkspace[2][1]=maskRotateWorkspace[2][2];
		maskRotateWorkspace[2][2]=maskRotateWorkspace[1][2];
		maskRotateWorkspace[1][2]=maskRotateWorkspace[0][2];

		maskRotateWorkspace[0][2]=maskRotateWorkspace[0][1];
		maskRotateWorkspace[0][1]=tmp;

	}
}

//---------------------------------------------------------------------------
void __fastcall TFormBrush::Rotate5x5(int inner, int outer){

	int tmp;
	if(inner!=0){
	for (int i = 0; i <inner; i++) {
		tmp=maskRotateWorkspace[1][1];

		maskRotateWorkspace[1][1]=maskRotateWorkspace[2][1];
		maskRotateWorkspace[2][1]=maskRotateWorkspace[3][1];
		maskRotateWorkspace[3][1]=maskRotateWorkspace[3][2];

		maskRotateWorkspace[3][2]=maskRotateWorkspace[3][3];
		maskRotateWorkspace[3][3]=maskRotateWorkspace[2][3];
		maskRotateWorkspace[2][3]=maskRotateWorkspace[1][3];

		maskRotateWorkspace[1][3]=maskRotateWorkspace[1][2];
		maskRotateWorkspace[1][2]=tmp;

	}}
	if(outer!=0){
	for (int i = 0; i <outer; i++) {

		tmp=maskRotateWorkspace[0][0];
		maskRotateWorkspace[0][0]=maskRotateWorkspace[1][0];
		maskRotateWorkspace[1][0]=maskRotateWorkspace[2][0];
		maskRotateWorkspace[2][0]=maskRotateWorkspace[3][0];
		maskRotateWorkspace[3][0]=maskRotateWorkspace[4][0];

		maskRotateWorkspace[4][0]=maskRotateWorkspace[4][1];
		maskRotateWorkspace[4][1]=maskRotateWorkspace[4][2];
		maskRotateWorkspace[4][2]=maskRotateWorkspace[4][3];
		maskRotateWorkspace[4][3]=maskRotateWorkspace[4][4];
		maskRotateWorkspace[4][4]=maskRotateWorkspace[3][4];

		maskRotateWorkspace[3][4]=maskRotateWorkspace[2][4];

		maskRotateWorkspace[2][4]=maskRotateWorkspace[1][4];
		maskRotateWorkspace[1][4]=maskRotateWorkspace[0][4];
		maskRotateWorkspace[0][4]=maskRotateWorkspace[0][3];
		maskRotateWorkspace[0][3]=maskRotateWorkspace[0][2];
		maskRotateWorkspace[0][2]=maskRotateWorkspace[0][1];
		maskRotateWorkspace[0][1]=tmp;

	}}
}

void __fastcall TFormBrush::Rotate7x7(int inner, int mid, int outer){

	int tmp;
	if(inner!=0){
	for (int i = 0; i <inner; i++) {
		tmp=maskRotateWorkspace[2][2];

		maskRotateWorkspace[2][2]=maskRotateWorkspace[3][2];
		maskRotateWorkspace[3][2]=maskRotateWorkspace[4][2];
		maskRotateWorkspace[4][2]=maskRotateWorkspace[4][3];

		maskRotateWorkspace[4][3]=maskRotateWorkspace[4][4];
		maskRotateWorkspace[4][4]=maskRotateWorkspace[3][4];
		maskRotateWorkspace[3][4]=maskRotateWorkspace[2][4];

		maskRotateWorkspace[2][4]=maskRotateWorkspace[2][3];
		maskRotateWorkspace[2][3]=tmp;

	}}
	if(mid!=0){
	for (int i = 0; i <mid; i++) {

		tmp=maskRotateWorkspace[1][1];
		maskRotateWorkspace[1][1]=maskRotateWorkspace[2][1];
		maskRotateWorkspace[2][1]=maskRotateWorkspace[3][1];
		maskRotateWorkspace[3][1]=maskRotateWorkspace[4][1];
		maskRotateWorkspace[4][1]=maskRotateWorkspace[5][1];

		maskRotateWorkspace[5][1]=maskRotateWorkspace[5][2];
		maskRotateWorkspace[5][2]=maskRotateWorkspace[5][3];
		maskRotateWorkspace[5][3]=maskRotateWorkspace[5][4];
		maskRotateWorkspace[5][4]=maskRotateWorkspace[5][5];
		maskRotateWorkspace[5][5]=maskRotateWorkspace[4][5];

		maskRotateWorkspace[4][5]=maskRotateWorkspace[3][5];

		maskRotateWorkspace[3][5]=maskRotateWorkspace[2][5];
		maskRotateWorkspace[2][5]=maskRotateWorkspace[1][5];
		maskRotateWorkspace[1][5]=maskRotateWorkspace[1][4];
		maskRotateWorkspace[1][4]=maskRotateWorkspace[1][3];
		maskRotateWorkspace[1][3]=maskRotateWorkspace[1][2];
		maskRotateWorkspace[1][2]=tmp;

	}}

	if(outer!=0){
	for (int i = 0; i <outer; i++) {

		tmp=maskRotateWorkspace[0][0];
		maskRotateWorkspace[0][0]=maskRotateWorkspace[1][0];
		maskRotateWorkspace[1][0]=maskRotateWorkspace[2][0];
		maskRotateWorkspace[2][0]=maskRotateWorkspace[3][0];
		maskRotateWorkspace[3][0]=maskRotateWorkspace[4][0];
		maskRotateWorkspace[4][0]=maskRotateWorkspace[5][0];
		maskRotateWorkspace[5][0]=maskRotateWorkspace[6][0];
		maskRotateWorkspace[6][0]=maskRotateWorkspace[6][1];

		maskRotateWorkspace[6][1]=maskRotateWorkspace[6][2];
		maskRotateWorkspace[6][2]=maskRotateWorkspace[6][3];
		maskRotateWorkspace[6][3]=maskRotateWorkspace[6][4];
		maskRotateWorkspace[6][4]=maskRotateWorkspace[6][5];
		maskRotateWorkspace[6][5]=maskRotateWorkspace[6][6];
		maskRotateWorkspace[6][6]=maskRotateWorkspace[5][6];

		maskRotateWorkspace[5][6]=maskRotateWorkspace[4][6];
		maskRotateWorkspace[4][6]=maskRotateWorkspace[3][6];
		maskRotateWorkspace[3][6]=maskRotateWorkspace[2][6];
		maskRotateWorkspace[2][6]=maskRotateWorkspace[1][6];
		maskRotateWorkspace[1][6]=maskRotateWorkspace[0][6];
		maskRotateWorkspace[0][6]=maskRotateWorkspace[0][5];

		maskRotateWorkspace[0][5]=maskRotateWorkspace[0][4];
		maskRotateWorkspace[0][4]=maskRotateWorkspace[0][3];
		maskRotateWorkspace[0][3]=maskRotateWorkspace[0][2];
		maskRotateWorkspace[0][2]=maskRotateWorkspace[0][1];

		maskRotateWorkspace[0][1]=tmp;

	}}


}
//---------------------------------------------------------------------------
int __fastcall TFormBrush::GetOctant(double angle_rad) {
	// Normalize the angle to the range [0, 2*pi)
	angle_rad = fmod(angle_rad, 2.0 * M_PI);

	if (angle_rad < 0) {
		// Ensure the angle is positive
		angle_rad += 2.0 * M_PI;
	}

	// Determine the octant
	if (angle_rad < M_PI / 4.0) {
		return 0; // First octant
	} else if (angle_rad < 2.0 * M_PI / 4.0) {
		return 1; // Second octant
	} else if (angle_rad < 3.0 * M_PI / 4.0) {
		return 2; // Third octant
	} else if (angle_rad < M_PI) {
		return 3; // Fourth octant
	} else if (angle_rad < 5.0 * M_PI / 4.0) {
		return 4; // Fifth octant
	} else if (angle_rad < 6.0 * M_PI / 4.0) {
		return 5; // Sixth octant
	} else if (angle_rad < 7.0 * M_PI / 4.0) {
		return 6; // Seventh octant
	} else {
		return 7; // Eighth octant
	}
}

//---------------------------------------------------------------------------

int __fastcall TFormBrush::GetHextant(double angle_rad) {
	// Normalize the angle to the range [0, 2*pi)
	angle_rad = fmod(angle_rad, 2.0 * M_PI);

	if (angle_rad < 0) {
		// Ensure the angle is positive
		angle_rad += 2.0 * M_PI;
	}

	// Determine the hextant
	if (	   angle_rad < 		 M_PI / 8.0) {
		return 0; // First hextant
	} else if (angle_rad < 2.0 * M_PI / 8.0) {
		return 1; // Second hextant
	} else if (angle_rad < 3.0 * M_PI / 8.0) {
		return 2; // Third hextant
	} else if (angle_rad < 4.0 * M_PI / 8.0) {
		return 3; // Fourth hextant
	} else if (angle_rad < 5.0 * M_PI / 8.0) {
		return 4; // Fifth hextant
	} else if (angle_rad < 6.0 * M_PI / 8.0) {
		return 5; // Sixth hextant
	} else if (angle_rad < 7.0 * M_PI / 8.0) {
		return 6; // Seventh hextant
	} else if (angle_rad < M_PI) {
		return 7; // Eighth hextant
	} else if (angle_rad < 9.0 * M_PI / 8.0) {
		return 8; // Ninth hextant
	} else if (angle_rad < 10.0 * M_PI / 8.0) {
		return 9; // Tenth hextant
	} else if (angle_rad < 11.0 * M_PI / 8.0) {
		return 10; // Eleventh hextant
	} else if (angle_rad < 12.0 * M_PI / 8.0) {
		return 11; // Twelfth hextant
	} else if (angle_rad < 13.0 * M_PI / 8.0) {
		return 12; // Thirteenth hextant
	} else if (angle_rad < 14.0 * M_PI / 8.0) {
        return 13; // Fourteenth hextant
    } else if (angle_rad < 15.0 * M_PI / 8.0) {
        return 14; // Fifteenth hextant
	} else {
        return 15; // Sixteenth hextant
	}
}


//---------------------------------------------------------------------------

int __fastcall TFormBrush::GetSeg24(double angle_rad) {
	// Normalize the angle to the range [0, 2*pi)
	angle_rad = fmod(angle_rad, 2.0 * M_PI);

	if (angle_rad < 0) {
		// Ensure the angle is positive
		angle_rad += 2.0 * M_PI;
	}

	// Determine the hextant
	if (	   angle_rad < 		 M_PI / 12.0) {
		return 0; // First segment
	} else if (angle_rad < 2.0 * M_PI / 12.0) {
		return 1; // Second segment
	} else if (angle_rad < 3.0 * M_PI / 12.0) {
		return 2; // Third segment
	} else if (angle_rad < 4.0 * M_PI / 12.0) {
		return 3; // Fourth segment
	} else if (angle_rad < 5.0 * M_PI / 12.0) {
		return 4; // Fifth segment
	} else if (angle_rad < 6.0 * M_PI / 12.0) {
		return 5; // Sixth segment
	} else if (angle_rad < 7.0 * M_PI / 12.0) {
		return 6; // Seventh segment
	} else if (angle_rad < 8.0 * M_PI / 12.0) {
		return 7; // Eighth segment
	} else if (angle_rad < 9.0 * M_PI / 12.0) {
		return 8; // Ninth segment
	} else if (angle_rad < 10.0 * M_PI / 12.0) {
		return 9; // Tenth segment
	} else if (angle_rad < 11.0 * M_PI / 12.0) {
		return 10; // Eleventh segment
	} else if (angle_rad < M_PI ) {
		return 11; // Twelfth segment
	} else if (angle_rad < 13.0 * M_PI / 12.0) {
		return 12; // Thirteenth segment
	} else if (angle_rad < 14.0 * M_PI / 12.0) {
		return 13; // Fourteenth segment
	} else if (angle_rad < 15.0 * M_PI / 12.0) {
		return 14; // Fifteenth segment
	} else if (angle_rad < 16.0 * M_PI / 12.0) {
		return 15; // Sixteenth segment
	} else if (angle_rad < 17.0 * M_PI / 12.0) {
		return 16; // Seventeenth segment
	} else if (angle_rad < 18.0 * M_PI / 12.0) {
		return 17; // Eigteenth segment
	} else if (angle_rad < 19.0 * M_PI / 12.0) {
		return 18; // Nineteenth segment
	} else if (angle_rad < 20.0 * M_PI / 12.0) {
		return 19; // 20th segment
	} else if (angle_rad < 21.0 * M_PI / 12.0) {
		return 20; // 21st segment
	} else if (angle_rad < 22.0 * M_PI / 12.0) {
		return 21; // 22nd segment
	} else if (angle_rad < 23.0 * M_PI / 12.0) {
		return 22; // 23rd segment
	} else {
		return 23; // 24th segment
	}
}


//---------------------------------------------------------------------------
void __fastcall TFormBrush::TurnBitmask_90(size_t d, bool dir){

	int** arr = new int*[d];
	for (size_t i = 0; i < d; ++i) arr[i] = new int[d];

	if(dir){
		for(size_t y=0; y<d; y++){
			for(size_t x=0; x<d; x++){
				arr[d-y-1][x]=maskRotateWorkspace[x][y];

			}
		}
	}
	else {
		for(size_t y=0; y<d; y++){
			for(size_t x=0; x<d; x++){
				arr[y][d-y-x]=maskRotateWorkspace[x][y];
			}
		}
	}

	for(size_t y=0; y<d; y++){
		for(size_t x=0; x<d; x++){
			maskRotateWorkspace[x][y]=arr[x][y];
		}
	}
	BrushmaskTimer->Enabled=true;

	for (size_t i = 0; i < d; ++i) {
		delete[] arr[i];
	}
	delete[] arr;


}
//---------------------------------------------------------------------------
void __fastcall TFormBrush::TurnBufmask_90(size_t d, bool dir){

	int** arr = new int*[d];
	for (size_t i = 0; i < d; ++i) arr[i] = new int[d];

	if(dir){
		for(size_t y=0; y<d; y++){
			for(size_t x=0; x<d; x++){
				arr[d-y-1][x]=CompareBufBitmask[x][y];

			}
		}
	}
	else {
		for(size_t y=0; y<d; y++){
			for(size_t x=0; x<d; x++){
				arr[y][d-y-x]=CompareBufBitmask[x][y];
			}
		}
	}

	for(size_t y=0; y<d; y++){
		for(size_t x=0; x<d; x++){
			CompareBufBitmask[x][y]=arr[x][y];
		}
	}
	BrushmaskTimer->Enabled=true;

	for (size_t i = 0; i < d; ++i) {
		delete[] arr[i];
	}
	delete[] arr;


}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::TurnBitmask_45(int cd, bool dir) {
	if (cd>32) return;
	int c=cd/2;
	int temp[32][32];

	// Copy the original matrix to a temporary matrix
	for (int i = 0; i < cd; i++) {
		for (int j = 0; j < cd; j++) {
			temp[i][j] = maskRotateWorkspace[i][j];
		}
	}
	if(!dir){
	 // Rotate the matrix 45 degrees counter-clockwise
	for (int i = 0; i < cd; i++) {
		for (int j = 0; j < cd; j++) {
			int new_i = c - 1 - j;
			int new_j = i + j ;

			// Check if the new indices are within bounds
			if (new_i >= 0 && new_i < cd && new_j >= 0 && new_j < cd) {
				maskRotateWorkspace[new_i][new_j] = temp[i][j];
			}
		}
	}
	}else{
	// Rotate the matrix 45 degrees clockwise
	for (int i=0; i<cd; i++) {
		for (int j=0; j<cd; j++) {
			int new_i = i+j ;
			int new_j = c-1-(i-j);

			// Check if the new indices are within bounds
			if (new_i >= 0 && new_i < cd && new_j >= 0 && new_j < cd) {
				maskRotateWorkspace[new_i][new_j] = temp[i][j];
			}
		}
	}}
}
//---------------------------------------------------------------------------
void __fastcall TFormBrush::TurnBitmask_180(char* data, int w, int h) {
	if (data == NULL || w <= 0 || h <= 0) return;
	for (int i=0; i<h/2; i++) {
		for (int j=0; j<w; j++) {
			char tmp 				= data[i*w+j];
			data[i*w+j] 			= data[(h-1-i)*w+(w-1-j)];
			data[(h-1-i)*w+(w-1-j)] = tmp;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormBrush::RetouchBitmask(void) {
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
            // Check if the elements are different
			if (CompareBufBitmask[i][j] != maskRotateWorkspace[i][j]) {
                int count = 0;

				// Check 8 surrounding elements
                for (int x = -1; x <= 1; ++x) {
                    for (int y = -1; y <= 1; ++y) {
                        int ni = i + x;
                        int nj = j + y;

                        // Check if indices are within bounds
						if (ni >= 0 && ni < 32 && nj >= 0 && nj < 32) {
                            // Check if the surrounding element has the same value
							if (maskRotateWorkspace[ni][nj] == maskRotateWorkspace[i][j]) {
                                ++count;
                            }
                        }
                    }
				}

                // Check if criteria are met
				if (count == 3 || count == 4) {
					// Replace the element in matrix2 with the corresponding element from matrix1
					maskRotateWorkspace[i][j] = CompareBufBitmask[i][j];
                }
			}
        }
	}
}

void __fastcall TFormBrush::SoftenBitmask(void) {

	char arr[32][32];
    for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
		  arr[i][j]=0;
		}
	}

	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			// Check if the elements are different
			int count = 0;

				// Check 8 surrounding elements
				for (int x = -1; x <= 1; ++x) {
					for (int y = -1; y <= 1; ++y) {
						int ni = i + x;
						int nj = j + y;

						// Check if indices are within bounds
						if (ni >= 0 && ni < 32 && nj >= 0 && nj < 32 ) {    //&& (nj!=j && ni!=i)
							// Check if the surrounding element has the same value
							if (maskRotateWorkspace[ni][nj] != maskRotateWorkspace[i][j]) {
								//++count;
								if(nj==j) ++count; //extra weight
							}
						}
					}
				}

			// Check if criteria are met
			if(count==2) arr[i][j]=1;

		}
	}
    for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			// Check if the elements are different
			int count = 0;

				// Check 8 surrounding elements
				for (int x = -1; x <= 1; ++x) {
					for (int y = -1; y <= 1; ++y) {
						int ni = i + x;
						int nj = j + y;

						// Check if indices are within bounds
						if (ni >= 0 && ni < 32 && nj >= 0 && nj < 32 ) {    //&& (nj!=j && ni!=i)
							// Check if the surrounding element has the same value
							if (maskRotateWorkspace[ni][nj] != maskRotateWorkspace[i][j]) {
								//++count;
								if(ni==i) ++count; //extra weight
							}
						}
					}
				}

			// Check if criteria are met
			if(count==2) arr[i][j]=1;

		}
	}
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			if(arr[i][j]==1){
				if(maskRotateWorkspace[i][j]==1) maskRotateWorkspace[i][j] = 0;
				if(maskRotateWorkspace[i][j]==0) maskRotateWorkspace[i][j] = 1;
			}
		}
	}

		 /*
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			// Check if the elements are different
			int count = 0;

				// Check 8 surrounding elements
				for (int x = -1; x <= 1; ++x) {
					for (int y = -1; y <= 1; ++y) {
						int ni = i + x;
						int nj = j + y;

						// Check if indices are within bounds
						if (ni >= 0 && ni < 32 && nj >= 0 && nj < 32 ) {    //&& (nj!=j && ni!=i)
							// Check if the surrounding element has the same value
							if (maskRotateWorkspace[ni][nj] != maskRotateWorkspace[i][j]) {
								//++count;
								if(ni==i || nj==j) ++count; //extra weight
							}
						}
					}
				}

			// Check if criteria are met
			if (count >= 3) {
			// Replace the element in matrix2 with the corresponding element from matrix1
			if(maskRotateWorkspace[i][j]==1) maskRotateWorkspace[i][j] = 0;
			if(maskRotateWorkspace[i][j]==0) maskRotateWorkspace[i][j] = 1;
			}
		}
	}
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			// Check if the elements are different
			int count = 0;

				// Check 8 surrounding elements
				for (int x = -1; x <= 1; ++x) {
					for (int y = -1; y <= 1; ++y) {
						int ni = i + x;
						int nj = j + y;

						// Check if indices are within bounds
						if (ni >= 0 && ni < 32 && nj >= 0 && nj < 32 ) {    //&& (nj!=j && ni!=i)
							// Check if the surrounding element has the same value
							if (maskRotateWorkspace[ni][nj] != maskRotateWorkspace[i][j]) {
								//++count;
								if(ni==i || nj==j) ++count; //extra weight
							}
						}
					}
				}

			// Check if criteria are met
			if (count >= 3) {
			// Replace the element in matrix2 with the corresponding element from matrix1
			if(maskRotateWorkspace[i][j]==1) maskRotateWorkspace[i][j] = 0;
			if(maskRotateWorkspace[i][j]==0) maskRotateWorkspace[i][j] = 1;
			}
		}
	}    */
}
void __fastcall TFormBrush::FillBrushMask(int x,int y)
{
	int s = iBrushSize[iBrushPresetIndex];

	bool col;
	unsigned char border=255;
	unsigned char fieldtest=254;
	unsigned char drawfield=253;


	int temp[32][32];

	for(int i=0;i<s+2;i++){
		for(int j=0;j<s+2;j++){

				temp[i][j]=border;
		}
	}
	for(int i=1;i<s+1;i++){
		for(int j=1;j<s+1;j++){
				temp[i][j]=(*ptr_tableBrush[iBrushPresetIndex])[i-1][j-1];
		}
	}

	int py=y+1;
	int px=x+1;
	col=temp[px][py];

	temp[px][py]=fieldtest; //seed a test on the clicked coordinate
	int cnt;
	
	while(true){
		cnt=0;

		for(int i=1;i<s+1;i++)
		{
			for(int j=1;j<s+1;j++)
			{
				if(temp[i][j]==fieldtest)
				{
					if(temp[i-1][j]==col) temp[i-1][j]=fieldtest;
					if(temp[i+1][j]==col) temp[i+1][j]=fieldtest;
					if(temp[i][j-1]==col) temp[i][j-1]=fieldtest;
					if(temp[i][j+1]==col) temp[i][j+1]=fieldtest;

					temp[i][j]=drawfield;
					cnt++;

				}
			}
		}

	if(!cnt) break;
	}
	col ^= true;
	for(int i=1;i<s+1;i++)
		{
			for(int j=1;j<s+1;j++)
			{
				if(temp[i][j]==drawfield) (*ptr_tableBrush[iBrushPresetIndex])[i-1][j-1]=col;
			}
		}
   bForbidMaskPen=true;
   Draw();
}
void __fastcall TFormBrush::PaintBoxMaskMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bForbidMaskPen=false;
	//TimerRestoreCaption->Interval=100;
	//TimerRestoreCaption->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::PaintBoxMaskMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Brush mask canvas.\n[Click] to toggle a mask pixel.\n[Ctrl+click] to fill-toggle a field of mask pixels.\n[Right-click, hold and drag] to rotate mask. (expect and take advantage of the distortion).";	
}
//---------------------------------------------------------------------------

void __fastcall TFormBrush::TimerRestoreCaptionTimer(TObject *Sender)
{
   FormBrush->Caption="Brush mask toolbox";
   TimerRestoreCaption->Enabled=false;
}
//---------------------------------------------------------------------------

