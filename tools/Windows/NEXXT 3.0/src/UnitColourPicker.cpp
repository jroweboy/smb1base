//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <algorithm>
#pragma hdrstop
#include "UnitMain.h"
#include "UnitColourPicker.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormColourPicker *FormColourPicker;

extern bool bTimerHighlightSubpalSafety;
extern int outPalette[64];
extern int bgPalCur;
extern bool sharedCol0;
extern int palActive;
extern unsigned char bgPal[4*16];
extern int palBank;
extern int colHover;
extern bool cueStats;
Graphics::TBitmap *wheelBuf = new Graphics::TBitmap;

const double sqrt3 = sqrt(3.0);
int timerPassePartoutX=0;
int timerPassePartoutY=0;
// Calculate the initial coordinates for the mask.
TPoint p1 = TPoint(288 / 3, 0);
TPoint p2 = TPoint(0, 288 - (288 / sqrt3));
TPoint p3 = TPoint(288, (288 / sqrt3));

TPoint p4 = TPoint((p1.x+p2.x)/2, (p1.y+p2.y)/2);
TPoint p5 = TPoint((p2.x+p3.x)/2, (p2.y+p3.y)/2);
TPoint p6 = TPoint((p3.x+p1.x)/2, (p3.y+p1.y)/2);


extern int iPreviewSubpalSet;
extern bool holdStats;
bool bPassePartoutOn = false;
bool bInvertHover = false;
bool bMatteHover = false;
bool bToggleInvertL= false;

const char icon_invert[16][32]= {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

const char icon_matte[16][54]= {
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0 },
	{0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0 },
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }
};



