//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitCHREditor.h"
#include "UnitBrush.h"
#include "UnitLineDetails.h"
#include "UnitBucketToolbox.h"
//#include "AntiJagMasks.h"
#include "math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCHREditor *FormCHREditor;


 Graphics::TBitmap *buffer;



bool bIsFilling=false;


extern bool bCustomBucketLoadSuccess;
const int const_crCustomNormalBucket=crSizeAll+1;
const int const_crCustomPreciseCursor=crSizeAll+2;
const int const_crCustomPreciseCursorSel=crSizeAll+3;

extern bool openByFileDone;
extern bankViewTable[];

extern int (*ptr_tableBrush[14])[16][16];
extern int iBrushPresetIndex;
extern int iBrushCursorAlignment[];


extern size_t iBrushSize[];
extern size_t iBrushSnapSize_x[];
extern size_t iBrushSnapSize_y[];


extern float brush_x_anchor;
extern float brush_y_anchor;

extern bool bLinePreset_modeQuick[];
extern bool bLinePreset_modeCoat[];

extern int uiScale;
extern bool	bBufCtrl;
extern bool	bBufShift;
extern bool	bBufAlt;
extern bool bSmudge;
extern int iSmudge;
extern bool bBufVK_3;
extern int globalLineSenderMode;
extern TRect chrSelection;
extern bool holdStats;
extern unsigned char chrSelected[];
extern bool cueCHRdrawAll;
extern bool bFillFirstIteration;
extern void Bresenham_line(int x0, int y0, int x1, int y1, int w, unsigned char *array, bool bIsRightAngle);

extern void plotFineQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array);
extern void Bezier_curve(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array, int midpoint_ix,int midpoint_iy);
extern void plotQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array, int midpoint_ix,int midpoint_iy);
extern void Bresenham_angle(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array, bool knee, bool join, int midpoint_ix,int midpoint_iy);
extern void Bresenham_2line(float x0, float y0, float x1, float y1, float x2, float y2, int pw, unsigned char* ptr, int midpoint_ix,int midpoint_iy);
extern void Hyperbola_curve(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array, int midpoint_ix, int midpoint_iy, int type);
extern void Bresenham_rect(int x0, int y0, int x2, int y2, int w, unsigned char *array);
extern void Line_Circle(int x0, int y0, int x2, int y2, int w, unsigned char *array);

extern int iLinePresetIndex;
extern int iLinePreset_rosterIndex[];


int CHR_scrollX;
int	CHR_scrollY;

int CHR_mmX;
int CHR_mmY;
int CHR_moveOriginX;
int	CHR_moveOriginY;
int lineOffX=0; //x0y0 adjust nudge offset
int lineOffY=0;
int mvOffX=0;   //move offset
int mvOffY=0;
int mvOriginX=0;
int mvOriginY=0;

int movx2,movy2;

bool bForbidPaint=false;
extern bool isMovingLine;
extern bool isNudgingLine;
extern bool bOldLine;
extern int outPalette[];
extern unsigned char bgPal[4*4*4];
extern int bgPalCur;
extern unsigned char *chr;
extern unsigned char chrBuf[];
extern unsigned char chrBufFill[];
extern unsigned char chrQuickLine[];
extern unsigned char chrBufLine[];
extern unsigned char tileViewTable[];
extern unsigned char sprModeTableReverse[];
unsigned char arr_linePlotCHR[128*128];
unsigned char arr_linePlotCHR2[128*128];

extern lineToolRoster;
extern int lineToolAlt;
extern int lineToolX;
extern int lineToolY;
extern int lineToolY_toggleperm;
extern bool lineRosterEnable[];
extern int palActive;
extern int penActive;
extern int tileActive;
extern int bankActive;
extern int palBank;

extern int chrSelectRect;
extern int tileSelRectWdt;
extern int tileSelRectHgt;


extern int iGlobalAlpha;
extern int tmpContinousIncDecTimer;
extern int continousIncDecDuration;
extern int fillPal;
int prevPal;
extern int blendMode;
extern RECT curMainWinPos;
extern bool prefStartShowCHR;
extern int CHREditorHover;
extern bool cueStats;

extern bool cueCHRdraw;
extern bool cueUpdateNametable;
extern bool cueUpdateTiles;
extern bool cueUpdateMetasprite;

extern int	cntCHRstrip;

extern bool bBrushMask[4];

const int originalFormHgt		=388;
const int originalFormWdt		=286;
const int originalPaintBox		=272;
const int originalGroupBoxWdt	=281;
const int originalGroupBoxHgt	=289;
extern bool bSnapToScreen;

extern int lineDownX;
extern int lineDownY;
extern int lineUpX;
extern int lineUpY;

extern bool lineDrawing;
extern int lineDashLen;
extern char perfect_pixel_mask_subtle[16][16];
extern char perfect_pixel_mask_lite[16][16];
extern char perfect_pixel_mask_lite[16][16];
extern char perfect_pixel_mask_mlite[16][16];
extern char perfect_pixel_mask_medium[16][16];
extern char perfect_pixel_mask_heavy[16][16];
extern char perfect_pixel_mask_qheavy[16][16];
extern char perfect_pixel_mask_diagonal1[16][16];


extern char *ptr_pixelperfectmask;

void __fastcall TFormCHREditor::DrawCHR(int xs,int ys,int tile)
{
	int i,j,x,y,pp,col;
	TRect r;
	int set=bankActive/16;
	y=ys;
	int grid= FormMain->CHRpixelgrid1->Checked?15:16;
	int r1,r2,g1,g2,b1,b2;
	int rAvg,gAvg,bAvg;

    //these are used to soften the pixel grid in case the two other grid types are shown.
	float temper=0;
	if(FormMain->ShowmidgridinCHReditor1->Checked) temper+= 0.4;
	if(FormMain->ShowCHREditortilegrid1) temper+= 0.7;
	if (temper>=1) temper *= 0.8;

	TColor colorIn1;
	TColor colorIn2;

	pp=tileViewTable[tile]*16+bankViewTable[set+tile];

	if(tile>=0&&tile<256)
	{
		for(i=0;i<8;i++)
		{
			x=xs;

			for(j=0;j<8;j++)
			{
				col=(((chr[pp+i]<<j)&128)>>7)|(((chr[pp+i+8]<<j)&128)>>6);
				colorIn1 = TColor(outPalette[bgPal[palBank*16+palActive*4+col]]);
				colorIn2 = GroupBox1->Color;
				//Graphics::GetRGBValues(colorIn1, r1, g1, b1);
				//Graphics::GetRGBValues(colorIn2, r2, g2, b2);
				r1 = GetRValue(colorIn1);
				g1 = GetGValue(colorIn1);
				b1 = GetBValue(colorIn1);
				r2 = GetRValue(colorIn2);
				g2 = GetGValue(colorIn2);
				b2 = GetBValue(colorIn2);


				int rAvg = (r1*(3+temper) + r2*(2-temper)) / 6;
				int gAvg = (g1*(3+temper) + g2*(2-temper)) / 6;
				int bAvg = (b1*(3+temper) + b2*(2-temper)) / 6;

				//PaintBoxChr->Canvas->Brush->Color=TColor(RGB(rAvg, gAvg, bAvg));
				buffer->Canvas->Brush->Color=TColor(RGB(rAvg, gAvg, bAvg));

				r.left=x;
				r.top=y;
				r.right=x+16;
				r.Bottom=y+16;

				//PaintBoxChr->Canvas->FillRect(r);
				buffer->Canvas->FillRect(r);

				//PaintBoxChr->Canvas->Brush->Color=TColor(outPalette[bgPal[palBank*16+palActive*4+col]]);
				buffer->Canvas->Brush->Color=TColor(outPalette[bgPal[palBank*16+palActive*4+col]]);

				r.left=x;
				r.top=y;
				r.right=x+grid;
				r.Bottom=y+grid;

				//PaintBoxChr->Canvas->FillRect(r);
				buffer->Canvas->FillRect(r);
				x+=16;
			}

			y+=16;
		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			x=xs;
			for(j=0;j<8;j++)
			{
				//PaintBoxChr->Canvas->Brush->Color=GroupBox1->Color;
				buffer->Canvas->Brush->Color=GroupBox1->Color;

				r.left=x;
				r.top=y;
				r.right=x+grid;
				r.Bottom=y+grid;
				//PaintBoxChr->Canvas->FillRect(r);
				buffer->Canvas->FillRect(r);
				x+=16;
			}
			y+=16;
		}
	}
}



void __fastcall TFormCHREditor::Draw(bool drawAll)
{
	int tx,ty;
	bool a=drawAll;
	bool b=btn2x2mode->Down;
	bool tg=FormMain->ShowCHREditortilegrid1->Checked;
	bool mn=FormMain->ShowmidgridinCHReditor1->Checked;
	int n=b?0:8;
	int f=1;
	if(N2x21->Checked) f=1;
	if(N3x31->Checked) f=2;
	if(N4x41->Checked) f=3;
	tx=tileActive&15;
	ty=tileActive/16;
	if(a){

	//draw bg
	buffer->Canvas->Brush->Color = GroupBox1->Color;
	buffer->Canvas->FillRect(Rect(0, 0, buffer->Width, buffer->Height));
	}

	//the checks lessen the burden of drawing these at high rates.

	//pre-row
	if(a){
						DrawCHR(-64        			,-64        ,ty>0&&tx>0  ?tileActive-17:-1); //NW
						DrawCHR( 64+8      			,-64        ,ty>0        ?tileActive-16:-1); //N
						DrawCHR( 64+8+128+n			,-64        ,ty>0&&tx<15 ?tileActive-15:-1);
	if(b && f==1) 		DrawCHR( 64+8+128+n+128+8	,-64        ,ty>0&&tx<14 ?tileActive-14:-1);

	else if(b && f==2){ DrawCHR( 64+8+128+n+128+0	,-64        ,ty>0&&tx<14 ?tileActive-14:-1);
						DrawCHR( 64+8+256+n+128+8	,-64        ,ty>0&&tx<13 ?tileActive-13:-1);
					  }
	else if(b && f==3){ DrawCHR( 64+8+128+n+128+0	,-64        ,ty>0&&tx<14 ?tileActive-14:-1);
						DrawCHR( 64+8+256+n+128+0	,-64        ,ty>0&&tx<13 ?tileActive-13:-1);
						DrawCHR( 64+8+384+n+128+8	,-64        ,ty>0&&tx<12 ?tileActive-12:-1);
					  }

	}

	//1st row from tileactive
	if(a)		DrawCHR(-64        			, 64+8      ,tx>0        ?tileActive-1 :-1);
				DrawCHR( 64+8      			, 64+8      ,tileActive);

	if(b || a)	DrawCHR( 64+8+128+n				   , 64+8 ,tx<15       ?tileActive+1 :-1);
	if(b && (f==2 || f==3))  DrawCHR( 64+8+256+n   , 64+8 ,tx<14       ?tileActive+2 :-1);
	if(b && (f==3)) DrawCHR( 64+8+384+n			   , 64+8 ,tx<13       ?tileActive+3 :-1);

	if(b && a)  DrawCHR( 64+8+128*f+n+128+8	, 64+8      ,tx<15-f       ?tileActive+1+f :-1);


	//2nd
	if(b || a)		DrawCHR(-64        			, 64+8+128+n,ty<15&&tx>0 ?tileActive+15:-1);
	if(b || a)		DrawCHR( 64+8      			, 64+8+128+n,ty<15       ?tileActive+16:-1);
	if(b || a)		DrawCHR( 64+8+128+n			, 64+8+128+n,ty<15&&tx<15?tileActive+17:-1);

	if(b && (f==2 || f==3))  DrawCHR( 64+8+256+n   , 64+8+128 ,tx<14       ?tileActive+18 :-1);
	if(b && (f==3))  DrawCHR( 64+8+384+n   , 64+8+128 ,tx<13       ?tileActive+19 :-1);

	if(b)   		DrawCHR( 64+8+128*f+n+128+8	, 64+8+128+n,ty<15&&tx<15-f?tileActive+17+f:-1);

	//3rd
	if(f==2 || f==3){

		if(b )		DrawCHR(-64        			, 64+8+256+n,ty<15&&tx>0 ?tileActive+31:-1);
		if(b )		DrawCHR( 64+8      			, 64+8+256+n,ty<15       ?tileActive+32:-1);
		if(b )		DrawCHR( 64+8+128+n			, 64+8+256+n,ty<15&&tx<15?tileActive+33:-1);

		if(b && (f==2 || f==3))  DrawCHR( 64+8+256+n   , 64+8+256 ,tx<14       ?tileActive+34 :-1);
		if(b && (f==3))  		 DrawCHR( 64+8+384+n   , 64+8+256 ,tx<13       ?tileActive+35 :-1);

	if(b)   		DrawCHR( 64+8+128*f+n+128+8	, 64+8+256+n,ty<15&&tx<15-f?tileActive+33+f:-1);



	}
	//4th
	if(f==3){

		if(b )		DrawCHR(-64        			, 64+8+384+n,ty<15&&tx>0 ?tileActive+47:-1);
		if(b )		DrawCHR( 64+8      			, 64+8+384+n,ty<15       ?tileActive+48:-1);
		if(b )		DrawCHR( 64+8+128+n			, 64+8+384+n,ty<15&&tx<15?tileActive+49:-1);

		if(b && (f==2 || f==3))  DrawCHR( 64+8+256+n   , 64+8+384 ,tx<14       ?tileActive+50 :-1);   //f==2 is redundant here.
		if(b && (f==3))  		 DrawCHR( 64+8+384+n   , 64+8+384 ,tx<13       ?tileActive+51 :-1);

	if(b)   		DrawCHR( 64+8+128*f+n+128+8	, 64+8+384+n,ty<15&&tx<15-f?tileActive+49+f:-1);



	}
	//last row
	if(b && a)
	{
			DrawCHR(-64        			, 64+8+128*f+n+128+8,ty<14&&tx>0 ?tileActive+15+f*16:-1);
			DrawCHR( 64+8      			, 64+8+128*f+n+128+8,ty<14       ?tileActive+16+f*16:-1);
			DrawCHR( 64+8+128+n			, 64+8+128*f+n+128+8,ty<14&&tx<15?tileActive+17+f*16:-1);
			if(f==1) 			DrawCHR( 64+8+128+n+128+8	, 64+8+128*f+n+128+8		,ty<14&&tx<14? tileActive+18+f*16:-1);
			else if(f==2){ 		DrawCHR( 64+8+128+n+128+0	, 64+8+128*f+n+128+8        ,ty<14&&tx<14 ?tileActive+18+f*16:-1);
								DrawCHR( 64+8+256+n+128+8	, 64+8+128*f+n+128+8        ,ty<13&&tx<13 ?tileActive+19+f*16:-1);
					  }
			else if(f==3){ 		DrawCHR( 64+8+128+n+128+0	, 64+8+128*f+n+128+8        ,ty<14&&tx<14 ?tileActive+18+f*16:-1);
								DrawCHR( 64+8+256+n+128+0	, 64+8+128*f+n+128+8        ,ty<13&&tx<13 ?tileActive+19+f*16:-1);
								DrawCHR( 64+8+384+n+128+8	, 64+8+128*f+n+128+8        ,ty<12&&tx<12 ?tileActive+20+f*16:-1);
					  }


	}

	if(mn){
		int d=b? 1+f:1;

		for(int y=0;y<d;y++){
			for(int x=0;x<d;x++){
				buffer->Canvas->Pen->Color = clGray;
				buffer->Canvas->Pen->Width = 1;
				buffer->Canvas->Pen->Style = psDash;
				buffer->Canvas->MoveTo(x*128+127+8, y*128+127+7);
				buffer->Canvas->LineTo(x*128+127+8, y*128+127+10);
				buffer->Canvas->MoveTo(x*128+127+7, y*128+127+8);
				buffer->Canvas->LineTo(x*128+127+10, y*128+127+8);
			}
		}
	}
	if(b){
		if(tg){
			buffer->Canvas->Pen->Color = TColor(RGB(80, 80, 80)); ;
			buffer->Canvas->Pen->Width = 1;
			buffer->Canvas->Pen->Style = psDot;

			buffer->Canvas->MoveTo(64+8, 			64+127+8);
			buffer->Canvas->LineTo(64+128*f+127+8, 	64+127+8);

			buffer->Canvas->MoveTo(64+127+8, 		64+8);
			buffer->Canvas->LineTo(64+127+8, 		64+128*f+127+8);

			if(f==2 || f==3){
				buffer->Canvas->MoveTo(64+8, 			64+127+128+8);
				buffer->Canvas->LineTo(64+128*f+127+8, 	64+127+128+8);

				buffer->Canvas->MoveTo(64+127+128+8, 	64+8);
				buffer->Canvas->LineTo(64+127+128+8, 	64+128*f+127+8);
			}
			if(f==3){
				buffer->Canvas->MoveTo(64+8, 			64+127+256+8);
				buffer->Canvas->LineTo(64+128*f+127+8, 	64+127+256+8);

				buffer->Canvas->MoveTo(64+127+256+8, 	64+8);
				buffer->Canvas->LineTo(64+127+256+8, 	64+128*f+127+8);
			}

		}
		if(mn){
			buffer->Canvas->Pen->Color = clGray;
			buffer->Canvas->Pen->Width = 1;
			buffer->Canvas->Pen->Style = psDash;

			//midpoint crosshairs
			buffer->Canvas->MoveTo(64+127+8, 64+127+6);
			buffer->Canvas->LineTo(64+127+8, 64+127+11);
			buffer->Canvas->MoveTo(64+127+6, 64+127+8);
			buffer->Canvas->LineTo(64+127+11, 64+127+8);

			if(f==2 || f==3){

				buffer->Canvas->MoveTo(64+127+128+8, 64+127+6);
				buffer->Canvas->LineTo(64+127+128+8, 64+127+11);
				buffer->Canvas->MoveTo(64+127+128+6, 64+127+8);
				buffer->Canvas->LineTo(64+127+128+11, 64+127+8);

				buffer->Canvas->MoveTo(64+127+8, 64+127+128+6);
				buffer->Canvas->LineTo(64+127+8, 64+127+128+11);
				buffer->Canvas->MoveTo(64+127+6, 64+127+128+8);
				buffer->Canvas->LineTo(64+127+11, 64+127+128+8);

				buffer->Canvas->MoveTo(64+127+128+8, 64+127+128+6);
				buffer->Canvas->LineTo(64+127+128+8, 64+127+128+11);
				buffer->Canvas->MoveTo(64+127+128+6, 64+127+128+8);
				buffer->Canvas->LineTo(64+127+128+11, 64+127+128+8);
			}
			if(f==3){
				buffer->Canvas->MoveTo(64+127+256+8, 64+127+6);
				buffer->Canvas->LineTo(64+127+256+8, 64+127+11);
				buffer->Canvas->MoveTo(64+127+256+6, 64+127+8);
				buffer->Canvas->LineTo(64+127+256+11, 64+127+8);

				buffer->Canvas->MoveTo(64+127+8, 64+127+256+6);
				buffer->Canvas->LineTo(64+127+8, 64+127+256+11);
				buffer->Canvas->MoveTo(64+127+6, 64+127+256+8);
				buffer->Canvas->LineTo(64+127+11, 64+127+256+8);

				buffer->Canvas->MoveTo(64+127+256+8, 64+127+128+6);
				buffer->Canvas->LineTo(64+127+256+8, 64+127+128+11);
				buffer->Canvas->MoveTo(64+127+256+6, 64+127+128+8);
				buffer->Canvas->LineTo(64+127+256+11, 64+127+128+8);

				buffer->Canvas->MoveTo(64+127+128+8, 64+127+256+6);
				buffer->Canvas->LineTo(64+127+128+8, 64+127+256+11);
				buffer->Canvas->MoveTo(64+127+128+6, 64+127+256+8);
				buffer->Canvas->LineTo(64+127+128+11, 64+127+256+8);

				buffer->Canvas->MoveTo(64+127+256+8, 64+127+256+6);
				buffer->Canvas->LineTo(64+127+256+8, 64+127+256+11);
				buffer->Canvas->MoveTo(64+127+256+6, 64+127+256+8);
				buffer->Canvas->LineTo(64+127+256+11, 64+127+256+8);


			}

			//edge crosshairs
			buffer->Canvas->MoveTo(64+8, 64+127+8);
			buffer->Canvas->LineTo(64+12, 64+127+8);

			buffer->Canvas->MoveTo(64+127+8, 64+8);
			buffer->Canvas->LineTo(64+127+8, 64+12);

			buffer->Canvas->MoveTo(64+128*f+127+8, 64+127+8);
			buffer->Canvas->LineTo(64+128*f+127+4, 64+127+8);

			buffer->Canvas->MoveTo(64+127+8, 64+128*f+127+8);
			buffer->Canvas->LineTo(64+127+8, 64+128*f+127+4);

			if(f==2 || f==3){
				buffer->Canvas->MoveTo(64+8, 64+127+128+8);
				buffer->Canvas->LineTo(64+12, 64+127+128+8);

				buffer->Canvas->MoveTo(64+127+128+8, 64+8);
				buffer->Canvas->LineTo(64+127+128+8, 64+12);


				buffer->Canvas->MoveTo(64+128*f+127+8, 64+127+128+8);
				buffer->Canvas->LineTo(64+128*f+127+4, 64+127+128+8);

				buffer->Canvas->MoveTo(64+127+128+8, 64+128*f+127+8);
				buffer->Canvas->LineTo(64+127+128+8, 64+128*f+127+4);
			}


			if(f==3){
				buffer->Canvas->MoveTo(64+8, 64+127+256+8);
				buffer->Canvas->LineTo(64+12, 64+127+256+8);

				buffer->Canvas->MoveTo(64+127+256+8, 64+8);
				buffer->Canvas->LineTo(64+127+256+8, 64+12);


				buffer->Canvas->MoveTo(64+128*f+127+8, 64+127+256+8);
				buffer->Canvas->LineTo(64+128*f+127+4, 64+127+256+8);

				buffer->Canvas->MoveTo(64+127+256+8, 64+128*f+127+8);
				buffer->Canvas->LineTo(64+127+256+8, 64+128*f+127+4);
			}
		}
	}
	// Copy the off-screen buffer to the canvas
	PaintBoxChr->Canvas->Draw(0, 0, buffer);
}



