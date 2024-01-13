//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "UnitMain.h"
#include "UnitNavigator.h"
#include "UnitNavThread.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormNavigator *FormNavigator;

Graphics::TBitmap *bufBmp;
int cueChunkX=0;
int cueChunkY=0;

extern bool openByFileDone;
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

int nameTableViewXbuf;
int nameTableViewYbuf;

int windowSizeSnapRange = 17;    //17 is a nice distance + the standard width of a scrollbar, which works to our goals.
//const float cHalf = 4;
//const int cFull = 8;
bool bHalfsize;   //mostly used to draw double width lines for selections and grids and such when scaling down
float fStretchScale = 1;

bool bClickedInsideScreen;
//---------------------------------------------------------------------------
__fastcall TFormNavigator::TFormNavigator(TComponent* Owner)
	: TForm(Owner)
{

}



//---------------------------------------------------------------------------
void __fastcall TFormNavigator::Draw(bool doSnap, bool doAlign)
{

	//int div = bHalfsize?2:1;
	//if(FormMain->IsBlockDrawing()) return;
	if(!openByFileDone) return;

	TRect r;

	int w=nameTableWidth;
	int h=nameTableHeight;
	int minW= 32 >= w	?	32:w;
	int minH= 30 >= h	?	30:h;          

	Map1->Stretch=true;
	Map1->Width=minW*8*fStretchScale;
	Map1->Height=minH*8*fStretchScale;
	Map1->Picture->Bitmap->PixelFormat=pf24bit;
	Map1->Picture->Bitmap->SetSize(minW*8,minH*8);

	//buffer
	bufBmp->SetSize(minW*8,minH*8);

	int lrAlign = (FormNavigator->ClientWidth - Map1->Width)/2;
	int tbAlign = (FormNavigator->ClientHeight - Map1->Height)/2;

	


	//Snap window to fit canvas if close enough
	if(doSnap){
		//17 too large, 17 too small (17 defined by windowSizeSnapRange)
		if(abs(FormNavigator->ClientWidth - minW*8*fStretchScale)<windowSizeSnapRange)  FormNavigator->ClientWidth=minW*8*fStretchScale;
		if(abs(FormNavigator->ClientHeight - minH*8*fStretchScale)<windowSizeSnapRange) FormNavigator->ClientHeight=minH*8*fStretchScale;

		//17*2 too small -arbitrarily chosen based on what feels good.
		if(FormNavigator->ClientWidth - minW*8*fStretchScale<windowSizeSnapRange*2)  FormNavigator->ClientWidth=minW*8*fStretchScale;
		if(FormNavigator->ClientHeight - minH*8*fStretchScale<windowSizeSnapRange*2) FormNavigator->ClientHeight=minH*8*fStretchScale;

        if(FormNavigator->ClientHeight > GetSystemMetrics(SM_CYFULLSCREEN)) FormNavigator->ClientHeight  = GetSystemMetrics(SM_CYFULLSCREEN);
	}
	int barVal=windowSizeSnapRange;

	if(doAlign)
	{
		if(FormNavigator->ClientWidth > Map1->Width) Map1->Left=lrAlign;
		else Map1->Left=0;

		if(FormNavigator->ClientHeight-barVal*2>Map1->Height) Map1->Top=tbAlign;
		else Map1->Top=0;
	}

	//a smoother auto-scrollbar behaviour than the GUI library provides out of the box.



	FormNavigator->AutoScroll=
		FormNavigator->ClientHeight+barVal<Map1->Height
		|| FormNavigator->ClientWidth+barVal<Map1->Width
		?true:false;


	//todo: figure out how to use threaded objects properly

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
        						FormMain->DrawTileChecker(Map1->Picture,x,y,tile,FormMain->AttrGet(j,i,false,false),j,i,false,false,1);
        					}
        					else
        					{
        						if(!FormMain->SpeedButtonSelTiles->Down||chrSelected[tile])
        						{
									FormMain->DrawTile(Map1->Picture,x,y,tile,FormMain->AttrGet(j,i,false,false),j,i,false,false,1,true,bHalfsize,false);
        						}
        						else
        						{
        							FormMain->DrawEmptyTile(Map1->Picture,x,y,FormMain->AttrGet(j,i,false,false),j,i,false,false,1);
        						}
        					}
        				}
        				else
        				{
        					FormMain->DrawEmptyTile(Map1->Picture,x,y,0,j,i,false,false,1);
        				}
        
        
        				x+=8;
        			}
        
        
        			y+=8;
        		}
        

		  bufBmp->Assign(Map1->Picture->Bitmap);
        
        
         if(nameSelection.left>=0&&nameSelection.top>=0)
        	{
        		r.left  =nameSelection.left;
        		r.right =nameSelection.right;
        		r.top   =nameSelection.top;
        		r.bottom=nameSelection.bottom;
        
        
        		FormMain->DrawSelection(Map1,r,1,bHalfsize,true);
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
        
        	Map1->Canvas->Brush->Style=bsClear;
        	Map1->Canvas->Pen->Color=TColor(0xffff00);
        	Map1->Canvas->Rectangle(r);
        
        	r.left  +=1;
        	r.top   +=1;
        	r.right -=1;
        	r.bottom-=1;
        
        
        	if (bHalfsize) Map1->Canvas->Pen->Color=TColor(0xffff00);
        	else Map1->Canvas->Pen->Color=TColor(0x444400);
        
        
        	Map1->Canvas->Rectangle(r);
        
        	r.left  -=2;
        	r.top   -=2;
        	r.right +=2;
        	r.bottom+=2;
        
            if 	(nameSelection.left  ==nameTableViewX
        		&&  nameSelection.top   ==nameTableViewY
        		&&	nameSelection.right ==nameTableViewX+32
        		&&	nameSelection.bottom==nameTableViewY+30)
        	{Map1->Canvas->Pen->Color=TColor(0xffffff);}
        	else Map1->Canvas->Pen->Color=TColor(0x444400);
        	Map1->Canvas->Rectangle(r);
         }
        
		 //Map1->Picture->Bitmap->Assign(bufBmp);
			Map1->Repaint();

			tempskip:

 }