//---------------------------------------------------------------------------
__fastcall TFormColourPicker::TFormColourPicker(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::FormShow(TObject *Sender)
{
	Draw();
}
//---------------------------------------------------------------------------
TColor __fastcall  TFormColourPicker::HSLToRGB(float H, float S, float L) {

	float C = (1 - fabs(2 * L - 1)) * S;
	float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
	float m = L - C / 2;
    float R, G, B;

    if (H >= 0 && H < 60) {
		R = C; G = X; B = 0;
	} else if (H >= 60 && H < 120)  {
        R = X; G = C; B = 0;
    } else if (H >= 120 && H < 180) {
        R = 0; G = C; B = X;
    } else if (H >= 180 && H < 240) {
		R = 0; G = X; B = C;
    } else if (H >= 240 && H < 300) {
        R = X; G = 0; B = C;
    } else {
        R = C; G = 0; B = X;
    }

    R = (R + m) * 255;
    G = (G + m) * 255;
    B = (B + m) * 255;

    return (TColor)RGB((int)R, (int)G, (int)B);
}
double __fastcall  TFormColourPicker::ColourDistance(TColor c1, TColor c2) {
	int r1 = GetRValue(c1);
	int g1 = GetGValue(c1);
	int b1 = GetBValue(c1);

	int r2 = GetRValue(c2);
	int g2 = GetGValue(c2);
	int b2 = GetBValue(c2);

	//return std::sqrt(std::pow(r1 - r2, 2) + std::pow(g1 - g2, 2) + std::pow(b1 - b2, 2));
	//use squared channels, much faster
	return (r1 - r2) * (r1 - r2) + (g1 - g2) * (g1 - g2) + (b1 - b2) * (b1 - b2);
}

/*
static inline double Sign(TPoint p1, TPoint p2, TPoint p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
} */

static inline int Sign(int x1, int y1, int x2, int y2, int x3, int y3) {
	return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}

static inline bool IsPointInTriangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3) {
	int d1, d2, d3;


	d1 = Sign(px, py, x1, y1, x2, y2);
	d2 = Sign(px, py, x2, y2, x3, y3);
	d3 = Sign(px, py, x3, y3, x1, y1);

   return !((d1 < 0 || d2 < 0 || d3 < 0) && (d1 > 0 || d2 > 0 || d3 > 0));

}
static inline bool IsColInSubpal(TColor col) {
   //palActive*4
	for(int i=0;i<16;i++){
		if (col == outPalette[bgPal[iPreviewSubpalSet*16+i]]) return true;
	}
	return false;
}
static inline double  calculateDistance(int x1, int y1, int x2, int y2){
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
};

 static inline int calculateDistanceSquared(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return dx * dx + dy * dy;
}
//---------------------------------------------------------------------------
void __fastcall  TFormColourPicker::DrawHSLColourWheel(Graphics::TBitmap *Bitmap) {
	unsigned char *dst;
    int red;
	int green;
	int blue;
	int width = Bitmap->Width;
	int height = Bitmap->Height;
	int centerX = (width) / 2;
	int centerY = (height) / 2;
	int radius = std::min(centerX, centerY)-16;
	 if (width <= 0 || height <= 0) {
		ShowMessage("Error: Invalid dimensions");
		return;
	}

	//Bitmap->PixelFormat = pf24bit;  // Ensure 24-bit color depth

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int dx = x - centerX;
			int dy = y - centerY;
			float distance = sqrt(dx * dx + dy * dy);
			dst=(unsigned char*)Bitmap->ScanLine[y]+x*3;
			if (distance <= radius) {
				if (dx == 0 && dy == 0) {
					// This should not happen since distance <= radius should exclude (0, 0)
					//TColor midpointcol = HSLToRGB(0, 0, 0.5);
					//Bitmap->Canvas->Pixels[x][y] = midpointcol;
					continue;
				}
				float angle = atan2(dy, dx) * 180 / M_PI + 180 - 45; //-45 is to turn warms up, cools down.
				//if(dy<0 || dx<0) angle = -angle;
				if (angle < 0) angle += 360;

				float L_expression = bToggleInvertL? (distance/ radius)*0.93:(1 - (distance/ radius)*0.87);
				float S_expression = bToggleInvertL?  1 : (0.8 + distance / radius)/2;
				float H = angle;
				float S = S_expression;
				float L = L_expression;//1 - (distance/ radius)*0.87; //classic: 0.5;

				TColor col = HSLToRGB(H, S, L);

				TColor closestCol = TColor(outPalette[0]);
				double minDistance = ColourDistance(col, TColor(outPalette[0]));

				 for (int i = 1; i < 64; i++) {
					double currentDistance = ColourDistance(col, TColor(outPalette[i]));
					if (currentDistance < minDistance) {
						minDistance = currentDistance;
						closestCol = TColor(outPalette[i]);
					}
				}


				//Bitmap->Canvas->Pixels[x][y] = closestCol;
				//Bitmap->ScanLine[y]+x*3 = closestCol;
				red = (closestCol & 0xFF);
				green = ((closestCol >> 8) & 0xFF);
				blue = ((closestCol >> 16) & 0xFF);
				*dst++=blue;
				*dst++=green;
				*dst++=red;
				if(dx == -1 && dy==0) Bitmap->Canvas->Pixels[x+1][y] = closestCol;
			} else {
				//Bitmap->Canvas->Pixels[x][y] = clBlack;
				*dst++=bToggleInvertL?(outPalette[0x30] >> 16 & 0xFF):0;
				*dst++=bToggleInvertL?(outPalette[0x30] >> 8 & 0xFF):0;
				*dst++=bToggleInvertL?(outPalette[0x30] & 0xFF):0;
			}
		}
	}
}


