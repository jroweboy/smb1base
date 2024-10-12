//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitNESBank.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBank *FormBank;

extern unsigned char *chr;
extern unsigned int bankViewTable[];
extern int palActive;
extern int palBank;
extern unsigned char bgPal[];
extern int outPalette[];
void __fastcall TFormBank::DrawBank(TPicture* pic,unsigned char* achr, unsigned char* bchr,bool drawSet)
{
	const int palette[4]={ 0x000000,0x606060,0xc0c0c0,0xffffff };
	unsigned char *dst1,*dst2;
	int i,j,k,pp,pds,size,x,y;
	int col,col_ds,r,g,b;
    int destcol;
	int set=0;
	if(btnA->Down) set=0;
	if(btnB->Down) set=1*256;
	if(btnC->Down) set=2*256;
	if(btnD->Down) set=3*256;


	bool bUseGray=btnGray->Down? true:false;

	int tempPalActive=0;
	if(btnPal0->Down) tempPalActive=0;
	if(btnPal1->Down) tempPalActive=1;
	if(btnPal2->Down) tempPalActive=2;
	if(btnPal3->Down) tempPalActive=3;

	int pos=TrackBar1->Position;

	int hi=0;

	pp=0;

	for(y=0;y<16;++y)
	{
		for(x=0;x<16;++x)
		{
			pds=bankViewTable[set+ x+y*16]+(x+y*16)*16;

			for(i=0;i<8;i++)
			{
				dst1=(unsigned char*)pic->Bitmap->ScanLine[(y*8+i)*2+0]+x*8*2*3;
				dst2=(unsigned char*)pic->Bitmap->ScanLine[(y*8+i)*2+1]+x*8*2*3;

				for(j=0;j<8;j++)
				{

					if(bUseGray)col_ds=palette[(((bchr[pds+i]<<j)&128)>>7)|(((bchr[pds+i+8]<<j)&128)>>6)];
					else col_ds=outPalette[bgPal[palBank*16+tempPalActive*4+(((bchr[pds+i]<<j)&128)>>7)|(((bchr[pds+i+8]<<j)&128)>>6)]];


					if(bUseGray)col=palette[(((achr[pp+i]<<j)&128)>>7)|(((achr[pp+i+8]<<j)&128)>>6)];
					else col=outPalette[bgPal[palBank*16+tempPalActive*4+(((achr[pp+i]<<j)&128)>>7)|(((achr[pp+i+8]<<j)&128)>>6)]];

					destcol = drawSet? col_ds:col;
					if(btnDiffCheck->Down){
					if(btnInverse->Down) hi = (col==col_ds)? pos: -pos;
					else 				 hi = (col!=col_ds)? pos: -pos;
					}
					else hi=0;

					r=(destcol>>16&0xff)+hi;
					g=(destcol>>8&0xff)+hi;
					b=(destcol&0xff)+hi;

					if(r>0xff) r=0xff;
					if(g>0xff) g=0xff;
					if(b>0xff) b=0xff;

					if(r<0) r=0;
					if(g<0) g=0;
					if(b<0) b=0;


					*dst1++=r;
					*dst1++=g;
					*dst1++=b;
					*dst1++=r;
					*dst1++=g;
					*dst1++=b;

					*dst2++=r;
					*dst2++=g;
					*dst2++=b;
					*dst2++=r;
					*dst2++=g;
					*dst2++=b;
				}
			}
			//if(!drawSet)
			pp+=16;
		}
	}
}



void __fastcall TFormBank::WrongFile(void)
{
	unsigned char tchr[4096];

	memset(tchr,0,4096);

	DrawBank(ImageCHR1->Picture,tchr,chr,false);
	DrawBank(ImageCHR2->Picture,tchr,chr,true);

	ImageCHR1->Repaint();
	ImageCHR2->Repaint();

	ButtonOK->Enabled=false;
	UpDownBank->Enabled=false;
	EditBank->Enabled=false;
}



