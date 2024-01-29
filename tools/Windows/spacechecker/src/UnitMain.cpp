//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;


unsigned char *file_data;
int file_size;
int file_type;

FILETIME nesLastModified;

enum {
	TYPE_NES,
	TYPE_RAW
};



void __fastcall TFormMain::DrawPRG(int x,int y,unsigned char *data,int fill)
{
	const int markw[17]={8,3,3,3,6,3,3,3,8,3,3,3,6,3,3,3,8};
	int i,j,n,bf,sy;
	unsigned char *src,*dst;
	TCanvas *c;

	c=ImageView->Picture->Bitmap->Canvas;
	src=data;

	for(i=0;i<256;++i)
	{
		dst=(unsigned char*)ImagePRG->Picture->Bitmap->ScanLine[i];

		for(j=0;j<64;++j) *dst++=((*src++==fill)?0:2);
	}

	c->Pen->Color=clBlack;
	c->Rectangle(x,y,x+66,y+258);
	c->Draw(x+1,y+1,ImagePRG->Picture->Bitmap);

	src=data;
	bf=0;

	for(i=0;i<256;++i)
	{
		n=64;

		for(j=0;j<64;++j) if(*src++) n=0;

		bf+=n;
	}

	sy=y;

	for(i=0;i<17;++i)
	{
		c->PenPos=TPoint(x+66,sy);
		c->LineTo(x+66+markw[i],sy);
		c->PenPos=TPoint(x+66,sy+1);
		c->LineTo(x+66+markw[i],sy+1);

		sy+=16;
	}

	c->TextOut(x,y+260,"~"+IntToStr(bf/1024)+"K free");
}



void __fastcall TFormMain::DrawCHR(int x,int y,unsigned char *data)
{
	const int markw[9]={8,3,6,3,8,3,6,3,8};
	unsigned char *dst;
	int i,j,sy;
	TCanvas *c;

	c=ImageView->Picture->Bitmap->Canvas;

	for(i=0;i<128;i++)
	{
		dst=(unsigned char*)ImageCHR->Picture->Bitmap->ScanLine[i];

		for(j=0;j<64;j++) *dst++=(!*data++?0:2);
	}

	c->Pen->Color=clBlack;
	c->Rectangle(x,y,x+66,y+130);
	c->Draw(x+1,y+1,ImageCHR->Picture->Bitmap);

	sy=y;

	for(i=0;i<9;++i)
	{
		c->PenPos=TPoint(x+66,sy);
		c->LineTo(x+66+markw[i],sy);
		c->PenPos=TPoint(x+66,sy+1);
		c->LineTo(x+66+markw[i],sy+1);

		sy+=16;
	}
}



bool __fastcall TFormMain::LoadNES(AnsiString name)
{
	WIN32_FILE_ATTRIBUTE_DATA fileAttrData={0};
	FILE *file;
	unsigned char header[16];
	int alloc_size;

	file=fopen(name.c_str(),"rb");

	if(!file) return false;

	fseek(file,0,SEEK_END);
	file_size=ftell(file);
	fseek(file,0,SEEK_SET);

	if(file_data) free(file_data);

	fread(header,16,1,file);

	if(memcmp(header,"NES",3))//not a NES ROM
	{
		file_type=TYPE_RAW;
		alloc_size=(file_size+16383)/16384*16384;	//round the size up
	}
	else
	{
		file_type=TYPE_NES;
		alloc_size=file_size;
	}

	file_data=(unsigned char*)malloc(alloc_size);
	memset(file_data,0,alloc_size);

	fseek(file,0,SEEK_SET);
	fread(file_data,file_size,1,file);
	fclose(file);

	GetFileAttributesEx(name.c_str(),GetFileExInfoStandard,&fileAttrData);
	nesLastModified=fileAttrData.ftLastWriteTime;

	return true;
}



void __fastcall TFormMain::ShowNES(void)
{
	AnsiString str;
	int lens[256];
	int i,j,x,y,prg,chr,mapper,off,fill,hgt,len,prev,max;

	ImageView->Picture->Bitmap->SetSize(ImageView->Width,ImageView->Height);

	ImageView->Picture->Bitmap->Canvas->Brush->Color=clWhite;
	ImageView->Picture->Bitmap->Canvas->Rectangle(0,0,ClientWidth,ClientHeight);

	if(!file_data)
	{
		ImageView->Picture->Bitmap->Canvas->TextOut(10,10,"No file loaded");
		return;
	}

	if(file_type==TYPE_NES)
	{
		prg=file_data[4];
		chr=file_data[5];
		mapper=((file_data[6]>>4)&0x0f)|(file_data[7]&0xf0);
		off=16;
	}
	else
	{
		prg=(file_size+16383)/16384;
		chr=0;
		mapper=-1;
		off=0;
	}

	for(i=0;i<256;i++) lens[i]=0;

	for(i=0;i<prg;i++)
	{
		len=0;
		prev=file_data[off++];

		for(j=1;j<16384;++j)
		{
			if(file_data[off]==prev)
			{
				len++;
			}
			else
			{
				if(len>=8&&len>lens[prev]) lens[prev]=len;

				prev=file_data[off];

				len=0;
			}

			off++;
		}
	}

	if(MEmptyAuto->Checked)
	{
		max=-1;
		fill=-1;

		for(i=0;i<256;i++)
		{
			if(lens[i]>max)
			{
				max=lens[i];
				fill=i;
			}
		}
	}
	else
	{
		if(MEmpty00->Checked) fill=0x00; else fill=0xff;
	}

	str=OpenDialogNES->FileName;

	if(file_type==TYPE_NES)
	{
		str+="  Mapper #"+IntToStr(mapper)+"  "+IntToStr(prg)+"xPRG  "+IntToStr(chr)+"xCHR  Empty is $"+IntToHex(fill,2);
	}
	else
	{
		str+="  (raw binary, "+IntToStr(file_size)+" bytes)";
	}

	ImageView->Picture->Bitmap->Canvas->TextOut(10,10,str);

	off=16;
	x=10;
	y=50;
	hgt=256+80;

	for(i=0;i<prg;i++)
	{
		ImageView->Picture->Bitmap->Canvas->TextOut(x,y,"PRG $"+IntToHex(i,2));

		DrawPRG(x,y+20,&file_data[off],fill);

		off+=16384;
		x+=64+20;

		if(x+64+10>=ClientWidth)
		{
			x=10;

			y+=hgt;

			if(i==prg-1) hgt=128+40;
		}
	}

	for(i=0;i<chr;i++)
	{
		ImageView->Picture->Bitmap->Canvas->TextOut(x,y,"CHR $"+IntToHex(i,2));

		DrawCHR(x,y+20,&file_data[off]);

		off+=8192;
		x+=64+20;

		if(x+64+10>=ClientWidth)
		{
			x=10;
			y+=hgt;
			hgt=128+40;
		}
	}
}