void __fastcall  TFormColourPicker::DrawSubpalHighlight(Graphics::TBitmap *Bitmap) {
	TColor col;
	int red;
	int green;
	int blue;
	unsigned char *dst;
	int skip=bToggleInvertL?0:16; //saves a little bit of cpu in normal mode.
	for (int y = skip; y < 288-skip; y++) {
		for (int x = skip; x < 288-skip; x++) {


			col = Bitmap->Canvas->Pixels[x][y];
			//|| (bToggleInvertL && col==(TColor)outPalette[0x30])
			if ((!bToggleInvertL && col==TColor(clBlack))) continue;    //dont do unnecessary calculations
			if (!IsColInSubpal(col))
			{
				blue = (col & 0xFF) / 3;
				green = ((col >> 8) & 0xFF) / 3;
				red = ((col >> 16) & 0xFF) / 3;
				//Bitmap->Canvas->Pixels[x][y] = (TColor)(red | (green << 8) | (blue << 16));
				dst=(unsigned char*)Bitmap->ScanLine[y]+x*3;
				*dst++=red;
				*dst++=green;
				*dst++=blue;
			}
		}
	}
}
void __fastcall  TFormColourPicker::DrawInvertButton(Graphics::TBitmap *Bitmap){
   int r,g,b;
   float f = bInvertHover? 1:1.5;
   unsigned char *dst;
   for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 32; x++) {

			if((!bToggleInvertL && icon_invert[y][x]!=0)){
				r = (0xFF) / f;
				g = (0xFF) / f;
				b = (0xFF) / f;
				dst=(unsigned char*)Bitmap->ScanLine[y+256+8]+(x+8)*3;
				*dst++=r;
				*dst++=g;
				*dst++=b;
			}
			if(bToggleInvertL && icon_invert[y][x]!=0){
				r = bInvertHover? 0x00:0xA0;
				g = bInvertHover? 0x00:0xA0;
				b = bInvertHover? 0x00:0xA0;
				dst=(unsigned char*)Bitmap->ScanLine[y+256+8]+(x+8)*3;
				*dst++=r;
				*dst++=g;
				*dst++=b;
			}

		}
   }
}
//---------------------------------------------------------------------------
 void __fastcall  TFormColourPicker::DrawMatteButton(Graphics::TBitmap *Bitmap){
   int r,g,b;
   float f = bMatteHover? 1:1.5;
   unsigned char *dst;
   for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 54; x++) {

			if((!bToggleInvertL && !bPassePartoutOn && icon_matte[y][x]!=0)
				|| (!bToggleInvertL && bPassePartoutOn && icon_matte[y][x]==0)){
				r = (0xFF) / f;
				g = (0xFF) / f;
				b = (0xFF) / f;
				dst=(unsigned char*)Bitmap->ScanLine[y+256+8]+(x+256+24-54)*3;
				*dst++=r;
				*dst++=g;
				*dst++=b;
			}

			if((bToggleInvertL && !bPassePartoutOn && icon_matte[y][x]!=0)
				|| (bToggleInvertL && bPassePartoutOn && icon_matte[y][x]==0)){
				r = bMatteHover? 0x00:0xA0;
				g = bMatteHover? 0x00:0xA0;
				b = bMatteHover? 0x00:0xA0;
				dst=(unsigned char*)Bitmap->ScanLine[y+256+8]+(x+256+24-54)*3;
				*dst++=r;
				*dst++=g;
				*dst++=b;
			}

		}
   }
}
//---------------------------------------------------------------------------
/*
class TDrawPassePartoutThread : public TThread
{
private:
	Graphics::TBitmap *Bitmap;
	int startY;
	int endY;
	int skip;
	int x1, y1, x2, y2, x3, y3;
	bool bToggleInvertL;
public:
	__fastcall TDrawPassePartoutThread(Graphics::TBitmap *Bitmap, int startY, int endY, int skip,
									   int x1, int y1, int x2, int y2, int x3, int y3, bool bToggleInvertL)
		: TThread(true), Bitmap(Bitmap), startY(startY), endY(endY), skip(skip),
		  x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), bToggleInvertL(bToggleInvertL) {}

	void __fastcall Execute();
};

void __fastcall TDrawPassePartoutThread::Execute()
{
	TColor col;
	int red, green, blue;
	unsigned char *dst;

	for (int y = startY; y < endY; y++) {
		for (int x = skip; x < 288 - skip; x++) {
			//col = Bitmap->Canvas->Pixels[x][y];

			if ((!bToggleInvertL && col == TColor(clBlack))) continue;
			if (!IsPointInTriangle(x, y, x1, y1, x2, y2, x3, y3)) {
				blue = (col & 0xFF) / 3;
				green = ((col >> 8) & 0xFF) / 3;
				red = ((col >> 16) & 0xFF) / 3;

				//dst = (unsigned char*)Bitmap->ScanLine[y] + x * 3;
				//*dst++ = red;
				//*dst++ = green;
				//*dst++ = blue;
			}
		}
	}
}     */
void __fastcall  TFormColourPicker::DrawPassePartout(Graphics::TBitmap *Bitmap) {
	/*
	const int numThreads = 4;
	TDrawPassePartoutThread *threads[numThreads];

	int skip = bToggleInvertL ? 0 : 16; // saves a little bit of cpu in normal mode.

	int sectionHeight = (288 - 2 * skip) / numThreads;
	for (int i = 0; i < numThreads; i++) {
		int startY = skip + i * sectionHeight;
		int endY = (i == numThreads - 1) ? 288 - skip : startY + sectionHeight;

		threads[i] = new TDrawPassePartoutThread(Bitmap, startY, endY, skip,
												 p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, bToggleInvertL);
		threads[i]->FreeOnTerminate = true;
		threads[i]->Resume();
	}

	// Wait for all threads to complete
	for (int i = 0; i < numThreads; i++) {
		threads[i]->WaitFor();
	}
	*/
	TColor col;
	int x1= p1.x;
	int x2= p2.x;
	int x3= p3.x;
	int y1= p1.y;
	int y2= p2.y;
	int y3= p3.y;
	int red;
	int green;
	int blue;
	unsigned char *dst;

	int skip=bToggleInvertL?0:16; //saves a little bit of cpu in normal mode.
	for (int y = skip; y < 288-skip; y++) {
		for (int x = skip; x < 288-skip; x++) {

			unsigned char* scanline = (unsigned char*)Bitmap->ScanLine[y];


			blue = scanline[x * 3];
			green = scanline[x * 3 + 1];
	  		red = scanline[x * 3 + 2];
			col = (TColor)RGB(red, green, blue);

			//|| (bToggleInvertL && col==(TColor)outPalette[0x30])
			if ((!bToggleInvertL && col==TColor(clBlack)) ) continue;    //dont do unnecessary calculations
			if (!IsPointInTriangle(x,y, x1,y1, x2,y2, x3,y3))
			{
				blue = (col & 0xFF) / 3;
				green = ((col >> 8) & 0xFF) / 3;
				red = ((col >> 16) & 0xFF) / 3;
				//Bitmap->Canvas->Pixels[x][y] = (TColor)(red | (green << 8) | (blue << 16));
				dst=(unsigned char*)Bitmap->ScanLine[y]+x*3;
				*dst++=red;
				*dst++=green;
				*dst++=blue;
			}
		}
	}
}