void __fastcall TFormCHREditor::ScrollLeft(void)
{
	/*int i,pp;
	int fOff=0;
	int fLen=16;
	char c[16];   //carry

	if (!ButtonBitmaskLo->Down&&FormMain->Applytonudge1->Checked) {fLen-=8; fOff=-8;}
	if (!ButtonBitmaskHi->Down&&FormMain->Applytonudge1->Checked) {fLen-=8;}

	if(fLen==0)return;

	FormMain->SetUndo();

	pp=tileActive*16+bankActive+fOff;

	for(i=0;i<fLen;i++)
	{
		if(ButtonNudgeInto->Down)
		{
			c[i]=chr[pp];
			chr[pp]=(chr[pp]<<1)|((chr[pp+16]>>7)&1);
			chr[pp+16]=(chr[pp+16]<<1)|((c[i]>>7)&1);
		}
		if(!ButtonNudgeInto->Down) chr[pp]=(chr[pp]<<1)|((chr[pp]>>7)&1);

		pp++;
	}

	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);
	FormMain->UpdateMetaSprite();*/
}



void __fastcall TFormCHREditor::ScrollHorz(bool isLeft)
{
	int i,j,k,l,pp;
	int tpp,tpp2;
	int ta=tileActive;
	int ba=bankActive;
	int set=ba/16;
	int dir=1;
	int fOff=0;
	int fLen=16;  // first 8 bytes low bit plane, next 8 bytes high bitplane
	char c[16];   //carry

	int xTiles=tileSelRectWdt;  //number of tiles to process
	int yTiles=tileSelRectHgt;

	if (!ButtonBitmaskHi->Down&&FormMain->Applytonudge1->Checked) {fLen-=8;}
	if (!ButtonBitmaskLo->Down&&FormMain->Applytonudge1->Checked) {fLen-=8; fOff=+8;}

	if(fLen==0||tileSelRectWdt==0||tileSelRectHgt==0)return;
	//^ tileSelRectHgt or Wdt can be 0 if the selection was negative

	FormMain->SetUndo();

	if(isLeft) dir=8*xTiles-1; //right: 1, left: width-1 iterations
	for(l=0;l<dir;l++)
	{
		for (k=0;k<yTiles;k++)
		{

			// prep carry
			pp = tileViewTable[ta + xTiles-1 +k*16]*16  + bankViewTable[set + ta + xTiles-1 +k*16];
			for(i=fOff;i<fLen+fOff;i++)	c[i]=chr[pp+i];


			// shift a row
			for (j=0;j<xTiles; j++)
			{
			   tpp = tileViewTable[ta  + xTiles-1-j +k*16]*16  + bankViewTable[set + ta  + xTiles-1-j +k*16];
			   tpp2 = tileViewTable[ta -1 + xTiles-1-j + k*16]*16  + bankViewTable[set + ta -1 + xTiles-1-j + k*16];

				for(i=fOff;i<fLen+fOff;i++) chr[tpp+i]=chr[tpp+i]>>1 | ((chr[tpp2+i]<<7)&128);
			}

			// merge carry with first tile
			pp = tileViewTable[ta +k*16]*16  + bankViewTable[set + ta +k*16];
			for(i=fOff;i<fLen+fOff;i++) chr[pp+i]=((chr[pp+i])&127)|((c[i]<<7)&128);
		}
	}
	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);
	FormMain->UpdateMetaSprite(false);
}



void __fastcall TFormCHREditor::ScrollVert(bool isDown)
{
	int i,j,k,l,pp,pp2,ptmp,ptmp2,t1,t2;
	int dir=1;
	int ta=tileActive;
	int ba=bankActive;
	int set=ba/16;
	int xTiles=tileSelRectWdt;
	int yTiles=tileSelRectHgt;
	//int xb=(xTiles-1)*16;

	if (!(ButtonBitmaskLo->Down||ButtonBitmaskHi->Down))
		if(FormMain->Applytonudge1->Checked)  return;
	//if (ButtonNudgeInto->Down)tiles=2;

	FormMain->SetUndo();
	if(isDown)  dir=8*yTiles-1;
	for(l=0;l<dir;l++)
	{
	for(k=0;k<xTiles;k++)
	{
		ptmp2=tileViewTable[ta+k]*16+bankActive;
		t1=chr[ptmp2];
		t2=chr[ptmp2+8];

		for (j= 0; j < yTiles; j++)
		{
			pp  =tileViewTable[ta+j*16+k]*16+bankViewTable[set+ta+j*16+k];
			pp2 =tileViewTable[ta+16+j*16+k]*16+bankViewTable[set+ta+16+j*16+k];
			ptmp=tileViewTable[ta+j*16+k]*16+7+bankViewTable[set+ta+j*16+k];


			for(i=0;i<7;i++)
			{
				if(ButtonBitmaskLo->Down||!FormMain->Applytonudge1->Checked) chr[pp+i]=chr[pp+1+i];
				if(ButtonBitmaskHi->Down||!FormMain->Applytonudge1->Checked) chr[pp+8+i]=chr[pp+9+i];
				//pp++;
			}
			//handle last row of tile
			if(yTiles==1||j+1==yTiles) //wrap condition
			{
				if(ButtonBitmaskLo->Down||!FormMain->Applytonudge1->Checked) chr[ptmp]=t1;
				if(ButtonBitmaskHi->Down||!FormMain->Applytonudge1->Checked) chr[ptmp+8]=t2;
			}
			else
			{

			   if(ButtonBitmaskLo->Down||!FormMain->Applytonudge1->Checked) chr[ptmp]=chr[pp2];
			   if(ButtonBitmaskHi->Down||!FormMain->Applytonudge1->Checked) chr[ptmp+8]=chr[pp2+8];
			}

		}

	}
	}
	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);

	FormMain->UpdateMetaSprite(false);
}



void __fastcall TFormCHREditor::ScrollDown(void)
{
	//bugged and not in use.

	int i,j,k,pp,pp2,ptmp,ptmp2,t1,t2,off;

	int xTiles=tileSelRectWdt;
	int yTiles=tileSelRectHgt;

	if (!(ButtonBitmaskLo->Down||ButtonBitmaskHi->Down))
		if(FormMain->Applytonudge1->Checked) return;


	off=(yTiles-1)*16; // * 16 tiles, not including the first count

	FormMain->SetUndo();

	for(k=0;k<xTiles;k++)
	{
		ptmp2=tileViewTable[tileActive+k+off]*16+bankActive;
		t1=chr[ptmp2];
		t2=chr[ptmp2+8];

		for (j= yTiles-1; j >= 0; j--)
		{
			pp  =tileViewTable[tileActive+j*16+k]*16+bankActive;
			pp2 =tileViewTable[tileActive+(j-1)*16+k]*16+bankActive;
			ptmp=tileViewTable[tileActive+j*16+k]*16+bankActive;
			ptmp2=tileViewTable[tileActive+k]*16+bankActive;



			//handle last row of tile
			if(yTiles==1||j==0) //wrap condition
			{
				if(ButtonBitmaskLo->Down||!FormMain->Applytonudge1->Checked) chr[ptmp2]=t1;
				if(ButtonBitmaskHi->Down||!FormMain->Applytonudge1->Checked) chr[ptmp2+8]=t2;
			}
			else //carry condition
			{

			   if(ButtonBitmaskLo->Down||!FormMain->Applytonudge1->Checked) chr[ptmp]=chr[pp2+7];
			   if(ButtonBitmaskHi->Down||!FormMain->Applytonudge1->Checked) chr[ptmp+8]=chr[pp2+8+7];
			}

			for(i=7;i>0;i--)
			{
				if(ButtonBitmaskLo->Down||!FormMain->Applytonudge1->Checked) chr[pp+i]=chr[pp-1+i];
				if(ButtonBitmaskHi->Down||!FormMain->Applytonudge1->Checked) chr[pp+8+i]=chr[pp+7+i];
				//pp++;
			}
		}

	}
	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);
	FormMain->UpdateMetaSprite(false);
}



void __fastcall TFormCHREditor::MirrorHorizontal(void)
{
	int i,j,pp,tmp;
	int x,y;
	int xTiles=chrSelection.right-chrSelection.left;//tileSelRectWdt;
	int yTiles=chrSelection.bottom-chrSelection.top;//tileSelRectHgt;
	int ba=bankActive;
	int set=ba/16;
	int ta=tileActive;
	int fLen=16;
	int fOff=0;
	//tileSelRectWdt      = chrSelection.right-chrSelection.left;
	//tileSelRectHgt		= chrSelection.bottom-chrSelection.top;
	if (!(ButtonBitmaskLo->Down||ButtonBitmaskHi->Down))
		if(FormMain->Applytomirror1->Checked) return;

	FormMain->SetUndo();

	if (!ButtonBitmaskLo->Down&&FormMain->Applytomirror1->Checked) {fLen-=8; fOff=8;}
	if (!ButtonBitmaskHi->Down&&FormMain->Applytomirror1->Checked) {fLen-=8;}


	for (y = 0; y < yTiles; y++)
	{
		for (x = 0; x < xTiles; x++)
		{
			pp=tileViewTable[ta+y*16+x]*16+bankViewTable[set+ta+y*16+x] +fOff;
			for(i=0;i<fLen;i++)
			{
				tmp=0;

				for(j=0;j<8;j++)
				{
					tmp|=(chr[pp]&(128>>j))?1<<j:0;
				}

				chr[pp++]=tmp;
			}

		}
	}

    bool b=btn2x2mode->Down?1:0;
	if(b  && tileSelRectWdt==2 && tileSelRectHgt<=2)
	{
		unsigned char tempchr[8];

		if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){

			memcpy(tempchr,													&chr[tileViewTable[ta   ]*16+bankViewTable[set+ta   ]],8);
			memcpy(&chr[tileViewTable[ta   ]*16+bankViewTable[set+ta   ]],	&chr[tileViewTable[ta+1 ]*16+bankViewTable[set+ta +1]],8);
			memcpy(&chr[tileViewTable[ta+1 ]*16+bankViewTable[set+ta+1 ]],	tempchr,8);
			if(tileSelRectHgt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]],8);
				memcpy(&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]],	&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]],8);
				memcpy(&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]],	tempchr,8);
			}
		}

		if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){


			memcpy(tempchr,														&chr[tileViewTable[ta   ]*16  +bankViewTable[set+ta ]+8],8);
			memcpy(&chr[tileViewTable[ta   ]*16+bankViewTable[set+ta   ]+8],	&chr[tileViewTable[ta+1 ]*16+bankViewTable[set+ta+1 ]+8],8);
			memcpy(&chr[tileViewTable[ta+1 ]*16+bankViewTable[set+ta+1 ]+8],	tempchr,8);
			if(tileSelRectHgt<=2){
				memcpy(tempchr,														&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]+8],8);
				memcpy(&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]+8],	&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]+8],8);
				memcpy(&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]+8],	tempchr,8);
			}
		}
	}
	if(b  && tileSelRectWdt==3 && tileSelRectHgt<=3)
	{
		unsigned char tempchr[8];

		if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){

			memcpy(tempchr,													&chr[tileViewTable[ta   ]*16+bankViewTable[set+ta   ]],8);
			memcpy(&chr[tileViewTable[ta   ]*16+bankViewTable[set+ta   ]],	&chr[tileViewTable[ta+2 ]*16+bankViewTable[set+ta +2]],8);
			memcpy(&chr[tileViewTable[ta+2 ]*16+bankViewTable[set+ta+2 ]],	tempchr,8);
			if(tileSelRectHgt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]],8);
				memcpy(&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]],	&chr[tileViewTable[ta+18]*16+bankViewTable[set+ta+18]],8);
				memcpy(&chr[tileViewTable[ta+18]*16+bankViewTable[set+ta+18]],	tempchr,8);
			}if(tileSelRectHgt<=3){
				memcpy(tempchr,													&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]],8);
				memcpy(&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]],	&chr[tileViewTable[ta+34]*16+bankViewTable[set+ta+34]],8);
				memcpy(&chr[tileViewTable[ta+34]*16+bankViewTable[set+ta+34]],	tempchr,8);
			}
		}

		if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){


			memcpy(tempchr,														&chr[tileViewTable[ta   ]*16  +bankViewTable[set+ta ]+8],8);
			memcpy(&chr[tileViewTable[ta   ]*16+bankViewTable[set+ta   ]+8],	&chr[tileViewTable[ta+2 ]*16+bankViewTable[set+ta+2 ]+8],8);
			memcpy(&chr[tileViewTable[ta+2 ]*16+bankViewTable[set+ta+2 ]+8],	tempchr,8);
			if(tileSelRectHgt<=2){
				memcpy(tempchr,														&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]+8],8);
				memcpy(&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]+8],	&chr[tileViewTable[ta+18]*16+bankViewTable[set+ta+18]+8],8);
				memcpy(&chr[tileViewTable[ta+18]*16+bankViewTable[set+ta+18]+8],	tempchr,8);
			}if(tileSelRectHgt<=3){

				memcpy(tempchr,													&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]+8],8);
				memcpy(&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]+8],	&chr[tileViewTable[ta+34]*16+bankViewTable[set+ta+34]+8],8);
				memcpy(&chr[tileViewTable[ta+34]*16+bankViewTable[set+ta+34]+8],	tempchr,8);
			}
		}
	}
	if(b  && tileSelRectWdt==4 && tileSelRectHgt<=4)
	{
		unsigned char tempchr[8];

		if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){

			//inner
			memcpy(tempchr,													&chr[tileViewTable[ta+1 ]*16+bankViewTable[set+ta +1]],8);
			memcpy(&chr[tileViewTable[ta+1 ]*16+bankViewTable[set+ta+1 ]],	&chr[tileViewTable[ta+2 ]*16+bankViewTable[set+ta +2]],8);
			memcpy(&chr[tileViewTable[ta+2 ]*16+bankViewTable[set+ta+2 ]],	tempchr,8);
			if(tileSelRectHgt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]],8);
				memcpy(&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]],	&chr[tileViewTable[ta+18]*16+bankViewTable[set+ta+18]],8);
				memcpy(&chr[tileViewTable[ta+18]*16+bankViewTable[set+ta+18]],	tempchr,8);
			}if(tileSelRectHgt<=3){
				memcpy(tempchr,													&chr[tileViewTable[ta+32+1]*16+bankViewTable[set+ta+32+1]],8);
				memcpy(&chr[tileViewTable[ta+32+1]*16+bankViewTable[set+ta+32+1]],	&chr[tileViewTable[ta+32+2 ]*16+bankViewTable[set+ta+32+2]],8);
				memcpy(&chr[tileViewTable[ta+32+2 ]*16+bankViewTable[set+ta+32+2 ]],	tempchr,8);
			}if(tileSelRectHgt<=4){
				memcpy(tempchr,													&chr[tileViewTable[ta+48+1]*16+bankViewTable[set+ta+48+1]],8);
				memcpy(&chr[tileViewTable[ta+48+1]*16+bankViewTable[set+ta+48+1]],	&chr[tileViewTable[ta+48+2]*16+bankViewTable[set+ta+48+2]],8);
				memcpy(&chr[tileViewTable[ta+48+2]*16+bankViewTable[set+ta+48+2]],	tempchr,8);
			}
			//outer

			memcpy(tempchr,													&chr[tileViewTable[ta+0 ]*16+bankViewTable[set+ta +0]],8);
			memcpy(&chr[tileViewTable[ta+0 ]*16+bankViewTable[set+ta+0 ]],	&chr[tileViewTable[ta+3 ]*16+bankViewTable[set+ta +3]],8);
			memcpy(&chr[tileViewTable[ta+3 ]*16+bankViewTable[set+ta+3 ]],	tempchr,8);
			if(tileSelRectHgt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+16+0]*16+bankViewTable[set+ta+16+0]],8);
				memcpy(&chr[tileViewTable[ta+16+0]*16+bankViewTable[set+ta+16+0]],	&chr[tileViewTable[ta+16+3]*16+bankViewTable[set+ta+16+3]],8);
				memcpy(&chr[tileViewTable[ta+16+3]*16+bankViewTable[set+ta+16+3]],	tempchr,8);
			}if(tileSelRectHgt<=3){
				memcpy(tempchr,													&chr[tileViewTable[ta+32+0]*16+bankViewTable[set+ta+32+0]],8);
				memcpy(&chr[tileViewTable[ta+32+0]*16+bankViewTable[set+ta+32+0]],	&chr[tileViewTable[ta+32+3 ]*16+bankViewTable[set+ta+32+3]],8);
				memcpy(&chr[tileViewTable[ta+32+3 ]*16+bankViewTable[set+ta+32+3 ]],	tempchr,8);
			}if(tileSelRectHgt<=4){
				memcpy(tempchr,													&chr[tileViewTable[ta+48+0]*16+bankViewTable[set+ta+48+0]],8);
				memcpy(&chr[tileViewTable[ta+48+0]*16+bankViewTable[set+ta+48+0]],	&chr[tileViewTable[ta+48+3]*16+bankViewTable[set+ta+48+3]],8);
				memcpy(&chr[tileViewTable[ta+48+3]*16+bankViewTable[set+ta+48+3]],	tempchr,8);
			}
		}

		if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){


				//inner
			memcpy(tempchr,													&chr[tileViewTable[ta+1 ]*16+bankViewTable[set+ta +1]+8],8);
			memcpy(&chr[tileViewTable[ta+1 ]*16+bankViewTable[set+ta+1 ]+8],	&chr[tileViewTable[ta+2 ]*16+bankViewTable[set+ta +2]+8],8);
			memcpy(&chr[tileViewTable[ta+2 ]*16+bankViewTable[set+ta+2 ]+8],	tempchr,8);
			if(tileSelRectHgt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]+8],8);
				memcpy(&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]+8],	&chr[tileViewTable[ta+18]*16+bankViewTable[set+ta+18]+8],8);
				memcpy(&chr[tileViewTable[ta+18]*16+bankViewTable[set+ta+18]+8],	tempchr,8);
			}if(tileSelRectHgt<=3){
				memcpy(tempchr,													&chr[tileViewTable[ta+32+1]*16+bankViewTable[set+ta+32+1]+8],8);
				memcpy(&chr[tileViewTable[ta+32+1]*16+bankViewTable[set+ta+32+1]+8],	&chr[tileViewTable[ta+32+2 ]*16+bankViewTable[set+ta+32+2]+8],8);
				memcpy(&chr[tileViewTable[ta+32+2 ]*16+bankViewTable[set+ta+32+2 ]+8],	tempchr,8);
			}if(tileSelRectHgt<=4){
				memcpy(tempchr,													&chr[tileViewTable[ta+48+1]*16+bankViewTable[set+ta+48+1]+8],8);
				memcpy(&chr[tileViewTable[ta+48+1]*16+bankViewTable[set+ta+48+1]+8],	&chr[tileViewTable[ta+48+2]*16+bankViewTable[set+ta+48+2]+8],8);
				memcpy(&chr[tileViewTable[ta+48+2]*16+bankViewTable[set+ta+48+2]+8],	tempchr,8);
			}
			//outer

			memcpy(tempchr,													&chr[tileViewTable[ta+0 ]*16+bankViewTable[set+ta +0]+8],8);
			memcpy(&chr[tileViewTable[ta+0 ]*16+bankViewTable[set+ta+0 ]+8],	&chr[tileViewTable[ta+3 ]*16+bankViewTable[set+ta +3]+8],8);
			memcpy(&chr[tileViewTable[ta+3 ]*16+bankViewTable[set+ta+3 ]+8],	tempchr,8);
			if(tileSelRectHgt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+16+0]*16+bankViewTable[set+ta+16+0]+8],8);
				memcpy(&chr[tileViewTable[ta+16+0]*16+bankViewTable[set+ta+16+0]+8],	&chr[tileViewTable[ta+16+3]*16+bankViewTable[set+ta+16+3]+8],8);
				memcpy(&chr[tileViewTable[ta+16+3]*16+bankViewTable[set+ta+16+3]+8],	tempchr,8);
			}if(tileSelRectHgt<=3){
				memcpy(tempchr,													&chr[tileViewTable[ta+32+0]*16+bankViewTable[set+ta+32+0]+8],8);
				memcpy(&chr[tileViewTable[ta+32+0]*16+bankViewTable[set+ta+32+0]+8],	&chr[tileViewTable[ta+32+3 ]*16+bankViewTable[set+ta+32+3]+8],8);
				memcpy(&chr[tileViewTable[ta+32+3 ]*16+bankViewTable[set+ta+32+3 ]+8],	tempchr,8);
			}if(tileSelRectHgt<=4){
				memcpy(tempchr,													&chr[tileViewTable[ta+48+0]*16+bankViewTable[set+ta+48+0]+8],8);
				memcpy(&chr[tileViewTable[ta+48+0]*16+bankViewTable[set+ta+48+0]+8],	&chr[tileViewTable[ta+48+3]*16+bankViewTable[set+ta+48+3]+8],8);
				memcpy(&chr[tileViewTable[ta+48+3]*16+bankViewTable[set+ta+48+3]+8],	tempchr,8);
            }

		}
	}
	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);
	FormMain->UpdateMetaSprite(false);
}