void __fastcall TFormNavigator::DrawRange(int tx,int ty,int tw,int th, bool repaint)
{
   if(bProcessDrawNavOn) return;  //not in use; multithreading is still an experiment.
   Map1->Picture->Bitmap->Assign(bufBmp);
   int i,j,x,y,tile;
   int w=nameTableWidth;
   int h=nameTableHeight;
   int cw = Map1->Picture->Width;
   int ch = Map1->Picture->Height;

		y=ty*8;

		for(i=0;i<th;i++)
		{
			x=tx*8;

			for(j=0;j<tw;j++)
			{

				//todo: encapsulate in safeguard.
				 if(x>=cw) continue;
				 if(y>=ch) continue;

				if(j>=0 && tx+j<w && i>=0 && ty+i<h) 
				{
					tile=nameTable[(ty+i)* w +tx+j];

					if(FormMain->SpeedButtonChecker->Down)
					{
						FormMain->DrawTileChecker(Map1->Picture,x,y,tile,FormMain->AttrGet(tx+j,ty+i,false,false),tx+j,ty+i,false,false,1);
					}
					else
					{
						if(!FormMain->SpeedButtonSelTiles->Down||chrSelected[tile])
						{
							FormMain->DrawTile(Map1->Picture,x,y,tile,FormMain->AttrGet(tx+j,ty+i,false,false),tx+j,ty+i,false,false,1,true,bHalfsize,false);
						}
						else
						{
							FormMain->DrawEmptyTile(Map1->Picture,x,y,FormMain->AttrGet(tx+j,ty+i,false,false),tx+j,ty+i,false,false,1);
						}
					}
				}
				else
				{
					FormMain->DrawEmptyTile(Map1->Picture,x,y,0,tx+j,ty+i,false,false,1);
				}


				x+=8;
			}


			y+=8;
		}



  if(repaint) Map1->Repaint();
  bufBmp->Assign(Map1->Picture->Bitmap);
  UpdateLines(false);
}
void __fastcall TFormNavigator::UpdateLines(bool getBuffer)
{
    TRect r;

	int w=nameTableWidth;
	int h=nameTableHeight;



	if(getBuffer) {
	Map1->Picture->Bitmap->Assign(bufBmp);
	}
    if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		r.left  =nameSelection.left;
		r.right =nameSelection.right;
		r.top   =nameSelection.top;
		r.bottom=nameSelection.bottom;


		FormMain->DrawSelection(Map1,r,1,bHalfsize,true);
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

	Map1->Canvas->Brush->Style=bsClear;
	Map1->Canvas->Pen->Color=TColor(0xffff00);
	Map1->Canvas->Rectangle(r);

	r.left  +=1;
	r.top   +=1;
	r.right -=1;
	r.bottom-=1;


	if (bHalfsize) Map1->Canvas->Pen->Color=TColor(0xffff00);
	else Map1->Canvas->Pen->Color=TColor(0x444400);


	Map1->Canvas->Rectangle(r);

	r.left  -=2;
	r.top   -=2;
	r.right +=2;
	r.bottom+=2;

    if 	(nameSelection.left  ==nameTableViewX
		&&  nameSelection.top   ==nameTableViewY
		&&	nameSelection.right ==nameTableViewX+32
		&&	nameSelection.bottom==nameTableViewY+30)
	{Map1->Canvas->Pen->Color=TColor(0xffffff);}
	else Map1->Canvas->Pen->Color=TColor(0x444400);
	Map1->Canvas->Rectangle(r);
 }

	Map1->Repaint();
}

