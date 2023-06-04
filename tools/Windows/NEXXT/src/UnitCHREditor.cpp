//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitCHREditor.h"
#include "UnitBrush.h"
#include "UnitLineDetails.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCHREditor *FormCHREditor;
extern bool openByFileDone;

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

extern void Bresenham_line(int x0, int y0, int x1, int y1, int w, unsigned char *array, bool bIsRightAngle);

extern void plotFineQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array);
extern void Bezier_curve(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array,int bi);
extern void plotQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array,int bi);
extern void Bresenham_angle(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array, bool knee, bool join,int bi);
extern void Bresenham_2line(float x0, float y0, float x2, float y2, int pw, unsigned char* ptr, int bi);
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
extern unsigned char chr[];
extern unsigned char chrBuf[];
extern unsigned char chrBufFill[];
extern unsigned char chrQuickLine[];
extern unsigned char chrBufLine[];
extern unsigned char tileViewTable[];
extern unsigned char sprModeTableReverse[];
unsigned char arr_linePlotCHR[128*128];
unsigned char arr_linePlotCHR2[128*128];

extern int lineToolRoster;
extern int lineToolAlt;
extern int lineToolX;
extern int lineToolY;
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

void __fastcall TFormCHREditor::DrawCHR(int xs,int ys,int tile)
{
	int i,j,x,y,pp,col;
	TRect r;

	y=ys;
	int grid= FormMain->CHRpixelgrid1->Checked?15:16;
    int r1,r2,g1,g2,b1,b2;
	int rAvg,gAvg,bAvg;
	TColor colorIn1;
	TColor colorIn2;

	pp=tileViewTable[tile]*16+bankActive;

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


				int rAvg = (r1*3 + r2*2) / 6;
				int gAvg = (g1*3 + g2*2) / 6;
				int bAvg = (b1*3 + b2*2) / 6;

				PaintBoxChr->Canvas->Brush->Color=TColor(RGB(rAvg, gAvg, bAvg));
				r.left=x;
				r.top=y;
				r.right=x+16;
				r.Bottom=y+16;

				PaintBoxChr->Canvas->FillRect(r);


				PaintBoxChr->Canvas->Brush->Color=TColor(outPalette[bgPal[palBank*16+palActive*4+col]]);

				r.left=x;
				r.top=y;
				r.right=x+grid;
				r.Bottom=y+grid;

				PaintBoxChr->Canvas->FillRect(r);

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
				PaintBoxChr->Canvas->Brush->Color=GroupBox1->Color;
				r.left=x;
				r.top=y;
				r.right=x+grid;
				r.Bottom=y+grid;
				PaintBoxChr->Canvas->FillRect(r);
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
	int n=b?0:8;

	tx=tileActive&15;
	ty=tileActive/16;


	//the checks lessen the burden of drawing these at high rates.

	if(a){
			DrawCHR(-64        			,-64        ,ty>0&&tx>0  ?tileActive-17:-1); //NW
			DrawCHR( 64+8      			,-64        ,ty>0        ?tileActive-16:-1); //N
			DrawCHR( 64+8+128+n			,-64        ,ty>0&&tx<15 ?tileActive-15:-1);
	if(b) 	DrawCHR( 64+8+128+n+128+8	,-64        ,ty>0&&tx<14 ?tileActive-14:-1);
	}

	if(a)		DrawCHR(-64        			, 64+8      ,tx>0        ?tileActive-1 :-1);
				DrawCHR( 64+8      			, 64+8      ,tileActive);
	if(b || a)	DrawCHR( 64+8+128+n			, 64+8      ,tx<15       ?tileActive+1 :-1);
	if(b && a)  DrawCHR( 64+8+128+n+128+8	, 64+8      ,tx<14       ?tileActive+2 :-1);
	//}

	if(b || a)		DrawCHR(-64        			, 64+8+128+n,ty<15&&tx>0 ?tileActive+15:-1);
	if(b || a)		DrawCHR( 64+8      			, 64+8+128+n,ty<15       ?tileActive+16:-1);
	if(b || a)		DrawCHR( 64+8+128+n			, 64+8+128+n,ty<15&&tx<15?tileActive+17:-1);
	if(b)   		DrawCHR( 64+8+128+n+128+8	, 64+8+128+n,ty<15&&tx<14?tileActive+18:-1);

	if(b && a)
	{
			DrawCHR(-64        			, 64+8+128+n+128+8,ty<14&&tx>0 ?tileActive+31:-1);
			DrawCHR( 64+8      			, 64+8+128+n+128+8,ty<14       ?tileActive+32:-1);
			DrawCHR( 64+8+128+n			, 64+8+128+n+128+8,ty<14&&tx<15?tileActive+33:-1);
			DrawCHR( 64+8+128+n+128+8	, 64+8+128+n+128+8,ty<14&&tx<14?tileActive+34:-1);
	}
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
	int ba=bankActive;
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
			pp = tileViewTable[tileActive + xTiles-1 +k*16]*16  + ba;
			for(i=fOff;i<fLen+fOff;i++)	c[i]=chr[pp+i];


			// shift a row
			for (j=0;j<xTiles; j++)
			{
			   tpp = tileViewTable[tileActive  + xTiles-1-j +k*16]*16  + ba;
			   tpp2 = tileViewTable[tileActive -1 + xTiles-1-j + k*16]*16  + ba;

				for(i=fOff;i<fLen+fOff;i++) chr[tpp+i]=chr[tpp+i]>>1 | ((chr[tpp2+i]<<7)&128);
			}

			// merge carry with first tile
			pp = tileViewTable[tileActive +k*16]*16  + ba;
			for(i=fOff;i<fLen+fOff;i++) chr[pp+i]=((chr[pp+i])&127)|((c[i]<<7)&128);
		}
	}
	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);
	FormMain->UpdateMetaSprite();
}



