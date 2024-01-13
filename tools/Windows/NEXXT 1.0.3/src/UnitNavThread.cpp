//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitNavThread.h"
#include "UnitMain.h"
#include "UnitNavigator.h"
#pragma package(smart_init)

extern bool bProcessDrawNavOn;

extern unsigned char nameTable[NAME_MAX_SIZE];
extern unsigned char attrTable[ATTR_MAX_SIZE];
extern unsigned char nameCopy[NAME_MAX_SIZE];
extern unsigned char chrSelected[256];
extern int nameTableViewX;
extern int nameTableViewY;
extern int nxDown;
extern int nyDown;
extern int nameXC;
extern int nameYC;

extern int nullTile;
extern int tileActive;

extern int nameTableWidth;
extern int nameTableHeight;

extern int tileSelRectWdt;
extern int tileSelRectHgt;

extern TRect nameSelection;
extern TRect chrSelection;
extern TRect nameSelBuf;
extern TRect destRect;

extern bool isLastClickedMetaSprite;
extern bool	isLastClickedSpriteList;
extern bool	isLastClickedNametable;
extern bool	isLastClickedTiles;


extern bool bMouseOverNav;
extern bool bOutsideSel;
extern bool clickV;
extern bool chrSelectRect;

extern bool cueStats;
extern bool cueUpdateTiles;
extern bool cueUpdateNametable;
extern bool cueUpdateNametableNoRepaint;

extern bool mouseDraggedNTSel;

extern bool bDrawDestShadow;
extern bool	bImageNameAccepted;
extern bool	bImageTileAccepted;

extern bool bBufCtrl;
extern bool bBufShift;
extern bool bBufAlt;

extern bool bSnapToScreen;

extern int nameTableViewXbuf;
extern int nameTableViewYbuf;

extern bool bHalfsize;
//extern TImage *Map1;
extern Graphics::TBitmap *bufBmp;
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TBulkDrawNav::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TBulkDrawNav::TBulkDrawNav(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void TBulkDrawNav::SetName()
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = "NEXXT_BulkDrawNav";
	info.dwThreadID = -1;
	info.dwFlags = 0;

	__try
	{
		RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),(DWORD*)&info );
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall TBulkDrawNav::Execute()
{
	SetName();
	//---- Place thread code here ----
	

	TRect r;
	int w=nameTableWidth;
	int h=nameTableHeight;
	int minW= 32 >= w	?	32:w;
	int minH= 30 >= h	?	30:h;

		//begin draw
	int y,x,i,j,tile;
	//


		y=0;

		for(i=0;i<minH;i++)
		{
			x=0;

			for(j=0;j<minW;j++)
			{
				if(j>=0&&j<w&&i>=0&&i<h)
				{
					tile=nameTable[i*w+j];

					if(FormMain->SpeedButtonChecker->Down)
					{
						FormMain->DrawTileChecker(FormNavigator->Map1->Picture,x,y,tile,FormMain->AttrGet(j,i,false,false),j,i,false,false,1);
					}
					else
					{
						if(!FormMain->SpeedButtonSelTiles->Down||chrSelected[tile])
						{
							FormMain->DrawTile(FormNavigator->Map1->Picture,x,y,tile,FormMain->AttrGet(j,i,false,false),j,i,false,false,1,true,bHalfsize,false);
						}
						else
						{
							FormMain->DrawEmptyTile(FormNavigator->Map1->Picture,x,y,FormMain->AttrGet(j,i,false,false),j,i,false,false,1);
						}
					}
				}
				else
				{
					FormMain->DrawEmptyTile(FormNavigator->Map1->Picture,x,y,0,j,i,false,false,1);
				}


				x+=8;
			}


			y+=8;
		}


  bufBmp->Assign(FormNavigator->Map1->Picture->Bitmap);


 if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		r.left  =nameSelection.left;
		r.right =nameSelection.right;
		r.top   =nameSelection.top;
		r.bottom=nameSelection.bottom;


		FormMain->DrawSelection(FormNavigator->Map1,r,1,bHalfsize,true);
	}

 if(bMouseOverNav){
	r.left  	=(nameTableViewX);
	r.right 	=(nameTableViewX+32);
	r.top   	=(nameTableViewY);
	r.bottom	=(nameTableViewY+30);



	if(nameTableWidth<32)
	{
		r.left=r.left +((32-w)/2);
		r.right=r.right +((32-w)/2);
	}
	if(nameTableHeight<32)
	{
		r.top=r.top + ((32-h)/2);
		r.bottom=r.bottom +((32-h)/2);
	}

	r.left  	=r.left 	*(8*1);
	r.right 	=r.right	*(8*1);
	r.top   	=r.top		*(8*1);
	r.bottom	=r.bottom	*(8*1);

	FormNavigator->Map1->Canvas->Brush->Style=bsClear;
	FormNavigator->Map1->Canvas->Pen->Color=TColor(0xffff00);
	FormNavigator->Map1->Canvas->Rectangle(r);

	r.left  +=1;
	r.top   +=1;
	r.right -=1;
	r.bottom-=1;


	if (bHalfsize) FormNavigator->Map1->Canvas->Pen->Color=TColor(0xffff00);
	else FormNavigator->Map1->Canvas->Pen->Color=TColor(0x444400);


	FormNavigator->Map1->Canvas->Rectangle(r);

	r.left  -=2;
	r.top   -=2;
	r.right +=2;
	r.bottom+=2;

    if 	(nameSelection.left  ==nameTableViewX
		&&  nameSelection.top   ==nameTableViewY
		&&	nameSelection.right ==nameTableViewX+32
		&&	nameSelection.bottom==nameTableViewY+30)
	{FormNavigator->Map1->Canvas->Pen->Color=TColor(0xffffff);}
	else FormNavigator->Map1->Canvas->Pen->Color=TColor(0x444400);
	FormNavigator->Map1->Canvas->Rectangle(r);
 }

 //FormNavigator->Map1->Picture->Bitmap->Assign(bufBmp);
	FormNavigator->Map1->Repaint();
	bProcessDrawNavOn=false;

}
//---------------------------------------------------------------------------