void __fastcall TFormNavigator::FormShow(TObject *Sender)
{
	if(bHalfsize) FormNavigator->Caption="Navigator | 50%";
	else FormNavigator->Caption="Navigator | 100%";
	Draw(true,true);
}
//---------------------------------------------------------------------------
void __fastcall TFormNavigator::FormResize(TObject *Sender)
{
    //note: FormResize is called once on win7 classic mode, but on every registered mouse move+resize event on win10

	Draw(false,true);
    if(FormNavigator->Height > GetSystemMetrics(SM_CYFULLSCREEN)) FormNavigator->ClientHeight  = GetSystemMetrics(SM_CYFULLSCREEN);
	//if user is hesitant in the "goldilocks snap zone" for half a second, suggest snap.
	ResizeTimer->Enabled=false;  //reset timer on every resize event
	ResizeTimer->Interval=500;
	ResizeTimer->Enabled=true;

}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::Map1DblClick(TObject *Sender)
{
	if(bBufShift){
		bHalfsize=false;
		if (fStretchScale==2) {
			FormNavigator->Caption="Navigator | 100%";
			if(fStretchScale>1)
			{
				FormNavigator->ClientWidth/=2;
				FormNavigator->ClientHeight/=2;
			}
			fStretchScale = 1;
		}
		else
		{
			FormNavigator->Caption="Navigator | 200%";
			fStretchScale = 2;
		}
	}
	else {
		bHalfsize^=true;
		if(bHalfsize) {
			FormNavigator->Caption="Navigator | 50%";
			if (fStretchScale>1)       { FormNavigator->ClientWidth/=4; FormNavigator->ClientHeight/=4;}
			else if(fStretchScale>0.5) { FormNavigator->ClientWidth/=2; FormNavigator->ClientHeight/=2;}
			fStretchScale = 0.5;
		}
		else {
			FormNavigator->Caption="Navigator | 100%";
			if(fStretchScale>1)
			{
				FormNavigator->ClientWidth/=2;
				FormNavigator->ClientHeight/=2;
			}

			fStretchScale = 1;
		}
	}

	Draw(true,true);
}
//---------------------------------------------------------------------------



void __fastcall TFormNavigator::ResizeTimerTimer(TObject *Sender)
{
  if(!openByFileDone) return;
  Draw(true,false);
  Draw(true,false); //second pass.
  ResizeTimer->Enabled=false;
}
//---------------------------------------------------------------------------


