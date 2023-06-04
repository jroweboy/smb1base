//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitCHREditor.h"
#include "UnitManageMetasprites.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormManageMetasprites *FormManageMetasprites;

extern AnsiString metaSpriteBankName;
extern AnsiString metaSpriteNames[256];
extern AnsiString metaSpriteNamesUndo[256];
extern AnsiString tmpMetaSprName;
extern int metaSpriteActive;
extern unsigned char metaSprites[];
extern bool prefStartShowMM;
extern bool cueUpdateMM;
extern bool bSnapToScreen;
void __fastcall TFormManageMetasprites::Update(void)
{
	AnsiString str;
	int i,j,cnt;

	ListBoxSprites->ItemIndex=metaSpriteActive;
	//cueUpdateMM=false;
	for(i=0;i<256;++i)
	{
		str=IntToStr(i)+": "+metaSpriteNames[i]+" | ";

		cnt=0;

		for(j=0;j<64*4;j+=4)
		{
			if(metaSprites[i*64*4+j]<255) ++cnt;
		}

		if(!cnt) str+="empty"; else str+=IntToStr(cnt)+" sprites";

		ListBoxSprites->Items->Strings[i]=str;
	}
	MetaLabel->Text=metaSpriteNames[metaSpriteActive];
	FormMain->DrawMetaSprite(ImageMetaSprite,metaSpriteActive,1,true,false,true);
}

void __fastcall TFormManageMetasprites::UpdateActive(void)
{
   AnsiString str;
   int i;
   int cnt=0;
   MetaLabel->Text=metaSpriteNames[metaSpriteActive];
   FormMain->DrawMetaSprite(ImageMetaSprite,metaSpriteActive,1,true,false,true);

   //update listing
   ListBoxSprites->ItemIndex=metaSpriteActive;
   str=IntToStr(metaSpriteActive)+": "+metaSpriteNames[metaSpriteActive]+" | ";
   for(i=0;i<64*4;i+=4)
		{
			if(metaSprites[metaSpriteActive*64*4+i]<255) ++cnt;
		}
  if(!cnt) str+="empty"; else str+=IntToStr(cnt)+" sprites";
  ListBoxSprites->Items->Strings[metaSpriteActive]=str;

}


