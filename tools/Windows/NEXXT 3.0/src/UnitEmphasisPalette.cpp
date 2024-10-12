//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <Clipbrd.hpp>
#pragma hdrstop

#include "UnitEmphasisPalette.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmphasisPalette *FormEmphasisPalette;
unsigned char cPPUMaskArray[8];
extern int fullPaletteGenerated[64*8];
extern int basePalette[64];
int popupTag;
int PPU_typeGenerated=1;
TPoint clickedImagePoint; //clicked point on the full palette canvas.

const float emph_nesst[8][3]={//from nintech.txt. also the only one commenting on ocular observation. i find it closer to what i see, too. 
	{100.0,100.0,100.0},
	{ 74.3, 91.5,123.9},
	{ 88.2,108.6, 79.4},
	{ 65.3, 98.0,101.9},
	{127.7,102.6, 90.5},
	{ 97.9, 90.8,102.3},
	{100.1, 98.7, 74.1},
	{ 75.0, 75.0, 75.0}
};

const float emph_nintendulator[8][3]={  //derived from nintendulator 0.98, but reordered bgr to fit NEXXT.
	{100.0,100.0,100.0},
	{ 85.0, 85.0,100.0},
	{ 85.0,100.0, 85.0},
	{ 70.0, 85.0, 85.0},
	{100.0, 85.0, 85.0},
	{ 85.0, 70.0, 85.0},
	{ 85.0, 85.0, 70.0},
	{ 70.0, 70.0, 70.0}
};

const float emph_mesen[8][3]={   //table derived from the function mesen uses. Probably a different way to express the same conversation/understanding as nintendulator.
	{100.00, 100.00, 100.00},
	{ 84.00,  84.00, 100.00},
	{ 84.00, 100.00,  84.00},
	{ 70.56,  84.00,  84.00},
	{100.00,  84.00,  84.00},
	{ 84.00,  70.56,  84.00},
	{ 84.00,  84.00,  70.56},
	{ 70.56,  70.56,  70.56}
};
bool IsHexDigit(char c) {
	return ((c >= '0' && c <= '9') ||
			(c >= 'A' && c <= 'F') ||
			(c >= 'a' && c <= 'f'));
}
extern int ppuMask;
AnsiString RemoveExt(AnsiString name)
{
	return ChangeFileExt(name,"");
}
void __fastcall TFormEmphasisPalette::MakePalette(void)
{
	int i,j;
	float r,g,b;









	//FG: bugfix. M bit no longer overwrites the result of RGB emph bits.
		
		for(j=0;j<8;j++)
		{
			for(i=0;i<64;i++)
			{

				r=((float)((basePalette[i]>>16)&0xff))/255.0;
				g=((float)((basePalette[i]>>8)&0xff))/255.0;
				b=((float)((basePalette[i])   &0xff))/255.0;

				if(PPU_typeGenerated==0){  //RGB PPU - Playchoice, Titler, etc.

					r = (cPPUMaskArray[j]&0x80)  ? 1.0 : r;
					g = (cPPUMaskArray[j]&0x40)  ? 1.0 : g;
					b = (cPPUMaskArray[j]&0x20)  ? 1.0 : b;
				}

				else{  //NES PPU

                    switch(PPU_typeGenerated){
						case 2:
							r = r * emph_nintendulator[cPPUMaskArray[j]>>5][0] /100.0;
							g = g * emph_nintendulator[cPPUMaskArray[j]>>5][1] /100.0;
							b = b * emph_nintendulator[cPPUMaskArray[j]>>5][2] /100.0;
							break;
						case 3:
							r = r * emph_mesen[cPPUMaskArray[j]>>5][0] /100.0;
							g = g * emph_mesen[cPPUMaskArray[j]>>5][1] /100.0;
							b = b * emph_mesen[cPPUMaskArray[j]>>5][2] /100.0;
							break;
						default:
							r = r * emph_nesst[cPPUMaskArray[j]>>5][0] /100.0;
							g = g * emph_nesst[cPPUMaskArray[j]>>5][1] /100.0;
							b = b * emph_nesst[cPPUMaskArray[j]>>5][2] /100.0;
					}


				}
                if(r>1.0) r=1.0;
				if(g>1.0) g=1.0;
				if(b>1.0) b=1.0;

				fullPaletteGenerated[i + (j*64)]=(((int)(255.0*r))<<16)|(((int)(255.0*g))<<8)|((int)(255.0*b));
			}
	   }
	/*
	if(ppuMask&0x01)
	{
	   for(i=0;i<64*8;i++)
		{
			fullPaletteGenerated[i]=fullPaletteGenerated[i&0xf0];

		}
	}
	*/
	/*
	The table found in the array 'emphasis'
	is the one from nintech.txt, which credits Chris Covell

	001        B: 074.3%        G: 091.5%        R: 123.9%
	010        B: 088.2%        G: 108.6%        R: 079.4%
	011        B: 065.3%        G: 098.0%        R: 101.9%
	100        B: 127.7%        G: 102.6%        R: 090.5%
	101        B: 097.9%        G: 090.8%        R: 102.3%
	110        B: 100.1%        G: 098.7%        R: 074.1%
	111        B: 075.0%        G: 075.0%        R: 075.0%
	*/

}

