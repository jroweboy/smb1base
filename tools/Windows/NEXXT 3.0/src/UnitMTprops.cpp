//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "UnitMain.h"
#include "UnitMTprops.h"
#include "UnitMetatileEditor.h"
#include "UnitName.h"
#include "UnitPropertyConditions.h"
#include "UnitPropID.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMTprops *FormMTprops;
 extern AnsiString propCHRlabel[];
extern bool bKeyEscape;
extern bool holdStats;

extern int mtPropsActive;

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

extern uint32_t *mtUsage_2x2;
extern uint32_t *mtUsage_4x4;
extern uint32_t *mtUsage_8x8;

extern uint32_t *mtContent_2x2;
extern uint32_t *mtContent_4x4;
extern uint32_t *mtContent_8x8;


extern int mtClickID;
extern unsigned int highlight_mt;
extern int mtClickID_store_2x2;
extern int mtClickID_store_4x4;
extern int mtClickID_store_8x8;
extern bool metaSelectMulti;
int mtHoverBtn= -1;
extern bool collision_specific;
extern unsigned char *currentTable_id;
extern unsigned char *currentTable_pal;
extern unsigned char *currentTable_props;
extern unsigned char *currentMetaSelected;

extern bool btnStateMtProps[8];
extern TRect *currentMetaSelection;

extern TRect metaSelection_2x2;
extern TRect metaSelection_4x4;
extern TRect metaSelection_8x8;

extern unsigned char metaSelected_2x2[];
extern unsigned char metaSelected_4x4[];
extern unsigned char metaSelected_8x8[];

extern int MetaCollisionGranularityX;
extern int MetaCollisionGranularityY;

extern bool bMTpropHover;
extern bool bMTuseDirectTile;
bool IsHexDigit(char c) {
	return ((c >= '0' && c <= '9') ||
			(c >= 'A' && c <= 'F') ||
			(c >= 'a' && c <= 'f'));
}

//---------------------------------------------------------------------------
__fastcall TFormMTprops::TFormMTprops(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::HexToClip(void)
{
	if (OpenClipboard(Handle)) {
		EmptyClipboard();
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (MaskEdit1->Text.Length() + 1) * sizeof(char));
		if (hGlobal != NULL) {
			char* pClipboardText = (char*)GlobalLock(hGlobal);

			if (pClipboardText != NULL) {
				strcpy(pClipboardText, MaskEdit1->Text.c_str());
				GlobalUnlock(hGlobal);
				SetClipboardData(CF_TEXT, hGlobal);
			}
		}
		CloseClipboard();
	}
}

//---------------------------------------------------------------------------