void __fastcall TFormCHREditor::MirrorVertical(void)
{
	int i,pp;
	int x,y;
    int ba=bankActive;
    int set=ba/16;
	int ta=tileActive;

	int xTiles=tileSelRectWdt;
	int yTiles=tileSelRectHgt;
	unsigned char tmp[16];

	if (!(ButtonBitmaskLo->Down||ButtonBitmaskHi->Down))
		if(FormMain->Applytomirror1->Checked) return;

	FormMain->SetUndo();

	for (y = 0; y < yTiles; y++)
	{
		for (x = 0; x < xTiles; x++)
		{
			pp=tileViewTable[ta+y*16+x]*16+bankViewTable[set+ta+y*16+x];

			for(i=0;i<8;i++)
			{
				if(ButtonBitmaskLo->Down||!FormMain->Applytomirror1->Checked) tmp[i]=chr[pp];
				if(ButtonBitmaskHi->Down||!FormMain->Applytomirror1->Checked) tmp[i+8]=chr[pp+8];
				pp++;
			}
			for(i=0;i<8;i++)
			{
				pp--;
				if(ButtonBitmaskLo->Down||!FormMain->Applytomirror1->Checked) chr[pp]=tmp[i];
				if(ButtonBitmaskHi->Down||!FormMain->Applytomirror1->Checked) chr[pp+8]=tmp[i+8];
			}
		}
	}
	bool b=btn2x2mode->Down?1:0;
	if(b && tileSelRectWdt<=2 && tileSelRectHgt==2)
	{
		//int pp=tileViewTable[tileActive]*16+ba;
		unsigned char tempchr[8];

		if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){
			memcpy(tempchr,						 							&chr[tileViewTable[ta]*16+bankViewTable[set+ta]],8);
			memcpy(&chr[tileViewTable[ta]*16+bankViewTable[set+ta]],		&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]],8);
			memcpy(&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]],	tempchr,8);
			if(tileSelRectWdt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]],8);
				memcpy(&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]], 	&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]],8);
				memcpy(&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]],	tempchr,8);
			}
		}

		if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
				memcpy(tempchr,						 							&chr[tileViewTable[ta]*16+bankViewTable[set+ta]+8],8);
				memcpy(&chr[tileViewTable[ta]*16 + bankViewTable[set+ta]+8],	&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]+8],8);
				memcpy(&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]+8],tempchr,8);
			if(tileSelRectWdt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]+8],8);
				memcpy(&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]+8], 	&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]+8],8);
				memcpy(&chr[tileViewTable[ta+17]*16+bankViewTable[set+ta+17]+8],tempchr,8);
			}
		}
	}
	if(b && tileSelRectWdt<=3 && tileSelRectHgt==3)
	{
		//int pp=tileViewTable[tileActive]*16+ba;
		unsigned char tempchr[8];

		if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){
				memcpy(tempchr,						 							&chr[tileViewTable[ta]*16+bankViewTable[set+ta]],8);
				memcpy(&chr[tileViewTable[ta]*16+bankViewTable[set+ta]],		&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]],8);
				memcpy(&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]],	tempchr,8);
			if(tileSelRectWdt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]],8);
				memcpy(&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]], 	&chr[tileViewTable[ta+33]*16+bankViewTable[set+ta+33]],8);
				memcpy(&chr[tileViewTable[ta+33]*16+bankViewTable[set+ta+33]],	tempchr,8);
			}if(tileSelRectWdt<=3){
				memcpy(tempchr,													&chr[tileViewTable[ta+2]*16+bankViewTable[set+ta+2]],8);
				memcpy(&chr[tileViewTable[ta+2]*16+bankViewTable[set+ta+2]], 	&chr[tileViewTable[ta+34]*16+bankViewTable[set+ta+34]],8);
				memcpy(&chr[tileViewTable[ta+34]*16+bankViewTable[set+ta+34]],	tempchr,8);
			}
		}

		if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
				memcpy(tempchr,						 							&chr[tileViewTable[ta]*16+bankViewTable[set+ta]+8],8);
				memcpy(&chr[tileViewTable[ta]*16 + bankViewTable[set+ta]+8],	&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]+8],8);
				memcpy(&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]+8],tempchr,8);
			if(tileSelRectWdt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]+8],8);
				memcpy(&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]+8], 	&chr[tileViewTable[ta+33]*16+bankViewTable[set+ta+33]+8],8);
				memcpy(&chr[tileViewTable[ta+33]*16+bankViewTable[set+ta+33]+8],tempchr,8);
			}if(tileSelRectWdt<=3){
				memcpy(tempchr,													&chr[tileViewTable[ta+2]*16+bankViewTable[set+ta+2]+8],8);
				memcpy(&chr[tileViewTable[ta+2]*16+bankViewTable[set+ta+2]+8], 	&chr[tileViewTable[ta+34]*16+bankViewTable[set+ta+34]+8],8);
				memcpy(&chr[tileViewTable[ta+34]*16+bankViewTable[set+ta+34]+8],	tempchr,8);
			}
		}
	}
	if(b && (tileSelRectWdt<=4) && tileSelRectHgt==4)
	{
		//int pp=tileViewTable[tileActive]*16+ba;
		unsigned char tempchr[8];

		if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){

			//inner
			memcpy(tempchr,						 							&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]],8);
			memcpy(&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]],		&chr[tileViewTable[ta+48]*16+bankViewTable[set+ta+32]],8);
			memcpy(&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]],	tempchr,8);

			if(tileSelRectWdt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+16+1]*16+bankViewTable[set+ta+16+1]],8);
				memcpy(&chr[tileViewTable[ta+16+1]*16+bankViewTable[set+ta+16+1]], 	&chr[tileViewTable[ta+32+1]*16+bankViewTable[set+ta+32+1]],8);
				memcpy(&chr[tileViewTable[ta+32+1]*16+bankViewTable[set+ta+32+1]],	tempchr,8);
			}
			if(tileSelRectWdt<=3){
				memcpy(tempchr,						 							&chr[tileViewTable[ta+16+2]*16+bankViewTable[set+ta+16+2]],8);
				memcpy(&chr[tileViewTable[ta+16+2]*16+bankViewTable[set+ta+16+2]],		&chr[tileViewTable[ta+32+2]*16+bankViewTable[set+ta+32+2]],8);
				memcpy(&chr[tileViewTable[ta+32+2]*16+bankViewTable[set+ta+32+2]],	tempchr,8);
			}
			if(tileSelRectWdt<=4){
				memcpy(tempchr,													&chr[tileViewTable[ta+16+3]*16+bankViewTable[set+ta+16+3]],8);
				memcpy(&chr[tileViewTable[ta+16+3]*16+bankViewTable[set+ta+16+3]], 	&chr[tileViewTable[ta+32+3]*16+bankViewTable[set+ta+32+3]],8);
				memcpy(&chr[tileViewTable[ta+32+3]*16+bankViewTable[set+ta+32+3]],	tempchr,8);
			}

			//outer
				memcpy(tempchr,						 							&chr[tileViewTable[ta]*16+bankViewTable[set+ta]],8);
				memcpy(&chr[tileViewTable[ta]*16+bankViewTable[set+ta]],		&chr[tileViewTable[ta+48]*16+bankViewTable[set+ta+48]],8);
				memcpy(&chr[tileViewTable[ta+48]*16+bankViewTable[set+ta+48]],	tempchr,8);
			if(tileSelRectWdt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]],8);
				memcpy(&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]], 	&chr[tileViewTable[ta+48+1]*16+bankViewTable[set+ta+48+1]],8);
				memcpy(&chr[tileViewTable[ta+48+1]*16+bankViewTable[set+ta+48+1]],	tempchr,8);
			}
			if(tileSelRectWdt<=3){
				memcpy(tempchr,						 							&chr[tileViewTable[ta+2]*16+bankViewTable[set+ta+2]],8);
				memcpy(&chr[tileViewTable[ta+2]*16+bankViewTable[set+ta+2]],		&chr[tileViewTable[ta+48+2]*16+bankViewTable[set+ta+48+2]],8);
				memcpy(&chr[tileViewTable[ta+48+2]*16+bankViewTable[set+ta+48+2]],	tempchr,8);
			}
			if(tileSelRectWdt<=4){
				memcpy(tempchr,													&chr[tileViewTable[ta+3]*16+bankViewTable[set+ta+3]],8);
				memcpy(&chr[tileViewTable[ta+3]*16+bankViewTable[set+ta+3]], 	&chr[tileViewTable[ta+48+3]*16+bankViewTable[set+ta+48+3]],8);
				memcpy(&chr[tileViewTable[ta+48+3]*16+bankViewTable[set+ta+48+3]],	tempchr,8);
			}
		}

		if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
			//inner
				memcpy(tempchr,						 							&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]+8],8);
				memcpy(&chr[tileViewTable[ta+16]*16+bankViewTable[set+ta+16]+8],		&chr[tileViewTable[ta+48]*16+bankViewTable[set+ta+32]+8],8);
				memcpy(&chr[tileViewTable[ta+32]*16+bankViewTable[set+ta+32]+8],	tempchr,8);
			if(tileSelRectWdt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+16+1]*16+bankViewTable[set+ta+16+1]+8],8);
				memcpy(&chr[tileViewTable[ta+16+1]*16+bankViewTable[set+ta+16+1]+8], 	&chr[tileViewTable[ta+32+1]*16+bankViewTable[set+ta+32+1]+8],8);
				memcpy(&chr[tileViewTable[ta+32+1]*16+bankViewTable[set+ta+32+1]+8],	tempchr,8);
			}if(tileSelRectWdt<=3){
				memcpy(tempchr,						 							&chr[tileViewTable[ta+16+2]*16+bankViewTable[set+ta+16+2]+8],8);
				memcpy(&chr[tileViewTable[ta+16+2]*16+bankViewTable[set+ta+16+2]+8],		&chr[tileViewTable[ta+32+2]*16+bankViewTable[set+ta+32+2]+8],8);
				memcpy(&chr[tileViewTable[ta+32+2]*16+bankViewTable[set+ta+32+2]+8],	tempchr,8);
			}if(tileSelRectWdt<=4){
				memcpy(tempchr,													&chr[tileViewTable[ta+16+3]*16+bankViewTable[set+ta+16+3]+8],8);
				memcpy(&chr[tileViewTable[ta+16+3]*16+bankViewTable[set+ta+16+3]+8], 	&chr[tileViewTable[ta+32+3]*16+bankViewTable[set+ta+32+3]+8],8);
				memcpy(&chr[tileViewTable[ta+32+3]*16+bankViewTable[set+ta+32+3]+8],	tempchr,8);
			}

			//outer
				memcpy(tempchr,						 							&chr[tileViewTable[ta]*16+bankViewTable[set+ta]+8],8);
				memcpy(&chr[tileViewTable[ta]*16+bankViewTable[set+ta]+8],		&chr[tileViewTable[ta+48]*16+bankViewTable[set+ta+48]+8],8);
				memcpy(&chr[tileViewTable[ta+48]*16+bankViewTable[set+ta+48]+8],	tempchr,8);
			if(tileSelRectWdt<=2){
				memcpy(tempchr,													&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]+8],8);
				memcpy(&chr[tileViewTable[ta+1]*16+bankViewTable[set+ta+1]+8], 	&chr[tileViewTable[ta+48+1]*16+bankViewTable[set+ta+48+1]+8],8);
				memcpy(&chr[tileViewTable[ta+48+1]*16+bankViewTable[set+ta+48+1]+8],	tempchr,8);
			}if(tileSelRectWdt<=3){
				memcpy(tempchr,						 							&chr[tileViewTable[ta+2]*16+bankViewTable[set+ta+2]+8],8);
				memcpy(&chr[tileViewTable[ta+2]*16+bankViewTable[set+ta+2]+8],		&chr[tileViewTable[ta+48+2]*16+bankViewTable[set+ta+48+2]+8],8);
				memcpy(&chr[tileViewTable[ta+48+2]*16+bankViewTable[set+ta+48+2]+8],	tempchr,8);
			}if(tileSelRectWdt<=4){
				memcpy(tempchr,													&chr[tileViewTable[ta+3]*16+bankViewTable[set+ta+3]+8],8);
				memcpy(&chr[tileViewTable[ta+3]*16+bankViewTable[set+ta+3]+8], 	&chr[tileViewTable[ta+48+3]*16+bankViewTable[set+ta+48+3]+8],8);
				memcpy(&chr[tileViewTable[ta+48+3]*16+bankViewTable[set+ta+48+3]+8],	tempchr,8);
			}
		}
	}
	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);
	FormMain->UpdateMetaSprite(false);
}



void __fastcall TFormCHREditor::Flip90(bool dir)
{
	int i,j,pp;
	int x,y;
	int xTiles=tileSelRectWdt;
	int yTiles=tileSelRectHgt;
	int set = bankActive/16;

	unsigned char tile[8][8],tile_flip[8][8];

	if (!(ButtonBitmaskLo->Down||ButtonBitmaskHi->Down))
		if(FormMain->Applytorotate1->Checked)return;
	FormMain->SetUndo();

	for (y = 0; y < yTiles; y++)
	{
		for (x = 0; x < xTiles; x++)
		{
			pp=tileViewTable[tileActive+y*16+x]*16+bankViewTable[set+tileActive+y*16+x];
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			tile[i][j]=(chr[pp]&(128>>j)?1:0)|(chr[pp+8]&(128>>j)?2:0);
		}

		pp++;
	}

	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			tile_flip[i][j]=dir?tile[j][7-i]:tile[7-j][i];
		}
	}

	//pp=tileActive*16+bankActive;
	pp=tileViewTable[tileActive+y*16+x]*16+bankViewTable[set+tileActive+y*16+x];

	for(i=0;i<8;i++)
	{
		chr[pp+0]=0;
		chr[pp+8]=0;

		for(j=0;j<8;j++)
		{
			if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked) chr[pp+0]|= (tile_flip[i][j]&1)<<(7-j);
			else chr[pp+0]|= (tile[i][j]&1)<<(7-j);
			if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked) chr[pp+8]|=((tile_flip[i][j]&2)>>1)<<(7-j);
			else chr[pp+8]|=((tile[i][j]&2)>>1)<<(7-j);
		}

		pp++;
	}
	}
		}
}

 void __fastcall TFormCHREditor::Rotate4tiles(bool dir)
 {
	bool b=btn2x2mode->Down?1:0;
	int set = bankActive/16;
	int ta = tileActive;
	int pp=tileViewTable[ta]*16+bankViewTable[set+ta];
	int pp1=tileViewTable[ta+1]*16+bankViewTable[set+ta+1];
	int pp2=tileViewTable[ta+2]*16+bankViewTable[set+ta+2];
	int pp3=tileViewTable[ta+3]*16+bankViewTable[set+ta+3];

	int pp16=tileViewTable[ta+16]*16+bankViewTable[set+ta+16];
	int pp17=tileViewTable[ta+17]*16+bankViewTable[set+ta+17];
	int pp18=tileViewTable[ta+18]*16+bankViewTable[set+ta+18];
	int pp19=tileViewTable[ta+19]*16+bankViewTable[set+ta+19];

	int pp32=tileViewTable[ta+32]*16+bankViewTable[set+ta+32];
	int pp33=tileViewTable[ta+33]*16+bankViewTable[set+ta+33];
	int pp34=tileViewTable[ta+34]*16+bankViewTable[set+ta+34];
	int pp35=tileViewTable[ta+35]*16+bankViewTable[set+ta+35];

	int pp48=tileViewTable[ta+48]*16+bankViewTable[set+ta+48];
	int pp49=tileViewTable[ta+49]*16+bankViewTable[set+ta+49];
	int pp50=tileViewTable[ta+50]*16+bankViewTable[set+ta+50];
	int pp51=tileViewTable[ta+51]*16+bankViewTable[set+ta+51];

	unsigned char tempchr[8];

	if(b && tileSelRectWdt==2 && tileSelRectHgt==2)

	{
		if(!dir){

			if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){
				memcpy(tempchr,&chr[pp],8);
				memcpy(&chr[pp],&chr[pp16],8);
				memcpy(&chr[pp16],&chr[pp17],8);
				memcpy(&chr[pp17],&chr[pp1],8);
				memcpy(&chr[pp1],tempchr,8);
			}

			if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
				//pp+=8;
				memcpy(tempchr,&chr[pp+8],8);
				memcpy(&chr[pp+8],&chr[pp16+8],8);
				memcpy(&chr[pp16+8],&chr[pp17+8],8);
				memcpy(&chr[pp17+8],&chr[pp1+8],8);
				memcpy(&chr[pp1+8],tempchr,8);
			}
		}
		else{
        	if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){
				memcpy(tempchr,&chr[pp],8);
				memcpy(&chr[pp],&chr[pp1],8);
				memcpy(&chr[pp1],&chr[pp17],8);
				memcpy(&chr[pp17],&chr[pp16],8);
				memcpy(&chr[pp16],tempchr,8);
			}

			if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
				//pp+=8;
				memcpy(tempchr,&chr[pp+8],8);
				memcpy(&chr[pp+8],&chr[pp1+8],8);
				memcpy(&chr[pp1+8],&chr[pp17+8],8);
				memcpy(&chr[pp17+8],&chr[pp16+8],8);
				memcpy(&chr[pp16+8],tempchr,8);
			}
		}
	}
	if(b && tileSelRectWdt==3 && tileSelRectHgt==3){
		for(int i=0;i<2;i++){
		if(!dir){

			if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){

				memcpy(tempchr,&chr[pp],8);

				memcpy(&chr[pp],&chr[pp16],8);
				memcpy(&chr[pp16],&chr[pp32],8);

				memcpy(&chr[pp32],&chr[pp33],8);
				memcpy(&chr[pp33],&chr[pp34],8);
				memcpy(&chr[pp34],&chr[pp18],8);

				memcpy(&chr[pp18],&chr[pp2],8);
				memcpy(&chr[pp2],&chr[pp1],8);
				memcpy(&chr[pp1],tempchr,8);

			}

			if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
				memcpy(tempchr,&chr[pp+8],8);

				memcpy(&chr[pp+8],&chr[pp16+8],8);
				memcpy(&chr[pp16+8],&chr[pp32+8],8);

				memcpy(&chr[pp32+8],&chr[pp33+8],8);
				memcpy(&chr[pp33+8],&chr[pp34+8],8);
				memcpy(&chr[pp34+8],&chr[pp18+8],8);

				memcpy(&chr[pp18+8],&chr[pp2+8],8);
				memcpy(&chr[pp2+8],&chr[pp1+8],8);
				memcpy(&chr[pp1+8],tempchr,8);

			}
		}
		else{
			if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){
				memcpy(tempchr,&chr[pp],8);
				memcpy(&chr[pp],&chr[pp1],8);
				memcpy(&chr[pp1],&chr[pp2],8);
				memcpy(&chr[pp2],&chr[pp18],8);
				memcpy(&chr[pp18],&chr[pp34],8);
				memcpy(&chr[pp34],&chr[pp33],8);
				memcpy(&chr[pp33],&chr[pp32],8);
				memcpy(&chr[pp32],&chr[pp16],8);
				memcpy(&chr[pp16],tempchr,8);

			}

			if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
				memcpy(tempchr,&chr[pp+8],8);
				memcpy(&chr[pp+8],&chr[pp1+8],8);
				memcpy(&chr[pp1+8],&chr[pp2+8],8);
				memcpy(&chr[pp2+8],&chr[pp18+8],8);
				memcpy(&chr[pp18+8],&chr[pp34+8],8);
				memcpy(&chr[pp34+8],&chr[pp33+8],8);
				memcpy(&chr[pp33+8],&chr[pp32+8],8);
				memcpy(&chr[pp32+8],&chr[pp16+8],8);
				memcpy(&chr[pp16+8],tempchr,8);
			}
		}
		}

	}
		if(b && tileSelRectWdt==4 && tileSelRectHgt==4)

	{
		if(!dir){

			//inner circle
			if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){

				memcpy(tempchr,&chr[pp17],8);
				memcpy(&chr[pp17],&chr[pp33],8);
				memcpy(&chr[pp33],&chr[pp34],8);
				memcpy(&chr[pp34],&chr[pp18],8);
				memcpy(&chr[pp18],tempchr,8);
			}

			if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){

				memcpy(tempchr,&chr[pp17+8],8);
				memcpy(&chr[pp17+8],&chr[pp33+8],8);
				memcpy(&chr[pp33+8],&chr[pp34+8],8);
				memcpy(&chr[pp34+8],&chr[pp18+8],8);
				memcpy(&chr[pp18+8],tempchr,8);
			}

			//outer circle
			for(int i=0;i<3;i++){
				if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){
				   memcpy(tempchr,&chr[pp],8);
				   memcpy(&chr[pp],&chr[pp16],8);
				   memcpy(&chr[pp16],&chr[pp32],8);
				   memcpy(&chr[pp32],&chr[pp48],8);

				   memcpy(&chr[pp48],&chr[pp49],8);
				   memcpy(&chr[pp49],&chr[pp50],8);
				   memcpy(&chr[pp50],&chr[pp51],8);
				   memcpy(&chr[pp51],&chr[pp35],8);
				   memcpy(&chr[pp35],&chr[pp19],8);
				   memcpy(&chr[pp19],&chr[pp3],8);

				   memcpy(&chr[pp3],&chr[pp2],8);
				   memcpy(&chr[pp2],&chr[pp1],8);
				   memcpy(&chr[pp1],tempchr,8);

				}
				if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
					memcpy(tempchr,&chr[pp+8],8);
					memcpy(&chr[pp+8],&chr[pp16+8],8);
					memcpy(&chr[pp16+8],&chr[pp32+8],8);
					memcpy(&chr[pp32+8],&chr[pp48+8],8);

					memcpy(&chr[pp48+8],&chr[pp49+8],8);
					memcpy(&chr[pp49+8],&chr[pp50+8],8);
					memcpy(&chr[pp50+8],&chr[pp51+8],8);
					memcpy(&chr[pp51+8],&chr[pp35+8],8);
					memcpy(&chr[pp35+8],&chr[pp19+8],8);
					memcpy(&chr[pp19+8],&chr[pp3+8],8);

					memcpy(&chr[pp3+8],&chr[pp2+8],8);
					memcpy(&chr[pp2+8],&chr[pp1+8],8);
					memcpy(&chr[pp1+8],tempchr,8);
				}
			}
		}
		else{

			//inner  cicle
			if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){

				memcpy(tempchr,&chr[pp17],8);
				memcpy(&chr[pp17],&chr[pp18],8);
				memcpy(&chr[pp18],&chr[pp34],8);
				memcpy(&chr[pp34],&chr[pp33],8);
				memcpy(&chr[pp33],tempchr,8);

			}

			if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){

				memcpy(tempchr,&chr[pp17+8],8);
				memcpy(&chr[pp17+8],&chr[pp18+8],8);
				memcpy(&chr[pp18+8],&chr[pp34+8],8);
				memcpy(&chr[pp34+8],&chr[pp33+8],8);
				memcpy(&chr[pp33+8],tempchr,8);
			}
			for(int i=0;i<3;i++){
				if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){
				   memcpy(tempchr,&chr[pp],8);
				   memcpy(&chr[pp],&chr[pp1],8);
				   memcpy(&chr[pp1],&chr[pp2],8);
				   memcpy(&chr[pp2],&chr[pp3],8);

				   memcpy(&chr[pp3],&chr[pp19],8);
				   memcpy(&chr[pp19],&chr[pp35],8);
				   memcpy(&chr[pp35],&chr[pp51],8);
				   memcpy(&chr[pp51],&chr[pp50],8);
				   memcpy(&chr[pp50],&chr[pp49],8);
				   memcpy(&chr[pp49],&chr[pp48],8);

				   memcpy(&chr[pp48],&chr[pp32],8);
				   memcpy(&chr[pp32],&chr[pp16],8);
				   memcpy(&chr[pp16],tempchr,8);

				}
				if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
					memcpy(tempchr,&chr[pp+8],8);
				   memcpy(&chr[pp+8],&chr[pp1+8],8);
				   memcpy(&chr[pp1+8],&chr[pp2+8],8);
				   memcpy(&chr[pp2+8],&chr[pp3+8],8);

				   memcpy(&chr[pp3+8],&chr[pp19+8],8);
				   memcpy(&chr[pp19+8],&chr[pp35+8],8);
				   memcpy(&chr[pp35+8],&chr[pp51+8],8);
				   memcpy(&chr[pp51+8],&chr[pp50+8],8);
				   memcpy(&chr[pp50+8],&chr[pp49+8],8);
				   memcpy(&chr[pp49+8],&chr[pp48+8],8);

				   memcpy(&chr[pp48+8],&chr[pp32+8],8);
				   memcpy(&chr[pp32+8],&chr[pp16+8],8);
				   memcpy(&chr[pp16+8],tempchr,8);
				}
			}
		}
	}
 }