void __fastcall TFormMain::DropFilesHandler(TWMDropFiles &Message)
{
	HDROP hd;
	char buf[MAX_PATH];

	hd=(HDROP)Message.Drop;

	if(DragQueryFile(hd,-1,NULL,NULL))
	{
		DragQueryFile(hd,0,buf,sizeof(buf));
		OpenDialogNES->FileName=buf;
		LoadNES(buf);
		ShowNES();
	}

	DragFinish(hd);
}



//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MOpenClick(TObject *Sender)
{
	if(OpenDialogNES->Execute())
	{
		LoadNES(OpenDialogNES->FileName);
		ShowNES();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
	PALETTEENTRY pal[256];
	int i,col;
	Graphics::TBitmap *bmp;

	bmp=new Graphics::TBitmap();
	bmp->SetSize(ImagePRG->Width,ImagePRG->Height);
	bmp->PixelFormat=pf8bit;

	pal[0].peBlue =255;
	pal[0].peGreen=255;
	pal[0].peRed  =255;

	pal[1].peBlue =240;
	pal[1].peGreen=240;
	pal[1].peRed  =240;

	pal[2].peBlue =48,
	pal[2].peGreen=255;
	pal[2].peRed  =48;

	pal[3].peBlue =0,
	pal[3].peGreen=230;
	pal[3].peRed  =0;

	SetPaletteEntries(bmp->Palette,0,4,pal);

	ImagePRG->Picture=new TPicture();
	ImagePRG->Picture->Bitmap=bmp;

	bmp=new Graphics::TBitmap();
	bmp->SetSize(ImageCHR->Width,ImageCHR->Height);
	bmp->PixelFormat=pf8bit;

	pal[0].peBlue =255;
	pal[0].peGreen=255;
	pal[0].peRed  =255;

	pal[1].peBlue =240;
	pal[1].peGreen=240;
	pal[1].peRed  =240;

	pal[2].peBlue =255,
	pal[2].peGreen=112;
	pal[2].peRed  =80;

	pal[3].peBlue =255,
	pal[3].peGreen=80;
	pal[3].peRed  =48;

	SetPaletteEntries(bmp->Palette,0,4,pal);

	ImageCHR->Picture=new TPicture();
	ImageCHR->Picture->Bitmap=bmp;

	ImageView->Picture=new TPicture();
	ImageView->Picture->Bitmap=new Graphics::TBitmap();
	ImageView->Picture->Bitmap->SetSize(ImageView->Width,ImageView->Height);
	ImageView->Picture->Bitmap->PixelFormat=pf24bit;

	file_data=NULL;
	file_size=0;
	file_type=-1;

	nesLastModified.dwLowDateTime=0;
	nesLastModified.dwHighDateTime=0;

	if(ParamStr(1)!="")
	{
		OpenDialogNES->FileName=ParamStr(1);
		LoadNES(ParamStr(1));
	}

	DragAcceptFiles(Handle,true);

	DoubleBuffered=true;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
	if(file_data)
	{
		free(file_data);

		file_data=NULL;
	}
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::MReloadClick(TObject *Sender)
{
	LoadNES(OpenDialogNES->FileName);
	ShowNES();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveReportClick(TObject *Sender)
{
	if(!SaveDialogReport->Execute()) return;

	ImageView->Picture->SaveToFile(SaveDialogReport->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormPaint(TObject *Sender)
{
	ShowNES();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TimerWatchTimer(TObject *Sender)
{
	WIN32_FILE_ATTRIBUTE_DATA fileAttrData={0};

	if(GetFileAttributesEx(OpenDialogNES->FileName.c_str(),GetFileExInfoStandard,&fileAttrData))
	{
		if(CompareFileTime((const FILETIME*)&fileAttrData.ftLastWriteTime,(const FILETIME*)&nesLastModified))
		{
			LoadNES(OpenDialogNES->FileName.c_str());
			ShowNES();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MWatchClick(TObject *Sender)
{
	if(!TimerWatch->Enabled)
	{
		MWatch->Caption="Unwatch";
		TimerWatch->Enabled=true;
	}
	else
	{
		MWatch->Caption="Watch";
		TimerWatch->Enabled=false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MEmptyAutoClick(TObject *Sender)
{
	LoadNES(OpenDialogNES->FileName);
	ShowNES();
}
//---------------------------------------------------------------------------

