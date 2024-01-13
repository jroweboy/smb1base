//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#pragma hdrstop
#include "UnitMain.h"
#include "UnitName.h"
#include "UnitBankCHR.h"
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

extern unsigned char chr[];
extern unsigned int curViewCHRbankCanvas2;


const int CHR_4k=4096;
const int bankwin=8;
extern char *chrBank;
//extern AnsiString *chrBankNames;
extern std::vector<std::string> chrBankLabels;

extern char tileViewTable[];
extern int palActive;
extern char chrSelected[];

extern int chrA_id[];
extern int chrB_id[];
extern int chrBanks;
AnsiString strList;
TRect curSelection;
TRect bnkSelection;
TRect bnkCursor;
TRect curCursor;
bool isBnkCursor=false;
bool curSetHover=false;
bool bnkSetHover=false;
bool clickSent=false;
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

	btnA->Down = FormMain->SpeedButtonChrBank1->Down;
	btnB->Down = FormMain->SpeedButtonChrBank2->Down;
	int x=0;
	int y=0;
	Image1     ->Picture->Bitmap->SetSize(128,128);
	Image2     ->Picture->Bitmap->SetSize(128,128);



	for(int i=0;i<256;i++) //tiles
	{
		//DrawTile(TPicture *pic, int x, int y, int tile, int pal, int tx, int ty, bool sel, bool efficientTarget, int inputScale, bool bIsNav, bool doubleWidth, bool banked)
		unsigned int bi=i+(256*curViewCHRbankCanvas2);

		FormMain->DrawTile(Image1->Picture,x,y,i,palActive,-1,-1,chrSelected[i],true,1,false,false,false);
		FormMain->DrawTile(Image2->Picture,x,y,bi,palActive,-1,-1,chrSelected[i],true,1,false,false,true);

		x+=8;
		if(x>=128){	x=0; y+=8; }
	}

	isBnkCursor=false;
	FormMain->DrawSelection(Image1,curSelection,1,true,false);
	FormMain->DrawSelection(Image2,bnkSelection,1,false,false);

	if(!clickSent)
	{isBnkCursor=true;
	if(curSetHover)	FormMain->DrawSelection(Image1,curCursor,1,true,false);
	if(bnkSetHover) FormMain->DrawSelection(Image2,bnkCursor,1,false,false);
	isBnkCursor=false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::MakeList(bool bSelectTop, bool bInit)
{
	if(bInit) ListBox1->Clear();
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
	Image1->Picture=new TPicture();
	Image1->Picture->Bitmap=new Graphics::TBitmap();
	Image1->Picture->Bitmap->PixelFormat=pf24bit;

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

	//curSelection.Height(len*8);
	//curSelection.Top(0);
	curSelection = TRect(0, 0, 16, 0 + len);
	curViewCHRbankCanvas2=ListBox1->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Insert1Click(TObject *Sender)
{
	int i,id, total;
	bool bDuplicate = ((TMenuItem*)Sender)->Tag==1?true:false;
	FormMain->SetUndo();

	id=ListBox1->ItemIndex;
	total=ListBox1->Items->Count-1;

	//expand chrBank memory
	chrBanks++;
	chrBank = (char*)realloc(chrBank,chrBanks*CHR_4k);

	/*
	AnsiString* newChrBankNames = new AnsiString[chrBanks];
	for (int i=0; i<chrBanks; i++) newChrBankNames[i] = chrBankNames[i];
	delete[] chrBankNames;
	chrBankNames = newChrBankNames;
	*/

	//push working sets
	for(i=0;i<bankwin;i++)
	{
		if (id>chrA_id[i]) chrA_id[i]+=bankwin;
		if (id>chrB_id[i]) chrB_id[i]+=bankwin;
	}
	//push banks
	chrBankLabels.insert(chrBankLabels.begin() + id + 1, chrBankLabels[id]);

	for(i=total;i>id;--i)
	{
		memcpy(&chrBank[i*CHR_4k],&chrBank[(i-1)*CHR_4k],CHR_4k);
		
	}
	//insertion mode
	if(!bDuplicate)
	{
		memset(&chrBank[id*CHR_4k],0,CHR_4k);
		chrBankLabels[id]="Unlabeled";
	}

	MakeList(false,false);
	if (chrBanks>1) Remove1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::btnAClick(TObject *Sender)
{
	FormMain->SpeedButtonChrBank1->Down=btnA->Down;
    FormMain->SpeedButtonChrBank2->Down=btnB->Down;
}
//---------------------------------------------------------------------------



void __fastcall TFormBankCHR::Image1MouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int off=0,len=0;

	if(btn256b->Down) {off=Y/8;  len=1;}
	if(btn512b->Down) {off=Y/16; off*=2; len=2;}
	if(btn1k->Down)	  {off=Y/32; off*=4; len=4;}
	if(btn2k->Down)   {off=Y/64; off*=8; len=8;}
	if(btn4k->Down)   {off=0;   len=16;}

	curSelection = TRect(0, off, 16, off+len);
    clickSent=true;
	Draw();
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int off=0,len=0;
	int tmp_y=Y/2;

	if(btn256b->Down) {off=tmp_y/8;  len=1;}
	if(btn512b->Down) {off=tmp_y/16; off*=2; len=2;}
	if(btn1k->Down)	  {off=tmp_y/32; off*=4; len=4;}
	if(btn2k->Down)   {off=tmp_y/64; off*=8; len=8;}
	if(btn4k->Down)   {off=0;   len=16;}

	bnkSelection = TRect(0, off, 16, off+len);
	clickSent=true;

	FormMain->BankCHRToWorkCHR();
	Draw();
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
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image1MouseLeave(TObject *Sender)
{
	curSetHover=false;
    clickSent=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image2MouseLeave(TObject *Sender)
{
	bnkSetHover=false;
	clickSent=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image2MouseEnter(TObject *Sender)
{
	bnkSetHover=true;	
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Image2MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	int off=0,len=0;
	int tmp_y=Y/2;

	if(btn256b->Down) {off=tmp_y/8;  len=1;}
	if(btn512b->Down) {off=tmp_y/16; off*=2; len=2;}
	if(btn1k->Down)	  {off=tmp_y/32; off*=4; len=4;}
	if(btn2k->Down)   {off=tmp_y/64; off*=8; len=8;}
	if(btn4k->Down)   {off=0;   len=16;}

	bnkCursor = TRect(0, off, 16, off+len);
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

	if(btn256b->Down) {off=Y/8;  len=1;}
	if(btn512b->Down) {off=Y/16; off*=2; len=2;}
	if(btn1k->Down)	  {off=Y/32; off*=4; len=4;}
	if(btn2k->Down)   {off=Y/64; off*=8; len=8;}
	if(btn4k->Down)   {off=0;   len=16;}

	curCursor = TRect(0, off, 16, off+len);
    clickSent=false;
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormBankCHR::Clear1Click(TObject *Sender)
{
	FormMain->SetUndo();

	int id=ListBox1->ItemIndex;
	memset(&chrBank[id*CHR_4k],0,CHR_4k);
}
//---------------------------------------------------------------------------

void __fastcall TFormBankCHR::Up1Click(TObject *Sender)
{
	int id=ListBox1->ItemIndex;
	unsigned char temp[CHR_4k];

	if(id<1) return;

	FormMain->SetUndo();
	//data
	memcpy(&temp					,&chrBank[(id-1)*CHR_4k],CHR_4k);
	memcpy(&chrBank[(id-1)*CHR_4k]	,&chrBank[id*CHR_4k]	,CHR_4k);
	memcpy(&chrBank[id*CHR_4k]		,&temp					,CHR_4k);

	//list
	std::iter_swap(chrBankLabels.begin()+id,chrBankLabels.begin()+id-1);
    ListBox1->ItemIndex--;
	MakeList(false,false);

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
	memcpy(&temp					,&chrBank[(id+1)*CHR_4k],CHR_4k);
	memcpy(&chrBank[(id+1)*CHR_4k]	,&chrBank[id*CHR_4k]	,CHR_4k);
	memcpy(&chrBank[id*CHR_4k]		,&temp					,CHR_4k);

	//list
	std::iter_swap(chrBankLabels.begin()+id,chrBankLabels.begin()+id+1);
    ListBox1->ItemIndex++;
	MakeList(false,false);
}
//---------------------------------------------------------------------------


void __fastcall TFormBankCHR::Remove1Click(TObject *Sender)
{
	if (chrBanks<=1) return;

	FormMain->SetUndo();

	int id=ListBox1->ItemIndex;
	int total=ListBox1->Items->Count-1;



	//push working sets

	for(int i=0;i<bankwin;i++)
	{
		if (id<=chrA_id[i]) chrA_id[i]-=bankwin;
		if (id<=chrB_id[i]) chrB_id[i]-=bankwin;
	}


	//push banks
	for(int i=id;i<total;++i)
	{
		memcpy(&chrBank[i*CHR_4k],&chrBank[(i+1)*CHR_4k],CHR_4k);
	}

	chrBankLabels.erase(chrBankLabels.begin() + id);
	ListBox1->Items->Delete(id);
	ListBox1->ItemIndex = min(id,total-1);
	//decrease chrBank memory
	chrBanks--;
	//chrBank = (char*)realloc(chrBank,chrBanks*CHR_4k * sizeof(char));
	MakeList(false,false);
	if (chrBanks<=1) Remove1->Enabled=false;
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