void __fastcall TFormCHREditor::Fill(TShiftState Shift,int x,int y, int tx, int ty, int extpp, int bufpp, bool isNametable)                  //int tempPal, int fillPal,
{
	//tx and ty are only used by tileset canvas.
	int sx,sy,sw,sh;
	FormMain->GetSelection(chrSelection,sx,sy,sw,sh);
	int margin=2;
	int set=bankActive/16;
	int bpp;
	unsigned char buf[128+2+2][128+2+2];
	int i,j,pp,col,cnt;


	bool noncontiguous = ((Shift.Contains(ssAlt)) && (!Shift.Contains(ssShift)));
	bool swapbucket = (Shift.Contains(ssRight));
	int mask;
	int tempCol;
	int chrSelCount=0;
	char swapbucket_buf[16];
	int colGapCount[256];
	for (i = 0; i < 256; i++) {
		 colGapCount[i]=0;
	}

	//if function is called with -1,-1 in x y point, don't update coordinates.
	static int stx;
	static int sty;

	if(x>=0) stx=x;
	if(y>=0) sty=y;

	//bytecode definitions
	unsigned char border=255;
	unsigned char fieldtest=254;
	unsigned char drawfield=253;
	unsigned char gaptest=252;
	unsigned char drawgap=251;
	unsigned char nodraw=250;

	//get toolbox state

	bool bFillFields = (FormBucketToolbox->btnBoth->Down || FormBucketToolbox->btnFields->Down);
	bool bFillGaps	= (FormBucketToolbox->btnBoth->Down || FormBucketToolbox->btnGaps->Down);
	bool bForgiving = (FormBucketToolbox->btnForgiving->Down);
	bool bFieldPen = (FormBucketToolbox->btnFieldPen->Down);
	bool bGapPen = (FormBucketToolbox->btnGapPen->Down);
	bool bSides =  (FormBucketToolbox->btnSides->Down);
	bool bSemis =  (FormBucketToolbox->btnSemis->Down);
	bool bDiags =  (FormBucketToolbox->btnDiags->Down);



	bool b8way  =  (FormBucketToolbox->btn8way->Down);
	bool bCway  =  (FormBucketToolbox->btnCustomway->Down);
	bool bCwayAware = (FormBucketToolbox->btnSmartCustom->Down);
	bool bC_nw	=  (FormBucketToolbox->btnC_nw->Down);
	bool bC_n   =  (FormBucketToolbox->btnC_n->Down);
	bool bC_ne  =  (FormBucketToolbox->btnC_ne->Down);
	bool bC_e   =  (FormBucketToolbox->btnC_e->Down);
	bool bC_se  =  (FormBucketToolbox->btnC_se->Down);
	bool bC_s   =  (FormBucketToolbox->btnC_s->Down);
	bool bC_sw  =  (FormBucketToolbox->btnC_sw->Down);
	bool bC_w   =  (FormBucketToolbox->btnC_w->Down);

	bool bClassicForceBuf = (FormBucketToolbox->btnForceBuf->Down && bCway && !FormBucketToolbox->btnForgiving->Down);
	bool bAwareForceBuf = (FormBucketToolbox->btnForceBuf->Down && bCwayAware && FormBucketToolbox->btnForgiving->Down);


	//remembered state
	static int replaceColField;
	static int replaceColGap;
	static int bordercol;

	if(noncontiguous)
	{
		for(i=0;i<256;i++) if(chrSelected[i])chrSelCount++;

		if(isNametable)
		{
			for(int py=0;py<8;py++){
				for(int px=0;px<8;px++){
					pp=extpp+py;
					mask=128>>px;
					bpp=bufpp+py;
					tempCol=(((chrBufFill[bpp]<<px)&128)>>7)|(((chrBufFill[bpp+8]<<px)&128)>>6);
					if (iSmudge==tempCol)
					{
						if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
						if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
					}
					if((swapbucket) && bgPalCur==tempCol)
					{
						if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((iSmudge&1)<<7)>>px);
						if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((iSmudge&2)<<6)>>px);
					}
				}
			}
		}
		//tileset; box selection
		else{
			if(chrSelectRect && sw>1 && sh>1)
			{
				for(int tx=0;tx<sw;tx++){
					for(int ty=0;ty<sh;ty++){

						for(int py=0;py<8;py++){
							for(int px=0;px<8;px++){
								pp=tileViewTable[tx+sx + (ty+sy)*16]*16 +bankViewTable[set+tx+sx + (ty+sy)*16]+py;
								bpp=tileViewTable[tx+sx + (ty+sy)*16]*16 +py;
								mask=128>>px;
								tempCol=(((chrBufFill[bpp]<<px)&128)>>7)|(((chrBufFill[bpp+8]<<px)&128)>>6);
								if (iSmudge==tempCol)
								{
									if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
									if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
								}
								if((swapbucket) && bgPalCur==tempCol)
								{
									if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((iSmudge&1)<<7)>>px);
									if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((iSmudge&2)<<6)>>px);
								}
							}
						}

					}
				}
			}
			else if(extpp>=0 && chrSelCount<2)  //single selection
			{
				for(int py=0;py<8;py++){
					for(int px=0;px<8;px++){
						pp=extpp+py;
						bpp=bufpp;
						mask=128>>px;
						tempCol=(((chrBufFill[bpp]<<px)&128)>>7)|(((chrBufFill[bpp+8]<<px)&128)>>6);
						if (iSmudge==tempCol)
						{
							if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
							if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
						}
						if((swapbucket) && bgPalCur==tempCol)
						{
							if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((iSmudge&1)<<7)>>px);
							if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((iSmudge&2)<<6)>>px);
						}
					}
				}

			}
			else   //multi selection
			{
				for(i=0;i<256;i++)
				{
					if(chrSelected[i])
					{
						for(int py=0;py<8;py++){
							for(int px=0;px<8;px++){
								pp=tileViewTable[i]*16 + bankViewTable[set + i] +py;
								bpp=tileViewTable[i]*16+py;
								mask=128>>px;
								tempCol=(((chrBufFill[bpp]<<px)&128)>>7)|(((chrBufFill[bpp+8]<<px)&128)>>6);
								if (iSmudge==tempCol)
								{
									if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
									if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
								}
								if((swapbucket) && bgPalCur==tempCol)
								{
									if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((iSmudge&1)<<7)>>px);
									if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((iSmudge&2)<<6)>>px);
								}

							}
						}
					}
				}
			}
		}
		return;
	}

	//classic and gap aware modes:
	//----------------------------
	//populate tile buffer
	if(isNametable){
		//since this code has been enclosed in isNametable for the advancement of a freer chr editor mode below,
		//some of this is redundant now.
		if(extpp<0){ pp=tileViewTable[tileActive]*16+bankViewTable[set+tileActive];   //old typical CHR editor behaviour
					bpp=tileViewTable[tileActive]*16;   }
		else {pp=extpp;  bpp=bufpp;}                           //typical tileset editor (and 2x2 mode) behaviour

		for(i=0;i<12;i++)
		{
			for(j=0;j<12;j++)
			{
				if(i<2||i>9||j<2||j>9)
				{
					buf[i][j]=255;
				}
				else
				{
					if (bAwareForceBuf || bClassicForceBuf) buf[i][j]=(chrBufFill[bpp]&(128>>(j-2))?1:0)|(chrBufFill[bpp+8]&(128>>(j-2))?2:0);
					else if ((penActive==0&&(!Shift.Contains(ssShift)))||extpp>=0)
							buf[i][j]=(chr[pp]&(128>>(j-2))?1:0)|(chr[pp+8]&(128>>(j-2))?2:0);
					else    buf[i][j]=(chrBufFill[bpp]&(128>>(j-2))?1:0)|(chrBufFill[bpp+8]&(128>>(j-2))?2:0);
				}
			}
			if(i>1&&i<10) pp++;
		}

		col=buf[sty+2][stx+2];  //cancel replace-mode if colour is the same, excluding border colour mode.

		if (penActive==0&&(!Shift.Contains(ssShift))) {
		if(col==bgPalCur && (bFieldPen)) return;
		}
		buf[sty+2][stx+2]=254; //seed a test on the clicked coordinate
    	cueCHRdrawAll=true;
	}
	//new typical
	else{


		for(i=0;i<128+2+2;i++){
			for(j=0;j<128+2+2;j++){
				buf[i][j]=255;
			}
		}
		for(int tx=sx;tx<sw+sx;tx++){
			for(int ty=sy;ty<sh+sy;ty++){

				for(int py=0;py<8;py++){

					for(int px=0;px<8;px++){


							pp=tileViewTable[tx + ty*16]*16 + bankViewTable[set + tx + ty*16]+py;
							bpp=tileViewTable[tx + ty*16]*16+ py;



						mask=128>>px;
						if (bAwareForceBuf || bClassicForceBuf)
							buf[ty*8+py+margin][tx*8+px+margin]=(((chrBufFill[bpp]<<px)&128)>>7)|(((chrBufFill[bpp+8]<<px)&128)>>6);
						else if ((penActive==0&&(!Shift.Contains(ssShift)))||extpp>=0)
							buf[ty*8+py+margin][tx*8+px+margin]=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);
						else
							buf[ty*8+py+margin][tx*8+px+margin]=(((chrBufFill[bpp]<<px)&128)>>7)|(((chrBufFill[bpp+8]<<px)&128)>>6);
					}
				}
			}

		}
		if(tx>=0&&ty>=0)	col=buf[ty*8 +sty+2][tx*8 +stx+2];
		else 				col=buf[sy*8 +sty+2][sx*8 +stx+2];
		//cancel replace-mode if colour is the same, excluding border colour mode.
		if (penActive==0&&(!Shift.Contains(ssShift))) {
			if(col==bgPalCur && (bFieldPen)) return;
		}
		//seed a test on the clicked coordinate
		if(tx>=0&&ty>=0)    buf[ty*8 + sty+2][tx*8 +stx+2]=254;
		else        		buf[sy*8 + sty+2][sx*8 +stx+2]=254;

	}

	int offx,offy,lenx,leny;
	if(isNametable){offx=margin;offy=margin;lenx=8+margin; leny=8+margin;}
	else {
		offx=margin+sx*8;
		offy=margin+sy*8;
		lenx=margin+(sx+sw)*8;
		leny=margin+(sy+sh)*8;

	}
	//first pass, fill field


	while(true)
	{
		cnt=0;

		
		for(i=offy;i<leny;i++)
		{
			for(j=offx;j<lenx;j++)
			{

				if(buf[i][j]==254)
				{

					if(bForgiving){
					int testfield=254;

					bool isGap=false;
					//north
					if(buf[i-1][j]==col && (!bCwayAware || (bCwayAware && (bC_n || bC_nw || bC_ne )))){

						if(bSides
							&& (buf[i-1][j+1]!=col && buf[i-1][j-1]!=col)
							&& (buf[i-1][j+1]!=testfield && buf[i-1][j-1]!=testfield)
							&& (buf[i-2][j]==col)) //blocks mistaking a surrounded corner for a gap
						 {
							 colGapCount[buf[i-1][j+1]]++;
							 colGapCount[buf[i-1][j-1]]++;
							 isGap=true;
						 }

						if(bSemis){
							if((buf[i][j+1]!=col && buf[i-1][j-1]!=col)
							&&  (buf[i][j+1]<250 && buf[i-1][j-1]<250))
							{
								colGapCount[buf[i][j+1]]++;
								colGapCount[buf[i-1][j-1]]++;
								isGap=true;
							}
							if((buf[i-1][j+1]!=col && buf[i][j-1]!=col)
							&&  (buf[i-1][j+1]<250 && buf[i][j-1]<250))
							{
								colGapCount[buf[i-1][j+1]]++;
								colGapCount[buf[i][j-1]]++;
								isGap=true;
							}
						}
						if(bDiags){
							if	((buf[i][j+1]!=col && buf[i-2][j-1]!=col)
							&& (buf[i][j+1]<250 && buf[i-2][j-1]<250)
							&& (buf[i-1][j+1]==col && buf[i-2][j]==col)) //blocks an undesired positive
							{
								colGapCount[buf[i][j+1]]++;
								colGapCount[buf[i-2][j-1]]++;
								isGap=true;
							}
							if	((buf[i-2][j+1]!=col && buf[i][j-1]!=col)
							&& (buf[i-2][j+1]<250 && buf[i][j-1]<250)
							&& (buf[i-2][j]==col && buf[i-1][j-1]==col)) //blocks an undesired positive on the other diagonal
							{
								colGapCount[buf[i-2][j+1]]++;
								colGapCount[buf[i][j-1]]++;
								isGap=true;
							}
						}

						if(isGap) buf[i-1][j]=252;
						else 	  buf[i-1][j]=254;
					}

					//south
					if(buf[i+1][j]==col && (!bCwayAware || (bCwayAware && (bC_s || bC_sw || bC_se )))){
						isGap=false;
						if(bSides
							&& (buf[i+1][j+1]!=col && buf[i+1][j-1]!=col)
							&& (buf[i+1][j+1]!=testfield && buf[i+1][j-1]!=testfield)
							&& (buf[i+2][j]==col))
						{
							colGapCount[buf[i+1][j+1]]++;
							colGapCount[buf[i+1][j-1]]++;
							isGap=true;
						}

						if(bSemis){
							if  ((buf[i][j+1]!=col && buf[i+1][j-1]!=col)
							&&  (buf[i][j+1]<250 && buf[i+1][j-1]<250))
							{
								colGapCount[buf[i][j+1]]++;
								colGapCount[buf[i+1][j-1]]++;
								isGap=true;
							}
							if ((buf[i+1][j+1]!=col && buf[i][j-1]!=col)
							&&  (buf[i+1][j+1]<250 && buf[i][j-1]<250))
							{
								colGapCount[buf[i+1][j+1]]++;
								colGapCount[buf[i][j-1]]++;
								isGap=true;
							}
						}
						if(bDiags){
							if	((buf[i][j+1]!=col && buf[i+2][j-1]!=col)
							&& (buf[i][j+1]<250 && buf[i+2][j-1]<250)
							&& (buf[i+1][j+1]==col && buf[i+2][j]==col))
							{
								colGapCount[buf[i][j+1]]++;
								colGapCount[buf[i+2][j-1]]++;
								isGap=true;
							}
							if	((buf[i+2][j+1]!=col && buf[i][j-1]!=col)
							&& (buf[i+2][j+1]<250 && buf[i][j-1]<250)
							&& (buf[i+2][j]==col && buf[i+1][j-1]==col))
							{
								colGapCount[buf[i+2][j+1]]++;
								colGapCount[buf[i][j-1]]++;
								isGap=true;
							}
						}
						if(isGap) buf[i+1][j]=252;
						else 	  buf[i+1][j]=254;
					}
					//west
					if(buf[i][j-1]==col && (!bCwayAware || (bCwayAware && (bC_w || bC_nw|| bC_sw )))){
						isGap=false;
						if(bSides
							&& (buf[i+1][j-1]!=col && buf[i-1][j-1]!=col)
							&& (buf[i+1][j-1]!=testfield && buf[i-1][j-1]!=testfield)
							&& (buf[i][j-2]==col))
							{
								colGapCount[buf[i+1][j-1]]++;
								colGapCount[buf[i+1][j-1]]++;
								isGap=true;
							}
						if(bSemis){
							if((buf[i+1][j]!=col && buf[i-1][j-1]!=col)
							&&  (buf[i+1][j]<250 && buf[i-1][j-1]<250))
							{
								colGapCount[buf[i+1][j]]++;
								colGapCount[buf[i-1][j-1]]++;
								isGap=true;
							}
							if((buf[i+1][j-1]!=col && buf[i-1][j]!=col)
							&&  (buf[i+1][j-1]<250 && buf[i-1][j]<250))
							{
								colGapCount[buf[i+1][j-1]]++;
								colGapCount[buf[i-1][j]]++;
								isGap=true;
							}
					   }
					   if(bDiags){
							if	((buf[i+1][j-2]!=col && buf[i-1][j]!=col)
							&& (buf[i+1][j-2]<250 && buf[i-1][j]<250)
							&& (buf[i][j-2]==col && buf[i-1][j-1]==col))

							{
								colGapCount[buf[i+1][j-2]]++;
								colGapCount[buf[i-1][j]]++;
								isGap=true;
							}
							if	((buf[i+1][j]!=col && buf[i-1][j-2]!=col)
							&& (buf[i+1][j]<250 && buf[i-1][j-2]<250)
							&& (buf[i+1][j-1]==col && buf[i][j-2]==col))
							{
								colGapCount[buf[i+1][j]]++;
								colGapCount[buf[i-1][j-2]]++;
								isGap=true;
							}
						}
					   if(isGap) buf[i][j-1]=252;
					   else 	 buf[i][j-1]=254;
					}
					//east
					if(buf[i][j+1]==col && (!bCwayAware || (bCwayAware && (bC_e || bC_ne || bC_se )))){
						isGap=false;
						if (bSides
						&& (buf[i+1][j+1]!=col && buf[i-1][j+1]!=col)
						&& (buf[i+1][j+1]!=testfield && buf[i-1][j+1]!=testfield)
						&& (buf[i][j+2]==col))
						{
						   colGapCount[buf[i+1][j+1]]++;
						   colGapCount[buf[i-1][j+1]]++;
						   isGap=true;
						}
						if(bSemis){
							if  ((buf[i+1][j]!=col && buf[i-1][j+1]!=col)
							&&  (buf[i+1][j]<250 && buf[i-1][j+1]<250))
							{
								colGapCount[buf[i+1][j]]++;
								colGapCount[buf[i-1][j+1]]++;
								isGap=true;
							}
							if((buf[i+1][j+1]!=col && buf[i-1][j]!=col)
							&&  (buf[i+1][j+1]<250 && buf[i-1][j]<250))
							{
								colGapCount[buf[i+1][j+1]]++;
								colGapCount[buf[i-1][j]]++;
								isGap=true;
							}
						}
                        if(bDiags){
							if	((buf[i+1][j+2]!=col && buf[i-1][j]!=col)
							&& (buf[i+1][j+2]<250 && buf[i-1][j]<250)
							&& (buf[i][j+2]==col && buf[i-1][j+1]==col))
							{
								colGapCount[buf[i+1][j+2]]++;
								colGapCount[buf[i-1][j]]++;
								isGap=true;
							}
							if	((buf[i+1][j]!=col && buf[i-1][j+2]!=col)
							&& (buf[i+1][j]<250 && buf[i-1][j+2]<250)
							&& (buf[i+1][j+1]==col && buf[i][j+2]==col))
							{
								colGapCount[buf[i+1][j]]++;
								colGapCount[buf[i-1][j+2]]++;
								isGap=true;
							}
						}
						if(isGap) buf[i][j+1]=252;
						else 	  buf[i][j+1]=254;
					}


					if(bFillFields)buf[i][j]=drawfield;
					else buf[i][j]=nodraw;
					}
					else{
					//normal mode

					if(bCway){
						if(bC_n && buf[i-1][j]==col) buf[i-1][j]=254;  else colGapCount[buf[i-1][j]]++;
						if(bC_s && buf[i+1][j]==col) buf[i+1][j]=254;  else colGapCount[buf[i+1][j]]++;
						if(bC_w && buf[i][j-1]==col) buf[i][j-1]=254;  else colGapCount[buf[i][j-1]]++;
						if(bC_e && buf[i][j+1]==col) buf[i][j+1]=254;  else colGapCount[buf[i][j+1]]++;

						if(bC_nw && buf[i-1][j-1]==col
						&& ((buf[i][j-1]==col || buf[i][j-1]==254) || (buf[i-1][j]==col || buf[i-1][j]==254)))
								buf[i-1][j-1]=254;
								else colGapCount[buf[i-1][j-1]]++;

						if(bC_ne && buf[i-1][j+1]==col
						&& ((buf[i][j+1]==col || buf[i][j+1]==254) || (buf[i-1][j]==col || buf[i-1][j]==254)))
								buf[i-1][j+1]=254;
								else colGapCount[buf[i-1][j+1]]++;

						if(bC_se && buf[i+1][j+1]==col
						&& ((buf[i][j+1]==col || buf[i][j+1]==254) || (buf[i+1][j]==col || buf[i+1][j]==254)))
								buf[i+1][j+1]=254;
								else colGapCount[buf[i+1][j+1]]++;

						if(bC_sw && buf[i+1][j-1]==col
						&& ((buf[i][j-1]==col || buf[i][j-1]==254) || (buf[i+1][j]==col || buf[i+1][j]==254)))
								buf[i+1][j-1]=254;
								else colGapCount[buf[i+1][j-1]]++;
					}
					else{

						if(buf[i-1][j]==col) buf[i-1][j]=254;  else colGapCount[buf[i-1][j]]++;
						if(buf[i+1][j]==col) buf[i+1][j]=254;  else colGapCount[buf[i+1][j]]++;
						if(buf[i][j-1]==col) buf[i][j-1]=254;  else colGapCount[buf[i][j-1]]++;
						if(buf[i][j+1]==col) buf[i][j+1]=254;  else colGapCount[buf[i][j+1]]++;

						if(b8way){
							if(buf[i-1][j-1]==col) buf[i-1][j-1]=254; else colGapCount[buf[i-1][j-1]]++;
							if(buf[i-1][j+1]==col) buf[i-1][j+1]=254; else colGapCount[buf[i-1][j+1]]++;
							if(buf[i+1][j+1]==col) buf[i+1][j+1]=254; else colGapCount[buf[i+1][j+1]]++;
							if(buf[i+1][j-1]==col) buf[i+1][j-1]=254; else colGapCount[buf[i+1][j-1]]++;

						}
					}

					buf[i][j]=253;
					}
					cnt++;
				}
			}
		}
		if(!cnt) break;

	}

	//second pass, fill gaps
	if(bForgiving) while(true)
	{
		cnt=0;
		
		for(i=offy;i<leny;i++)
		{
			for(j=offx;j<lenx;j++)
			{
				if(buf[i][j]==252)
				{
					//standard forgiving rules
					if(buf[i-1][j]==col)
						if(buf[i-1][j+1]!=col && buf[i-1][j-1]!=col &&  (buf[i][j]!=253 || buf[i][j]!=255))
							 buf[i-1][j]=252;

					if(buf[i+1][j]==col)
						if(buf[i+1][j+1]!=col && buf[i+1][j-1]!=col &&  (buf[i][j]!=253 || buf[i][j]!=255))
							 buf[i+1][j]=252;


					if(buf[i][j-1]==col)
						if(buf[i+1][j-1]!=col && buf[i-1][j-1]!=col && (buf[i][j]!=253 || buf[i][j]!=255))
							 buf[i][j-1]=252;


					if(buf[i][j+1]==col)
						if(buf[i+1][j+1]!=col && buf[i-1][j+1]!=col && (buf[i][j]!=253 || buf[i][j]!=255))
							 buf[i][j+1]=252;


					if(bFillGaps && buf[i][j] !=250) buf[i][j]=251;
					else buf[i][j]=250;

					cnt++;
				}
			}
		}
		if(!cnt) break;
	}

	//fill behaviour
	if(bFillFirstIteration || bForgiving){
		bordercol=0;
		int maxcnt=0;

		for(i=0;i<250;i++){
			if (colGapCount[i]>maxcnt) {
				 bordercol=i;
				maxcnt=colGapCount[i];
			}
		}

	replaceColField= bFieldPen?  bgPalCur:bordercol;
	replaceColGap=   bGapPen?    bgPalCur:bordercol;
	bFillFirstIteration=false;
	}
	if (penActive==0&&(!Shift.Contains(ssShift)))
	{
		for(i=offy;i<leny;i++)
		{
			for(j=offx;j<lenx;j++)
			{
				if(buf[i][j]==253) buf[i][j]=replaceColField;
				if(buf[i][j]==251) buf[i][j]=replaceColGap;
			}
		}
	}
	if (penActive==1||penActive==2||(Shift.Contains(ssShift)))
	{
		if (penActive==1||(Shift.Contains(ssShift)))
		{
			if(!Shift.Contains(ssAlt))fillPal++;
			else fillPal--;
		}
		else //penActive must be 2
		{
			if(!Shift.Contains(ssAlt))fillPal--;
			else fillPal++;
		}

		if(FormMain->IncDecCap1->Checked==true)
		{
			if (fillPal >3) fillPal = 3;
			if (fillPal <0) fillPal = 0;
		}

		for(i=offy;i<leny;i++)
		{
			for(j=offx;j<lenx;j++)
			{
				if(buf[i][j]==253 ) buf[i][j]=fillPal;
				if(buf[i][j]==251 ) buf[i][j]=fillPal;
			}
		}
	}

	if(isNametable){
		if(extpp<0) pp=tileViewTable[tileActive]*16+bankViewTable[set+tileActive];   //old typical CHR editor behaviour
		else pp=extpp;                             //typical tileset editor behaviour

		for(i=2;i<10;i++)
		{
			chr[pp]=0;
			chr[pp+8]=0;
			for(j=2;j<10;j++)
			{
				if(buf[i][j]<250){
					chr[pp]|=(buf[i][j]&1)<<(9-j);
					chr[pp+8]|=((buf[i][j]&2)>>1)<<(9-j);
				}
			}
			pp++;
		}
	}
	//new standard population behaviour
	else
	{
		for(int tx=sx;tx<sw+sx;tx++){
			for(int ty=sy;ty<sh+sy;ty++){

				for(int py=0;py<8;py++){
					pp=tileViewTable[tx + ty*16]*16 +bankViewTable[set + tx + ty*16]+py;
					chr[pp]=0;
					chr[pp+8]=0;
					for(int px=0;px<8;px++){
						if(buf[ty*8+py+margin][tx*8+px+margin]<250){
							chr[pp]|=(buf[ty*8+py+margin][tx*8+px+margin]&1)<<(7-px);
							chr[pp+8]|=((buf[ty*8+py+margin][tx*8+px+margin]&2)>>1)<<(7-px);
						}
					}
				}
			}
		}
	}
	cueCHRdrawAll=true;
}

