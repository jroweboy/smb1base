//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#pragma hdrstop
#include "UnitMain.h"
#include "UnitName.h"
#include "UnitBankCHR.h"
#include "UnitNavigator.h"
#include "UnitSwapBanks.h"
#include "UnitCHREditor.h"
#include "UnitManageMetasprites.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBankCHR *FormBankCHR;
inline int min(int a, int b) {
	return (a < b) ? a : b;
}
inline int max(int a, int b) {
	return (a > b) ? a : b;
}
extern bool openByFileDone;
extern bool bKeyEscape;
extern bool holdStats;

extern unsigned char *chr;
extern unsigned int bankViewTable[];

extern unsigned char *tileProperties;
extern unsigned int curViewCHRbankCanvas2;


const int CHR_4k=4096;
const int bankwin=8;
//extern char *chrBank;
//extern AnsiString *chrBankNames;
extern std::vector<std::string> chrBankLabels;

extern char tileViewTable[];
extern int palActive;
extern char chrSelected[];


extern int chrBanks;
AnsiString strList;

TRect curSelection;
TRect bnkSelection;
TRect bnkCursor;
TRect curCursor;

TRect curSelBuf;

bool isBnkCursor=false;
bool curSetHover=false;
bool bnkSetHover=false;
bool clickSent=false;

extern bool cueUpdateMetasprite;
extern bankActive;

int bankview_xDown;
int	bankview_yDown;

