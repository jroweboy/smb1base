//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitPropID.h"
#include "UnitMain.h"
#include "UnitCHRbit.h"
#include "UnitMTprops.h"
#include "UnitMetatileEditor.h"
#include "UnitNavigator.h"
#include "UnitBankCHR.h"
#include "UnitSwapBanks.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPropID *FormPropID;

bool bNotManualTrackChange=false;

extern int mtHoverBtn;
extern int hoverBtn;

extern AnsiString collisionIDlabel[];
extern unsigned char collisionID_R[];
extern unsigned char collisionID_G[];
extern unsigned char collisionID_B[];
bool bCollisionID_editLabelActive=false;
extern bool bColissionIDHover;
extern bool chrSelectRect;
extern unsigned char chrSelected[];
extern int bankActive;
extern TRect chrSelection;
extern unsigned char tileViewTable[];
extern unsigned int bankViewTable[];
extern unsigned char *tileProperties;

extern bool cueUpdateNametable;
extern bool cueUpdateTiles;

extern int iPropIDHoveredItem;

extern char *currentTable_id;
extern char *currentTable_pal;
extern char *currentTable_props;
extern unsigned char *currentMetaSelected;
extern TRect *currentMetaSelection;

extern unsigned char metaSelected_2x2[];
extern unsigned char metaSelected_4x4[];
extern unsigned char metaSelected_8x8[];
extern unsigned char *metatileSet_2x2_id;
extern unsigned char *metatileSet_4x4_id;
extern unsigned char *metatileSet_8x8_id;

extern unsigned char *metatileSet_2x2_pal;
extern unsigned char *metatileSet_4x4_pal;
extern unsigned char *metatileSet_8x8_pal;

extern unsigned char *metatileSet_2x2_props;
extern unsigned char *metatileSet_4x4_props;
extern unsigned char *metatileSet_8x8_props;

//extern uint32_t *mtUsage_2x2;
//extern uint32_t *mtUsage_4x4;
//extern uint32_t *mtUsage_8x8;

extern char *mtContent_2x2;
extern char *mtContent_4x4;
extern char *mtContent_8x8;


extern int mtClickID;
extern unsigned int highlight_mt;
extern int mtClickID_store_2x2;
extern int mtClickID_store_4x4;
extern int mtClickID_store_8x8;
extern bool metaSelectMulti;
extern TRect *currentMetaSelection;