void __fastcall TFormCHREditor::ScrollVert(bool isDown)
{
	int i,j,k,l,pp,pp2,ptmp,ptmp2,t1,t2;
	int dir=1;

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
		ptmp2=tileViewTable[tileActive+k]*16+bankActive;
		t1=chr[ptmp2];
		t2=chr[ptmp2+8];

		for (j= 0; j < yTiles; j++)
		{
			pp  =tileViewTable[tileActive+j*16+k]*16+bankActive;
			pp2 =tileViewTable[tileActive+16+j*16+k]*16+bankActive;
			ptmp=tileViewTable[tileActive+j*16+k]*16+7+bankActive;


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
	FormMain->UpdateMetaSprite();
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
	FormMain->UpdateMetaSprite();
}



void __fastcall TFormCHREditor::MirrorHorizontal(void)
{
	int i,j,pp,tmp;
	int x,y;
	int xTiles=tileSelRectWdt;
	int yTiles=tileSelRectHgt;
	int ba=bankActive;
	int ta=tileActive;
	int fLen=16;
	int fOff=0;

	if (!(ButtonBitmaskLo->Down||ButtonBitmaskHi->Down))
		if(FormMain->Applytomirror1->Checked) return;

	FormMain->SetUndo();

	if (!ButtonBitmaskLo->Down&&FormMain->Applytomirror1->Checked) {fLen-=8; fOff=8;}
	if (!ButtonBitmaskHi->Down&&FormMain->Applytomirror1->Checked) {fLen-=8;}


	for (y = 0; y < yTiles; y++)
	{
		for (x = 0; x < xTiles; x++)
		{
			pp=tileViewTable[tileActive+y*16+x]*16+ba+fOff;
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
	if(b && tileSelRectWdt==2 && tileSelRectHgt==2)
	{
		unsigned char tempchr[8];

		if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){
			memcpy(tempchr,							&chr[tileViewTable[ta]*16+ba],8);
			memcpy(&chr[tileViewTable[ta]*16+ba],	&chr[tileViewTable[ta+1]*16+ba],8);
			memcpy(&chr[tileViewTable[ta+1]*16+ba],	tempchr,8);

			memcpy(tempchr,								&chr[tileViewTable[ta+16]*16+ba],8);
			memcpy(&chr[tileViewTable[ta+16]*16+ba],	&chr[tileViewTable[ta+17]*16+ba],8);
			memcpy(&chr[tileViewTable[ta+17]*16+ba],	tempchr,8);

		}

		if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){


			memcpy(tempchr,							&chr[tileViewTable[ta]*16+ba+8],8);
			memcpy(&chr[tileViewTable[ta]*16+ba+8],	&chr[tileViewTable[ta+1]*16+ba+8],8);
			memcpy(&chr[tileViewTable[ta+1]*16+ba+8],	tempchr,8);

			memcpy(tempchr,								&chr[tileViewTable[ta+16]*16+ba+8],8);
			memcpy(&chr[tileViewTable[ta+16]*16+ba+8],	&chr[tileViewTable[ta+17]*16+ba+8],8);
			memcpy(&chr[tileViewTable[ta+17]*16+ba+8],	tempchr,8);
		}
	}

	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);
	FormMain->UpdateMetaSprite();
}