//---------------------------------------------------------------------------
__fastcall TFormManageMetasprites::TFormManageMetasprites(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::FormCreate(TObject *Sender)
{
	FormManageMetasprites->Left=(Screen->Width+FormMain->Width)/2;
	FormManageMetasprites->Top=(Screen->Height-FormMain->Height)/2+FormCHREditor->Height;
	ImageMetaSprite->Picture=new TPicture();
	ImageMetaSprite->Picture->Bitmap=new Graphics::TBitmap();
	ImageMetaSprite->Picture->Bitmap->PixelFormat=pf24bit;
	ImageMetaSprite->Picture->Bitmap->SetSize(ImageMetaSprite->Width,ImageMetaSprite->Height);

	DoubleBuffered=true;
	ListBoxSprites->DoubleBuffered=true;

	Update();
	if(prefStartShowMM==true) FormManageMetasprites->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::FormShow(TObject *Sender)
{
	Update();
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::ListBoxSpritesClick(TObject *Sender)
{
	metaSpriteActive=ListBoxSprites->ItemIndex;

	FormMain->UpdateMetaSprite();

	//Update();
	UpdateActive();
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::SpeedButtonInsertClick(TObject *Sender)
{
	int i,id;

	FormMain->SetUndo();
	
	id=ListBoxSprites->ItemIndex;

	for(i=255;i>id;--i)
	{
		memcpy(&metaSprites[i*64*4],&metaSprites[(i-1)*64*4],64*4);
		if(CheckMoveName->Checked){
			metaSpriteNames[i]	= metaSpriteNames[i-1];
		}
	}

	memset(&metaSprites[id*64*4],255,64*4);
	if(CheckMoveName->Checked){
		metaSpriteNames[id] = "unnamed";
	}

	if(metaSpriteActive<255) ++metaSpriteActive;

	FormMain->UpdateMetaSprite();
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
		memcpy(&metaSprites[i*64*4],&metaSprites[(i+1)*64*4],64*4);
        if(CheckMoveName->Checked){
			metaSpriteNames[i]	= metaSpriteNames[i+1];
		}
	}

	memset(&metaSprites[255*64*4],255,64*4);

	FormMain->UpdateMetaSprite();
	Update();
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::SpeedButtonMoveUpClick(TObject *Sender)
{
	int i,id;
	unsigned char temp[64*4];





	id=ListBoxSprites->ItemIndex;

	if(id<1) return;

	FormMain->SetUndo();
	
	//data
	memcpy(&temp,&metaSprites[(id-1)*64*4],64*4);
	memcpy(&metaSprites[(id-1)*64*4],&metaSprites[id*64*4],64*4);
	memcpy(&metaSprites[id*64*4],&temp,64*4);

	//name
	if(CheckMoveName->Checked){
		tmpMetaSprName 			= metaSpriteNames[id-1];
		metaSpriteNames[id-1]   = metaSpriteNames[id];
		metaSpriteNames[id]		= tmpMetaSprName;
    }

	UpdateActive();
	--metaSpriteActive;

	FormMain->UpdateMetaSprite();
	UpdateActive();
}
//---------------------------------------------------------------------------
void __fastcall TFormManageMetasprites::SpeedButtonMoveDownClick(
TObject *Sender)
{
	int i,id;
	unsigned char temp[64*4];

	id=ListBoxSprites->ItemIndex;

	if(id>=255) return;

	FormMain->SetUndo();
	
	memcpy(&temp,&metaSprites[(id+1)*64*4],64*4);
	memcpy(&metaSprites[(id+1)*64*4],&metaSprites[id*64*4],64*4);
	memcpy(&metaSprites[id*64*4],&temp,64*4);

	//name
	if(CheckMoveName->Checked){
		tmpMetaSprName 			= metaSpriteNames[id+1];
		metaSpriteNames[id+1]   = metaSpriteNames[id];
		metaSpriteNames[id]		= tmpMetaSprName;
	}

	UpdateActive();
	++metaSpriteActive;

	FormMain->UpdateMetaSprite();
	UpdateActive();
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
		memcpy(&metaSprites[i*64*4],&metaSprites[(i-1)*64*4],64*4);
		if(CheckMoveName->Checked){
			metaSpriteNames[i]	= metaSpriteNames[i-1];
		}
	}
    int count = 0;
	AnsiString delimiter = "__";
	if(CheckMoveName->Checked){
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
	if(metaSpriteActive<255) ++metaSpriteActive;

	FormMain->UpdateMetaSprite();
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
	FormManageMetasprites->ScreenSnap=bSnapToScreen;
}
//---------------------------------------------------------------------------


void __fastcall TFormManageMetasprites::MetaLabelKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    FormMain->EnableDisableTypeConflictShortcuts(true);

	if(Key==VK_RETURN)
	{
		//Key=0;
		metaSpriteNames[metaSpriteActive]=MetaLabel->Text;
		UpdateActive();
		//ListBoxSprites->Focused();
		//FormMain->EnableDisableTypeConflictShortcuts(false);
	}
	if (Key==VK_ESCAPE){
		//Key=0;
		MetaLabel->Text=metaSpriteNames[metaSpriteActive];
		//ListBoxSprites->Focused();
		//FormMain->EnableDisableTypeConflictShortcuts(false);
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::MetaLabelClick(TObject *Sender)
{
	MetaLabel->Focused();
    FormMain->EnableDisableTypeConflictShortcuts(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::MetaLabelEnter(TObject *Sender)
{
   FormMain->EnableDisableTypeConflictShortcuts(true);

}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::MetaLabelExit(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(false);
	FormManageMetasprites->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::FormDeactivate(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	FormMain->EnableDisableTypeConflictShortcuts(false);	
}
//---------------------------------------------------------------------------

void __fastcall TFormManageMetasprites::ListBoxSpritesDblClick(TObject *Sender)
{
		FormMain->PageControlEditor->ActivePageIndex=1;
		FormMain->PageControlEditorChange(Sender);
}
//---------------------------------------------------------------------------


