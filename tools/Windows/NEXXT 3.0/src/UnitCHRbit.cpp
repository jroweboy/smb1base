//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include "UnitMain.h"
#include "UnitCHRbit.h"
#include "UnitName.h"
#include "UnitNavigator.h"
#include "UnitPropertyConditions.h"
#include "UnitMetatileEditor.h"
#include "UnitPropID.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCHRbit *FormCHRbit;

extern AnsiString propCHRlabel[];

extern int CHRCollisionGranularityX;
extern int CHRCollisionGranularityY;
extern bool openByFileDone;
extern int bankActive;
extern bool chrSelectRect;
extern TRect chrSelection;
extern unsigned char tileViewTable[];
extern unsigned int bankViewTable[];
extern unsigned char chrSelected[];
extern unsigned char *tileProperties;
extern bool btnStateProps[8];
extern int tileActive;
extern bool bKeyEscape;
extern bool holdStats;
int hoverBtn= -1;
extern bool collision_specific;
int conditionClicked;
extern char propConditional[];

extern bool propCondition[4][8];
extern bool propShowCondMap;
extern bool propShowCondTile;

extern bool bColissionIDHover;
extern bool bMTpropHover;

AnsiString RemoveExt(AnsiString name)
{
	return ChangeFileExt(name,"");
}



