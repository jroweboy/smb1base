//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSwapColors.h"
#include "UnitSwapAttributes.h"

#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSwapColors *FormSwapColors;
extern int iSwap_WhichSubpal;
extern int iSwap_Pal0Subpal;
extern int bankActive;
extern int palBank;
extern int nameTableWidth;
extern int nameTableHeight;

extern TRect nameSelection;

extern unsigned char bgPal[];
extern unsigned char palBuf[];
extern unsigned char palImportBuf[];
extern unsigned char chr[];
extern unsigned char chrSelected[];
extern unsigned char chrBuf[];
extern unsigned char chrImportBuf[];
extern unsigned char attrTable[];
extern unsigned char tmpAttrTable[];
extern unsigned char metaSprites[];
extern unsigned char metaSpritesBuf[];
extern bool sharedCol0;
extern bool cueUpdateMetasprite;
extern bool cueUpdateNametable;
extern bool holdStats;
//---------------------------------------------------------------------------
__fastcall TFormSwapColors::TFormSwapColors(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapColors::ButtonSwapClick(TObject *Sender)
{
	Swap=true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapColors::ButtonCancelClick(TObject *Sender)
{
	Swap=false;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapColors::FormCreate(TObject *Sender)
{

	Map[0]=0;
	Map[1]=1;
	Map[2]=2;
	Map[3]=3;
	Swap=false;
	RadioPalCurrent->Checked=true;
	RemapPalette=(RadioPalCurrent->Checked || RadioPalOne->Checked || RadioPalAll->Checked);

	if(FormMain->ConfirmNameSelection())
	{
		RadioButtonSelection->Checked=true;
	}
	else if (RadioButtonSelection->Checked) {
		 RadioButton4K->Checked=true;
		 RadioButtonSelection->Enabled=false;
	} 
	//CheckBoxPal->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::RadioButton4KClick(TObject *Sender)
{
	if(RadioButton4K->Checked)
	{
		Selection=false;
		WholeCHR=false;
	}
	if(RadioButton8K->Checked)
	{
		Selection=false;
		WholeCHR=true;
	}
	if(RadioButtonSelection->Checked)
	{
		Selection=true;
	}
	if(RadioPatternNone->Checked)
	{
       Selection=false;
	   WholeCHR=false;
	}
	PreviewSwap();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::CheckBoxPalClick(TObject *Sender)
{
	RemapPalette=(RadioPalCurrent->Checked || RadioPalOne->Checked || RadioPalAll->Checked);
	//PreviewSwap(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button1Click(TObject *Sender)
{
	int pp;

	pp=((TButton*)Sender)->Tag;

	if(Sender==ButtonWhichSubpal) iSwap_WhichSubpal=(iSwap_WhichSubpal+1)&3;
	else if(Sender==ButtonCol0) iSwap_Pal0Subpal=(iSwap_Pal0Subpal+1)&3;


	else Map[pp]=(Map[pp]+1)&3;

	PreviewSwap();
	Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::FormPaint(TObject *Sender)
{
	Button1->Caption=Map[0];
	Button2->Caption=Map[1];
	Button3->Caption=Map[2];
	Button4->Caption=Map[3];
	ButtonWhichSubpal->Caption=iSwap_WhichSubpal;
	ButtonCol0->Caption="subpal "+IntToStr(iSwap_Pal0Subpal);
	PreviewSwap();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonResetClick(TObject *Sender)
{
	Map[0]=0;
	Map[1]=1;
	Map[2]=2;
	Map[3]=3;

	RadioPalCurrent->Checked=true;
	PreviewSwap();
	Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonDarkerClick(TObject *Sender)
{
	Map[0]=0;
	Map[1]=0;
	Map[2]=1;
	Map[3]=2;

	//CheckBoxPal->Checked=false;

	PreviewSwap();
	Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonBrighterClick(TObject *Sender)
{
	Map[0]=1;
	Map[1]=2;
	Map[2]=3;
	Map[3]=3;

	//CheckBoxPal->Checked=false;

	PreviewSwap();
	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapColors::PreviewSwap()
{
	int tmp_AttrSize = (nameTableWidth+3)/4*((nameTableHeight+3)/4);
	unsigned char paltemp[4];
	bool swap[256*2];
	int i,j,k,pp,col,bit;
	int ib, is, bankOff;
	bool bSwapColours;
	if((FormSwapColors->Active==true)){
		memcpy (chr, chrBuf, 4096*2);  // restore.
		memcpy (bgPal, palBuf, 4*16);
		bSwapColours=true;
	}
	else if(FormSwapAttributes->Active){
		memcpy (bgPal, palBuf, 4*16);
		memcpy (attrTable,tmpAttrTable , tmp_AttrSize);
		memcpy (metaSprites, metaSpritesBuf, 256*64*4);
		bSwapColours=false;
	}
	else {
		memcpy (chr, chrImportBuf, 4096*2);
		memcpy (bgPal, palImportBuf, 4*16);
		bSwapColours=true;
	}
	if(bSwapColours)
	{
		for(i=0;i<256*2;i++) swap[i]=false;
			if (CheckBox1->Checked) {
		 if(!RadioPatternNone->Checked)
			{
				if(Selection)
        		{
        			for(i=0;i<256;i++) swap[(bankActive/4096*256)+i]=chrSelected[i];
				}
        		else
        		{
					if(WholeCHR)
        			{
        				for(i=0;i<256*2;i++) swap[i]=true;
					}
        			else
        			{
        				for(i=0;i<256;i++) swap[(bankActive/4096*256)+i]=true;
        			}
        		}
        
        		for(i=0;i<256*2;i++)
				{
        			if(swap[i])
        			{
        				pp=i*16;
        
        				for(j=0;j<8;j++)
        				{
        					for(k=0;k<8;k++)
        					{
        						bit=1<<k;
        						col=((chr[pp]&bit)?1:0)|((chr[pp+8]&bit)?2:0);
        						col=Map[col];
        						chr[pp]=(chr[pp]&~bit)|((col&1)<<k);
        						chr[pp+8]=(chr[pp+8]&~bit)|((col>>1)<<k);
							}
        					pp++;
        				}
        			}
        		}
        	}
			if(RemapPalette)
				{
					if(RadioPalOne->Checked) {ib=iSwap_WhichSubpal; is=1; bankOff=palBank;}
					else if(RadioPalAll->Checked) {ib=0; is=4*4; bankOff=0;}
					else {ib=0;is=4;bankOff=palBank;}
					for(i=ib;i<is+ib;i++)
					{
						//for(j=0;j<4;j++) paltemp[FormSwapColors->Map[j]]=bgPal[palBank*16+i*4+j];
						//iPal0Subpal
						//iWhichSubpal

						for(j=0;j<4;j++) paltemp[j]=bgPal[bankOff*16+i*4+Map[j]];
        
						for(j=0;j<4;j++) bgPal[bankOff*16+i*4+j]=paltemp[j];
					}
				}
	}
		if(sharedCol0)
		{
			//this overrides the results of pal_validate by overwriting its input

			col=bgPal[palBank*16+iSwap_Pal0Subpal*4+0];

			bgPal[palBank*16+0*4+0]=col;
			bgPal[palBank*16+1*4+0]=col;
			bgPal[palBank*16+2*4+0]=col;
			bgPal[palBank*16+3*4+0]=col;
		}
		}
		else //function is called by Swap attributes / subpalettes dialogue.
		{
			if (CheckBox1->Checked) {
			bool perform[4];
			for (int i = 0; i < 4; i++) perform[i]=false;

			if ((FormSwapAttributes->RadioPalAll->Checked) | (FormSwapAttributes->RadioPalOne->Checked)) {
				for (int i = 0; i < 4; i++) perform[i]=true;
				if (FormSwapAttributes->RadioPalOne->Checked) perform[palBank]=false;  //all but *this one*

			}
			if (FormSwapAttributes->RadioPalCurrent->Checked) {
				perform[palBank]=true;
			}

			//memcpy (paltemp,&bgPal,16*4);

			for (int i = 0; i < 4; i++) {   //iterates through the sets
				if (perform[i]){

					int set=i*16;

					for (int j = 0; j < 4; j++) {   //swaps subpals in a set
						memcpy (&bgPal[set+j*4],&palBuf[set+FormSwapAttributes->Map[j]*4],4);
					}
				}
			}

	if (FormSwapAttributes->RadioButton4K->Checked)//name inherited from palswap dialogue, really refers to "perform on entire table".
	{
		for (int i = 0; i < tmp_AttrSize; i++) {

		unsigned char result = 0;
			for (int j = 0; j < 8; j += 2) {
				result |= (FormSwapAttributes->Map[(tmpAttrTable[i] >> j)& 0x3 ] & 0x3) << j;
			}
			attrTable[i] = result;


					//another (computationally faster) way of expressing the same
					/*
					attrTable[i] =
					( ( FormSwapAttributes->Map[ (tmpAttrTable[i] & 0x3)]       & 0x3)
					| ((FormSwapAttributes->Map[((tmpAttrTable[i] & 0x3) << 2)] & 0x3) << 2)
					| ((FormSwapAttributes->Map[((tmpAttrTable[i] & 0x3) << 4)] & 0x3) << 4)
					| ((FormSwapAttributes->Map[((tmpAttrTable[i] & 0x3) << 6)] & 0x3) << 6)
					);
					*/

				}
	}
	else if (FormSwapAttributes->RadioButtonSelection->Checked)
	{
		int x,y,w,h;
		FormMain->GetSelection(nameSelection,x,y,w,h);

		for(int i=0;i<h;i+=2)
		{
			for(int j=0;j<w;j+=2)
			{
				int tmp = FormSwapAttributes->Map[FormMain->AttrGet(x+j,y+i,false,false)];
				FormMain->AttrSet(x+j, y+i, tmp, false);

			}
		}
	}
	if (FormSwapAttributes->RadioSpritesAll->Checked)
	{
		for(int i=0;i<256;++i)
		{
			int off=i*64*4;
			for(int j=0;j<64;j+=4)
			{
				if(metaSprites[off+j]<255)
				{
					//reorder the 2 least significant bits of the sprite attribute
					unsigned char attr = metaSprites[off+j+2];
					metaSprites[off + j + 2] =
						(attr & 0xFC) | (FormSwapAttributes->Map[attr & 0x3]& 0x3);
				}
			}
		}
	}
		}}
		FormMain->pal_validate();
		FormMain->UpdateTiles(true);
		cueUpdateNametable=true;
		FormMain->DrawPalettes();
		cueUpdateMetasprite=true;
}
void __fastcall TFormSwapColors::CheckBoxPalMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	PreviewSwap();
}
//---------------------------------------------------------------------------




void __fastcall TFormSwapColors::CheckBox1Click(TObject *Sender)
{
  if(!CheckBox1->Checked)
	{
		memcpy (chr, chrBuf, 4096*2);  // restore.
		memcpy (bgPal, palBuf, 4*16);
	}

	PreviewSwap();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button5Click(TObject *Sender)
{
	int tmp;
	tmp = Map[1];
	Map[1] = Map[3];
	Map[3] = tmp;
	PreviewSwap();
	Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button6Click(TObject *Sender)
{
    int tmp;

	tmp = Map[0];
	Map[0] = Map[3];
	Map[3] = tmp;

	tmp = Map[1];
	Map[1] = Map[2];
	Map[2] = tmp;

	PreviewSwap();
	Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::RadioPalCurrentMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//PreviewSwap();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::RadioPalCurrentMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   PreviewSwap();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button7Click(TObject *Sender)
{

	int tmp = Map[3];
	Map[3] = Map[2];
	Map[2] = Map[1];
	Map[1] = Map[0];
	Map[0] = tmp;

	PreviewSwap();
	Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button8Click(TObject *Sender)
{
    int tmp = Map[0];
	Map[0] = Map[1];
	Map[1] = Map[2];
	Map[2] = Map[3];
	Map[3] = tmp;

	PreviewSwap();
	Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int pp;
	int val = 1;

	pp=((TButton*)Sender)->Tag;
	if(Shift.Contains(ssRight)||Shift.Contains(ssShift)) val = -1;

	if(Sender==ButtonWhichSubpal) iSwap_WhichSubpal=(iSwap_WhichSubpal+val)&3;
	else if(Sender==ButtonCol0) iSwap_Pal0Subpal=(iSwap_Pal0Subpal+val)&3;


	else Map[pp]=(Map[pp]+val)&3;

	PreviewSwap();
	Repaint();	
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonResetMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Resets ordering of palette indices.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonResetMouseLeave(TObject *Sender)
{
	FormMain->LabelStats->Caption="---";
	holdStats=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button3MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Click to increment.\nShift-click or right-click to decrement.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonWhichSubpalMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Picks which subpalette to change in case just 1 subpalette is to be changed.\nClick to increment.\nShift-click or right-click to decrement.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonCol0MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Chooses from which subpalette the 'common colour' will be picked, in case of conflict.\nClick to increment.\nShift-click or right-click to decrement.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonDarkerMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="A preset that conflates colour indexes down, usually meaning 'darker'.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonBrighterMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="A preset that conflates colour indexes up, usually meaning 'brighter'.";

}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button7MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Rotates colour indices one step to the right.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button8MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Rotates colour indices one step to the left.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button5MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Flip order of colours 1-3 (colour 0 excluded).";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::Button6MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Flip order of all colours.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonCancelMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Discard changes and exit this dialogue.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::ButtonSwapMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Keep changes and exit this dialogue.";
	//FormMain->Update();
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapColors::FormShow(TObject *Sender)
{
  RadioButtonSelection->Enabled=true;
  //if (CheckBox1->Checked){
		//memcpy (chr, chrBuf, 4096*2);  // restore.
	   //	memcpy (bgPal, palBuf, 4*16);
		//PreviewSwap();
  //}
}
//---------------------------------------------------------------------------