extern TRect metaSelection_2x2;
extern TRect metaSelection_4x4;
extern TRect metaSelection_8x8;
extern bool holdStats;
AnsiString RemoveExt(AnsiString name)
{
	return ChangeFileExt(name,"");
}
//---------------------------------------------------------------------------
__fastcall TFormPropID::TFormPropID(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormPropID::FormCreate(TObject *Sender)
{
	MakeList();
	UpdateUI();
}
//---------------------------------------------------------------------------
void __fastcall TFormPropID::ListBox1DrawItem(TWinControl *Control, int id,
	  TRect &Rect, TOwnerDrawState State)
{
	 TListBox *listBox = dynamic_cast<TListBox*>(Control);

	TColor tmpCol = TColor(
					(((int)collisionID_B[id])<<16)
					|(((int)collisionID_G[id])<<8)
					|(((int)collisionID_R[id])));
	if (listBox)
	{
		listBox->Canvas->Brush->Color = tmpCol;

		// draw the background
		listBox->Canvas->FillRect(Rect);

		//force text to be black
		if((collisionID_R[id]+collisionID_G[id]+collisionID_B[id])/3 > 0x90)
				listBox->Canvas->Font->Color = clBlack;
		else    listBox->Canvas->Font->Color = clWhite;
		//draw the text of the item
		listBox->Canvas->TextOut(Rect.Left + 2, Rect.Top + 0, listBox->Items->Strings[id]);

		
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormPropID::MakeList(void)
{
	ListBox1->Items->Clear();

	for (int i = 0; i < 256; i++)
	{
		ListBox1->Items->Add(IntToHex(i,2)+": "+ collisionIDlabel[i]);
	}
}



void __fastcall TFormPropID::TrackRChange(TObject *Sender)
{
	if (bNotManualTrackChange) return;

	int id=ListBox1->ItemIndex;
	collisionID_R[id]=TrackR->Position;
	ListBox1->Invalidate();
	ListBox1->Update();
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::TrackGChange(TObject *Sender)
{
    if (bNotManualTrackChange) return;

	int id=ListBox1->ItemIndex;
	collisionID_G[id]=TrackG->Position;
	ListBox1->Invalidate();
	ListBox1->Update();
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::TrackBChange(TObject *Sender)
{
	if (bNotManualTrackChange) return;

	int id=ListBox1->ItemIndex;
	collisionID_B[id]=TrackB->Position;
	ListBox1->Invalidate();
	ListBox1->Update();
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::ListBox1Click(TObject *Sender)
{
	int id=ListBox1->ItemIndex;
	bool tmpCtrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
	TrackR->Position=collisionID_R[id];
	TrackG->Position=collisionID_G[id];
	TrackB->Position=collisionID_B[id];
	Edit1->Text=collisionIDlabel[id];

	//toggle bit buttons

	FormCHRbit->UpdateBitButtons(true);
	FormMTprops->UpdateBitButtons_tileClick(true);
	//ctrl? apply to selection.
	if(tmpCtrl){
		if(FormMetatileTool->Visible) SetMTprops(id);
		else						  SetCHRprops(id);
	}
	FormMain->UpdateTiles(true);
	FormMain->UpdateNameTable(-1,-1,true);

	if (FormNavigator!= NULL)FormNavigator->Draw(false,false,false);
	if (FormBankCHR != NULL)if(FormBankCHR->Visible)FormBankCHR->Update();
	if (FormMetatileTool != NULL)FormMetatileTool->UpdateUI(true);     //true=cue, false=direct
	if (FormSwapBanks != NULL) if(FormSwapBanks->Visible) FormSwapBanks->FullUpdate(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::Edit1Click(TObject *Sender)
{
	Edit1->Focused();
	FormMain->EnableDisableTypeConflictShortcuts(true);
	bCollisionID_editLabelActive=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::Edit1KeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	int id=ListBox1->ItemIndex;
	FormMain->EnableDisableTypeConflictShortcuts(true);
	bCollisionID_editLabelActive=true;
	if(Key==VK_RETURN)
	{
		FormMain->SetUndo();
		bCollisionID_editLabelActive=false;
		collisionIDlabel[id]=Edit1->Text;
		ListBox1->Items->Strings[id]=IntToHex(id,2)+": "+collisionIDlabel[id];
		ListBox1->Invalidate();
		ListBox1->Update();
	}
	if (Key==VK_ESCAPE){
		//Key=0;
		Edit1->Text=collisionIDlabel[id];
		bCollisionID_editLabelActive=false;
		//ListBoxSprites->Focused();
		//FormMain->EnableDisableTypeConflictShortcuts(false);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::Edit1Enter(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(true);
   bCollisionID_editLabelActive=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::Edit1Exit(TObject *Sender)
{
	int id=ListBox1->ItemIndex;
	FormMain->EnableDisableTypeConflictShortcuts(false);

	if(bCollisionID_editLabelActive){
		FormMain->SetUndo();
		collisionIDlabel[id]=Edit1->Text;
	}

	bCollisionID_editLabelActive=false;
	//FormPropID->SetFocus();

	ListBox1->Invalidate();
	ListBox1->Update();
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::ListBox1MouseEnter(TObject *Sender)
{
	bColissionIDHover=true;
    FormMain->UpdateAll();
	FormMain->LabelStats->Caption="Hovering highlights chr or metatiles using this ID.\nClicking selects the ID item to edit.\nCtrl-clicking applies this ID to the current selection in a metatile (if metatile tool is open),\n or CHR tile, if the metatile tool isn't open.";
	holdStats=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormPropID::ListBox1MouseLeave(TObject *Sender)
{

	if(btnHold->Down==false)bColissionIDHover=false;
    FormMain->UpdateAll();
	FormMain->LabelStats->Caption="---";

}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::Label1MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Edits the label of the current collision ID item.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::Label2MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Edits the colour code of the currently selected collision ID item.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::btnHoldMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="When down, highlights remain active even after moving the cursor outside the list.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::btnTilesMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="When down, highlights are ON for metatiles, when hovering over the list.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::btnMetasMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="When down, highlights are ON for tiles, when hovering over the list.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::btnSaveMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Saves current list to file.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::btnLoadMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Loads list from file.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::btnLoadClick(TObject *Sender)
{
	OpenDialog1->Filter="All valid (*.idl, *.nss)|*.idl;*.nss;|Collision ID list (*.idl)|*.idl|Session (*.nss)|*.nss|Any (*.*)|*.*";

	OpenDialog1->Title="Open collision ID list";
	if(!OpenDialog1->Execute()) return;

	FormMain->BlockDrawing(true);

	if(FormMain->OpenCollisionIDList(OpenDialog1->FileName)){
		OpenDialog1->FileName=RemoveExt(OpenDialog1->FileName);
	}
	FormMain->BlockDrawing(false);
    MakeList();
	UpdateUI();
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::btnSaveClick(TObject *Sender)
{
	if(!SaveDialog1->Execute()) return;

	FormMain->BlockDrawing(true);

	FormMain->SaveCollisionIDList(SaveDialog1->FileName);

	FormMain->BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::FormClose(TObject *Sender, TCloseAction &Action)
{
   if(!FormMTprops->Visible)mtHoverBtn= -1;
   if(!FormCHRbit->Visible)	hoverBtn = -1;
   bColissionIDHover=false;

   FormMain->UpdateTiles(false);
   //if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
   FormMetatileTool->UpdateUI(true);

   FormMain->EnableDisableTypeConflictShortcuts(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormPropID::UpdateUI(void)
{
  ListBox1->Invalidate();
	ListBox1->Update();
   if(ListBox1->ItemIndex<0) ListBox1->ItemIndex=0;
	int id=ListBox1->ItemIndex;
	bNotManualTrackChange=true;
	TrackR->Position=collisionID_R[id];
	TrackG->Position=collisionID_G[id];
	TrackB->Position=collisionID_B[id];
	bNotManualTrackChange=false;
	Edit1->Text=collisionIDlabel[id];
}
void __fastcall TFormPropID::FormShow(TObject *Sender)
{
	UpdateUI();
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::SetCHRprops(unsigned char id)
{
	int i,j,k,x,y,w,h;
	int set = bankActive/16;
	int index;

	FormMain->SetUndo();

	if(chrSelectRect){
		FormMain->GetSelection(chrSelection,x,y,w,h);

		index=0;

		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				index = bankViewTable[set + x + y*16]/16 + tileViewTable[((x+j) + (y+i)*16)];

				tileProperties[index]= id;
			}
		}
	}
	else{
    	for(i=0;i<256;i++)
		{
			if(chrSelected[i])
			{
				index=bankViewTable[set + i]/16 + (tileViewTable[i]);
				tileProperties[index]= id;
			}
		}
	}

	/*
	index=bankViewTable[set + tileActive]/16 + tileViewTable[tileActive];
	int tmp = tileProperties[index];
	AnsiString hexString = IntToHex(tmp, 2);
	MaskEdit1->Text=hexString;

	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
	*/
}

//---------------------------------------------------------------------------

void __fastcall TFormPropID::SetMTprops(unsigned char id)
{
	//note: disabled a longtime warning for TopLeftID by commenting it out. However,
	//i do not remember if it was intended to connect to something useful later
    //i have left it commented out, in case i remember what it could be good for. 
	int i,j,k,x,y,w,h;
	int index;

	FormMain->SetUndo();

	bool bTab2x2 = FormMetatileTool->PageControl1->ActivePage==FormMetatileTool->TabSheet2x2?true:false;
	bool bTab4x4 = FormMetatileTool->PageControl1->ActivePage==FormMetatileTool->TabSheet4x4?true:false;
	bool bTab8x8 = FormMetatileTool->PageControl1->ActivePage==FormMetatileTool->TabSheet8x8?true:false;
	int d;

	if(bTab2x2)
	{
		d=2;

		currentTable_id = metatileSet_2x2_id;
		currentTable_pal = metatileSet_2x2_pal;
		currentTable_props = metatileSet_2x2_props;
		currentMetaSelected = metaSelected_2x2;
		currentMetaSelection = &metaSelection_2x2;
	}
	if(bTab4x4)
	{
		d=4;

		currentTable_id = metatileSet_4x4_id;
		currentTable_pal = metatileSet_4x4_pal;
		currentTable_props = metatileSet_4x4_props;
		currentMetaSelected = metaSelected_4x4;
		currentMetaSelection = &metaSelection_4x4;
	}
	if(bTab8x8)
	{
		d=8;

		currentTable_id = metatileSet_8x8_id;
		currentTable_pal = metatileSet_8x8_pal;
		currentTable_props = metatileSet_8x8_props;
		currentMetaSelected = metaSelected_8x8;
		currentMetaSelection = &metaSelection_8x8;
	}
	//int topLeftID; //provides a hex representation of the value of the top left cell.

	if(!metaSelectMulti)  //box selection
	{
		if(currentMetaSelection->left>=0 && currentMetaSelection->top>=0)
		{
			FormMain->GetSelection(*currentMetaSelection,x,y,w,h);
		}
		else   //no selection? do all places.
		{
			x=0; y=0;
			w=d; h=d;

		}

		index=0;
		//topLeftID = mtClickID*d*d + (x) + (y)*d;

        for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				index = mtClickID*d*d + (x+j) + (y+i)*d;
				currentTable_props[index] = id;
       		}
		}
	}
	else    //multi select
	{
    	bool firstMatch=true;

		for(i=0;i<d*d;i++)
		{
			if(currentMetaSelected[i])
			{
				if(firstMatch){
					//topLeftID = mtClickID*d*d + i;
					firstMatch=false;
				}
				index=mtClickID*d*d + i;
			}
		}
	}

}
//---------------------------------------------------------------------------


void __fastcall TFormPropID::ListBox1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	int iPropIDHoveredItem = ListBox1->ItemAtPos(Point(X, Y), true);
	if (iPropIDHoveredItem<0) iPropIDHoveredItem=ListBox1->ItemIndex;

	if(btnHov->Down){
		cueUpdateNametable=true;
		cueUpdateTiles=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormPropID::btnHovClick(TObject *Sender)
{
	if (iPropIDHoveredItem<0) iPropIDHoveredItem=ListBox1->ItemIndex;

	if(btnHov->Down){
		cueUpdateNametable=true;
		cueUpdateTiles=true;
	}
}
//---------------------------------------------------------------------------