void __fastcall TFormMTprops::ClipToHexEdit(void) {
	if (OpenClipboard(0)) {
		HGLOBAL hData = GetClipboardData(CF_TEXT);
		if (hData != NULL) {
			char* clipboardText = static_cast<char*>(GlobalLock(hData));
			if (clipboardText != NULL) {
				String str = clipboardText;
				GlobalUnlock(hData);
                
				if (str.Length() >= 1) {
					String hexChars = str.SubString(1, 2);

                    bool isHex = true;
                    for (int i = 1; i <= hexChars.Length(); i++) {
                        if (!IsHexDigit(hexChars[i])) {
                            isHex = false;
							break;
                        }
					}

					if (isHex) {
						MaskEdit1->Text = hexChars;
					}
				}
			}
		}

		CloseClipboard();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn0labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit0 label";
	FormName->EditName->Text=btn0label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn0label->Caption=FormName->EditName->Text;
	propCHRlabel[0]=btn0label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::PaintBox1Paint(TObject *Sender)
{
	TCanvas *Canvas = PaintBox1->Canvas;
	String Text = "hex";
	int TextWidth = Canvas->TextWidth(Text);
	int TextHeight = Canvas->TextHeight(Text);

	Canvas->Font->Orientation = -900; // Rotate the font by 90 degrees
	Canvas->TextOut(PaintBox1->Height+5 - TextHeight, PaintBox1->Width+6 - TextWidth, Text);
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn1labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit1 label";
	FormName->EditName->Text=btn1label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn1label->Caption=FormName->EditName->Text;
	propCHRlabel[1]=btn1label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn2labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit2 label";
	FormName->EditName->Text=btn2label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn2label->Caption=FormName->EditName->Text;
	propCHRlabel[2]=btn2label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn3labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit3 label";
	FormName->EditName->Text=btn3label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn3label->Caption=FormName->EditName->Text;
	propCHRlabel[3]=btn3label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn4labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit4 label";
	FormName->EditName->Text=btn4label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn4label->Caption=FormName->EditName->Text;
	propCHRlabel[4]=btn4label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn5labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit5 label";
	FormName->EditName->Text=btn5label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn5label->Caption=FormName->EditName->Text;
	propCHRlabel[5]=btn5label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn6labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit6 label";
	FormName->EditName->Text=btn6label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn6label->Caption=FormName->EditName->Text;
	propCHRlabel[6]=btn6label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn7labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit7 label";
	FormName->EditName->Text=btn7label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn7label->Caption=FormName->EditName->Text;
	propCHRlabel[7]=btn7label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn0MouseEnter(TObject *Sender)
{
	bMTpropHover=true;

	if(Sender==btn0||Sender==btn0label||Sender==btnC0) mtHoverBtn=0;
	if(Sender==btn1||Sender==btn1label||Sender==btnC1) mtHoverBtn=1;
	if(Sender==btn2||Sender==btn2label||Sender==btnC2) mtHoverBtn=2;
	if(Sender==btn3||Sender==btn3label||Sender==btnC3) mtHoverBtn=3;

	if(Sender==btn4||Sender==btn4label||Sender==btnC4) mtHoverBtn=4;
	if(Sender==btn5||Sender==btn5label||Sender==btnC5) mtHoverBtn=5;
	if(Sender==btn6||Sender==btn6label||Sender==btnC6) mtHoverBtn=6;
	if(Sender==btn7||Sender==btn7label||Sender==btnC7) mtHoverBtn=7;

	FormMain->UpdateTiles(false);
	//if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if((FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn0MouseLeave(TObject *Sender)
{
	 if(btnHold->Down==false){
		mtHoverBtn= -1;

		FormMain->UpdateTiles(false);
		//if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
		if((FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
   }
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn0MouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
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




	//int set = bankActive/16;
	int tag=((TSpeedButton*)Sender)->Tag;

	if (Button == mbRight)
	{
		metaSelectMulti=true;

		for(int i;i<d*d;i++)
		{
			//TODO: test this
			int index = mtClickID*(d*d)+i;
			int mask = 1 << tag;
			int bit = (currentTable_props[index] & mask);// >> tag;
			currentMetaSelected[i] = bit>0? true:false;
		}
	}

	FormMain->UpdateTiles(false);
	//if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if((FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn0Click(TObject *Sender)
{
	int i,j,k,x,y,w,h;
	//int set = bankActive/16;
	int index;
	bool tmpCtrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
	bool tmpShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000);

	bool forceDown = (tmpCtrl) && (!tmpShift);
	bool forceUp = (tmpCtrl) && (tmpShift);

	if(forceDown) ((TSpeedButton*)Sender)->Down=true;
	if(forceUp)  ((TSpeedButton*)Sender)->Down=false;

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

	btnStateMtProps[0] = btn0->Down;
	btnStateMtProps[1] = btn1->Down;
	btnStateMtProps[2] = btn2->Down;
	btnStateMtProps[3] = btn3->Down;
	btnStateMtProps[4] = btn4->Down;
	btnStateMtProps[5] = btn5->Down;
	btnStateMtProps[6] = btn6->Down;
	btnStateMtProps[7] = btn7->Down;

	int topLeftID; //provides a hex representation of the value of the top left cell.

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
		topLeftID = mtClickID*d*d + (x) + (y)*d;

		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				index = mtClickID*d*d + (x+j) + (y+i)*d;

				if(Sender==btn0){

					if (btnStateMtProps[0]) {currentTable_props[index] |= (1 << 0);
					} else {currentTable_props[index] &= ~(1 << 0);}}
				if(Sender==btn1){
					if (btnStateMtProps[1]) {currentTable_props[index] |= (1 << 1);
					} else {currentTable_props[index] &= ~(1 << 1);}}
				if(Sender==btn2){
					if (btnStateMtProps[2]) {currentTable_props[index] |= (1 << 2);
					} else {currentTable_props[index] &= ~(1 << 2);}}
				if(Sender==btn3){
					if (btnStateMtProps[3]) {currentTable_props[index] |= (1 << 3);
					} else {currentTable_props[index] &= ~(1 << 3);}}
				if(Sender==btn4){
					if (btnStateMtProps[4]) {currentTable_props[index] |= (1 << 4);
					} else {currentTable_props[index] &= ~(1 << 4);}}
				if(Sender==btn5){
					if (btnStateMtProps[5]) {currentTable_props[index] |= (1 << 5);
					} else {currentTable_props[index] &= ~(1 << 5);}}
				if(Sender==btn6){
					if (btnStateMtProps[6]) {currentTable_props[index] |= (1 << 6);
					} else {currentTable_props[index] &= ~(1 << 6);}}
				if(Sender==btn7){
					if (btnStateMtProps[7]) {currentTable_props[index] |= (1 << 7);
					} else {currentTable_props[index] &= ~(1 << 7);}}
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
					topLeftID = mtClickID*d*d + i;
					firstMatch=false;
				}
				index=mtClickID*d*d + i;

					if(Sender==btn0){
					if (btnStateMtProps[0]) {currentTable_props[index] |= (1 << 0);
					} else {currentTable_props[index] &= ~(1 << 0);}}
				if(Sender==btn1){
					if (btnStateMtProps[1]) {currentTable_props[index] |= (1 << 1);
					} else {currentTable_props[index] &= ~(1 << 1);}}
				if(Sender==btn2){
					if (btnStateMtProps[2]) {currentTable_props[index] |= (1 << 2);
					} else {currentTable_props[index] &= ~(1 << 2);}}
				if(Sender==btn3){
					if (btnStateMtProps[3]) {currentTable_props[index] |= (1 << 3);
					} else {currentTable_props[index] &= ~(1 << 3);}}
				if(Sender==btn4){
					if (btnStateMtProps[4]) {currentTable_props[index] |= (1 << 4);
					} else {currentTable_props[index] &= ~(1 << 4);}}
				if(Sender==btn5){
					if (btnStateMtProps[5]) {currentTable_props[index] |= (1 << 5);
					} else {currentTable_props[index] &= ~(1 << 5);}}
				if(Sender==btn6){
					if (btnStateMtProps[6]) {currentTable_props[index] |= (1 << 6);
					} else {currentTable_props[index] &= ~(1 << 6);}}
				if(Sender==btn7){
					if (btnStateMtProps[7]) {currentTable_props[index] |= (1 << 7);
					} else {currentTable_props[index] &= ~(1 << 7);}}

			}
		}
	}



	int tmp = currentTable_props[topLeftID];
	tmp = tmp & 0xFF;
	mtPropsActive = tmp;
	AnsiString hexString = IntToHex(tmp, 2);
	MaskEdit1->Text=hexString;

	//FormMain->UpdateTiles(false);
	//if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if((FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn1x1metaClick(TObject *Sender)
{
	bMTuseDirectTile=btnDirect->Down;
	MetaCollisionGranularityX=1;
	MetaCollisionGranularityY=1;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn2x1metaClick(TObject *Sender)
{
	bMTuseDirectTile=btnDirect->Down;
	MetaCollisionGranularityX=2;
	MetaCollisionGranularityY=1;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn2x2metaClick(TObject *Sender)
{
	bMTuseDirectTile=btnDirect->Down;
	MetaCollisionGranularityX=2;
	MetaCollisionGranularityY=2;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn4x2metaClick(TObject *Sender)
{
	bMTuseDirectTile=btnDirect->Down;
	MetaCollisionGranularityX=4;
	MetaCollisionGranularityY=2;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::btn4x4metaClick(TObject *Sender)
{
	bMTuseDirectTile=btnDirect->Down;
	MetaCollisionGranularityX=4;
	MetaCollisionGranularityY=4;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormMTprops::FormClose(TObject *Sender, TCloseAction &Action)
{
   mtHoverBtn= -1;
   //if(btnShowCHR->Down) FormMain->UpdateTiles(false);
   //if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
   FormMetatileTool->UpdateUI(true);
   FormMain->EnableDisableTypeConflictShortcuts(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::FormDeactivate(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::UpdateBitButtons_tileClick(bool bID_listClick)
{
	int i,j,k,x,y,w,h;

	extern int bankActive;
	extern bool chrSelectRect;
	extern TRect chrSelection;
    extern unsigned int bankViewTable[];
	extern unsigned char tileViewTable[];
	//extern uint32_t tileProperties[];
	extern unsigned char *tileProperties;
	extern int tileActive;
	extern unsigned char chrSelected[];
	//extern bool btnStateProps[8];
	extern char propConditional[];

	int index;
	int set = bankActive/16;

	//set buttons according to tileActive
	if(chrSelectRect)
	{
		FormMain->GetSelection(chrSelection,x,y,w,h);
		index = bankViewTable[set+x+y*16]/16 + tileViewTable[x + y*16];


	}
	else
	{
		index=bankViewTable[set+tileActive]/16 + tileActive;

	}

	for(k=0;k<8;k++)
	{
		if(bID_listClick) btnStateMtProps[k]= (FormPropID->ListBox1->ItemIndex & (1 << k)) != 0;

		else btnStateMtProps[k]= (tileProperties[index] & (1 << k)) != 0;

	}

	btn0->Down=   btnStateMtProps[0];
	btn1->Down=   btnStateMtProps[1];
	btn2->Down=   btnStateMtProps[2];
	btn3->Down=   btnStateMtProps[3];
	btn4->Down=   btnStateMtProps[4];
	btn5->Down=   btnStateMtProps[5];
	btn6->Down=   btnStateMtProps[6];
	btn7->Down=   btnStateMtProps[7];

	//find inconsistencies in selection and flag buttons accordingly.
	bool bitDiff[8]={false};
	unsigned int bit;
	int mask;
	i=0;
	//j=0;
	if(chrSelectRect)
	{
		FormMain->GetSelection(chrSelection,x,y,w,h);
		if(w<2 && h<2) goto skip;

		for (int k = 0; k < 8; k++) {
		bool rowDiff[16] = {false};
		// Loop over each row in the region
		for (int j = 0; j < h; j++) {
			// Loop over each tile in the row
			for (int i = 0; i < w; i++) {
				int mask = 1 << k;
				int index = bankViewTable[set+ (x+i) + (y+j)*16]/16 + tileViewTable[(x+i) + (y+j)*16];
				int bit = (tileProperties[index] & mask) >> k;

				// Check if the current tile's bit value is different from the bit value of the previous tile in this row
				if (i > 0) {
					int previousIndex = bankViewTable[set+(x+i-1) + (y+j)*16]/16 + tileViewTable[(x+i-1) + (y+j)*16];
					int previousBit = (tileProperties[previousIndex] & mask) >> k;
					if (bit != previousBit) {
						rowDiff[i-1] = true;
						rowDiff[i] = true;
					}
				}
			}
		}
		// Combine the row differences using a bitwise OR operation
		bool diff = false;
		for (int i = 0; i < w-1; i++) {
			diff = diff || rowDiff[i];
		}
		bitDiff[k] = diff;
	}

	}
	else
	{
		int previousIndex = bankViewTable[set+tileActive]/16 + tileActive;
		for(i=0;i<256;i++)
			{

				if(chrSelected[i])
				{
					for (int k = 0; k < 8; k++) {
						int index = bankViewTable[set+i]/16 + tileViewTable[i];
						int mask = 1 << k;
						int bit = (tileProperties[index] & mask) >> k;
						int previousBit = (tileProperties[previousIndex] & mask) >> k;

						if (bit != previousBit) bitDiff[k] = true;
					}
					previousIndex = i; // Update previousIndex to the current selected char index
				}
			}
	}
	skip:
	btn0->Caption=   bitDiff[0]?	"0(!)":"0"	;
	btn1->Caption=   bitDiff[1]?	"1(!)":"1"	;
	btn2->Caption=   bitDiff[2]?	"2(!)":"2"	;
	btn3->Caption=   bitDiff[3]?	"3(!)":"3"	;
	btn4->Caption=   bitDiff[4]?	"4(!)":"4"	;
	btn5->Caption=   bitDiff[5]?	"5(!)":"5"	;
	btn6->Caption=   bitDiff[6]?	"6(!)":"6"	;
	btn7->Caption=   bitDiff[7]?	"7(!)":"7"	;


	AnsiString str;
	for(i=0;i<8;i++){
		switch (propConditional[i])
		{
			case 1:
				str="yes";
				break;
			case 2:
				str="all";
				break;
			default:
				str="no";
				break;
		}
		if(i==0) btnC0->Caption=str;
		if(i==1) btnC1->Caption=str;
		if(i==2) btnC2->Caption=str;
		if(i==3) btnC3->Caption=str;

		if(i==4) btnC4->Caption=str;
		if(i==5) btnC5->Caption=str;
		if(i==6) btnC6->Caption=str;
		if(i==7) btnC7->Caption=str;

   }

   btn0label->Caption=propCHRlabel[0];
   btn1label->Caption=propCHRlabel[1];
   btn2label->Caption=propCHRlabel[2];
   btn3label->Caption=propCHRlabel[3];
   btn4label->Caption=propCHRlabel[4];
   btn5label->Caption=propCHRlabel[5];
   btn6label->Caption=propCHRlabel[6];
   btn7label->Caption=propCHRlabel[7];


   if(FormPropConditions->Visible) FormPropConditions->SetConditions();

}

//---------------------------------------------------------------------------

void __fastcall TFormMTprops::UpdateBitButtons_metasetClick(void)
{
	int i,j,k,x,y,w,h;
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

	extern int bankActive;
	extern bool chrSelectRect;
	extern TRect chrSelection;
	extern unsigned char tileViewTable[];
	extern unsigned int bankViewTable[];
	//extern uint32_t tileProperties[];
    extern unsigned char *tileProperties;
	extern int tileActive;
	extern unsigned char chrSelected[];
	//extern bool btnStateProps[8];
	extern char propConditional[];
	int index = mtClickID*d*d;

	for(k=0;k<8;k++)
	{
		btnStateMtProps[k]= (currentTable_props[index] & (1 << k)) != 0;
	}

	btn0->Down=   btnStateMtProps[0];
	btn1->Down=   btnStateMtProps[1];
	btn2->Down=   btnStateMtProps[2];
	btn3->Down=   btnStateMtProps[3];
	btn4->Down=   btnStateMtProps[4];
	btn5->Down=   btnStateMtProps[5];
	btn6->Down=   btnStateMtProps[6];
	btn7->Down=   btnStateMtProps[7];

	//find inconsistencies in selection and flag buttons accordingly.
	bool bitDiff[8]={false};
	unsigned int bit;
	int mask;



	for (int k = 0; k < 8; k++) {
		bool rowDiff[64] = {false};
			for (int i = 0; i < d*d; i++) {
				int mask = 1 << k;
				int bit = (currentTable_props[i] & mask) >> k;

				// Check if the current tile's bit value is different from the bit value of the previous tile in this row
				if (i > 0) {
					int previousBit = (currentTable_props[i-1] & mask) >> k;
					if (bit != previousBit) {
						rowDiff[i-1] = true;
						rowDiff[i] = true;
				}
			}
		}
		bool diff = false;
			for (int i = 0; i < d*d-1; i++) {
			diff = diff || rowDiff[i];
		}
		bitDiff[k] = diff;
	}





	skip:
	btn0->Caption=   bitDiff[0]?	"0(!)":"0"	;
	btn1->Caption=   bitDiff[1]?	"1(!)":"1"	;
	btn2->Caption=   bitDiff[2]?	"2(!)":"2"	;
	btn3->Caption=   bitDiff[3]?	"3(!)":"3"	;
	btn4->Caption=   bitDiff[4]?	"4(!)":"4"	;
	btn5->Caption=   bitDiff[5]?	"5(!)":"5"	;
	btn6->Caption=   bitDiff[6]?	"6(!)":"6"	;
	btn7->Caption=   bitDiff[7]?	"7(!)":"7"	;


	AnsiString str;
	for(i=0;i<8;i++){
		switch (propConditional[i])
		{
			case 1:
				str="yes";
				break;
			case 2:
				str="all";
				break;
			default:
				str="no";
				break;
		}
		if(i==0) btnC0->Caption=str;
		if(i==1) btnC1->Caption=str;
		if(i==2) btnC2->Caption=str;
		if(i==3) btnC3->Caption=str;

		if(i==4) btnC4->Caption=str;
		if(i==5) btnC5->Caption=str;
		if(i==6) btnC6->Caption=str;
		if(i==7) btnC7->Caption=str;

   }

   btn0label->Caption=propCHRlabel[0];
   btn1label->Caption=propCHRlabel[1];
   btn2label->Caption=propCHRlabel[2];
   btn3label->Caption=propCHRlabel[3];
   btn4label->Caption=propCHRlabel[4];
   btn5label->Caption=propCHRlabel[5];
   btn6label->Caption=propCHRlabel[6];
   btn7label->Caption=propCHRlabel[7];


   if(FormPropConditions->Visible) FormPropConditions->SetConditions();

}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::UpdateBitButtons_metatileClick(void)
{
   //todo
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::btnC0MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	extern int conditionClicked;
	extern char propConditional[];

	conditionClicked=((TSpeedButton*)Sender)->Tag;
	if (Button == mbLeft)
	{
		FormMain->SetUndo();
		propConditional[conditionClicked]++;
		if(propConditional[conditionClicked] > 2) propConditional[conditionClicked]=0;

		switch (propConditional[conditionClicked])
		{
			case 1:
				((TSpeedButton*)Sender)->Caption="yes";
				break;
			case 2:
				((TSpeedButton*)Sender)->Caption="all";
				break;
			default:
				((TSpeedButton*)Sender)->Caption="no";
				break;
		}
	}
	FormPropConditions->SetConditions();
	FormPropConditions->Show();
	//if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	//if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if((FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMTprops::MaskEdit1MouseEnter(TObject *Sender)
{
	collision_specific=true;
	mtHoverBtn=0; //just a non-negative value between 0...7, doesn't matter
	//if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	//if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if((FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::MaskEdit1MouseLeave(TObject *Sender)
{
	collision_specific=false;	
	//if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	//if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if((FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::MaskEdit1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    /*
	int set = bankActive/16;
	unsigned int mask = StrToIntDef("0x" + MaskEdit1->Text, 0);

	if (Button == mbRight)
	{

		chrSelectRect=false;

		for(int i;i<256;i++)
		{
			int index = set + tileViewTable[i];
			chrSelected[tileViewTable[i]] = tileProperties[index]==mask? true:false;

		}
	}
	*/
   //	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
   //	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if((FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::MaskEdit1Enter(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::MaskEdit1Exit(TObject *Sender)
{
	bool bTab2x2 = FormMetatileTool->PageControl1->ActivePage==FormMetatileTool->TabSheet2x2?true:false;
	bool bTab4x4 = FormMetatileTool->PageControl1->ActivePage==FormMetatileTool->TabSheet4x4?true:false;
	bool bTab8x8 = FormMetatileTool->PageControl1->ActivePage==FormMetatileTool->TabSheet8x8?true:false;
	int d;
	int tmp;
	int x,y,w,h;
	int i,j;
	int index;
	if(bTab2x2){
		d=2;
		currentTable_props = metatileSet_2x2_props;
		currentMetaSelection = &metaSelection_2x2;
	}
	if(bTab4x4){
		d=4;
		currentTable_props = metatileSet_4x4_props;
		currentMetaSelection = &metaSelection_4x4;
	}
	if(bTab8x8){
		d=8;
		currentTable_props = metatileSet_8x8_props;
		currentMetaSelection = &metaSelection_8x8;
	}




	//extern bool btnStateProps[8];
   //int set = bankActive/16;

	tmp = StrToIntDef("0x" + MaskEdit1->Text, 0);

	btn7->Down = (tmp & 0x80) != 0;
	btn6->Down = (tmp & 0x40) != 0;
	btn5->Down = (tmp & 0x20) != 0;
	btn4->Down = (tmp & 0x10) != 0;
	btn3->Down = (tmp & 0x08) != 0;
	btn2->Down = (tmp & 0x04) != 0;
	btn1->Down = (tmp & 0x02) != 0;
	btn0->Down = (tmp & 0x01) != 0;

	btnStateMtProps[0] = btn0->Down;
	btnStateMtProps[1] = btn1->Down;
	btnStateMtProps[2] = btn2->Down;
	btnStateMtProps[3] = btn3->Down;
	btnStateMtProps[4] = btn4->Down;
	btnStateMtProps[5] = btn5->Down;
	btnStateMtProps[6] = btn6->Down;
	btnStateMtProps[7] = btn7->Down;

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

				if(Sender==btn0){

					if (btnStateMtProps[0]) {currentTable_props[index] |= (1 << 0);
					} else {currentTable_props[index] &= ~(1 << 0);}}
				if(Sender==btn1){
					if (btnStateMtProps[1]) {currentTable_props[index] |= (1 << 1);
					} else {currentTable_props[index] &= ~(1 << 1);}}
				if(Sender==btn2){
					if (btnStateMtProps[2]) {currentTable_props[index] |= (1 << 2);
					} else {currentTable_props[index] &= ~(1 << 2);}}
				if(Sender==btn3){
					if (btnStateMtProps[3]) {currentTable_props[index] |= (1 << 3);
					} else {currentTable_props[index] &= ~(1 << 3);}}
				if(Sender==btn4){
					if (btnStateMtProps[4]) {currentTable_props[index] |= (1 << 4);
					} else {currentTable_props[index] &= ~(1 << 4);}}
				if(Sender==btn5){
					if (btnStateMtProps[5]) {currentTable_props[index] |= (1 << 5);
					} else {currentTable_props[index] &= ~(1 << 5);}}
				if(Sender==btn6){
					if (btnStateMtProps[6]) {currentTable_props[index] |= (1 << 6);
					} else {currentTable_props[index] &= ~(1 << 6);}}
				if(Sender==btn7){
					if (btnStateMtProps[7]) {currentTable_props[index] |= (1 << 7);
					} else {currentTable_props[index] &= ~(1 << 7);}}
			}
		}
	}
	else    //multi select
	{
		//bool firstMatch=true;

		for(i=0;i<d*d;i++)
		{
			if(currentMetaSelected[i])
			{
				/*
				if(firstMatch){
					topLeftID = mtClickID*d*d + i;
					firstMatch=false;
				}
				*/
				index=mtClickID*d*d + i;

					if(Sender==btn0){
					if (btnStateMtProps[0]) {currentTable_props[index] |= (1 << 0);
					} else {currentTable_props[index] &= ~(1 << 0);}}
				if(Sender==btn1){
					if (btnStateMtProps[1]) {currentTable_props[index] |= (1 << 1);
					} else {currentTable_props[index] &= ~(1 << 1);}}
				if(Sender==btn2){
					if (btnStateMtProps[2]) {currentTable_props[index] |= (1 << 2);
					} else {currentTable_props[index] &= ~(1 << 2);}}
				if(Sender==btn3){
					if (btnStateMtProps[3]) {currentTable_props[index] |= (1 << 3);
					} else {currentTable_props[index] &= ~(1 << 3);}}
				if(Sender==btn4){
					if (btnStateMtProps[4]) {currentTable_props[index] |= (1 << 4);
					} else {currentTable_props[index] &= ~(1 << 4);}}
				if(Sender==btn5){
					if (btnStateMtProps[5]) {currentTable_props[index] |= (1 << 5);
					} else {currentTable_props[index] &= ~(1 << 5);}}
				if(Sender==btn6){
					if (btnStateMtProps[6]) {currentTable_props[index] |= (1 << 6);
					} else {currentTable_props[index] &= ~(1 << 6);}}
				if(Sender==btn7){
					if (btnStateMtProps[7]) {currentTable_props[index] |= (1 << 7);
					} else {currentTable_props[index] &= ~(1 << 7);}}

			}
		}
	}






   //	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	//if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(FormMetatileTool->Visible) FormMetatileTool->DrawTimer->Enabled=true;
	FormMain->EnableDisableTypeConflictShortcuts(false);
	FormMTprops->SetFocus();	
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::MaskEdit1KeyPress(TObject *Sender, char &Key)
{
	FormMain->EnableDisableTypeConflictShortcuts(true);
	//extern bool btnStateProps[8];
	bool bTab2x2 = FormMetatileTool->PageControl1->ActivePage==FormMetatileTool->TabSheet2x2?true:false;
	bool bTab4x4 = FormMetatileTool->PageControl1->ActivePage==FormMetatileTool->TabSheet4x4?true:false;
	bool bTab8x8 = FormMetatileTool->PageControl1->ActivePage==FormMetatileTool->TabSheet8x8?true:false;
	int d;
    int x,y,w,h;
	int i,j;
    int index;

	if(bTab2x2){
		d=2;
		currentTable_props = metatileSet_2x2_props;
		currentMetaSelection = &metaSelection_2x2;
	}
	if(bTab4x4){
		d=4;
		currentTable_props = metatileSet_4x4_props;
		currentMetaSelection = &metaSelection_4x4;
	}
	if(bTab8x8){
		d=8;
		currentTable_props = metatileSet_8x8_props;
		currentMetaSelection = &metaSelection_8x8;
	}


	if(Key==VK_ESCAPE)
	{
		int index = mtClickID*d*d;
		int tmp = currentTable_props[index];
		AnsiString hexString = IntToHex(tmp, 2);
		MaskEdit1->Text=hexString;

		Key=0;
	}
	if (Key == VK_RETURN)
		{
			Key = 0;
			int tmp;
			//int set = bankActive/16;
			tmp = StrToIntDef("0x" + MaskEdit1->Text, 0);

			btn7->Down = (tmp & 0x80) != 0;
			btn6->Down = (tmp & 0x40) != 0;
			btn5->Down = (tmp & 0x20) != 0;
			btn4->Down = (tmp & 0x10) != 0;
			btn3->Down = (tmp & 0x08) != 0;
			btn2->Down = (tmp & 0x04) != 0;
			btn1->Down = (tmp & 0x02) != 0;
			btn0->Down = (tmp & 0x01) != 0;

			btnStateMtProps[0] = btn0->Down;
			btnStateMtProps[1] = btn1->Down;
			btnStateMtProps[2] = btn2->Down;
			btnStateMtProps[3] = btn3->Down;
			btnStateMtProps[4] = btn4->Down;
			btnStateMtProps[5] = btn5->Down;
			btnStateMtProps[6] = btn6->Down;
			btnStateMtProps[7] = btn7->Down;
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

				if(Sender==btn0){

					if (btnStateMtProps[0]) {currentTable_props[index] |= (1 << 0);
					} else {currentTable_props[index] &= ~(1 << 0);}}
				if(Sender==btn1){
					if (btnStateMtProps[1]) {currentTable_props[index] |= (1 << 1);
					} else {currentTable_props[index] &= ~(1 << 1);}}
				if(Sender==btn2){
					if (btnStateMtProps[2]) {currentTable_props[index] |= (1 << 2);
					} else {currentTable_props[index] &= ~(1 << 2);}}
				if(Sender==btn3){
					if (btnStateMtProps[3]) {currentTable_props[index] |= (1 << 3);
					} else {currentTable_props[index] &= ~(1 << 3);}}
				if(Sender==btn4){
					if (btnStateMtProps[4]) {currentTable_props[index] |= (1 << 4);
					} else {currentTable_props[index] &= ~(1 << 4);}}
				if(Sender==btn5){
					if (btnStateMtProps[5]) {currentTable_props[index] |= (1 << 5);
					} else {currentTable_props[index] &= ~(1 << 5);}}
				if(Sender==btn6){
					if (btnStateMtProps[6]) {currentTable_props[index] |= (1 << 6);
					} else {currentTable_props[index] &= ~(1 << 6);}}
				if(Sender==btn7){
					if (btnStateMtProps[7]) {currentTable_props[index] |= (1 << 7);
					} else {currentTable_props[index] &= ~(1 << 7);}}
			}
		}
	}
	else    //multi select
	{
		//bool firstMatch=true;

		for(i=0;i<d*d;i++)
		{
			if(currentMetaSelected[i])
			{
				/*
				if(firstMatch){
					topLeftID = mtClickID*d*d + i;
					firstMatch=false;
				}
                */
				index=mtClickID*d*d + i;

					if(Sender==btn0){
					if (btnStateMtProps[0]) {currentTable_props[index] |= (1 << 0);
					} else {currentTable_props[index] &= ~(1 << 0);}}
				if(Sender==btn1){
					if (btnStateMtProps[1]) {currentTable_props[index] |= (1 << 1);
					} else {currentTable_props[index] &= ~(1 << 1);}}
				if(Sender==btn2){
					if (btnStateMtProps[2]) {currentTable_props[index] |= (1 << 2);
					} else {currentTable_props[index] &= ~(1 << 2);}}
				if(Sender==btn3){
					if (btnStateMtProps[3]) {currentTable_props[index] |= (1 << 3);
					} else {currentTable_props[index] &= ~(1 << 3);}}
				if(Sender==btn4){
					if (btnStateMtProps[4]) {currentTable_props[index] |= (1 << 4);
					} else {currentTable_props[index] &= ~(1 << 4);}}
				if(Sender==btn5){
					if (btnStateMtProps[5]) {currentTable_props[index] |= (1 << 5);
					} else {currentTable_props[index] &= ~(1 << 5);}}
				if(Sender==btn6){
					if (btnStateMtProps[6]) {currentTable_props[index] |= (1 << 6);
					} else {currentTable_props[index] &= ~(1 << 6);}}
				if(Sender==btn7){
					if (btnStateMtProps[7]) {currentTable_props[index] |= (1 << 7);
					} else {currentTable_props[index] &= ~(1 << 7);}}

			}
		}
	}



	//if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	//if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if((FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;

			//MaskEdit1Exit(Sender);
		}
	if (!isxdigit(Key) && Key != 0x08) // allow backspace
	{

		Key = 0; // discard the key press
	}

	if(Key == 0x08) //handle backspace
	{
		MaskEdit1->Text="";
		MaskEdit1->SelStart = 0;
		return;
	}


	if (MaskEdit1->SelLength == 0 && MaskEdit1->SelStart == MaskEdit1->MaxLength)
	{
		MaskEdit1->SelStart = 0;    //wraps around

	}

	if(Key)
	{
		if(MaskEdit1->SelLength < 2) MaskEdit1->SelLength = 1; //acts as insert mode.
	}




	
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::FormShow(TObject *Sender)
{
	bMTuseDirectTile=btnDirect->Down;	
}
//---------------------------------------------------------------------------

void __fastcall TFormMTprops::btnDirectClick(TObject *Sender)
{
	bMTuseDirectTile=btnDirect->Down;
	FormMain->UpdateAll();	
}
//---------------------------------------------------------------------------