extern bOutsideSel;
//---------------------------------------------------------------------------
__fastcall TFormBankCHR::TFormBankCHR(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormBankCHR::Update(void)
{
    MakeList(false,false);
	Draw();
}

void __fastcall TFormBankCHR::Draw(void)
{
	if (chrBanks<=1) Remove1->Enabled=false;
	else             Remove1->Enabled=true;

	btnA->Down=FormMain->SpeedButtonChrBank1->Down;
	btnB->Down=FormMain->SpeedButtonChrBank2->Down;
	btnC->Down=FormMain->SpeedButtonChrBank3->Down;
	btnD->Down=FormMain->SpeedButtonChrBank4->Down;



	int x=0;
	int y=0;
	int n=2;
	Image1     ->Picture->Bitmap->SetSize(128*n,128*n);
	Image2     ->Picture->Bitmap->SetSize(128*n,128*n);



	for(int i=0;i<256;i++) //tiles
	{
		//DrawTile(TPicture *pic, int x, int y, int tile, int pal, int tx, int ty, bool sel, bool efficientTarget, int inputScale, bool bIsNav, bool doubleWidth, bool banked)
		//unsigned int bi=i+(256*curViewCHRbankCanvas2);

		FormMain->DrawTile(Image1->Picture,x*n,y*n,i,i,palActive,-1,-1,i,false,1*n,false,false,false,false,false);
		FormMain->DrawTile(Image2->Picture,x*n,y*n,i,i,palActive,-1,-1,i,false,1*n,false,false,false,true,false);

		x+=8;
		if(x>=128){	x=0; y+=8; }
	}

	isBnkCursor=false;
	Image1->Canvas->Pen->Style=psSolid;
	Image2->Canvas->Pen->Style=psSolid;
	FormMain->DrawSelection(Image1,curSelection,1*n,true,false);
	FormMain->DrawSelection(Image2,bnkSelection,1*n,false,false);

	if(!clickSent)
	{isBnkCursor=true;


		if(curSetHover){
			Image1->Canvas->Pen->Style=psDash;
			FormMain->DrawSelection(Image1,curCursor,1*n,true,false);
		}
		if(bnkSetHover)
		{
			Image2->Canvas->Pen->Style=psDash;
			FormMain->DrawSelection(Image2,bnkCursor,1*n,false,false);
		}
	isBnkCursor=false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::MakeList(bool bSelectTop, bool bInit)
{
	if(bInit){
	 ListBox1->Clear();
	 chrBankLabels.clear();

	for (int i = 0; i < chrBanks; i++) {
		chrBankLabels.push_back("Unlabeled");
	}
	}
	AnsiString tmp;
	for(int i=0;i<chrBanks;i++)

	{

		tmp = chrBankLabels.at(i).c_str();
		strList=IntToHex(i,3)+": \t"+tmp+"\t @ $"+IntToHex(i*4096,6);

		if(bInit)		ListBox1->Items->Add(strList);
		else 			ListBox1->Items->Strings[i] = strList;



	}
	if(bSelectTop) ListBox1->Selected[0]=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormBankCHR::FormShow(TObject *Sender)
{
	Draw();	
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::FormCreate(TObject *Sender)
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

	

	if(openByFileDone) MakeList(true,true);
	else OpenByFileAssociationMakeListTimer->Enabled=true;

	int len;
	if(btn512b->Down) {len=2;}
	if(btn1k->Down)   {len=4;}
	if(btn2k->Down)   {len=8;}
	if(btn4k->Down)   {len=16;}


	curSelection = TRect(0, 0, 16, 0 + len);
	curViewCHRbankCanvas2=ListBox1->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Insert1Click(TObject *Sender)
{
	unsigned int b,i,id, total;
	bool bDuplicate = ((TMenuItem*)Sender)->Tag==1?true:false;
	FormMain->SetUndo();

	id=ListBox1->ItemIndex;
	total=ListBox1->Items->Count;

	//expand chrBank memory
	chrBanks++;
	chr = (char*)realloc(chr,chrBanks*CHR_4k);

	//push working sets
	for(b=0;b<4;b++){
		for(i=0;i<256;i++){
			if(bankViewTable[b*256 + i]>=id*CHR_4k) bankViewTable[b*256 + i] +=CHR_4k;
		}
	}

	//push banks
	chrBankLabels.insert(chrBankLabels.begin() + id + 1, chrBankLabels[id]);

	for(i=total;i>id;i--)
	{
		memcpy(&chr[i*CHR_4k],&chr[(i-1)*CHR_4k],CHR_4k);
		
	}
	//insertion mode
	if(!bDuplicate)
	{
		memset(&chr[id*CHR_4k],0,CHR_4k);
		chrBankLabels[id]="Unlabeled";
	}

	MakeList(false,false);
	if (chrBanks>3) Remove1->Enabled=true;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btnAClick(TObject *Sender)
{
   	int tag;
	TSpeedButton *speedButton = dynamic_cast<TSpeedButton*>(Sender);
	if (speedButton){
		tag = speedButton->Tag;
	}


	bankActive=tag*4096;




	FormMain->SpeedButtonChrBank1->Down=btnA->Down;
	FormMain->SpeedButtonChrBank2->Down=btnB->Down;
	FormMain->SpeedButtonChrBank3->Down=btnC->Down;
	FormMain->SpeedButtonChrBank4->Down=btnD->Down;

	//bankActive=btnA->Down?0:4096;
	/*

	FormMain->UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);

	cueUpdateMetasprite=true;
	Draw(); */
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------



void __fastcall TFormBankCHR::Image1MouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int off=0,len=0;
	int tx=X/8;
	int ty=Y/8;
	bankview_xDown=X/8;
	bankview_yDown=Y/8;

    bOutsideSel=false;

	if((curSelection.right<=tx)
		|(curSelection.left>tx)
		|(curSelection.bottom<=ty)
		|(curSelection.top>ty))
			bOutsideSel=true;
	//if(!bank1SelectRect&&bank1Active!=ty*16+tx) bOutsideSel=true;


	if(btnFree->Down){
		if(Shift.Contains(ssLeft)){
		if(Shift.Contains(ssShift)){
			 curSelection = TRect(X/8, Y/8, X/8+1, Y/8+1);

			 if(curSelection.left < 0) {curSelection.left=0; curSelection.right=1;}
			 if(curSelection.top  < 0) {curSelection.top=0; curSelection.bottom=1;}
			 if(curSelection.left > 15) {curSelection.left=15; curSelection.right=16;}
			 if(curSelection.top  > 15) {curSelection.top=15; curSelection.bottom=16;}


		}
		else{
			off=Y/8;
			len=1;
			curSelection = TRect(0, off, 16, off+len);
		}
		}
		else if(Shift.Contains(ssRight)||(!bOutsideSel)){
			curCursor.left		=	curSelection.left;
			curCursor.top		=	curSelection.top;
			curCursor.right		=	curSelection.right;
			curCursor.bottom  	=	curSelection.bottom;

		}
		//else if(Shift.Contains(ssRight)||(!bOutsideSel)){
			//Screen->Cursor = crSizeAll;

			curSelBuf.left		=	curSelection.left;
			curSelBuf.top		=	curSelection.top;
			curSelBuf.right		=	curSelection.right;
			curSelBuf.bottom  	=	curSelection.bottom;
		//}
	}
	else{
		if(btn256b->Down) {off=Y/8;  len=1;}
		if(btn512b->Down) {off=Y/16; off*=2; len=2;}
		if(btn1k->Down)	  {off=Y/32; off*=4; len=4;}
		if(btn2k->Down)   {off=Y/64; off*=8; len=8;}
		if(btn4k->Down)   {off=0;   len=16;}
	curSelection = TRect(0, off, 16, off+len);
	}
	clickSent=true;
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image2MouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int off=0,len=0;
	int tmp_y=Y/2;
	int whichTable=bankActive/16;
	int curbank=curViewCHRbankCanvas2;
	int set_off;
	clickSent=true;

	if(btnFree->Down){
		int sx,sy,sw,sh;
		int dx,dy,dw,dh;

		bnkSelection = bnkCursor;
		FormMain->GetSelection(bnkSelection,sx,sy,sw,sh);
		FormMain->GetSelection(curSelection,dx,dy,dw,dh);
		off=sy*16+sx;
		set_off=dy*16+dx;
		for(int y=0; y<dh; y++){
			for(int x=0; x<dw; x++){

				bankViewTable[whichTable + (dy+y)*16 + dx+x] = (off + curbank*256 - set_off)*16;
			}
		}
	}
	else{
		if(btn256b->Down) {off=tmp_y/8;  		 len=1;}
		if(btn512b->Down) {off=tmp_y/16; off*=2; len=2;}
		if(btn1k->Down)	  {off=tmp_y/32; off*=4; len=4;}
		if(btn2k->Down)   {off=tmp_y/64; off*=8; len=8;}
		if(btn4k->Down)   {off=0;   len=16;}

		bnkSelection = TRect(0, off, 16, off+len);


	//int *ptr;
	int row=16;
	off *=16;
	len *=16;

		set_off=curSelection.Top*row;

		int cnt=0;


		for(int i=off;i<off+len;i++){
			bankViewTable[whichTable + set_off + cnt] = (off + curbank*256 - set_off)*16;
			cnt++;
		}
	}
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btn4kClick(TObject *Sender)
{
   curSelection = TRect(0, 0, 16, 16);
   bnkSelection = TRect(0, 0, 16, 16);
   Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btn2kClick(TObject *Sender)
{
  int len;
  int x,y,w,h;

  if(Sender==btn2k) len=8;
  if(Sender==btn1k) len=4;
  if(Sender==btn512b) len=2;
  if(Sender==btn256b) len=1;

  FormMain->GetSelection(curSelection,x,y,w,h);
  y/=len; y*=len;
  curSelection = TRect(0, y, 16, y+len);

  FormMain->GetSelection(bnkSelection,x,y,w,h);
  y/=len; y*=len;
  bnkSelection = TRect(0, y, 16, y+len);

  Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image1MouseEnter(TObject *Sender)
{
	curSetHover=true;
	FormMain->LabelStats->Caption="View/work set: Represents the tiles currently viewed and edited; by default mapped to 1 physical bank each.\nClicking selects a 'bank window', which can be mapped to the contents of any bank.\nChoose the size of a window with the 'select' buttons: 4k is the whole canvas, while 1/4 is 1 row.\nMapping tiles to a window is done by clicking on the other canvas (bank canvas).";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image1MouseLeave(TObject *Sender)
{
	curSetHover=false;
	clickSent=false;
	FormMain->LabelStats->Caption="---";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image2MouseLeave(TObject *Sender)
{
	bnkSetHover=false;
	clickSent=false;
	FormMain->LabelStats->Caption="---";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image2MouseEnter(TObject *Sender)
{
	bnkSetHover=true;
	FormMain->LabelStats->Caption="Views 256 tiles (4k) of physical tile bank memory (CHR memory in NES jargon).\nClicking assigns this bank to the current work/view set.\nIf using bank windows, clicking will assign the chosen portion of the set to the chosen window in the other canvas.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image2MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	int off=0,len=0;
	int tmp_y=Y/2;
	if(btnFree->Down){
		int tx=X/16;
		int ty=Y/16;

		int w = abs(curSelection.right - curSelection.left);
		int h = abs(curSelection.bottom - curSelection.top);

		bnkCursor.left=tx;
		bnkCursor.right=tx+w;
		bnkCursor.top=ty;
		bnkCursor.bottom=ty+h;


		for (int i=0; i<16; i++)  //long enough loop
		{
			if(bnkCursor.left<0)
			{	bnkCursor.left++;
				bnkCursor.right++;
			}

			if(bnkCursor.right>0x10)
			{	bnkCursor.left--;
				bnkCursor.right--;
			}

			if(bnkCursor.top<0 )
			{	bnkCursor.top++;
				bnkCursor.bottom++;
			}
			if(bnkCursor.bottom>0x10)
			{	bnkCursor.top--;
				bnkCursor.bottom--;
			}
		}
	}
	else{
		if(btn256b->Down) {off=tmp_y/8;  len=1;}
		if(btn512b->Down) {off=tmp_y/16; off*=2; len=2;}
		if(btn1k->Down)	  {off=tmp_y/32; off*=4; len=4;}
		if(btn2k->Down)   {off=tmp_y/64; off*=8; len=8;}
		if(btn4k->Down)   {off=0;   len=16;}
		bnkCursor = TRect(0, off, 16, off+len);
	}

    clickSent=false;
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::DrawTimerTimer(TObject *Sender)
{
	if(!openByFileDone) return;
	Draw();
	DrawTimer->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	int off=0,len=0;

	int tx=X/8;
	int ty=Y/8;
	curSetHover=true;
	if(btnFree->Down){
		if(Shift.Contains(ssRight)&&!bOutsideSel)
		{
			curSelection.left=curSelBuf.left+tx-bankview_xDown;
			curSelection.right=curSelBuf.right+tx-bankview_xDown;
			curSelection.top=curSelBuf.top+ty-bankview_yDown;
			curSelection.bottom=curSelBuf.bottom+ty-bankview_yDown;


			for (int i=0; i<16; i++)  //long enough loop
			{
				if(curSelection.left<0)
				{	curSelection.left++;
					curSelection.right++;
				}

				if(curSelection.right>0x10)
				{	curSelection.left--;
					curSelection.right--;
				}

				if(curSelection.top<0 )
				{	curSelection.top++;
					curSelection.bottom++;
				}
				if(curSelection.bottom>0x10)
				{	curSelection.top--;
					curSelection.bottom--;
				}
			}
		}
		//box selection
		else if(Shift.Contains(ssLeft))
		{
			curSetHover=false;
			if(Shift.Contains(ssShift)){
				if(tx<bankview_xDown)
				{	curSelection.left=tx+1-(tx<curSelection.right?1:0);
					curSelection.right=bankview_xDown+1;
				}
				if(tx>=bankview_xDown)
				{	curSelection.right =tx+(tx>=curSelection.left?1:0);
					curSelection.left=bankview_xDown;
				}

				if(curSelection.left<0)			curSelection.left=0;
				if(curSelection.right>0x10) curSelection.right=0x10;


				if(curSelection.top<0 )			curSelection.top=0;
				if(curSelection.bottom>0xF)	curSelection.bottom=0xF;

			}
			else{
				curSelection.left  =0;
				curSelection.right =16;

			}

			if(ty<bankview_yDown)
			{	curSelection.top=ty-(ty>=curSelection.bottom ?1:0);
				curSelection.bottom=bankview_yDown+1;
			}
			if(ty>=bankview_yDown)
			{	curSelection.bottom=ty+(ty>=curSelection.top ?1:0);
				curSelection.top=bankview_yDown;
			}
		}
		else{
			if(Shift.Contains(ssShift)){
				curCursor = TRect(X/8, Y/8, X/8+1, Y/8+1);
			}
			else{
				curCursor = TRect(0, Y/8, 16, Y/8+1);
			}
		}
	}
	else{
		if(btn256b->Down) {off=Y/8;  len=1;}
		if(btn512b->Down) {off=Y/16; off*=2; len=2;}
		if(btn1k->Down)	  {off=Y/32; off*=4; len=4;}
		if(btn2k->Down)   {off=Y/64; off*=8; len=8;}
		if(btn4k->Down)   {off=0;   len=16;}

		curCursor = TRect(0, off, 16, off+len);
	}
	clickSent=false;
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormBankCHR::Clear1Click(TObject *Sender)
{
	FormMain->SetUndo();

	int id=ListBox1->ItemIndex;
	memset(&chr[id*CHR_4k],0,CHR_4k);
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Up1Click(TObject *Sender)
{
	int id=ListBox1->ItemIndex;
	unsigned char temp[CHR_4k];

	if(id<1) return;

	FormMain->SetUndo();
	//data
	memcpy(&temp					,&chr[(id-1)*CHR_4k],CHR_4k);
	memcpy(&chr[(id-1)*CHR_4k]	,&chr[id*CHR_4k]	,CHR_4k);
	memcpy(&chr[id*CHR_4k]		,&temp	  			,CHR_4k);

	//list
	std::iter_swap(chrBankLabels.begin()+id,chrBankLabels.begin()+id-1);
    ListBox1->ItemIndex--;
	MakeList(false,false);
    FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Down1Click(TObject *Sender)
{
    int id=ListBox1->ItemIndex;
	int total=ListBox1->Items->Count-1;
	unsigned char temp[CHR_4k];

	if(id>=total) return;

	FormMain->SetUndo();
	//data
	memcpy(&temp				,&chr[(id+1)*CHR_4k],CHR_4k);
	memcpy(&chr[(id+1)*CHR_4k]	,&chr[id*CHR_4k]	,CHR_4k);
	memcpy(&chr[id*CHR_4k]		,&temp					,CHR_4k);

	//list
	std::iter_swap(chrBankLabels.begin()+id,chrBankLabels.begin()+id+1);
    ListBox1->ItemIndex++;
	MakeList(false,false);
}
//---------------------------------------------------------------------------


void __fastcall TFormBankCHR::Remove1Click(TObject *Sender)
{
	if (chrBanks<=4) return;

	FormMain->SetUndo();

	unsigned int id=ListBox1->ItemIndex;
	int total=ListBox1->Items->Count-1;



	//push working sets
	/*
	for(int b=0;b<4;b++)for(int i=0;i<256;i++){
		if(id*4096<=bankViewTable[b*256 + i])bankViewTable[b*256 + i]-=4096;
	} */
	for( int b=0;b<4;b++){
		for( int i=0;i<256;i++){
			if(bankViewTable[b*256 + i]>=id*CHR_4k) bankViewTable[b*256 + i] -=CHR_4k;
		}
	}

	//push banks
	for(int i=id;i<total;++i)
	{
		memcpy(&chr[i*CHR_4k],&chr[(i+1)*CHR_4k],CHR_4k);
	}



	chrBankLabels.erase(chrBankLabels.begin() + id);
	ListBox1->Items->Delete(id);
	ListBox1->ItemIndex = min(id,total-1);
	//decrease chrBank memory
	chrBanks--;
	//chrBank = (char*)realloc(chrBank,chrBanks*CHR_4k * sizeof(char));
	MakeList(false,false);
	if (chrBanks<=4) Remove1->Enabled=false;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::SpeedButton1Click(TObject *Sender)
{
	int id=ListBox1->ItemIndex;
	FormName->Caption="Rename CHR bank label";
	FormName->EditName->Text=chrBankLabels.at(id).c_str();
	FormName->ShowModal();
	if(bKeyEscape) return;
	chrBankLabels.at(id)=FormName->EditName->Text.c_str();
	MakeList(false,false);
	FormSwapBanks->UpdateLists(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::ListBox1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	//while holding a button, create an onHover event for showing tileset on canvas 2.	
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::ListBox1Click(TObject *Sender)
{
	curViewCHRbankCanvas2=ListBox1->ItemIndex;
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::OpenByFileAssociationMakeListTimerTimer(
      TObject *Sender)
{
	//this is only to be called when loading by file association. Else the timer is not active.
	if(!openByFileDone) return;
	MakeList(true,false);
	OpenByFileAssociationMakeListTimer->Enabled=false;
}
//---------------------------------------------------------------------------


void __fastcall TFormBankCHR::Button1Click(TObject *Sender)
{
   curViewCHRbankCanvas2=1;
	Draw();
}
//---------------------------------------------------------------------------



void __fastcall TFormBankCHR::Copy1Click(TObject *Sender)
{
	extern CF_CHR;
	unsigned char tmp_chrCopy[4096];
	unsigned char tmp_propCopy[256];
	int i,j,k,pp,ps,x,y,w,h;
	//int chrmax = chrBanks*4096;
	//int propmax = chrBanks*256;
	int bank= ListBox1->ItemIndex*4096;
    bool cut=false;
	//FormMain->SetUndo();
	int prop_p;

	x=0;
	y=0;
	w=16;
	h=16;
	if(cut) FormMain->SetUndo();

	pp=0;
	prop_p=0;
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			for(k=0;k<16;k++)
			{
				ps=bank+((x+j) + (y+i)*16)*16 + k;

				tmp_chrCopy[pp++]=chr[ps];
				if(cut)  chr[ps]=0;
			}
			tmp_propCopy[prop_p++]=tileProperties[bank/16 + (x+j) + (y+i)*16];
			if(cut)     tileProperties[bank/16 + (x+j) + (y+i)*16]=0;
		}
	}





	DWORD Len=3*sizeof(int)+sizeof(tmp_chrCopy)+sizeof(tmp_propCopy);

	HGLOBAL hDst=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,Len);

	if (hDst)
	{
		LPSTR gDst=(LPSTR)GlobalLock(hDst);

		if (gDst)
		{
			if (OpenClipboard(Handle))
			{
				EmptyClipboard();

				*((int*)&gDst[0*sizeof(int)])=w;
				*((int*)&gDst[1*sizeof(int)])=h;
				*((int*)&gDst[2*sizeof(int)])=true;
				memcpy(&gDst[12],tmp_chrCopy,sizeof(tmp_chrCopy));
				memcpy(&gDst[12+sizeof(tmp_chrCopy)],tmp_propCopy,sizeof(tmp_propCopy));


				GlobalUnlock(hDst);

				SetClipboardData(CF_CHR,hDst);
				CloseClipboard();
			}
		}
	}	
}
//---------------------------------------------------------------------------


void __fastcall TFormBankCHR::Paste1Click(TObject *Sender)
{
	//general
	extern CF_CHR;
	extern chrCopyWidth;
	extern chrCopyHeight;
	extern chrCopyRect;

	unsigned char tmp_chrCopy[4096];
	unsigned char tmp_propCopy[256];

	int i,j,k,pp,pd,x,y,w,h;

	int bank= ListBox1->ItemIndex*4096;

	bool bIsFlex=false;

	int xlen;   //width of selection
	int ylen;   //height of selection
	//int chrmax = chrBanks*4096;
	//int propmax = chrBanks*256;

	
	int maxlen; //the longest a paste run can be; based on clipboard contents
	int countlen = 0; //manual counter for the above.



	int propOff; //used by rect selection sources for properties.
	//used by bitplane masks
	int fLen=16;
	int fOff=0;

	//used by colour masks
	unsigned char mask[16];
	bool b0 = FormCHREditor->Protect0->Down;
	bool b1 = FormCHREditor->Protect1->Down;
	bool b2 = FormCHREditor->Protect2->Down;
	bool b3 = FormCHREditor->Protect3->Down;

	//apply bitplane masks
	if (!(FormCHREditor->ButtonBitmaskLo->Down||FormCHREditor->ButtonBitmaskHi->Down))
		{if(FormMain->Applytopaste1->Checked) return;}
	if (!FormCHREditor->ButtonBitmaskLo->Down&&FormMain->Applytopaste1->Checked) {fLen-=8; fOff=8;}
	if (!FormCHREditor->ButtonBitmaskHi->Down&&FormMain->Applytopaste1->Checked) {fLen-=8;}


	//get clipboard
	OpenClipboard(Handle);

	HGLOBAL hClipBuf = GetClipboardData(CF_CHR);

	if (hClipBuf)
	{
		LPSTR gSrc = (LPSTR)GlobalLock(hClipBuf);

		if (gSrc != NULL)
		{
			chrCopyWidth =*((int*)&gSrc[0*sizeof(int)]);
			chrCopyHeight=*((int*)&gSrc[1*sizeof(int)]);
			chrCopyRect  =*((int*)&gSrc[2*sizeof(int)]);
			memcpy(tmp_chrCopy,&gSrc[12],sizeof(tmp_chrCopy));
			memcpy(tmp_propCopy,&gSrc[12+sizeof(tmp_chrCopy)],sizeof(tmp_propCopy));

			GlobalUnlock(gSrc);
		}
	}

	CloseClipboard();

    //if clipboard was 2-dimensional
	if(chrCopyRect)
	{
		FormMain->SetUndo();
		FormMain->GetSelection(bnkSelection,x,y,w,h);
		if(w&&h) bIsFlex=true;
		else { x=0; y=0; w=16; h=16;}
    	maxlen = chrCopyHeight * chrCopyWidth;
	
		if(bIsFlex)
		{
		   xlen = w;
		   ylen = h;
		}
		else
		{
			xlen = chrCopyWidth;
			ylen = chrCopyHeight;
		}

		//start of paste from buffer.
		pp=0;

		for(i=0;i<ylen;i++)
		{
			for(j=0;j<xlen;j++)
			{
				if (countlen >= maxlen) continue;

				if(x+j<16&&y+i<16)  //safety for making sure selection isnÃ‚Â´t larger than table.
				{
					for(int m=0;m<16;m++) mask[m]=0xFF; //set mask.

					for(int l=0;l<8;l++)
					{

						pd=bank + ((x+j) + (y+i)*16) *16 + l;
						//set mask
						if(FormMain->Applytopaste2->Checked){

							if (b0) mask[l]	   =~mask[l]	|(chr[pd]	|chr[pd+8]);
							if (b1) mask[l]    = mask[l]   &~(chr[pd]	&(chr[pd]^chr[pd+8]));
							if (b2)	mask[l]    = mask[l]   &~(chr[pd+8]	&(chr[pd]^chr[pd+8]));
							if (b3) mask[l]	   = mask[l]	-(chr[pd]	&chr[pd+8]);

							mask[l+8]	=mask[l]; //makes the 1bit mask applicable to 2bit gfx.
						}
					}

					for(k=0;k<fLen;k++)  //pasting tile
					{

						pd=bank + ((x+j) + (y+i)*16) *16 + k;
						chr[pd+fOff]=(chr[pd+fOff]&~mask[k+fOff]);
						chr[pd+fOff]=chr[pd+fOff]|(tmp_chrCopy[pp+k+fOff]&mask[k+fOff]);

					}
					tileProperties[bank/16 + (x+j) + (y+i)*16]=tmp_propCopy[pp/16];
					//propOff =  tileViewTable[(x+j) + (y+i)*16];
					//tileProperties[((bankViewTable[bankActive/16]/16)%propmax) + propOff]=propCopy[(pp/16)];

				}
				pp+=16;
				countlen++;
			}
		}
	}
	//if clipboard was 1-dimensional (from multi-select, likely).
	else
	{
		if(chrCopyWidth<1) return;
		FormMain->SetUndo();
		FormMain->GetSelection(bnkSelection,x,y,w,h);
		if(w&&h) bIsFlex=true;
		else { x=0; y=0; w=16; h=16;}

		if(bIsFlex)
		{
		   xlen = w;
		   ylen = h;
		}
		else
		{
			xlen = chrCopyWidth;
			ylen = 1;
		}
		pp=0+fOff;
		pd=0+fOff;
		int prop_p = 0;
		int prop_d = 0;
		//added to signal the origin to the user, in case of no selection or present multi selection


		for(j=0;j<ylen;j++)
		{
			for(i=0;i<xlen;i++)
			{
				if (countlen >= chrCopyWidth) continue;
				if (bIsFlex) {pd=(bank)+(((x+i)*16 +(y+j)*256 + fOff)&0x0fff);
							  prop_d=(bank/16)+(((x+i) +(y+j)*16)&0xff);
				}
				else 		 {pd=bank+(pd&0x0fff);
							  prop_d=(bank/16)+(prop_d&0xff);
				}
				for(int m=0;m<16;m++) mask[m]=0xFF; //set mask.

				for(int l=0;l<8;l++)
				{
					//pd=bankActive+(x+j)*16+(y+i)*256+l;
					//set mask
					if(FormMain->Applytopaste2->Checked){

						if (b0) mask[l]	   =~mask[l]	|(chr[pd+l]	|chr[pd+8+l]);
						if (b1) mask[l]    = mask[l]   &~(chr[pd+l]	&(chr[pd+l]^chr[pd+8+l]));
						if (b2)	mask[l]    = mask[l]   &~(chr[pd+8+l]	&(chr[pd+l]^chr[pd+8+l]));
						if (b3) mask[l]	   = mask[l]	-(chr[pd+l]	&chr[pd+8+l]);

						mask[l+8]	=mask[l]; //makes the 1bit mask applicable to 2bit gfx.
					}
				}

				for(k=0;k<fLen;k++)
				{
					//new; using colour protection masks
					chr[pd]=(chr[pd]&~mask[k]);          //+fOff
					chr[pd]=chr[pd]|(tmp_chrCopy[pp]&mask[k]);   //+fOff
					pd++;
					pp++;

					//original
					//chr[pd++]=chrCopy[pp++];
				}
				if(fLen==8){pp+=8;pd+=8;}
				countlen++;

				tileProperties[prop_d]=tmp_propCopy[prop_p];
				prop_p++;
				prop_d++;
			}
		}
	}
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Insert1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Inserts a 4k chr bank before the currently selected bank.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Clear1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Clears all contents from the currently selected bank.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Duplicate1MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Inserts a duplicate of the currently selected bank; before it.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Up1MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Moves the bank item up 4k.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Down1MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Moves the bank item down 4k.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::SpeedButton1MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Gives your bank a new label.\n[Enter] confirms the change, [esc] exits without change.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Copy1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Puts the current bank item on the clipboard.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Paste1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Pastes CHR into the current bank item.\nPastes can be delimited by using a selection rectangle.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Remove1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Removes the currently selected bank item.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Insert1MouseLeave(TObject *Sender)
{
	FormMain->LabelStats->Caption="---";	
}
//---------------------------------------------------------------------------



void __fastcall TFormBankCHR::btn4kMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Sets the bank window selection to a full set of 256 tiles (4k in NES CHR).\nTip: useful for resetting a work/view set to a single bank.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btn2kMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Sets the bank window selection to a half set of 128 tiles (8 rows).";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btn1kMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Sets the bank window selection to a set of 64 tiles (4 rows).";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btn512bMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Sets the bank window selection to a set of 32 tiles (2 rows).";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btn256bMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Sets the bank window selection to a set of 16 tiles (1 row).";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::SpeedButton5Click(TObject *Sender)
{
	FormMain->SetUndo();
	for(int b=0;b<4;b++)
		for (int i=0; i < 256; i++) {
			bankViewTable[b*256 + i] = b*4096;
	}
	FormMain->UpdateAll();	
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::SpeedButton5MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Resets CHR bank windows to startup default.\nWork sets A/B/C/D are mapped to CHR banks 0,1,2,3.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btnFreeClick(TObject *Sender)
{
   //curSelection = TRect(0, 0, 1, 1);
   //bnkSelection = TRect(0, 0, 1, 1);
   Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btnFreeMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Free selection mode:\n[Drag] on the current work set to pick any number of rows. [Shift-drag] to pick a rectangle of tiles.\n[Click], as usual, on the CHR bank canvas to assign the selection.\nNote:\tClassic NES mappers can't map banks to individual tiles directly, but newer mappers may.";
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    

	if(!Shift.Contains(ssCtrl)&&!Shift.Contains(ssShift))
	{
		bool b=false;
		if(Key==VK_F1) {FormMain->PageControlEditor->ActivePageIndex=0; b=true;}
		if(Key==VK_F2) {FormMain->PageControlEditor->ActivePageIndex=1; b=true;}
		if(Key==VK_F3) {FormMain->PageControlEditor->ActivePageIndex=2; b=true;}
		if (b) {FormMain->PageControlEditorChange(Sender);}
        //*/

		if(Key==VK_PAUSE)
		{
			FormMain->AlphaBlend^=true;
			if (!Shift.Contains(ssShift))
			{
				FormCHREditor->AlphaBlend^=true;
				FormManageMetasprites->AlphaBlend^=true;
			}
		}




			if(Key=='R') FormMain->Red1Click(FormMain->Red1);
			if(Key=='G') FormMain->Red1Click(FormMain->Green1);
			if(Key=='B') FormMain->Red1Click(FormMain->Blue1);
			if(Key=='M') FormMain->Red1Click(FormMain->Gray1);

			//if(Key=='F' && !(nameSelection.left<0&&nameSelection.top<0)) FillMap();

			if(Key=='X') FormMain->Toggletileset1Click(FormMain->Toggletileset1);
			if(Key=='A') FormMain->Attributes1Click(FormMain->Attributes1);
			if(Key=='S') FormMain->SelectedOnly1Click(FormMain->SelectedOnly1);
			if(Key=='W') FormMain->ApplyTiles1Click(FormMain->ApplyTiles1);
			if(Key=='E') FormMain->ApplyAttributes1Click(FormMain->ApplyAttributes1);
			if(Key=='D') FormMain->Tilegrid1Click(FormMain->GridOnOff1);
            

	}
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::SpeedButton2Click(TObject *Sender)
{
	FormSwapBanks->Show();	
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::SpeedButton2MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Shows the complimentary CHR bank swap tool";
}
//---------------------------------------------------------------------------