void __fastcall TFormCHREditor::MirrorVertical(void)
{
	int i,pp;
	int x,y;
    int ba=bankActive;
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
			pp=tileViewTable[tileActive+y*16+x]*16+ba;
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
	if(b && tileSelRectWdt==2 && tileSelRectHgt==2)
	{
		//int pp=tileViewTable[tileActive]*16+ba;
		unsigned char tempchr[8];

		if(ButtonBitmaskLo->Down||!FormMain->Applytorotate1->Checked){
			memcpy(tempchr,						 	&chr[tileViewTable[ta]*16+ba],8);
			memcpy(&chr[tileViewTable[ta]*16+ba],	&chr[tileViewTable[ta+16]*16+ba],8);
			memcpy(&chr[tileViewTable[ta+16]*16+ba],tempchr,8);

			memcpy(tempchr,							&chr[tileViewTable[ta+1]*16+ba],8);
			memcpy(&chr[tileViewTable[ta+1]*16+ba], &chr[tileViewTable[ta+17]*16+ba],8);
			memcpy(&chr[tileViewTable[ta+17]*16+ba],tempchr,8);

		}

		if(ButtonBitmaskHi->Down||!FormMain->Applytorotate1->Checked){
			//pp+=8;
			memcpy(tempchr,						 	&chr[tileViewTable[ta]*16+ba+8],8);
			memcpy(&chr[tileViewTable[ta]*16+ba+8],	&chr[tileViewTable[ta+16]*16+ba+8],8);
			memcpy(&chr[tileViewTable[ta+16]*16+ba+8],tempchr,8);

			memcpy(tempchr,							&chr[tileViewTable[ta+1]*16+ba+8],8);
			memcpy(&chr[tileViewTable[ta+1]*16+ba+8], &chr[tileViewTable[ta+17]*16+ba+8],8);
			memcpy(&chr[tileViewTable[ta+17]*16+ba+8],tempchr,8);
		}
	}
	Draw(true);

	FormMain->UpdateNameTable(-1,-1,true);
	FormMain->UpdateTiles(false);
	FormMain->UpdateMetaSprite();
}