bool IsHexDigit(char c) {
	return ((c >= '0' && c <= '9') ||
			(c >= 'A' && c <= 'F') ||
			(c >= 'a' && c <= 'f'));
}
//---------------------------------------------------------------------------
__fastcall TFormCHRbit::TFormCHRbit(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TFormCHRbit::HexToClip(void)
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

void __fastcall TFormCHRbit::ClipToHexEdit(void) {
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
void __fastcall TFormCHRbit::UpdateBitButtons(bool bID_listClick)
{
	int i,j,k,x,y,w,h;

	int index;
	int set = bankActive/16;

	//set buttons according to tileActive
	if(chrSelectRect)
	{
		FormMain->GetSelection(chrSelection,x,y,w,h);
		index = bankViewTable[set + x + y*16]/16 + tileViewTable[x + y*16];


	}
	else
	{
		index=bankViewTable[set + tileActive]/16 + tileActive;

	}
	/*
	char message[100];
	sprintf(message, "%d", index);
	MessageBox(NULL, message, "Debug", MB_OK);
	*/



	for(k=0;k<8;k++)
	{
		if(bID_listClick) btnStateProps[k]= (FormPropID->ListBox1->ItemIndex & (1 << k)) != 0;
		else btnStateProps[k]= (tileProperties[index] & (1 << k)) != 0;

	}

	btn0->Down=   btnStateProps[0];
	btn1->Down=   btnStateProps[1];
	btn2->Down=   btnStateProps[2];
	btn3->Down=   btnStateProps[3];
	btn4->Down=   btnStateProps[4];
	btn5->Down=   btnStateProps[5];
	btn6->Down=   btnStateProps[6];
	btn7->Down=   btnStateProps[7];

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
				int index = bankViewTable[set+(x+i) + (y+j)*16]/16 + tileViewTable[(x+i) + (y+j)*16];
                int bit = (tileProperties[index] & mask) >> k;

                // Check if the current tile's bit value is different from the bit value of the previous tile in this row
				if (i > 0) {
					int previousIndex = bankViewTable[set+ (x+i-1) + (y+j)*16]/16 + tileViewTable[(x+i-1) + (y+j)*16];
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
void __fastcall TFormCHRbit::btn6labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit6 label";
	FormName->EditName->Text=btn6label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn6label->Caption=FormName->EditName->Text;
	propCHRlabel[6]=btn6label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormCHRbit::btn7labelClick(TObject *Sender)
{
   FormName->Caption="Rename bit7 label";
	FormName->EditName->Text=btn7label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn7label->Caption=FormName->EditName->Text;
	propCHRlabel[7]=btn7label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormCHRbit::btn5labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit5 label";
	FormName->EditName->Text=btn5label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn5label->Caption=FormName->EditName->Text;
	propCHRlabel[5]=btn5label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormCHRbit::btn4labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit4 label";
	FormName->EditName->Text=btn4label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn4label->Caption=FormName->EditName->Text;
	propCHRlabel[4]=btn4label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormCHRbit::btn3labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit3 label";
	FormName->EditName->Text=btn3label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn3label->Caption=FormName->EditName->Text;
	propCHRlabel[3]=btn3label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormCHRbit::btn2labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit2 label";
	FormName->EditName->Text=btn2label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn2label->Caption=FormName->EditName->Text;
	propCHRlabel[2]=btn2label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormCHRbit::btn1labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit1 label";
	FormName->EditName->Text=btn1label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn1label->Caption=FormName->EditName->Text;
	propCHRlabel[1]=btn1label->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TFormCHRbit::btn0labelClick(TObject *Sender)
{
	FormName->Caption="Rename bit0 label";
	FormName->EditName->Text=btn0label->Caption;
	FormName->ShowModal();
	if(bKeyEscape) return;
	btn0label->Caption=FormName->EditName->Text;
	propCHRlabel[0]=btn0label->Caption;
}

void __fastcall TFormCHRbit::btn0Click(TObject *Sender)
{
	int i,j,k,x,y,w,h;
	int set = bankActive/16;
	int index;

    FormMain->SetUndo();


	btnStateProps[0] = btn0->Down;
	btnStateProps[1] = btn1->Down;
	btnStateProps[2] = btn2->Down;
	btnStateProps[3] = btn3->Down;
	btnStateProps[4] = btn4->Down;
	btnStateProps[5] = btn5->Down;
	btnStateProps[6] = btn6->Down;
	btnStateProps[7] = btn7->Down;



	if(chrSelectRect)
	{
		FormMain->GetSelection(chrSelection,x,y,w,h);


		index=0;

		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				index = bankViewTable[set + x + y*16]/16 + tileViewTable[((x+j) + (y+i)*16)];

				if(Sender==btn0){
					if (btnStateProps[0]) {tileProperties[index] |= (1 << 0);
					} else {tileProperties[index] &= ~(1 << 0);}}
				if(Sender==btn1){
					if (btnStateProps[1]) {tileProperties[index] |= (1 << 1);
					} else {tileProperties[index] &= ~(1 << 1);}}
				if(Sender==btn2){
					if (btnStateProps[2]) {tileProperties[index] |= (1 << 2);
					} else {tileProperties[index] &= ~(1 << 2);}}
				if(Sender==btn3){
					if (btnStateProps[3]) {tileProperties[index] |= (1 << 3);
					} else {tileProperties[index] &= ~(1 << 3);}}
				if(Sender==btn4){
					if (btnStateProps[4]) {tileProperties[index] |= (1 << 4);
					} else {tileProperties[index] &= ~(1 << 4);}}
				if(Sender==btn5){
					if (btnStateProps[5]) {tileProperties[index] |= (1 << 5);
					} else {tileProperties[index] &= ~(1 << 5);}}
				if(Sender==btn6){
					if (btnStateProps[6]) {tileProperties[index] |= (1 << 6);
					} else {tileProperties[index] &= ~(1 << 6);}}
				if(Sender==btn7){
					if (btnStateProps[7]) {tileProperties[index] |= (1 << 7);
					} else {tileProperties[index] &= ~(1 << 7);}}
			}
		}
	}
	else
	{
		for(i=0;i<256;i++)
		{
			if(chrSelected[i])
			{
				index=bankViewTable[set + i]/16 + (tileViewTable[i]);

					if(Sender==btn0){
					if (btnStateProps[0]) {tileProperties[index] |= (1 << 0);
					} else {tileProperties[index] &= ~(1 << 0);}}
				if(Sender==btn1){
					if (btnStateProps[1]) {tileProperties[index] |= (1 << 1);
					} else {tileProperties[index] &= ~(1 << 1);}}
				if(Sender==btn2){
					if (btnStateProps[2]) {tileProperties[index] |= (1 << 2);
					} else {tileProperties[index] &= ~(1 << 2);}}
				if(Sender==btn3){
					if (btnStateProps[3]) {tileProperties[index] |= (1 << 3);
					} else {tileProperties[index] &= ~(1 << 3);}}
				if(Sender==btn4){
					if (btnStateProps[4]) {tileProperties[index] |= (1 << 4);
					} else {tileProperties[index] &= ~(1 << 4);}}
				if(Sender==btn5){
					if (btnStateProps[5]) {tileProperties[index] |= (1 << 5);
					} else {tileProperties[index] &= ~(1 << 5);}}
				if(Sender==btn6){
					if (btnStateProps[6]) {tileProperties[index] |= (1 << 6);
					} else {tileProperties[index] &= ~(1 << 6);}}
				if(Sender==btn7){
					if (btnStateProps[7]) {tileProperties[index] |= (1 << 7);
					} else {tileProperties[index] &= ~(1 << 7);}}

			}
		}
	}

	//index=set + tileViewTable[(x) + (y)*16];
	index=bankViewTable[set + tileActive]/16 + tileViewTable[tileActive];
	//FormMain->LabelStats->Caption="Property ID: "+IntToStr(tileProperties[index]);
	int tmp = tileProperties[index];
	AnsiString hexString = IntToHex(tmp, 2);
	MaskEdit1->Text=hexString;

	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn0MouseEnter(TObject *Sender)
{
	bMTpropHover=false;
	bColissionIDHover=false;

	if(Sender==btn0||Sender==btn0label||Sender==btnC0) hoverBtn=0;
	if(Sender==btn1||Sender==btn1label||Sender==btnC1) hoverBtn=1;
	if(Sender==btn2||Sender==btn2label||Sender==btnC2) hoverBtn=2;
	if(Sender==btn3||Sender==btn3label||Sender==btnC3) hoverBtn=3;

	if(Sender==btn4||Sender==btn4label||Sender==btnC4) hoverBtn=4;
	if(Sender==btn5||Sender==btn5label||Sender==btnC5) hoverBtn=5;
	if(Sender==btn6||Sender==btn6label||Sender==btnC6) hoverBtn=6;
	if(Sender==btn7||Sender==btn7label||Sender==btnC7) hoverBtn=7;

	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn0MouseLeave(TObject *Sender)
{

   if(btnHold->Down==false){
	hoverBtn= -1;
	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::FormCreate(TObject *Sender)
{
    
	for(int i=0; i<512;i++){
		tileProperties[i]=0;
	}
	btn0label->Caption=propCHRlabel[0];
	btn1label->Caption=propCHRlabel[1];
	btn2label->Caption=propCHRlabel[2];
	btn3label->Caption=propCHRlabel[3];
	btn4label->Caption=propCHRlabel[4];
	btn5label->Caption=propCHRlabel[5];
	btn6label->Caption=propCHRlabel[6];
	btn7label->Caption=propCHRlabel[7];
	}
//---------------------------------------------------------------------------


void __fastcall TFormCHRbit::FormClose(TObject *Sender, TCloseAction &Action)
{
hoverBtn= -1;
   if(btnShowCHR->Down) FormMain->UpdateTiles(false);
   if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
   FormMain->EnableDisableTypeConflictShortcuts(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::MaskEdit1KeyPress(TObject *Sender, char &Key)
{
	FormMain->EnableDisableTypeConflictShortcuts(true);
	if(Key==VK_ESCAPE)
	{
		int set = bankActive/16;
		int index = bankViewTable[set + tileActive]/16 + tileViewTable[tileActive];
		int tmp = tileProperties[index];
		AnsiString hexString = IntToHex(tmp, 2);
		MaskEdit1->Text=hexString;

		Key=0;
	}
	if (Key == VK_RETURN) // Check if Enter key was pressed
		{
			//SelectNext(Screen->ActiveControl, true, true);
			Key = 0;
            	int tmp;
   int set = bankActive/16;
	//if (MaskEdit1->Text=="" || MaskEdit1->Text==" ") {tmp=0;}
	tmp = StrToIntDef("0x" + MaskEdit1->Text, 0);

	btn7->Down = (tmp & 0x80) != 0;
	btn6->Down = (tmp & 0x40) != 0;
	btn5->Down = (tmp & 0x20) != 0;
	btn4->Down = (tmp & 0x10) != 0;
	btn3->Down = (tmp & 0x08) != 0;
	btn2->Down = (tmp & 0x04) != 0;
	btn1->Down = (tmp & 0x02) != 0;
	btn0->Down = (tmp & 0x01) != 0;

	btnStateProps[0] = btn0->Down;
	btnStateProps[1] = btn1->Down;
	btnStateProps[2] = btn2->Down;
	btnStateProps[3] = btn3->Down;
	btnStateProps[4] = btn4->Down;
	btnStateProps[5] = btn5->Down;
	btnStateProps[6] = btn6->Down;
	btnStateProps[7] = btn7->Down;

    if(chrSelectRect)
	{
		int x,y,w,h;
		FormMain->GetSelection(chrSelection,x,y,w,h);
		FormMain->SetUndo();

		int index=0;

		for(int i=0;i<h;i++)
		{
			for(int j=0;j<w;j++)
			{
				index = bankViewTable[set + (x+j) + (y+i)*16]/16 + tileViewTable[((x+j) + (y+i)*16)];


					if (btnStateProps[0]) {tileProperties[index] |= (1 << 0);
					} else {tileProperties[index] &= ~(1 << 0);}

					if (btnStateProps[1]) {tileProperties[index] |= (1 << 1);
					} else {tileProperties[index] &= ~(1 << 1);}

					if (btnStateProps[2]) {tileProperties[index] |= (1 << 2);
					} else {tileProperties[index] &= ~(1 << 2);}

					if (btnStateProps[3]) {tileProperties[index] |= (1 << 3);
					} else {tileProperties[index] &= ~(1 << 3);}

					if (btnStateProps[4]) {tileProperties[index] |= (1 << 4);
					} else {tileProperties[index] &= ~(1 << 4);}

					if (btnStateProps[5]) {tileProperties[index] |= (1 << 5);
					} else {tileProperties[index] &= ~(1 << 5);}

					if (btnStateProps[6]) {tileProperties[index] |= (1 << 6);
					} else {tileProperties[index] &= ~(1 << 6);}

					if (btnStateProps[7]) {tileProperties[index] |= (1 << 7);
					} else {tileProperties[index] &= ~(1 << 7);}
			}
		}
	}
	else
	{
		FormMain->SetUndo();

		for(int i=0;i<256;i++)
		{
			if(chrSelected[i])
			{
				int index=bankViewTable[set + i]/16 + (tileViewTable[i]);


					if (btnStateProps[0]) {tileProperties[index] |= (1 << 0);
					} else {tileProperties[index] &= ~(1 << 0);}

					if (btnStateProps[1]) {tileProperties[index] |= (1 << 1);
					} else {tileProperties[index] &= ~(1 << 1);}

					if (btnStateProps[2]) {tileProperties[index] |= (1 << 2);
					} else {tileProperties[index] &= ~(1 << 2);}

					if (btnStateProps[3]) {tileProperties[index] |= (1 << 3);
					} else {tileProperties[index] &= ~(1 << 3);}

					if (btnStateProps[4]) {tileProperties[index] |= (1 << 4);
					} else {tileProperties[index] &= ~(1 << 4);}

					if (btnStateProps[5]) {tileProperties[index] |= (1 << 5);
					} else {tileProperties[index] &= ~(1 << 5);}

					if (btnStateProps[6]) {tileProperties[index] |= (1 << 6);
					} else {tileProperties[index] &= ~(1 << 6);}

					if (btnStateProps[7]) {tileProperties[index] |= (1 << 7);
					} else {tileProperties[index] &= ~(1 << 7);}

			}
		}
	}







	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;

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


void __fastcall TFormCHRbit::MaskEdit1Exit(TObject *Sender)
{
	int tmp;
   int set = bankActive/16;
	//if (MaskEdit1->Text=="" || MaskEdit1->Text==" ") {tmp=0;}
	tmp = StrToIntDef("0x" + MaskEdit1->Text, 0);

	btn7->Down = (tmp & 0x80) != 0;
	btn6->Down = (tmp & 0x40) != 0;
	btn5->Down = (tmp & 0x20) != 0;
	btn4->Down = (tmp & 0x10) != 0;
	btn3->Down = (tmp & 0x08) != 0;
	btn2->Down = (tmp & 0x04) != 0;
	btn1->Down = (tmp & 0x02) != 0;
	btn0->Down = (tmp & 0x01) != 0;

	btnStateProps[0] = btn0->Down;
	btnStateProps[1] = btn1->Down;
	btnStateProps[2] = btn2->Down;
	btnStateProps[3] = btn3->Down;
	btnStateProps[4] = btn4->Down;
	btnStateProps[5] = btn5->Down;
	btnStateProps[6] = btn6->Down;
	btnStateProps[7] = btn7->Down;

    if(chrSelectRect)
	{
		int x,y,w,h;
		FormMain->GetSelection(chrSelection,x,y,w,h);
		FormMain->SetUndo();

		int index=0;

		for(int i=0;i<h;i++)
		{
			for(int j=0;j<w;j++)
			{
				index = bankViewTable[set+(x+j) + (y+i)*16]/16 + tileViewTable[((x+j) + (y+i)*16)];


					if (btnStateProps[0]) {tileProperties[index] |= (1 << 0);
					} else {tileProperties[index] &= ~(1 << 0);}

					if (btnStateProps[1]) {tileProperties[index] |= (1 << 1);
					} else {tileProperties[index] &= ~(1 << 1);}

					if (btnStateProps[2]) {tileProperties[index] |= (1 << 2);
					} else {tileProperties[index] &= ~(1 << 2);}

					if (btnStateProps[3]) {tileProperties[index] |= (1 << 3);
					} else {tileProperties[index] &= ~(1 << 3);}

					if (btnStateProps[4]) {tileProperties[index] |= (1 << 4);
					} else {tileProperties[index] &= ~(1 << 4);}

					if (btnStateProps[5]) {tileProperties[index] |= (1 << 5);
					} else {tileProperties[index] &= ~(1 << 5);}

					if (btnStateProps[6]) {tileProperties[index] |= (1 << 6);
					} else {tileProperties[index] &= ~(1 << 6);}

					if (btnStateProps[7]) {tileProperties[index] |= (1 << 7);
					} else {tileProperties[index] &= ~(1 << 7);}
			}
		}
	}
	else
	{
		FormMain->SetUndo();

		for(int i=0;i<256;i++)
		{
			if(chrSelected[i])
			{
				int index=bankViewTable[set+i]/16 + (tileViewTable[i]);


					if (btnStateProps[0]) {tileProperties[index] |= (1 << 0);
					} else {tileProperties[index] &= ~(1 << 0);}

					if (btnStateProps[1]) {tileProperties[index] |= (1 << 1);
					} else {tileProperties[index] &= ~(1 << 1);}

					if (btnStateProps[2]) {tileProperties[index] |= (1 << 2);
					} else {tileProperties[index] &= ~(1 << 2);}

					if (btnStateProps[3]) {tileProperties[index] |= (1 << 3);
					} else {tileProperties[index] &= ~(1 << 3);}

					if (btnStateProps[4]) {tileProperties[index] |= (1 << 4);
					} else {tileProperties[index] &= ~(1 << 4);}

					if (btnStateProps[5]) {tileProperties[index] |= (1 << 5);
					} else {tileProperties[index] &= ~(1 << 5);}

					if (btnStateProps[6]) {tileProperties[index] |= (1 << 6);
					} else {tileProperties[index] &= ~(1 << 6);}

					if (btnStateProps[7]) {tileProperties[index] |= (1 << 7);
					} else {tileProperties[index] &= ~(1 << 7);}

			}
		}
	}







	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
	FormMain->EnableDisableTypeConflictShortcuts(false);
	FormCHRbit->SetFocus();

}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::PaintBox1Paint(TObject *Sender)
{
	TCanvas *Canvas = PaintBox1->Canvas;
	String Text = "hex";
	int TextWidth = Canvas->TextWidth(Text);
	int TextHeight = Canvas->TextHeight(Text);

	Canvas->Font->Orientation = -900; // Rotate the font by 90 degrees
	Canvas->TextOut(PaintBox1->Height+5 - TextHeight, PaintBox1->Width+6 - TextWidth, Text);

}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::MaskEdit1MouseEnter(TObject *Sender)
{
	collision_specific=true;
    hoverBtn=0; //just a non-negative value between 0...7, doesn't matter
	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::MaskEdit1MouseLeave(TObject *Sender)
{
	collision_specific=false;
	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::BitBtnTimerTimer(TObject *Sender)
{
    if(!openByFileDone) return;
	if(FormCHRbit->Visible) UpdateBitButtons(false);
    BitBtnTimer->Enabled=false;
}
//---------------------------------------------------------------------------


void __fastcall TFormCHRbit::btnC0MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    

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
	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnHoldClick(TObject *Sender)
{
    if(btnHold->Down==false)hoverBtn= -1;

	FormMain->UpdateTiles(false);
	FormMain->UpdateNameTable(-1,-1,true);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::MaskEdit1Enter(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(true);	
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::MaskEdit1Click(TObject *Sender)
{
   // Block the default context menu
        TMessage Msg;
        Msg.Msg = WM_CONTEXTMENU;
        Msg.LParam = reinterpret_cast<int>(MaskEdit1->Handle);
        Msg.WParam = MAKELPARAM(0, 0);
		MaskEdit1->Dispatch(&Msg);

	FormMain->EnableDisableTypeConflictShortcuts(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::FormDeactivate(TObject *Sender)
{
	FormMain->EnableDisableTypeConflictShortcuts(false);	
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnHoldMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Hold: when down, highlights remain until this toolbox is closed/nor hovering over another 'highlight button' negates it.";
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnHoldMouseLeave(TObject *Sender)
{
	FormMain->LabelStats->Caption="---";		
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnShowCHRMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Highlights are shown on the tileset with this button down.";
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnShowScreenMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Highlights are shown on the screen/map with this button down.";

}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn0MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
	int set = bankActive/16;
	int tag=((TSpeedButton*)Sender)->Tag;

	if (Button == mbRight)
	{
		chrSelectRect=false;

		for(int i;i<256;i++)
		{
			int index = bankViewTable[set+i]/16 + tileViewTable[i];
			int mask = 1 << tag;
			int bit = (tileProperties[index] & mask);// >> tag;
			chrSelected[tileViewTable[i]] = bit>0? true:false;
		}
	}
	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::MaskEdit1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int set = bankActive/16;
	 int mask = StrToIntDef("0x" + MaskEdit1->Text, 0);

	if (Button == mbRight)
	{

		chrSelectRect=false;

		for(int i;i<256;i++)
		{
			int index = bankViewTable[set+i]/16 + tileViewTable[i];
			chrSelected[tileViewTable[i]] = tileProperties[index]==mask? true:false;

		}
	}
	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormCHRbit::btn1x1Click(TObject *Sender)
{
	CHRCollisionGranularityX=1;
	CHRCollisionGranularityY=1;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn2x1Click(TObject *Sender)
{
   CHRCollisionGranularityX=2;
   CHRCollisionGranularityY=1;
   FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn2x2Click(TObject *Sender)
{
	CHRCollisionGranularityX=2;
	CHRCollisionGranularityY=2;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn4x2Click(TObject *Sender)
{
	CHRCollisionGranularityX=4;
	CHRCollisionGranularityY=2;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn4x4Click(TObject *Sender)
{
	CHRCollisionGranularityX=4;
	CHRCollisionGranularityY=4;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn1x1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Map/nametable highlights are shown with a per-tile granularity.\nA screen of unpacked reference data equals 960 bytes.\nAn example of an NES game using per-tile referencing is Metroid; facilitating PRG-RAM.";

}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn2x1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Map/nametable highlights are shown with a 2x1 granularity.\nA screen of unpacked reference data equals 480 bytes.";

}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn2x2MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Map/nametable highlights are shown with a 2x2 granularity.\nA screen of unpacked reference data equals 240 bytes.";

}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn4x2MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Map/nametable highlights are shown with a 4x2 granularity.\nA screen of unpacked reference data equals 120 bytes.";

}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btn4x4MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Map/nametable highlights are shown with a 4x4 granularity.\nA screen of unpacked reference data equals 60 bytes.";

}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnShowMetaMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Highlights are shown on metatiles with this button down.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnShowNavClick(TObject *Sender)
{
	if(btnShowNav->Down && FormNavigator->Visible) FormNavigator->Draw(false,false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnShowMetaClick(TObject *Sender)
{
	if((btnShowMeta->Down) && (FormMetatileTool->Visible)) FormMetatileTool->DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnShowScreenClick(TObject *Sender)
{
	if(btnShowScreen->Down) FormMain->UpdateNameTable(-1,-1,true);
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnShowCHRClick(TObject *Sender)
{
	if(btnShowCHR->Down) FormMain->UpdateTiles(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormCHRbit::btnShowNavMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Highlights are shown on the Navigator with this button down.";		
}
//---------------------------------------------------------------------------

