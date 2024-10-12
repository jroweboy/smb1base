//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSwapBanks.h"
#include "UnitMain.h"
#include "UnitBankCHR.h"
#include "UnitCHREditor.h"
#include "UnitNavigator.h"
#include "UnitManageMetasprites.h"
#include "UnitBankCHR.h"
#include "UnitCHRbit.h"
#include "UnitMTprops.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
 TFormSwapBanks *FormSwapBanks;


 extern int tileActive;
 extern unsigned char *chr;
 extern bool chrSelectRect;
 extern chrBanks;
 extern nullTile;
 //extern uint32_t tileProperties[];
 extern unsigned char *tileProperties;

 extern unsigned int curViewCHRbankCanvas2;
 extern bool cueUpdateMetasprite;
 extern int tileSelRectWdt;
 extern int tileSelRectHgt;
 extern unsigned char chrSelected[];
 extern int palActive;
 extern bool bOutsideSel;
 extern bool isBnkCursor;
 bool bank1SelectRect=false;
 bool bLeftCanvasClicked;
 bool bSwapBankShowGrid=false;
 int bank1Active=0;
 int bank2Active=0;

 int bank1_xDown=0;
 int bank1_yDown=0;


 int bank2_xDown=0;
 int bank2_yDown=0;

 TRect bank1Selection;
 TRect bank2Selection;
 TRect bank1SelBuf;
 TRect bank2SelBuf;

 //---------------------------------------------------------------------------
