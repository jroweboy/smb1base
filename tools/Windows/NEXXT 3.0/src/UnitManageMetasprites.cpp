//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitCHREditor.h"
#include "UnitManageMetasprites.h"
#include "UnitMetaspritePlaybackRules.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormManageMetasprites *FormManageMetasprites;

AnsiString formTitle = "Metasprite manager | direct animator";

extern AnsiString metaSpriteBankName;
extern AnsiString metaSpriteNames[256];
extern AnsiString metaSpriteNamesUndo[256];
extern AnsiString tmpMetaSprName;
extern int metaSpriteActive;
extern unsigned char metaSprites[];
extern bool prefStartShowMM;
extern bool cueUpdateMM;
extern bool bSnapToScreen;

TDateTime time1, time2;

bool bMetaManagerEditLabelActive=false;
bool bIgnoreChangeFrameRate=false;
bool bReversePlayback=true;
extern int metaSpriteDirectFrame_Cursor;
extern uint32_t  metaSpriteDirectFrame_NTSC[256];
extern uint32_t metaSpriteDirectFrame_PAL[256];
extern unsigned char metaspriteDirectBytecode[256];

extern bool metaSpriteDirectStart[256];
extern bool metaSpriteDirectLoop[256];
extern bool metaSpriteDirectCall[256];
extern bool metaSpriteDirectCall2[256];
extern bool metaSpriteDirectRevert[256];
extern bool metaSpriteDirectSeconds[256];
extern bool metaSpriteDirectHalfSeconds[256];

extern int iMsprTimeDelay;
extern TImageList *imageListMetaSprites;


extern bool isLastClickedMetaSprite;
extern bool	isLastClickedSpriteList;
extern bool cueUpdateMetasprite;
extern bool cueStats;

int iEditStep=1;

extern int  iWarnMsprCyan;
extern bool bWarnMsprCyan;
extern bool bWarnMsprYellow;
extern bool bWarnMsprOrange;
extern bool bWarnMsprRed;


extern bool prefStartMsprListID;
extern bool prefStartMsprListLabel;
extern bool prefStartMsprListNTSC;
extern bool prefStartMsprListPAL;
extern bool prefStartMsprListCount;
extern bool prefStartMsprListTag;



bool msprList_isDragging=false;
int  msprList_dragID= -1;
int  msprList_dropID= -1;
inline int min(int a, int b) {
	return (a < b) ? a : b;
}
inline int max(int a, int b) {
	return (a > b) ? a : b;
}
	Graphics::TBitmap *animBuf = new Graphics::TBitmap;


 float zoomFactor = 2.0; // Zoom level of 200%
int zoomZoomedWidth = 128 * zoomFactor;
int zoomZoomedHeight = 128 * zoomFactor;

int zoomImageWidth = 128;
int zoomImageHeight = 128;

int zoomPosX = (zoomImageWidth - zoomZoomedWidth) / 2;
int zoomPosY = (zoomImageHeight - zoomZoomedHeight) / 3;

int zoomPosX_start = zoomPosX;
int zoomPosY_start = zoomPosY;


TRect zoomDstRect(zoomPosX, zoomPosY, zoomPosX + zoomZoomedWidth, zoomPosY + zoomZoomedHeight);
//TRect zooomSrcRect(0, 0, animBuf->Width, animBuf->Height);


int	iMsprMouseX_down;
int	iMsprMouseY_down;






	void __fastcall TFormManageMetasprites::Update(void)
{

	AnsiString str;
	int i,j,cnt;
	bool bCall, bCall2;
	bool bLoop;
	bool bStart;
	bool bRevert;
	bool bSeconds;
	bool bHalfSeconds;
	ListBoxSprites->ItemIndex=metaSpriteActive;
	//cueUpdateMM=false;
	for(i=0;i<256;++i)
	{
		/*
		str=IntToStr(i)+": "+metaSpriteNames[i]+" | ";

		cnt=0;

		for(j=0;j<64*4;j+=4)
		{
			if(metaSprites[i*64*4+j]<255) ++cnt;
		}

		if(!cnt) str+="empty"; else str+=IntToStr(cnt)+" sprites";
		*/
		bCall  		=  metaSpriteDirectCall[i] && RadioCall->Caption=="call";
		bLoop  		=  metaSpriteDirectLoop[i];
		bStart 		=  metaSpriteDirectStart[i] && RadioStart->Caption=="start";
		bCall2 		=  metaSpriteDirectCall2[i] && RadioStart->Caption=="call2";


		bRevert		=  metaSpriteDirectRevert[i] && (RadioStart->Caption=="revert" || RadioCall->Caption=="revert");
		bSeconds	=  metaSpriteDirectSeconds[i] && (RadioStart->Caption=="seconds" || RadioCall->Caption=="seconds");
		bHalfSeconds=  metaSpriteDirectHalfSeconds[i] && (RadioStart->Caption=="h. secs" || RadioCall->Caption=="h. secs");

		str="";
		if(btnShowOrder->Down){
			str+=IntToStr(i)+": ";
			if(i<10) str+="  ";
		}
		if(btnShowLabels->Down) str = str + metaSpriteNames[i] + " | ";
		cnt = 0;
		for(j=0;j<64*4;j+=4)
		{
			if(metaSprites[i*64*4+j]<255) ++cnt;
		}


		if(btnShowNTSC->Down && btnShowPAL->Down)
			str = str + "Duration: " +StrToInt(metaSpriteDirectFrame_NTSC[i]) +":"+StrToInt(metaSpriteDirectFrame_PAL[i]);
		else if (btnShowNTSC->Down)
			str = str + "Duration: " +StrToInt(metaSpriteDirectFrame_NTSC[i]);
		else if (btnShowPAL->Down) 
			str = str + "Duration: " +StrToInt(metaSpriteDirectFrame_PAL[i]);

		if((btnShowNTSC->Down || btnShowPAL->Down) && btnShowCount->Down)
			str += " | ";

		if(btnShowCount->Down){
			if(!cnt) str+="empty"; 
			else if (cnt==1) str+=IntToStr(cnt)+" sprite";
			else    		   str+=IntToStr(cnt)+" sprites";
		}

		if(btnShowTags->Down){
			if(bCall) str+=" <call>";
			if(bCall2) str+=" <call2>";
			if(bStart) str+=" <start>";
			if(bLoop) str+=" <loop>";
			if(bRevert) str+=" <revert>";
			if(bSeconds) str+=" <seconds>";
			if(bHalfSeconds) str+=" <h. secs>";

		}

		ListBoxSprites->Items->Strings[i]=str;
	}

	MetaLabel->Text=metaSpriteNames[metaSpriteActive];
	//FormMain->DrawMetaSprite(ImageMetaSprite,metaSpriteActive,1,true,false,true);
}

void __fastcall TFormManageMetasprites::UpdateActive(bool doBitmap)
{

   AnsiString str;
   int i;
   int cnt=0;
   MetaLabel->Text=metaSpriteNames[metaSpriteActive];



   bool	bCall=  metaSpriteDirectCall[metaSpriteActive] && RadioCall->Caption=="call";
   bool	bLoop=  metaSpriteDirectLoop[metaSpriteActive];
   bool	bStart=  metaSpriteDirectStart[metaSpriteActive] && RadioStart->Caption=="start";
   bool	bCall2=  metaSpriteDirectCall2[metaSpriteActive] && RadioStart->Caption=="call2";
   bool	bRevert=  metaSpriteDirectRevert[metaSpriteActive] && (RadioStart->Caption=="revert" || RadioCall->Caption=="revert");
   bool	bSeconds=  metaSpriteDirectSeconds[metaSpriteActive] && (RadioStart->Caption=="seconds" || RadioCall->Caption=="seconds");
   bool	bHalfSeconds=  metaSpriteDirectHalfSeconds[metaSpriteActive] && (RadioStart->Caption=="h. secs" || RadioCall->Caption=="h. secs");


   //FormMain->DrawMetaSprite(ImageMetaSprite,metaSpriteActive,1,true,false,true);
   if(doBitmap){

   FormMain->DrawListedMetaSprite(imageListMetaSprites,metaSpriteActive,1,1,btnGrid->Down,btnBox->Down,!btnBox->Down, btnSil->Down, btnWarn->Down);
   if(SpeedButton1->Caption=="Play") {
	//Graphics::TBitmap *bitmap = new Graphics::TBitmap;
	//bitmap->Width=128;
	//bitmap->Height=128;

    

	AssignFrame(true);
	/*
	imageListMetaSprites->GetBitmap(metaSpriteActive, animBuf);

	if(btnZoom->Down) ImageMetaSprite->Canvas->StretchDraw(zoomDstRect, animBuf);
	else ImageMetaSprite->Picture->Bitmap->Assign(animBuf);
	*/


	//ImageMetaSprite->Picture->Bitmap->Assign(bitmap);


	//delete bitmap;
   }
   }
   //ImageMetaSprite->Picture->Bitmap->Assign(imageListMetaSprites->Bitmap[metaSpriteActive]);


   //update listing
   ListBoxSprites->ItemIndex=metaSpriteActive;
   str="";
		if(btnShowOrder->Down){
			str+=IntToStr(metaSpriteActive)+": ";
			if(metaSpriteActive<10) str+="  ";
		}
   if(btnShowLabels->Down) str = str + metaSpriteNames[metaSpriteActive] + " | "; 

   for(i=0;i<64*4;i+=4)
		{
			if(metaSprites[metaSpriteActive*64*4+i]<255) ++cnt;
		}


  if(btnShowNTSC->Down && btnShowPAL->Down)
	str = str + "Duration: " +StrToInt(metaSpriteDirectFrame_NTSC[metaSpriteActive]) +":"+StrToInt(metaSpriteDirectFrame_PAL[metaSpriteActive]);
  else if (btnShowNTSC->Down)
	str = str + "Duration: " +StrToInt(metaSpriteDirectFrame_NTSC[metaSpriteActive]);
  else if (btnShowPAL->Down) 
	str = str + "Duration: " +StrToInt(metaSpriteDirectFrame_PAL[metaSpriteActive]);

  
  if((btnShowNTSC->Down || btnShowPAL->Down) && btnShowCount->Down)
	str += " | ";

  if(btnShowCount->Down){
	if(!cnt) str+="empty"; 
	else if (cnt==1) str+=IntToStr(cnt)+" sprite";
	else    		   str+=IntToStr(cnt)+" sprites";
   }

  if(btnShowTags->Down){
		if(bCall) str+=" <call>";
		if(bCall2) str+=" <call2>";
		if(bStart) str+=" <start>";
		if(bLoop) str+=" <loop>";
		if(bRevert) str+=" <revert>";
		if(bSeconds) str+=" <seconds>";
		if(bHalfSeconds) str+=" <h. secs>";
  }


  ListBoxSprites->Items->Strings[metaSpriteActive]=str;

  //update direct animation settings


  RadioCall->Checked= bCall;
  RadioLoop->Checked= bLoop;
  RadioStart->Checked=bStart;
  RadioNone->Checked = !(bCall||bLoop||bStart);



  //todo: update frame duration here
  bIgnoreChangeFrameRate=true;
  //durationNTSC->Text=IntToStr(metaSpriteDirectFrame_NTSC[metaSpriteActive]);
  //durationPAL->Text=IntToStr(metaSpriteDirectFrame_PAL[metaSpriteActive]);

  bIgnoreChangeFrameRate=false;

}