void __fastcall TFormNavigator::FormCanResize(TObject *Sender, int &NewWidth,
	  int &NewHeight, bool &Resize)
{
	//snaps quickly after accepted resize if in the "goldilocks zone"
	//ResizeTimer->Interval=100;
	//ResizeTimer->Enabled=true;

	
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::CueDrawTimerTimer(TObject *Sender)
{
	if(!openByFileDone) return;
	Draw(false,false);
	CueDrawTimer->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::Map1MouseEnter(TObject *Sender)
{
	bMouseOverNav=true;
	Draw(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::Map1MouseLeave(TObject *Sender)
{
	bMouseOverNav=false;
	Draw(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	FormMain->FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::Map1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	 isLastClickedMetaSprite=false;
	 isLastClickedSpriteList=false;
	 isLastClickedNametable=true;
	 isLastClickedTiles=false;

	 bOutsideSel=false;
	 //+ Map1->Left
	 float div = fStretchScale*8;     //4: 50%, 8: 100%, 16: 200%
	 int nx = X / div;
	 int ny = Y / div;
	 nxDown=nx;
	 nyDown=ny;

     

	 //if(nameXC<0||nameXC>=nameTableWidth||nameYC<0||nameYC>=nameTableHeight) return;
	if (Shift.Contains(ssRight)) { //prep context for inside/outside selection
		FormMain->SelectTile(nameTable[nx*nameTableWidth+ny]);
		FormMain->SelectPalette(FormMain->AttrGet(nx,ny,false,false));
		FormMain->UpdateTiles(true);
		if((nameSelection.right<=nx)
				|(nameSelection.left>nx)
				|(nameSelection.bottom<=ny)
				|(nameSelection.top>ny))
		{
					bOutsideSel=true;
					nameSelection.left=-1;
					nameSelection.top =-1;



		}
		else{
			if(!Shift.Contains(ssShift) && nameSelection.left != -1 && nameSelection.top != -1)
				Map1->BeginDrag(false,-1);

			nameSelBuf.left		=	nameSelection.left;
			nameSelBuf.top		=	nameSelection.top;
			nameSelBuf.right	=	nameSelection.right;
			nameSelBuf.bottom  	=	nameSelection.bottom;
			//these are probably redundant
			destRect.left		=	nameSelection.left;
			destRect.top		=	nameSelection.top;
			destRect.right		=	nameSelection.right;
			destRect.bottom  	=	nameSelection.bottom;

			

		}
		//FormMain->UpdateNameTable(-1,-1,true);
		//Draw(false,false);
        FormMain->NameLinesTimer->Enabled=true;
		CueLinesTimer->Enabled=true;
	}

	else if(Shift.Contains(ssLeft)&&(clickV))  //Quickpaste/V-paste
	{
		//potential future feature: put the below tasks here but have them be
		//legal only for a !modulo of the copy w/h.
		//Right now they´re in the MouseDown event.
		FormMain->SetUndo();

		//set selection
		nameSelection.left  =nx;
		nameSelection.top   =ny;
		nameSelection.right =nameSelection.left;
		nameSelection.bottom=nameSelection.top;

		for(int i=0;i<256;++i) chrSelected[i]=0;

		chrSelected[tileActive]=1;
		chrSelectRect=true;
		FormMain->PasteMap();
		//deselect
		nameSelection.left=-1;     //make a conditional that happens before lineUpdate?.
		nameSelection.top =-1;

		cueStats=true;
		UpdateLines(false);
		//Draw(false,false);   // bookmark: this could use a local update
		return;
	}
	else if(Shift.Contains(ssShift)&&Shift.Contains(ssLeft))   //begin selection
	{

		nameSelection.left  =nx;
		nameSelection.top   =ny;
		nameSelection.right =nameSelection.left+1;
		nameSelection.bottom=nameSelection.top +1;

		

		chrSelection.right =chrSelection.left+1;
		chrSelection.bottom=chrSelection.top +1;

		for(int i=0;i<256;++i) chrSelected[i]=0;

		chrSelected[tileActive]=1;
		chrSelectRect=true;

		tileSelRectWdt=1;
		tileSelRectHgt=1;

		FormMain->UpdateTiles(true);
		FormMain->UpdateNameTable(-1,-1,true);

		UpdateLines(true);

	}
	else if (Shift.Contains(ssLeft))  //drag view
	{
		if((nameTableViewX+32<=nx)
				|(nameTableViewX>nx)
				|(nameTableViewY+30<=ny)
				|(nameTableViewY>ny))
		{
				//mouse cursor is outside screen cursor
				bClickedInsideScreen=false;
				Map1MouseMove(Sender,Shift,X,Y);
		}
		else
		{
			bClickedInsideScreen=true;

			nameTableViewXbuf=nameTableViewX;
			nameTableViewYbuf=nameTableViewY;
			Map1MouseMove(Sender,Shift,X,Y);
		}
	}
	else
	{
		//if(Shift.Contains(ssLeft)) //drag viewport
		//FormMain->SetUndo(); //under what conditions would we want to do it?
		Map1MouseMove(Sender,Shift,X,Y);
	}
	cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::Map1MouseMove(TObject *Sender,
	  TShiftState Shift, int X, int Y)
{
	float div = fStretchScale*8;//bHalfsize?cHalf:cFull;
	int nx = X/ div;
	int ny = Y/ div;

	nameXC=nx;  //used for interaction with old code in the main canvas
	nameYC=ny;

	if(nameXC<0||nameXC>=nameTableWidth||nameYC<0||nameYC>=nameTableHeight)
	{
		nameXC=-1;
		nameYC=-1;

		cueStats=true;
		return;
	}

	if(Shift.Contains(ssLeft)) if(FormMain->MouseTypeIn(X,Y)) return; //should this be in mousedown too?
	if(Shift.Contains(ssLeft)) if(clickV) return;
	if(!FormNavigator->Active) return;
	//layout mode begins:

	if(Shift.Contains(ssShift))
	{
		if(Shift.Contains(ssLeft))
		{

			mouseDraggedNTSel=true;

			if(nameXC<nxDown)   {nameSelection.left=nameXC+1-(nameXC<nameSelection.right?1:0);
				nameSelection.right=nxDown+1;
				}
			if(nameXC>=nxDown) {nameSelection.right =nameXC+(nameXC>=nameSelection.left?1:0);
				nameSelection.left=nxDown;
				}
			if(nameYC<nyDown)  {nameSelection.top=nameYC-(nameYC>=nameSelection.bottom ?1:0);
				nameSelection.bottom=nyDown+1;
			    }
			if(nameYC>=nyDown) {nameSelection.bottom=nameYC+(nameYC>=nameSelection.top ?1:0);
				nameSelection.top=nyDown;				  
				}
			//FormMain->UpdateNameTable(-1,-1,true);
			//cueUpdateNametable=true;
			//cueUpdateNametableNoRepaint=true;
			//CueDrawTimer->Enabled=true;
			FormMain->NameLinesTimer->Enabled=true;
			CueLinesTimer->Enabled=true;
		}
        else if(Shift.Contains(ssRight))
		{
			if(bOutsideSel){
				nameSelection.left=-1;
				nameSelection.top =-1;
				cueUpdateNametable=true;
				//CueDrawTimer->Enabled=true;
				CueLinesTimer->Enabled=true;
				FormMain->NameLinesTimer->Enabled=true;
				//UpdateNameTable(-1,-1,true);
			}
			else if(!Shift.Contains(ssCtrl))
			{
				nameSelection.left=nameSelBuf.left+nameXC-nxDown;
				nameSelection.right=nameSelBuf.right+nameXC-nxDown;
				nameSelection.top=nameSelBuf.top+nameYC-nyDown;
				nameSelection.bottom=nameSelBuf.bottom+nameYC-nyDown;

				for (int i=0; i<32; i++)  //long enough loop
				{
					if(nameSelection.left<0)   	{	nameSelection.left++;
													nameSelection.right++;}

					if( nameSelection.right>nameTableWidth)
												{	nameSelection.left--;
													nameSelection.right--;}

					if(nameSelection.top<0)   	{	nameSelection.top++;
													nameSelection.bottom++;}

					if(nameSelection.bottom>nameTableHeight)
												{	nameSelection.top--;
													nameSelection.bottom--;}
				}
				cueUpdateTiles=true;
				//cueUpdateNametable=true;
				//CueDrawTimer->Enabled=true;
				FormMain->NameLinesTimer->Enabled=true;
				CueLinesTimer->Enabled=true;
			}
		}
	    cueStats=true;
		return;
	}
	if(!Shift.Contains(ssShift)){
		if((Shift.Contains(ssLeft)) && bClickedInsideScreen)
		{
			nameTableViewX=nameTableViewXbuf+nameXC-nxDown;
			nameTableViewY=nameTableViewYbuf+nameYC-nyDown;

			if(nameTableViewX<0) nameTableViewX=0;
			if(nameTableViewY<0) nameTableViewY=0;

			if(nameTableViewX+32>=nameTableWidth) nameTableViewX=nameTableWidth-32;
			if(nameTableViewY+30>=nameTableHeight) nameTableViewY=nameTableHeight-30;
			FormMain->CorrectView();
			//CueDrawTimer->Enabled=true;
			CueLinesTimer->Enabled=true;
			//FormMain->NameLinesTimer->Enabled=true;
			//cueUpdateNametable=true;
			//FormMain->UpdateNameTable(-1,-1,true);
			CorrectNT->Enabled=true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::Map1EndDrag(TObject *Sender, TObject *Target,
      int X, int Y)
{
	bDrawDestShadow=false;
	bImageNameAccepted=false;
	bImageTileAccepted=false;
	FormMain->UpdateNameTable(-1,-1,true);
    UpdateLines(false);
	//Draw(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::Map1DragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
	if(!FormNavigator->Active) return;
	Accept=false;
	if(!bOutsideSel) Accept=true;
	float div = fStretchScale*8; //bHalfsize?cHalf:cFull;
	X = X/ div;
	Y = Y/ div;
	if(Accept==true)
	{
		bImageNameAccepted=true;
		bImageTileAccepted=false;
		if (!bOutsideSel)  //decided at mouse down-time
		{
			int cAlHgt = (viewPortHeight-nameTableHeight)/2;
			int cAlWdt = (viewPortWidth-nameTableWidth)/2;
			if  ( cAlHgt<0)cAlHgt=0;
			if  (cAlWdt<0) cAlWdt=0;

			destRect.left	=nameSelection.left		+X-nxDown+cAlWdt;
			destRect.right	=nameSelection.right	+X-nxDown+cAlWdt;
			destRect.top	=nameSelection.top		+Y-nyDown+cAlHgt;
			destRect.bottom	=nameSelection.bottom	+Y-nyDown+cAlHgt;


			for (int i=0; i<32; i++)  //long enough loop

				{
					if(destRect.left<0+cAlWdt)   	{	destRect.left++;
												destRect.right++;}
					if(destRect.right>nameTableWidth+cAlWdt)
											{	destRect.left--;
												destRect.right--;}
					if(destRect.top<0+cAlHgt)   	{	destRect.top++;
												destRect.bottom++;}
					if(destRect.bottom>nameTableHeight+cAlHgt)
											{	destRect.top--;
												destRect.bottom--;}
				}

		}
		else
		{
			destRect.left=X;
			destRect.top=X;
			destRect.right=Y+1;
			destRect.bottom=Y+1;
		}
		bDrawDestShadow=true;
		
		//cueUpdateNametableNoRepaint=true;
		//cueUpdateNametable=true;
		cueUpdateTiles=false;
		//CueDrawTimer->Enabled=true;
		CueLinesTimer->Enabled=true;
        FormMain->NameLinesTimer->Enabled=true;
		cueStats=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::Map1DragDrop(TObject *Sender, TObject *Source,
      int X, int Y)
{
	bool bClone = ( bBufCtrl &&  !bBufShift && bBufAlt);
	bool bSwap	= (!bBufCtrl && !bBufShift && !bBufAlt);
	bool bMove	= ( bBufCtrl && !bBufShift && !bBufAlt);

	bool bOverride = false;

	float div = fStretchScale*8; //bHalfsize?cHalf:cFull;
	X = X/ div;
	Y = Y/ div;
	if(X<0||X>=(nameTableWidth)||Y<0||Y>=(nameTableHeight)) return;
    if(!FormNavigator->Active) return;
	int w=1;
	int h=1;


	if (!bOutsideSel) //if grabbed from inside selection, retain size. ImageNameDragDrop is only call while this is true, but in case a context changes it..
	{
		w=destRect.right-destRect.left;
		h=destRect.bottom-destRect.top;
	}
	//these are used by the preserve all tilenames; try preserve order method.
	unsigned char tempName;
	unsigned char tempAttr;

	//this array is used by the preserve structure; overwrite tilenames method.

	unsigned char tempTable[sizeof(w*h)];


	int cAlHgt = (viewPortHeight-nameTableHeight)/2;
	int cAlWdt = (viewPortWidth-nameTableWidth)/2;
	if  (cAlHgt<0) cAlHgt=0;
	if  (cAlWdt<0) cAlWdt=0;




	



	int xSrc = nameSelection.left;//+ cAlWdt;//;
	int ySrc = nameSelection.top;//+ cAlHgt;// ;
	int xDst = destRect.left-cAlWdt;//+nameTableViewX;
	int yDst = destRect.top-cAlHgt;//+nameTableViewY;

	int dst = (yDst   ) * nameTableWidth
			 +  xDst;		//+ nameTableViewX;
	int src	= (ySrc  ) * nameTableWidth
			+ xSrc   ;




	bool bDiagonal=(xSrc!=xDst && ySrc!=yDst)?true:false;

	if(dst==src) return;
	if((FormMain->SpeedButtonPal->Down==false) && (FormMain->SpeedButtonTiles->Down==false))
		{bOverride=true;}

	//begin swap
	FormMain->SetUndo();



	//--method 1: the "preserve all tilenames" fashioned from the CHR swap routine.
	//-----------
	int offset;  	//gets loaded with an expression for sorting order.

	//----select sorting method
	for(int sy=0; sy<h*nameTableWidth; sy+=nameTableWidth) {
		for(int sx=0; sx<w; sx++) {
			if (ySrc==yDst) {
				if      (xSrc>xDst)  {offset=sx+(sy);}  //dir: east
				else if (xSrc==xDst) {return;}		   	//reduncancy; center; added for the note.
				else    			 {offset=((w-1))-sx+((h-1)*nameTableWidth)-(sy);} 		// west
			}
			else if (ySrc>yDst) {
				if      (xSrc==xDst) {offset=sx+(sy); }  //dir: north
				else if (xSrc>xDst)  {offset=sx+(sy); }  //dir: north-east
				else 	    		 {offset=((w-1))-sx+(sy);}   	//dir: north-west
			}
			else {
				if 	    (xSrc==xDst) {offset=sx+((h-1)*nameTableWidth)-(sy); } //south
				else if (xSrc>xDst) {offset=sx+((h-1)*nameTableWidth)-(sy); }  //south-east
				else    {offset=((w-1))-sx+((h-1)*nameTableWidth)-(sy); }      //south-west
			}

		 //-swap 1 tile for another.
		 if(!bDiagonal && (FormMain->SpeedButtonTiles->Down || bOverride))  //the extra conditionals are because the loop does an extra row & line for attributes.
		 {
			//swap
			if(bSwap){
				memcpy(&tempName				   , &nameTable[dst+offset],1);
				memcpy(&nameTable	[dst+offset], &nameTable[src+offset] ,1);
				memcpy(&nameTable	[src+offset]  , &tempName			 ,1);
			}
			//move
			if(bMove){
				//memcpy(&tempName				   , &nameTable[dst+offset],1);
				memcpy(&nameTable	[dst+offset], &nameTable[src+offset] ,1);
				memcpy(&nameTable	[src+offset]  , &nullTile			 ,1);
			}
			//clone
			if(bClone){
				//memcpy(&tempName				   , &nameTable[dst+offset],1);
				memcpy(&nameTable	[dst+offset], &nameTable[src+offset] ,1);
				//memcpy(&nameTable	[src+offset]  , &tempName			 ,1);
			}

		 }
		}
	}


	if(bDiagonal  && (FormMain->SpeedButtonTiles->Down || bOverride)){
	//--method 2: a simple copypaste swap with buffer.
	//----------
	//It seems to preserve the structure better in some cases.
	int offset;
	//-swap rows of tilenames

		if(bSwap){
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameCopy[src+offset]	, &nameTable[src+offset],w);
			}
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameTable	[src+offset], &nameTable[dst+offset],w);
			}
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameTable	[dst+offset], &nameCopy[src+offset]	,w);
			}
		}
		if(bClone){
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameTable	[dst+offset], &nameTable[src+offset],w);
			}
		}
		if(bMove)
		{
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameCopy[dst+offset]	, &nameTable[src+offset],w);
			}
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				//memcpy(&nameTable	[src+offset], nullTile	,w);
				for(int j = 0; j < w; j++) nameTable[src+offset+j]=nullTile;
			}
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameTable	[dst+offset], &nameCopy[dst+offset],w);
			}
		}
	}

	//swap attributes
	if(FormMain->SpeedButtonPal->Down||bOverride)
	{
	  int i,j,xo,yo;
	  int n=1;

	  for (i=0;i<h;i++){
			for(j=0;j<w;++j){
				if (ySrc>yDst) yo=i;	if (ySrc<=yDst) yo=(h-n-i);
				if (xSrc>xDst) xo=j;	if (xSrc<=xDst) xo=(w-n-j);
				FormMain->AttrSet(xo, yo, FormMain->AttrGet(xSrc+xo,ySrc+yo,false,false),true);
			}
	  }
	  if(bSwap)
	  {
		for (i=0;i<h;i++){
			for(j=0;j<w;++j){
				if (ySrc>yDst) yo=i;	if (ySrc<=yDst) yo=(h-n-i);
				if (xSrc>xDst) xo=j;	if (xSrc<=xDst) xo=(w-n-j);
				FormMain->AttrSet(xSrc+xo, ySrc+yo, FormMain->AttrGet(xDst+xo,yDst+yo,false,false),false);
			}
		}
	  }
	  for (i=0;i<h;i++){
			for(j=0;j<w;++j){
				if (ySrc>yDst) yo=i;	if (ySrc<=yDst) yo=(h-n-i);
				if (xSrc>xDst) xo=j;	if (xSrc<=xDst) xo=(w-n-j);
				FormMain->AttrSet(xDst+xo, yDst+yo, FormMain->AttrGet(xo,yo,true,false),false);
			}
		}
	  
	}
	int dx,dy,dw,dh;
    if(nameSelection.left>=0&&nameSelection.top>=0)
	{
	FormMain->GetSelection(nameSelection,dx,dy,dw,dh);
	DrawRange(dx,dy,dw,dh,false);
	}
	nameSelection.left 		= destRect.left-cAlWdt;
	nameSelection.top 		= destRect.top-cAlHgt;
	nameSelection.right 	= destRect.right-cAlWdt;
	nameSelection.bottom 	= destRect.bottom-cAlHgt;
	//FormMain->UpdateNameTable(-1,-1,false);

	FormMain->GetSelection(nameSelection,dx,dy,dw,dh);
	DrawRange(dx,dy,dw,dh,false);
	//UpdateLines(false);
	//Draw(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	FormMain->FormKeyUp(Sender,Key,Shift);	
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::FormActivate(TObject *Sender)
{
	FormNavigator->ScreenSnap=bSnapToScreen;
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::FormCreate(TObject *Sender)
{

	bufBmp=new Graphics::TBitmap();
	bufBmp->SetSize(nameTableWidth*8,nameTableHeight*8);
	bufBmp->PixelFormat=pf4bit;

	//Map1->Picture=new TPicture();
	//Map1->Picture->Bitmap=new Graphics::TBitmap();
	Map1->Picture->Bitmap->PixelFormat=pf24bit;
	Map1->Picture->Bitmap->SetSize(Map1->Width,Map1->Height);

}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::FormDestroy(TObject *Sender)
{
	delete bufBmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::CueLinesTimerTimer(TObject *Sender)
{
    if(!openByFileDone) return;
	UpdateLines(true);
	CueDrawTimer->Enabled=false;
	CueLinesTimer->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::CorrectNTTimer(TObject *Sender)
{
    //FormMain->CorrectView();
	if(!openByFileDone) return;
	FormMain->UpdateNameTable(-1,-1,true);
	CorrectNT->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormNavigator::CueChunkDrawTimer(TObject *Sender)
{
		if(!openByFileDone) return;
		int z=32;


		DrawRange(cueChunkX,cueChunkY,z,z,false);

		cueChunkX+=z;
		if (cueChunkX>nameTableWidth) {cueChunkY+=z; cueChunkX=0;}
		if (cueChunkY>nameTableHeight)
		{
			CueChunkDraw->Enabled=false;
			cueChunkX=0;
			cueChunkY=0;
			UpdateLines(false);
		}
	
}
//---------------------------------------------------------------------------