__fastcall TFormSwapBanks::TFormSwapBanks(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
 void __fastcall  TFormSwapBanks::UpdateLists(bool keepSel){

  int count1 = FormBankCHR->ListBox1->Count;
  int count2 = ListBox2->Count;

  int maxItemCount;

  if (count1 > count2) {maxItemCount = count1;}
  else 				   {maxItemCount = count2;}

  bool* bufSel = new bool[maxItemCount];
  if(keepSel){
	for(int i=0; i<ListBox2->Count; i++){
		bufSel[i]=ListBox2->Selected[i];
	}
  }

  ListBox1->Clear();
  ListBox2->Clear();

  for (int i=0; i<FormBankCHR->ListBox1->Count; ++i)
  {
		ListBox1->Items->Add(FormBankCHR->ListBox1->Items->Strings[i]);
		ListBox2->Items->Add(FormBankCHR->ListBox1->Items->Strings[i]);
  }
  ListBox1->ItemIndex=FormBankCHR->ListBox1->ItemIndex;

  if(keepSel){
	 for(int i=0; i<FormBankCHR->ListBox1->Count; i++){
		ListBox2->Selected[i]=bufSel[i];
	}
  }
  else{
	for(int i=0; i<FormBankCHR->ListBox1->Count; i++){
		ListBox2->Selected[i]=false;
	}
	if(ListBox1->ItemIndex < ListBox1->Count) ListBox2->Selected[ListBox1->ItemIndex]=true;
	else  ListBox2->Selected[ListBox1->ItemIndex-1]=true;
  }
  if(ListBox2->SelCount==1) 	 {btnSwap->Enabled=true;  btnClone->Enabled=true;  btnMove->Enabled=true;}
  else if (ListBox2->SelCount<1) {btnSwap->Enabled=false; btnClone->Enabled=false; btnMove->Enabled=false;}
  else if (ListBox2->SelCount>1) {btnSwap->Enabled=false; btnClone->Enabled=true;  btnMove->Enabled=true;}

  delete bufSel;
 }
 //---------------------------------------------------------------------------
 void __fastcall  TFormSwapBanks::FullUpdate(bool keepSel){
	UpdateLists(keepSel);
	UpdateCanvas1();
	UpdateCanvas2();
 }

 //---------------------------------------------------------------------------

 void __fastcall  TFormSwapBanks::UpdateCanvas1(void){
	int x=0;
	int y=0;
	int n=2;
	Image1     ->Picture->Bitmap->SetSize(128*n,128*n);

	int bi = ListBox1->ItemIndex*256;
	//-(curViewCHRbankCanvas2*4096);

	for(int i=0;i<256;i++) //tiles
	{
		//DrawTile(TPicture *pic, int x, int y, int tile, int pal, int tx, int ty, bool sel, bool efficientTarget, int inputScale, bool bIsNav, bool doubleWidth, bool banked)
		//unsigned int bi=i+(256*curViewCHRbankCanvas2);

		FormMain->DrawTile(Image1->Picture,x*n,y*n,bi+i,bi+i,palActive,-1,-1,i,false,1*n,false,false,false,true,true);
		x+=8;
		if(x>=128){	x=0; y+=8; }
	}

	if(bank1SelectRect)
	{
	   Image1->Canvas->Pen->Style = psSolid;
	   bank1Active=bank1Selection.top*16+bank1Selection.left;
	   isBnkCursor=false;
	   FormMain->DrawSelection(Image1,bank1Selection,1*n,true,false);
	}

	if(bSwapBankShowGrid){
			TColor dGray = static_cast<TColor>(RGB(64, 64, 64));
			Image1->Canvas->Pen->Width = 1;
			Image1->Canvas->Pen->Style = psDot;
			Image1->Canvas->Pen->Color = dGray;
			Image1->Canvas->MoveTo(0, 63);
			Image1->Canvas->LineTo(255, 63);
			Image1->Canvas->MoveTo(0, 128+63);
			Image1->Canvas->LineTo(255, 128+63);

			Image1->Canvas->Pen->Color = clGray;
			Image1->Canvas->MoveTo(127, 0);
			Image1->Canvas->LineTo(127, 255);
			Image1->Canvas->MoveTo(0, 127);
			Image1->Canvas->LineTo(255, 127);
	}


	Image1->Refresh();
 }
 //---------------------------------------------------------------------
  void __fastcall  TFormSwapBanks::UpdateCanvas2(void){
	int x=0;
	int y=0;
	int n=2;
	Image2     ->Picture->Bitmap->SetSize(128*n,128*n);

	int bi = ListBox2->ItemIndex*256;// -(curViewCHRbankCanvas2*4096);

	for(int i=0;i<256;i++) //tiles
	{
		//DrawTile(TPicture *pic, int x, int y, int tile, int pal, int tx, int ty, bool sel, bool efficientTarget, int inputScale, bool bIsNav, bool doubleWidth, bool banked)
		//unsigned int bi=i+(256*curViewCHRbankCanvas2);

		FormMain->DrawTile(Image2->Picture,x*n,y*n,bi+i,bi+i,palActive,-1,-1,i,false,1*n,false,false,false,true,true);
		x+=8;
		if(x>=128){	x=0; y+=8; }
	}

	if(bank1SelectRect)
	{
       Image2->Canvas->Pen->Style = psSolid;
	   bank2Active=bank2Selection.top*16+bank2Selection.left;
	   isBnkCursor=true;
	   FormMain->DrawSelection(Image2,bank2Selection,1*n,true,false);
	}

	if(bSwapBankShowGrid){
			TColor dGray = static_cast<TColor>(RGB(64, 64, 64));
			Image2->Canvas->Pen->Width = 1;
			Image2->Canvas->Pen->Style = psDot;
			Image2->Canvas->Pen->Color = dGray;
			Image2->Canvas->MoveTo(0, 63);
			Image2->Canvas->LineTo(255, 63);
			Image2->Canvas->MoveTo(0, 128+63);
			Image2->Canvas->LineTo(255, 128+63);

			Image2->Canvas->Pen->Color = clGray;
			Image2->Canvas->MoveTo(127, 0);
			Image2->Canvas->LineTo(127, 255);
			Image2->Canvas->MoveTo(0, 127);
			Image2->Canvas->LineTo(255, 127);
	}

	Image2->Refresh();
 }
 //---------------------------------------------------------------------
void __fastcall TFormSwapBanks::Swap4k(int mode)
{
	unsigned char temp[4096];
	unsigned char propt[256];

	FormMain->SetUndo();

	int bank1=ListBox1->ItemIndex*4096;
	int bank2;

	memcpy(temp,chr+bank1,4096);
	for(int i=0;i<chrBanks;i++){
		if(ListBox2->Selected[i]){

			bank2=i*4096;
			if(mode==0) memcpy(chr+bank1,chr+bank2,4096);

			memcpy(chr+bank2,temp,4096);
		}

	}
	if(mode==2) memset(chr+bank1,0,4096);

	if(chkInclProps->Checked){
		bank1=bank1/16;
		memcpy(propt,&tileProperties[bank1]	,256);
		for(int i=0;i<chrBanks;i++){
			if(ListBox2->Selected[i]){
				bank2=i*256;

				if(mode==0) memcpy(&tileProperties[bank1]  	,&tileProperties[bank2]	,256);
							memcpy(&tileProperties[bank2]	,propt		  			,256);
			}
		}
		if(mode==2) memset(&tileProperties[bank1] ,0,256);
	}

	UpdateCanvas1();
	UpdateCanvas2();
	FormMain->UpdateTiles(true);
	FormMain->UpdateNameTable(-1,-1,true);
	if (FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if (FormCHRbit->Visible) FormCHRbit->UpdateBitButtons(false);
	if(FormMTprops->Visible)FormMTprops->UpdateBitButtons_tileClick(false);
	cueUpdateMetasprite=true;

}
//---------------------------------------------------------------------------
void __fastcall TFormSwapBanks::SwapSelection(int mode)
{
	bool bSwap	= mode==0;
	bool bClone = mode==1;
	bool bMove	= mode==2;

	//unsigned char temp[4096];
	unsigned char tempchr[16];
	unsigned char propt[256];

	FormMain->SetUndo();

	int ps=bank1Active;                     //first tile in bank1
	int pd=bank2Active;                     //first tile in bank2
    int bank1=ListBox1->ItemIndex*4096;
	int bank1prop=bank1/16;
	int bank2;                          //chosen in for loop.
	const int tw=16;   					//tileset table width

	//used to determine order of swap deployment to minimize issues when 2 selections overlap
	int xSource = bank1Active&(tw-1);
	int ySource = bank1Active/tw;
	int xDest = bank2Active&(tw-1);
	int yDest = bank2Active/tw;
    int offset;

	int w=abs(bank1Selection.right-bank1Selection.left);
	int	h=abs(bank1Selection.bottom-bank1Selection.top);

	for(int sy=0; sy<h*tw; sy+=tw) {
		for(int sx=0; sx<w; sx++) {
			if (ySource>=yDest) {
				if   (xSource>xDest) offset=sx+(sy);
				else 			 offset=(w-1)-sx+(sy);
			}
			else {
				if 	 (xSource>xDest) offset=sx+((h-1)*tw)-(sy);
				else 			 offset=(w-1)-sx+((h-1)*tw)-(sy);
			}
            //swap
			if(bSwap){
				memcpy(tempchr				, &chr[bank1+(ps+offset)*16]	,16);
				for(int i=0;i<chrBanks;i++){
					if(ListBox2->Selected[i]){
						bank2=i*4096;
						memcpy(&chr[bank1+(ps+offset)*16]	, &chr[bank2+(pd+offset)*16]	,16);
						memcpy(&chr[bank2+(pd+offset)*16]	, tempchr				,16);
					}
				}

				if(chkInclProps->Checked){

					memcpy(propt			   ,&tileProperties[bank1prop+ps+offset]	,1);
					for(int i=0;i<chrBanks;i++){
						if(ListBox2->Selected[i]){
							bank2=i*256;
							memcpy(&tileProperties		[bank1prop+ps+offset], &tileProperties[bank2+pd+offset]	,1);
							memcpy(&tileProperties		[bank2+pd+offset], propt,1);
						}
					}
				}
			}

			//move
			if(bMove || bClone){
				memcpy(tempchr				, &chr[bank1+(ps+offset)*16]	,16);
				if(bMove)memset(&chr[bank1+(ps+offset)*16],0,16);
				for(int i=0;i<chrBanks;i++){
					if(ListBox2->Selected[i]){
						bank2=i*4096;
						memcpy(&chr[bank2+(pd+offset)*16], tempchr,16);
					}
				}


				if(chkInclProps->Checked){
					bank1=bank1/16;
					memcpy(propt			   ,&tileProperties[bank1prop+ps+offset]	,1);
					if(bMove)memset(&tileProperties[bank1prop+ps+offset],0,1);
					for(int i=0;i<chrBanks;i++){
						if(ListBox2->Selected[i]){
							bank2=i*256;
							memcpy(&tileProperties		[bank2+pd+offset], propt,1);
						}
					}
				}
			}
		}
	}
	UpdateCanvas1();
	UpdateCanvas2();
	FormMain->UpdateTiles(true);
	//WorkCHRToBankCHR();
	FormMain->UpdateNameTable(-1,-1,true);
	if (FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if (FormCHRbit->Visible) FormCHRbit->UpdateBitButtons(false);
	if(FormMTprops->Visible)FormMTprops->UpdateBitButtons_tileClick(false);
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormSwapBanks::FormShow(TObject *Sender)
{
	FullUpdate(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::FormCreate(TObject *Sender)
{
    this->Left=FormMain->Left - this->Width;
	this->Top=(Screen->Height-FormMain->Height)/4;
	Image1->Picture=new TPicture();
	Image1->Picture->Bitmap=new Graphics::TBitmap();
	Image1->Picture->Bitmap->PixelFormat=pf24bit;
    Image1->Stretch=true;

	Image2->Picture=new TPicture();
	Image2->Picture->Bitmap=new Graphics::TBitmap();
	Image2->Picture->Bitmap->PixelFormat=pf24bit;
	Image2->Stretch=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::ListBox2Click(TObject *Sender)
{
  if(ListBox2->SelCount==1) 	 {btnSwap->Enabled=true;  btnClone->Enabled=true;  btnMove->Enabled=true;}
  else if (ListBox2->SelCount<1) {btnSwap->Enabled=false; btnClone->Enabled=false; btnMove->Enabled=false;}
  else if (ListBox2->SelCount>1) {btnSwap->Enabled=false; btnClone->Enabled=true;  btnMove->Enabled=true;}
	UpdateCanvas2();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::ListBox1Click(TObject *Sender)
{
	UpdateCanvas1();	
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::Image1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if(!bLeftCanvasClicked) return;
   if(X>=0&&X<(128)&&Y>=0&&Y<(128))
   {
		int tx=X/8;
		int ty=Y/8;

		//if(Shift.Contains(ssShift))
		//{
			//drag selection
			if(Shift.Contains(ssRight)&&!bOutsideSel)
			{


				bank1Selection.left=bank1SelBuf.left+tx-bank1_xDown;
				bank1Selection.right=bank1SelBuf.right+tx-bank1_xDown;

				bank1Selection.top=bank1SelBuf.top+ty-bank1_yDown;
				bank1Selection.bottom=bank1SelBuf.bottom+ty-bank1_yDown;

				if(!Shift.Contains(ssCtrl)){
					bank2Selection.left=bank1Selection.left;
					bank2Selection.right=bank1Selection.right;
					bank2Selection.top=bank1Selection.top;
					bank2Selection.bottom=bank1Selection.bottom;
			   }

				for (int i=0; i<16; i++)  //long enough loop
				{
					if(bank1Selection.left<0)
					{	bank1Selection.left++;
						bank1Selection.right++;

					}

					if(bank1Selection.right>0x10)
					{	bank1Selection.left--;
						bank1Selection.right--;

					}

					if(bank1Selection.top<0 )
					{	bank1Selection.top++;
						bank1Selection.bottom++;

					}

					if(bank1Selection.bottom>0x10)
					{	bank1Selection.top--;
						bank1Selection.bottom--;

					}

				}
				for (int i=0; i<16; i++)  //long enough loop
				{
					if(bank2Selection.left<0)
					{
						bank2Selection.left++;
						bank2Selection.right++;
					}

					if(bank2Selection.right>0x10)
					{
						bank2Selection.left--;
						bank2Selection.right--;
					}

					if(bank2Selection.top<0)
					{
						bank2Selection.top++;
						bank2Selection.bottom++;
					}

					if(bank2Selection.bottom>0x10)
					{
						bank2Selection.top--;
						bank2Selection.bottom--;
					}

				}
				//cueUpdateTiles=true;
				//cueUpdateNametable=true;


			}

			//box selection
			if(Shift.Contains(ssLeft))
				{
					if(Shift.Contains(ssShift)){
					if(tx<bank1_xDown) {bank1Selection.left=tx+1-(tx<bank1Selection.right?1:0);
								  bank1Selection.right=bank1_xDown+1;
								  }
					if(tx>=bank1_xDown) {bank1Selection.right =tx+(tx>=bank1Selection.left?1:0);
								  bank1Selection.left=bank1_xDown;
								  }
					}
					else{
                        bank1Selection.left  =0;
						bank1Selection.right =16;

					}

					if(ty<bank1_yDown)  {bank1Selection.top=ty-(ty>=bank1Selection.bottom ?1:0);
								   bank1Selection.bottom=bank1_yDown+1;
								   }
					if(ty>=bank1_yDown) {bank1Selection.bottom=ty+(ty>=bank1Selection.top ?1:0);
								   bank1Selection.top=bank1_yDown;
								   }

				   if(bank1Selection.top<0) bank1Selection.top=0;
				   if(bank1Selection.left<0) bank1Selection.left=0;
				   if(bank1Selection.right>0x10) bank1Selection.right=0x10;
				   if(bank1Selection.bottom>0x10) bank1Selection.bottom=0x10;


				   bank1Active=bank1Selection.top*16+bank1Selection.left;

				   bank2Selection.left=bank1Selection.left;
				   bank2Selection.right=bank1Selection.right;
				   bank2Selection.top=bank1Selection.top;
				   bank2Selection.bottom=bank1Selection.bottom;

				   }
				//}

			}
	UpdateCanvas1();
	UpdateCanvas2();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::Image1MouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bSwapBankShowGrid=true;
	int tx=X/(8);
	int ty=Y/(8);
	bank1_xDown=tx;    //used for relative positioning when dragging selection, as well as performing positive selections to the left/up.
	bank1_yDown=ty;

	bLeftCanvasClicked=true;

	bOutsideSel=false;

	if((bank1Selection.right<=tx)
		|(bank1Selection.left>tx)
		|(bank1Selection.bottom<=ty)
		|(bank1Selection.top>ty))
			bOutsideSel=true;
	if(!bank1SelectRect&&bank1Active!=ty*16+tx) bOutsideSel=true;
	bank1Active=ty*16+tx;
	//if(Shift.Contains(ssShift))
	//{
		//tile + box select method
		if(Shift.Contains(ssLeft)||(bOutsideSel))
		{
			if(Shift.Contains(ssShift)){
				bank1Selection.left  =bank1Active&15;
				bank1Selection.right =bank1Selection.left+1;
			}
			else{
				bank1Selection.left  =0;
				bank1Selection.right =16;
			}
			bank1Selection.top   =bank1Active/16;
			bank1Selection.bottom=bank1Selection.top +1;

			bank1SelectRect=true;

			if(!Shift.Contains(ssCtrl)){
				bank2Selection.left  =bank1Selection.left;
				bank2Selection.top   =bank1Selection.top;
				bank2Selection.right =bank1Selection.right;
				bank2Selection.bottom=bank1Selection.bottom;
			}
			else{

					bank2Selection.right =bank2Selection.left+1;
					bank2Selection.bottom=bank2Selection.top +1;

			}

			//SetTile(tileActive);
			//UpdateTiles(true);

		}
		else if(Shift.Contains(ssRight)||(!bOutsideSel)){
			//Screen->Cursor = crSizeAll;

			bank1SelBuf.left		=	bank1Selection.left;
			bank1SelBuf.top			=	bank1Selection.top;
			bank1SelBuf.right		=	bank1Selection.right;
			bank1SelBuf.bottom  	=	bank1Selection.bottom;
		}
	//}

	UpdateCanvas1();
	UpdateCanvas2();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::Button2Click(TObject *Sender)
{
	bank1SelectRect=false;
	UpdateCanvas1();
	UpdateCanvas2();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::Image2MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	if(bLeftCanvasClicked) return;
	if(X>=0&&X<(128)&&Y>=0&&Y<(128))
   {
		int tx=X/8;
		int ty=Y/8;

		
			//drag selection
			if((Shift.Contains(ssRight)||Shift.Contains(ssLeft))&&!bOutsideSel)
			{


				bank2Selection.left=bank2SelBuf.left+tx-bank2_xDown;
				bank2Selection.right=bank2SelBuf.right+tx-bank2_xDown;
				bank2Selection.top=bank2SelBuf.top+ty-bank2_yDown;
				bank2Selection.bottom=bank2SelBuf.bottom+ty-bank2_yDown;



				for (int i=0; i<16; i++)  //long enough loop - felt safer than while
				{
					if(bank2Selection.left<0)
					{
						bank2Selection.left++;
						bank2Selection.right++;
					}

					if(bank2Selection.right>0x10)
					{
						bank2Selection.left--;
						bank2Selection.right--;
					}

					if(bank2Selection.top<0)
					{
						bank2Selection.top++;
						bank2Selection.bottom++;
					}

					if(bank2Selection.bottom>0x10)
					{
						bank2Selection.top--;
						bank2Selection.bottom--;
					}

				}
				//cueUpdateTiles=true;
				//cueUpdateNametable=true;
					UpdateCanvas1();
					UpdateCanvas2();

			}

			}
	//bank2Active=bank2Selection.top*16+bank2Selection.left;

}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::Image2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bSwapBankShowGrid=true;
	bLeftCanvasClicked=false;
	int tx=X/(8);
	int ty=Y/(8);
	bank2_xDown=tx;    //used for relative positioning when dragging selection, as well as performing positive selections to the left/up.
	bank2_yDown=ty;


	bOutsideSel=false;

	if((bank2Selection.right<=tx)
		|(bank2Selection.left>tx)
		|(bank2Selection.bottom<=ty)
		|(bank2Selection.top>ty))
			bOutsideSel=true;
	if(!bank1SelectRect&&bank2Active!=ty*16+tx) bOutsideSel=true;
	if(!bOutsideSel){


			bank2SelBuf.left		=	bank2Selection.left;
			bank2SelBuf.top			=	bank2Selection.top;
			bank2SelBuf.right		=	bank2Selection.right;
			bank2SelBuf.bottom  	=	bank2Selection.bottom;
		}
	UpdateCanvas1();
	UpdateCanvas2();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::btnSwapClick(TObject *Sender)
{
	if(bank1SelectRect) SwapSelection(0);
	else                Swap4k(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::btnCloneClick(TObject *Sender)
{
	if(bank1SelectRect) SwapSelection(1);
	else                Swap4k(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::btnMoveClick(TObject *Sender)
{
	if(bank1SelectRect) SwapSelection(2);
	else                Swap4k(2);
}
//---------------------------------------------------------------------------



void __fastcall TFormSwapBanks::Image1MouseEnter(TObject *Sender)
{
	bSwapBankShowGrid=true;
	UpdateCanvas1();
	UpdateCanvas2();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::Image1MouseLeave(TObject *Sender)
{
	bSwapBankShowGrid=false;
	UpdateCanvas1();
	UpdateCanvas2();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::Image2MouseEnter(TObject *Sender)
{
   bSwapBankShowGrid=true;
	UpdateCanvas1();
	UpdateCanvas2();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapBanks::Image2MouseLeave(TObject *Sender)
{
	bSwapBankShowGrid=false;
	UpdateCanvas1();
	UpdateCanvas2();
}
//---------------------------------------------------------------------------