//---------------------------------------------------------------------------
__fastcall TFormManageMetasprites::TFormManageMetasprites(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::FormCreate(TObject *Sender)
{
	FormManageMetasprites->Left=FormMain->Left - this->Width;
	FormManageMetasprites->Top=(Screen->Height-FormMain->Height)/4;
	ImageMetaSprite->Picture=new TPicture();
	ImageMetaSprite->Picture->Bitmap=new Graphics::TBitmap();
	ImageMetaSprite->Picture->Bitmap->PixelFormat=pf24bit;
	ImageMetaSprite->Picture->Bitmap->SetSize(ImageMetaSprite->Width,ImageMetaSprite->Height);
	Graphics::TBitmap *animBuf = new Graphics::TBitmap;
	animBuf->PixelFormat = pf24bit;



	DoubleBuffered=true;
	ListBoxSprites->DoubleBuffered=true;

 

	UpDownNTSC->Position=metaSpriteDirectFrame_NTSC[metaSpriteActive];
    UpDownPAL->Position=metaSpriteDirectFrame_PAL[metaSpriteActive];


	UpDownDur->Position=iEditStep;


	btnShowOrder->Down			=   prefStartMsprListID;
	btnShowLabels->Down			=	prefStartMsprListLabel;
	btnShowNTSC->Down			=	prefStartMsprListNTSC;
	btnShowPAL->Down			=	prefStartMsprListPAL;
	btnShowCount->Down			=	prefStartMsprListCount;
	btnShowTags->Down			=	prefStartMsprListTag;





	DisplayZoomLevel();
	Update();
	UpdateActive(true);
	if(prefStartShowMM==true) FormManageMetasprites->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::FormShow(TObject *Sender)
{
	 ListBoxSprites->SetFocus();
    DisplayZoomLevel();
	Update();
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::ListBoxSpritesClick(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(true);

	//bool bShift		= (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bool bCtrl		= (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
	//bool bAlt		= (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

	//bool switchMeta = metaSpriteActive=ListBoxSprites->ItemIndex? false:true;
	metaSpriteActive=ListBoxSprites->ItemIndex;

	if(bCtrl) metaSpriteDirectFrame_Cursor=metaSpriteActive;

	FormMain->UpdateMetaSprite(true);

	//Update();
	UpdateActive(true);
	durationNTSC->Text=IntToStr(metaSpriteDirectFrame_NTSC[metaSpriteActive]);
	durationPAL->Text=IntToStr(metaSpriteDirectFrame_PAL[metaSpriteActive]);
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::SpeedButtonInsertClick(TObject *Sender)
{
	int i,id;

	FormMain->SetUndo();
	
	id=ListBoxSprites->ItemIndex;

	for(i=255;i>id;--i)
	{
		if (CheckMoveSprites->Checked) memcpy(&metaSprites[i*64*4],&metaSprites[(i-1)*64*4],64*4);
		if(CheckMoveLabels->Checked){
			metaSpriteNames[i]	= metaSpriteNames[i-1];
		}
	}

	if (CheckMoveSprites->Checked) memset(&metaSprites[id*64*4],255,64*4);
	if(CheckMoveLabels->Checked){
		metaSpriteNames[id] = "unnamed";
        int count = 0;
		AnsiString delimiter = "__";

		for (int i = 0; i < 256; i++) {
			if (i == id) continue;
			AnsiString currentName = metaSpriteNames[i];
			AnsiString targetName = metaSpriteNames[id];
			int pos = currentName.Pos(delimiter);
			if (pos > 0) currentName = currentName.SubString(1, pos-1);  // Ignore delimiter and characters after it
			pos = targetName.Pos(delimiter);
			if (pos > 0) targetName = targetName.SubString(1, pos-1);  // Ignore delimiter and characters after it
			if (currentName == targetName) count++;
		}
		if(count) metaSpriteNames[id]=metaSpriteNames[id]+"__"+IntToStr(count);


	}
	if(CheckMoveTags->Checked){
			metaSpriteDirectStart[id]			= false;
			metaSpriteDirectLoop[id]			= false;
			metaSpriteDirectCall[id]			= false;
			metaSpriteDirectCall2[id]			= false;
			metaSpriteDirectRevert[id]			= false;
			metaSpriteDirectSeconds[id]			= false;
			metaSpriteDirectHalfSeconds[id]		= false;
	}
	if(CheckMoveDurations->Checked){
			 metaSpriteDirectFrame_NTSC[id]		= UpDownNTSC->Position;
			 metaSpriteDirectFrame_PAL[id]		= UpDownPAL->Position;
	}

	if(metaSpriteActive<255) ++metaSpriteActive;

	FormMain->UpdateMetaSprite(true);
	Update();
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::SpeedButtonRemoveClick(TObject *Sender)
{
	int i,id;

	FormMain->SetUndo();

	id=ListBoxSprites->ItemIndex;

	for(i=id;i<255;++i)
	{
		if (CheckMoveSprites->Checked) memcpy(&metaSprites[i*64*4],&metaSprites[(i+1)*64*4],64*4);
		if(CheckMoveLabels->Checked){
			metaSpriteNames[i]	= metaSpriteNames[i+1];
		}
		if(CheckMoveTags->Checked){
			metaSpriteDirectStart[i]			= metaSpriteDirectStart[i+1];
			metaSpriteDirectLoop[i]			= metaSpriteDirectLoop[i+1];
			metaSpriteDirectCall[i]			= metaSpriteDirectCall[i+1];
			metaSpriteDirectCall2[i]			= metaSpriteDirectCall2[i+1];
			metaSpriteDirectRevert[i]			= metaSpriteDirectRevert[i+1];
			metaSpriteDirectSeconds[i]			= metaSpriteDirectSeconds[i+1];
			metaSpriteDirectHalfSeconds[i]		= metaSpriteDirectHalfSeconds[i+1];
		}
		if(CheckMoveDurations->Checked){
			 metaSpriteDirectFrame_NTSC[i]		= metaSpriteDirectFrame_NTSC[i+1];
			 metaSpriteDirectFrame_PAL[i]		= metaSpriteDirectFrame_PAL[i+1];
		}
	}

    if (CheckMoveSprites->Checked)	memset(&metaSprites[255*64*4],255,64*4);

	FormMain->UpdateMetaSprite(true);
	Update();
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::SpeedButtonMoveUpClick(TObject *Sender)
{
	int i,id;
	unsigned char temp[64*4];
	unsigned int iTemp;
	/*
	for reference
	metaSpriteDirectStart[256];
	metaSpriteDirectLoop[256];
	metaSpriteDirectCall[256];
	metaSpriteDirectCall2[256];
	metaSpriteDirectRevert[256];
	metaSpriteDirectSeconds[256];
	metaSpriteDirectHalfSeconds[256];
	*/


	id=ListBoxSprites->ItemIndex;

	if(id<1) return;

	FormMain->SetUndo();
	
	//data
	if (CheckMoveSprites->Checked){
		memcpy(&temp,&metaSprites[(id-1)*64*4],64*4);
		memcpy(&metaSprites[(id-1)*64*4],&metaSprites[id*64*4],64*4);
		memcpy(&metaSprites[id*64*4],&temp,64*4);
    }
	//name
	if(CheckMoveLabels->Checked){
		tmpMetaSprName 			= metaSpriteNames[id-1];
		metaSpriteNames[id-1]   = metaSpriteNames[id];
		metaSpriteNames[id]		= tmpMetaSprName;
    }
	if(CheckMoveTags->Checked){
		iTemp					   	  		= metaSpriteDirectStart[id-1];
		metaSpriteDirectStart[id-1]	  		= metaSpriteDirectStart[id];
		metaSpriteDirectStart[id] 	  		= iTemp;

		iTemp						  		= metaSpriteDirectLoop[id-1];
		metaSpriteDirectLoop[id-1]	  		= metaSpriteDirectLoop[id];
		metaSpriteDirectLoop[id] 	  		= iTemp;

		iTemp								= metaSpriteDirectCall[id-1];
		metaSpriteDirectCall[id-1]	  		= metaSpriteDirectCall[id];
		metaSpriteDirectCall[id] 	  		= iTemp;

		iTemp								= metaSpriteDirectCall2[id-1];
		metaSpriteDirectCall2[id-1]	  		= metaSpriteDirectCall2[id];
		metaSpriteDirectCall2[id] 	  		= iTemp;

		iTemp						  		= metaSpriteDirectRevert[id-1];
		metaSpriteDirectRevert[id-1]  		= metaSpriteDirectRevert[id];
		metaSpriteDirectRevert[id] 	  		= iTemp;

		iTemp						  		= metaSpriteDirectSeconds[id-1];
		metaSpriteDirectSeconds[id-1] 		= metaSpriteDirectSeconds[id];
		metaSpriteDirectSeconds[id]   		= iTemp;

		iTemp						  		= metaSpriteDirectHalfSeconds[id-1];
		metaSpriteDirectHalfSeconds[id-1]	= metaSpriteDirectHalfSeconds[id];
		metaSpriteDirectHalfSeconds[id] 	= iTemp;
	}
	if(CheckMoveDurations->Checked){
		iTemp								=  metaSpriteDirectFrame_NTSC[id-1];
		metaSpriteDirectFrame_NTSC[id-1]	=  metaSpriteDirectFrame_NTSC[id];
		metaSpriteDirectFrame_NTSC[id]		=  iTemp;

		iTemp								=  metaSpriteDirectFrame_PAL[id-1];
		metaSpriteDirectFrame_PAL[id-1]	=  metaSpriteDirectFrame_PAL[id];
		metaSpriteDirectFrame_PAL[id]		=  iTemp;
	}


	UpdateActive(true);
	--metaSpriteActive;

	FormMain->UpdateMetaSprite(true);
	UpdateActive(true);
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::SpeedButtonMoveDownClick(
TObject *Sender)
{
	int i,id;
	unsigned char temp[64*4];
    unsigned int iTemp;
	id=ListBoxSprites->ItemIndex;

	if(id>=255) return;

	FormMain->SetUndo();
	
	if (CheckMoveSprites->Checked){
		memcpy(&temp,&metaSprites[(id+1)*64*4],64*4);
		memcpy(&metaSprites[(id+1)*64*4],&metaSprites[id*64*4],64*4);
		memcpy(&metaSprites[id*64*4],&temp,64*4);
    }
	//name
	if(CheckMoveLabels->Checked){
		tmpMetaSprName 			= metaSpriteNames[id+1];
		metaSpriteNames[id+1]   = metaSpriteNames[id];
		metaSpriteNames[id]		= tmpMetaSprName;
	}
    if(CheckMoveTags->Checked){
		iTemp					   	  		= metaSpriteDirectStart[id+1];
		metaSpriteDirectStart[id+1]	  		= metaSpriteDirectStart[id];
		metaSpriteDirectStart[id] 	  		= iTemp;

		iTemp						  		= metaSpriteDirectLoop[id+1];
		metaSpriteDirectLoop[id+1]	  		= metaSpriteDirectLoop[id];
		metaSpriteDirectLoop[id] 	  		= iTemp;

		iTemp								= metaSpriteDirectCall[id+1];
		metaSpriteDirectCall[id+1]	  		= metaSpriteDirectCall[id];
		metaSpriteDirectCall[id] 	  		= iTemp;

		iTemp								= metaSpriteDirectCall2[id+1];
		metaSpriteDirectCall2[id+1]	  		= metaSpriteDirectCall2[id];
		metaSpriteDirectCall2[id] 	  		= iTemp;

		iTemp						  		= metaSpriteDirectRevert[id+1];
		metaSpriteDirectRevert[id+1]  		= metaSpriteDirectRevert[id];
		metaSpriteDirectRevert[id] 	  		= iTemp;

		iTemp						  		= metaSpriteDirectSeconds[id+1];
		metaSpriteDirectSeconds[id+1] 		= metaSpriteDirectSeconds[id];
		metaSpriteDirectSeconds[id]   		= iTemp;

		iTemp						  		= metaSpriteDirectHalfSeconds[id+1];
		metaSpriteDirectHalfSeconds[id+1]	= metaSpriteDirectHalfSeconds[id];
		metaSpriteDirectHalfSeconds[id] 	= iTemp;
	}
    if(CheckMoveDurations->Checked){
		iTemp								=  metaSpriteDirectFrame_NTSC[id+1];
		metaSpriteDirectFrame_NTSC[id+1]	=  metaSpriteDirectFrame_NTSC[id];
		metaSpriteDirectFrame_NTSC[id]		=  iTemp;

		iTemp								=  metaSpriteDirectFrame_PAL[id+1];
		metaSpriteDirectFrame_PAL[id+1]	=  metaSpriteDirectFrame_PAL[id];
		metaSpriteDirectFrame_PAL[id]		=  iTemp;
	}

	UpdateActive(true);
	++metaSpriteActive;

	FormMain->UpdateMetaSprite(true);
	UpdateActive(true);
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::SpeedButtonDuplicateClick(
      TObject *Sender)
{
	int i,id;

	FormMain->SetUndo();
	
	id=ListBoxSprites->ItemIndex;

	for(i=255;i>id;--i)
	{
		if (CheckMoveSprites->Checked) memcpy(&metaSprites[i*64*4],&metaSprites[(i-1)*64*4],64*4);
		if(CheckMoveLabels->Checked){
			metaSpriteNames[i]	= metaSpriteNames[i-1];

		}
		if(CheckMoveTags->Checked){
			metaSpriteDirectStart[i]		= metaSpriteDirectStart[i-1];
			metaSpriteDirectLoop[i]         = metaSpriteDirectLoop[i-1];
			metaSpriteDirectCall[i]         = metaSpriteDirectCall[i-1];
			metaSpriteDirectCall2[i]        = metaSpriteDirectCall2[i-1];
			metaSpriteDirectRevert[i]       = metaSpriteDirectRevert[i-1];
			metaSpriteDirectSeconds[i]      = metaSpriteDirectSeconds[i-1];
			metaSpriteDirectHalfSeconds[i]	= metaSpriteDirectHalfSeconds[i-1];
		}
		if(CheckMoveDurations->Checked){
		   metaSpriteDirectFrame_NTSC[i]	= metaSpriteDirectFrame_NTSC[i-1];
		   metaSpriteDirectFrame_PAL[i]		= metaSpriteDirectFrame_PAL[i-1];
		}
	}


	int count = 0;
	AnsiString delimiter = "__";
	if(CheckMoveLabels->Checked){
		for (int i = 0; i < 256; i++) {
			if (i == id+1) continue;
			AnsiString currentName = metaSpriteNames[i];
			AnsiString targetName = metaSpriteNames[id+1];
			int pos = currentName.Pos(delimiter);
			if (pos > 0) currentName = currentName.SubString(1, pos-1);  // Ignore delimiter and characters after it
			pos = targetName.Pos(delimiter);
			if (pos > 0) targetName = targetName.SubString(1, pos-1);  // Ignore delimiter and characters after it
			if (currentName == targetName) count++;
		}
		metaSpriteNames[id+1]=metaSpriteNames[id+1]+"__"+IntToStr(count);

	}
	if(CheckMoveTags->Checked){
		metaSpriteDirectStart[id+1]			= metaSpriteDirectStart[id];
		metaSpriteDirectLoop[id+1]			= metaSpriteDirectLoop[id];
		metaSpriteDirectCall[id+1]			= metaSpriteDirectCall[id];
		metaSpriteDirectCall2[id+1]			= metaSpriteDirectCall2[id];
		metaSpriteDirectRevert[id+1]		= metaSpriteDirectRevert[id];
		metaSpriteDirectSeconds[id+1]		= metaSpriteDirectSeconds[id];
		metaSpriteDirectHalfSeconds[id+1]	= metaSpriteDirectHalfSeconds[id];
	}
	if(CheckMoveDurations->Checked){
		 metaSpriteDirectFrame_NTSC[id+1]	= metaSpriteDirectFrame_NTSC[id];
		 metaSpriteDirectFrame_PAL[id+1]		= metaSpriteDirectFrame_PAL[id];
	}

	if(metaSpriteActive<255) ++metaSpriteActive;

	FormMain->UpdateMetaSprite(true);
	Update();
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButtonCopyClick(TObject *Sender)
{
	FormMain->SpeedButtonMetaSpriteCopyClick(Sender);	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButtonPasteClick(TObject *Sender)
{
	FormMain->SpeedButtonMetaSpritePasteClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::FormKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	if(msprList_isDragging)
	{
		   if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) FormManageMetasprites->Caption="Move";
		   else if(Shift.Contains(ssCtrl) && Shift.Contains(ssAlt)) FormManageMetasprites->Caption="Clone";
		   else FormManageMetasprites->Caption="Swap";
           ListBoxSprites->Invalidate();
	}
	if(!Shift.Contains(ssCtrl))
	{
		bool b;
		if(Key==VK_F1) {FormMain->PageControlEditor->ActivePageIndex=0; b=true;}
		if(Key==VK_F2) {FormMain->PageControlEditor->ActivePageIndex=1; b=true;}
		if(Key==VK_F3) {FormMain->PageControlEditor->ActivePageIndex=2; b=true;}
		if (b) {FormMain->PageControlEditorChange(Sender);

		}
		if(Key==VK_ESCAPE) FormManageMetasprites->Close();

		if(Key==VK_OEM_4||Key==VK_OEM_COMMA) FormMain->SpeedButtonPrevMetaSpriteClick(Sender);// [
		if(Key==VK_OEM_6||Key==VK_OEM_PERIOD) FormMain->SpeedButtonNextMetaSpriteClick(Sender);// ]
	}
}
//---------------------------------------------------------------------------



void __fastcall TFormManageMetasprites::FormActivate(TObject *Sender)
{
  
	ListBoxSprites->SetFocus();
    DisplayZoomLevel();
	FormManageMetasprites->ScreenSnap=bSnapToScreen;
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::MetaLabelKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    FormMain->EnableDisableTypeConflictShortcuts(true);
	bMetaManagerEditLabelActive=true;
	if(Key==VK_RETURN)
	{
		FormMain->SetUndo();
		bMetaManagerEditLabelActive=false;
		metaSpriteNames[metaSpriteActive]=MetaLabel->Text;
		FormMain->UpdateMetaSpriteLabel();
		UpdateActive(false);
		//ListBoxSprites->Focused();
		//FormMain->EnableDisableTypeConflictShortcuts(false);
	}
	if (Key==VK_ESCAPE){
		//Key=0;
		MetaLabel->Text=metaSpriteNames[metaSpriteActive];
        bMetaManagerEditLabelActive=false;
		//ListBoxSprites->Focused();
		//FormMain->EnableDisableTypeConflictShortcuts(false);
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::MetaLabelClick(TObject *Sender)
{
	MetaLabel->Focused();
	FormMain->EnableDisableTypeConflictShortcuts(true);
	bMetaManagerEditLabelActive=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::MetaLabelEnter(TObject *Sender)
{
   FormMain->EnableDisableTypeConflictShortcuts(true);
   bMetaManagerEditLabelActive=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::MetaLabelExit(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(false);

	if(bMetaManagerEditLabelActive){
		FormMain->SetUndo();
		metaSpriteNames[metaSpriteActive]=MetaLabel->Text;
		FormMain->UpdateMetaSpriteLabel();

	}

	bMetaManagerEditLabelActive=false;
	FormMain->UpdateMetaSpriteLabel();
	FormManageMetasprites->SetFocus();

	UpdateActive(false);

}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::FormDeactivate(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(false);
	bMetaManagerEditLabelActive=false;
	msprList_isDragging=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::FormClose(TObject *Sender,
	  TCloseAction &Action)
{
    SpeedButton1->Caption="Play";
	TimerFrameTick->Enabled=false;

	FormMain->EnableDisableTypeConflictShortcuts(false);
	bMetaManagerEditLabelActive=false;
		
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesDblClick(TObject *Sender)
{
		FormMain->PageControlEditor->ActivePageIndex=1;
		FormMain->PageControlEditorChange(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::MetaLabelToClip(bool cut)
{
	if (OpenClipboard(Handle)) {
		EmptyClipboard();
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (MetaLabel->Text.Length() + 1) * sizeof(char));
		if (hGlobal != NULL) {
			char* pClipboardText = (char*)GlobalLock(hGlobal);

			if (pClipboardText != NULL) {
				strcpy(pClipboardText, MetaLabel->Text.c_str());
				GlobalUnlock(hGlobal);
				SetClipboardData(CF_TEXT, hGlobal);
			}
		}
		CloseClipboard();
	}
	if(cut) MetaLabel->Text="";
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ClipToMetaLabel(void) {
	int len=64;

	if (OpenClipboard(0)) {
		HGLOBAL hData = GetClipboardData(CF_TEXT);
		if (hData != NULL) {
			char* clipboardText = static_cast<char*>(GlobalLock(hData));
			if (clipboardText != NULL) {

				GlobalUnlock(hData);
				AnsiString str = clipboardText;
				if(str.Length() > len) {
					str = str.SubString(0, len);
				}
				MetaLabel->Text = str;
			}
		}
		CloseClipboard();
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::SpeedButton1Click(TObject *Sender)
{
	bool isNTSC = PlayNTSC->Checked;

	if(SpeedButton1->Caption=="Play"){
		SpeedButton1->Caption="Pause";
		if(RadioSteady->Checked) iMsprTimeDelay= isNTSC? 60:50;
		else{
			if(isNTSC) iMsprTimeDelay=  metaSpriteDirectFrame_NTSC[metaSpriteDirectFrame_Cursor];
			else	   iMsprTimeDelay=	metaSpriteDirectFrame_PAL[metaSpriteDirectFrame_Cursor];
		}
		if		(iMsprTimeDelay>0) 	 btnRelease->Enabled=false;
		else if (RadioHold->Checked) btnRelease->Enabled=true;

		bReversePlayback= metaSpriteDirectRevert[metaSpriteDirectFrame_Cursor];
		//buffer pictures
		FormManageMetasprites->Caption="Buffering...";

		//FormMain->DrawListedMetaSprite(imageListMetaSprites,0,256,1,btnGrid->Down,btnBox->Down,!btnBox->Down, btnSil->Down, btnWarn->Down);
		FormMain->DrawListedMetasrpiteParallel(btnGrid->Down,btnBox->Down,btnSil->Down,btnWarn->Down);

		FormManageMetasprites->Caption=formTitle;

		TimerFrameTick->Enabled=true;
	}
	else if(SpeedButton1->Caption=="Pause"){
		SpeedButton1->Caption="Play";
		TimerFrameTick->Enabled=false;
	}
}
//---------------------------------------------------------------------------



void __fastcall TFormManageMetasprites::RadioStartClick(TObject *Sender)
{
   metaSpriteDirectStart[metaSpriteActive]=true;
   metaSpriteDirectLoop[metaSpriteActive]=false;
   metaSpriteDirectCall[metaSpriteActive]=false;
   UpdateActive(false);
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::RadioCallClick(TObject *Sender)
{
	metaSpriteDirectStart[metaSpriteActive]=false;
	metaSpriteDirectLoop[metaSpriteActive]=false;
	metaSpriteDirectCall[metaSpriteActive]=true;
	UpdateActive(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::RadioLoopClick(TObject *Sender)
{
	metaSpriteDirectStart[metaSpriteActive]=false;
	metaSpriteDirectLoop[metaSpriteActive]=true;
	metaSpriteDirectCall[metaSpriteActive]=false;
	UpdateActive(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::RadioNoneClick(TObject *Sender)
{
   metaSpriteDirectStart[metaSpriteActive]=false;
   metaSpriteDirectLoop[metaSpriteActive]=false;
   metaSpriteDirectCall[metaSpriteActive]=false;
   UpdateActive(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::btnShowLabelsClick(TObject *Sender)
{
   Update();
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::btnGridClick(TObject *Sender)
{
   //UpdateActive(true);
   //
   //FormMain->DrawListedMetaSprite(imageListMetaSprites,0,256,1,btnGrid->Down,btnBox->Down,!btnBox->Down, btnSil->Down, btnWarn->Down);
   FormMain->DrawListedMetaSprite(imageListMetaSprites,metaSpriteActive,1,1,btnGrid->Down,btnBox->Down,!btnBox->Down, btnSil->Down, btnWarn->Down);
   FormMain->DrawListedMetaSprite(imageListMetaSprites,metaSpriteDirectFrame_Cursor,1,1,btnGrid->Down,btnBox->Down,!btnBox->Down, btnSil->Down, btnWarn->Down);

   AssignFrame(true);
   /*
   imageListMetaSprites->GetBitmap(metaSpriteDirectFrame_Cursor, animBuf);

   if(btnZoom->Down) ImageMetaSprite->Canvas->StretchDraw(zoomDstRect, animBuf);
   else ImageMetaSprite->Picture->Bitmap->Assign(animBuf);
   */
   //FormMain->DrawListedMetasrpiteParallel(btnGrid->Down,btnBox->Down,btnSil->Down,btnWarn->Down);
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::btnShowLabelsMouseLeave(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="---";
    FormManageMetasprites->Caption=formTitle;
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButton3MouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Previous frame.\n[ctrl+click] ignores loops.\nStops playback when pressed.";
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButton4MouseEnter(
	  TObject *Sender)
{
   FormMain->LabelStats->Caption="Next frame.\n[ctrl+click] ignores loops.\nStops playback when pressed.";
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButton5MouseEnter(
      TObject *Sender)
{
   FormMain->LabelStats->Caption="Next animation.";		
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButton2MouseEnter(
      TObject *Sender)
{
  FormMain->LabelStats->Caption="Start of animation / Previous animation.";			
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButton1MouseEnter(
      TObject *Sender)
{
  FormMain->LabelStats->Caption="Play/pause.\n Ctrl-click plays from start.";		
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::btnShowLabelsMouseEnter(
      TObject *Sender)
{
  FormMain->LabelStats->Caption="Display metasprite labels in list.";		
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::btnShowNTSCMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Display NTSC duration in list.\n An ntsc tick lasts ~16.67 ms, which nexxt rounds to 17 ms.";			
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::btnShowPALMouseEnter(
	  TObject *Sender)
{
	FormMain->LabelStats->Caption="Display PAL duration in list.\n A pal tick lasts 20 ms.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::btnShowCountMouseEnter(
      TObject *Sender)
{
   FormMain->LabelStats->Caption="Display sprite object count per metasprite.";		
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::btnShowTagsMouseEnter(
	  TObject *Sender)
{
   FormMain->LabelStats->Caption="Display animation tags.";
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::RadioNoneMouseEnter(
      TObject *Sender)
{
   FormMain->LabelStats->Caption="No tag. Metasprite is treated normally in an animation.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::RadioStartMouseEnter(
      TObject *Sender)
{
FormMain->LabelStats->Caption="Start tag.\nCan be used as a method to search for the beginning of an animation.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButton7Click(TObject *Sender)
{
	FormMetaspritePlaybackRules->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesDrawItem(
	  TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{
	TListBox *listBox = dynamic_cast<TListBox*>(Control);

	bool isSelected = (State.Contains(odSelected));
	int cur = metaSpriteDirectFrame_Cursor;
    bool bCtrl		= (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
	bool bAlt		= (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
	TColor tmpCol;

	TColor selected 	= TColor(0x888855);
	TColor cursor 		= TColor(0xffeeaa);
	TColor both 		= TColor(0xC4BB88);

	TColor dragDest		= TColor(0xff8866);//(0xBB88C4);
	TColor cloneDest	= TColor(0x7777ff);//(0xBBC488);
	TColor swapDest		= TColor(0x77ff77);//(0xC48888);

	TColor dragSrc		= TColor(0x999999);

	TColor odd 	  		= TColor(clCream);
	TColor even 	  	= TColor(0xeeeeee);

	if (msprList_isDragging && Index == msprList_dragID)
							tmpCol = dragSrc;
	else if (isSelected && Index == cur)
								tmpCol = both;

	else if (isSelected)		tmpCol = selected;
	else if (msprList_isDragging && Index == msprList_dropID)
	{
		if(bCtrl && !bAlt) tmpCol = dragDest;
		else if(bAlt)	   tmpCol = cloneDest;
		else			   tmpCol = swapDest;
	}

	else if(Index == cur) 		tmpCol = cursor;
	else if(Index%2 != 0) 		tmpCol = odd;
	else  						tmpCol = even;
	if (listBox)
	{
		listBox->Canvas->Brush->Color = tmpCol;

		// draw the background
		listBox->Canvas->FillRect(Rect);

		// make text white if selected
		if(tmpCol == selected || tmpCol == dragSrc)
				listBox->Canvas->Font->Color = clWhite;
		else    listBox->Canvas->Font->Color = clBlack;

		//draw the text of the item
		listBox->Canvas->TextOut(Rect.Left + 2, Rect.Top + 0, listBox->Items->Strings[Index]);

		
		
	}	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::TimerFrameTickTimer(
	  TObject *Sender)
{

    if(btnRelease->Enabled) return;
	iMsprTimeDelay--;	

	//FormManageMetasprites->Caption=iMsprTimeDelay;
	bool useStart = FormMetaspritePlaybackRules->RadioButton1->Checked;
	bool isNTSC = PlayNTSC->Checked;

	//bool bUseCall  		=  RadioCall->Caption=="call";

	//bool bUseCall2 		=  RadioStart->Caption=="call2";


	bool bUseRevert		=  (RadioStart->Caption=="revert" || RadioCall->Caption=="revert");
	bool bUseSeconds	=  (RadioStart->Caption=="seconds" || RadioCall->Caption=="seconds");
	bool bUseHalfSeconds=  (RadioStart->Caption=="h. secs" || RadioCall->Caption=="h. secs");


	//init and set time factor and sytem rate in ms.
	int f=1.0;
	if(RadioX2->Checked) f=2;
	int r = isNTSC? 17:20;            //approximation of ~16.67

	int direction = bReversePlayback? -1:1;


	//calc rate in use.
	r=r*f;

	TimerFrameTick->Interval=r;
	
	if(iMsprTimeDelay<=0){

	//apply loopback rules
	if(metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor]){
			while (1) {
				if (metaSpriteDirectFrame_Cursor>0) metaSpriteDirectFrame_Cursor--;
				if (metaSpriteDirectFrame_Cursor == 0) break;
				else if ( useStart &&  metaSpriteDirectStart[metaSpriteDirectFrame_Cursor]) break;
				else if ( !useStart && metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor])  {metaSpriteDirectFrame_Cursor++; break;}
				else continue;
		}
	}
	else if(bUseRevert && metaSpriteDirectRevert[metaSpriteDirectFrame_Cursor]){
		if(direction>0) direction= -1;
        bReversePlayback=true;
		metaSpriteDirectFrame_Cursor=(metaSpriteDirectFrame_Cursor+direction)%256;

	}

	else {
		if(bReversePlayback){
			if(metaSpriteDirectFrame_Cursor==0) bReversePlayback=false;
			else if (( useStart && metaSpriteDirectStart[metaSpriteDirectFrame_Cursor])
					|| (bUseRevert && metaSpriteDirectRevert[metaSpriteDirectFrame_Cursor-1])
					|| metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor-1])
			{
				bReversePlayback=false;
			}
			}
			direction = bReversePlayback? -1:1;
			metaSpriteDirectFrame_Cursor=(metaSpriteDirectFrame_Cursor+direction)%256;

	}


	//set new time
		if(RadioSteady->Checked) iMsprTimeDelay= isNTSC? 30:25;
		else{
			if(isNTSC) iMsprTimeDelay=  metaSpriteDirectFrame_NTSC[metaSpriteDirectFrame_Cursor];
			else	   iMsprTimeDelay=	metaSpriteDirectFrame_PAL[metaSpriteDirectFrame_Cursor];
        }

		//Duration caps
		if(FormMetaspritePlaybackRules->Radio63Cap->Checked && iMsprTimeDelay>63) iMsprTimeDelay=63;
		if(FormMetaspritePlaybackRules->Radio127Cap->Checked && iMsprTimeDelay>127) iMsprTimeDelay=127;
		if(FormMetaspritePlaybackRules->Radio255Cap->Checked && iMsprTimeDelay>255) iMsprTimeDelay=255;

		//Duration multipliers:
		if((bUseSeconds && metaSpriteDirectSeconds[metaSpriteDirectFrame_Cursor])
			 || (bUseHalfSeconds && metaSpriteDirectHalfSeconds[metaSpriteDirectFrame_Cursor]))
			 {
				int amt = isNTSC? 30:25;
				if (bUseSeconds && metaSpriteDirectSeconds[metaSpriteDirectFrame_Cursor]) amt *=2;
				iMsprTimeDelay  *=amt;
		}


		//If new time is 0, apply 0 flag rules
		if(RadioHold->Checked){
			if		(iMsprTimeDelay>0) 	 btnRelease->Enabled=false;
			else 						 btnRelease->Enabled=true;
		}
		if(RadioSkip->Checked){
			if		(iMsprTimeDelay==0){
					//skip ahead (or loop)
					if(metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor]){
						while (1) {
							if (metaSpriteDirectFrame_Cursor>0) metaSpriteDirectFrame_Cursor--;
							if (metaSpriteDirectFrame_Cursor == 0) break;
							else if ( useStart &&  metaSpriteDirectStart[metaSpriteDirectFrame_Cursor]) break;
							else if ( !useStart && metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor])  {metaSpriteDirectFrame_Cursor++; break;}
							else continue;
						}
					}
					else metaSpriteDirectFrame_Cursor=(metaSpriteDirectFrame_Cursor+1)%256;

				if(RadioSteady->Checked) iMsprTimeDelay= isNTSC? 30:25;
				else{
					if(isNTSC) iMsprTimeDelay=  metaSpriteDirectFrame_NTSC[metaSpriteDirectFrame_Cursor];
					else	   iMsprTimeDelay=	metaSpriteDirectFrame_PAL[metaSpriteDirectFrame_Cursor];
				}
			}
		}

	//refresh
	ListBoxSprites->Invalidate();
	FormMain->DrawListedMetaSprite(imageListMetaSprites,metaSpriteDirectFrame_Cursor,1,1,btnGrid->Down,btnBox->Down,!btnBox->Down, btnSil->Down, btnWarn->Down);

	AssignFrame(true);


	}
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButton3Click(TObject *Sender)
{
	SpeedButton1->Caption="Play";
	TimerFrameTick->Enabled=false;	

	bool bCtrl		= (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

	metaSpriteDirectFrame_Cursor--;
	
	if(!bCtrl){
		if(metaSpriteDirectStart[metaSpriteDirectFrame_Cursor+1] ||  metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor])
		{
			while(1){
				if (metaSpriteDirectFrame_Cursor<255) metaSpriteDirectFrame_Cursor++;
				if (metaSpriteDirectFrame_Cursor == 255) break;
				else if (  metaSpriteDirectStart[metaSpriteDirectFrame_Cursor]) break;
				else if (  metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor])  {metaSpriteDirectFrame_Cursor++; break;}
				else continue;
		  }
		  metaSpriteDirectFrame_Cursor--;
		}
	}
	if(metaSpriteDirectFrame_Cursor<0) metaSpriteDirectFrame_Cursor=0;
    if(metaSpriteDirectFrame_Cursor>255) metaSpriteDirectFrame_Cursor=255;
	//visuals
	ListBoxSprites->Invalidate();

	AssignFrame(true);
	/*
	imageListMetaSprites->GetBitmap(metaSpriteDirectFrame_Cursor, animBuf);
	if(btnZoom->Down) ImageMetaSprite->Canvas->StretchDraw(zoomDstRect, animBuf);
   else ImageMetaSprite->Picture->Bitmap->Assign(animBuf);

	*/
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButton4Click(TObject *Sender)
{
	SpeedButton1->Caption="Play";
	TimerFrameTick->Enabled=false;	

	bool bCtrl		= (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;



	if(!bCtrl){
		if(metaSpriteDirectStart[metaSpriteDirectFrame_Cursor] ||  metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor])
		{
			while(1){
			if (metaSpriteDirectFrame_Cursor>0) metaSpriteDirectFrame_Cursor--;
			if (metaSpriteDirectFrame_Cursor == 0) break;
			else if (  metaSpriteDirectStart[metaSpriteDirectFrame_Cursor]) break;
			else if (  metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor])  {metaSpriteDirectFrame_Cursor++; break;}
			else continue;
		  }
		  metaSpriteDirectFrame_Cursor--;
		}
	}
	metaSpriteDirectFrame_Cursor++;
	if(metaSpriteDirectFrame_Cursor<0) metaSpriteDirectFrame_Cursor=0;
    if(metaSpriteDirectFrame_Cursor>255) metaSpriteDirectFrame_Cursor=255;
	//visuals
	ListBoxSprites->Invalidate();
	AssignFrame(true);
	/*
	imageListMetaSprites->GetBitmap(metaSpriteDirectFrame_Cursor, animBuf);
	if(btnZoom->Down) ImageMetaSprite->Canvas->StretchDraw(zoomDstRect, animBuf);
   else ImageMetaSprite->Picture->Bitmap->Assign(animBuf);
	*/
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Select metasprite to edit.\n Additionally, Ctrl-click sets the playback cursor.";
	if(msprList_isDragging) {
		Screen->Cursor = crDrag;
		bool bCtrl		= (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
		bool bAlt		= (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
		if(bCtrl && !bAlt) FormManageMetasprites->Caption="Move";
		   else if(bCtrl && bAlt) FormManageMetasprites->Caption="Clone";
		   else FormManageMetasprites->Caption="Swap";
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationNTSCKeyPress(
      TObject *Sender, char &Key)
{
	if(!((Key>='0'&&Key<='9')||Key==VK_BACK||Key==VK_DELETE)) Key=0;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationPALKeyPress(
      TObject *Sender, char &Key)
{
	if(!((Key>='0'&&Key<='9')||Key==VK_BACK||Key==VK_DELETE)) Key=0;	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationPALClick(TObject *Sender)
{
	((TEdit*)Sender)->SelectAll();	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationNTSCClick(TObject *Sender)
{
	((TEdit*)Sender)->SelectAll();	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationNTSCExit(TObject *Sender)
{
	int n;

	if(!TryStrToInt(durationNTSC->Text,n)) n=0;

	if(n<0) n=0;

	if(n>16384) n=16384;
	metaSpriteDirectFrame_NTSC[metaSpriteActive]=n;
	durationNTSC->Text=IntToStr(n);	

	FormMain->EnableDisableTypeConflictShortcuts(false);
    UpdateActive(false);
	ListBoxSprites->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationPALExit(TObject *Sender)
{
	int n;

	if(!TryStrToInt(durationPAL->Text,n)) n=0;

	if(n<0) n=0;

	if(n>16384) n=16384;
	metaSpriteDirectFrame_PAL[metaSpriteActive]=n;	
	durationPAL->Text=IntToStr(n);	
	FormMain->EnableDisableTypeConflictShortcuts(false);
	UpdateActive(false);
	ListBoxSprites->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationNTSCEnter(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(true);	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationPALEnter(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(true);	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesKeyPress(
	  TObject *Sender, char &Key)
{
	bool isDuration=false;
	bool isTags=false;


	bool bCall=false;
	bool bCall2=false;
	bool bStart=false;
	bool bRevert=false;
	bool bSeconds=false;
	bool bHalfSeconds=false;

	if (isdigit(Key) || isalpha(Key))
	{
		bool isNTSC = PlayNTSC->Checked;
		isDuration=true;
		int n;
		if (isdigit(Key)) n = Key - '0';  // This converts '0'-'9' to 0-9
		else if (Key=='q') {n=10; }
		else if (Key=='w') {n=11; }
		else if (Key=='e') {n=12; }
		else if (Key=='r') {n=13; }
		else if (Key=='t') {n=14; }
		else if (Key=='y') {n=15; }
		else if (Key=='u') {n=16; }
		else if (Key=='i') {n=17; }
		else if (Key=='o') {n=18; }
		else if (Key=='p') {n=19; }
		else if (Key=='a') {n=20; }
		else if (Key=='s') {n=30; }
		else if (Key=='d') {n=40; }
		else if (Key=='f') {n=50; }
		else if (Key=='g') {n=60; }

		else goto skip;
		FormMain->SetUndo();
		if(isNTSC) 	{
			metaSpriteDirectFrame_NTSC[metaSpriteActive]=n;
			durationNTSC->Text=IntToStr(n);	 
		}
		else {
			metaSpriteDirectFrame_PAL[metaSpriteActive]=n;	
			durationPAL->Text=IntToStr(n);	 
		}

		if(chkLink->Checked){
			if(!isNTSC) 	{
				metaSpriteDirectFrame_NTSC[metaSpriteActive]=n;
				durationNTSC->Text=IntToStr(n);
				if(chkAdjust->Checked){
				// metaSpriteDirectFrame_NTSC[metaSpriteActive]+=(metaSpriteDirectFrame_PAL[metaSpriteActive]/5);
					float adjusted_PAL = metaSpriteDirectFrame_PAL[metaSpriteActive] * 0.2f;
					float closest_diff = 99999;

					for (unsigned int tmp = 0; tmp <= metaSpriteDirectFrame_PAL[metaSpriteActive]; ++tmp) {
						float adjusted_NTSC = tmp * 0.1667;
						float diff = fabs(adjusted_PAL - adjusted_NTSC);

						if (diff < closest_diff) {
							closest_diff = diff;
							metaSpriteDirectFrame_NTSC[metaSpriteActive] = tmp;
						}
					}
				durationNTSC->Text=IntToStr(metaSpriteDirectFrame_NTSC[metaSpriteActive]);
				}

			}
			else {
				metaSpriteDirectFrame_PAL[metaSpriteActive]=n;
				durationPAL->Text=IntToStr(n);
				if(chkAdjust->Checked){
					//metaSpriteDirectFrame_PAL[metaSpriteActive]-=(metaSpriteDirectFrame_NTSC[metaSpriteActive]/6);
					float adjusted_NTSC = metaSpriteDirectFrame_NTSC[metaSpriteActive] * 0.1667f;

					float closest_diff = 99999;

					for (unsigned int tmp = 0; tmp <= metaSpriteDirectFrame_NTSC[metaSpriteActive]; ++tmp) {
						float adjusted_PAL = tmp * 0.20;
						float diff = fabs(adjusted_NTSC - adjusted_PAL);

						if (diff < closest_diff) {
							closest_diff = diff;
							metaSpriteDirectFrame_PAL[metaSpriteActive] = tmp;
						}
					}

				durationPAL->Text=IntToStr(metaSpriteDirectFrame_PAL[metaSpriteActive]);
				}
			}
	  }

	}
	skip:
	



	if (Key=='l') { 
		isTags=true;
		isDuration=false;
		FormMain->SetUndo();
		if(RadioLoop->Checked){
			RadioNone->Checked=true;


			if(RadioStart->Caption=="start") bStart=true;
			 else if (RadioStart->Caption=="call2") bCall2=true;
			 else if (RadioStart->Caption=="revert") bRevert=true;
			 else if (RadioStart->Caption=="seconds") bSeconds=true;
			 else if (RadioStart->Caption=="h. secs") bHalfSeconds=true;

			if(RadioCall->Caption=="call") bCall=true;
			else if (RadioCall->Caption=="revert") bRevert=true;
			else if (RadioCall->Caption=="seconds") bSeconds=true;
			else if (RadioCall->Caption=="h. secs") bHalfSeconds=true;

								metaSpriteDirectLoop[metaSpriteActive]=false;
			if(bCall) 			metaSpriteDirectCall[metaSpriteActive]=false;
			if(bCall2)          metaSpriteDirectCall2[metaSpriteActive]=false;
			if(bStart) 			metaSpriteDirectStart[metaSpriteActive]=false;
			if(bRevert) 		metaSpriteDirectRevert[metaSpriteActive]=false;
			if(bSeconds) 		metaSpriteDirectSeconds[metaSpriteActive]=false;
			if(bHalfSeconds) 	metaSpriteDirectHalfSeconds[metaSpriteActive]=false;

		}
		else{
			RadioLoop->Checked=true;
			if(RadioStart->Caption=="start") bStart=true;
			 else if (RadioStart->Caption=="call2") bCall2=true;
			 else if (RadioStart->Caption=="revert") bRevert=true;
			 else if (RadioStart->Caption=="seconds") bSeconds=true;
			 else if (RadioStart->Caption=="h. secs") bHalfSeconds=true;

			if(RadioCall->Caption=="call") bCall=true;
			else if (RadioCall->Caption=="revert") bRevert=true;
			else if (RadioCall->Caption=="seconds") bSeconds=true;
			else if (RadioCall->Caption=="h. secs") bHalfSeconds=true;
								metaSpriteDirectLoop[metaSpriteActive]=true;
			if(bCall) 			metaSpriteDirectCall[metaSpriteActive]=false;
			if(bCall2)          metaSpriteDirectCall2[metaSpriteActive]=false;
			if(bStart) 			metaSpriteDirectStart[metaSpriteActive]=false;
			if(bRevert) 		metaSpriteDirectRevert[metaSpriteActive]=false;
			if(bSeconds) 		metaSpriteDirectSeconds[metaSpriteActive]=false;
			if(bHalfSeconds) 	metaSpriteDirectHalfSeconds[metaSpriteActive]=false;
		}
	}

	if (Key=='m') {
		isTags=true;
		isDuration=false;
		FormMain->SetUndo();
		if(RadioStart->Checked){
			RadioNone->Checked=true;
			if(RadioStart->Caption=="start") bStart=true;
			 else if (RadioStart->Caption=="call2") bCall2=true;
			 else if (RadioStart->Caption=="revert") bRevert=true;
			 else if (RadioStart->Caption=="seconds") bSeconds=true;
			 else if (RadioStart->Caption=="h. secs") bHalfSeconds=true;

			if(RadioCall->Caption=="call") bCall=true;
			else if (RadioCall->Caption=="revert") bRevert=true;
			else if (RadioCall->Caption=="seconds") bSeconds=true;
			else if (RadioCall->Caption=="h. secs") bHalfSeconds=true;

								metaSpriteDirectLoop[metaSpriteActive]=false;
			if(bCall) 			metaSpriteDirectCall[metaSpriteActive]=false;
			if(bCall2)          metaSpriteDirectCall2[metaSpriteActive]=false;
			if(bStart) 			metaSpriteDirectStart[metaSpriteActive]=false;
			if(bRevert) 		metaSpriteDirectRevert[metaSpriteActive]=false;
			if(bSeconds) 		metaSpriteDirectSeconds[metaSpriteActive]=false;
			if(bHalfSeconds) 	metaSpriteDirectHalfSeconds[metaSpriteActive]=false;
		}
		else{
			RadioStart->Checked=true;

			if(RadioStart->Caption=="start") bStart=true;
			else if (RadioStart->Caption=="call2") bCall2=true;
			else if (RadioStart->Caption=="revert") bRevert=true;
			else if (RadioStart->Caption=="seconds") bSeconds=true;
			else if (RadioStart->Caption=="h. secs") bHalfSeconds=true;

								metaSpriteDirectLoop[metaSpriteActive]=false;
			if(bCall2)          metaSpriteDirectCall2[metaSpriteActive]=false;
			if(bStart) 			metaSpriteDirectStart[metaSpriteActive]=true;
			if(bRevert) 		metaSpriteDirectRevert[metaSpriteActive]=true;
			if(bSeconds) 		metaSpriteDirectSeconds[metaSpriteActive]=true;
			if(bHalfSeconds) 	metaSpriteDirectHalfSeconds[metaSpriteActive]=true;
	   }
	}
	if (Key=='k') {
		isTags=true;
		isDuration=false;
		FormMain->SetUndo();
		if(RadioCall->Checked){
			RadioNone->Checked=true;

			if(RadioStart->Caption=="start") bStart=true;
			 else if (RadioStart->Caption=="call2") bCall2=true;
			 else if (RadioStart->Caption=="revert") bRevert=true;
			 else if (RadioStart->Caption=="seconds") bSeconds=true;
			 else if (RadioStart->Caption=="h. secs") bHalfSeconds=true;

			if(RadioCall->Caption=="call") bCall=true;
			else if (RadioCall->Caption=="revert") bRevert=true;
			else if (RadioCall->Caption=="seconds") bSeconds=true;
			else if (RadioCall->Caption=="h. secs") bHalfSeconds=true;

								metaSpriteDirectLoop[metaSpriteActive]=false;
			if(bCall) 			metaSpriteDirectCall[metaSpriteActive]=false;
			if(bCall2)          metaSpriteDirectCall2[metaSpriteActive]=false;
			if(bStart) 			metaSpriteDirectStart[metaSpriteActive]=false;
			if(bRevert) 		metaSpriteDirectRevert[metaSpriteActive]=false;
			if(bSeconds) 		metaSpriteDirectSeconds[metaSpriteActive]=false;
			if(bHalfSeconds) 	metaSpriteDirectHalfSeconds[metaSpriteActive]=false;
		}
		else{
			RadioCall->Checked=true;
			if(RadioCall->Caption=="call") bCall=true;
			else if (RadioCall->Caption=="revert") bRevert=true;
			else if (RadioCall->Caption=="seconds") bSeconds=true;
			else if (RadioCall->Caption=="h. secs") bHalfSeconds=true;
								metaSpriteDirectLoop[metaSpriteActive]=false;
				if(bCall) 			metaSpriteDirectCall[metaSpriteActive]=true;
				if(bCall2)          metaSpriteDirectCall2[metaSpriteActive]=false;
				if(bStart) 			metaSpriteDirectStart[metaSpriteActive]=false;
				if(bRevert) 		metaSpriteDirectRevert[metaSpriteActive]=true;
				if(bSeconds) 		metaSpriteDirectSeconds[metaSpriteActive]=true;
				if(bHalfSeconds) 	metaSpriteDirectHalfSeconds[metaSpriteActive]=true;
	   }
	}
	if (Key=='n') {
		isTags=true;
		isDuration=false;
		FormMain->SetUndo();
		RadioNone->Checked=true;

		if(RadioStart->Caption=="start") bStart=true;
			 else if (RadioStart->Caption=="call2") bCall2=true;
			 else if (RadioStart->Caption=="revert") bRevert=true;
			 else if (RadioStart->Caption=="seconds") bSeconds=true;
			 else if (RadioStart->Caption=="h. secs") bHalfSeconds=true;

			if(RadioCall->Caption=="call") bCall=true;
			else if (RadioCall->Caption=="revert") bRevert=true;
			else if (RadioCall->Caption=="seconds") bSeconds=true;
			else if (RadioCall->Caption=="h. secs") bHalfSeconds=true;

								metaSpriteDirectLoop[metaSpriteActive]=false;
			if(bCall) 			metaSpriteDirectCall[metaSpriteActive]=false;
			if(bCall2)          metaSpriteDirectCall2[metaSpriteActive]=false;
			if(bStart) 			metaSpriteDirectStart[metaSpriteActive]=false;
			if(bRevert) 		metaSpriteDirectRevert[metaSpriteActive]=false;
			if(bSeconds) 		metaSpriteDirectSeconds[metaSpriteActive]=false;
			if(bHalfSeconds) 	metaSpriteDirectHalfSeconds[metaSpriteActive]=false;
	}

	UpdateOneListItem(metaSpriteActive); //update previous


	if(metaSpriteDirectStart[metaSpriteActive+1] ||  metaSpriteDirectLoop[metaSpriteActive]){
	{
		  if(((isDuration && chkValidDuration->Checked) || (isTags && chkValidTag->Checked)) && chkLoopDurationStep->Checked)
			while(1){
			if (metaSpriteActive>0) metaSpriteActive--;
			if (metaSpriteActive == 0) break;
			else if (  metaSpriteDirectStart[metaSpriteActive]) break;
			else if (  metaSpriteDirectLoop[metaSpriteActive])  {metaSpriteActive++; break;}
			else continue;
		  }
		  }
	}
	else {
		int m=0;
		if(isDuration && chkValidDuration->Checked) m = iEditStep;
		if(isTags && chkValidTag->Checked) m = iEditStep;
		metaSpriteActive=(metaSpriteActive+m)%256;
	}
	UpdateOneListItem(metaSpriteActive);

	//update metasprite tab
	cueUpdateMetasprite=true;
	cueStats=true;
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;



	//ListBoxSprites->Invalidate();

	Key = 0;
	durationNTSC->Text=IntToStr(metaSpriteDirectFrame_NTSC[metaSpriteActive]);
	durationPAL->Text=IntToStr(metaSpriteDirectFrame_PAL[metaSpriteActive]);	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesEnter(
      TObject *Sender)
{
		 ListBoxSprites->SetFocus();
		FormMain->EnableDisableTypeConflictShortcuts(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesExit(TObject *Sender)
{
		FormMain->EnableDisableTypeConflictShortcuts(false);		
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationPALKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if(Key==VK_RETURN)
	{
		FormMain->SetUndo();
        	int n;

	if(!TryStrToInt(durationPAL->Text,n)) n=0;

		if(n<0) n=0;

		if(n>16384) n=16384;
		metaSpriteDirectFrame_PAL[metaSpriteActive]=n;	
		durationPAL->Text=IntToStr(n);	
	    UpdateActive(false);
		ListBoxSprites->Invalidate();

		//UpdateOneListItem(metaSpriteActive);
	}
	if (Key==VK_ESCAPE){
		durationPAL->Text=metaSpriteDirectFrame_PAL[metaSpriteActive];
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::durationNTSCKeyDown(
      TObject *Sender, WORD &Key, TShiftState Shift)
{
	 if(Key==VK_RETURN)
	{
		FormMain->SetUndo();


		int n;

		if(!TryStrToInt(durationNTSC->Text,n)) n=0;

		if(n<0) n=0;

		if(n>16384) n=16384;
		metaSpriteDirectFrame_NTSC[metaSpriteActive]=n;
		durationNTSC->Text=IntToStr(n);
        UpdateActive(false);
		ListBoxSprites->Invalidate();

		//UpdateOneListItem(metaSpriteActive);
	}
	if (Key==VK_ESCAPE){
		durationNTSC->Text=metaSpriteDirectFrame_NTSC[metaSpriteActive];
	}
}
//---------------------------------------------------------------------------


 void __fastcall TFormManageMetasprites::UpdateOneListItem(int id)
 {
	   AnsiString str;
	int j,cnt;
	bool bCall, bCall2, bRevert, bSeconds, bHalfSeconds; 
	bool bLoop;
	bool bStart;
 
		ListBoxSprites->ItemIndex=metaSpriteActive;
		bCall  		=  metaSpriteDirectCall[id] && RadioCall->Caption=="call";
		bLoop  		=  metaSpriteDirectLoop[id];
		bStart 		=  metaSpriteDirectStart[id] && RadioStart->Caption=="start";
		bCall2 		=  metaSpriteDirectCall2[id] && RadioStart->Caption=="call2";


		bRevert		=  metaSpriteDirectRevert[id] && (RadioStart->Caption=="revert" || RadioCall->Caption=="revert");
		bSeconds	=  metaSpriteDirectSeconds[id] && (RadioStart->Caption=="seconds" || RadioCall->Caption=="seconds");
		bHalfSeconds=  metaSpriteDirectHalfSeconds[id] && (RadioStart->Caption=="h. secs" || RadioCall->Caption=="h. secs");
		str=IntToStr(id)+": ";
		if(id<10) str+="  ";
		if(btnShowLabels->Down) str = str + metaSpriteNames[id] + " | "; 
		cnt = 0;
		for(j=0;j<64*4;j+=4)
		{
			if(metaSprites[id*64*4+j]<255) ++cnt;
		}


		if(btnShowNTSC->Down && btnShowPAL->Down)
			str = str + "Duration: " +StrToInt(metaSpriteDirectFrame_NTSC[id]) +":"+StrToInt(metaSpriteDirectFrame_PAL[id]);
		else if (btnShowNTSC->Down)
			str = str + "Duration: " +StrToInt(metaSpriteDirectFrame_NTSC[id]);
		else if (btnShowPAL->Down) 
			str = str + "Duration: " +StrToInt(metaSpriteDirectFrame_PAL[id]);

		if((btnShowNTSC->Down || btnShowPAL->Down) && btnShowCount->Down)
			str += " | ";

		if(btnShowCount->Down){
			if(!cnt) str+="empty"; 
			else if (cnt==1) str+=IntToStr(cnt)+" sprite";
			else    		   str+=IntToStr(cnt)+" sprites";
		}

		if(btnShowTags->Down){
			if(bCall) str+=" <call>";
			if(bCall2) str+=" <call2>";
			if(bStart) str+=" <start>";
			if(bLoop) str+=" <loop>";
			if(bRevert) str+=" <revert>";
			if(bSeconds) str+=" <seconds>";
			if(bHalfSeconds) str+=" <h. secs>";
		}

		ListBoxSprites->Items->Strings[id]=str;
	
 }

//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::FormDestroy(TObject *Sender)
{
	//delete animBuf;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::chkLinkMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Link: editing either field will also update the other.";
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::chkAdjustMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Adjust: if linked, will try to adjust the frame ticks of the other system to match in duration.\nAn ntsc frame is ~16.67ms, while a pal frame is 20ms. 6 ntsc frames equal 5 pal frames in time.\nHand adjustment may be necessary for a loop length to be equal.\nMost NES games don't go through this trouble, but the possibility available here.";
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::RadioX1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Normal playback speed. Frame ticks correspond to a normal NTSC or PAL NES.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::RadioX2MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Tick intervals are doubled; halving playback speed.\nCan be useful to study what's going on.\nAlternately, you can exploit this playback speed to count durations twice as your intended format, only updating animations on even frames."; 
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::RadioSteadyMouseEnter(
	  TObject *Sender)
{
	FormMain->LabelStats->Caption="Overrides frame durations with a static half second.\nUseful for examining an animation devoid of the timing aspect.";
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::btnReleaseClick(TObject *Sender)
{
	btnRelease->Enabled=false;	
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::UpDownNTSCClick(TObject *Sender,
      TUDBtnType Button)
{
	int n;

	if(!TryStrToInt(durationNTSC->Text,n)) n=0;

	if(n<0) n=0;

	if(n>16384) n=16384;
	metaSpriteDirectFrame_NTSC[metaSpriteActive]=n;
	durationNTSC->Text=IntToStr(n);	

	FormMain->EnableDisableTypeConflictShortcuts(false);
    UpdateActive(false);
	ListBoxSprites->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::UpDownPALClick(TObject *Sender,
      TUDBtnType Button)
{
		int n;

	if(!TryStrToInt(durationPAL->Text,n)) n=0;

	if(n<0) n=0;

	if(n>16384) n=16384;
	metaSpriteDirectFrame_PAL[metaSpriteActive]=n;	
	durationPAL->Text=IntToStr(n);	
	FormMain->EnableDisableTypeConflictShortcuts(false);
    UpdateActive(false);
	ListBoxSprites->Invalidate();
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::SpeedButton2Click(TObject *Sender)
{
	//bool useStart = FormMetaspritePlaybackRules->RadioButton1->Checked;
	bool isNTSC = PlayNTSC->Checked;
	TimerFrameTick->Enabled=false;


	//if at start of loop, prepare to skip to previous loop
	if (metaSpriteDirectFrame_Cursor>0 && TimerSkipDelay->Enabled){
		 if (  metaSpriteDirectStart[metaSpriteDirectFrame_Cursor-1]) metaSpriteDirectFrame_Cursor--;
		else if (  metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor-1])  metaSpriteDirectFrame_Cursor--;

	}
	TimerSkipDelay->Enabled=false;

	while (1) {
		if (metaSpriteDirectFrame_Cursor>0) metaSpriteDirectFrame_Cursor--;
		if (metaSpriteDirectFrame_Cursor == 0) break;
		else if (  metaSpriteDirectStart[metaSpriteDirectFrame_Cursor]) break;
		else if (  metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor])  {metaSpriteDirectFrame_Cursor++; break;}
		else continue;
	}
	//set duration
	if(RadioSteady->Checked) iMsprTimeDelay= isNTSC? 60:50;
	else{
			if(isNTSC) iMsprTimeDelay=  metaSpriteDirectFrame_NTSC[metaSpriteDirectFrame_Cursor];
			else	   iMsprTimeDelay=	metaSpriteDirectFrame_PAL[metaSpriteDirectFrame_Cursor];
	}

	//visuals
	ListBoxSprites->Invalidate();
	AssignFrame(true);
	/*
	imageListMetaSprites->GetBitmap(metaSpriteDirectFrame_Cursor, animBuf);
	if(btnZoom->Down) ImageMetaSprite->Canvas->StretchDraw(zoomDstRect, animBuf);
	else ImageMetaSprite->Picture->Bitmap->Assign(animBuf);

    */
	TimerSkipDelay->Enabled=true;



}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::TimerSkipDelayTimer(
	  TObject *Sender)
{
   TimerSkipDelay->Enabled=false;
   //fix button state and reenable timer
	if(SpeedButton1->Caption=="Pause") //means is playing
	{
		TimerFrameTick->Enabled=true;
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::SpeedButton5Click(TObject *Sender)
{
	bool isNTSC = PlayNTSC->Checked;
	TimerFrameTick->Enabled=false;


	//if at start of loop, prepare to skip to previous loop
	if (metaSpriteDirectFrame_Cursor>0 && TimerSkipDelay->Enabled){
		 if (  metaSpriteDirectStart[metaSpriteDirectFrame_Cursor+1]) metaSpriteDirectFrame_Cursor++;
		else if (  metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor+1])  metaSpriteDirectFrame_Cursor++;

	}
	TimerSkipDelay->Enabled=false;

	while (1) {
		if (metaSpriteDirectFrame_Cursor<255) metaSpriteDirectFrame_Cursor++;
		if (metaSpriteDirectFrame_Cursor == 255) break;
		else if (  metaSpriteDirectStart[metaSpriteDirectFrame_Cursor]) break;
		else if (  metaSpriteDirectLoop[metaSpriteDirectFrame_Cursor])  {metaSpriteDirectFrame_Cursor++; break;}
		else continue;
	}
	//set duration
	if(RadioSteady->Checked) iMsprTimeDelay= isNTSC? 60:50;
	else{
			if(isNTSC) iMsprTimeDelay=  metaSpriteDirectFrame_NTSC[metaSpriteDirectFrame_Cursor];
			else	   iMsprTimeDelay=	metaSpriteDirectFrame_PAL[metaSpriteDirectFrame_Cursor];
	}

	//visuals
	ListBoxSprites->Invalidate();
	AssignFrame(true);
	/*
	imageListMetaSprites->GetBitmap(metaSpriteDirectFrame_Cursor, animBuf);
	//ImageMetaSprite->Picture->Bitmap->Assign(animBuf);
	if(btnZoom->Down) ImageMetaSprite->Canvas->StretchDraw(zoomDstRect, animBuf);
	else ImageMetaSprite->Picture->Bitmap->Assign(animBuf);
    */
	TimerSkipDelay->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::EditStepDurExit(TObject *Sender)
{
	/*
	int iDurationStep=1;
	int iTagStep=1;
	*/
	int n;

	if(!TryStrToInt(EditStepDur->Text,n)) n=0;

	if(n<0) n=0;

	if(n>255) n=0;
	iEditStep=n;
	EditStepDur->Text=IntToStr(n);

	FormMain->EnableDisableTypeConflictShortcuts(false);
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::EditStepDurKeyDown(TObject *Sender,
	  WORD &Key, TShiftState Shift)
{
	 if(Key==VK_RETURN)
	{
		int n;

		if(!TryStrToInt(EditStepDur->Text,n)) n=0;

		if(n<0) n=0;

		if(n>16) n=0;
		iEditStep=n;
		EditStepDur->Text=IntToStr(n);
	}
	if (Key==VK_ESCAPE){
		EditStepDur->Text=iEditStep;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::EditStepDurKeyPress(
      TObject *Sender, char &Key)
{
		if(!((Key>='0'&&Key<='9')||Key==VK_BACK||Key==VK_DELETE)) Key=0;	
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::ImageMetaSpriteMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
	  int Y)
{
	iMsprMouseX_down = X;
	iMsprMouseY_down = Y;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ImageMetaSpriteMouseMove(
      TObject *Sender, TShiftState Shift, int X, int Y)
{
   if(btnZoom->Down && Shift.Contains(ssLeft)){
	  int tmpX = X-iMsprMouseX_down +zoomPosX;
	  int tmpY = Y-iMsprMouseY_down +zoomPosY;

	  if (tmpX< -64*zoomFactor) tmpX=-64*zoomFactor;
	  if (tmpY< -64*zoomFactor) tmpY= -64*zoomFactor;
	  if (tmpX>0) tmpX=0;
	  if (tmpY>0) tmpY=0;

      //special case
	  if(zoomFactor==1.5){
		 if (tmpX< -43*zoomFactor) tmpX=-43*zoomFactor;
		 if (tmpY< -43*zoomFactor) tmpY= -43*zoomFactor;
	  }

	  //zoomPosX = tmpX;
	  //zoomPosY = tmpY;

	  //zoomDstRect(zoomPosX, zoomPosY, zoomPosX + zoomZoomedWidth, zoomPosY + zoomZoomedHeight)
	  zoomDstRect.left = tmpX;
	  zoomDstRect.top  = tmpY;
	  zoomDstRect.right = tmpX + zoomZoomedWidth;
	  zoomDstRect.bottom  = tmpY + zoomZoomedHeight;

	  TimerPan->Enabled=true;
	  //AssignFrame(false);
	  //ImageMetaSprite->Canvas->StretchDraw(zoomDstRect, animBuf);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ImageMetaSpriteMouseEnter(
      TObject *Sender)
{
	if(btnZoom->Down) 	 {Screen->Cursor = crSizeAll;
		FormMain->LabelStats->Caption="Scroll to change zoom level. Grab to pan.";
	}
	else                 Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ImageMetaSpriteMouseLeave(
      TObject *Sender)
{
	if(btnZoom->Down) 	 Screen->Cursor = crDefault;
	FormMain->LabelStats->Caption="---";
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::FormMouseWheel(TObject *Sender,
      TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
	if(!btnZoom->Down) btnZoom->Down=true;

	int tmpD=WheelDelta/2;

	if(tmpD>0) zoomFactor+=0.5;
	else zoomFactor-=0.5;
	if(zoomFactor>4) zoomFactor=4;
	if(zoomFactor<1.5) zoomFactor=1.5;

	float pan_counterweight = (zoomFactor -2)/2;

	zoomZoomedWidth = 128 * zoomFactor;
	zoomZoomedHeight = 128 * zoomFactor;

	zoomPosX = (zoomImageWidth - zoomZoomedWidth) / 2;
	zoomPosY = (zoomImageHeight - zoomZoomedHeight) / (3-pan_counterweight);

	zoomDstRect.left = zoomPosX;
	zoomDstRect.top  = zoomPosY;
	zoomDstRect.right = zoomPosX + zoomZoomedWidth;
	zoomDstRect.bottom  = zoomPosY + zoomZoomedHeight;
	AssignFrame(false);


    DisplayZoomLevel();


	//ImageMetaSprite->Canvas->StretchDraw(zoomDstRect, animBuf);
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::SpeedButton6Click(TObject *Sender)
{

	FormMain->SetUndo();

	int id=ListBoxSprites->ItemIndex;

	if (CheckMoveSprites->Checked) memset(&metaSprites[id*64*4],255,64*4);
    if(CheckMoveLabels->Checked){
		metaSpriteNames[id] = "unnamed";
	}
	if(CheckMoveTags->Checked){
			metaSpriteDirectStart[id]			= false;
			metaSpriteDirectLoop[id]			= false;
			metaSpriteDirectCall[id]			= false;
			metaSpriteDirectCall2[id]			= false;
			metaSpriteDirectRevert[id]			= false;
			metaSpriteDirectSeconds[id]			= false;
			metaSpriteDirectHalfSeconds[id]		= false;
	}
	if(CheckMoveDurations->Checked){
			 metaSpriteDirectFrame_NTSC[id]		= 6;
			 metaSpriteDirectFrame_PAL[id]		= 5;
	}
	FormMain->UpdateMetaSprite(true);
	Update();
}
//---------------------------------------------------------------------------



void __fastcall TFormManageMetasprites::AssignFrame(bool refetch)
{

   bool doGrid = btnGrid->Down;
   bool doBoxes = btnBox->Down;
   bool doWarning =  btnWarn->Down;
   int hcol, vcol;
   extern int spriteGridX;
   extern int spriteGridY;

   //not necessary for zoom/pan
   int mspr_id = SpeedButton1->Caption=="Play"?  metaSpriteActive:metaSpriteDirectFrame_Cursor;
   if(refetch) imageListMetaSprites->GetBitmap(mspr_id, animBuf);


   //apply stuff.

   if(btnZoom->Down) ImageMetaSprite->Canvas->StretchDraw(zoomDstRect, animBuf);
   else ImageMetaSprite->Picture->Bitmap->Assign(animBuf);


   TCanvas *canvas = ImageMetaSprite->Canvas;
   if (doGrid) {

	float tmpzoom = btnZoom->Down? zoomFactor:1;
	int gridSize = 8*tmpzoom; // Adjust as needed

	//canvas->Pen->Color = clGray; // Set dot color
	canvas->Pen->Width = 1; // Set dot size
	TColor gridcol = TColor(0x606060);
	TColor crosscol = TColor(0x303030);

	int tmpPosX = btnZoom->Down? zoomDstRect.left:0;
	int tmpPosY = btnZoom->Down? zoomDstRect.top:0;

	for (int x = 0+tmpPosX; x < ImageMetaSprite->Width; x += gridSize) {
		for (int y = 0+tmpPosY; y < ImageMetaSprite->Height; y += gridSize) {


				canvas->Pixels[x][y] = gridcol;


			for (int x2 = 0; x2 < gridSize; x2 += 2) {
				for (int y2 = 0; y2 < gridSize; y2 += 2) {
					if((x==x2+x && y!=y2+y) || (x!=x2+x && y==y2+y)){

						if(x+x2 < ImageMetaSprite->Width &&  y+y2 < ImageMetaSprite->Height){
							if(y+y2-tmpPosY==spriteGridY*tmpzoom || x+x2-tmpPosX==spriteGridX*tmpzoom)
								crosscol=  TColor(0x808080);
							else crosscol=  TColor(0x303030);
							}
							canvas->Pixels[x+x2][y+y2]= crosscol;
						}
					}
				}
			}
		}
	}
   if(doBoxes || doWarning){

		//FormManageMetasprites->Caption="!";
		bool frame_all = FormMain->SpeedButtonFrameAll->Down;
		bool frame_none = FormMain->SpeedButtonFrameNone->Down;
		int pp=mspr_id*64*4+63*4;
		int sx,sy;
		int cy;
		int check[128];
		memset(check,0,sizeof(check));
		extern int spriteActive;
		int tmpPosX = btnZoom->Down? zoomDstRect.left:0;
		int tmpPosY = btnZoom->Down? zoomDstRect.top:0;

		TColor colbox = clBlack;
		float tmpzoom = btnZoom->Down? zoomFactor:1;
		//memset(check,0,sizeof(check));
		for(int i=63;i>=0;--i)//reverse order to make proper sprites drawing priority
		{
			sy   =metaSprites[pp+0];
			//tile=metaSprites[pp+1];
			//attr=metaSprites[pp+2];
			sx   =metaSprites[pp+3];

			if(sy<255)
			{
				if(doBoxes){
				colbox=frame_all?clGray:clBlack;


				if(i<FormMain->ListBoxSpriteList->Items->Count)
					if(!frame_none&&(FormMain->ListBoxSpriteList->Selected[i]))
					colbox=clWhite; //clMenu if we need the distinction
				if(!frame_none&&(spriteActive==i)) colbox=clWhite;
				}

				if(doWarning){
					cy=sy;

					for(int j=0;j<(FormMain->SpeedButtonSprite8x16->Down?16:8);++j)
					{
						if(cy>=-63&&cy<128) ++check[cy];

						++cy;
					}
			   }

			if(colbox!=clBlack)
			{
				canvas->Brush->Style=bsClear;
				canvas->Pen->Color=colbox;
				canvas->Rectangle(
				(sx)*tmpzoom +tmpPosX,
				(sy)*tmpzoom +tmpPosY,
				(sx)*tmpzoom + tmpPosX+ 8*tmpzoom,
				(sy)*tmpzoom +tmpPosY +(FormMain->SpeedButtonSprite8x16->Down?16*tmpzoom:8*tmpzoom));
			}

			}
			pp-=4;
		}
		if(doWarning){
			int tmpy;
			for(int i=0;i<128;++i)
			{
				if(check[i]>4 && bWarnMsprYellow)
				{
					for(int z=0; z<zoomFactor; z++){
						tmpy =  tmpPosY+i*tmpzoom+z;
						canvas->Pixels[1][tmpy] = TColor(0x00daff);
						canvas->Pixels[126][tmpy] = TColor(0x00daff);
					}
					//DrawSpriteDot(img->Picture,1,img->Height/scale/2-64+i,(TColor)0x00daff,scale);
					//DrawSpriteDot(img->Picture,126,img->Height/scale/2-64+i,(TColor)0x00daff,scale);
				}

				if(check[i]>7 && bWarnMsprOrange)
				{
					for(int z=0; z<zoomFactor; z++){
						tmpy =  tmpPosY+i*tmpzoom+z;
						canvas->Pixels[1][tmpy] = TColor(0x0090f0);
						canvas->Pixels[2][tmpy] = TColor(0x0090f0);
						canvas->Pixels[125][tmpy] = TColor(0x0090f0);
						canvas->Pixels[126][tmpy] = TColor(0x0090f0);
					}
				}
				if(check[i]>8 && bWarnMsprRed)
				{
                    for(int z=0; z<zoomFactor; z++){
						tmpy =  tmpPosY+i*tmpzoom+z;
					canvas->Pixels[1][tmpy] = TColor(0x2222e0);
					canvas->Pixels[2][tmpy] = TColor(0x2222e0);
					canvas->Pixels[3][tmpy] = TColor(0x2222e0);
					canvas->Pixels[124][tmpy] = TColor(0x2222e0);
					canvas->Pixels[125][tmpy] = TColor(0x2222e0);
					canvas->Pixels[126][tmpy] = TColor(0x2222e0);
					}
				}
				if(check[i]>iWarnMsprCyan && bWarnMsprCyan)
				{
						for(int z=0; z<zoomFactor; z++){
							tmpy =  tmpPosY+i*tmpzoom+z;
							canvas->Pixels[0][tmpy] = TColor(0xaaaa00);
							canvas->Pixels[127][tmpy] = TColor(0xaaaa00);
						}

				}
			}
		}
	}
   ImageMetaSprite->Repaint();
   }

//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ImageMetaSpriteMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	 if(btnZoom->Down){
	  /*
	  int tmpX = X-iMsprMouseX_down +zoomPosX;
	  int tmpY = Y-iMsprMouseY_down +zoomPosY;

	  if (tmpX< -64*zoomFactor) tmpX=-64*zoomFactor;
	  if (tmpY< -64*zoomFactor) tmpY= -64*zoomFactor;
	  if (tmpX>0) tmpX=0;
	  if (tmpY>0) tmpY=0;
	  */
	  zoomPosX = zoomDstRect.left;//-(X-iMsprMouseX_down);
	  zoomPosY = zoomDstRect.top;//-(Y-iMsprMouseY_down);

	  AssignFrame(false);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::TimerPanTimer(TObject *Sender)
{
	TimerPan->Enabled=false;
	AssignFrame(false);

}
//---------------------------------------------------------------------------



void __fastcall TFormManageMetasprites::DisplayZoomLevel(void)
{
	if(zoomFactor==1.5)btnZoom->Caption="150%";
	if(zoomFactor==2.0)btnZoom->Caption="200%";
	if(zoomFactor==2.5)btnZoom->Caption="250%";
	if(zoomFactor==3.0)btnZoom->Caption="300%";
	if(zoomFactor==3.5)btnZoom->Caption="350%";
	if(zoomFactor==4.0)btnZoom->Caption="400%";
}

void __fastcall TFormManageMetasprites::UpDownDurClick(TObject *Sender,
      TUDBtnType Button)
{
		int n;

		if(!TryStrToInt(EditStepDur->Text,n)) n=0;

		if(n<0) n=0;

		if(n>16) n=0;
		iEditStep=n;
		EditStepDur->Text=IntToStr(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	if(Shift.Contains(ssRight)) {

	TPoint pt(X, Y);
		msprList_dragID = ListBoxSprites->ItemAtPos(pt, true);

		if (msprList_dragID != -1) {
			msprList_isDragging=true;
			Screen->Cursor = crDrag;

			bool bCtrl		= (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool bAlt		= (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
			if(bCtrl && !bAlt) FormManageMetasprites->Caption="Move";
			else if(bCtrl && bAlt) FormManageMetasprites->Caption="Clone";
			else FormManageMetasprites->Caption="Swap";
		}
		else {
			msprList_isDragging=false;
			Screen->Cursor = crDefault;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	if(msprList_isDragging){

		TPoint pt(X, Y);
		msprList_dropID = ListBoxSprites->ItemAtPos(pt, true);


		if((msprList_dragID!=msprList_dropID)
			&& msprList_dragID>=0
			&& msprList_dragID>=0
			&& msprList_dragID<256
			&& msprList_dragID<256
			&& msprList_dropID>=0
			&& msprList_dropID>=0
			&& msprList_dropID<256
			&& msprList_dropID<256

			)
		{

			FormMain->SetUndo();
			metaSpriteActive =  msprList_dropID; //focus follows with
			//buffer
			AnsiString	buf_drag_metaSpriteNames		= metaSpriteNames[msprList_dragID];

			bool buf_drag_metaSpriteDirectStart			= metaSpriteDirectStart[msprList_dragID];
			bool buf_drag_metaSpriteDirectLoop			= metaSpriteDirectLoop[msprList_dragID];
			bool buf_drag_metaSpriteDirectCall			= metaSpriteDirectCall[msprList_dragID];
			bool buf_drag_metaSpriteDirectCall2			= metaSpriteDirectCall2[msprList_dragID];
			bool buf_drag_metaSpriteDirectRevert		= metaSpriteDirectRevert[msprList_dragID];
			bool buf_drag_metaSpriteDirectSeconds		= metaSpriteDirectSeconds[msprList_dragID];
			bool buf_drag_metaSpriteDirectHalfSeconds	= metaSpriteDirectHalfSeconds[msprList_dragID];
			int buf_drag_metaSpriteDirectFrame_NTSC		= metaSpriteDirectFrame_NTSC[msprList_dragID];
			int buf_drag_metaSpriteDirectFrame_PAL		= metaSpriteDirectFrame_PAL[msprList_dragID];

			char buf_drag_metasprite[64*4];
			memcpy(&buf_drag_metasprite[0],&metaSprites[msprList_dragID*64*4],64*4);
			if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)){
				//remove & shift
				if(msprList_dragID<msprList_dropID){
					for(int i=msprList_dragID;i<msprList_dropID;++i)
					{
						if (CheckMoveSprites->Checked) memcpy(&metaSprites[i*64*4],&metaSprites[(i+1)*64*4],64*4);

						if(CheckMoveLabels->Checked){
							metaSpriteNames[i]	= metaSpriteNames[i+1];
						}
						if(CheckMoveTags->Checked){
							metaSpriteDirectStart[i]			= metaSpriteDirectStart[i+1];
							metaSpriteDirectLoop[i]				= metaSpriteDirectLoop[i+1];
							metaSpriteDirectCall[i]				= metaSpriteDirectCall[i+1];
							metaSpriteDirectCall2[i]			= metaSpriteDirectCall2[i+1];
							metaSpriteDirectRevert[i]			= metaSpriteDirectRevert[i+1];
							metaSpriteDirectSeconds[i]			= metaSpriteDirectSeconds[i+1];
							metaSpriteDirectHalfSeconds[i]		= metaSpriteDirectHalfSeconds[i+1];
						}
							if(CheckMoveDurations->Checked){
								metaSpriteDirectFrame_NTSC[i]		= metaSpriteDirectFrame_NTSC[i+1];
								metaSpriteDirectFrame_PAL[i]		= metaSpriteDirectFrame_PAL[i+1];
							}
						}
				}
				else{
					for(int i=msprList_dragID;i>msprList_dropID;--i)
					{
						if (CheckMoveSprites->Checked) memcpy(&metaSprites[i*64*4],&metaSprites[(i-1)*64*4],64*4);

						if(CheckMoveLabels->Checked){
							metaSpriteNames[i]	= metaSpriteNames[i-1];
						}
						if(CheckMoveTags->Checked){
							metaSpriteDirectStart[i]			= metaSpriteDirectStart[i-1];
							metaSpriteDirectLoop[i]				= metaSpriteDirectLoop[i-1];
							metaSpriteDirectCall[i]				= metaSpriteDirectCall[i-1];
							metaSpriteDirectCall2[i]			= metaSpriteDirectCall2[i-1];
							metaSpriteDirectRevert[i]			= metaSpriteDirectRevert[i-1];
							metaSpriteDirectSeconds[i]			= metaSpriteDirectSeconds[i-1];
							metaSpriteDirectHalfSeconds[i]		= metaSpriteDirectHalfSeconds[i-1];
						}
						if(CheckMoveDurations->Checked){
								metaSpriteDirectFrame_NTSC[i]		= metaSpriteDirectFrame_NTSC[i-1];
								metaSpriteDirectFrame_PAL[i]		= metaSpriteDirectFrame_PAL[i-1];
						}
					}
				}//end remove and shift.
				}else if (!Shift.Contains(ssCtrl) ||  (!Shift.Contains(ssCtrl) && Shift.Contains(ssAlt))) //swap
				{
					if (CheckMoveSprites->Checked) memcpy(&metaSprites[msprList_dragID*64*4],&metaSprites[msprList_dropID*64*4],64*4);

					if(CheckMoveLabels->Checked){
						metaSpriteNames[msprList_dragID]	= metaSpriteNames[msprList_dropID];
					}
					if(CheckMoveTags->Checked){
						metaSpriteDirectStart[msprList_dragID]			= metaSpriteDirectStart[msprList_dropID];
						metaSpriteDirectLoop[msprList_dragID]			= metaSpriteDirectLoop[msprList_dropID];
						metaSpriteDirectCall[msprList_dragID]			= metaSpriteDirectCall[msprList_dropID];
						metaSpriteDirectCall2[msprList_dragID]			= metaSpriteDirectCall2[msprList_dropID];
						metaSpriteDirectRevert[msprList_dragID]			= metaSpriteDirectRevert[msprList_dropID];
						metaSpriteDirectSeconds[msprList_dragID]		= metaSpriteDirectSeconds[msprList_dropID];
						metaSpriteDirectHalfSeconds[msprList_dragID]	= metaSpriteDirectHalfSeconds[msprList_dropID];
					}
					if(CheckMoveDurations->Checked){
						metaSpriteDirectFrame_NTSC[msprList_dragID]		= metaSpriteDirectFrame_NTSC[msprList_dropID];
						metaSpriteDirectFrame_PAL[msprList_dragID]		= metaSpriteDirectFrame_PAL[msprList_dropID];
					}
				}
			
			//insert from buffer
			if (CheckMoveSprites->Checked) memcpy(&metaSprites[msprList_dropID*64*4],&buf_drag_metasprite[0],64*4);

			if(CheckMoveLabels->Checked){
				if(Shift.Contains(ssCtrl) && Shift.Contains(ssAlt)){
					int count = 0;
					AnsiString delimiter = "__";
                    for (int i = 0; i < 256; i++) {
						if (i == msprList_dropID) continue;
						AnsiString currentName = metaSpriteNames[i];
						AnsiString targetName = buf_drag_metaSpriteNames;
						int pos = currentName.Pos(delimiter);
						if (pos > 0) currentName = currentName.SubString(1, pos-1);  // Ignore delimiter and characters after it
						pos = targetName.Pos(delimiter);
						if (pos > 0) targetName = targetName.SubString(1, pos-1);  // Ignore delimiter and characters after it
						if (currentName == targetName) count++;
					}
					metaSpriteNames[msprList_dropID]=buf_drag_metaSpriteNames+"__"+IntToStr(count);

				}

				else metaSpriteNames[msprList_dropID]	= buf_drag_metaSpriteNames;
			}
			if(CheckMoveTags->Checked){
				metaSpriteDirectStart[msprList_dropID]			= buf_drag_metaSpriteDirectStart;
				metaSpriteDirectLoop[msprList_dropID]			= buf_drag_metaSpriteDirectLoop;
				metaSpriteDirectCall[msprList_dropID]			= buf_drag_metaSpriteDirectCall;
				metaSpriteDirectCall2[msprList_dropID]			= buf_drag_metaSpriteDirectCall2;
				metaSpriteDirectRevert[msprList_dropID]			= buf_drag_metaSpriteDirectRevert;
				metaSpriteDirectSeconds[msprList_dropID]		= buf_drag_metaSpriteDirectSeconds;
				metaSpriteDirectHalfSeconds[msprList_dropID]	= buf_drag_metaSpriteDirectHalfSeconds;
			}
			if(CheckMoveDurations->Checked){
				metaSpriteDirectFrame_NTSC[msprList_dropID]	= buf_drag_metaSpriteDirectFrame_NTSC;
				metaSpriteDirectFrame_PAL[msprList_dropID]		= buf_drag_metaSpriteDirectFrame_PAL;
			}
		}   //index safety enclosure
	FormMain->UpdateMetaSprite(true);
	Update();


	}   //isDragging enclosure
	FormManageMetasprites->Caption=formTitle;
	msprList_isDragging=false;
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    msprList_isDragging=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesMouseMove(
      TObject *Sender, TShiftState Shift, int X, int Y)
{
		if(Shift.Contains(ssRight)){

		TPoint pt(X, Y);
		msprList_dropID = ListBoxSprites->ItemAtPos(pt, true);
		ListBoxSprites->Invalidate();
	   }
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesKeyUp(
      TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(msprList_isDragging)
	{
		bool bCtrl		= (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
		bool bAlt		= (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
		if(bCtrl && !bAlt) FormManageMetasprites->Caption="Move";
		   else if(bCtrl && bAlt) FormManageMetasprites->Caption="Clone";
		   else FormManageMetasprites->Caption="Swap";
    ListBoxSprites->Invalidate();
	}
}
//---------------------------------------------------------------------------

