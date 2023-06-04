//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "UnitMain.h"
#include "UnitSwapPatternColour.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSwapPatternColour *FormSwapPatternColour;

extern unsigned char chr[];
extern unsigned char chrSelected[];
extern unsigned char chrBuf[];


extern bool cueUpdateMetasprite;
extern bool cueUpdateNametable;
extern bool holdStats;
extern int bankActive;

//---------------------------------------------------------------------------
__fastcall TFormSwapPatternColour::TFormSwapPatternColour(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::ButtonSwapClick(TObject *Sender)
{
	Swap=true;
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::ButtonCancelClick(TObject *Sender)
{
	Swap=false;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::CheckBox1Click(TObject *Sender)
{
	if(!CheckBox1->Checked)
	{
		memcpy (chr, chrBuf, 4096*2);  // restore.
	}

	PreviewSwap();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::FormCreate(TObject *Sender)
{
	Map[0]=0;
	Map[1]=1;
	Map[2]=2;
	Map[3]=3;
	Swap=false;
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::FormPaint(TObject *Sender)
{
	Button1->Caption=Map[0];
	Button2->Caption=Map[1];
	Button3->Caption=Map[2];
	Button4->Caption=Map[3];
	
	PreviewSwap();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::ButtonResetClick(TObject *Sender)
{
	Map[0]=0;
	Map[1]=1;
	Map[2]=2;
	Map[3]=3;

	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::ButtonDarkerClick(TObject *Sender)
{
	Map[0]=0;
	Map[1]=0;
	Map[2]=1;
	Map[3]=2;


	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::ButtonBrighterClick(TObject *Sender)
{
	Map[0]=1;
	Map[1]=2;
	Map[2]=3;
	Map[3]=3;

	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::Button7Click(TObject *Sender)
{
	int tmp = Map[3];
	Map[3] = Map[2];
	Map[2] = Map[1];
	Map[1] = Map[0];
	Map[0] = tmp;

	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::Button8Click(TObject *Sender)
{
	int tmp = Map[0];
	Map[0] = Map[1];
	Map[1] = Map[2];
	Map[2] = Map[3];
	Map[3] = tmp;
	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::Button5Click(TObject *Sender)
{
	int tmp;
	tmp = Map[1];
	Map[1] = Map[3];
	Map[3] = tmp;
	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::Button6Click(TObject *Sender)
{
	int tmp;

	tmp = Map[0];
	Map[0] = Map[3];
	Map[3] = tmp;

	tmp = Map[1];
	Map[1] = Map[2];
	Map[2] = tmp;
	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::Button1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int pp;
	int val = 1;

	pp=((TButton*)Sender)->Tag;
	if(Shift.Contains(ssRight)||Shift.Contains(ssShift)) val = -1;
	Map[pp]=(Map[pp]+val)&3;

	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::RadioButton4KClick(TObject *Sender)
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
	/*
	if(RadioPatternNone->Checked)
	{
	   Selection=false;
	   WholeCHR=false;
	}
	*/
	PreviewSwap();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::PreviewSwap()
{
	//unsigned char paltemp[4];
	bool swap[256*2];
	int i,j,k,pp,col,bit;
	int ib, is, bankOff;

	memcpy (chr, chrBuf, 4096*2);  // restore.
	//memcpy (bgPal, palBuf, 4*16);


	for(i=0;i<256*2;i++) swap[i]=false;
	if (CheckBox1->Checked)
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
	FormMain->pal_validate();
	FormMain->UpdateTiles(true);
	cueUpdateNametable=true;
	FormMain->DrawPalettes();
	cueUpdateMetasprite=true;
}
void __fastcall TFormSwapPatternColour::Button9Click(TObject *Sender)
{
	for(int i=0;i<4;i++)
	{
		Map[i]++;
		if (Map[i]>3) Map[i]=3;
	}
	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::Button10Click(TObject *Sender)
{
	for(int i=0;i<4;i++)
	{
		Map[i]--;
		if (Map[i]<0) Map[i]=0;
	}
	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::Button11Click(TObject *Sender)
{
    for(int i=0;i<4;i++)
	{
		Map[i]++;
		if (Map[i]>3) Map[i]=0;
	}
	Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFormSwapPatternColour::Button12Click(TObject *Sender)
{
   for(int i=0;i<4;i++)
	{
		Map[i]--;
		if (Map[i]<0) Map[i]=3;
	}
	Repaint();
}
//---------------------------------------------------------------------------