void __fastcall TFormBank::ShowBank(void)
{
	FILE *file;
	unsigned char header[16],tchr[4096];
	int mapnum,size;

	file=fopen(FileName.c_str(),"rb");

	if(!file)
	{
		WrongFile();
		return;
	}

	fseek(file,0,SEEK_END);
	size=ftell(file);
	fseek(file,0,SEEK_SET);

	if(size<16+16384+8192)//file is too small
	{
		fclose(file);
		WrongFile();
		return;
	}

	fread(header,16,1,file);

	if(memcmp(header,"NES",3))//not a NES ROM
	{
		fclose(file);
		WrongFile();
		return;
	}

	mapnum=((header[6]>>4)&0x0f)|(header[7]&0xf0);
	PRG=header[4];
	CHR=header[5];

	LabelInfo->Caption="Mapper "+IntToStr(mapnum)+", PRG: "+IntToStr(PRG)+", CHR: "+IntToStr(CHR);

	if(!CHR||Bank>=CHR)
	{
		fclose(file);
		WrongFile();
		return;
	}

	fseek(file,16+PRG*16384+Bank*4096,SEEK_SET);
	fread(tchr,4096,1,file);
	fclose(file);

	//DrawBank(ImageCHR1->Picture,tchr,false);
	//DrawBank(ImageCHR2->Picture,chr,true);
	DrawBank(ImageCHR1->Picture,tchr,chr,false);
	DrawBank(ImageCHR2->Picture,tchr,chr,true);
	ImageCHR1->Repaint();
	ImageCHR2->Repaint();

	if(UpDownBank->Position>=CHR) UpDownBank->Position=CHR-1;

	UpDownBank->Min=0;
	UpDownBank->Max=CHR-1;

	ButtonOK->Enabled=true;
	UpDownBank->Enabled=true;
	EditBank->Enabled=true;
}



//---------------------------------------------------------------------------
__fastcall TFormBank::TFormBank(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormBank::FormCreate(TObject *Sender)
{
	ImageCHR1->Picture=new TPicture();
	ImageCHR1->Picture->Bitmap=new Graphics::TBitmap();
	ImageCHR1->Picture->Bitmap->SetSize(ImageCHR1->Width,ImageCHR1->Height);
	ImageCHR1->Picture->Bitmap->PixelFormat=pf24bit;

	ImageCHR2->Picture=new TPicture();
	ImageCHR2->Picture->Bitmap=new Graphics::TBitmap();
	ImageCHR2->Picture->Bitmap->SetSize(ImageCHR2->Width,ImageCHR2->Height);
	ImageCHR2->Picture->Bitmap->PixelFormat=pf24bit;

	Bank=0;
	OK=false;
	FileName="";
}
//---------------------------------------------------------------------------
void __fastcall TFormBank::ButtonCancelClick(TObject *Sender)
{
	OK=false;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormBank::ButtonOKClick(TObject *Sender)
{
	OK=true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormBank::FormShow(TObject *Sender)
{
	btnA->Down=FormMain->SpeedButtonChrBank1->Down;
	btnB->Down=FormMain->SpeedButtonChrBank2->Down;
	btnC->Down=FormMain->SpeedButtonChrBank3->Down;
	btnD->Down=FormMain->SpeedButtonChrBank4->Down;

	if(palActive==0) btnPal0->Down=true;
	if(palActive==1) btnPal1->Down=true;
	if(palActive==2) btnPal2->Down=true;
	if(palActive==3) btnPal3->Down=true;

	ShowBank();
}
//---------------------------------------------------------------------------

void __fastcall TFormBank::EditBankChange(TObject *Sender)
{
	EditBank->Text=IntToStr(UpDownBank->Position);
	Bank=UpDownBank->Position;
	ShowBank();
}
//---------------------------------------------------------------------------

void __fastcall TFormBank::btnAClick(TObject *Sender)
{
	ShowBank();
}
//---------------------------------------------------------------------------


void __fastcall TFormBank::btnGrayClick(TObject *Sender)
{
	ShowBank();
}
//---------------------------------------------------------------------------

void __fastcall TFormBank::btnDiffCheckClick(TObject *Sender)
{
	ShowBank();
}
//---------------------------------------------------------------------------

void __fastcall TFormBank::TrackBar1Change(TObject *Sender)
{
	ShowBank();
}
//---------------------------------------------------------------------------