void __fastcall TFormEmphasisPalette::DrawCol(int x,int y,int size,int c)
{
	TRect r;

	r.left  =x;
	r.top   =y;
	r.right =x+size;
	r.bottom=y+size;

	ImageEmphPalette->Canvas->Brush->Color=TColor(fullPaletteGenerated[c]);
	ImageEmphPalette->Canvas->FillRect(r);

}

//---------------------------------------------------------------------------
void __fastcall TFormEmphasisPalette::HexToClip(void)
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

void __fastcall TFormEmphasisPalette::ClipToHexEdit(void) {
	if (OpenClipboard(0)) {
		HGLOBAL hData = GetClipboardData(CF_TEXT);
		if (hData != NULL) {
			char* clipboardText = static_cast<char*>(GlobalLock(hData));
			if (clipboardText != NULL) {
				String str = clipboardText;
				GlobalUnlock(hData);
                
				if (str.Length() >= 1) {
					String hexChars = str.SubString(1, 6);

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
void __fastcall TFormEmphasisPalette::Save192b(int tag)
{
	FILE *file;
	unsigned char pal[192];
	int i;
	AnsiString name;

	FormMain->BlockDrawing(true);

	name=RemoveExt(SaveDialogBitmap->FileName);

	if(name=="") name="partial_emphasis_palette";

	SaveDialogBitmap->FileName=name+".pal";


	if(SaveDialogPal->Execute())
	{
		name=RemoveExt(SaveDialogPal->FileName)+".pal";
		/*
		if(!FormMain->OverwritePrompt(name))
		{
			BlockDrawing(false);
			return;
		}
		*/
		int pp=0;
		for(i=0;i<192;i+=3)
		{

			pal[i]   = (uint8_t)(fullPaletteGenerated[pp +tag*192] >> 16);
			pal[i+1] = (uint8_t)(fullPaletteGenerated[pp +tag*192] >> 8);
			pal[i+2] = (uint8_t) fullPaletteGenerated[pp +tag*192];
			pp++;
		}

		file=fopen(name.c_str(),"wb");

		if(file)
		{
			fwrite(pal,192,1,file);
			fclose(file);
		}
	}

	FormMain->BlockDrawing(false);
}

//---------------------------------------------------------------------------
void __fastcall TFormEmphasisPalette::Save1536b(void)
{
   FILE *file;
	unsigned char pal[1536];
	int i;
	AnsiString name;

	FormMain->BlockDrawing(true);

	name=RemoveExt(SaveDialogBitmap->FileName);

	if(name=="") name="full_emphasis_palette";

	SaveDialogBitmap->FileName=name+".pal";


	if(SaveDialogPal->Execute())
	{
		name=RemoveExt(SaveDialogPal->FileName)+".pal";
		/*
		if(!FormMain->OverwritePrompt(name))
		{
			BlockDrawing(false);
			return;
		}
		*/
		int pp=0;
		for(i=0;i<1536;i+=3)
		{
			pal[i]   = (uint8_t)(fullPaletteGenerated[pp] >> 16);
			pal[i+1] = (uint8_t)(fullPaletteGenerated[pp] >> 8);
			pal[i+2] = (uint8_t) fullPaletteGenerated[pp];
            pp++;
		}

		file=fopen(name.c_str(),"wb");

		if(file)
		{
			fwrite(pal,1536,1,file);
			fclose(file);
		}
	}

	FormMain->BlockDrawing(false);
}

//---------------------------------------------------------------------------
void __fastcall TFormEmphasisPalette::Save192b_BMP(int tag)
{
	TPicture *picture;
	Graphics::TBitmap *bmp;
	int i,j,x,y;
	AnsiString str;

	FormMain->BlockDrawing(true);

	str=RemoveExt(SaveDialogBitmap->FileName);

	if(str=="") str="partial_emphasis_palette";

	SaveDialogBitmap->FileName=str+".bmp";

	if(SaveDialogBitmap->Execute())
	{
		TRect sr(0, tag * 64, 256, 64+tag * 64);
		TRect dr(0, 0, 		  256, 64);


		picture=new TPicture();
		bmp=new Graphics::TBitmap();
		bmp->SetSize(256,64);
		bmp->PixelFormat=pf24bit;
		//SetBMPPalette(bmp);
		picture->Bitmap=bmp;

		bmp->Canvas->CopyRect(dr, ImageEmphPalette->Picture->Bitmap->Canvas, sr);
		picture->SaveToFile(SaveDialogBitmap->FileName);

		delete bmp;
		delete picture;
	}

	FormMain->BlockDrawing(false);
}

//---------------------------------------------------------------------------
void __fastcall TFormEmphasisPalette::Save1536b_BMP(void)
{

	AnsiString str;

	FormMain->BlockDrawing(true);

	str=RemoveExt(SaveDialogFullBitmap->FileName);

	if(str=="") str="full_emphasis_palette";

	SaveDialogFullBitmap->FileName=str+".bmp";

	if(SaveDialogFullBitmap->Execute())
	{


		/*picture=new TPicture();
		bmp=new Graphics::TBitmap();
		bmp->SetSize(256,512);
		bmp->PixelFormat=pf24bit;
		SetBMPPalette(bmp);
		picture->Bitmap=bmp;

		picture->SaveToFile(SaveDialogFullBitmap->FileName);

		delete bmp;
		delete picture;
		*/
		ImageEmphPalette->Picture->SaveToFile(SaveDialogFullBitmap->FileName);
	}

	FormMain->BlockDrawing(false);
}

//---------------------------------------------------------------------------
void __fastcall TFormEmphasisPalette::SetPPUmaskArray(void)
{
	for (int i = 0; i < 8; i++) {
		cPPUMaskArray[i] = (i<<5);
	}
	cPPUMaskArray[0] = 0;
	cPPUMaskArray[1] = 1<<5;
	cPPUMaskArray[2] = 2<<5;
	cPPUMaskArray[3] = 3<<5;

	cPPUMaskArray[4] = 4<<5;
	cPPUMaskArray[5] = 5<<5;
	cPPUMaskArray[6] = 6<<5;
	cPPUMaskArray[7] = 7<<5;


}

//---------------------------------------------------------------------------
void __fastcall TFormEmphasisPalette::UpdateCheckboxes(void)
{


	//--- set checkboxes programmatically.
	chkR0->Checked = cPPUMaskArray[0]&0x20;
	chkG0->Checked = cPPUMaskArray[0]&0x40;
	chkB0->Checked = cPPUMaskArray[0]&0x80;

	chkR1->Checked = cPPUMaskArray[1]&0x20;
	chkG1->Checked = cPPUMaskArray[1]&0x40;
	chkB1->Checked = cPPUMaskArray[1]&0x80;

	chkR2->Checked = cPPUMaskArray[2]&0x20;
	chkG2->Checked = cPPUMaskArray[2]&0x40;
	chkB2->Checked = cPPUMaskArray[2]&0x80;

	chkR3->Checked = cPPUMaskArray[3]&0x20;
	chkG3->Checked = cPPUMaskArray[3]&0x40;
	chkB3->Checked = cPPUMaskArray[3]&0x80;

	chkR4->Checked = cPPUMaskArray[4]&0x20;
	chkG4->Checked = cPPUMaskArray[4]&0x40;
	chkB4->Checked = cPPUMaskArray[4]&0x80;

	chkR5->Checked = cPPUMaskArray[5]&0x20;
	chkG5->Checked = cPPUMaskArray[5]&0x40;
	chkB5->Checked = cPPUMaskArray[5]&0x80;

	chkR6->Checked = cPPUMaskArray[6]&0x20;
	chkG6->Checked = cPPUMaskArray[6]&0x40;
	chkB6->Checked = cPPUMaskArray[6]&0x80;

	chkR7->Checked = cPPUMaskArray[7]&0x20;
	chkG7->Checked = cPPUMaskArray[7]&0x40;
	chkB7->Checked = cPPUMaskArray[7]&0x80;




}

//---------------------------------------------------------------------------
void __fastcall TFormEmphasisPalette::UpdateCanvas(void)
{
	MakePalette();

	int x,y,pp;
	y=0;pp=0;
	for(int i=0;i<4*8;i++)
	{
		x=0;

		for(int j=0;j<16;j++)
		{
			DrawCol(x,y,16,pp);   //x,y,size,c
			pp++;
			x+=16;
		}
		y+=16;
	}
	ImageEmphPalette->Refresh();

}

//---------------------------------------------------------------------------
__fastcall TFormEmphasisPalette::TFormEmphasisPalette(TComponent* Owner)
	: TForm(Owner)
{
}

void __fastcall TFormEmphasisPalette::FormCreate(TObject *Sender)
{
	ImageEmphPalette->Picture=new TPicture();
	ImageEmphPalette->Picture->Bitmap=new Graphics::TBitmap();
	ImageEmphPalette->Picture->Bitmap->PixelFormat=pf24bit;
	ImageEmphPalette     ->Picture->Bitmap->SetSize(256,512);

	SetPPUmaskArray();
	UpdateCheckboxes();
	UpdateCanvas();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmphasisPalette::FormShow(TObject *Sender)
{
     UpdateCheckboxes();
	 UpdateCanvas();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmphasisPalette::PaintBox1Paint(TObject *Sender)
{
	TCanvas *Canvas = PaintBox1->Canvas;
	String Text = "hex";
	int TextWidth = Canvas->TextWidth(Text);
	int TextHeight = Canvas->TextHeight(Text);

	Canvas->Font->Orientation = -900; // Rotate the font by 90 degrees
	Canvas->TextOut(PaintBox1->Height+5 - TextHeight, PaintBox1->Width+6 - TextWidth, Text);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::ImageEmphPaletteClick(TObject *Sender)
{
	TImage *image = dynamic_cast<TImage*>(Sender);

	if (image) {
		TPoint cursorPos = ScreenToClient(Mouse->CursorPos);
		TColor pixelColor = image->Canvas->Pixels[clickedImagePoint.x][clickedImagePoint.y];

		// Convert the TColor to a hex string
		AnsiString hexColor = IntToHex(static_cast<int>(pixelColor), 6);

		// Set the hex color as the caption of the TMaskEdit control
		MaskEdit1->Text = hexColor;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::ImageEmphPaletteMouseMove(TObject *Sender,
	  TShiftState Shift, int X, int Y)
{
	clickedImagePoint.x = X;
	clickedImagePoint.y = Y;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::SpeedButton20Click(TObject *Sender)
{
	HexToClip();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::SpeedButton21Click(TObject *Sender)
{
	SetPPUmaskArray();
	UpdateCheckboxes();
	UpdateCanvas();
}
//---------------------------------------------------------------------------




void __fastcall TFormEmphasisPalette::chkB0MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
       	TCheckBox* checkBox = dynamic_cast<TCheckBox*>(Sender);
		cPPUMaskArray[checkBox->Tag]^=0x80;
		//UpdateCheckboxes();
		UpdateCanvas();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::chkG0MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
      	TCheckBox* checkBox = dynamic_cast<TCheckBox*>(Sender);
		cPPUMaskArray[checkBox->Tag]^=0x40;
		//UpdateCheckboxes();
		UpdateCanvas();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::chkR0MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	  TCheckBox* checkBox = dynamic_cast<TCheckBox*>(Sender);
		cPPUMaskArray[checkBox->Tag]^=0x20;
		//UpdateCheckboxes();
		UpdateCanvas();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::SpeedButton1Click(TObject *Sender)
{
	//handle saving 192b portion
	TSpeedButton* speedButton = dynamic_cast<TSpeedButton*>(Sender);
	int tag = speedButton->Tag;
	Save192b(tag);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::SpeedButton18Click(TObject *Sender)
{
	//handle saving full emphasis palette
	Save1536b();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::SpeedButton2Click(TObject *Sender)
{
	//handle saving 192b portion as BMP
	TSpeedButton* speedButton = dynamic_cast<TSpeedButton*>(Sender);
	int tag = speedButton->Tag;
	Save192b_BMP(tag);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::SpeedButton19Click(TObject *Sender)
{
	//handle saving full 1936b as BMP
	Save1536b_BMP();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::SpeedButton22Click(TObject *Sender)
{
	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;
	TSpeedButton* speedButton = dynamic_cast<TSpeedButton*>(Sender);
	popupTag = speedButton->Tag;

	PopupMenu1->Popup(x,y);

}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::PutCarrayonclipboard1Click(
	  TObject *Sender)
{
	uint8_t uint8Array[192];
	int offset = popupTag*64;
	int pp=0;
	for(int i=0; i<192; i+=3) {
		uint8Array[i]   = (uint8_t)(fullPaletteGenerated[pp+offset] >> 16);
		uint8Array[i+1] = (uint8_t)(fullPaletteGenerated[pp+offset] >> 8);
		uint8Array[i+2] = (uint8_t) fullPaletteGenerated[pp+offset];
        pp++;
	}


	char str[65536],buf[1024];
	TMenuItem* menuItem = dynamic_cast<TMenuItem*>(Sender);
	int itemTag = menuItem->Tag;

	switch (popupTag) {
		case 0:
			sprintf (buf,"000");
			break;
		case 1:
			sprintf (buf,"0c0");
			break;
		case 2:
			sprintf (buf,"180");
			break;
		case 3:
			sprintf (buf,"240");
			break;
		case 4:
			sprintf (buf,"300");
			break;
		case 5:
			sprintf (buf,"3c0");
			break;
		case 6:
			sprintf (buf,"480");
			break;
		case 7:
			sprintf (buf,"540");
			break;
		default:
			sprintf (buf,"000");

	}

	if (itemTag==0) sprintf (str,"char pal_0x%s[192]={\n\t",buf);
	if (itemTag==1) sprintf (str,"byte[] pal_0x%s={\n\t",buf);

	for (int i = 0; i < sizeof(uint8Array); i++) {
		char hexValue[6];  // space for "0x00, " and null terminator
		snprintf(hexValue, sizeof(hexValue), "0x%02x", uint8Array[i]);

		strcat(str, hexValue);

		// add a ", " for all but the last element
		if (i < sizeof(uint8Array) - 1) {
			strcat(str, ", ");
		}

		// newline and tab rules
		if ((i + 1) % 12 == 0) {
			strcat(str, "\n");
			if (i < sizeof(uint8Array) - 1) {
				if ((i + 1) % (12*4) == 0) {
					strcat(str, "\n");
				}
				strcat(str, "\t");
				}
		}
		else if ((i + 1) % 3 == 0) {
		   strcat(str, " ");
		}
	}

	printf("%s\n", str);
	strcat(str, "};");

	Clipboard()->SetTextBuf(str);

}
//---------------------------------------------------------------------------
void UncheckMenuItemsRecursive(TMenuItem *menuItem, bool deepRecurse)
{
	if (menuItem)
	{
		for (int i = 0; i < menuItem->Count; i++)
		{
			TMenuItem *subItem = menuItem->Items[i];
			subItem->Checked = false;
			// Recursively call the function to uncheck items within submenus
			if(deepRecurse) UncheckMenuItemsRecursive(subItem,true);
		}
	}
}
void __fastcall TFormEmphasisPalette::NESPPU1Click(TObject *Sender)
{
	PPU_typeGenerated = 1;
	UpdateCanvas();

	for (int i = 0; i < PopupMenuMethod->Items->Count; i++)
	{
			TMenuItem *menuItem = PopupMenuMethod->Items->Items[i];
			menuItem->Checked = false;
			UncheckMenuItemsRecursive(menuItem, false); //item, deep recurse.
	}

	((TMenuItem*)Sender)->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::RGBPPUPlaychoiceetc1Click(TObject *Sender)
{
	PPU_typeGenerated = 0;
	UpdateCanvas();

	for (int i = 0; i < PopupMenuMethod->Items->Count; i++)
	{
			TMenuItem *menuItem = PopupMenuMethod->Items->Items[i];
			menuItem->Checked = false;
			UncheckMenuItemsRecursive(menuItem, false); //item, deep recurse.
	}

	((TMenuItem*)Sender)->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::SpeedButton30Click(TObject *Sender)
{
	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;

	PopupMenuMethod->Popup(x,y);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::Nintendulator1Click(TObject *Sender)
{

   PPU_typeGenerated = 2;
   UpdateCanvas();

   for (int i = 0; i < PopupMenuMethod->Items->Count; i++)
	{
			TMenuItem *menuItem = PopupMenuMethod->Items->Items[i];
			menuItem->Checked = false;
			UncheckMenuItemsRecursive(menuItem, false); //item, deep recurse.
	}

	((TMenuItem*)Sender)->Checked=true;


}
//---------------------------------------------------------------------------

void __fastcall TFormEmphasisPalette::Mesen1Click(TObject *Sender)
{
	PPU_typeGenerated = 3;
	UpdateCanvas();

	for (int i = 0; i < PopupMenuMethod->Items->Count; i++)
	{
			TMenuItem *menuItem = PopupMenuMethod->Items->Items[i];
			menuItem->Checked = false;
			UncheckMenuItemsRecursive(menuItem, false); //item, deep recurse.
	}

	((TMenuItem*)Sender)->Checked=true;

}
//---------------------------------------------------------------------------