//---------------------------------------------------------------------------
void __fastcall TFormCHREditor::Line(TShiftState Shift,int mdx, int mdy, int x,int y,int mode)
{
  globalLineSenderMode=mode;

  int set=bankActive/16;
  //bool bSmear = ((GetKeyState(VK_CAPITAL) & 0x0001)!=0);
  bool bSmear = FormLineDetails->btnSmear->Down;
  bool bMove  = FormLineDetails->btnMove->Down;
  //bool bReuse = FormLineDetails->btnReuse->Down;
  bool bQuick = FormLineDetails->btnQuick->Down;

  //bool b=btn2x2mode->Down?1:0;
  unsigned char* ptr;

  bool bTaperEnable = FormLineDetails->CheckEnableBrush->Checked;
  bool bTaperFromMid= ((FormLineDetails->btnTaperFromMid->Down) && (bTaperEnable));
  bool bTaperIn		= ((FormLineDetails->btnTaperIn->Down) && (bTaperEnable));
  bool bTaperOut	= ((FormLineDetails->btnTaperOut->Down) && (bTaperEnable));
  bool bTaper2		= (bTaperIn && bTaperOut);
  bool bBestBresen  = ((!bTaperFromMid) &&  (!bTaper2) && lineToolAlt==0); //.

  //bool bUseBrush	=FormLineDetails->CheckEnableBrush->Checked;
  bool bBrush=btnThick->Down;
  int iThick		=(bBrush)?iBrushSize[iBrushPresetIndex]:1;
  int ix,iy;
  int tmp=lineToolRoster;
  int pw=128; 								   //width of plotmask canvas
  //for plot to chr transfer
  int pp, bpp;
  int mask;
  int tempPal;
  //for plot
  int mask1,mask2;
  int plotmask;
  //used for simple line/taper in/out
  int x0,x2,y0,y2; //used by application mask plotter
  int px0,px2,py0,py2;

  //used for brush
  int br_x0[16][16];
  int br_y0[16][16];
  int br_x2[16][16];
  int br_y2[16][16];

  // derived from 0 and 2 above, this done to perform taper rules independently
  int br_x1[16][16];
  int br_y1[16][16];

  //maybe needed, we'll see
  int tx0,tx1,ty0,ty1;

  int pp0, pp1;

  //refresh buffers
  //memcpy (chr, chrBuf, 8192);
  for(int tile=0;tile<256;tile++){
	int tmp_pp=tileViewTable[tile]*16+bankViewTable[tile+set];
	memcpy(chr+tmp_pp,chrBuf+tileViewTable[tile]*16,16);
  }


  //memset (,0,sizeof(arr_linePlotCHR));
  memcpy (arr_linePlotCHR,arr_linePlotCHR2,sizeof(arr_linePlotCHR2));
  //if(bBufVK_3) goto test;

  for(iy=0; iy<iThick; iy++)
	{
	for(int ix=0; ix<iThick; ix++)
		{
		//if(bBrushMask[i]==false) continue;
		if((*ptr_tableBrush[iBrushPresetIndex])[ix][iy]==false) continue;

		//figure out points - cursor to pixel
		if(mode==0) //CHR Editor mode
		//---------------------------
		{
			FormCHREditor->SpeedButtonHFlip->Enabled=false;
			FormCHREditor->SpeedButtonVFlip->Enabled=false;
			FormCHREditor->SpeedButtonRotateCW->Enabled=false;
			FormCHREditor->SpeedButtonRotateCCW->Enabled=false;



			int o=(lineOffX)*16;
			int p=(lineOffY)*16;

			px0=(((mdx-(64+8))/16)+lineOffX)&7;
			py0=(((mdy-(64+8))/16)+lineOffY)&7;
			px2=((x-(64+8))/16)&7;
			py2=((y-(64+8))/16)&7;

			if(btnQuant->Down){px0=px0&14; py0=py0&14;	px2=px2&14; py2=py2&14;}


			//pixel in a tile	tile in editor	   		tile offset
			x0 = (px0) 			+ ((mdx+o-(64+8))/128)*8     + (tileActive&15)*8;

			y0 = (py0) 			+ ((mdy+p-(64+8))/128)*8	+ (tileActive/16)*8;
			x2 = (px2) 			+ ((x-(64+8))/128)*8 		+ (tileActive&15)*8;
			y2 = (py2) 			+ ((y-(64+8))/128)*8	    + (tileActive/16)*8;


			if(bMove){


				if(bOldLine){

					mvOffX=((x-(64+8))/16)-((mvOriginX-(64+8))/16);
					mvOffY=((y-(64+8))/16)-((mvOriginY-(64+8))/16);
					x0=mvOffX;
					y0=mvOffY;
				}
				else
				{
					mvOffX=((x-(64+8))/16)-((mvOriginX-(64+8))/16);
					mvOffY=((y-(64+8))/16)-((mvOriginY-(64+8))/16);
					x0+=mvOffX;//-x_dist;
					y0+=mvOffY;//-y_dist;
				}
				CHR_moveOriginX=mdx+mvOffX*16; //64+8+(x0*16);
				CHR_moveOriginY=mdy+mvOffY*16; //64+8+(y0*16);
			}
			CHR_mmX=x; //talks to move toggle.
			CHR_mmY=y;
		}
		if(mode==1) //Tileset canvas mode
		//-------------------------------
		{

			//pixel in a tile	tile
			x0 = (mdx+0.5)/uiScale+lineOffX;
			y0 = (mdy+0.5)/uiScale+lineOffY;
			x2 = (x+0.5)/uiScale;
			y2 = (y+0.5)/uiScale;

			if(btnQuant->Down){x0=x0&126; y0=y0&126;	x2=x2&126; y2=y2&126;}

			if(bMove){
				if(bOldLine){
				   mvOffX= (x)/uiScale -(mvOriginX)/uiScale;
				   mvOffY= (y)/uiScale -(mvOriginY)/uiScale;
				   x0=mvOffX;
				   y0=mvOffY;
			}
			else{
				mvOffX=(x)/uiScale -(mvOriginX)/uiScale;
				mvOffY=(y)/uiScale -(mvOriginY)/uiScale;
				x0+=mvOffX;
				y0+=mvOffY;
			}
			CHR_moveOriginX=((mdx+(x-mvOriginX))/uiScale)*uiScale;
			CHR_moveOriginY=((mdy+(y-mvOriginY))/uiScale)*uiScale;
		}
		CHR_mmX=((x+0.5)/uiScale)*uiScale;
		CHR_mmY=((y+0.5)/uiScale)*uiScale;
	}

	//set up brush lines
	//------------------


	//used by split line and bezier curve.
	int bix = 0;
	int biy = 0;

	//if (bBrush) bi=-1;
	if (lineToolRoster==0) {bix = 0; biy= 0;}

	//aligment adjustment of line strokes:
	int tmpTag=iBrushCursorAlignment[iBrushPresetIndex];
	int tmpOff=iThick-1;
	int eix,eiy;            //endpoint offsets

	if(tmpTag==0){ 	eix=ix; 			eiy=iy;}  	//center right
	if(tmpTag==1){ 	eix=ix-tmpOff/2;	eiy=iy;}  			//top center
	if(tmpTag==2){ 	eix=ix-tmpOff; 		eiy=iy;}  			//top right

	if(tmpTag==3){ 	eix=ix;	   			eiy=iy-tmpOff/2;}  	//center left
	if(tmpTag==4){ 	eix=ix-tmpOff/2; 	eiy=iy-tmpOff/2;}  	//center-center
	if(tmpTag==5){ 	eix=ix-tmpOff; 		eiy=iy-tmpOff/2;}  	//center right

	if(tmpTag==6){ 	eix=ix;				eiy=iy-tmpOff;}  	//bottom left
	if(tmpTag==7){ 	eix=ix-tmpOff/2;	eiy=iy-tmpOff;}  	//bottom center
	if(tmpTag==8){ 	eix=ix-tmpOff; 		eiy=iy-tmpOff;}  	//bottom right

	/*
	if(bTaper2) {
		bix = eix;
		biy = eiy;
	}

	if((bTaperFromMid) && ((!bTaperIn) && (!bTaperOut))) {bix=0-eix; biy=0-eiy;}
	*/


	br_x0[ix][iy] = x0 + (bTaperIn? 0 : eix);
	br_y0[ix][iy] = y0 + (bTaperIn? 0 : eiy);
	br_x2[ix][iy] = x2 + (bTaperOut?  0 : eix);
	br_y2[ix][iy] = y2 + (bTaperOut?  0 : eiy);



	//used by bresenham angles

	//no taper
	if((!bTaperIn) && (!bTaperOut)){
		//bix = eix;
		//biy = eiy;
		if(tmp>=0){
			br_x1[ix][iy] = x0 +eix;
			br_y1[ix][iy] = y2 +eiy;
		}
		if(tmp<0){

			br_x1[ix][iy] = x2 +eix;
			br_y1[ix][iy] = y0 +eiy;
		}
	}
	//2 tapers, no taper-from-mid
	else if(!bTaperFromMid && (bTaper2)){
		bix = eix;
		biy = eiy;

		if(tmp>=0){
			br_x1[ix][iy] = x0 +eix;
			br_y1[ix][iy] = y2 +eiy;
		}
		if(tmp<0){
			br_x1[ix][iy] = x2 +eix;
			br_y1[ix][iy] = y0 +eiy;
		}
	}
	//taper from mid, 1 taper
	else if(bTaperFromMid && (!bTaper2)){
		bix = eix;
		biy = eiy;

		if(tmp>=0){
			br_x1[ix][iy] = x0 +eix;
			br_y1[ix][iy] = y2 +eiy;
		}
		if(tmp<0){
			br_x1[ix][iy] = x2 +eix;
			br_y1[ix][iy] = y0 +eiy;
		}
	}

	//inverted taper
	else if((bTaperFromMid) && (bTaper2)){
		 br_x0[ix][iy] = x0 + (eix);
		 br_y0[ix][iy] = y0 + (eiy);
		 br_x2[ix][iy] = x2 + (eix);
		 br_y2[ix][iy] = y2 + (eiy);

		 bix = 0-eix;
		 biy = 0-eiy;

		if(tmp>0){
			br_x1[ix][iy] = x0;
			br_y1[ix][iy] = y2;
		}
		else if(tmp<0){
			br_x1[ix][iy] = x2;
			br_y1[ix][iy] = y0;
		}

	}
	//1 taper, no "from mid"
	else {
		bix = eix/2;
		biy = eiy/2;
		if(tmp>=0){
		br_x1[ix][iy] = x0 +eix/2;
		br_y1[ix][iy] = y2 +eiy/2;
		}
		if(tmp<0){
			br_x1[ix][iy] = x2 +eix/2;
			br_y1[ix][iy] = y0 +eiy/2;
		}

	}


	ptr = arr_linePlotCHR;

	pw=128;
	bool bHyperAuto = FormLineDetails->btnAutoSizeHyperY->Down;
	//call appropriate plot
	int hbx,hby;
	switch (tmp) {
		case 8:
			 hbx = (br_x0[ix][iy]+br_x2[ix][iy])/2+lineToolX;
			 hby = (br_y0[ix][iy]+br_y2[ix][iy])/2-7+lineToolY_toggleperm;
			 if(bHyperAuto) hby = (br_y0[ix][iy]+br_y2[ix][iy])/2 - abs(br_x0[ix][iy]-br_x2[ix][iy])/2    +lineToolY_toggleperm;

			 Hyperbola_curve(br_x0[ix][iy], br_y0[ix][iy], hbx, hby, br_x2[ix][iy], br_y2[ix][iy], pw, ptr, bix, biy, 7);
			 FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterHypercave->Top;
			 break;
		case 7:
			 hbx = (br_x0[ix][iy]+br_x2[ix][iy])/2+lineToolX;
			 hby = (br_y0[ix][iy]+br_y2[ix][iy])/2-7+lineToolY_toggleperm;
			 if(bHyperAuto) hby = (br_y0[ix][iy]+br_y2[ix][iy])/2 - abs(br_x0[ix][iy]-br_x2[ix][iy])/2    +lineToolY_toggleperm;

			 Hyperbola_curve(br_x0[ix][iy], br_y0[ix][iy], hbx, hby, br_x2[ix][iy], br_y2[ix][iy], pw, ptr, bix, biy, 6);
			 FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterHyperline->Top;
			 break;
		case 6:
			 hbx = (br_x0[ix][iy]+br_x2[ix][iy])/2+lineToolX;
			 hby = (br_y0[ix][iy]+br_y2[ix][iy])/2-7+lineToolY_toggleperm;
			 if(bHyperAuto) hby = (br_y0[ix][iy]+br_y2[ix][iy])/2 - abs(br_x0[ix][iy]-br_x2[ix][iy])/2    +lineToolY_toggleperm;

			 Hyperbola_curve(br_x0[ix][iy], br_y0[ix][iy], hbx, hby, br_x2[ix][iy], br_y2[ix][iy], pw, ptr, bix, biy, 5);
			 FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterHyperbola->Top;
			 break;
        case 5:
			 Line_Circle(br_x0[ix][iy], br_y0[ix][iy], br_x2[ix][iy], br_y2[ix][iy], pw, ptr);
			 FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterRectangle->Top;
			 break;

		case 4:
			Bresenham_rect(br_x0[ix][iy], br_y0[ix][iy], br_x2[ix][iy], br_y2[ix][iy], pw, ptr);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterRectangle->Top;
			break;
		case 3:
			Bresenham_angle(br_x0[ix][iy], br_y0[ix][iy],   br_x1[ix][iy]+lineToolX, br_y1[ix][iy]+lineToolY,   br_x2[ix][iy], br_y2[ix][iy], pw, ptr,false,true,bix,biy);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterAngle->Top;
			break;
		case 2:
			Bresenham_angle(br_x0[ix][iy], br_y0[ix][iy],   br_x1[ix][iy]+lineToolAlt+lineToolX, br_y1[ix][iy]+lineToolAlt+lineToolY,   br_x2[ix][iy], br_y2[ix][iy], pw, ptr,true,true,bix,biy);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterKnee->Top;
			break;
		case 1:
			//if(mode==1) plotQuadBezier(br_x0[i], br_y0[i],   br_x0[i]+lineToolAlt+lineToolX, br_y2[i]+lineToolAlt+lineToolY,   br_x2[i], br_y2[i], pw, ptr, bi);
			//else
			Bezier_curve(br_x0[ix][iy], br_y0[ix][iy],   br_x0[ix][iy]+lineToolAlt+lineToolX, br_y2[ix][iy]+lineToolAlt+lineToolY,   br_x2[ix][iy], br_y2[ix][iy], pw, ptr, bix,biy);


			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterCurve->Top;
			break;
		case -1:

			Bezier_curve(br_x0[ix][iy], br_y0[ix][iy],   	  br_x2[ix][iy]-lineToolAlt+lineToolX, br_y0[ix][iy]-lineToolAlt+lineToolY,   br_x2[ix][iy], br_y2[ix][iy], pw, ptr, bix,biy);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterCurve->Top;
			break;
		case -2:
			Bresenham_angle(br_x0[ix][iy], br_y0[ix][iy],   br_x1[ix][iy]-lineToolAlt+lineToolX, br_y1[ix][iy]-lineToolAlt+lineToolY,   br_x2[ix][iy], br_y2[ix][iy], pw, ptr,true,true,bix,biy);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterKnee->Top;
			break;
		case -3:
			Bresenham_angle(br_x0[ix][iy], br_y0[ix][iy],   br_x1[ix][iy]+lineToolX, br_y1[ix][iy]+lineToolY,   br_x2[ix][iy], br_y2[ix][iy], pw, ptr,false,true,bix,biy);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterAngle->Top;
			break;
		case -4:
			Bresenham_rect(br_x0[ix][iy], br_y0[ix][iy], br_x2[ix][iy], br_y2[ix][iy], pw, ptr);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterRectangle->Top;
			break;
		case -5:
			Line_Circle(br_x0[ix][iy], br_y0[ix][iy], br_x2[ix][iy], br_y2[ix][iy], pw, ptr);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterRectangle->Top;
			break;
		case -6:
			 hbx = (br_x0[ix][iy]+br_x2[ix][iy])/2+lineToolX;
			 hby = (br_y0[ix][iy]+br_y2[ix][iy])/2+7-lineToolY_toggleperm;
			 if(bHyperAuto) hby = (br_y0[ix][iy]+br_y2[ix][iy])/2 + abs(br_x0[ix][iy]-br_x2[ix][iy])/2    -lineToolY_toggleperm;

			 Hyperbola_curve(br_x0[ix][iy], br_y0[ix][iy], hbx, hby, br_x2[ix][iy], br_y2[ix][iy], pw, ptr, bix, biy,5);
			 FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterHyperbola->Top;
			 break;
		case -7:
			 hbx = (br_x0[ix][iy]+br_x2[ix][iy])/2+lineToolX;
			 hby = (br_y0[ix][iy]+br_y2[ix][iy])/2+7-lineToolY_toggleperm;

			 if(bHyperAuto) hby = (br_y0[ix][iy]+br_y2[ix][iy])/2 + abs(br_x0[ix][iy]-br_x2[ix][iy])/2    -lineToolY_toggleperm;

			 Hyperbola_curve(br_x0[ix][iy], br_y0[ix][iy], hbx, hby, br_x2[ix][iy], br_y2[ix][iy], pw, ptr, bix, biy,6);
			 FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterHyperline->Top;
			 break;
		case -8:
			 hbx = (br_x0[ix][iy]+br_x2[ix][iy])/2+lineToolX;
			 hby = (br_y0[ix][iy]+br_y2[ix][iy])/2+7-lineToolY_toggleperm;
			 if(bHyperAuto) hby = (br_y0[ix][iy]+br_y2[ix][iy])/2 + abs(br_x0[ix][iy]-br_x2[ix][iy])/2   -lineToolY_toggleperm;

			 Hyperbola_curve(br_x0[ix][iy], br_y0[ix][iy], hbx, hby, br_x2[ix][iy], br_y2[ix][iy], pw, ptr, bix, biy,7);
			 FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterHypercave->Top;
			 break;

		default:
			if(bBestBresen)
			{
				//when a midpoint is not of use, a simple single line
				//is better, since this routine is then able to even the
				//inclination steps, which is often sought after in pixel art.

				Bresenham_line(br_x0[ix][iy]+lineToolX, br_y0[ix][iy]+lineToolY, br_x2[ix][iy], br_y2[ix][iy], pw, ptr,false);
			}
			else
			{
			  //less perfect, but OK for when a midpoint is required.
			  //can look a little odd when the line is a hypothenuse and the opposite and adjacent measures are odd.
			  //the errors that remain are somewhat obscured by the fact that is is only used
			  //when taper combos are used.
			  Bresenham_2line(	br_x0[ix][iy]+lineToolX, br_y0[ix][iy]+lineToolY,
								(x0+x2)/2, (y0+y2)/2,
								br_x2[ix][iy], br_y2[ix][iy], pw, ptr, bix,biy);
			}
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterLine->Top;
		}
	}}

  FormLineDetails->LineToolIndicator->Top+1;
  if(tmp>=0) {
	if(tmp>=5) FormLineDetails->LineToolIndicator->Caption="_z";
	else       FormLineDetails->LineToolIndicator->Caption="`z";
  }
  else {
   if(tmp>= -5)		FormLineDetails->LineToolIndicator->Caption="`y";
   else             FormLineDetails->LineToolIndicator->Caption="_y";
  }

  //Bezier_fine(x0, y0,   x0, y1,   x1, y1, pw, arr_linePlotCHR);

  FormMain->LabelStats->Caption=
  "x0,y0: "+IntToStr(x0)+","+IntToStr(y0)+"\tx2,y2: "+IntToStr(x2)+","+IntToStr(y2);
  FormMain->StatusUpdateWaiter->Enabled=true;
  holdStats=true;

  //transfer plot application mask to chr
  //.....................................

  //get update range
  int sx,sy,w,h;
  FormMain->GetSelection(chrSelection,sx,sy,w,h);


  //perform

  //smear OR move
  if(bSmear){memcpy (arr_linePlotCHR2,arr_linePlotCHR,sizeof(arr_linePlotCHR2));}
  if(bQuick){
	//memcpy (chrQuickLine,chr, 8192);}
	for(int tile=0;tile<256;tile++){
		int tmp_pp=tileViewTable[tile]*16+bankViewTable[tile+set];
			memcpy(chrQuickLine+tile*16,chr+tmp_pp,16);
		}
  }
  int tx;
  int ty;
  if(mode==1)
  {
	if (w<=1) {w=16; sx=0;}
	if (h<=1) {h=16; sy=0;}
  }

  for(tx=0;tx<w;tx++){
	for(ty=0;ty<h;ty++){
		for(int py=0;py<8;py++){
			for(int px=0;px<8;px++){




				pp=tileViewTable[sx+tx+(sy+ty)*16]*16 + bankViewTable[set+ sx+tx+(sy+ty)*16]+py;
				bpp=tileViewTable[sx+tx+(sy+ty)*16]*16 +py;

				mask=128>>px;

				int plotoff = (sx+tx)*8 + ((sy+ty)*8)*pw + px + (py)*pw;


				if((plotoff <0) || plotoff>sizeof(arr_linePlotCHR)) continue;

				plotmask =  arr_linePlotCHR[plotoff];


				tempPal=(((chrBuf[bpp]<<px)&128)>>7)|(((chrBuf[bpp+8]<<px)&128)>>6);

				if(FormMain->Applytopen2->Checked){
					if(Protect0->Down && tempPal==0) continue;
					if(Protect1->Down && tempPal==1) continue;
					if(Protect2->Down && tempPal==2) continue;
					if(Protect3->Down && tempPal==3) continue;
				}
				//todo - somehow include mask from plot
				//if (penActive==0&&(!Shift.Contains(ssShift))&&plotmask==1)
				if ((penActive==0&&(!bBufShift))&&plotmask==1)
				{
					if(!bSmudge){
								if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
								if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
							}
							else{
								if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((iSmudge&1)<<7)>>px);
								if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((iSmudge&2)<<6)>>px);
                           }
				}
				//if (penActive==1||(Shift.Contains(ssShift))&&plotmask==1)
				if ((penActive==1||(bBufShift))&&plotmask==1)
				{
					if(!bBufAlt)tempPal++;
					else tempPal--;

					if(FormMain->IncDecCap1->Checked==true)
					{
						if (tempPal >3) tempPal = 3;
						if (tempPal <0) tempPal = 0;
					}


					if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((tempPal&1)<<7)>>px);
					if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((tempPal&2)<<6)>>px);

				}
				if (penActive==2&&plotmask==1)
				{
					if(!bBufAlt)tempPal--;
					else tempPal++;

					if(FormMain->IncDecCap1->Checked==true)
					{
						if (tempPal >3) tempPal = 3;
						if (tempPal <0) tempPal = 0;
					}

					if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((tempPal&1)<<7)>>px);
					if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((tempPal&2)<<6)>>px);
				}
			}
		}
	}
  }
  cueCHRdrawAll=true;
}