//---------------------------------------------------------------------------


void __fastcall TFormColourPicker::Draw(void)
{
	DrawHSLColourWheel(wheelBuf);
	Image1->Picture->Bitmap->Assign(wheelBuf);
	if(bPassePartoutOn) DrawPassePartout(Image1->Picture->Bitmap);
	DrawInvertButton(Image1->Picture->Bitmap);
	DrawMatteButton(Image1->Picture->Bitmap);


}
//---------------------------------------------------------------------------

void __fastcall TFormColourPicker::RefreshBuf(bool forceNoPasse)
{
	Image1->Picture->Bitmap->Assign(wheelBuf);
	if(!forceNoPasse) if(bPassePartoutOn) DrawPassePartout(Image1->Picture->Bitmap);
	DrawInvertButton(Image1->Picture->Bitmap);
	DrawMatteButton(Image1->Picture->Bitmap);

}

//---------------------------------------------------------------------------

void __fastcall TFormColourPicker::FormActivate(TObject *Sender)
{
	Draw();
}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::FormCreate(TObject *Sender)
{
	Image1->Picture->Bitmap->PixelFormat=pf24bit;
	Image1->Picture->Bitmap->Width=288;
	Image1->Picture->Bitmap->Height=2588;
	Image1->Width=288;
	Image1->Height=288;

	//Graphics::TBitmap *buf = new Graphics::TBitmap;
    wheelBuf->PixelFormat = pf24bit;
	wheelBuf->Width=288;
	wheelBuf->Height=288;
}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{


	//invert diagram button
	if(Shift.Contains(ssLeft) && X>8-3 && X<32+8+3 && Y>256+8-3 && Y<256+16+8+3){
		bToggleInvertL^=true;
		if(bToggleInvertL) FormMain->LabelStats->Caption="Toggle to light center";
		else               FormMain->LabelStats->Caption="Toggle to dark center";
		holdStats=true;

		Draw();
		return;
	}

    //toggle matte button
	if(Shift.Contains(ssLeft) && X>256-24-8-3 && X<256-24-8+54+3 && Y>256+8-3 && Y<256+16+8+3){
		bPassePartoutOn^=true;
		if(bPassePartoutOn) FormMain->LabelStats->Caption="Click to toggle matte OFF.\tOr, press [space] to toggle Matte, while this window is active.\nWhile on, [right-click] anywhere to snap the closest matte node there.\n[Hold right + drag] to move continously. [Scroll] to rotate.\nUse the Matte to find functional or inspiring colour ranges to pick from.";
		else                FormMain->LabelStats->Caption="Click to toggle matte ON.\tOr, press [space] to toggle Matte, while this window is active.\nWhile on, [right-click] anywhere to snap the closest matte node there.\n[Hold right + drag] to move continously. [Scroll] to rotate.\nUse the Matte to find functional or inspiring colour ranges to pick from.";
		holdStats=true;

		Draw();
		return;
	}
	holdStats=false;

	if(Shift.Contains(ssRight) && !(Shift.Contains(ssShift) || Shift.Contains(ssCtrl) || Shift.Contains(ssAlt) ) ){
	if(!bPassePartoutOn) return;
	TPoint midPoint;
	midPoint.x = (p1.x + p2.x +p3.x)/3;
	midPoint.y = (p1.y + p2.y +p3.y)/3;


	int d1 = calculateDistanceSquared(X, Y, p1.x, p1.y);
	int d2 = calculateDistanceSquared(X, Y, p2.x, p2.y);
	int d3 = calculateDistanceSquared(X, Y, p3.x, p3.y);
	//double mpd = calculateDistance(X, Y, midPoint.x, midPoint.y);
	int minDistance = d1;
	TPoint cursorPoint;
	cursorPoint.x = X;
	cursorPoint.y = Y;
	TPoint* closestPoint = &p1;

	if (d2 < minDistance) {
		minDistance = d2;
		closestPoint = &p2;
	}

	if (d3 < minDistance) {
		minDistance = d3;
		closestPoint = &p3;
	}
		/*if (mpd < minDistance) {
			 p1.x += cursorPoint.x-midPoint.x;
			 p2.x += cursorPoint.x-midPoint.x;
			 p3.x += cursorPoint.x-midPoint.x;
			 p1.y += cursorPoint.y-midPoint.y;
			 p2.y += cursorPoint.y-midPoint.y;
			 p3.y += cursorPoint.y-midPoint.y;

		} */
		*closestPoint= cursorPoint;

		Image1->Picture->Bitmap->Assign(wheelBuf);
		if(bPassePartoutOn) DrawPassePartout(Image1->Picture->Bitmap);
		DrawInvertButton(Image1->Picture->Bitmap);
        DrawMatteButton(Image1->Picture->Bitmap);

		//Image1->Refresh();
		return;
	}



	int n=0;
	int entry;


	TColor col = wheelBuf->Canvas->Pixels[X][Y];

	TColor closestCol = TColor(outPalette[0]);
	double minDistance = ColourDistance(col, TColor(outPalette[0]));

	for (int i = 1; i < 64; i++) {
		double currentDistance = ColourDistance(col, TColor(outPalette[i]));
		if (currentDistance < minDistance) {
						minDistance = currentDistance;
						closestCol = TColor(outPalette[i]);
						n = i;
		}
	}
	if (closestCol == TColor(clBlack)) {
		n=0x0F;
	}

	if(Shift.Contains(ssLeft)){

		FormMain->SetUndo();
		if(sharedCol0)     //checks the rule of universal backdrop colour
		{
			entry=palActive*4+bgPalCur;
			if(!(entry&3)|(Shift.Contains(ssAlt))&(!Shift.Contains(ssCtrl))) entry=0;
			bgPal[palBank*16+entry]=n&0xff;
		}
		else
		{
			bgPal[palBank*16+palActive*4+bgPalCur]=n&0xff;
		}
		if(Shift.Contains(ssCtrl))
		{
			bgPalCur++;

			bgPalCur=bgPalCur&3;
			if (bgPalCur==0)
			{
				if(sharedCol0) bgPalCur=1;
				if(Shift.Contains(ssShift))
				{
					palActive++;
					palActive=palActive&3;
				}
			}
		}
		else if(Shift.Contains(ssShift))
		{
			palActive++;
			palActive=palActive&3;
		}
	}

	else if(Shift.Contains(ssRight))
	{
		if(Shift.Contains(ssCtrl))
		{
			bgPalCur++;

			bgPalCur=bgPalCur&3;
			if (bgPalCur==0)
			{
				if(sharedCol0) bgPalCur=1;
				if(Shift.Contains(ssShift))
				{
					palActive++;
					palActive=palActive&3;
				}
			}


		}
		else if(Shift.Contains(ssShift))
		{
			palActive++;
			palActive=palActive&3;
		}

	}



	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::Image1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	bool tmp=bInvertHover;
	bInvertHover=(X>8-3 && X<32+8+3 && Y>256+8-3 && Y<256+16+8+3);
	if(tmp!=bInvertHover) {

		RefreshBuf(false);
	}


	tmp=bMatteHover;
	bMatteHover=(X>256-24-8-3 && X<256-24-8+54+3 && Y>256+8-3 && Y<256+16+8+3);
	if(tmp!=bMatteHover) {

		RefreshBuf(false);
	}

    if((X>8-3 && X<32+8+3 && Y>256+8-3 && Y<256+16+8+3)){
			if(bToggleInvertL) FormMain->LabelStats->Caption="Toggle to light center";
			else               FormMain->LabelStats->Caption="Toggle to dark center";
			holdStats=true;
	}
	else if((X>256-24-8-3 && X<256-24-8+54+3 && Y>256+8-3 && Y<256+16+8+3)){
			if(bPassePartoutOn) FormMain->LabelStats->Caption="Click to toggle matte OFF.\tOr, press [space] to toggle Matte, while this window is active.\nWhile on, [right-click] anywhere to snap the closest matte node there.\n[Hold right + drag] to move continously. [Scroll] to rotate.\nUse the Matte to find functional or inspiring colour ranges to pick from.";
			else                FormMain->LabelStats->Caption="Click to toggle matte ON.\tOr, press [space] to toggle Matte, while this window is active.\nWhile on, [right-click] anywhere to snap the closest matte node there.\n[Hold right + drag] to move continously. [Scroll] to rotate.\nUse the Matte to find functional or inspiring colour ranges to pick from.";
			holdStats=true;
	}
	else  holdStats=false;

	if(Shift.Contains(ssRight) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssShift) && !Shift.Contains(ssAlt)){

		timerPassePartoutX=X;
		timerPassePartoutY=Y;
		TimerPassePartout->Enabled=true;
		//Image1->Refresh();

	}


	int n=0;
	//int entry;
	TColor col = wheelBuf->Canvas->Pixels[X][Y];

	TColor closestCol = TColor(outPalette[0]);
	double minDistance = ColourDistance(col, TColor(outPalette[0]));

	for (int i = 1; i < 64; i++) {
		double currentDistance = ColourDistance(col, TColor(outPalette[i]));
		if (currentDistance < minDistance) {
						minDistance = currentDistance;
						closestCol = TColor(outPalette[i]);
						n = i;
		}
	}
	if (closestCol == TColor(clBlack)) {
		n=0x0F;
	}
	if (n==0x20) n=0x30; //high white

	//colHover=n;
	FormColourPicker->Caption="Colour Rose | $"+IntToHex(n,2);

	if(!holdStats) FormMain->LabelStats->Caption="Click: Pick.\tAlt-click: Pick backdrop.\t\tRight-click: Move Matte Node.\tSpace: Matte on/off.\nCtrl-click: Ripple-pick subpalette.\t\t\tCtrl-rightclick: Skip through subpalette.\nCtrl+Shift-click: Ripple-pick whole set.\t\tCtrl+Shift-rightclick: Skip through whole set.\nShift-click: Pick and step to next subpal.\t\tShift-rightclick: Skip to next subpal.";

	//cueStats=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::Image1MouseLeave(TObject *Sender)
{
	//colHover=-1;
	//cueStats=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::TimerPassePartoutTimer(TObject *Sender)
{

	TimerRefresh->Enabled=false;
	TimerPassePartout->Enabled=false;
	TimerHighlightSubpal->Enabled=false;
	TPoint midPoint;
	midPoint.x = (p1.x + p2.x +p3.x)/3;
	midPoint.y = (p1.y + p2.y +p3.y)/3;


	int d1 = calculateDistanceSquared(timerPassePartoutX, timerPassePartoutY, p1.x, p1.y);
	int d2 = calculateDistanceSquared(timerPassePartoutX, timerPassePartoutY, p2.x, p2.y);
	int d3 = calculateDistanceSquared(timerPassePartoutX, timerPassePartoutY, p3.x, p3.y);
	//double mpd = calculateDistance(X, Y, midPoint.x, midPoint.y);
	int minDistance = d1;
	TPoint cursorPoint;
	cursorPoint.x = timerPassePartoutX;
	cursorPoint.y = timerPassePartoutY;
	TPoint* closestPoint = &p1;

	if (d2 < minDistance) {
		minDistance = d2;
		closestPoint = &p2;
	}

	if (d3 < minDistance) {
		minDistance = d3;
		closestPoint = &p3;
	}
		/*if (mpd < minDistance) {
			 p1.x += cursorPoint.x-midPoint.x;
			 p2.x += cursorPoint.x-midPoint.x;
			 p3.x += cursorPoint.x-midPoint.x;
			 p1.y += cursorPoint.y-midPoint.y;
			 p2.y += cursorPoint.y-midPoint.y;
			 p3.y += cursorPoint.y-midPoint.y;

		} */
		*closestPoint= cursorPoint;

		Image1->Picture->Bitmap->Assign(wheelBuf);
		if(bPassePartoutOn) DrawPassePartout(Image1->Picture->Bitmap);
		DrawInvertButton(Image1->Picture->Bitmap);
    	DrawMatteButton(Image1->Picture->Bitmap);

	}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::TimerHighlightSubpalTimer(
	  TObject *Sender)
{
	//if (bTimerHighlightSubpalSafety) return;
	TimerRefresh->Enabled=false;
	TimerPassePartout->Enabled=false;
	TimerHighlightSubpal->Enabled=false;


	Image1->Picture->Bitmap->Assign(wheelBuf);
	DrawSubpalHighlight(Image1->Picture->Bitmap);
	DrawInvertButton(Image1->Picture->Bitmap);
    DrawMatteButton(Image1->Picture->Bitmap);

	//bTimerHighlightSubpalSafety=true;      //use if we want this in a "mousemove" routine

}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::FormMouseEnter(TObject *Sender)
{
	RefreshBuf(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::FormKeyPress(TObject *Sender, char &Key)
{
	if(Key==VK_SPACE){
		bPassePartoutOn^=true;
		RefreshBuf(false);
	}
	else{
		FormMain->FormKeyPress(Sender, Key);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::FormMouseLeave(TObject *Sender)
{
	RefreshBuf(false);
}
//---------------------------------------------------------------------------

TPoint rotatePoint(double p_x, double p_y, double cosAngle, double sinAngle) {
    TPoint rotated;
	rotated.x = p_x * cosAngle - p_y * sinAngle;
	rotated.y = p_x * sinAngle + p_y * cosAngle;
    return rotated;
}

void __fastcall TFormColourPicker::FormMouseWheel(TObject *Sender,
	  TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
	Handled=true;

	double midPoint_x,midPoint_y;
	midPoint_x = (p1.x + p2.x + p3.x) / 3;
	midPoint_y = (p1.y + p2.y + p3.y) / 3;
	double p1Translated_x, p2Translated_x, p3Translated_x;
	double p1Translated_y, p2Translated_y, p3Translated_y;

	p1Translated_x = p1.x - midPoint_x ;
	p1Translated_y = p1.y - midPoint_y ;

	p2Translated_x = p2.x - midPoint_x ;
	p2Translated_y = p2.y - midPoint_y ;

	p3Translated_x = p3.x - midPoint_x ;
	p3Translated_y = p3.y - midPoint_y ;

	double angle;
	angle = (WheelDelta<0)? M_PI / 180 : -M_PI / 180;
	double cosAngle = cos(angle*36);
	double sinAngle = sin(angle*36);



	double p1Rotated_x = p1Translated_x * cosAngle - p1Translated_y * sinAngle;
	double p1Rotated_y = p1Translated_x * sinAngle + p1Translated_y * cosAngle;

	double p2Rotated_x = p2Translated_x * cosAngle - p2Translated_y * sinAngle;
	double p2Rotated_y = p2Translated_x * sinAngle + p2Translated_y * cosAngle;

	double p3Rotated_x = p3Translated_x * cosAngle - p3Translated_y * sinAngle;
	double p3Rotated_y = p3Translated_x * sinAngle + p3Translated_y * cosAngle;

	 // these constants approximate a compensation for error margins.
	 // Why are they better than .5 in both directions?
	 // I have no idea, during testing they just seem to work better to reduce shrinkage / drift.
	double offset =  (WheelDelta<0)? 0.66:0.67;
	// Translate points back
	p1.x = p1Rotated_x + midPoint_x +offset;
	p1.y = p1Rotated_y + midPoint_y +offset;

	p2.x = p2Rotated_x + midPoint_x +offset;
	p2.y = p2Rotated_y + midPoint_y +offset;

	p3.x = p3Rotated_x + midPoint_x +offset;
	p3.y = p3Rotated_y + midPoint_y +offset;

	TimerRefresh->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::TimerRefreshTimer(TObject *Sender)
{
	TimerRefresh->Enabled=false;
	TimerPassePartout->Enabled=false;
	TimerHighlightSubpal->Enabled=false;


	Image1->Picture->Bitmap->Assign(wheelBuf);
	if(bPassePartoutOn) DrawPassePartout(Image1->Picture->Bitmap);
	DrawInvertButton(Image1->Picture->Bitmap);
	DrawMatteButton(Image1->Picture->Bitmap);

}
//---------------------------------------------------------------------------
void __fastcall TFormColourPicker::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if(Key==VK_SPACE){
	}
	else{
		FormMain->FormKeyUp(Sender, Key, Shift);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormColourPicker::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(Key==VK_SPACE){
	}
	else{
		FormMain->FormKeyDown(Sender, Key, Shift);
	}
}
//---------------------------------------------------------------------------