void __fastcall TFormCHREditor::Flip90(bool dir)
{
	int i,j,pp;
	int x,y;
	int xTiles=tileSelRectWdt;
	int yTiles=tileSelRectHgt;

	unsigned char tile[8][8],tile_flip[8][8];
	if (!(ButtonBitmaskLo->Down||ButtonBitmaskHi->Down))
		if(FormMain->Applytorotate1->Checked)return;
	FormMain->SetUndo();

	for (y = 0; y < yTiles; y++)
	{
		for (x = 0; x < xTiles; x++)
		{
			pp=tileViewTable[tileActive+y*16+x]*16+bankActive;
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
	pp=tileViewTable[tileActive+y*16+x]*16+bankActive;

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
	int pp=tileViewTable[tileActive]*16+bankActive;
	int pp1=tileViewTable[tileActive+1]*16+bankActive;
	int pp16=tileViewTable[tileActive+16]*16+bankActive;
	int pp17=tileViewTable[tileActive+17]*16+bankActive;


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

 }

void __fastcall TFormCHREditor::Fill(TShiftState Shift,int x,int y,int extpp, bool isNametable)                  //int tempPal, int fillPal,
{
	unsigned char buf[10][10];
	int i,j,pp,col,cnt;
	bool noncontiguous = ((Shift.Contains(ssAlt)) && (!Shift.Contains(ssShift)));
	int mask;
	int tempCol;
	int chrSelCount=0;

	if(noncontiguous)
	{
		for(i=0;i<256;i++) if(chrSelected[i])chrSelCount++;

		int x,y,w,h;

		if(isNametable)
		{
			for(int py=0;py<8;py++){
				for(int px=0;px<8;px++){
					pp=extpp+py;
					mask=128>>px;
					tempCol=(((chrBufFill[pp]<<px)&128)>>7)|(((chrBufFill[pp+8]<<px)&128)>>6);
					if (iSmudge==tempCol)
					{
						if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
						if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
					}
				}
			}
		}

		else{



			FormMain->GetSelection(chrSelection,x,y,w,h);
			//if(w>=0 && h>=0) //there is a selection

			if(chrSelectRect && w>1 && h>1)
			{
				for(int tx=0;tx<w;tx++){
					for(int ty=0;ty<h;ty++){
						for(int py=0;py<8;py++){
							for(int px=0;px<8;px++){
								pp=tileViewTable[tx+x + (ty+y)*16]*16 +bankActive+py;
								mask=128>>px;
								tempCol=(((chrBufFill[pp]<<px)&128)>>7)|(((chrBufFill[pp+8]<<px)&128)>>6);
								if (iSmudge==tempCol)
								{
									if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
									if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
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
						mask=128>>px;
						tempCol=(((chrBufFill[pp]<<px)&128)>>7)|(((chrBufFill[pp+8]<<px)&128)>>6);
						if (iSmudge==tempCol)
						{
							if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
							if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
						}
					}
				}

			}
			else
			{
				for(i=0;i<256;i++)
				{
					if(chrSelected[i])
					{
						for(int py=0;py<8;py++){
							for(int px=0;px<8;px++){
								pp=tileViewTable[i]*16+bankActive+py;
								mask=128>>px;
								tempCol=(((chrBufFill[pp]<<px)&128)>>7)|(((chrBufFill[pp+8]<<px)&128)>>6);
								if (iSmudge==tempCol)
								{
									if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
									if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
								}
							}
						}
					}
				}
			}
		}
		return;
	}



	if(extpp<0) pp=tileViewTable[tileActive]*16+bankActive;   //typical CHR editor behaviour
	else pp=extpp;                             //typical tileset editor (and 2x2 mode) behaviour
	//else pp=tileViewTable[extpp];

	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if(i==0||i==9||j==0||j==9)
			{
				buf[i][j]=255;
			}
			else
			{
				if ((penActive==0&&(!Shift.Contains(ssShift)))||extpp>=0)
						buf[i][j]=(chr[pp]&(128>>(j-1))?1:0)|(chr[pp+8]&(128>>(j-1))?2:0);
				else    buf[i][j]=(chrBufFill[pp]&(128>>(j-1))?1:0)|(chrBufFill[pp+8]&(128>>(j-1))?2:0);
			}
		}
		if(i>0&&i<9) pp++;
	}

	col=buf[y+1][x+1];
	if (penActive==0&&(!Shift.Contains(ssShift))) {
		if(col==bgPalCur) return;
	}
	else {} // reserved for new inc/dec fill behaviour.

	buf[y+1][x+1]=254;

	while(true)
	{
		cnt=0;
		for(i=1;i<9;i++)
		{
			for(j=1;j<9;j++)
			{
				if(buf[i][j]==254)
				{
					if(buf[i-1][j]==col) buf[i-1][j]=254;
					if(buf[i+1][j]==col) buf[i+1][j]=254;
					if(buf[i][j-1]==col) buf[i][j-1]=254;
					if(buf[i][j+1]==col) buf[i][j+1]=254;
					buf[i][j]=253;
					cnt++;
				}
			}
		}
		if(!cnt) break;
	}

	if (penActive==0&&(!Shift.Contains(ssShift)))
	{
		for(i=1;i<9;i++)
		{
			for(j=1;j<9;j++)
			{
				if(buf[i][j]==253) buf[i][j]=bgPalCur;
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

		for(i=1;i<9;i++)
		{
			for(j=1;j<9;j++)
			{
				if(buf[i][j]==253) buf[i][j]=fillPal;
			}
		}
	}


	if(extpp<0) pp=tileViewTable[tileActive]*16+bankActive;   //typical CHR editor behaviour
	else pp=extpp;                             //typical tileset editor behaviour
	//else pp=tileViewTable[extpp];

	for(i=1;i<9;i++)
	{
		chr[pp]=0;
		chr[pp+8]=0;
		for(j=1;j<9;j++)
		{
			chr[pp]|=(buf[i][j]&1)<<(8-j);
			chr[pp+8]|=((buf[i][j]&2)>>1)<<(8-j);
		}
		pp++;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormCHREditor::Line(TShiftState Shift,int mdx, int mdy, int x,int y,int mode)
{
  globalLineSenderMode=mode;

  //bool bSmear = ((GetKeyState(VK_CAPITAL) & 0x0001)!=0);
  bool bSmear = FormLineDetails->btnSmear->Down;
  bool bMove  = FormLineDetails->btnMove->Down;
  //bool bReuse = FormLineDetails->btnReuse->Down;
  bool bQuick = FormLineDetails->btnQuick->Down;

  //bool b=btn2x2mode->Down?1:0;
  unsigned char* ptr;

  bool bTaperEnable =FormLineDetails->CheckEnableBrush->Checked;
  bool bTaper2		=((FormLineDetails->btnTaper2->Down) && (bTaperEnable));
  bool bTaperIn		=((FormLineDetails->btnTaperIn->Down) && (bTaperEnable));
  bool bTaperOut	=((FormLineDetails->btnTaperOut->Down) && (bTaperEnable));
  bool bBestBresen  = ((!bTaper2) && lineToolAlt==0); //.

  //bool bUseBrush	=FormLineDetails->CheckEnableBrush->Checked;
  bool bBrush=btnThick->Down;
  int iThick		=(bBrush)	?4:1;

  int ix,iy;
  int tmp=lineToolRoster;
  int pw=128; 								   //width of plotmask canvas
  //for plot to chr transfer
  int pp;
  int mask;
  int tempPal;
  //for plot
  int mask1,mask2;
  int plotmask;
  //used for simple line/taper in/out
  int x0,x2,y0,y2; //used by application mask plotter
  int px0,px2,py0,py2;

  //used for brush
  int br_x0[4];
  int br_y0[4];
  int br_x2[4];
  int br_y2[4];



  //maybe needed, we'll see
  int tx0,tx1,ty0,ty1;

  int pp0, pp1;


  //refresh buffers
  memcpy (chr, chrBuf, 8192);

  
  //memset (,0,sizeof(arr_linePlotCHR));
  memcpy (arr_linePlotCHR,arr_linePlotCHR2,sizeof(arr_linePlotCHR2));
  //if(bBufVK_3) goto test;

  for(int i=0; i<iThick; i++)
  {
	if(bBrushMask[i]==false) continue;

	//figure out points - cursor to pixel
	if(mode==0) //CHR Editor mode
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
		x0 = (px0) 			+ ((mdx+o)<64+8+128?0:8)	+ (tileActive&15)*8;
		y0 = (py0) 			+ ((mdy+p)<64+8+128?0:8)	+ (tileActive/16)*8;
		x2 = (px2) 			+ (x<64+8+128?0:8)		+ (tileActive&15)*8;
		y2 = (py2) 			+ (y<64+8+128?0:8)	    + (tileActive/16)*8;
		//int x_dist= x2-x0;
		//int y_dist= y2-y0;


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
			else
			{
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
	if(i==0){ ix=0; iy=0;}
	if(i==1){ ix=1; iy=0;}
	if(i==2){ ix=0; iy=1;}
	if(i==3){ ix=1; iy=1;}


	int bi = 0;
	//if (bBrush) bi=-1;
	if (lineToolRoster==0) bi = 0;

	//if both,auto-midpoint
	if((bTaperIn) && (bTaperOut)) bTaper2=true;
	//manual use of midpoint
	if(bTaper2) bi=i;

	if((bTaper2) && ((!bTaperIn) && (!bTaperOut))) bi=0-i;



	br_x0[i] = x0 + (bTaperIn? 0 : ix);
	br_y0[i] = y0 + (bTaperIn? 0 : iy);
	br_x2[i] = x2 + (bTaperOut?  0 : ix);
	br_y2[i] = y2 + (bTaperOut?  0 : iy);




	ptr = arr_linePlotCHR;

	pw=128;


	/* //maybe reestablish for some tools' special
	float mpOff=lineToolMid;
	int disx=0;
	int disy=0;
	if (mpOff != 0.0) {
	// Calculate the displacement vector
	disx = (int)(mpOff / 16.0 * (x2 - x0));
	disy = (int)(mpOff / 16.0 * (y2 - y0));
	}
	*/

	//call appropriate plot

	switch (tmp) {
		case 3:
			Bresenham_angle(br_x0[i], br_y0[i],   br_x0[i]+lineToolX, br_y2[i]+lineToolY,   br_x2[i], br_y2[i], pw, ptr,false,true,bi);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterAngle->Top;
			break;
		case 2:
			Bresenham_angle(br_x0[i], br_y0[i],   br_x0[i]+lineToolAlt+lineToolX, br_y2[i]+lineToolAlt+lineToolY,   br_x2[i], br_y2[i], pw, ptr,true,true,bi);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterKnee->Top;
			break;
		case 1:

			//if(mode==1) plotQuadBezier(br_x0[i], br_y0[i],   br_x0[i]+lineToolAlt+lineToolX, br_y2[i]+lineToolAlt+lineToolY,   br_x2[i], br_y2[i], pw, ptr, bi);
			//else
			Bezier_curve(br_x0[i], br_y0[i],   br_x0[i]+lineToolAlt+lineToolX, br_y2[i]+lineToolAlt+lineToolY,   br_x2[i], br_y2[i], pw, ptr, bi);


			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterCurve->Top;
			break;
		case -1:
			Bezier_curve(br_x0[i], br_y0[i],   	  br_x2[i]-lineToolAlt+lineToolX, br_y0[i]-lineToolAlt+lineToolY,   br_x2[i], br_y2[i], pw, ptr, bi);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterCurve->Top;
			break;
		case -2:
			Bresenham_angle(br_x0[i], br_y0[i],   br_x2[i]-lineToolAlt+lineToolX, br_y0[i]-lineToolAlt+lineToolY,   br_x2[i], br_y2[i], pw, ptr,true,true,bi);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterKnee->Top;
			break;
		case -3:
			Bresenham_angle(br_x0[i], br_y0[i],   br_x2[i]+lineToolX, br_y0[i]+lineToolY,   br_x2[i], br_y2[i], pw, ptr,false,true,bi);
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterAngle->Top;
			break;
		default:
			if(bBestBresen)
			{
				//when a midpoint is not of use, a simple single line
				//is better, since this routine is then able to even the
				//inclination steps, which is often sought after in pixel art.

				Bresenham_line(br_x0[i]+lineToolX, br_y0[i]+lineToolY, br_x2[i], br_y2[i], pw, ptr,false);
			}
			else
			{
			  //less perfect, but OK for when a midpoint is required.
			  //can look a little odd when the line is a hypothenuse and the opposite and adjacent measures are odd.
			  //the errors that remain are somewhat obscured by the fact that is is only used
			  //when taper combos are used.
			  Bresenham_2line(br_x0[i]+lineToolX, br_y0[i]+lineToolY, br_x2[i], br_y2[i], pw, ptr, bi);
			}
			FormLineDetails->LineToolIndicator->Top=FormLineDetails->btnRosterLine->Top;
		}
	}

  FormLineDetails->LineToolIndicator->Top+1;
  if(tmp>=0) FormLineDetails->LineToolIndicator->Caption="`z";
  else       FormLineDetails->LineToolIndicator->Caption="`y";


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
  if(bQuick){memcpy (chrQuickLine,chr, 8192);}
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




				pp=tileViewTable[sx+tx+(sy+ty)*16]*16 + bankActive +py;
				mask=128>>px;

				int plotoff = (sx+tx)*8 + ((sy+ty)*8)*pw + px + (py)*pw;


				if((plotoff <0) || plotoff>sizeof(arr_linePlotCHR)) continue;

				plotmask =  arr_linePlotCHR[plotoff];


				tempPal=(((chrBuf[pp]<<px)&128)>>7)|(((chrBuf[pp+8]<<px)&128)>>6);

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
	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;
	bool bQuick = FormLineDetails->btnQuick->Down;

	bool b=btn2x2mode->Down?1:0;
	int n=b?128:0;
	int hi=64+8+128+n;
	int lo=64+8;
	int cm=3;   //tilechange margin generostiy
	int pm=4;   //paint area margin generosity

	//if(!Shift.Contains(ssCtrl)&&!Shift.Contains(ssRight)) FormMain->SetUndo();
	if(!Shift.Contains(ssRight)) FormMain->SetUndo();

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
			memcpy (chrBuf, chr, 8192); //preps a referencepoint for brushstrokes
			memcpy (chrBufFill, chr, 8192); //preps a reference point for inc/dec fill
		}
		if(bQuick || (Shift.Contains(ssCtrl)&&!bQuick))
		{
		   if(lineUpX<0) memcpy (chrBuf, chr, 8192);
		   else memcpy (chr, chrBuf, 8192);       //chrQuickLine
		   memcpy (chrBufFill, chr, 8192);
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
			int tx=X<64+8+128?0:1;
			int ty=Y<64+8+128?0:16;
			int px=((X-(64+8))/16)&7;
			int py=((Y-(64+8))/16)&7;
			if(btnQuant->Down){px=px&14; py=py&14;}

			int pp=tileViewTable[tileActive+tx+ty]*16+bankActive+py;


			iSmudge=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);


		}

		PaintBoxChrMouseMove(Sender,Shift,X,Y);
	}
	else{ lineUpX= -1; lineUpY= -1;}
	
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::PaintBoxChrMouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{
	if(bForbidPaint) return;
	bool b=btn2x2mode->Down?1:0;
	int n=b?128:0;
	int hi=64+8+128+n;
	int lo=64+8;
	//int cm=3;   //tilechange margin generostiy
	int pm=2;   //paint area margin generosity

	CHR_mmX=X;
	CHR_mmY=Y;

	int px,py,pp,mask;
	int tx,ty;
	int ix,iy; //pixel modifiers for thick pen
	int gx,gy,g; //guards for thick pen
	int iThick=btnThick->Down?4:1;
	int tempPal;


	CHREditorHover=0; //this can be repurposed later for identifying pixels and memory
					  //for now it just acts to give stats intel.
	cueStats=true;

	if((Y>=lo-pm && Y<hi+pm && X>=lo-pm && X<hi+pm) || (lineDrawing))
	{
		if(b){
		//this safeguards drawing outside intended memory when the user manually forces
		//2x2 mode to point towards a down/right edge of the tilesheet

		//because safeguards have been put in place on another level,
		//this is probably a redundancy at this point.

			bool btx=X<64+8+128?false:true;
			bool bty=Y<64+8+128?false:true;

			int ox=(tileActive&15)+btx;
			int oy=(tileActive/16)+bty;

			if(ox>15||oy>15) return;
		}

			/*
		if(lineDrawing){
			if (X<64+8) 		X=64+8;
			if (X>=64+8+128+n)	X=63+8+128+n;
			if (Y<64+8)        Y=64+8;
			if (Y>=64+8+128+n)   Y=63+8+128+n;
		} */
		X= (X >= lo)? X:lo;
		X= (X < hi)? X:hi-1;
		Y= (Y >= lo)? Y:lo;
		Y= (Y < hi)? Y:hi-1;
		//--

		//tx=X<64+8+128?0:16;
		//ty=Y<64+8+128?0:16*16;
		tx=X<64+8+128?0:1;
		ty=Y<64+8+128?0:16;


		px=((X-(64+8))/16)&7;
		py=((Y-(64+8))/16)&7;
		if(btnQuant->Down){px=px&14; py=py&14;}

		//pp=tileActive*16+py+bankActive+tx+ty;
		pp=tileViewTable[tileActive+tx+ty]*16+bankActive+py;

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
             
			 for(int i=0; i<iThick; i++)
			 {

				if(btnThick->Down)
				{
					if(bBrushMask[i]==false) continue;

					if(i==0){ ix=0; iy=0;}
					if(i==1){ ix=16; iy=0;}
					if(i==2){ ix=0; iy=16;}
					if(i==3){ ix=16; iy=16;}
					//recalc pixel to be edited

					//tx=X<64+ix+128	?0:16;
					//ty=Y<64+iy+128	?0:16*16;

					tx=X<64+ix+128	?0:1;
					ty=Y<64+iy+128	?0:16;

					px=((X-(64+ix))/16)&7;
					py=((Y-(64+iy))/16)&7;

					gx=((X-(64+ix))/16);
					gy=((Y-(64+iy))/16);

					if(b)g=15; else g=7;
					if(!btnQuant->Down){
						if((i==1||3)&&gx>g) continue;
						if((i==2||3)&&gy>g) continue;
					}
					if(btnQuant->Down){
						px=((X-(64+8))/16)&6;
						py=((Y-(64+8))/16)&6;

						//tx=X<64+8+128	?0:16;
						//ty=Y<64+8+128	?0:16*16;
						tx=X<64+8+128	?0:1;
						ty=Y<64+8+128	?0:16;

						if(i==1){px++;}
						if(i==2){py++;}
						if(i==3){px++; py++;}
					}



					//pp=tileActive*16+py+bankActive+tx+ty;
					pp=tileViewTable[tileActive+tx+ty]*16+bankActive+py;
					mask=128>>px;

				}



					tempPal=(((chrBuf[pp]<<px)&128)>>7)|(((chrBuf[pp+8]<<px)&128)>>6);
					fillPal=(((chrBufFill[pp]<<px)&128)>>7)|(((chrBuf[pp+8]<<px)&128)>>6);



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
							memcpy (chrBuf, chr, 8192);
						}
					}

					if(!Shift.Contains(ssCtrl))
					{
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
						if(b)pp=tileViewTable[tileActive+tx+ty]*16+bankActive; else pp = -1;
						Fill(Shift,px,py,pp,false); //tempPal,fillPal
					}
				}
			}

			cueUpdateMetasprite=true;
			cueUpdateNametable=true;
			cueUpdateTiles=true;
			cueCHRdraw=true;
			//if(b)cueCHRdraw=true;

			//if(!b)Draw(false);
			//FormMain->UpdateNameTable(-1,-1,true);
			//FormMain->UpdateTiles(false);
			//FormMain->UpdateMetaSprite();
		}

		if(Shift.Contains(ssRight))
		{
			bgPalCur=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);
			FormMain->DrawPalettes();
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
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
			if(Key=='A'||Key==VK_LEFT)  ScrollHorz(true);
			if(Key=='D'||Key==VK_RIGHT) ScrollHorz(false);
			if(Key=='W'||Key==VK_UP)    ScrollVert(false);
			if(Key=='S'||Key==VK_DOWN)  ScrollVert(true);



		}
		if(!lineDrawing)
		{
			if(Key=='5') {Protect0->Down^=true; FormMain->Protect0->Down = Protect0->Down;}
			if(Key=='6') {Protect1->Down^=true; FormMain->Protect1->Down = Protect1->Down;}
			if(Key=='7') {Protect2->Down^=true; FormMain->Protect2->Down = Protect2->Down;}
			if(Key=='8') {Protect3->Down^=true; FormMain->Protect3->Down = Protect3->Down;}
			//if(Key=='V') MirrorVertical();    //this is an alias for T
			if(Key=='R') SpeedButtonRotateCWClick(Sender);//{Flip90(false); Rotate4tiles(false);}
			if(Key=='G') {btnSmudge->Down=true; FormMain->btnSmudge->Down=true; bSmudge=true;}
			if(Key=='T') FormLineDetails->btnSmear->Down^=true;
			if(Key=='K') FormLineDetails->btnQuick->Down^=true;
		}


		//if(Key=='H') MirrorHorizontal();
		
		if(Key=='L') SpeedButtonRotateCCWClick(Sender);//{Flip90(true); Rotate4tiles(true);}

		if(Key=='B') {btnThick->Down^=true; FormMain->btnThick->Down = btnThick->Down;}
		if(Key=='N') {btnLine->Down^=true;   FormMain->btnLine->Down = btnLine->Down;}
		if(Key=='U') {btnQuant->Down^=true;  FormMain->btnQuant->Down = btnQuant->Down;}
		if(Key=='M') FormBrush->Show();

		if(Key=='I') ButtonBitmaskHi->Down^=true;
		if(Key=='O') ButtonBitmaskLo->Down^=true;
		
		if(Key=='P') SpeedButtonDoWrap->Down^=true;
		if(Key=='Q') FormMain->Show();
		if(Key=='X') FormMain->Toggletileset1Click(FormMain->Toggletileset1);

		if(Key=='E') {btn2x2mode->Down^=true;}

		if(Key==VK_OEM_4||Key==VK_OEM_COMMA) FormMain->SpeedButtonPrevMetaSpriteClick(Sender);// [
		if(Key==VK_OEM_6||Key==VK_OEM_PERIOD) FormMain->SpeedButtonNextMetaSpriteClick(Sender);// ]

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
	FormMain->UpdateMetaSprite();
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
	FormMain->UpdateMetaSprite();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormCreate(TObject *Sender)
{
    memset (arr_linePlotCHR,0,sizeof(arr_linePlotCHR));
	memset (arr_linePlotCHR2,0,sizeof(arr_linePlotCHR2));


	DoubleBuffered=true;

	GroupBox1->DoubleBuffered=true;
	if(FormCHREditor->Position==poDesigned)
			{
			FormCHREditor->Left=(Screen->Width+FormMain->Width)/2;
			FormCHREditor->Top=(Screen->Height-FormMain->Height)/2;
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
	if(l &&(t==false))
	{
		if(!bCtrl){
			if(bAlt)   //just alt pressed
			{
				if(tmpD<0) lineToolAlt++;
				else lineToolAlt--;
			}
			else{     //nothing pressed
				lineToolAlt=0;
				lineToolX=0;
				lineToolY=0;
				if(tmpD<0) lineToolRoster--;
				else lineToolRoster++;

				for (int i = 0; i < 3; i++) {
					absl = abs(lineToolRoster);
					if (lineRosterEnable[absl]==false)
					{
						if(tmpD<0) lineToolRoster--;
						else lineToolRoster++;
					}
				}
				if (lineToolRoster > 3) lineToolRoster = 3;
				if (lineToolRoster < -3) lineToolRoster = -3;
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
				if(tmpD<0) lineToolY++;
				else lineToolY--;
			}
		}
		PaintBoxChrMouseMove(Sender,Shift,CHR_mmX,CHR_mmY);

		FormLineDetails->Repaint();

		TimerScrollEvent->Enabled;

	}
	else
	{
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
   int x,y;
   int xTiles=tileSelRectWdt;
   int yTiles=tileSelRectHgt;

	FormMain->SetUndo();

	for (y = 0; y < yTiles; y++)
	{
		for (x = 0; x < xTiles; x++)
		{
			pp=tileActive*16+bankActive+y*256+x*16;
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
	FormMain->UpdateMetaSprite();
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
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::FormDeactivate(TObject *Sender)
{
	FormMain->Attributes1->Enabled=true;
	FormMain->SelectedOnly1->Enabled=true;
	FormMain->ApplyTiles1->Enabled=true;
	FormMain->ApplyAttributes1->Enabled=true;
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
}
//---------------------------------------------------------------------------

void __fastcall TFormCHREditor::btn2x2modeClick(TObject *Sender)
{
	bool b=btn2x2mode->Down;
	int n=b?128:0;

	FormCHREditor->Width =originalFormWdt+n;
	FormCHREditor->Height=originalFormHgt+n;

	GroupBox1->Width	=originalGroupBoxWdt+n;
	GroupBox1->Height	=originalGroupBoxHgt+n;

	PaintBoxChr->Width  =originalPaintBox+n;
	PaintBoxChr->Height  =originalPaintBox+n;

	if(b)
	{
		if((tileActive&15)==15) tileActive--;
		if(tileActive>=0xF0) tileActive-=16;
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

			if(n5 && !n5p) {Protect0->Down^=true;	key=true;}
			if(n6 && !n6p) {Protect1->Down^=true;	key=true;}
			if(n7 && !n7p) {Protect2->Down^=true;	key=true;}
			if(n8 && !n8p) {Protect3->Down^=true;	key=true;}


			if(n9 && !n9p) {key=true;}
			if(n0 && !n0p) {key=true;}

			if ((vkSmear && !vkpSmear) || (t && !tp))
				{FormLineDetails->btnSmear->Down^=true; key=true;}



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


			if ((vkQuick && !vkpQuick) || (k && !k))   {FormLineDetails->btnQuick->Down^=true;  key=true;}


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
	if(Sender==btnLine)FormMain->btnLine->Down = btnLine->Down;
	if(Sender==btnQuant)FormMain->btnQuant->Down = btnQuant->Down;
	if(Sender==btnThick)FormMain->btnThick->Down = btnThick->Down;
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