void __fastcall TFormCHREditor::TileChange(int xoff,int yoff)
{
	int tx,ty;
	bool b=btn2x2mode->Down;
	int n=b?14:15;

	tx=(tileActive&15)+xoff;
	ty=(tileActive/16)+yoff;

	if(tx<0||tx>n||ty<0||ty>n) return;

	FormMain->SetTile(tileActive+=(yoff*16+xoff));
	if(b) {}

	Draw(true);
}
int __fastcall TFormCHREditor::GetNeighborTile(int xoff,int yoff)
{
	int ttx,tty;
	bool b=btn2x2mode->Down;
	int n=b?14:15;

	ttx=(tileActive&15)+xoff;
	tty=(tileActive/16)+yoff;

	if(ttx<0||ttx>n||tty<0||tty>n) return tileActive;
	return (tileActive+=(yoff*16+xoff));
}
//---------------------------------------------------------------------------
__fastcall TFormCHREditor::TFormCHREditor(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormCHREditor::PaintBoxChrPaint(TObject *Sender)
{
	Draw(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::PaintBoxChrMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//added here in addition to keydown and keyup.
    //there was a context that wouldm't register properly without.
	unsigned int set = bankActive/16;
	unsigned int pp;
	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;
	bool bQuick = FormLineDetails->btnQuick->Down;

	bool b=btn2x2mode->Down?1:0;
    int f=1;
	if(N2x21->Checked) f=1;
	if(N3x31->Checked) f=2;
	if(N4x41->Checked) f=3;
	int n=b?128*f:0;
	int hi=64+8+128+n;
	int lo=64+8;
	int cm=3;   //tilechange margin generostiy
	int pm=4;   //paint area margin generosity

	//left click actions + noncontiguous fill
	if((!Shift.Contains(ssRight))
	|| (Shift.Contains(ssRight) && bBufCtrl && bBufAlt) ){
		FormMain->SetUndo();
	}


	//if(!Shift.Contains(ssRight)){
		if(Y<64+cm)
		{
			if(X<64+cm) TileChange(-1,-1);
			if(X>=64+8-pm&&X<64+8+128+n+pm) TileChange(0,-1);
			if(X>=64+8+128+8+n-cm) TileChange(1,-1);
			bForbidPaint=true;
		}

		if(Y>=64+8-pm&&Y<64+8+128+n+pm)
		{
			if(X<64+cm) TileChange(-1,0);
			if(X>=64+8+128+8+n-cm) TileChange(1,0);
			bForbidPaint=true;
		}

		if(Y>=64+8+128+8+n-cm)
		{
			if(X<64+cm) TileChange(-1,1);
			if(X>=64+8-pm&&X<64+8+128+n+pm) TileChange(0,1);
			if(X>=64+8+128+8+n-cm) TileChange(1,1);
			bForbidPaint=true;
		}
	//}

	if(Y>=lo-pm && Y<hi+pm && X>=lo-pm && X<hi+pm)
	{


		lineDrawing = ((btnLine->Down)&& (!Shift.Contains(ssCtrl)) );
		if(lineDrawing && !bQuick) FormLineDetails->btnMove->Enabled=true;

		if((!lineDrawing) || !bQuick){
			//memcpy (chrBuf, chr, 8192); //preps a referencepoint for brushstrokes
			for(int tile=0;tile<256;tile++){
				pp=tileViewTable[tile]*16+bankViewTable[tile+set];
				memcpy(chrBuf+tileViewTable[tile]*16,chr+pp,16);
			}
			memcpy (chrBufFill, chrBuf, 4096);
		}
		if(bQuick || (Shift.Contains(ssCtrl)&&!bQuick))
		{
		   if(lineUpX<0) {
				//memcpy (chrBuf, chr, 8192);
				for(int tile=0;tile<256;tile++){
					pp=tileViewTable[tile]*16+bankViewTable[tile+set];
					memcpy(chrBuf+tileViewTable[tile]*16,chr+pp,16);
				}
		   }
		   else {
				//memcpy (chr, chrBuf, 8192);       //chrQuickLine
				for(int tile=0;tile<256;tile++){
					pp=tileViewTable[tile]*16+bankViewTable[tile+set];
					memcpy(chr+pp,chrBuf+tileViewTable[tile]*16,16);
				}
		   }
		   //memcpy (chrBufFill, chr, 8192);
		   for(int tile=0;tile<256;tile++){
					pp=tileViewTable[tile]*16+bankViewTable[tile+set];
					memcpy(chrBufFill+tileViewTable[tile]*16,chr+pp,16);
				}
		}



		if(bQuick)
		{
			bOldLine=true;   //user wants to retouch the position of old line
			 if(lineUpX<0) lineUpX= X;
			 if(lineUpY<0) lineUpY= Y;
			lineDownX=lineUpX;
			lineDownY=lineUpY;
			lineDownX= (lineUpX > lo)? lineUpX:lo;
			lineDownX= (lineUpX < hi)? lineUpX:hi-1;
			lineDownY= (lineUpY > lo)? lineUpY:lo;
			lineDownY= (lineUpY < hi)? lineUpY:hi-1;

		}
		else{

			bOldLine=false;
			if(bBufVK_3) memcpy(arr_linePlotCHR2,arr_linePlotCHR,sizeof(arr_linePlotCHR2));
			else memset (arr_linePlotCHR2,0,sizeof(arr_linePlotCHR2));
			lineDownX=X;
			lineDownY=Y;
			lineDownX= (X > lo)? X:lo;
			lineDownX= (X < hi)? X:hi-1;
			lineDownY= (Y > lo)? Y:lo;
			lineDownY= (Y < hi)? Y:hi-1;
		}


		lineToolAlt=0;
		lineToolX=0;
        lineToolY=0;
		bForbidPaint=false;
		lineDrawing = ((btnLine->Down)&&(!Shift.Contains(ssCtrl)));
		if(!lineDrawing) {lineUpX= -1; lineUpY= -1;}
		if(FormLineDetails->CheckResetLineNudge){
			lineOffX=0;
			lineOffY=0;
		}
		if(FormLineDetails->btnResetLine->Down)
		{
			lineToolRoster=0;
			

			for (int i = 0; i < 3; i++) {
				if (lineRosterEnable[lineToolRoster]==false) lineToolRoster++;
			}
			if (lineToolRoster > 3) lineToolRoster = 3;
		}

		if(bSmudge ||(bBufCtrl && bBufAlt))   //smudge or noncontiguous fill
		{

			int tx=(X-(64+8))/128;
			int ty=(Y-(64+8))/128*16;
			int px=((X-(64+8))/16)&7;
			int py=((Y-(64+8))/16)&7;
			if(btnQuant->Down){px=px&14; py=py&14;}

			int pp=tileViewTable[tileActive+tx+ty]*16+bankViewTable[set+tileActive+tx+ty]+py;


			iSmudge=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);


		}
		bIsFilling=true;
		bFillFirstIteration=true;

		if(FormMain->AntiJagSubtle1->Checked) ptr_pixelperfectmask=&perfect_pixel_mask_subtle[0][0];
		else if(FormMain->AntiJagLight1->Checked) ptr_pixelperfectmask=&perfect_pixel_mask_lite[0][0];
		else if(FormMain->AntiJagMLight1->Checked) ptr_pixelperfectmask=&perfect_pixel_mask_mlite[0][0];
		else if(FormMain->AntiJagMedium1->Checked) ptr_pixelperfectmask=&perfect_pixel_mask_medium[0][0];
		else if(FormMain->AntiJagMediumHeavy1->Checked) ptr_pixelperfectmask=&perfect_pixel_mask_heavy[0][0];
		else if(FormMain->AntiJagQHeavy1->Checked) ptr_pixelperfectmask=&perfect_pixel_mask_qheavy[0][0];
		//else if(FormMain->AntiJagTiltedpen1->Checked) ptr_pixelperfectmask=&perfect_pixel_mask_diagonal1[0][0];

		PaintBoxChrMouseMove(Sender,Shift,X,Y);
	}
	else{ lineUpX= -1; lineUpY= -1;}
	
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::PaintBoxChrMouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{

	if(bCustomBucketLoadSuccess && Shift.Contains(ssCtrl)){
		Screen->Cursor=(TCursor)const_crCustomNormalBucket;
	} else  Screen->Cursor=crDefault;

	int bpp;
	int set = bankActive/16;
	if(bForbidPaint) return;
	bool b=btn2x2mode->Down?1:0;


	int f=1;
	if(N2x21->Checked) f=1;
	if(N3x31->Checked) f=2;
	if(N4x41->Checked) f=3;
	int n=b?128*f:0;
	int hi=64+8+128+n;
	int lo=64+8;

	int pm=2;   //paint area margin generosity
	int ftx=0;
	int fty=0;
	int fpx,fpy;

	CHR_mmX=X;       //used for line adjustments by keyboard
	CHR_mmY=Y;
	CHR_scrollX=X;   //used for refreshing line when scrolling the wheel
	CHR_scrollY=Y;

	int px,py,pp,bufpp,mask;
	int tx,ty;
	int ix,iy; //pixel modifiers for thick pen
	int gx,gy,g; //guards for thick pen
	int iThick=btnThick->Down?iBrushSize[iBrushPresetIndex]:1;
	int tempPal;


	CHREditorHover=0; //this can be repurposed later for identifying pixels and memory
					  //for now it just acts to give stats intel.
	cueStats=true;

	if((Y>=lo-pm && Y<hi+pm && X>=lo-pm && X<hi+pm) || (lineDrawing))
	{

		//if(lineDrawing)


		X= (X >= lo)? X:lo;
		X= (X < hi)? X:hi-1;
		Y= (Y >= lo)? Y:lo;
		Y= (Y < hi)? Y:hi-1;



		//tx=X<64+8+128?0:1;
		//ty=Y<64+8+128?0:16;
		tx=(X-(64+8))/128;
		ty=(Y-(64+8))/128*16;
		//used by fill

		//ftx=X<64+8+128?0:8;
		//fty=Y<64+8+128?0:8;
		ftx=((X-(64+8))/128)*8;
		fty=((Y-(64+8))/128)*8;


		px=((X-(64+8))/16)&7;
		py=((Y-(64+8))/16)&7;
		if(btnQuant->Down){px=px&14; py=py&14;}
		fpx=px;
		fpy=py;

		//pp=tileActive*16+py+bankActive+tx+ty;
		//pp=tileViewTable[tileActive+tx+ty]*16+bankActive+py;
		pp=tileViewTable[tileActive+tx+ty]*16+bankViewTable[set+tileActive+tx+ty]+py;
		bpp=tileViewTable[tileActive+tx+ty]*16+py;

		mask=128>>px;

		if(Shift.Contains(ssLeft))
		{
			 //test: refresh line buffer here

			 //if(btnLine->Down)	Line(Shift,lineDownX,lineDownY,X,Y,0);
			 if(lineDrawing)
			 {
				 Line(Shift,lineDownX,lineDownY,X,Y,0);
			 }
			 else
			 {
			 //int d = sqrt(iThick);
			 for(int br_y=0; br_y<iThick; br_y++)
			 {
			 for(int br_x=0; br_x<iThick; br_x++)
			 {
				if(btnThick->Down)
				{
					//if(bBrushMask[i]==false) continue;
					if((*ptr_tableBrush[iBrushPresetIndex])[br_x][br_y]==false) continue;
                    if(!bFillFirstIteration && FormMain->AntiJagOn1->Checked){
							if(ptr_pixelperfectmask[((Y-(64+8))%16)*16 + ((X-(64+8))%16)]==1) return;
					}
					bFillFirstIteration=false;

					ix= br_x*16 -(float)(brush_x_anchor*16.0);
					iy= br_y*16 -(float)(brush_y_anchor*16.0);

					//tx=		X+ix <	64+8+128 ?0:1;
					//ty=		Y+iy <	64+8+128 ?0:16;
					tx=		(X+ix-(64+8))/128;
					ty=		(Y+iy-(64+8))/128*16;
					px=((X-(64+8)+ix)/16)&7;
					py=((Y-(64+8)+iy)/16)&7;

                    //g stands for guard, makes sure we don't draw outside.
					gx=((X-(64+8)+ix)/16);
					gy=((Y-(64+8)+iy)/16);

					if(b)g=((1+f)*8)-1; else g=7;

					if(!btnQuant->Down){
						//if((i==1||3)&&gx>g) continue;
						//if((i==2||3)&&gy>g) continue;
						if(gx<0) continue;
						if(gy<0) continue;
						if(gx>g) continue;
						if(gy>g) continue;
					}
					if(btnQuant->Down){


						//px=((X-(64+8))/16)&6;
						//py=((Y-(64+8))/16)&6;
						int qsx = iBrushSnapSize_x[iBrushPresetIndex];
						int qsy = iBrushSnapSize_y[iBrushPresetIndex];

						px=(((((X-(64+8))/16))/ qsx)*qsx);
						py=(((((Y-(64+8))/16))/ qsy)*qsy);

						//tx=X<64+8+128	?0:16;
						//ty=Y<64+8+128	?0:16*16;


						px+=br_x;
						py+=br_y;
						tx=(px)<8	?0:1;
						ty=(py)<8	?0:16;

						px&=7;
						py&=7;

					}


					bpp=tileViewTable[tileActive+tx+ty]*16+py;
					pp=tileViewTable[tileActive+tx+ty]*16+bankViewTable[set+tileActive+tx+ty]+py;
					//pp=tileActive*16+py+bankActive+tx+ty;
					//pp=tileViewTable[tileActive+tx+ty]*16+bankActive+py;


					mask=128>>px;

				}



					tempPal=(((chrBuf[bpp]<<px)&128)>>7)|(((chrBuf[bpp+8]<<px)&128)>>6);
					fillPal=(((chrBufFill[bpp]<<px)&128)>>7)|(((chrBufFill[bpp+8]<<px)&128)>>6);



					if(FormMain->Applytopen2->Checked){
						if(Protect0->Down && tempPal==0) continue;
						if(Protect1->Down && tempPal==1) continue;
						if(Protect2->Down && tempPal==2) continue;
						if(Protect3->Down && tempPal==3) continue;
					}
					//determine if inc/dec inks should be per click or continous
					if(FormMain->IncDecPerclick1->Checked==false)
					{
						tmpContinousIncDecTimer++;
						if (tmpContinousIncDecTimer>continousIncDecDuration)
						{
							tmpContinousIncDecTimer=0;
							//memcpy (chrBuf, chr, 8192);
							for(int tile=0;tile<256;tile++){
								int tmp_pp=tileViewTable[tile]*16+bankViewTable[tile+set];
								memcpy(chrBuf+tileViewTable[tile]*16,chr+tmp_pp,16);
							}
						}
					}

					if(!Shift.Contains(ssCtrl))
					{
						//we're reusing this bool for pen to discern if we should improve diagonal pixel drawing
						if(!bFillFirstIteration && FormMain->AntiJagOn1->Checked){
							if(ptr_pixelperfectmask[((Y-(64+8))%16)*16 + ((X-(64+8))%16)]==1) return;
						}
						bFillFirstIteration=false;

						if (penActive==3&&(!Shift.Contains(ssShift)))
							{
								tmpContinousIncDecTimer++;
								if(tmpContinousIncDecTimer>continousIncDecDuration||tmpContinousIncDecTimer==0)
								{
									tmpContinousIncDecTimer=0;
									if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
									if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
								}
						}
						if (penActive==0&&(!Shift.Contains(ssShift)))
						{
							if(!bSmudge){
								if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
								if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
							}
							else{
								if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((iSmudge&1)<<7)>>px);
								if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((iSmudge&2)<<6)>>px);
							}
						}
						if (penActive==1||(Shift.Contains(ssShift)))
						{
							if(!Shift.Contains(ssAlt))tempPal++;
							else tempPal--;

							if(FormMain->IncDecCap1->Checked==true)
							{
								if (tempPal >3) tempPal = 3;
								if (tempPal <0) tempPal = 0;
							}


							if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((tempPal&1)<<7)>>px);
							if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((tempPal&2)<<6)>>px);

						}
						if (penActive==2)
						{
							if(!Shift.Contains(ssAlt))tempPal--;
							else tempPal++;

							if(FormMain->IncDecCap1->Checked==true)
							{
								if (tempPal >3) tempPal = 3;
								if (tempPal <0) tempPal = 0;
							}

							if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((tempPal&1)<<7)>>px);
							if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((tempPal&2)<<6)>>px);
						}
					}
					else
					{
						lineUpX= -1; lineUpY= -1;
						//if(b)pp=tileActive*16+bankActive+tx+ty; else pp = -1;
						if(b)pp=tileViewTable[tileActive+tx+ty]*16+bankViewTable[set+tileActive+tx+ty];
						else pp = -1;
						bufpp=tileViewTable[tileActive+tx+ty]*16;
						Fill(Shift,ftx+fpx,fty+fpy,-1,-1,pp,bufpp,false);
					}
				}}
			}

			cueUpdateMetasprite=true;
			cueUpdateNametable=true;
			cueUpdateTiles=true;

			cueCHRdraw=true;
            return;
		}

		if(Shift.Contains(ssRight))
		{
			if(Shift.Contains(ssAlt) && Shift.Contains(ssCtrl))
			{
				lineUpX= -1; lineUpY= -1;

				//if(b)pp=tileActive*16+bankActive+tx+ty; else pp = -1;
				if(b)pp=tileViewTable[tileActive+tx+ty]*16+bankViewTable[set+tileActive+tx+ty];
				else pp = -1;

				bufpp=tileViewTable[tileActive+tx+ty]*16;
				Fill(Shift,ftx+fpx,fty+fpy,-1,-1,pp,bufpp,false);
				//Fill(Shift,px,py,pp,bufpp,false); //tempPal,fillPal

				cueUpdateMetasprite=true;
				cueUpdateNametable=true;
				cueUpdateTiles=true;

				cueCHRdraw=true;
			}
			else{
				bgPalCur=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);
				FormMain->DrawPalettes();
			}
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
    if(FormMain->Active) return;

	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;


	if(bCustomBucketLoadSuccess && bBufCtrl){
		Screen->Cursor=(TCursor)const_crCustomNormalBucket;
	} else  Screen->Cursor=crDefault;


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
			if(Key=='A'||Key==VK_LEFT)  ScrollHorz(true);
			if(Key=='D'||Key==VK_RIGHT) ScrollHorz(false);
			if(Key=='W'||Key==VK_UP)    ScrollVert(false);
			if(Key=='S'||Key==VK_DOWN)  ScrollVert(true);



		}
		if(!lineDrawing)
		{

			//if(Key=='V') MirrorVertical();    //this is an alias for T
			if(Key=='R') SpeedButtonRotateCWClick(Sender);//{Flip90(false); Rotate4tiles(false);}
			if(Key=='G') {btnSmudge->Down=true; FormMain->btnSmudge->Down=true; bSmudge=true;}
			if(Key=='T')
			{
				FormLineDetails->btnSmear->Down^=true;
				FormMain->LineCoating1->Checked=FormLineDetails->btnSmear->Down;
				int i = iLinePresetIndex;
				bLinePreset_modeCoat[i]=FormLineDetails->btnSmear->Down;
				if(FormLineDetails->btnSmear->Down) 	FormMain->LabelStats->Caption="Coat mode ON.";
				else									FormMain->LabelStats->Caption="Coat mode OFF.";
				FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;
			}

			if(Key=='K')
			{
				FormLineDetails->btnQuick->Down^=true;
				FormMain->LineQuickmultiline1->Checked=FormLineDetails->btnQuick->Down;
                int i = iLinePresetIndex;
				bLinePreset_modeQuick[i]=FormLineDetails->btnQuick->Down;
				if(FormLineDetails->btnQuick->Down) 	FormMain->LabelStats->Caption="Quick multiline ON.";
				else			   						FormMain->LabelStats->Caption="Quick multiline OFF.";
				FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;
			}

			if(Key=='F') FormBucketToolbox->ToggleFillMode();
			if(Key=='C') FormBucketToolbox->ToggleFloodMode();
			//int scancode_oe= MapVirtualKey(VK_OEM_1, MAPVK_VK_TO_VSC);

			/* moved to appropriate menu actions

			if(Key=='5') {Protect0->Down^=true; FormMain->Protect0->Down = Protect0->Down;}
			if(Key=='6') {Protect1->Down^=true; FormMain->Protect1->Down = Protect1->Down;}
			if(Key=='7') {Protect2->Down^=true; FormMain->Protect2->Down = Protect2->Down;}
			if(Key=='8') {Protect3->Down^=true; FormMain->Protect3->Down = Protect3->Down;}
			*/
		}


		//if(Key=='H') MirrorHorizontal();
		
		if(Key=='L') SpeedButtonRotateCCWClick(Sender);//{Flip90(true); Rotate4tiles(true);}

		if(Key=='B') {btnThick->Down^=true; FormMain->btnThick->Down = btnThick->Down;}
		if(Key=='N') {btnLine->Down^=true;   FormMain->btnLine->Down = btnLine->Down;}
		if(Key=='U') {btnQuant->Down^=true;  FormMain->btnQuant->Down = btnQuant->Down;}
		if(Key=='M') FormBrush->Visible^=true;

		if(Key=='I') ButtonBitmaskHi->Down^=true;
		if(Key=='O') ButtonBitmaskLo->Down^=true;
		
		if(Key=='P') SpeedButtonDoWrap->Down^=true;
		if(Key=='Q') FormMain->Show();
		if(Key=='X') FormMain->Toggletileset1Click(FormMain->Toggletileset1);

		if(Key=='E') {btn2x2mode->Down^=true;}

		if(Key==VK_OEM_4||Key==VK_OEM_COMMA) FormMain->SpeedButtonPrevMetaSpriteClick(Sender);// [
		if(Key==VK_OEM_6||Key==VK_OEM_PERIOD) FormMain->SpeedButtonNextMetaSpriteClick(Sender);// ]


		if(Key==(int)MapVirtualKey(0x27, 1)) FormBrush->ChangePreset(-1);
		if(Key==(int)MapVirtualKey(0x28, 1)) FormBrush->ChangePreset(+1);
		if(Key==(int)MapVirtualKey(0x2B, 1)) FormBrush->ChangePreset(+7);

		if(Key==VK_NUMPAD7) TileChange(-1,-1);
		if(Key==VK_NUMPAD8) TileChange( 0,-1);
		if(Key==VK_NUMPAD9) TileChange(+1,-1);

		if(Key==VK_NUMPAD4) TileChange(-1,0);
		if(Key==VK_NUMPAD5) FormMain->MCHREditorClick(Sender);
		if(Key==VK_NUMPAD6) TileChange(+1,0);

		if(Key==VK_NUMPAD1) TileChange(-1,+1);
		if(Key==VK_NUMPAD2) TileChange( 0,+1);
		if(Key==VK_NUMPAD3) TileChange(+1,+1);


	}

	if(FormMain->PageControlEditor->ActivePage==FormMain->TabSheetName)
	{

         //todo: maybe make use comma, period?
		if(Shift.Contains(ssCtrl))
		{
			if(Key==VK_OEM_4) FormMain->ChangeNameTableFrame(-1);// [
			if(Key==VK_OEM_6) FormMain->ChangeNameTableFrame(1);// ]
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::SpeedButtonHFlipClick(TObject *Sender)
{
	if(!lineDrawing)MirrorHorizontal();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::SpeedButtonVFlipClick(TObject *Sender)
{
	if(!lineDrawing)MirrorVertical();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::SpeedButtonRotateCCWClick(TObject *Sender)
{
	if(!lineDrawing){
	Flip90(true);
	Rotate4tiles(true);

	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);


	FormMain->UpdateMetaSprite(false);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::SpeedButtonRotateCWClick(TObject *Sender)
{
	if(!lineDrawing){
	Flip90(false);
	Rotate4tiles(false);
	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);

	FormMain->UpdateMetaSprite(false);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormCreate(TObject *Sender)
{
	memset (arr_linePlotCHR,0,sizeof(arr_linePlotCHR));
	memset (arr_linePlotCHR2,0,sizeof(arr_linePlotCHR2));

	buffer = new Graphics::TBitmap();

	buffer->Width = PaintBoxChr->Width;
	buffer->Height = PaintBoxChr->Height;
	buffer->PixelFormat=pf24bit;
	DoubleBuffered=true;

	GroupBox1->DoubleBuffered=true;
	if(FormCHREditor->Position==poDesigned)
			{
			FormCHREditor->Left=(Screen->Width-FormMain->Width)/2.3 + FormMain->Width;
			FormCHREditor->Top=(Screen->Height-FormMain->Height)/4;
			//FormCHREditor->Position=poDesigned;
			//FormCHREditor->Left=(curMainWinPos.left+FormMain->Width);
			//FormCHREditor->Top=(curMainWinPos.top);
			//curMainWinPos
			/*TPoint P;
			P = Mouse->CursorPos;
			FormCHREditor->Left=P.x;
			FormCHREditor->Top=P.y;*/
			}
	if(FormMain->CHReditortoolbartop->Checked){
		GroupBox2->Align=alTop;
		GroupBox1->Align=alBottom;
	}
	if(FormMain->CHReditortoolbarbottom->Checked){
		GroupBox1->Align=alTop;
		GroupBox2->Align=alBottom;
	}
	GroupBox1->Color=TColor(0xCFCFCF);
	FormCHREditor->ScreenSnap=bSnapToScreen;
	if(prefStartShowCHR==true) FormCHREditor->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormMouseWheel(TObject *Sender,
	  TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
	bool l=btnLine->Down;
	bool t=TimerScrollEvent->Enabled;
	bool bCtrl=Shift.Contains(ssCtrl)?true:false;
	//bool bShift=Shift.Contains(ssShift)?true:false;
	bool bAlt=Shift.Contains(ssAlt)?true:false;

	int tmpD=WheelDelta/2;
	int absl;
	//FormMain->Caption=IntToStr(tmpD);

	if(l )
	{

		if(bCtrl==false){
			if(bAlt)   //just alt pressed
			{
				if(tmpD<0) lineToolAlt++;
				else lineToolAlt--;

			}
			else if(t==false){     //nothing pressed, t is for excluding double trigs

				lineToolAlt=0;
				lineToolX=0;
				lineToolY=0;
				if(tmpD<0) lineToolRoster--;
				else lineToolRoster++;

				for (int i = 0; i < 8; i++) {
					absl = abs(lineToolRoster);
					if (lineRosterEnable[absl]==false)
					{
						if(tmpD<0) lineToolRoster--;
						else lineToolRoster++;
					}
				}
				if (lineToolRoster > 8) lineToolRoster = -8;
				if (lineToolRoster < -8) lineToolRoster = 8;

				int tmpi = iLinePresetIndex;
				iLinePreset_rosterIndex[tmpi]=lineToolRoster;
			}
		}
		else  //ctrl is assumed
		{
			if(bAlt)   //ctrl + alt
			{
				if(tmpD<0) lineToolX++;
				else lineToolX--;
			}
			else     //just ctrl
			{

				if(tmpD<0) {lineToolY++; lineToolY_toggleperm--;}
				else {lineToolY--; lineToolY_toggleperm--;}
			}
		}

		bool b=btn2x2mode->Down?1:0;
		int n=b?128:0;
		int hi=64+8+128+n;
		int lo=64+8;

		CHR_scrollX= (CHR_scrollX >= lo)? CHR_scrollX:lo;
		CHR_scrollX= (CHR_scrollX < hi)? CHR_scrollX:hi-1;
		CHR_scrollY= (CHR_scrollY >= lo)? CHR_scrollY:lo;
		CHR_scrollY= (CHR_scrollY < hi)? CHR_scrollY:hi-1;


		if(lineDrawing)	Line(Shift,lineDownX,lineDownY,CHR_scrollX,CHR_scrollY,0);
		cueUpdateTiles=true;
		cueUpdateNametable=true;
		TimerScrollEvent->Enabled=true;
	}

	else if(bIsFilling){
			//this prevents the release of buttons to trigger other scroll wheel modes while the wheel is spinning
			FormMain->TimerScrollWheelDisabler->Enabled=true;
			if(tmpD<0) FormBucketToolbox->RotateCW();
			else FormBucketToolbox->RotateCCW();
			//PaintBoxChrMouseMove(Sender,Shift,X,Y);
			Fill(Shift,-1,-1,-1,-1,-1, 0,false);
	}
	else
	{
		if(FormMain->TimerScrollWheelDisabler->Enabled==true){
		FormMain->TimerScrollWheelDisabler->Enabled=false;
		FormMain->TimerScrollWheelDisabler->Enabled=true;
		return;
		}

		if(WheelDelta<0)
		{

			if(Shift.Contains(ssShift)||Shift.Contains(ssCtrl))
			{
				if(Shift.Contains(ssShift)) tileActive=(tileActive-1)&255;
				if(Shift.Contains(ssCtrl)) tileActive=(tileActive+16)&255;
				FormMain->SetTile(tileActive);
				Draw(true);
			}
			else { bgPalCur=(bgPalCur-1)&3; FormMain->DrawPalettes(); }
		}
		else
		{
			if(Shift.Contains(ssShift)||Shift.Contains(ssCtrl))
			{
				if(Shift.Contains(ssShift)) tileActive=(tileActive+1)&255;
				if(Shift.Contains(ssCtrl)) tileActive=(tileActive-16)&255;
				FormMain->SetTile(tileActive);
				Draw(true);
			}
			else { bgPalCur=(bgPalCur+1)&3; FormMain->DrawPalettes();  }
		}

	}
	Handled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::PaintBoxChrMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bool bSmear = FormLineDetails->btnSmear->Down;
	//bool bMove  = FormLineDetails->btnMove->Down;
	//bool bReuse = FormLineDetails->btnReuse->Down;
	bool bQuick = FormLineDetails->btnQuick->Down;
	bIsFilling=false;
	lineUpX=X;
		lineUpY=Y;
		mvOriginX=0;
		mvOriginY=0;
		lineDownX=0;
		lineDownY=0;
	if(lineDrawing){
		FormCHREditor->SpeedButtonHFlip->Enabled=true;
		FormCHREditor->SpeedButtonVFlip->Enabled=true;
		FormCHREditor->SpeedButtonRotateCW->Enabled=true;
		FormCHREditor->SpeedButtonRotateCCW->Enabled=true;

		FormLineDetails->btnMove->Down=false;
	}
	else{lineUpX= -1; lineUpY= -1;}

		
	if(bSmear || bQuick){memcpy (arr_linePlotCHR2,arr_linePlotCHR,sizeof(arr_linePlotCHR2));}

	FormLineDetails->btnMove->Enabled=false;
	lineDrawing = false;
	
	isMovingLine= false;
	bForbidPaint = false;
	if(Y>=64+8 && Y<64+4+128 && X>=64+8 && X<64+4+128)
	{
	//not in use
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormCHREditor::CHRIncDec(TObject *Sender)
{
   int i,j,pp,mask;
   int tempPal;
   int startx=chrSelection.Left;
   int starty=chrSelection.Top;
   int lenx=chrSelection.Width();
   int leny=chrSelection.Height();
   int set = bankActive/16;

   FormMain->SetUndo();

	for (int y = starty; y < starty+leny; y++)
	{
		for (int x = startx; x < startx+lenx; x++)
		{
			//pp = tileActive*16  +y*256+x*16 +bankActive;
			pp = tileViewTable[ x + y*16]*16 + bankViewTable[set  + x + y*16];

			for(i=0;i<8;i++)
			{
				for(j=1;j<9;j++)
				{
					mask=128>>8-j;
					tempPal=(((chr[pp]<<8-j)&128)>>7)|(((chr[pp+8]<<8-j)&128)>>6);

					if(Sender==CHRInc || Sender==FormMain->CHRInc)tempPal++;
					if(Sender==CHRDec || Sender==FormMain->CHRDec)tempPal--;

					if(!SpeedButtonDoWrap->Down)
					{
						if (tempPal >3) tempPal = 3;
						if (tempPal <0) tempPal = 0;
					}

					chr[pp]=(chr[pp]&~mask)|(((tempPal&1)<<7)>>8-j);
					chr[pp+8]=(chr[pp+8]&~mask)|(((tempPal&2)<<6)>>8-j);
				}
			pp++;
			}
		}
	}



	if(FormCHREditor->Visible) Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);


	FormMain->UpdateMetaSprite(false);
}
//---------------------------------------------------------------------------

//-------------
void __fastcall TFormCHREditor::CHRIncClick(TObject *Sender)
{
  CHRIncDec(Sender);
  

}
//---------------------------------------------------------------------------



void __fastcall TFormCHREditor::SpeedButton1UpClick(TObject *Sender)
{
	if(Sender==SpeedButton1Left)
		ScrollHorz(true);
	if(Sender==SpeedButton1Right)
		ScrollHorz(false);
	if(Sender==SpeedButton1Up)
		ScrollVert(false);
	if(Sender==SpeedButton1Down)
		ScrollVert(true);
}
//---------------------------------------------------------------------------


void __fastcall TFormCHREditor::FormActivate(TObject *Sender)
{
	FormCHREditor->ScreenSnap=bSnapToScreen;
	FormCHREditor->AlphaBlendValue=iGlobalAlpha;
	FormMain->Attributes1->Enabled=false;
	FormMain->SelectedOnly1->Enabled=false;
	FormMain->ApplyTiles1->Enabled=false;
	FormMain->ApplyAttributes1->Enabled=false;
	FormMain->Fill1->ShortCut=TextToShortCut("(None)");
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormDeactivate(TObject *Sender)
{
	FormMain->Attributes1->Enabled=true;
	FormMain->SelectedOnly1->Enabled=true;
	FormMain->ApplyTiles1->Enabled=true;
	FormMain->ApplyAttributes1->Enabled=true;
	if(FormMain->SpeedButtonTypeIn->Down==false)
		FormMain->Fill1->ShortCut=TextToShortCut("F");
	if(Screen->Cursor==(TCursor)const_crCustomNormalBucket
			|| Screen->Cursor==crSizeAll
			|| Screen->Cursor==const_crCustomPreciseCursor
			|| Screen->Cursor==const_crCustomPreciseCursorSel
			|| Screen->Cursor!=crDrag) Screen->Cursor=crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btnSmudgeClick(TObject *Sender)
{
   bSmudge=btnSmudge->Down; FormMain->btnSmudge->Down=btnSmudge->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::SpeedButtonToggleEditorClick(TObject *Sender)
{
	FormCHREditor->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::PaintBoxChrMouseLeave(TObject *Sender)
{
	CHREditorHover=-1;
	cueStats=true;
	bIsFilling=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btn2x2modeClick(TObject *Sender)
{
	bool b=btn2x2mode->Down;
	int f=1;
	if(N2x21->Checked) f=1;
	if(N3x31->Checked) f=2;
	if(N4x41->Checked) f=3;
	int n=b?128*f:0;

	FormCHREditor->Width =originalFormWdt+n;
	FormCHREditor->Height=originalFormHgt+n;

	GroupBox1->Width	=originalGroupBoxWdt+n;
	GroupBox1->Height	=originalGroupBoxHgt+n;

	PaintBoxChr->Width  =originalPaintBox+n;
	PaintBoxChr->Height  =originalPaintBox+n;

	buffer->Width = PaintBoxChr->Width;
	buffer->Height = PaintBoxChr->Height;

	if(b)
	{

		if(f==1){
			if((tileActive&15)==15) tileActive--;
			if(tileActive>=0xF0) tileActive-=16;
		}
		if(f==2){
			if((tileActive&15)>=14) tileActive= (tileActive&0xF0) + (tileActive&15) -((tileActive&15)-13);
			if(tileActive>=0xE0) tileActive-=16*(((tileActive&0xF0)/16)-13);
		}
		if(f==3){
			if((tileActive&15)>=13) tileActive= (tileActive&0xF0) + (tileActive&15) -((tileActive&15)-12);
			if(tileActive>=0xD0) tileActive-=16*(((tileActive&0xF0)/16)-12);
		}
	}

	FormMain->SetTile(tileActive);
	if(FormCHREditor->Visible) Draw(true);
}
//---------------------------------------------------------------------------









void __fastcall TFormCHREditor::Protect0MouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			Protect0->Down=true;    //bc click applies after, this one is inverted
			Protect1->Down=true;
			Protect2->Down=true;
			Protect3->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(Protect0->Down && (Protect1->Down||Protect2->Down||Protect3->Down))
		{
			Protect0->Down=false; //force always down, except if others are already up

		}
		Protect1->Down=false;
		Protect2->Down=false;
		Protect3->Down=false;
	}
	if(Shift.Contains(ssShift))
	{

		Protect1->Down=false;
		Protect2->Down=false;
		Protect3->Down=false;
		if(!Protect0->Down)
		{
			Protect0->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormCHREditor::Protect1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			Protect0->Down=true;
			Protect1->Down=true;    //bc click applies after, this one is inverted
			Protect2->Down=true;
			Protect3->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(Protect1->Down && (Protect0->Down||Protect2->Down||Protect3->Down))
		{
			Protect1->Down=false; //force always down, except if others are already up

		}
		Protect0->Down=false;
		Protect2->Down=false;
		Protect3->Down=false;
	}
	if(Shift.Contains(ssShift))
	{

		Protect0->Down=false;
		Protect2->Down=false;
		Protect3->Down=false;
		if(!Protect1->Down)
		{
			Protect1->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::Protect2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			Protect0->Down=true;
			Protect1->Down=true;    //bc click applies after, this one is inverted
			Protect2->Down=true;
			Protect3->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(Protect2->Down && (Protect0->Down||Protect1->Down||Protect3->Down))
		{
			Protect2->Down=false; //force always down, except if others are already up

		}
		Protect0->Down=false;
		Protect1->Down=false;
		Protect3->Down=false;
	}
	if(Shift.Contains(ssShift))
	{

		Protect0->Down=false;
		Protect1->Down=false;
		Protect3->Down=false;
		if(!Protect2->Down)
		{
			Protect2->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::Protect3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			Protect0->Down=true;
			Protect1->Down=true;    //bc click applies after, this one is inverted
			Protect2->Down=true;
			Protect3->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(Protect3->Down && (Protect0->Down||Protect1->Down||Protect2->Down))
		{
			Protect3->Down=false; //force always down, except if others are already up

		}
		Protect0->Down=false;
		Protect1->Down=false;
		Protect2->Down=false;
	}
	if(Shift.Contains(ssShift)) //force all up
	{

		Protect0->Down=false;
		Protect1->Down=false;
		Protect2->Down=false;
		if(!Protect3->Down)
		{
			Protect3->Down=true; //force always up

		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::Protect0MouseEnter(TObject *Sender)
{
	int n;
	AnsiString str;
	if(Sender==Protect0) n=0;
	if(Sender==Protect1) n=1;
	if(Sender==Protect2) n=2;
	if(Sender==Protect3) n=3;
	str="Protects pixels of colour "+IntToStr(n)+" from being drawn over."
		+"\n\n[Click] to Toggle.\t\t[Ctrl+click] to mute chosen colour while enabling the others."
		+"\n[Shift + click] to enable all.\t[Alt+click] to 'solo' chosen colour while disabling the others.";
	FormMain->LabelStats->Caption=str;
}
//--------------------------------------------------------------------------
void __fastcall TFormCHREditor::Protect0MouseLeave(TObject *Sender)
{
	FormMain->LabelStats->Caption="---";
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::ButtonBitmaskLoMouseEnter(TObject *Sender)
{
	AnsiString str;
	if(Sender==ButtonBitmaskLo && ButtonBitmaskLo->Down)  str="Disable editing the Low bitplane.";
	if(Sender==ButtonBitmaskLo && !ButtonBitmaskLo->Down) str="Enable editing the Low bitplane.";
	if(Sender==ButtonBitmaskHi && ButtonBitmaskHi->Down)  str="Disable editing the High bitplane.";
	if(Sender==ButtonBitmaskHi && !ButtonBitmaskHi->Down) str="Enable editing the High bitplane.";

	str+="\tSee the menu item [Draw->Bitmask options...] for applications.\n\nNES patterns are stored in 2 bitplanes. It can sometimes be useful to split/merge bitplanes or edit them separately.\nThe low bit represents colour 1. The high; colour 2. Both bits set / clear constitute colour 3 / 0, resp.";
	FormMain->LabelStats->Caption=str;


}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::CHRIncMouseEnter(TObject *Sender)
{
	AnsiString str;
	if(Sender==CHRInc) {str="Increment the index of each pixel in the selected tile(s).";
		if(SpeedButtonDoWrap->Down) str+="\nOverflow wraps from colour 3 to 0.";
		else str+="\nOverflow caps to colour 3.";
		str+="\n\nWhen a subpalette is ordered from dark to bright, this action is equivalent to making selected tiles brighter.";
		}
	if(Sender==CHRDec) {str="Decrement the index of each pixel in the selected tile(s).";
		if(SpeedButtonDoWrap->Down) str+="\nUnderflow wraps from colour 0 to 3.";
		else str+="\nUnderflow caps to colour 0.";
		str+="\n\nWhen a subpalette is ordered from dark to bright, this action is equivalent to making selected tiles darker.";
		}
	FormMain->LabelStats->Caption=str;
}
//---------------------------------------------------------------------------


void __fastcall TFormCHREditor::SpeedButtonDoWrapMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="When down, the value +/- buttons will wrap colour indices around from 0 to 3 or vise versa.\nWhen up, the above buttons cap to values 3 or 0 repsectively.";
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btnSmudgeMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Smudge: When down, the pen/brush automatically samples the pixel clicked on.\nHolding [G] momentarily enables smudge. Lifting [G] always releases this button.\nTips:\t-useful for defining clusters / borders between fields.\n\t-while drawing; alternate holding and releasing [G] for a convenient colour switch.";
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::SpeedButton1UpMouseEnter(TObject *Sender)
{
	AnsiString str;
	bool b =(FormBrush->Active|FormCHREditor->Active);
	if (b)	str="[WASD] Nudges the pattern of the selected tile(s) ";
	else str="Nudges the pattern of the selected tile(s) ";
	if(Sender==SpeedButton1Up) str+="up.";
	if(Sender==SpeedButton1Down) str+="down.";
	if(Sender==SpeedButton1Left) str+="left.";
	if(Sender==SpeedButton1Right) str+="right.";
	str +="\nWraparound occurs at the seams of the box selection.";
	FormMain->LabelStats->Caption=str;

}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::SpeedButtonHFlipMouseEnter(TObject *Sender)
{
	AnsiString str;
	str="[";
	//if (!b) str+="Shift+";      //nope.
	if(Sender==SpeedButtonHFlip) str+="H] ";
	if(Sender==SpeedButtonVFlip) str+="V or T] ";
	if(Sender==SpeedButtonRotateCCW) str+="L] ";
	if(Sender==SpeedButtonRotateCW) str+="R] ";


	str+="Flips the pattern of the selected tile(s) ";

	if(Sender==SpeedButtonHFlip) str+="horizontally.";
	if(Sender==SpeedButtonVFlip) str+="vertically.";
	if(Sender==SpeedButtonRotateCCW) str+="counter clockwise.";
	if(Sender==SpeedButtonRotateCW) str+="clockwise.";

	if(btn2x2mode->Down) str +="\nIn 2x2 mode; tiles also swap places so long as 2x2 tiles are box selected.";
	FormMain->LabelStats->Caption=str;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::SpeedButtonToggleEditorMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="[Q] Toggles focus between CHR Editor and Main Editor.\n\nTip:\tToggling focus makes the respective sets of hotkeys available \n\tfrom each window at the press of the hotkey [Q].";
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btn2x2modeMouseEnter(TObject *Sender)
{
	bool b =(FormBrush->Active|FormCHREditor->Active);
	if(b)	FormMain->LabelStats->Caption="[E] Toggles between 2x2 and 1 tile edit mode.\n\nTip:\tIn 2x2 mode; The flip/rotate actions also swap tile places so long as 2x2 tiles remain box selected.";
    else	FormMain->LabelStats->Caption="[Shift+E] Toggles between 2x2 and 1 tile edit mode.\n\nTip:\tIn 2x2 mode; The flip/rotate actions also swap tile places so long as 2x2 tiles remain box selected.";

}
//---------------------------------------------------------------------------


void __fastcall TFormCHREditor::btnThickMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssRight))FormBrush->Show();
	FormMain->TogglePenBrush1->Checked=btnThick->Down;
}
//---------------------------------------------------------------------------



void __fastcall TFormCHREditor::btnQuantMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
      FormMain->Quantized1->Checked=btnQuant->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btnThickMouseEnter(TObject *Sender)
{
	bool b =(FormBrush->Active || FormCHREditor->Active  || FormMain->PageControlEditor->ActivePage==FormMain->TabSheetTile);
	if (b)	FormMain->LabelStats->Caption="[B] Toggles between Pen (1px) and Brush (masked 2x2) tool on the Left mouse button.\n\nTip:\tRight-click this button or press [Shift-F4] to open brush shape editor.";
	else 	FormMain->LabelStats->Caption="[Shift+B] Toggles between Pen (1px) and Brush (masked 2x2) tool on the Left mouse button.\n\nTip:\tRight-click this button or press [Shift-F4] to open brush shape editor.";
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btnQuantMouseEnter(TObject *Sender)
{
	bool b =(FormBrush->Active ||FormCHREditor->Active  || FormMain->PageControlEditor->ActivePage==FormMain->TabSheetTile);
	if (b)	FormMain->LabelStats->Caption="[U] Quantizes pen/brush application to be valid only for even coordinates; counting from 0.\n\nTips:\tIn pen mode, using this mode while also nudging the patterns can create a variety of patterns.\n\tIn Brush mode, it's effective with different brush masks [M], but also for drawing 'chunky pixels'.";
	else    FormMain->LabelStats->Caption="[Shift+U] Quantizes pen/brush application to be valid only for even coordinates; counting from 0.\n\nTips:\tIn pen mode, using this mode while also nudging the patterns can create a variety of patterns.\n\tIn Brush mode, it's effective with different brush masks [Shift+F4], but also for drawing 'chunky pixels'.";

}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormShow(TObject *Sender)
{
	FormCHREditor->AlphaBlendValue=iGlobalAlpha;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormPaint(TObject *Sender)
{
	FormCHREditor->AlphaBlendValue=iGlobalAlpha;	
}
//---------------------------------------------------------------------------



void __fastcall TFormCHREditor::TimerScrollEventTimer(TObject *Sender)
{
     if(!openByFileDone) return;
	TimerScrollEvent->Enabled=false;
	TShiftState currentState = TShiftState();
	PaintBoxChrMouseMove(Sender,currentState,CHR_mmX,CHR_mmY);
    FormLineDetails->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btnLineMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//if(Shift.Contains(ssRight))
	if(!FormLineDetails->Visible) FormLineDetails->Show();


}
//---------------------------------------------------------------------------


void __fastcall TFormCHREditor::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	//if(Key==VK_OEM_3 || Key==VK_OEM_5) bBufVK_3=true;
	bBufVK_3=false;
	if(Key=='G') {btnSmudge->Down=false; FormMain->btnSmudge->Down=false; bSmudge=false;}
	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;

	if(bCustomBucketLoadSuccess && bBufCtrl){
		Screen->Cursor=(TCursor)const_crCustomNormalBucket;
	} else  Screen->Cursor=crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormKeyPress(TObject *Sender, char &Key)
{
	if (GetKeyState(VK_CONTROL) & 0x8000) // Check if Ctrl key is down
	{
			if(lineDrawing){
			CHR_moveOriginX=movx2;
			CHR_moveOriginY=movy2;

			}
	}
	
}
//---------------------------------------------------------------------------



void __fastcall TFormCHREditor::AsyncKeyTimerTimer(TObject *Sender)
{
	if(!openByFileDone) return;
	bool key=false;
	static bool vkpSmear = false;
	static bool vkpMove = false;
	static bool vkpQuick = false;

	static bool wp = false;
	static bool ap = false;
	static bool sp = false;
	static bool dp = false;
	static bool cp = false;
	static bool bp = false;

	static bool kp = false;
	static bool tp = false;

	static bool n1p = false;
	static bool n2p = false;
	static bool n3p = false;
	static bool n4p = false;

	static bool n5p = false;
	static bool n6p = false;
	static bool n7p = false;
	static bool n8p = false;

	static bool n9p = false;
	static bool n0p = false;



	bool w = GetAsyncKeyState('W') & 0x8000;
	bool a = GetAsyncKeyState('A') & 0x8000;
	bool s = GetAsyncKeyState('S') & 0x8000;
	bool d = GetAsyncKeyState('D') & 0x8000;
	bool c = GetAsyncKeyState('C') & 0x8000;
	bool b = GetAsyncKeyState('B') & 0x8000;

	bool k = GetAsyncKeyState('K') & 0x8000;
	bool t = GetAsyncKeyState('T') & 0x8000;

	bool n1 = GetAsyncKeyState('1') & 0x8000;
	bool n2 = GetAsyncKeyState('2') & 0x8000;
	bool n3 = GetAsyncKeyState('3') & 0x8000;
	bool n4 = GetAsyncKeyState('4') & 0x8000;

	bool n5 = GetAsyncKeyState('5') & 0x8000;
	bool n6 = GetAsyncKeyState('6') & 0x8000;
	bool n7 = GetAsyncKeyState('7') & 0x8000;
	bool n8 = GetAsyncKeyState('8') & 0x8000;

	bool n9 = GetAsyncKeyState('9') & 0x8000;
	bool n0 = GetAsyncKeyState('0') & 0x8000;

	//GetAsyncKeyState('VK_CAPITAL') & 0x0001;
	bool vkSmear = GetAsyncKeyState(0x09) & 0x8000;    //tab
	bool vkMove  = GetAsyncKeyState('V') & 0x8000;
	bool vkQuick = ((GetAsyncKeyState(0xC0) & 0x8000)   //VK_OEM_3
				|| (GetAsyncKeyState(0xDC) & 0x8000));  //VK_OEM_5

	//bool bSmear = ((GetKeyState(VK_CAPITAL) & 0x0001)!=0);




	if(lineDrawing){
			if(w && !wp) {lineOffY--;  key=true;}
			if(a && !ap) {lineOffX--; key=true;}
			if(s && !sp) {lineOffY++;  key=true;}
			if(d && !dp) {lineOffX++;  key=true;}

			if(c && !cp) {lineOffX=0;lineOffY=0;  key=true;}

			if(b && !bp)   {key=true;}

			if(n1 && !n1p) {key=true;}
			if(n2 && !n2p) {key=true;}
			if(n3 && !n3p) {key=true;}
			if(n4 && !n4p) {key=true;}

			if(n5 && !n5p) {Protect0->Down^=true;	FormMain->Protect0->Down=Protect0->Down; key=true;}
			if(n6 && !n6p) {Protect1->Down^=true;	FormMain->Protect0->Down=Protect1->Down; key=true;}
			if(n7 && !n7p) {Protect2->Down^=true;	FormMain->Protect0->Down=Protect2->Down; key=true;}
			if(n8 && !n8p) {Protect3->Down^=true;	FormMain->Protect0->Down=Protect3->Down; key=true;}


			if(n9 && !n9p) {key=true;}
			if(n0 && !n0p) {key=true;}

			if ((vkSmear && !vkpSmear) || (t && !tp)){
				FormLineDetails->btnSmear->Down^=true;
				FormMain->LineCoating1->Checked=FormLineDetails->btnSmear->Down;
				int i = iLinePresetIndex;
				bLinePreset_modeCoat[i]=FormLineDetails->btnSmear->Down;
				if(FormLineDetails->btnSmear->Down) 	FormMain->LabelStats->Caption="Coat mode ON.";
				else									FormMain->LabelStats->Caption="Coat mode OFF.";
				FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;
				key=true;
			}



			if (vkMove && !vkpMove)
			{
				FormLineDetails->btnMove->Down^=true;
				if(FormLineDetails->btnMove->Down)
				{
					mvOriginX=CHR_mmX;
					mvOriginY=CHR_mmY;
				}
				else {
					lineDownX=CHR_moveOriginX;
					lineDownY=CHR_moveOriginY;
				}


				key=true;
			}


			if ((vkQuick && !vkpQuick) || (k && !k))
			{
				FormLineDetails->btnQuick->Down^=true;  key=true;
				FormMain->LineQuickmultiline1->Checked=FormLineDetails->btnQuick->Down;
				int i = iLinePresetIndex;
				bLinePreset_modeQuick[i]=FormLineDetails->btnQuick->Down;
				if(FormLineDetails->btnQuick->Down) 	FormMain->LabelStats->Caption="Quick multiline ON.";
				else			   						FormMain->LabelStats->Caption="Quick multiline OFF.";
				FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;
			}


			if(key){
				TShiftState shiftState = TShiftState();
				
				Line(shiftState,lineDownX,lineDownY,CHR_mmX,CHR_mmY,globalLineSenderMode);
				cueUpdateMetasprite=true;
				cueUpdateNametable=true;
				cueUpdateTiles=true;
			

				cueCHRdraw=true;
            }

			//PaintBoxChrMouseMove(Sender,shiftState,CHR_mmX,CHR_mmY);

			//TimerScrollEvent->Enabled=true;
			//
			//TPoint mousePos;
			//GetCursorPos(&mousePos);
			//MouseMove(Shift, mousePos.x, mousePos.y);
		}

wp = w;
ap = a;
sp = s;
dp = d;
cp = c;
bp = b;

kp = k;
tp = t;

n1p = n1;
n2p = n2;
n3p = n3;
n4p = n4;
n5p = n5;
n6p = n6;
n7p = n7;
n8p = n8;
n9p = n9;
n0p = n0;

vkpSmear = vkSmear;
vkpMove  = vkMove;
vkpQuick = vkQuick;



}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btnLineMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Multipurpose Line tool.\nClicking (or pressesing Shift+F5) opens the line toolbox.";
}
//---------------------------------------------------------------------------


void __fastcall TFormCHREditor::btnQuantClick(TObject *Sender)
{
	if(Sender==btnLine){FormMain->btnLine->Down = btnLine->Down;
        lineToolY_toggleperm=0;
		FormMain->Linemode1->Checked=btnLine->Down;
	}
	if(Sender==btnQuant){FormMain->btnQuant->Down = btnQuant->Down;
		FormMain->Quantized1->Checked=btnQuant->Down;
	}
	if(Sender==btnThick){FormMain->btnThick->Down = btnThick->Down;
		FormMain->TogglePenBrush1->Checked=btnThick->Down;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormCHREditor::SpeedButtonDoWrapClick(TObject *Sender)
{
	FormMain->SpeedButtonDoWrap->Down=SpeedButtonDoWrap->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::Protect0Click(TObject *Sender)
{
	FormMain->Protect0->Down=Protect0->Down;
	FormMain->Protect1->Down=Protect1->Down;
	FormMain->Protect2->Down=Protect2->Down;
	FormMain->Protect3->Down=Protect3->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::ButtonBitmaskHiClick(TObject *Sender)
{
	FormMain->ButtonBitmaskHi->Down= ButtonBitmaskHi->Down;
	FormMain->ButtonBitmaskLo->Down= ButtonBitmaskLo->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btnLineMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (btnLine->Down==false) {
		FormCHREditor->SpeedButtonHFlip->Enabled=true;
		FormCHREditor->SpeedButtonVFlip->Enabled=true;
		FormCHREditor->SpeedButtonRotateCW->Enabled=true;
		FormCHREditor->SpeedButtonRotateCCW->Enabled=true;

	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormDestroy(TObject *Sender)
{
	delete buffer;	
}
//---------------------------------------------------------------------------


void __fastcall TFormCHREditor::N2x21Click(TObject *Sender)
{
    btn2x2mode->Down=true;
	bool b=btn2x2mode->Down;
	int f=1;
	int tag = ((TMenuItem*)Sender)->Tag;
	switch (tag) {
		case 1:  btn2x2mode->Caption="&Edit 3x3 tiles"; FormMain->N2x2tileeditmode1->Caption="3&x3 CHR Editor mode"; f=2; break;
		case 2:  btn2x2mode->Caption="&Edit 4x4 tiles"; FormMain->N2x2tileeditmode1->Caption="4&x4 CHR Editor mode"; f=3; break;

	default:
		// case 0
		btn2x2mode->Caption="&Edit 2x2 tiles"; FormMain->N2x2tileeditmode1->Caption="3&x3 CHR Editor mode"; f=1;  break;
		
	}
	if(b){


		int n=b?128*f:0;

		FormCHREditor->Width =originalFormWdt+n;
		FormCHREditor->Height=originalFormHgt+n;

		GroupBox1->Width	=originalGroupBoxWdt+n;
		GroupBox1->Height	=originalGroupBoxHgt+n;

		PaintBoxChr->Width  =originalPaintBox+n;
		PaintBoxChr->Height  =originalPaintBox+n;

	  buffer->Width = PaintBoxChr->Width;
		buffer->Height = PaintBoxChr->Height;

		if(b)
		{
			if(f==1){
				if((tileActive&15)==15) tileActive--;
				if(tileActive>=0xF0) tileActive-=16;
			}
			if(f==2){
				if((tileActive&15)>=14) tileActive= (tileActive&0xF0) + (tileActive&15) -((tileActive&15)-13);
				if(tileActive>=0xE0) tileActive-=16*(((tileActive&0xF0)/16)-13);
			}
			if(f==3){
				if((tileActive&15)>=13) tileActive= (tileActive&0xF0) + (tileActive&15) -((tileActive&15)-12);
				if(tileActive>=0xD0) tileActive-=16*(((tileActive&0xF0)/16)-12);
			}
		}
		FormMain->SetTile(tileActive);
		if(FormCHREditor->Visible) Draw(true);
	 }
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::SpeedButton1Click(TObject *Sender)
{
	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;
	PopupMenuEditMode->Popup(x,y);	
}
//---------------------------------------------------------------------------

