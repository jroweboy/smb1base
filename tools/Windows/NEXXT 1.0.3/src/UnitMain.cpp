//------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <math.h>
#include <dir.h>

#include <Clipbrd.hpp>
#include <windows.h>
#include <vector>
#pragma hdrstop

#include "UnitMain.h"
#include "UnitCHREditor.h"
#include "UnitSwapColors.h"
#include "UnitNametableOffset.h"
#include "UnitNESBank.h"
#include "UnitSetSize.h"
#include "UnitName.h"
#include "UnitManageMetasprites.h"
#include "UnitInputNumber.h"
#include "UnitMetaspriteOffset.h"
#include "UnitPreferences.h"
#include "UnitBrush.h"
#include "About.h"
#include "UnitNavigator.h"
#include "UnitNavThread.h"
#include "UnitImportBitmap.h"
#include "UnitLossyDetails.h"
#include "UnitSwapAttributes.h"
#include "UnitLineDetails.h"
#include "UnitBankCHR.h"
#include "UnitCHRbit.h"
#include "UnitPropertyConditions.h"
#include "UnitSwapPatternColour.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;


#include "palette.h"
#include "smallnums.h"


CRITICAL_SECTION CriticalSection;

AnsiString strStat;
AnsiString tmpStat;
Graphics::TBitmap *BufBmpName;
Graphics::TBitmap *BufBmpTiles;
extern Graphics::TBitmap *bufBmp;

bool openByFileDone=false;
bool bProcessDrawNavOn=false;
int iGlobalAlpha;
int iRadioOpenSave;

bool bSnapToScreen;
bool bWarnMsprYellow;
bool bWarnMsprOrange;
bool bWarnMsprRed;

bool bExportPalFilename;
bool bExportPalSet;

bool bKeyEscape;
bool bIgnoreKey=true;
bool bDrawMarqSwitch=false;
int basePalette[64];

int outPalette[64];
int ppuMask;

int ppuMaskSet[4];
unsigned char palBuf[4*16];
unsigned char palImportBuf[4*16]; //used by bmp import specifically.
unsigned char bgPal[4*16];
unsigned char bgPalUndo[4*16];
unsigned char bgPalCheckpoint[4*16];
unsigned char bgPalUndoCheckPoint[4*16];

bool bSmudge=0;
int iSmudge=0;
int bgPalCur;
int bgPalCurOld;
int palActive;
int palActiveOld;

int lineToolRoster=0; //chooses line tool
int lineToolAlt=0;
int lineToolX=0;
int lineToolY=0;
bool lineRosterEnable[4]; //whether to include in roster
int lineDashLen=1;
int globalLineSenderMode=0;  //used to communicate with async line keys which mode to refresh.
int penActive; //chooses drawing mode
int tileActive;
int bankActive=0;
int nameXC;
int nameYC;
int tileXC;
int tileYC;
int palHover;
int palColHover;
int colHover;
int palDragColor;
int palBank;

//int tileViewTagCur=0;
int tileViewTagBuf=0;
unsigned char tileViewTable[256]; //used to indirect tile arrangements wihtout changing physical memory.

//unsigned char sprModeTableReverse[256];
int lineDownX;
int lineDownY;
int lineUpX= -1;
int lineUpY= -1;


extern int mvOriginX;
extern int mvOriginY;
extern int CHR_mmX;
extern int CHR_mmY;
bool lineDrawing=false;
bool isMovingLine;
bool isNudgingLine;
bool bOldLine;
extern unsigned char arr_linePlotCHR[128*128];
extern unsigned char arr_linePlotCHR2[128*128];
extern bool bForbidPaint;
extern int lineOffX; //x0y0 adjust nudge offset
extern int lineOffY;


AnsiString propCHRlabel[8];
bool bClearUint32Array=false;
uint32_t tileProperties[512];
uint32_t tilePropertiesUndo[512];
uint32_t checkpointTileProperties[512];
uint32_t checkpointTilePropertiesUndo[512];


uint32_t propCopy[256];
bool btnStateProps[8];

char propConditional[8]={0,0,0,0,0,0,0,2};   //0=no, 1=yes, 2=all
char propConditionalUndo[8]={0,0,0,0,0,0,0,2};
char checkpointPropConditional[8]={0,0,0,0,0,0,0,2};
char checkpointPropConditionalUndo[8]={0,0,0,0,0,0,0,2};

bool propCondition[4][8];
bool propConditionUndo[4][8];
bool checkpointPropCondition[4][8];
bool checkpointPropConditionUndo[4][8];

bool propShowCondMap=true;
bool propShowCondTile=false;
extern int hoverBtn;
extern bool collision_specific;

unsigned char chr[8192];
unsigned char chrBuf[8192]; //in preparation for stroke-oriented ink algo. Also by ColourSwap.
unsigned char chrImportBuf[8192]; //used by bitmap import specifically.
unsigned char chrBufFill[8192];
unsigned char chrBufLine[8192];
unsigned char chrQuickLine[8192];
unsigned char chrCopy[4096];





unsigned int chrBanks = 4; //4 is default starting banks.
unsigned int undo_chrBanks = 4;
unsigned int checkpoint_chrBanks = 4;
unsigned int undo_checkpoint_chrBanks = 4;
unsigned int curViewCHRbankCanvas2=0;

unsigned char *chrBank = (char*)calloc((chrBanks*4096), sizeof(char));
unsigned char *undo_chrBank = (char*)calloc((chrBanks*4096), sizeof(char));
unsigned char *checkpoint_chrBank = (char*)calloc((chrBanks*4096), sizeof(char));
unsigned char *undo_checkpoint_chrBank = (char*)calloc((chrBanks*4096), sizeof(char));
unsigned char *tmp_chrBank = (char*)calloc((chrBanks*4096), sizeof(char));

std::vector<std::string> chrBankLabels;
std::vector<std::string> undo_chrBankLabels;
std::vector<std::string> checkpoint_chrBankLabels;
std::vector<std::string> undo_checkpoint_chrBankLabels;
std::vector<std::string> tmp_chrBankLabels;
//segmented chr bank windows
unsigned int chrA_id[16]=				 { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};   //default 0,25k bank mappings for set A
unsigned int chrB_id[16]=				 {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};   //default 0,25k bank mappings for set B
unsigned int undo_chrA_id[16]=			 { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};   //default 0,25k bank mappings for set A
unsigned int undo_chrB_id[16]=			 {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};   //default 0,25k bank mappings for set B
unsigned int checkpoint_chrA_id[16]=	 { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};   //default 0,25k bank mappings for set A
unsigned int checkpoint_chrB_id[16]=	 {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};   //default 0,25k bank mappings for set B
unsigned int undo_checkpoint_chrA_id[16]={ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};   //default 0,25k bank mappings for set A
unsigned int undo_checkpoint_chrB_id[16]={16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};   //default 0,25k bank mappings for set B



extern TRect curCursor;
extern TRect bnkCursor;
extern TRect curSelection;
extern TRect bnkSelection;
extern bool isBnkCursor;
unsigned char subpalCopy[4]; //for copying a single subpalette

unsigned char nameTable[NAME_MAX_SIZE];
unsigned char attrTable[ATTR_MAX_SIZE];
unsigned char tmpNameTable[NAME_MAX_SIZE];
unsigned char tmpAttrTable[ATTR_MAX_SIZE];

int importBMP_tile_count;
int importBMP_tile_count_reduced;

int nameTableViewX;
int nameTableViewY;

TRect nameSelection;
int nameCopyWidth;
int nameCopyHeight;
int nameTableWidth;
int nameTableHeight;
int buf_nameWidth;      //used by bmp import to restore
int buf_nameHeight;

unsigned char nameCopy[NAME_MAX_SIZE];
unsigned char attrCopy[NAME_MAX_SIZE];

unsigned char undoChr[8192];
unsigned char undoNameTable[NAME_MAX_SIZE];
unsigned char undoAttrTable[ATTR_MAX_SIZE];

unsigned char checkpointChr[8192];
unsigned char checkpointNameTable[NAME_MAX_SIZE];
unsigned char checkpointAttrTable[ATTR_MAX_SIZE];

unsigned char undoCheckpointChr[8192];
unsigned char undoCheckpointNameTable[NAME_MAX_SIZE];
unsigned char undoCheckpointAttrTable[ATTR_MAX_SIZE];

int undoNameTableWidth;
int undoNameTableHeight;

int checkpointNameTableWidth;
int checkpointNameTableHeight;

int undoCheckpointNameTableWidth;
int undoCheckpointNameTableHeight;



bool unsavedChanges;

TRect chrSelection;
TRect chrSelBuf;
TRect nameSelBuf;
int chrCopyWidth;
int chrCopyHeight;
bool chrCopyRect;
unsigned char chrSelected[256];
bool bMultiSelectRemoveMode;
bool chrSelectRect;
//global drag source state
bool bImageNameAccepted;
bool bImageTileAccepted;
//global mousedown state
int txDown;    //tileset
int tyDown;
int nxDown;   //nametable/screen/map
int nyDown;

bool bOutsideSel;
bool bDrawDestShadow;
bool bMouseOverNam;
bool bMouseOverNav;
bool bAllowLockMessage=true;

int typeInFontOffset;
int typeInASCIIOffset;

int prevMouseY;
int prevMouseX; //FG: reserved for potential features to come

unsigned char metaSpritesBuf[256*64*4];
unsigned char metaSprites[256*64*4];
unsigned char undoMetaSprites[256*64*4];
unsigned char checkpointMetaSprites[256*64*4];
unsigned char undoCheckpointMetaSprites[256*64*4];

//BROKE STUDIO
AnsiString metaSpriteBankName;
AnsiString metaSpriteNames[256];
AnsiString metaSpriteNamesUndo[256];
AnsiString metaSpriteNamesCheckpoint[256];
AnsiString metaSpriteNamesCheckpointUndo[256];

AnsiString tmpMetaSprName;
//AnsiString metaSpriteNameCopy;
char metaSpriteNameCopy[256];
//

int spriteActive;
int metaSpriteActive;

int spriteGridX;
int spriteGridY;
int undoSpriteGridX;
int undoSpriteGridY;

int checkpointSpriteGridX;
int checkpointSpriteGridY;
int undoCheckpointSpriteGridX;
int undoCheckpointSpriteGridY;


int spriteHover;
int spriteDrag;
int spriteDragX;
int spriteDragY;

int CHREditorHover;
int bigTilesHover;

unsigned char metaSpriteCopy	[64*4];

//metasprite direct animation properties
char cMsprLabel[256][31];

int CHRLatestOffset;
int CHRLatestSize;

int uiScale;


int CF_CHR;
int CF_NAM;
int CF_META;
int CF_PAL;
int CF_SUBPAL;




int tmpContinousIncDecTimer;
int continousIncDecDuration=3;

bool sharedCol0 = true;
bool safeColsOnly = true;


bool clickV;
bool clickC;

bool bBufCtrl;
bool bBufShift;
bool bBufAlt;
bool bBufVK_3; //grave accent

int cf_palRotation; //used to paste several subpalette entries in sequence from clipboard
int blendMode;

int oldNameXC;
int oldNameYC;
//int oldTileXC;
//int oldTileYC;
int oldCHRRectWdt;
int oldCHRRectHgt;

int returnCarriagePos;
int nullTile;

int fillPal;

int tileSelRectWdt=1;
int tileSelRectHgt=1;

bool holdStats;
bool cueStats;
bool cueUpdateMetasprite;
bool cueUpdateMM;
bool cueUpdateTiles;
bool cueUpdateNametable;
bool cueUpdateNametableNoRepaint;
bool cueUpdateNTstrip;
int  cntNTstrip=0;
bool cueRemoveDoubles;
bool cueCHRdraw;
bool cueCHRdrawAll;

bool cueNT;
bool throttleOK = true;
bool throttleOKselRect = false;
bool throttleOKtileSelRect = false;
bool mouseDraggedNTSel = false;
bool mouseDraggedTileSel = false;

bool bbigTileMouseDown = false;
bool bForceNameScale = false;

bool isLastClickedMetaSprite;
bool isLastClickedSpriteList;
bool isLastClickedNametable;
bool isLastClickedTiles;

bool byte1bool;
bool db1bool;

bool acceptPalDrag;
int palDragVal;


bool bBrushMask[4];
bool bBrushMaskBuffer[4];

//these are "factory preset" startup settings. Some are chars for reusing an old nesst routine in a slightly hacky way.
//those are cast as temporary ints later when necessary (in the preferences dialogue).
unsigned char prefStartScale[1]={2};
unsigned char prefStartCol[1]={3};
unsigned char prefStartSubpal[1]={0};

bool prefStartGridShow=false;
bool prefStartGrid1=false;
bool prefStartGrid2=true;
bool prefStartGrid4=false;

bool prefStartGrid32x30=false;
bool prefStartGridPixelCHR=true;

bool prefStartShowCHR=false;
bool prefStartShowMM=false;

bool prefStartScale2x=false;
bool prefStartScale3x=false;
bool prefStartScale4x=false;


bool prefStartCol0=false;
bool prefStartCol1=false;
bool prefStartCol2=false;
bool prefStartCol3=false;

bool prefStartSubpal0=false;
bool prefStartSubpal1=false;
bool prefStartSubpal2=false;
bool prefStartSubpal3=false;

RECT curMainWinPos;

TRect destRect;

int iSwap_WhichSubpal=0;
int iSwap_Pal0Subpal=0;

inline int min(int a, int b) {
	return (a < b) ? a : b;
}
inline int max(int a, int b) {
    return (a > b) ? a : b;
}
inline int ViewTablePos(int c) {
	int i;
	for (i = 0; i<256; i++) {
		if (tileViewTable[i] == c) {
			return i;
		}
	}
	return 0;
}

inline const char * const BoolToString(bool b)
{
	return b?"true":"false";
}

inline bool GetTileHasContents(char* tile)
{
	for(int i=0;i<16;i++)
	{
		if(tile[i]) return true;
	}
	return false;
}

inline int count_diff_bits(unsigned char a) {
	int count = 0;
	//unsigned char first_bit = a & 1;
    int i = 0;
    
    //check first 7 bits against the more significant bit
    while (i<7) { 
        
        count += (a & 1) ^ ((a >> 1) & 1);
        a >>= 1;
        i++;
    }
	//a >>=1; //one last bit shift down to compare the last bit with 'first_bit'

	//count += (a & 1) ^ first_bit;
	return count;
}
inline int count_diff_bytes(char *ptr, int size, char mask) {
	int difference = 0;
	int i;
	for (i = 0; i < size-1; i++) {
		char xor_result = ptr[i] ^ ptr[i+1];
		for (int j = 0; j < 8; j++) {
			difference += ((xor_result >> j) & 1);
		}
	}
		/*
		i++;
			char xor_result2 = ptr[i] ^ ptr[0];
			for (int j = 0; j < 8; j++) {
					difference += ((xor_result2 >> j) & 1);
				}
	*/
	return difference;
}

int DrawTimerDuration=500;
const int masterPaletteColumns=14; //project: convert to 16

const unsigned char bgPalDefault[5*16]={

	//nesst classic grayscale
	0x0f,0x00,0x10,0x30,
	0x0f,0x00,0x10,0x30,
	0x0f,0x00,0x10,0x30,
	0x0f,0x00,0x10,0x30,

	//nesst classic startup ("A")
	0x0f,0x00,0x10,0x30,
	0x0f,0x01,0x21,0x31,
	0x0f,0x06,0x16,0x26,
	0x0f,0x09,0x19,0x29,

	//nesst classic "B"
	0x0f,0x11,0x21,0x31,
	0x0f,0x12,0x22,0x32,
	0x0f,0x13,0x23,0x33,
	0x0f,0x14,0x24,0x34,

	//nesst classic "C"
	0x0f,0x15,0x25,0x35,
	0x0f,0x16,0x26,0x36,
	0x0f,0x17,0x27,0x37,
	0x0f,0x18,0x28,0x38,

	//nesst classic "D"
	0x0f,0x19,0x29,0x39,
	0x0f,0x1a,0x2a,0x3a,
	0x0f,0x1b,0x2b,0x3b,
	0x0f,0x1c,0x2c,0x3c

};

void CheckAndCreateFolder(const std::string& folderName)
{
	//usage:
	//CheckAndCreateFolder("backups");


	// Get the path to the executable
	char exePath[MAXPATH];
    GetModuleFileName(NULL, exePath, MAXPATH);

	// Extract the "home" folder of the executable path,
	// then add desired folder name to it for evaluation
	char homePath[MAXPATH];
	strncpy(homePath, exePath, strrchr(exePath, '\\') - exePath + 1);
    strcat(homePath, folderName.c_str());

	// Make folder if not existing
    struct ffblk fileInfo;
    int result = findfirst(homePath, &fileInfo, FA_DIREC);
	if (result == 0)	findclose(&fileInfo);
	else				mkdir(homePath);

}

void SetCheckMenuItem(TMenuItem *m, int tag)
{
	for (int i = 0; i < m->Count; i++) {
		TMenuItem *item = m->Items[i];
		if (item->Tag == tag) {
			item->Checked = true;
			break;
		}
	}
}

void SetCheckPopupMenuItem(TPopupMenu *PopupMenu, int tag)
{
  for (int i = 0; i < PopupMenu->Items->Count; i++)
  {
    TMenuItem *Item = PopupMenu->Items->Items[i];
    Item->Checked = (Item->Tag == tag);
  }
}

 void RemoveCharacter(char *str, char character)
{
    int i, j = 0;
    int n = strlen(str);
    char *newstr = (char *)malloc(n);
    for (i = 0; i < n; i++)
    {
        if (str[i] != character)
        {
            newstr[j] = str[i];
            j++;
        }
    }
    newstr[j] = '\0';
    strcpy(str, newstr);
    free(newstr);
}

AnsiString reg_load_str(const char *name,AnsiString def)
{
	HKEY key;
	DWORD type,size;
	char *str;
	AnsiString ret;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_SECTION,0,KEY_READ,&key)!=ERROR_SUCCESS) return def;

	type=REG_SZ;
	size=1024;
	str=(char*)malloc(size);
	if(RegQueryValueEx(key,name,NULL,&type,(unsigned char*)str,&size)!=ERROR_SUCCESS) ret=def; else ret=str;
	free(str);
	RegCloseKey(key);

	return ret;
}



void reg_save_str(const char *name,AnsiString str)
{
	HKEY key;
	DWORD disp;

	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE,REG_SECTION,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&disp)!=ERROR_SUCCESS) return;

	RegSetValueEx(key,name,0,REG_SZ,str.c_str(),strlen(str.c_str())+1);
	RegCloseKey(key);
}



AnsiString num_to_3char(int i)
{
	AnsiString str;

	if(i<0) str="-"; else str=" ";

	i=abs(i);

	if(i>=100) str+=IntToStr((i/100)%10);
	if(i>=10 ) str+=IntToStr((i/10 )%10); else str+=" ";

	str+=IntToStr(i%10);

	return str;
}



int get_file_size(FILE *file)
{
	int size;

	fseek(file,0,SEEK_END);
	size=ftell(file);
	fseek(file,0,SEEK_SET);

	return size;
}



int read_dword(unsigned char *data)
{
	return data[0]+(data[1]<<8)+(data[2]<<16)+(data[3]<<24);
}

bool __fastcall TFormMain::ConfirmNameSelection(void)
{
	return (nameSelection.left>=0&&nameSelection.top>=0);
}

int __fastcall TFormMain::AttrGet(int x,int y, bool doBuffer, bool returnByte)
{
	int pal;

	if(doBuffer) pal=attrCopy[y/4*((nameTableWidth+3)/4)+x/4];
	else pal=attrTable[y/4*((nameTableWidth+3)/4)+x/4];

	//return full byte
	if(returnByte) return pal;

	//return extracted value
	if(x&2) pal>>=2;
	if(y&2) pal>>=4;

	return pal&3;
}



void __fastcall TFormMain::AttrSet(int x,int y,int pal,bool doBuffer)
{
	int pp,mask;

	pp=y/4*((nameTableWidth+3)/4)+x/4;

	mask=3;
	pal=pal&3;

	if(x&2)
	{
		pal<<=2;
		mask<<=2;
	}
	if(y&2)
	{
		pal<<=4;
		mask<<=4;
	}

	if(pp<nameTableWidth*nameTableHeight)
	{

		if(doBuffer)attrCopy[pp]=(attrCopy[pp]&(mask^255))|pal;
		else attrTable[pp]=(attrTable[pp]&(mask^255))|pal;
	}
}



int encode_rle(unsigned char *src,int size,unsigned char *dst)
{
	int stat[256];
	int i,tag,sym,sym_prev,len,ptr;

	memset(stat,0,sizeof(stat));

	for(i=0;i<size;++i) ++stat[src[i]];

	tag=-1;

	for(i=0;i<256;++i)
	{
		if(!stat[i])
		{
			tag=i;
			break;
		}
	}

	if(tag<0) return -1;

	ptr=0;
	len=1;
	sym_prev=-1;

	dst[ptr++]=tag;

	for(i=0;i<size;++i)
	{
		sym=src[i];

		if(sym_prev!=sym||len>=255||i==size-1)
		{
			if(len>1)
			{
				if(len==2)
				{
					dst[ptr++]=sym_prev;
				}
				else
				{
					dst[ptr++]=tag;
					dst[ptr++]=len-1;
				}
			}

			dst[ptr++]=sym;

			sym_prev=sym;

			len=1;
		}
		else
		{
			++len;
		}
	}

	dst[ptr++]=tag;	//end of file marked with zero length rle
	dst[ptr++]=0;

	return ptr;
}



int name_size(void)
{
	return nameTableWidth*nameTableHeight;
}



int attr_size(void)
{
	return (nameTableWidth+3)/4*((nameTableHeight+3)/4);
}



AnsiString save_data(const char *name,unsigned char *src,int size,int format,bool rle)
{
	char arrname[256],temp[256];
	FILE *file;
	int i,tag,pp,len,sym,sym_prev,type;
	unsigned char *dst;

	if(rle)
	{
		dst=(unsigned char*)malloc(size*2);

		size=encode_rle(src,size,dst);

		if(size<0)
		{
			free(dst);
			return "No unused values found (tiles and/or attributes), can't be saved as RLE due to limitations of the format.";
		}
	}
	else
	{
		dst=src;
	}

	i=strlen(name);

	while(i)
	{
		if(name[i]=='\\'||name[i]=='/')
		{
			i++;
			break;
		}
		i--;
	}

	strcpy(arrname,&name[i]);

	for(i=strlen(arrname)-1;i>=0;i--)
	{
		if(arrname[i]=='.')
		{
			arrname[i]=0;
			break;
		}
	}

	if(arrname[0]!='_'&&!(arrname[0]>='a'&&arrname[0]<='z')&&!(arrname[0]>='A'&&arrname[0]<='Z'))
	{
		strcpy(temp,arrname);
		strcpy(arrname,"n");
		strcat(arrname,temp);
	}

	switch(format)
	{
	case SAVE_FORMAT_H:
		{
			file=fopen(name,"wt");

			if(!file) break;

			fprintf(file,"const unsigned char %s[%i]={\n",arrname,size);

			for(i=0;i<size;i++)
			{
				fprintf(file,"0x%2.2x",dst[i]);
				if(i<size-1) fprintf(file,",");
				if((i&15)==15||i==(size-1)) fprintf(file,"\n");
			}

			fprintf(file,"};\n");
			fclose(file);
		}
		break;

	case SAVE_FORMAT_ASM:
		{
			file=fopen(name,"wt");

			if(!file) break;

			fprintf(file,"%s:\n",arrname);

			for(i=0;i<size;i++)
			{
				if(!(i&15))
				{
				if(byte1bool) fprintf(file,"\t.byte ");      //quick workaround.
				if(db1bool) fprintf(file,"\t.db ");
				}
				fprintf(file,"$%2.2x",dst[i]);
				if(i<size-1&&(i&15)<15) fprintf(file,",");
				if((i&15)==15||i==(size-1)) fprintf(file,"\n");
			}

			fprintf(file,"\n");
			fclose(file);
		}
		break;

	case SAVE_FORMAT_BIN:
		{
			file=fopen(name,"wb");

			if(!file) break;

			fwrite(dst,size,1,file);
			fclose(file);
		}
		break;
	}

	if(rle) free(dst);

	return "";
}



inline void mem_exchange(unsigned char *src,unsigned char *dst,int len)
{
	int temp;

	while(len)
	{
		temp=*src;
		*src++=*dst;
		*dst++=temp;
		len--;
	}
}
inline void mem_32exchange(uint32_t *src,uint32_t *dst,int len)
{
	int temp;
    len = len/sizeof(uint32_t);
	while(len)
	{
		temp=*src;
		*src++=*dst;
		*dst++=temp;
		len--;
	}
}
 inline void swap_bool_2d(bool arr1[][8], bool arr2[][8], size_t rows, size_t cols)
{
    const size_t max_rows = 4; // maximum possible rows
    const size_t max_cols = 8; // maximum possible columns
    bool temp[max_rows][max_cols];

    if (rows > max_rows || cols > max_cols) {
        // handle error: input arrays are too large
        return;
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            temp[i][j] = arr1[i][j];
            arr1[i][j] = arr2[i][j];
            arr2[i][j] = temp[i][j];
		}
	}
}
bool CheckTag(const char* text, size_t size, const char* tag) {
	size_t tag_size = strlen(tag);
	if (size < tag_size) {
		// The string is smaller than the tag, so the tag cannot be present.
		return false;
	}

    for (size_t i = 0; i <= size - tag_size; ++i) {
		if (memcmp(text + i, tag, tag_size) == 0) {
			// The tag was found.
			return true;
		}
	}

	// The tag was not found.
	return false;
}

void Bresenham_line(int x0, int y0, int x1, int y1, int w, unsigned char *array,bool isRightAngle) {
	int x,y; //for counters

	int dash = lineDashLen +1;
	bool bDots = FormLineDetails->btnDots->Down;
	bool bDashes  = FormLineDetails->btnDashes->Down;
	if (bDots==false && bDashes==false) {bDots=true; bDashes=true; }

	int dx = (abs(x1 - x0)+1)<<1;   //edit. "classic" is dx = abs((x1 - x0)
	int dy = (abs(y1 - y0)+1)<<1;   //edit. "classic" is abs((y1 - y0)
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err;
	int max_iterations = max(abs(x1 - x0), abs(y1 - y0)); //may be used as a reference to abort plot prematurely somewhere along the line.

	int maxi_subtract = isRightAngle? abs(lineToolAlt) : 0;

	max_iterations=max_iterations-maxi_subtract;

	int cnt=0;

	if(dx > dy)  //x is fast axis
	{
		err = (dy - dx) >>1;
		for (x=x0,y=y0; sx<0 ? x>=x1:x<=x1; x+=sx) {

			int plOff = max(min(y*w + x,128*128-1),0);
			if(x%dash == 0){ 	if(bDots)    *(array + plOff) = 1; }//plot
			else           {    if(bDashes)  *(array + plOff) = 1; }

			err += dy;
			if(err<<1 >= dx) {y+=sy; err-=dx;}

			//used for user controlled early abort.
			cnt++;
			if(cnt>max_iterations) return;

		}
	}
	else  //y is fast axis
	{
		err = (dx - dy) >>1;                           //this reverses the offset introduced at init
		for (x=x0,y=y0; sy< 0 ? y>=y1:y<=y1; y+=sy) {


			if(y%dash == 0){ 	if(bDots)    *(array + x +y*w) = 1; }//plot
			else           {    if(bDashes)  *(array + x +y*w) = 1; }


			err += dx;
			if (err<<1 >= dy) {x+=sx; err-=dy;}

            //used for user controlled early abort.
			cnt++;
			if(cnt>max_iterations) return;

		}
	}
}
void Bresenham_2line(float x0, float y0, float x2, float y2, int pw, unsigned char* ptr, int bi) {
	//this method is primarily used when it's required of a straight line
	//to have a midpoint calculated.
    //That's currently some cases related to taper in/out. 

	float mx1, mx2, my1, my2;
	float corr = 0.5f;
    float div = 2.0f;
	float ix=0.0f, iy=0.0f;
	float f=0.99f;

	//double taper midpoint offsets
	if(bi==0){ ix=0.0f; iy=0.0f;}
	if(bi==1){ ix=f; 	iy=0.0f;}
	if(bi==2){ ix=0.0f; iy=f;}
	if(bi==3){ ix=f; 	iy=f;}

	if(bi==-1){ix=-f; 	iy=0.0f;}
	if(bi==-2){ix=0.0f; iy=-f;}
	if(bi==-3){ix=-f; 	iy=-f;}

	if(y0 < y2 && x0 < x2) {
		mx1 = (x0 + x2) / div;
		my1 = (y0 + y2) / div;

		mx2 = (x0 + x2) / div + corr;
		my2 = (y0 + y2) / div + corr;
	}
	else if(y0 >= y2 && x0 < x2) {
		mx1 = (x0 + x2) / div;
		my1 = (y0 + y2) / div + corr;

		mx2 = (x0 + x2) / div + corr;
		my2 = (y0 + y2) / div;
	}
	else if(y0 < y2 && x0 >= x2) {
		mx1 = (x0 + x2) / div + corr;
		my1 = (y0 + y2) / div;

		mx2 = (x0 + x2) / div;
		my2 = (y0 + y2) / div + corr;
	}
	else {
		mx1 = (x0 + x2 ) / div + corr;
		my1 = (y0 + y2 ) / div + corr;

		mx2 = (x0 + x2 ) / div;
		my2 = (y0 + y2 ) / div;
    }

    //apply center taper if valid

	if (lineToolRoster<0) {mx1=mx1-ix; my1=my1-iy;
						   mx2=mx2-ix; my2=my2-iy;}


	//only this clause matters, currently. Full condition kept
	//in case i decide to make to make the middle roster choice polar.  
	else                  {mx1=mx1+ix; my1=my1+iy;
						   mx2=mx2+ix; my2=my2+iy;}

	Bresenham_line(x0, y0, (int)mx1+lineToolAlt, (int)my1+lineToolAlt, pw, ptr, false);
	Bresenham_line((int)mx2+lineToolAlt, (int)my2+lineToolAlt, x2, y2, pw, ptr, false);
	//Bresenham_line(x2, y2, (int)mx2+lineToolAlt, (int)my2+lineToolAlt, pw, ptr);
}
void Bresenham_angle(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array, bool knee, bool join, int bi) {

	int ix, iy;
	//bool bTaperEnable =FormLineDetails->CheckEnableBrush->Checked;
	//bool bTaper2		=((FormLineDetails->btnTaper2->Down) && (bTaperEnable));
	//bool bTaperIn		=((FormLineDetails->btnTaperIn->Down) && (bTaperEnable));
	//bool bTaperOut	=((FormLineDetails->btnTaperOut->Down) && (bTaperEnable));
	bool bIsRightAngle = (!knee) && join;
	//double taper midpoint offsets
	if(bi==0){ ix=0; iy=0;}

	if(bi==1){ ix=1; iy=0;}
	if(bi==2){ ix=0; iy=1;}
	if(bi==3){ ix=1; iy=1;}

	if(bi==-1){ ix=-1; iy=0;}
	if(bi==-2){ ix=0; iy=-1;}
	if(bi==-3){ ix=-1; iy=-1;}



	float f;
	if(knee) //simulates hard knee curve
	{
		f=0.66;        //the rounding error *down* is important.


		int mx = (x0 + x2) / 2;   			//dirty midpoint calc
		int my = (y0 + y2) / 2;

		int dix = (int)((x1 - mx) * f );     //midpoint displacement
		int diy = (int)((y1 - my) * f );
		x1 = mx + dix;
		y1 = my + diy;


	//further operations to soften the knee can be done here
	//if softened, add a bezier to connect.
	}
	else
	{

		x1=(int)x1+ix;
		y1=(int)y1+iy;
	}
	//apply center taper if valid
	/*
	int sxy = lineToolRoster<0 ? 0 : 1;

	if (lineToolRoster<0 ) {
		if((bTaper2) && ((!bTaperIn) && (!bTaperOut)))  {x1=x1+ix; y1=y1+iy;}
		else  {x1=x1-ix; y1=y1-iy;}
	}
	else                  {x1=x1+ix; y1=y1+iy;}

	*/
	if (lineToolRoster<0 ) {x1=x1-ix; y1=y1-iy;
							x0=x0-ix; y0=y0-iy;

	}

	else                  {x1=x1+ix; y1=y1+iy;}
	

	Bresenham_line(x0,y0, x1,y1,  w, array,bIsRightAngle);
	//Bresenham_line(x1,y1, x2,y2,  w, array);

	if(join) Bresenham_line(x2,y2, x1,y1,  w, array,bIsRightAngle);
	else 	 Bresenham_line(x1,y1, x2,y2,  w, array,bIsRightAngle);
}
void plotQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array)
{ /* plot a limited quadratic Bezier segment */
    int cnt=0;
	int dash = lineDashLen +1;
	bool bDots = FormLineDetails->btnDots->Down;
	bool bDashes  = FormLineDetails->btnDashes->Down;
	if (bDots==false && bDashes==false) {bDots=true; bDashes=true; }


	int sx = x2-x1, sy = y2-y1;
	long xx = x0-x1, yy = y0-y1, xy; /* relative values for checks */
	double dx, dy, err, cur = xx*sy-yy*sx; /* curvature */


	//assert(xx*sx <= 0 && yy*sy <= 0); /* sign of gradient must not change */
	if(!(xx*sx <= 0 && yy*sy <= 0)) return;

	if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) { /* begin with longer part */
		x2 = x0; x0 = sx+x1; y2 = y0; y0 = sy+y1; cur = -cur; /* swap P0 P2 */
	}
	if (cur != 0) { /* no straight line */
		xx += sx; xx *= sx = x0 < x2 ? 1 : -1; /* x step direction */
		yy += sy; yy *= sy = y0 < y2 ? 1 : -1; /* y step direction */
		xy = 2*xx*yy; xx *= xx; yy *= yy; /* differences 2nd degree */
		if (cur*sx*sy < 0) { /* negated curvature? */
			xx = -xx; yy = -yy; xy = -xy; cur = -cur;
		}
		dx = 4.0*sy*cur*(x1-x0)+xx-xy; /* differences 1st degree */
		dy = 4.0*sx*cur*(y0-y1)+yy-xy;
		xx += xx; yy += yy; err = dx+dy+xy; /* error 1st step */

		do {
			// plot curve
			int plOff = max(min(y0*w + x0,128*128-1),0);

			if(cnt%dash == 0){ 	   if(bDots)    *(array+plOff) = 1; }
			else              {    if(bDashes)  *(array+plOff) = 1; }
			cnt++;
			if (x0 == x2 && y0 == y2) return; // last pixel -> curve finished
			y1 = 2*err < dx; // save value for test of y step
			if (2*err > dy) { x0 += sx; dx -= xy; err += dy += yy; } // x step
			if ( y1 ) { y0 += sy; dy -= xy; err += dx += xx; } // y step
			if (cnt>256) break;
		} //while ((dy < 0 && dx > 0) || (dx < 0 && abs(dx) > abs(x2-x0)) || (dy > 0 && abs(dy) > abs(y2-y0)));
		while (dy < 0 && dx > 0); // gradient negates -> algorithm fails

	}
 Bresenham_line(x0,y0, x2,y2,  w, array,false); /* plot remaining part to end */
}

void plotFineQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array)
{
	int cnt=0;
	int dash = lineDashLen +1;
	bool bDots = FormLineDetails->btnDots->Down;
	bool bDashes  = FormLineDetails->btnDashes->Down;
	if (bDots==false && bDashes==false) {bDots=true; bDashes=true; }


	int sx = x0<x2 ? 1 : -1, sy = y0<y2 ? 1 : -1; /* step direction */
	long f = 1, fx = x0-2*x1+x2, fy = y0-2*y1+y2;
	long long x = 2*fx*fx, y = 2*fy*fy, xy = 2*fx*fy*sx*sy;
	long long cur = sx*sy*(fx*(y2-y0)-fy*(x2-x0)); /* curvature */
	/* compute error increments of P0 */
	long long dx = abs(y0-y1)*xy-abs(x0-x1)*y-cur*abs(y0-y2);
	long long dy = abs(x0-x1)*xy-abs(y0-y1)*x+cur*abs(x0-x2);
	/* compute error increments of P2 */
	long long ex = abs(y2-y1)*xy-abs(x2-x1)*y+cur*abs(y0-y2);
	long long ey = abs(x2-x1)*xy-abs(y2-y1)*x-cur*abs(x0-x2);

	/* sign of gradient must not change */
	//assert((x0-x1)*(x2-x1) <= 0 && (y0-y1)*(y2-y1) <= 0);
	if(!((x0-x1)*(x2-x1) <= 0 && (y0-y1)*(y2-y1) <= 0)) return;

	if (cur == 0) { Bresenham_line(x0,y0, x2,y2,  w, array,false); return; } /* straight line */
	/* compute required minimum resolution factor */
	if (dx == 0 || dy == 0 || ex == 0 || ey == 0)
		f = abs(xy/cur)/2+1; /* division by zero: use curvature */
	else {
		fx = 2*y/dx; if (fx > f) f = fx; /* increase resolution */
		fx = 2*x/dy; if (fx > f) f = fx;
		fx = 2*y/ex; if (fx > f) f = fx;
		fx = 2*x/ey; if (fx > f) f = fx;
	} /* negated curvature? */
	if (cur < 0) { x = -x; y = -y; dx = -dx; dy = -dy; xy = -xy; }
	dx = f*dx+y/2-xy; dy = f*dy+x/2-xy; ex = dx+dy+xy; /* error 1.step */

	for (fx = fy = f; ; ) { /* plot curve */
		/* plot curve */

		int plOff = max(min(y0*w + x0,128*128-1),0);
		if(cnt%dash == 0){ 	   if(bDots)    *(array+plOff) = 1; }
		else              {    if(bDashes)  *(array+plOff) = 1; }
		cnt++;


		if (x0 == x2 && y0 == y2) break;
		int iter=0;
		do { /* move f sub-pixel */

			ey = 2*ex-dy; /* save value for test of y step */
			if (2*ex >= dx) { fx--; dy -= xy; ex += dx += y; } /* x step */
			if (ey <= 0) { fy--; dx -= xy; ex += dy += x; } /* y step */
			iter++;
			if(iter>256) break;
		} while (fx > 0 && fy > 0); /* pixel complete? */
		if (2*fx <= f) { x0 += sx; fx += f; } /* sufficient sub-steps.. */
		if (2*fy <= f) { y0 += sy; fy += f; } /* .. for a pixel? */
	}
}
void plotQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array, int bi)
{
	//int dash = lineDashLen +1;
	//bool bDots = FormLineDetails->btnDots->Down;
	//bool bDashes  = FormLineDetails->btnDashes->Down;
	//if (bDots==false && bDashes==false) {bDots=true; bDashes=true; }

	/*
	int ix, iy;
	//double taper midpoint offsets
	if(bi==0){ ix=0; iy=0;}

	if(bi==1){ ix=1; iy=0;}
	if(bi==2){ ix=0; iy=1;}
	if(bi==3){ ix=1; iy=1;}

	if(bi==-1){ ix=-1; iy=0;}
	if(bi==-2){ ix=0; iy=-1;}
	if(bi==-3){ ix=-1; iy=-1;}

	//secret sauce
	//tips the curve in aesthetic favour
	float f=0.99;

	/*
	float mpOff=lineToolMid;
	if (mpOff != 0.0) {
	// Calculate the displacement vector
	int dx = (int)(mpOff / 16.0 * (x2 - x0));
	int dy = (int)(mpOff / 16.0 * (y2 - y0));

	// Add the displacement to the midpoint
	mx += dx;
	my += dy;
	}
	*/

    /*
	int mx = (x0 + x2) / 2;   			//midpoint calc
	int my = (y0 + y2) / 2;
	int dix = (int)((x1 - mx) * f);     //midpoint displacement
	int diy = (int)((y1 - my) * f);
	x1 = mx + dix + ix;
	y1 = my + diy + iy;
	*/


	//algo begin

	/* plot any quadratic Bezier curve */
    int x = x0-x1, y = y0-y1;
	double t = x0-2*x1+x2, r;
    if ((long)x*(x2-x1) > 0) { /* horizontal cut at P4? */
        if ((long)y*(y2-y1) > 0) /* vertical cut at P6 too? */
        if (fabs((y0-2*y1+y2)/t*x) > abs(y)) { /* which first? */
            x0 = x2; x2 = x+x1; y0 = y2; y2 = y+y1; /* swap points */
        } /* now horizontal cut at P4 comes first */
        t = (x0-x1)/t;
        r = (1-t)*((1-t)*y0+2.0*t*y1)+t*t*y2; /* By(t=P4) */
        t = (x0*x2-x1*x1)*t/(x0-x1); /* gradient dP4/dx=0 */
        x = floor(t+0.5); y = floor(r+0.5);
        r = (y1-y0)*(t-x0)/(x1-x0)+y0; /* intersect P3 | P0 P1 */
        plotQuadBezierSeg(x0,y0, x,floor(r+0.5), x,y,  w, array);
        r = (y1-y2)*(t-x2)/(x1-x2)+y2; /* intersect P4 | P1 P2 */
        x0 = x1 = x; y0 = y; y1 = floor(r+0.5); /* P0 = P4, P1 = P8 */
    }
    if ((long)(y0-y1)*(y2-y1) > 0) { /* vertical cut at P6? */
        t = y0-2*y1+y2; t = (y0-y1)/t;
        r = (1-t)*((1-t)*x0+2.0*t*x1)+t*t*x2; /* Bx(t=P6) */
        t = (y0*y2-y1*y1)*t/(y0-y1); /* gradient dP6/dy=0 */
        x = floor(r+0.5); y = floor(t+0.5);
        r = (x1-x0)*(t-y0)/(y1-y0)+x0; /* intersect P6 | P0 P1 */
		plotQuadBezierSeg(x0,y0, floor(r+0.5),y, x,y,  w, array);
        r = (x1-x2)*(t-y2)/(y1-y2)+x2; /* intersect P7 | P1 P2 */
        x0 = x; x1 = floor(r+0.5); y0 = y1 = y; /* P0 = P6, P1 = P7 */
    }
 plotQuadBezierSeg(x0,y0, x1,y1, x2,y2,  w, array); /* remaining part */
}

void Bezier_curve(int x0, int y0, int x1, int y1, int x2, int y2, int w, unsigned char *array, int bi) {

	int dash = lineDashLen +1;
	bool bDots = FormLineDetails->btnDots->Down;
	bool bDashes  = FormLineDetails->btnDashes->Down;
	if (bDots==false && bDashes==false) {bDots=true; bDashes=true; }

	int ix, iy;
	//double taper midpoint offsets
	if(bi==0){ ix=0; iy=0;}

	if(bi==1){ ix=1; iy=0;}
	if(bi==2){ ix=0; iy=1;}
	if(bi==3){ ix=1; iy=1;}

	if(bi==-1){ ix=-1; iy=0;}
	if(bi==-2){ ix=0; iy=-1;}
	if(bi==-3){ ix=-1; iy=-1;}

	//secret sauce
	//tips the curve in aesthetic favour
	float f=0.99;




	int mx = (x0 + x2) / 2;   			//midpoint calc
	int my = (y0 + y2) / 2;
	int dix = (int)((x1 - mx) * f);     //midpoint displacement
	int diy = (int)((y1 - my) * f);
	//
	/*
	float mpOff=lineToolMid;
	if (mpOff != 0.0) {
	// Calculate the displacement vector
	int dx = (int)(mpOff / 16.0 * (x2 - x0));
	int dy = (int)(mpOff / 16.0 * (y2 - y0));

	// Add the displacement to the midpoint
	mx += dx;
	my += dy;
	}
	*/
	int tmp_x1 = x1 +ix;  //this is for quad bezier when regular fails.
	int tmp_y1 = y1 +iy;

	x1 = mx + dix + ix;
	y1 = my + diy + iy;
	//the following is based on Alois Zingl's line drawing Bezier.

	int sx = x0<x2 ? 1 : -1;
	int sy = y0<y2 ? 1 : -1; 				// step direction

	double x = x0-2*x1+x2;
	double y = y0-2*y1+y2;

	double xy = 2*x*y*sx*sy;
	double cur = sx*sy*(x*(y2-y0)-y*(x2-x0))/2; 	// curvature

	/* compute error increments of P0 */
	double dx = (1-2*abs(x0-x1))*y*y+abs(y0-y1)*xy-2*cur*abs(y0-y2);
	double dy = (1-2*abs(y0-y1))*x*x+abs(x0-x1)*xy+2*cur*abs(x0-x2);

	/* compute error increments of P2 */
	double ex = (1-2*abs(x2-x1))*y*y+abs(y2-y1)*xy+2*cur*abs(y0-y2);
	double ey = (1-2*abs(y2-y1))*x*x+abs(x2-x1)*xy-2*cur*abs(x0-x2);

	/* sign of gradient must not change */
	assert((x0-x1)*(x2-x1) <= 0 && (y0-y1)*(y2-y1) <= 0);
	//if(!((x0-x1)*(x2-x1) <= 0 && (y0-y1)*(y2-y1) <= 0)) return;

	if (cur==0) { Bresenham_line(x0,y0, x2,y2, w, array,false); return; } // straight line

	x *= 2*x; y *= 2*y;
	if (cur < 0) { /* negated curvature */
		x = -x; dx = -dx; ex = -ex; xy = -xy;
		y = -y; dy = -dy; ey = -ey;
	}

	/* algorithm fails for almost straight line, check error values */


	if (dx >= -y || dy <= -x || ex <= -y || ey >= -x) {

		// approximation


		x1 = (x0+4*x1+x2)/6;
		y1 = (y0+4*y1+y2)/6;

		//Bresenham_line(x0,y0, x1,y1,  w, array);
		//Bresenham_line(x1,y1, x2,y2,  w, array);

		plotQuadBezier(x0,y0, tmp_x1,tmp_y1, x2,y2, w, array,bi);
		//plotFineQuadBezier(x0,y0, x1,y1, x2,y2, w, array);  //can't handle sharp curve after all
		return;
	}


	dx -= xy; ex = dx+dy; dy -= xy; /* error of 1.step */
	int cnt=0;

	for(;;) { // plot curve

		//*(array + y0*w + x0) = 1;
		int plOff = max(min(y0*w + x0,128*128-1),0);
		if(cnt%dash == 0){ 	if(bDots)    *(array + plOff) = 1; }//plot
		else              {    if(bDashes)  *(array + plOff) = 1; }



		cnt++;
		ey = 2*ex-dy; /* save value for test of y step */
		if (2*ex >= dx) { /* x step */
			if (x0 == x2) break;
			x0 += sx; dy -= xy; ex += dx += y;
		}
		if (ey <= 0) { /* y step */
			if (y0 == y2) break;
			y0 += sy; dx -= xy; ex += dy += x;
		}
    if(cnt>256) break;
	}
}

void __fastcall TFormMain::WorkCHRToBankCHR(void)
{
	for(int i=0;i<8;i++) //transfer from bank to work
	{
		memcpy(chr+(i*256)		,chrBank+(256*chrA_id[i])	,256);
		memcpy(chr+4096+(i*256)	,chrBank+(256*chrB_id[i])	,256);
	}
}
void __fastcall TFormMain::BankCHRToWorkCHR(void)
{
	for(int i=0;i<8;i++) //transfer from bank to work
	{
		memcpy(chrBank+(256*chrA_id[i])	,chr+(i*256)	,256);
		memcpy(	chrBank+(256*chrA_id[i]) ,chr+4096+(i*256)	,256);
	}
}



void __fastcall TFormMain::pal_validate()
{
	int i,j,col;

	//sets the background colour to common
	if(sharedCol0)
	{
		col=bgPal[palBank*16+0*4+0];

		bgPal[palBank*16+1*4+0]=col;
		bgPal[palBank*16+2*4+0]=col;
		bgPal[palBank*16+3*4+0]=col;
	}

	//makes illegal or unsafe colours unavailable
	if(safeColsOnly)
	{
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				switch(bgPal[palBank*16+i*4+j])    //fg todo: make this optional
				{
					case 0x0d:
					case 0x1d:
					case 0x0e:
					case 0x1e:
					case 0x2e:
					case 0x3e:
					case 0x1f:
					case 0x2f:
					case 0x3f:
					bgPal[palBank*16+i*4+j]=colBlack;
					break;
				}
			}
		}
	}
}


void palette_calc(void)
{
	int i;
	float r,g,b;

	//FG: bugfix. M bit no longer overwrites the result of RGB emph bits.
	//if(!(ppuMask&0x01))
	//{
		for(i=0;i<64;i++)
		{
			r=((float)((basePalette[i]>>16)&0xff))/255.0;
			g=((float)((basePalette[i]>>8)&0xff))/255.0;
			b=((float)(basePalette[i]&0xff))/255.0;
			r=r*emphasis[ppuMask>>5][0]/100.0;
			g=g*emphasis[ppuMask>>5][1]/100.0;
			b=b*emphasis[ppuMask>>5][2]/100.0;
			if(r>1.0) r=1.0;
			if(g>1.0) g=1.0;
			if(b>1.0) b=1.0;
			outPalette[i]=(((int)(255.0*r))<<16)|(((int)(255.0*g))<<8)|((int)(255.0*b));
		}

    if(ppuMask&0x01)
	{
	   for(i=0;i<64;i++)
		{
			outPalette[i]=outPalette[i&0xf0];

		}
	}

	/*
	FG: For posterity, the table found in the array 'emphasis'
	is the one from nintech.txt, which credits Chris Covell

	001        B: 074.3%        G: 091.5%        R: 123.9%
	010        B: 088.2%        G: 108.6%        R: 079.4%
	011        B: 065.3%        G: 098.0%        R: 101.9%
	100        B: 127.7%        G: 102.6%        R: 090.5%
	101        B: 097.9%        G: 090.8%        R: 102.3%
	110        B: 100.1%        G: 098.7%        R: 074.1%
	111        B: 075.0%        G: 075.0%        R: 075.0%
	*/

	/*}

	else
	{
		for(i=0;i<64;i++)
		{
			outPalette[i]=basePalette[i&0xf0];
		}
	}*/
}



AnsiString RemoveExt(AnsiString name)
{
	return ChangeFileExt(name,"");
}



AnsiString GetExt(AnsiString name)
{
	name=ExtractFileName(name);

	return name.SubString(name.LastDelimiter(".")+1,name.Length()-name.LastDelimiter(".")).LowerCase();
}



void __fastcall TFormMain::ClearNametable(bool useNulltile)
{
	int tmp	= useNulltile ? nullTile : 0 ;

	memset(nameTable,tmp,sizeof(nameTable));
	memset(attrTable,0,sizeof(attrTable));
}



void __fastcall TFormMain::DrawCol(int x,int y,int size,int c,bool sel)
{
	TRect r;

	r.left  =x;
	r.top   =y;
	r.right =x+size;
	r.Bottom=y+size;

	PaintBoxPal->Canvas->Brush->Color=TColor(outPalette[c]);
	PaintBoxPal->Canvas->FillRect(r);

	if(sel)
	{
		PaintBoxPal->Canvas->Pen->Color=TColor(0xffffff);
		PaintBoxPal->Canvas->Rectangle(r);
		PaintBoxPal->Canvas->Pen->Color=TColor(0);

		r.left  +=1;
		r.top   +=1;
		r.right -=1;
		r.bottom-=1;

		PaintBoxPal->Canvas->Rectangle(r);
	}
}



void __fastcall TFormMain::DrawBGPal(int x,int y,int pal)
{
	DrawCol(x   ,y,20,bgPal[palBank*16+pal*4+0],pal==palActive&&bgPalCur==0?true:false);
	DrawCol(x+20,y,20,bgPal[palBank*16+pal*4+1],pal==palActive&&bgPalCur==1?true:false);
	DrawCol(x+40,y,20,bgPal[palBank*16+pal*4+2],pal==palActive&&bgPalCur==2?true:false);
	DrawCol(x+60,y,20,bgPal[palBank*16+pal*4+3],pal==palActive&&bgPalCur==3?true:false);
}

void __fastcall TFormMain::EnableDisableTypeConflictShortcuts(bool forceDisable)
{
    bool a = SpeedButtonTypeIn->Down;
    bool b = forceDisable;
	Renamecurrentmetasprite1->ShortCut=(a||b)?  TextToShortCut("(None)"):TextToShortCut("Shift+R");
	Renameallmetasprites1->ShortCut=(a||b)?  TextToShortCut("(None)"):TextToShortCut("Shift+Alt+R");

	Colour001->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("1");
	Colour011->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("2");
	Colour101->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("3");
	Colour111->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("4");


	PenMode1->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("9");
	PenMode2->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("0");

	Pal0->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+1");
	Pal1->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+2");
	Pal2->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+3");
	Pal3->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+4");

	Openascurrentset1->ShortCut=(a||b)?         TextToShortCut("(None)"):TextToShortCut("Shift+O");
	MPaletteOpen->ShortCut=(a||b)?              TextToShortCut("(None)"):TextToShortCut("Shift+O");

	Savecurrentset1->ShortCut=(a||b)?           TextToShortCut("(None)"):TextToShortCut("Shift+S");
	MPaletteSave->ShortCut=(a||b)?              TextToShortCut("(None)"):TextToShortCut("Shift+S");

	//MCHRSaveLatest->ShortCut=(a||b)?			TextToShortCut("(None)"):TextToShortCut("Shift+S");
	IncDecCap1->ShortCut=(a||b)?				TextToShortCut("(None)"):TextToShortCut("Shift+M");
	MCHRFreqSort->ShortCut=(a||b)?				TextToShortCut("(None)"):TextToShortCut("Shift+F");
	MCHRDensitySort->ShortCut=(a||b)?			TextToShortCut("(None)"):TextToShortCut("Shift+D");
	MPaletteResetGrayscale->ShortCut=(a||b)?	TextToShortCut("(None)"):TextToShortCut("Shift+G");
	MPaletteCopy->ShortCut=(a||b)?				TextToShortCut("(None)"):TextToShortCut("Shift+C");
	MPalettePaste->ShortCut=(a||b)?				TextToShortCut("(None)"):TextToShortCut("Shift+V");

	Fill1->ShortCut=(a||b)?	            TextToShortCut("(None)"):TextToShortCut("F"); 
    Delete1->ShortCut=(a||b)?	        TextToShortCut("(None)"):TextToShortCut("Del");
    N2x2tileeditmode1->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+E");
    
    Drawontilesetmodeonoff1->ShortCut=(a||b)?	   TextToShortCut("(None)"):TextToShortCut("Shift+D");
    Reverttocheckpoint1->ShortCut=(a||b)?	       TextToShortCut("(None)"):TextToShortCut("Shift+Z");
    
    TogglePenBrush1->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+B");
    Quantized1->ShortCut=(a||b)?	     TextToShortCut("(None)"):TextToShortCut("Shift+U");
    Recallcolour1->ShortCut=(a||b)?	     TextToShortCut("(None)"):TextToShortCut("Z");


    MPalA->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+5");
    MPalB->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+6");
    MPalC->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+7");
    MPalD->ShortCut=(a||b)?	 TextToShortCut("(None)"):TextToShortCut("Shift+8");
}

void __fastcall TFormMain::DrawPalettes(void)
{
	int i,j,x,y,pp,col;

	if(!Visible) return;

	DrawBGPal( 16, 0,0);
	DrawBGPal(128, 0,1);
	DrawBGPal( 16,32,2);
	DrawBGPal(128,32,3);

	y=64;
	pp=0;

	col=bgPal[palBank*16+palActive*4+bgPalCur];

	//if(col==0x0f) col=0x0d;

	for(i=0;i<4;i++)
	{
		x=0;

		for(j=0;j<16;j++)
		{
			if (j<16)   //was 14;
			{
				DrawCol(x,y,16,pp,pp==col);   //x,y,size,c,sel

				pp++;

			}
			else {DrawCol(x,y,16,13,i*16+j==col);}
			x+=16;
		}

		//pp+=2;
		y+=16;
	}
}



void __fastcall TFormMain::DrawTile(TPicture *pic,int x,int y,unsigned int tile,int pal,int tx,int ty,bool sel, bool efficientTarget, int inputScale, bool bIsNav, bool doubleWidth, bool banked)
{
	//tx and ty decide what sort of grid should be overlaid.


	int j,k,l,m,pp,col,r,g,b;
	unsigned char *dst;
	int rr[8],gg[8],bb[8];
	int hgrid,vgrid,hcol,vcol;
	int scale=inputScale;
	int tyTmp;

	bool bOn = SpeedButtonGridAll->Down;
	bool bNamDrag = (ImageName->Dragging() || FormNavigator->Map1->Dragging());

	bool bAuto = AutoViewDragMode1->Checked;
	bool btiletab = PageControlEditor->ActivePage==TabSheetTile?true:false;
	bool bEfficient = btiletab&&efficientTarget==true&&(uiScale==2);

	bool doHighlight = (hoverBtn>=0);

	int set = bankActive/16;
	bool highlight;


	if(doHighlight){

		if(collision_specific){
			//highlights all tiles of this particular bit combination
			highlight= (tileProperties[set+tile]==tileProperties[set+tileActive]);


			//do we want to consider conditionals in this special case?
			/*
			if(tx<0&&ty<0)  //target is Tileset canvas(es)
			{
			   //for any bits set in this combination, what to do?
			}
			else
			{

			}
            */
		}
		else  //Individual bits!
		{
            //highlights all tiles of this particular bit button
			highlight = (tileProperties[set+tile] & (1 << (hoverBtn))) != 0;

            if(tx<0&&ty<0)  //target is Tileset canvas(es)
			{
				//AND with current bits' condition, if any.
				if(FormPropConditions->chkTiles->Checked && (propConditional[hoverBtn]>0))
					highlight = highlight && propCondition[pal][hoverBtn];

				//AND with any conditionals set to "all"
				for(int bitcount=0;bitcount<8;bitcount++){
				   if(FormPropConditions->chkTiles->Checked && (propConditional[bitcount]==2 && btnStateProps[bitcount]))
					highlight = highlight && propCondition[pal][bitcount];
				   }
			}
			else
			{
				//AND with current bits' condition, if any.
				if(FormPropConditions->chkMap->Checked && (propConditional[hoverBtn]>0))
					highlight = highlight && propCondition[pal][hoverBtn];

				//AND with any conditionals set to "all"
				for(int bitcount=0;bitcount<8;bitcount++){
				   if(FormPropConditions->chkMap->Checked && (propConditional[bitcount]==2 && btnStateProps[bitcount]))
					highlight = highlight && propCondition[pal][bitcount];
				}
			}




		}

	}
	if(bEfficient) scale = scale/2;

	if(!banked)  pp=tile*16+bankActive; //using A/B sets directly
	else{                               //mmc5 etc handling goes here
		pp=tile*16+curViewCHRbankCanvas2*4096; //this is for the chr bank manager.
	}
	if(chrSelectRect) sel=false;

	for(j=0;j<8;++j)
	{
		for(k=0;k<8;++k)
		{
			if(!banked) col=(((chr[pp+j]<<k)&128)>>7)|(((chr[pp+j+8]<<k)&128)>>6);
			else        col=(((chrBank[pp+j]<<k)&128)>>7)|(((chrBank[pp+j+8]<<k)&128)>>6);

			col=outPalette[bgPal[palBank*16+pal*4+col]];

			rr[k]=(col>>16)&0xff;
			gg[k]=(col>>8)&0xff;
			bb[k]= col&0xff;
		}

		for(l=0;l<scale;++l)
		{
			dst=(unsigned char*)pic->Bitmap->ScanLine[y+j*scale+l]+x*3;


			for(k=0;k<8;++k)
			{
				int addcol=0;

				if(doHighlight){
					if(highlight) addcol=  45;
					else 		  addcol= -20;

				}
				r=max(min(rr[k]+addcol,255),0);
				g=max(min(gg[k]+addcol,255),0);
				b=max(min(bb[k]+addcol,255),0);
				for(m=0;m<scale;++m)
				{
					*dst++=r;
					*dst++=g;
					*dst++=b;
				}
			}
		}
	}


	hgrid=0;
	vgrid=0;
	hcol=64; //originally 64
	vcol=64; //originally 64
	if (bOn || (bAuto && bNamDrag))
	{

	if(tx<0&&ty<0)  //target is Tileset canvas(es)
	{

		if(SpeedButtonGridTile->Down||SpeedButtonGridAtr->Down||SpeedButtonGridBlock->Down)

		{
			hgrid=2; //spacing: 2 means every other dot.
			vgrid=2;
		}
	}
	else    //target is Screen canvas or otherwise
	{
		if(SpeedButtonGridTile->Down)
		{
			hgrid=2;
			vgrid=2;
			if(doubleWidth==true) {hgrid=0; vgrid=0;}
		}
		if(SpeedButtonGridAtr->Down)
		{
			if(!(ty&1))
			{
				hcol=96;  //originally 96
				//if(ImageName->Dragging()) hcol=80;
				//if(doubleWidth==true) {hcol=hcol-8;  vcol=vcol-8;}
				hgrid=1;
			}
			if(!(tx&1))
			{
				vcol=96; //originally 96
				//if(ImageName->Dragging()) hcol=80;
			   //	if(doubleWidth==true) {hcol=hcol-8;  vcol=vcol-8;}
				vgrid=1;
			}
			//Tilegrid2->Checked^=true;
		}
		if(SpeedButtonGridBlock->Down)
		{
			tyTmp = ty;
			if (((ty / 30) % 2 == 1) && (SpeedButtonGridScreen->Down)) tyTmp -= 2;
			if(!(tyTmp&3))

			{
				hcol=128;  //0riginally 128
				//if(ImageName->Dragging() ) hcol=112;
				//if(doubleWidth==true) {hcol=hcol-8;  vcol=vcol-8;}
				hgrid=1;
			}
			if(!(tx&3))
			{
				vcol=128;
				//if(ImageName->Dragging()) hcol=112;
				//if(doubleWidth==true) {hcol=hcol-8;  vcol=vcol-8;}
				vgrid=1;
			}
			//Tilegrid4->Checked^=true;  //FG bookmark
		}

	}
	}

    if(
        (bAuto && bNamDrag)
		|| (AlwaysNavigator1->Checked 		&&   bIsNav)
		|| (AlwaysCanvas1->Checked 			&& !(bIsNav))

		|| (MouseNavigator1->Checked 		&&   bIsNav 	&& bMouseOverNav)
		|| (MouseCanvas1->Checked 			&& !(bIsNav) 	&& bMouseOverNam)

		|| (ButtonNavigator1->Checked 		&& (SpeedButtonGridScreen->Down && (bOn || (bAuto && bNamDrag))) 		&& bIsNav)
		|| (ButtonCanvas1->Checked  		&& (SpeedButtonGridScreen->Down && (bOn || (bAuto && bNamDrag)))		&& !(bIsNav))

		|| (MouseButtonNavigator1->Checked 	&& ((SpeedButtonGridScreen->Down && (bOn || (bAuto && bNamDrag))) || bMouseOverNav)	&& bIsNav)
		|| (MouseButtonCanvas1->Checked 	&& ((SpeedButtonGridScreen->Down && (bOn || (bAuto && bNamDrag))) || bMouseOverNam)	&& !(bIsNav))
		)
		{
			if(!(ty%30))
			{
				hcol=128;  //0riginally 128
				//if(ImageName->Dragging()) hcol=112;
				hgrid=1;
			}
			if(!(tx%32))
			{
				vcol=128;
				//if(ImageName->Dragging()) hcol=112;
				vgrid=1;
			}
		}


	if(hgrid)
	{
		dst=(unsigned char*)pic->Bitmap->ScanLine[y]+x*3;

		for(j=0;j<8*scale;j+=hgrid)
		{
			*dst+++=hcol;
			*dst+++=hcol;
			*dst+++=hcol;

			if(hgrid>1) dst+=(hgrid-1)*3;
		}

		if(doubleWidth==true && y>0)
		{
			dst=(unsigned char*)pic->Bitmap->ScanLine[y-1]+x*3;
            for(j=0;j<8*scale;j+=hgrid)
			{
				*dst+++=hcol;
				*dst+++=hcol;
				*dst+++=hcol;

				//if(hgrid>1) dst+=(hgrid-1)*3;
			}
		}
	}

	if(vgrid)
	{
		for(j=1;j<8*scale;j+=vgrid)
		{
			dst=(unsigned char*)pic->Bitmap->ScanLine[y+j]+x*3;

			*dst+++=vcol;
			*dst+++=vcol;
			*dst+++=vcol;

			  if(doubleWidth==true && x>0) //
			  {
				dst=(unsigned char*)pic->Bitmap->ScanLine[y+j]+(x-1)*3;
              
				*dst+++=vcol;
				*dst+++=vcol;
				*dst+++=vcol;
			  }
         

		}

		*dst=*dst;//to prevent warning
	}
}



void __fastcall TFormMain::DrawTileChecker(TPicture *pic,int x,int y,int tile,int pal,int tx,int ty,bool sel, bool efficientTarget, int inputScale)
{
	const unsigned char checker[16]={
		0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
		0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff
	};
	int j,k,l,m,col,r,g,b;
	int rr[8],gg[8],bb[8];
	unsigned char *dst;
	int hgrid,vgrid,hcol,vcol;
	int scale=inputScale;
	bool btiletab = PageControlEditor->ActivePage==TabSheetTile?true:false;
	bool bEfficient = btiletab&&efficientTarget==true&&(uiScale==2);
	if(bEfficient) scale = scale/2;

	for(j=0;j<8;j++)
	{
		for(k=0;k<8;k++)
		{
			col=(((checker[j]<<k)&128)>>7)|(((checker[j+8]<<k)&128)>>6);

			col=outPalette[bgPal[palBank*16+pal*4+col]];

			rr[k]=(col>>16)&0xff;
			gg[k]=(col>>8)&0xff;
			bb[k]=col&0xff;
		}

		for(l=0;l<scale;++l)
		{
			dst=(unsigned char*)pic->Bitmap->ScanLine[y+j*scale+l]+x*3;

			for(k=0;k<8;++k)
			{
				r=rr[k];
				g=gg[k];
				b=bb[k];

				for(m=0;m<scale;++m)
				{
					*dst++=r;
					*dst++=g;
					*dst++=b;
				}
			}
		}
	}
}



void __fastcall TFormMain::DrawEmptyTile(TPicture *pic,int x,int y,int pal,int tx,int ty,bool sel,bool efficientTarget, int inputScale)
{
	int j,k,l,m,pp,col;
	unsigned char *dst;
	int scale=inputScale;

	bool btiletab = PageControlEditor->ActivePage==TabSheetTile?true:false;
	bool bEfficient = btiletab&&efficientTarget==true&&(uiScale==2);
	if(bEfficient) scale = scale/2;

	for(j=0;j<8;++j)
	{
		for(l=0;l<scale;++l)
		{
			dst=(unsigned char*)pic->Bitmap->ScanLine[y+j*scale+l]+x*3;

			for(k=0;k<8;++k)
			{
				if((j^k)&4) col=0x80; else col=0xc0;

				for(m=0;m<scale;++m)
				{
					*dst++=col;
					*dst++=col;
					*dst++=col;
				}
			}
		}
	}
}



void __fastcall TFormMain::DrawSpriteDot(TPicture *pic,int x,int y,TColor color,int scale)
{
	x*=scale;
	y*=scale;

	pic->Bitmap->Canvas->Brush->Style=bsSolid;
	pic->Bitmap->Canvas->Brush->Color=color;
	pic->Bitmap->Canvas->Pen->Color=color;
	pic->Bitmap->Canvas->FillRect(TRect(x,y,x+scale,y+scale));
}



void __fastcall TFormMain::DrawSpriteTile(TPicture *pic,int x,int y,int tile,int attr,TColor frame,int scale)
{
	int tx,ty,j,k,l,m,pp,col,r,g,b;
	int rr[8],gg[8],bb[8];
	unsigned char *dst;
	unsigned char chrt[16][8];

	x*=scale;
	y*=scale;

	if(!SpeedButtonSprite8x16->Down)
	{
		pp=(tile&255)*16+bankActive;

		for(j=0;j<8;++j)
		{
			for(k=0;k<8;++k)
			{
				if(attr&128) ty=7-j; else ty=j;//vflip
				if(attr&64 ) tx=7-k; else tx=k;//hflip

				chrt[ty][tx]=(((chr[pp+j]<<k)&128)>>7)|(((chr[pp+j+8]<<k)&128)>>6);
			}
		}
	}
	else
	{
		for(j=0;j<16;++j)
		{
			if(j==0) pp=(tile&254)*16+((tile&1)*4096);
			if(j==8) pp=(tile&254)*16+16+((tile&1)*4096);

			for(k=0;k<8;++k)
			{
				if(attr&128) ty=15-j; else ty=j;//vflip
				if(attr&64 ) tx= 7-k; else tx=k;//hflip

				chrt[ty][tx]=(((chr[pp+(j&7)]<<k)&128)>>7)|(((chr[pp+(j&7)+8]<<k)&128)>>6);
			}
		}
	}

	for(j=0;j<(SpeedButtonSprite8x16->Down?16:8);++j)
	{
		for(k=0;k<8;++k)
		{
			col=chrt[j][k];

			if(col)
			{
				col=outPalette[bgPal[palBank*16+(attr&3)*4+col]];

				rr[k]=(col>>16)&0xff;
				gg[k]=(col>>8)&0xff;
				bb[k]= col&0xff;
			}
			else
			{
				rr[k]=-1;
			}
		}

		for(l=0;l<scale;++l)
		{
			if(y+j*scale+l>=pic->Bitmap->Height) break;
			
			dst=(unsigned char*)pic->Bitmap->ScanLine[y+j*scale+l]+x*3;

			for(k=0;k<8;++k)
			{
				r=rr[k];
				g=gg[k];
				b=bb[k];

				if(r>=0)
				{
					for(m=0;m<scale;++m)
					{
						*dst++=r;
						*dst++=g;
						*dst++=b;
					}
				}
				else
				{
					dst+=3*scale;
				}
			}
		}
	}

	if(frame!=clBlack)
	{
		pic->Bitmap->Canvas->Brush->Style=bsClear;
		pic->Bitmap->Canvas->Pen->Color=frame;
		pic->Bitmap->Canvas->Rectangle(x,y,x+8*scale,y+(SpeedButtonSprite8x16->Down?16*scale:8*scale));
	}
}



void __fastcall TFormMain::DrawExportTile16(TPicture *pic,int x,int y,int tile,int pal,int tx,int ty,bool direct)
{
	int j,k,pp;;
	unsigned char *dst;
	int col1,col2;

	if (direct)  pp=tile;
	if (!direct) pp=tile*16+bankActive;

	for(j=0;j<8;j++)
	{
		dst=(unsigned char*)pic->Bitmap->ScanLine[y+j]+x/2;

		for(k=0;k<8;k+=2)
		{
			col1=(pal<<2)|(((chr[pp+j]<<k)&128)>>7)|(((chr[pp+j+8]<<k)&128)>>6);
			col2=(pal<<2)|(((chr[pp+j]<<(k+1))&128)>>7)|(((chr[pp+j+8]<<(k+1))&128)>>6);

			*dst++=col2|(col1<<4);
		}
	}
}



void __fastcall TFormMain::DrawSelection(TImage *image,TRect rect, int inputScale, bool bHalfsize, bool bTargetNav)
{
	int adjust = bTargetNav?8:0;

	TRect r;
	TRect l;
	TColor c;

	//if(&rect==&curCursor || &rect==&bnkCursor) c=TColor(0x888888);

	if(isBnkCursor) c=TColor(0x888888);
	else c=TColor(0xffffff);


	r.left  =rect.left  *(8*inputScale);
	r.top   =rect.top   *(8*inputScale);
	r.right =rect.right *(8*inputScale);
	r.bottom=rect.bottom*(8*inputScale);

	r.left  -=1;
	r.top   -=1;
	r.right +=1;
	r.bottom+=1;

	image->Canvas->Brush->Style=bsClear;
	image->Canvas->Pen->Color=c;
	image->Canvas->Rectangle(r);

	r.left  +=1;
	r.top   +=1;
	r.right -=1;
	r.bottom-=1;

	if (bHalfsize)image->Canvas->Pen->Color=c;
	else image->Canvas->Pen->Color=TColor(0x000000);
	image->Canvas->Rectangle(r);

	r.left  -=2;
	r.top   -=2;
	r.right +=2;
	r.bottom+=2;

	image->Canvas->Pen->Color=TColor(0x000000);
	image->Canvas->Rectangle(r);


		if (bDrawDestShadow==true)
		{


			r.left  =destRect.left;
			r.top   =destRect.top;
			r.right =destRect.right;
			r.bottom=destRect.bottom;

			if(bTargetNav) //correct destRect
			{
			   if(nameTableWidth<32)
			   {
				r.left=r.left -((32-nameTableWidth)/2);
				r.right=r.right -((32-nameTableWidth)/2);
			   }
			   if(nameTableHeight<32)
			   {
			   r.top=r.top - ((32-nameTableHeight)/2);
			   r.bottom=r.bottom -((32-nameTableHeight)/2);
			   }
			}
			r.left  *=(8*inputScale);
			r.top   *=(8*inputScale);
			r.right *=(8*inputScale);
			r.bottom*=(8*inputScale);

			r.left  -=1;
			r.top   -=1;
			r.right +=1;
			r.bottom+=1;

			image->Canvas->Brush->Style=bsClear;
			image->Canvas->Pen->Color=TColor(0x888888);
			image->Canvas->Rectangle(r);

			

			r.left  -=1;
			r.top   -=1;
			r.right +=1;
			r.bottom+=1;

			if (bHalfsize)image->Canvas->Pen->Color=TColor(0x888888);
			else image->Canvas->Pen->Color=TColor(0x000000);
			image->Canvas->Rectangle(r);

			//modifier symbols
			 image->Canvas->Brush->Style=bsSolid;
			//move
			if(bBufCtrl && !bBufShift && !bBufAlt){

				image->Canvas->Pen->Color=TColor(0x000000);
				l.left  =4 			+ destRect.left  *(8*inputScale);
				l.top   =4-adjust 	+ destRect.top   *(8*inputScale);
				l.right =l.left+2;	l.bottom=l.top+8;
				image->Canvas->Rectangle(l);

			   l.left+=2;	l.right+=2;	l.top+=0;	l.bottom=l.top+2; image->Canvas->Rectangle(l);
			   l.left+=2;	l.right+=2;	l.top+=2;	l.bottom=l.top+6; image->Canvas->Rectangle(l);
			   l.left+=2;	l.right+=2;	l.top-=2;	l.bottom=l.top+2; image->Canvas->Rectangle(l);
			   l.left+=2;	l.right+=2;	l.top+=2;	l.bottom=l.top+6; image->Canvas->Rectangle(l);

				image->Canvas->Pen->Color=TColor(0x888888);
				l.left  =3 			+destRect.left  *(8*inputScale);
				l.top   =3-adjust 	+destRect.top   *(8*inputScale);
				l.right =l.left+2;	l.bottom=l.top+8;
				image->Canvas->Rectangle(l);

			   l.left+=2;	l.right+=2;	l.top+=0;	l.bottom=l.top+2; image->Canvas->Rectangle(l);
			   l.left+=2;	l.right+=2; l.top+=2;	l.bottom=l.top+6; image->Canvas->Rectangle(l);
			   l.left+=2;	l.right+=2;	l.top-=2;	l.bottom=l.top+2; image->Canvas->Rectangle(l);
			   l.left+=2;	l.right+=2; l.top+=2;	l.bottom=l.top+6; image->Canvas->Rectangle(l);
			}
			//clone
			if(bBufCtrl && !bBufShift && bBufAlt)
			{

				image->Canvas->Pen->Color=TColor(0x000000);
				l.left  =4 					+destRect.left  *(8*inputScale);
				l.top   =(4+2)-adjust 		+destRect.top *(8*inputScale);
				l.right =l.left+2;	l.bottom=l.top+4;
				image->Canvas->Rectangle(l);
				l.left+=2;	l.right+=4;	l.top-=2;	l.bottom=l.top+2; image->Canvas->Rectangle(l);
				l.left+=0;	l.right+=0;	l.top+=6;	l.bottom=l.top+2; image->Canvas->Rectangle(l);


				image->Canvas->Pen->Color=TColor(0x888888);
				l.left  =3 					+destRect.left  *(8*inputScale);
				l.top   =(3+2)-adjust 		+destRect.top *(8*inputScale);
				l.right =l.left+2;	l.bottom=l.top+4;
				image->Canvas->Rectangle(l);
				l.left+=2;	l.right+=4;	l.top-=2;	l.bottom=l.top+2; image->Canvas->Rectangle(l);
				l.left+=0;	l.right+=0;	l.top+=6;	l.bottom=l.top+2; image->Canvas->Rectangle(l);
			}

		}

		

	
}



void __fastcall TFormMain::UpdateTiles(bool updchr)
{
	int i,j,x,y;
	int ui8=8*uiScale;
	int ui128=128*uiScale;
	bool bbigTile=(PageControlEditor->ActivePage==TabSheetTile?true:false)&&!bForceNameScale;
	TRect rect;
	if(bImageNameAccepted) return;  //this was a cosmetic fix to a performance issue i now believe is fixed regarding holding alt key. Kept for redundancy.
	if(!Visible) return;
	//if (mouseDraggedTileSel==true&&(Throttle->OnTimer==false)) return;
	//throttleOKtileSelRect = false;
	x=0;
	y=0;

	for(i=0;i<256;i++)
	{

		if(bbigTile)
			 DrawTile(ImageBigTiles->Picture,x,y,tileViewTable[i],palActive,-1,-1,chrSelected[i],false,uiScale,false,false,false);
		else DrawTile(ImageTiles->Picture,x,y,tileViewTable[i],palActive,-1,-1,chrSelected[i],false,uiScale,false,false,false);


		x+=ui8;


		if(x>=ui128)
		{
			x=0;

			y+=ui8;
		}
	}

	if(!chrSelectRect)

	{
		for(i=0;i<256;i++)
		{
			if(chrSelected[i])
			{
				rect.Left=i&15;
				rect.Right=rect.Left+1;
				rect.Top=i/16;
				rect.Bottom=rect.Top+1;


				if(bbigTile) DrawSelection(ImageBigTiles,rect,uiScale,false,false);
				else DrawSelection(ImageTiles,rect,uiScale,false,false);
			}
		}
	}
	else
	{

		if(bbigTile) DrawSelection(ImageBigTiles,chrSelection,uiScale,false,false);
		else DrawSelection(ImageTiles,chrSelection,uiScale,false,false);
	}


	if(bbigTile) ImageBigTiles->Repaint();
    else ImageTiles->Repaint();

	if((updchr) && (openByFileDone)) FormCHREditor->PaintBoxChr->Repaint();
}



void __fastcall TFormMain::UpdateNameTable(int tx,int ty,bool repaint)
{
	int i,j,x,y,nx,ny,tile;
	TRect r;
	bool bBigTile = (PageControlEditor->ActivePage==TabSheetTile?true:false)&&!bForceNameScale;
	if(bBigTile&&(uiScale==2)) {ImageTiles->Picture->Bitmap->SetSize(ImageTiles->Width*2,ImageTiles->Height*2);
	}
	else if (bBigTile) {
		ImageTiles->Picture->Bitmap->SetSize(ImageTiles->Width*2,ImageTiles->Height*2);
		ImageTiles->Stretch = true;
	}
	else {
		ImageTiles->Picture->Bitmap->SetSize(ImageTiles     ->Width,ImageTiles     ->Height);
		ImageTiles->Stretch = false;
	}
	if(!Visible) return;

	if (mouseDraggedNTSel==true&&throttleOKselRect==false) {cueNT=true; return;}
	throttleOKselRect = false;


	if(tx<0||ty<0)
	{
		//redraw whole viewport

		y=0;
		unsigned char tmp_chrSelected[256];
		for (i=0;i<256;i++) tmp_chrSelected[tileViewTable[i]]=chrSelected[i];

		for(i=0;i<32;i++)
		{
			x=0;

			for(j=0;j<32;j++)
			{
				nx=j+nameTableViewX;
				ny=i+nameTableViewY;

				if(nx>=0&&nx<nameTableWidth&&ny>=0&&ny<nameTableHeight)
				{
					tile=nameTable[ny*nameTableWidth+nx];

					if(SpeedButtonChecker->Down)
					{
						if(bBigTile)DrawTileChecker(ImageTiles->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,true,uiScale);
						else DrawTileChecker(ImageName->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale);

					}
					else
					{
						if(!SpeedButtonSelTiles->Down||tmp_chrSelected[tile])
						{
							if(bBigTile)DrawTile(ImageTiles->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,true,uiScale,false,false,false);
							else DrawTile(ImageName->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale,false,false,false);
						}
						else
						{
							if(bBigTile) DrawEmptyTile(ImageTiles->Picture,x,y,AttrGet(nx,ny,false,false),nx,ny,false,true,uiScale);
							else DrawEmptyTile(ImageName->Picture,x,y,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale);
						}
					}
				}
				else
				{
					if(bBigTile) DrawEmptyTile(ImageTiles->Picture,x,y,0,nx,ny,false,true,uiScale);
					else DrawEmptyTile(ImageName->Picture,x,y,0,nx,ny,false,false,uiScale);
				}

				if(bBigTile&&(uiScale==2)) x+=(8*uiScale)/2;
				else x+=8*uiScale;
			}

			if(bBigTile&&(uiScale==2)) y+=(8*uiScale)/2;
			else y+=8*uiScale;
		}
	}
	else
	{
		//redraw one attribute cell

		//delimit edges of drawing field (odd scroll positions otherwise draw outside memory)

		//quantize
		tx&=~1;
		ty&=~1;

		y=(ty-nameTableViewY)*8*uiScale;
		//if(!(nameTableViewY%2 && (ty==0||32)))


		for(i=0;i<2;i++)
		{
			x=(tx-nameTableViewX)*8*uiScale;

			for(j=0;j<2;j++)
			{
				nx=j+tx;
				ny=i+ty;

				if(y<0 || y>=32*8*uiScale) continue;   //protects against writing outside viewport bitmap


				if(nx>=0&&nx<nameTableWidth&&ny>=0&&ny<nameTableHeight)
				{
					tile=nameTable[ny*nameTableWidth+nx];

					if(SpeedButtonChecker->Down)
					{
						if(bBigTile) DrawTileChecker(ImageTiles->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale);
						else DrawTileChecker(ImageName->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale);
					}
					else
					{
						if(!SpeedButtonSelTiles->Down||chrSelected[tile])
						{
							if(bBigTile) DrawTileChecker(ImageTiles->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale);
							else DrawTile(ImageName->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale,false,false,false);
						}
						else
						{
							if(bBigTile) (ImageTiles->Picture,x,y,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale);
							else DrawEmptyTile(ImageName->Picture,x,y,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale);
						}
					}
				}
				else
				{
					if(bBigTile) DrawEmptyTile(ImageTiles->Picture,x,y,0,nx,ny,false,false,uiScale);
					else DrawEmptyTile(ImageName->Picture,x,y,0,nx,ny,false,false,uiScale);
				}

				x+=8*uiScale;
			}

			y+=8*uiScale;
		}
	}

	BufBmpName->Assign(ImageName->Picture->Bitmap);

	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		r.left  =nameSelection.left  -nameTableViewX;
		r.right =nameSelection.right -nameTableViewX;
		r.top   =nameSelection.top   -nameTableViewY;
		r.bottom=nameSelection.bottom-nameTableViewY;


		if(!bBigTile) DrawSelection(ImageName,r,uiScale,false,false);  //selection only meaningful on the big canvas.
	}

	if(repaint) {
		if(bBigTile) ImageTiles->Repaint();
		else ImageName->Repaint();
	}
	//update appropriate menu items
	SelectedOnly1->Checked=SpeedButtonSelTiles->Down;
}

void __fastcall TFormMain::UpdateNameStrip(bool repaint, int off)
{
	int i,j,x,y,nx,ny,tile;
	TRect r;
	bool bBigTile = (PageControlEditor->ActivePage==TabSheetTile?true:false)&&!bForceNameScale;

	/*
	if(bBigTile&&(uiScale==2)) {ImageTiles->Picture->Bitmap->SetSize(ImageTiles->Width*2,ImageTiles->Height*2);
	}
	else if (bBigTile) {
		ImageTiles->Picture->Bitmap->SetSize(ImageTiles->Width*2,ImageTiles->Height*2);
		ImageTiles->Stretch = true;
	}
	else {
		ImageTiles->Picture->Bitmap->SetSize(ImageTiles     ->Width,ImageTiles     ->Height);
		ImageTiles->Stretch = false;
	} */
	if(!Visible) return;

	if (mouseDraggedNTSel==true&&throttleOKselRect==false) {cueNT=true; return;}
	throttleOKselRect = false;

	i=cntNTstrip+off;

	//y=i*8;
	if(bBigTile&&(uiScale==2)) y=i*(8*uiScale)/2;
	else y=i*8*uiScale;
	x=0;

	for(j=0;j<32;j++)
	{
		nx=j+nameTableViewX;
		ny=i+nameTableViewY;

		if(nx>=0&&nx<nameTableWidth&&ny>=0&&ny<nameTableHeight)
		{
			tile=nameTable[ny*nameTableWidth+nx];
			if(SpeedButtonChecker->Down)
			{
				if(bBigTile)DrawTileChecker(ImageTiles->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,true,uiScale);
				else DrawTileChecker(ImageName->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale);
			}
			else
			{
				if(!SpeedButtonSelTiles->Down||chrSelected[tile])
				{
					if(bBigTile)DrawTile(ImageTiles->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,true,uiScale,false,false,false);
					else 		DrawTile(ImageName->Picture,x,y,tile,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale,false,false,false);
				}
				else
				{
					if(bBigTile) DrawEmptyTile(ImageTiles->Picture,x,y,AttrGet(nx,ny,false,false),nx,ny,false,true,uiScale);
					else DrawEmptyTile(ImageName->Picture,x,y,AttrGet(nx,ny,false,false),nx,ny,false,false,uiScale);
				}
			}
		}
		else
		{
			if(bBigTile) DrawEmptyTile(ImageTiles->Picture,x,y,0,nx,ny,false,true,uiScale);
			else DrawEmptyTile(ImageName->Picture,x,y,0,nx,ny,false,false,uiScale);
		}
		if(bBigTile&&(uiScale==2)) x+=(8*uiScale)/2;
		else x+=8*uiScale;
	}

	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		r.left  =nameSelection.left  -nameTableViewX;
		r.right =nameSelection.right -nameTableViewX;
		r.top   =nameSelection.top   -nameTableViewY;
		r.bottom=nameSelection.bottom-nameTableViewY;

		if(bBigTile) ;//DrawSelection(ImageTiles,r);      //disabled since this mode only is meant to pan - selection is irrelevant.
		else DrawSelection(ImageName,r,uiScale,false,false);
	}

	if(repaint) {

		if(bBigTile) ImageTiles->Repaint();
		else ImageName->Repaint();
	}
	//update appropriate menu items
	SelectedOnly1->Checked=SpeedButtonSelTiles->Down;
}

void ExportBMPToClipboard(char *bitmapBuffer, size_t len, bool cbDoClear)
{
	//non-vcl dependent way of copying to clip. currently not in use.
	HGLOBAL hResult;
	OpenClipboard(NULL);
	if (cbDoClear) EmptyClipboard();

	len -= sizeof(BITMAPFILEHEADER);
	hResult = GlobalAlloc(GMEM_MOVEABLE, len);

	memcpy(GlobalLock(hResult), bitmapBuffer + sizeof(BITMAPFILEHEADER), len);
	GlobalUnlock(hResult);
	SetClipboardData(CF_DIB, hResult);
	CloseClipboard();
	GlobalFree(hResult);
}

void __fastcall TFormMain::CopyCHR(bool copy,bool cut)
{
	int i,j,k,pp,ps,x,y,w,h;
	//FormMain->SetUndo();
	int prop_p;

	if(chrSelectRect)
	{
		GetSelection(chrSelection,x,y,w,h);
		if(w&&h&&cut) SetUndo();

		pp=0;
		prop_p=0;
		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{

				for(k=0;k<16;k++)
				{
					ps=bankActive + tileViewTable[(x+j) + (y+i)*16]*16 + k;

					if(copy) chrCopy[pp++]=chr[ps];
					if(cut)  chr[ps]=0;
				}
				if(copy) 	propCopy[prop_p++]=tileProperties[(bankActive/16) + (x+j) + (y+i)*16];
				if(cut)     tileProperties[(bankActive/16) + (x+j) + (y+i)*16]=0;
			}
		}

		if(copy)
		{
			chrCopyWidth=w;
			chrCopyHeight=h;
			chrCopyRect=true;
		}
	}
	else
	{
		if(cut) SetUndo();

		pp=0;
		w=0;
		h=-1;
		prop_p=0;

		for(i=0;i<256;i++)
		{
			if(chrSelected[i])
			{
				for(j=0;j<16;j++)
				{
					ps=bankActive+i*16+j;
					if(copy) chrCopy[pp++]=chr[ps];
					if(cut) chr[ps]=0;
				}
				if(copy) propCopy[prop_p++]=tileProperties[(bankActive/16)+i];
				if(cut) tileProperties[(bankActive/16)+i]=0;

				w++;
			}
		}

		if(copy)
		{
			chrCopyWidth=w;
			chrCopyHeight=h;
			chrCopyRect=false;
		}
	}

	DWORD Len=3*sizeof(int)+sizeof(chrCopy)+sizeof(propCopy);

	HGLOBAL hDst=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,Len);

	if (hDst)
	{
		LPSTR gDst=(LPSTR)GlobalLock(hDst);

		if (gDst)
		{
			if (OpenClipboard(Handle))
			{
				EmptyClipboard();

				*((int*)&gDst[0*sizeof(int)])=chrCopyWidth;
				*((int*)&gDst[1*sizeof(int)])=chrCopyHeight;
				*((int*)&gDst[2*sizeof(int)])=chrCopyRect;
				memcpy(&gDst[12],chrCopy,sizeof(chrCopy));
				memcpy(&gDst[12+sizeof(chrCopy)],propCopy,sizeof(propCopy));


				GlobalUnlock(hDst);

				SetClipboardData(CF_CHR,hDst);
				CloseClipboard();
			}
		}
	}

	if(cut)
	{
		UpdateTiles(true);
		//UpdateMetaSprite();
		cueUpdateMetasprite=true;

		UpdateNameTable(-1,-1,true);
		if (FormNavigator->Visible)
		{
			//bMouseOverNav=false;
			//FormNavigator->CueDrawTimer->Enabled=true;
			FormNavigator->Draw(false,false);
			//FormNavigator->Repaint();
		}
		if(FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();

	}

	if(cut){     //sanitize line drawing buffers
		memset (arr_linePlotCHR,0,sizeof(arr_linePlotCHR2));
		memset (arr_linePlotCHR2,0,sizeof(arr_linePlotCHR2));
		memcpy (chrBuf, chr, 8192);
		lineUpX= -1;
		lineUpY= -1;
	}
}



void __fastcall TFormMain::PasteCHR(void)
{
	//general
	int i,j,k,pp,pd,x,y,w,h;


	//used by flex-paste
	bool b=FormCHREditor->btn2x2mode->Down;
	int selectThres=b?2:1;
	bool bIsFlex;

	int xlen;   //width of selection
	int ylen;   //height of selection

	int maxlen; //the longest a paste run can be; based on clipboard contents
	int countlen = 0; //manual counter for the above.

    int propOff; //used by rect selection sources for properties.
	//used by bitplane masks
	int fLen=16;
	int fOff=0;

	//used by colour masks
	unsigned char mask[16];
	bool b0 = FormCHREditor->Protect0->Down;
	bool b1 = FormCHREditor->Protect1->Down;
	bool b2 = FormCHREditor->Protect2->Down;
	bool b3 = FormCHREditor->Protect3->Down;


	//apply bitplane masks
	if (!(FormCHREditor->ButtonBitmaskLo->Down||FormCHREditor->ButtonBitmaskHi->Down))
		{if(Applytopaste1->Checked) return;}
	if (!FormCHREditor->ButtonBitmaskLo->Down&&Applytopaste1->Checked) {fLen-=8; fOff=8;}
	if (!FormCHREditor->ButtonBitmaskHi->Down&&Applytopaste1->Checked) {fLen-=8;}


	//get clipboard
	OpenClipboard(Handle);

	HGLOBAL hClipBuf = GetClipboardData(CF_CHR);

	if (hClipBuf)
	{
		LPSTR gSrc = (LPSTR)GlobalLock(hClipBuf);

		if (gSrc != NULL)
		{
			chrCopyWidth =*((int*)&gSrc[0*sizeof(int)]);
			chrCopyHeight=*((int*)&gSrc[1*sizeof(int)]);
			chrCopyRect  =*((int*)&gSrc[2*sizeof(int)]);
			memcpy(chrCopy,&gSrc[12],sizeof(chrCopy));
            memcpy(propCopy,&gSrc[12+sizeof(chrCopy)],sizeof(propCopy));

			GlobalUnlock(gSrc);
		}
	}

	CloseClipboard();


	//if clipboard was 2-dimensional
	if(chrCopyRect)
	{
		if(chrCopyWidth<1||chrCopyHeight<1) return;
		SetUndo();
		GetSelection(chrSelection,x,y,w,h);


		//determines if flex-paste should be used and modifies for iterations accordingly.
		bIsFlex = (w>selectThres || h>selectThres);
		maxlen = chrCopyHeight * chrCopyWidth;
	
		if(bIsFlex)
		{
		   xlen = w;
		   ylen = h;
		}
		else
		{
			xlen = chrCopyWidth;
			ylen = chrCopyHeight;
		}

		//start of paste from buffer.
		pp=0;

		for(i=0;i<ylen;i++)
		{
			for(j=0;j<xlen;j++)
			{
				if (countlen >= maxlen) continue;

				if(x+j<16&&y+i<16)  //safety for making sure selection isn´t larger than table.
				{
					for(int m=0;m<16;m++) mask[m]=0xFF; //set mask.

					for(int l=0;l<8;l++)
					{

						pd=bankActive + tileViewTable[(x+j) + (y+i)*16]*16 + l;
						//set mask
						if(Applytopaste2->Checked){

							if (b0) mask[l]	   =~mask[l]	|(chr[pd]	|chr[pd+8]);
							if (b1) mask[l]    = mask[l]   &~(chr[pd]	&(chr[pd]^chr[pd+8]));
							if (b2)	mask[l]    = mask[l]   &~(chr[pd+8]	&(chr[pd]^chr[pd+8]));
							if (b3) mask[l]	   = mask[l]	-(chr[pd]	&chr[pd+8]);

							mask[l+8]	=mask[l]; //makes the 1bit mask applicable to 2bit gfx.
						}
					}

					for(k=0;k<fLen;k++)  //pasting tile
					{

						pd=bankActive + tileViewTable[(x+j) + (y+i)*16]*16 + k;
						chr[pd+fOff]=(chr[pd+fOff]&~mask[k+fOff]);
						chr[pd+fOff]=chr[pd+fOff]|(chrCopy[pp+k+fOff]&mask[k+fOff]);

					}
					propOff =  tileViewTable[(x+j) + (y+i)*16];
					tileProperties[(bankActive/16) + propOff]=propCopy[(pp/16)];

				}
				pp+=16;
				countlen++;
			}
		}
	}
	//if clipboard was 1-dimensional (from multi-select, likely).
	else
	{
		if(chrCopyWidth<1) return;
		SetUndo();
		GetSelection(chrSelection,x,y,w,h);
		bIsFlex = (w>selectThres || h>selectThres);

		if(bIsFlex)
		{
		   xlen = w;
		   ylen = h;
		}
		else
		{
			xlen = chrCopyWidth;
			ylen = 1;
		}
		pp=0+fOff;
		pd=tileActive*16+fOff;
		int prop_p = 0;
		int prop_d = tileActive;
		//added to signal the origin to the user, in case of no selection or present multi selection
		if (!bIsFlex) SelectTile(tileActive);

		for(j=0;j<ylen;j++)
		{
			for(i=0;i<xlen;i++)
			{
				if (countlen >= chrCopyWidth) continue;
				if (bIsFlex) {pd=bankActive+(((x+i)*16 +(y+j)*256 + fOff)&0x0fff);
							  prop_d=(bankActive/16)+(((x+i) +(y+j)*16)&0xff);
				}
				else 		 {pd=bankActive+(pd&0x0fff);
							  prop_d=(bankActive/16)+(prop_d&0xff);
				}
				for(int m=0;m<16;m++) mask[m]=0xFF; //set mask.

				for(int l=0;l<8;l++)
				{
					//pd=bankActive+(x+j)*16+(y+i)*256+l;
					//set mask
					if(Applytopaste2->Checked){

						if (b0) mask[l]	   =~mask[l]	|(chr[pd+l]	|chr[pd+8+l]);
						if (b1) mask[l]    = mask[l]   &~(chr[pd+l]	&(chr[pd+l]^chr[pd+8+l]));
						if (b2)	mask[l]    = mask[l]   &~(chr[pd+8+l]	&(chr[pd+l]^chr[pd+8+l]));
						if (b3) mask[l]	   = mask[l]	-(chr[pd+l]	&chr[pd+8+l]);

						mask[l+8]	=mask[l]; //makes the 1bit mask applicable to 2bit gfx.
					}
				}

				for(k=0;k<fLen;k++)
				{
					//new; using colour protection masks
					chr[pd]=(chr[pd]&~mask[k]);          //+fOff
					chr[pd]=chr[pd]|(chrCopy[pp]&mask[k]);   //+fOff
					pd++;
					pp++;

					//original
					//chr[pd++]=chrCopy[pp++];
				}
				if(fLen==8){pp+=8;pd+=8;}
				countlen++;

				tileProperties[prop_d]=propCopy[prop_p];
				prop_p++;
				prop_d++;
			}
		}
	}

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	if(FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();
	if (FormNavigator->Visible) FormNavigator->Draw(false,false);

	cueUpdateMetasprite=true;
}



void __fastcall TFormMain::NameTableScrollLeft(bool all)
{
	unsigned char temp[1024];
	int i,j,k;

	for(k=0;k<(all?2:1);k++)
	{
		for(i=0;i<nameTableHeight;i++) temp[i]=nameTable[i*nameTableWidth];

		for(i=0;i<nameTableWidth-1;i++)
		{
			for(j=0;j<nameTableHeight;j++)
			{
				nameTable[j*nameTableWidth+i]=nameTable[j*nameTableWidth+i+1];
			}
		}

		for(i=0;i<nameTableHeight;i++) nameTable[i*nameTableWidth+nameTableWidth-1]=temp[i];
	}

	if(all)
	{
		for(i=0;i<nameTableHeight/2;i++) temp[i]=AttrGet(0,i*2,false,false);

		for(i=0;i<nameTableWidth/2-1;i++)
		{
			for(j=0;j<nameTableHeight/2;j++)
			{
				AttrSet(i*2,j*2,AttrGet(i*2+2,j*2,false,false),false);
			}
		}

		for(i=0;i<nameTableHeight/2;i++) AttrSet(nameTableWidth-2,i*2,temp[i],false);
	}

	UpdateNameTable(-1,-1,true);
	//FormNavigator->Draw(false,false);
	FormNavigator->UpdateLines(true);
}



void __fastcall TFormMain::NameTableScrollRight(bool all)
{
	unsigned char temp[1024];
	int i,j,k;

	for(k=0;k<(all?2:1);k++)
	{
		for(i=0;i<nameTableHeight;i++) temp[i]=nameTable[i*nameTableWidth+nameTableWidth-1];

		for(i=nameTableWidth-1;i>0;i--)
		{
			for(j=0;j<nameTableHeight;j++)
			{
				nameTable[j*nameTableWidth+i]=nameTable[j*nameTableWidth+i-1];
			}
		}

		for(i=0;i<nameTableHeight;i++) nameTable[i*nameTableWidth]=temp[i];
	}

	if(all)
	{
		for(i=0;i<nameTableHeight/2;i++) temp[i]=AttrGet(nameTableWidth-2,i*2,false,false);

		for(i=nameTableWidth/2-1;i>0;i--)
		{
			for(j=0;j<nameTableHeight/2;j++)
			{
				AttrSet(i*2,j*2,AttrGet(i*2-2,j*2,false,false),false);
			}
		}

		for(i=0;i<nameTableHeight/2;i++) AttrSet(0,i*2,temp[i],false);
	}

	UpdateNameTable(-1,-1,true);
	//FormNavigator->Draw(false,false);
	FormNavigator->UpdateLines(true);
}



void __fastcall TFormMain::NameTableScrollUp(bool all)
{
	unsigned char temp[1024];
	int i,j,k;

	for(k=0;k<(all?2:1);k++)
	{
		for(i=0;i<nameTableWidth;i++) temp[i]=nameTable[i];

		for(i=0;i<nameTableHeight-1;i++)
		{
			for(j=0;j<nameTableWidth;j++)
			{
				nameTable[i*nameTableWidth+j]=nameTable[(i+1)*nameTableWidth+j];
			}
		}

		for(i=0;i<nameTableWidth;i++) nameTable[nameTableWidth*nameTableHeight-nameTableWidth+i]=temp[i];
	}

	if(all)
	{
		for(i=0;i<nameTableWidth/2;i++) temp[i]=AttrGet(i*2,0,false,false);

		for(i=0;i<nameTableHeight/2-1;i++)
		{
			for(j=0;j<nameTableWidth/2;j++)
			{
				AttrSet(j*2,i*2,AttrGet(j*2,i*2+2,false,false),false);
			}
		}

		for(i=0;i<nameTableWidth/2;i++) AttrSet(i*2,nameTableHeight-2,temp[i],false);
	}

	UpdateNameTable(-1,-1,true);
	//FormNavigator->Draw(false,false);
	FormNavigator->UpdateLines(true);
}



void __fastcall TFormMain::NameTableScrollDown(bool all)
{
	unsigned char temp[1024];
	int i,j,k;

	for(k=0;k<(all?2:1);k++)
	{
		for(i=0;i<nameTableWidth;i++) temp[i]=nameTable[nameTableWidth*nameTableHeight-nameTableWidth+i];

		for(i=nameTableHeight-1;i>0;i--)
		{
			for(j=0;j<nameTableWidth;j++)
			{
				nameTable[i*nameTableWidth+j]=nameTable[(i-1)*nameTableWidth+j];
			}
		}

		for(i=0;i<nameTableWidth;i++) nameTable[i]=temp[i];
	}

	if(all)
	{
		for(i=0;i<nameTableWidth/2;i++) temp[i]=AttrGet(i*2,nameTableHeight-2,false,false);

		for(i=nameTableHeight/2-1;i>0;i--)
		{
			for(j=0;j<nameTableWidth/2;j++)
			{
				AttrSet(j*2,i*2,AttrGet(j*2,i*2-2,false,false),false);
			}
		}

		for(i=0;i<nameTableWidth/2;i++) AttrSet(i*2,0,temp[i],false);
	}

	UpdateNameTable(-1,-1,true);
	//FormNavigator->Draw(false,false);
	FormNavigator->UpdateLines(true);
}



bool __fastcall TFormMain::OpenCHR(AnsiString name)
{
	unsigned char buf[4096];
	FILE *file;
	int i,pp,off,size,type;

	file=fopen(name.c_str(),"rb");

	type=-1;

	if(file)
	{
		size=get_file_size(file);

		off=0;

		switch(size)
		{
		case 8192:
			fread(chr,8192,1,file);
			type=4;
			break;

		case 4096:
			fread(chr+bankActive,4096,1,file);
			type=3;
			break;

		default:
			if(size<4096&&!(size&15))
			{
				fread(buf,size,1,file);

				pp=tileActive*16;
				off=pp;

				for(i=0;i<size;i++)
				{
					chr[bankActive+pp++]=buf[i];
					if(pp>=4096) pp=0;
				}

				if(size==2048) type=2; else if(size==1024) type=1; else type=0;
			}
			else
			{
				Application->MessageBox("Wrong file size","Error",MB_OK);
				fclose(file);
				return false;
			}
		}
	}

	fclose(file);

	if(type<0) return false;

	SetLatestCHR(off,size);
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);

	if(openByFileDone) {
		if (FormNavigator->Visible) FormNavigator->Draw(true,true);
		if (FormCHRbit->Visible)	FormCHRbit->UpdateBitButtons();
	}

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;

	return true;
}

bool __fastcall TFormMain::OpenTprop(AnsiString name)
{
	unsigned char buf[512];
	FILE *file;
	int i,pp,size,type;

	file=fopen(name.c_str(),"rb");

	type=-1;

	if(file)
	{
		size=get_file_size(file);

		//off=0;

		switch(size)
		{
		case 512:
			fread(buf,512,1,file);
            for(i=0;i<size;i++)	tileProperties[i]=buf[i];
			type=2;
			break;

		case 256:
			fread(buf,256,1,file);
			for(i=0;i<size;i++)	tileProperties[(bankActive/16)+i]=buf[i];
			type=1;
			break;

		default:
			if(size<256)
			{
				fread(buf,size,1,file);

				pp=tileActive;
				//off=pp;

				for(i=0;i<size;i++)
				{
					tileProperties[(bankActive/16)+pp++]=buf[i];
					if(pp>=256) pp=0;
				}
				type=0;
			}
			else
			{
				Application->MessageBox("Wrong file size","Error",MB_OK);
				fclose(file);
				return false;
			}
		}
	}

	fclose(file);

	if(type<0) return false;

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);

	if(openByFileDone) {
		if (FormNavigator->Visible) FormNavigator->Draw(true,true);
		if (FormCHRbit->Visible)	FormCHRbit->UpdateBitButtons();
	}
	return true;
}


void __fastcall TFormMain::SaveCHR(int offset,int size)
{
	unsigned char buf[8192];
	FILE *file;
	int i,pp,off;
	AnsiString name;

	if(!SaveDialogChr->Execute()) return;

	name=RemoveExt(SaveDialogChr->FileName)+".chr";

	if(!OverwritePrompt(name)) return;

	file=fopen(name.c_str(),"rb");

	if(file)
	{
		fseek(file,0,SEEK_END);
		i=ftell(file);
		fclose(file);

		if(size!=i)
		{
			if(Application->MessageBox(("Previous file has different size ("+IntToStr(i)+" bytes)!\nDo you really want to overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES) return;
		}
	}

	file=fopen(name.c_str(),"wb");

	if(!file) return;

	if(size==4096||size==8192)
	{
		if(size==4096)
		{
			memcpy(buf,chr+bankActive,4096);
		}
		else
		{
			memcpy(buf,chr,8192);
		}
	}
	else
	{
		pp=offset;

		for(i=0;i<size;i++)
		{
			if(pp>=4096) pp=0;

			buf[i]=chr[bankActive+pp];

			++pp;
		}
	}

	fwrite(buf,size,1,file);
	fclose(file);

	SetLatestCHR(offset,size);
}

void __fastcall TFormMain::SaveTprop(int offset, int size)
{
	unsigned char buf[512];
	FILE *file;
	int i,pp,off;
	AnsiString name;

	if(!SaveDialogTprop->Execute()) return;

	name=RemoveExt(SaveDialogTprop->FileName)+".tprop";

	if(!OverwritePrompt(name)) return;

	file=fopen(name.c_str(),"rb");

	if(file)
	{
		fseek(file,0,SEEK_END);
		i=ftell(file);
		fclose(file);

		if(size!=i)
		{
			if(Application->MessageBox(("Previous file has different size ("+IntToStr(i)+" bytes)!\nDo you really want to overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES) return;
		}
	}

	file=fopen(name.c_str(),"wb");

	if(!file) return;

	if(size==256||size==512)
	{
		if(size==256)
		{
			for(int i=0;i<size;i++)buf[i]=(tileProperties[i+(bankActive/16)])&0xFF;
			//memcpy(buf,tileProperties+(bankActive/16),256);
		}
		else
		{
			 for(int i=0;i<size;i++)buf[i]=tileProperties[i]&0xFF;
			//memcpy(buf,tileProperties,512);
		}
	}
	else
	{
		pp=offset;

		for(i=0;i<size;i++)
		{
			if(pp>=256) pp=0;

			buf[i]=tileProperties[(bankActive/16)+pp]&0xFF;

			++pp;
		}
	}

	fwrite(buf,size,1,file);
	fclose(file);
}

void __fastcall TFormMain::InterleaveCHR(bool dir)
{
	unsigned char buf[4096];
	uint32_t bufProp[256];

	int table[256];
	int i,j,pp,pd,num;

	SetUndo();

	for(i=0;i<16;i++)
	{
		num=(i/2)*32+(i&1);

		for(j=0;j<16;j++)
		{
			if(dir) table[i*16+j]=num; else table[num]=i*16+j;
			num+=2;
		}
	}
	int set=bankActive/16;
	pp=bankActive;
	for(i=0;i<256;i++)
	{
		pd=table[i]*16;

		for(j=0;j<16;j++)
		{
			buf[pd++]=chr[pp++];
		}
		bufProp[table[i]]=tileProperties[set+i];
	}

	memcpy(chr+bankActive,buf,4096);
	memcpy(tileProperties+set,bufProp,256*sizeof(uint32_t));

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	if (FormNavigator->Visible) FormNavigator->Draw(false,false);
    if (FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}



int parse_skip_empty(unsigned char* data,int ptr,int size)
{
	char c;
	
	if(ptr<0) return ptr;
	
	while(ptr<size)
	{
		c=data[ptr];

		if(c>' ') break;

		++ptr;
	}

	return ptr;
}

int parse_skip_tag(unsigned char* data,int ptr,int size,const char* tag)
{
	if(ptr<0) return ptr;
	
	if(!memcmp(&data[ptr],tag,strlen(tag))) return ptr+strlen(tag); else return -1;
}

int parse_skip_value(unsigned char* data,int ptr,int size)
{
	char c;
	
	if(ptr<0) return ptr;
	
	while(ptr<size)
	{
		c=data[ptr];

		if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='$'||c=='_')) break;

		++ptr;
	}

	return ptr;
}

int parse_get_value(unsigned char* data,int ptr,int size)
{
	int n;
	char c;
	bool hex;

	if(ptr<0) return ptr;
	
	n=0;
	hex=false;
	
	while(ptr<size)
	{
		c=data[ptr++];

		if(c>='a'&&c<='z') c-=32;

		if(c=='$'||c=='X')
		{
			hex=true;
			continue;
		}

		if(!((c>='A'&&c<='Z')||(c>='0'&&c<='9'))) break;

		if(!hex)
			{
				if(c>='0'&&c<='9')
				{
					n=n*10+(c-'0');
				}
				else return -1;
			}
			else
			{
				if(c>='0'&&c<='9')
				{
					n=n*16+(c-'0');
				}
				else
				if(c>='A'&&c<='F')
				{
					n=n*16+(c-'A'+10);
				}
				else return -1;
			}
	}

	return n;
}



bool __fastcall TFormMain::OpenNameTable(AnsiString name)
{
	FILE *file;
	unsigned char *src,*out;
	unsigned char *dst_mrle;
	unsigned char dst[1024];
	int i,tag,pp,pd,ptr,wdt,hgt,src_size;
	AnsiString ext;

	ext=GetExt(name);

	file=fopen(name.c_str(),"rb");

	if(!file) return false;

	src_size=get_file_size(file);

	src=(unsigned char*)malloc(src_size);

	if(!src)
	{
		fclose(file);
		return false;
	}

	fread(src,src_size,1,file);
	fclose(file);

	if(ext=="map"||ext=="mrle")
	{
		wdt=src[src_size-4]+src[src_size-3]*256;
		hgt=src[src_size-2]+src[src_size-1]*256;

		if(wdt>NAME_MAX_WIDTH||hgt>NAME_MAX_WIDTH)
		{
			Application->MessageBox("Map size is too large, or a wrong file format","Error",MB_OK);
			free(src);
			return false;
		}

		nameTableWidth=wdt;
		nameTableHeight=hgt;
	}
	if(ext=="map"){
		memcpy(nameTable,src,name_size());
		memcpy(attrTable,&src[name_size()],attr_size());
	}
	else if (ext=="mrle")
	{

			int nt_mrle=wdt*hgt;
			int attr_mrle=(wdt/4)*(hgt/4);
			dst_mrle=(unsigned char*)malloc(nt_mrle+attr_mrle);
			//
			tag=src[0];
			pp=1;
			pd=0;

			while(pp<src_size-4)
			{
				  if(src[pp]==tag)
				  {
					  pp++;
					  if(src[pp]==0) break;
					  for(i=0;i<src[pp];i++) dst_mrle[pd++]=dst_mrle[pd-1];
					  pp++;
				  }
				  else
				  {
					dst_mrle[pd++]=src[pp++];
				  }
			}
			memcpy(nameTable,dst_mrle,nt_mrle);
			memcpy(attrTable,dst_mrle+nt_mrle,attr_mrle);
            free(dst_mrle);

	}
	else
	{
		if(ext=="asm"||ext=="s")
		{
			ptr=0;
			ptr=parse_skip_empty(src,ptr,src_size);
			ptr=parse_skip_value(src,ptr,src_size);
			ptr=parse_skip_tag(src,ptr,src_size,":");

			if(ptr<0)
			{
				Application->MessageBox("Can't parse assembly code","Error",MB_OK);
				free(src);
				return false;
			}

			out=(unsigned char*)malloc(2048);
			pd=0;

			while(pd<2048)
			{
				if(ptr>=src_size) break;
				
				if(ptr<0)
				{
					Application->MessageBox("Error while parsing assembly file","Error",MB_OK);
					free(out);
					free(src);
					return false;
				}

				ptr=parse_skip_empty(src,ptr,src_size);

				if(src[ptr]=='.')
				{
					ptr=parse_skip_tag(src,ptr,src_size,".byte");
					ptr=parse_skip_tag(src,ptr,src_size,".db");    //fg
					ptr=parse_skip_empty(src,ptr,src_size);
				}

				out[pd++]=parse_get_value(src,ptr,src_size);
				ptr=parse_skip_value(src,ptr,src_size);
				ptr=parse_skip_empty(src,ptr,src_size);
				
				if(src[ptr]==',') ++ptr;
			}

			free(src);
			src=out;
			src_size=pd;
			if(src_size!=960&&src_size!=1024) ext="rle";
		}

		if(ext=="h")
		{
			ptr=0;
			ptr=parse_skip_empty(src,ptr,src_size);
			ptr=parse_skip_tag(src,ptr,src_size,"const");
			ptr=parse_skip_empty(src,ptr,src_size);
			ptr=parse_skip_tag(src,ptr,src_size,"unsigned");
			ptr=parse_skip_empty(src,ptr,src_size);
			ptr=parse_skip_tag(src,ptr,src_size,"char");
			ptr=parse_skip_empty(src,ptr,src_size);
			ptr=parse_skip_value(src,ptr,src_size);
			ptr=parse_skip_empty(src,ptr,src_size);
			ptr=parse_skip_tag(src,ptr,src_size,"[");
			ptr=parse_skip_empty(src,ptr,src_size);
			//size=parse_get_value(src,ptr,src_size);
			ptr=parse_skip_value(src,ptr,src_size);
			ptr=parse_skip_empty(src,ptr,src_size);
			ptr=parse_skip_tag(src,ptr,src_size,"]");
			ptr=parse_skip_empty(src,ptr,src_size);
			ptr=parse_skip_tag(src,ptr,src_size,"=");
			ptr=parse_skip_empty(src,ptr,src_size);
			ptr=parse_skip_tag(src,ptr,src_size,"{");

			if(ptr<0)
			{
				Application->MessageBox("Can't parse C header","Error",MB_OK);
				free(src);
				return false;
			}

			out=(unsigned char*)malloc(2048);
			pd=0;

			while(pd<2048)
			{
				if(ptr>=src_size) break;

				if(ptr<0)
				{
					Application->MessageBox("Error while parsing C header","Error",MB_OK);
					free(out);
					free(src);
					return false;
				}

				ptr=parse_skip_empty(src,ptr,src_size);
				out[pd++]=parse_get_value(src,ptr,src_size);
				ptr=parse_skip_value(src,ptr,src_size);
				ptr=parse_skip_empty(src,ptr,src_size);
				if(src[ptr]=='}') break;
				ptr=parse_skip_tag(src,ptr,src_size,",");
			}

			free(src);
			src=out;
			src_size=pd;
			if(src_size!=960&&src_size!=1024) ext="rle";
		}

		if(ext!="nrle" && ext!="rle")  //any other, predominantly .nam
		{
			if(src_size==960||src_size==1024)
			{
				memcpy(nameTable,src,960);

				if(src_size==1024) memcpy(attrTable,&src[960],64);
			}
			else if(src_size==768) // TMS9918 mode 2 (MSX etc)
			{
				//32x24
				memcpy(nameTable,src,768);  //PN without CT (colour table; currently not supported).
				nameTableWidth=32;
				nameTableHeight=24;
			}
			else if (src_size==896) //PCE/TurboGFX
			{
				//32x28
				memcpy(nameTable,src,896);  //no colour information either.
                nameTableWidth=32;
				nameTableHeight=28;
			}

			else
			{
				Application->MessageBox("NES Nametable should be 960 or 1024 bytes long","Error",MB_OK);
				free(src);
				return false;
			}
		}
		else
		{
			if(src_size<2048)
			{
				tag=src[0];
				pp=1;
				pd=0;

				while(pp<src_size)
				{
					if(src[pp]==tag)
					{
						pp++;
						if(src[pp]==0) break;
						for(i=0;i<src[pp];i++) dst[pd++]=dst[pd-1];
						pp++;
					}
					else
					{
						dst[pd++]=src[pp++];
					}
				}
			}

			if(pd==959||pd==1023) pd++;//for really old files saved when RLE packer had a bug
			if(pd==960||pd==1024) memcpy(nameTable,dst,960);
			if(pd==1024) memcpy(attrTable,dst+960,64);
			if(pd!=960&&pd!=1024) Application->MessageBox("Can't decode RLE","Error",MB_OK);
		}

		nameTableWidth=32;
		nameTableHeight=30;
	}

	free(src);

	UpdateAll();

	return true;
}



bool __fastcall TFormMain::OpenPalette(AnsiString name)
{
	FILE *file;
	unsigned char pal[16];
	int i;
	AnsiString ext;
	ext=GetExt(name);
	file=fopen(name.c_str(),"rb");

	if(file)
	{
		if((ext=="dat" && get_file_size(file)>=16) || (get_file_size(file)==16))
		{
			fread(pal,16,1,file);
			fclose(file);

			for(i=0;i<4;i++)
			{
				bgPal[palBank*16+0*4+i]=pal[i+0];
				bgPal[palBank*16+1*4+i]=pal[i+4];
				bgPal[palBank*16+2*4+i]=pal[i+8];
				bgPal[palBank*16+3*4+i]=pal[i+12];
			}

			UpdateAll();

			if(ext=="pal")return true; //non .pal extensions (such as .dat) count as imports and do not overload session file names.
		}
		else
		{
			if (ext=="dat") Application->MessageBox("YY-CHR .dat should be at least 16 bytes long","Error",MB_OK);
			else Application->MessageBox("File should be 16 bytes long","Error",MB_OK);
		}
	}

	return false;
}



void __fastcall TFormMain::GetSelection(TRect r,int &x,int &y,int &w,int &h)
{
	if(r.left<r.right)
	{
		x=r.left;
		w=r.right-x;
	}
	else
	{
		x=r.right;
		w=r.left-x;
	}
	if(r.top<r.bottom)
	{
		y=r.top;
		h=r.bottom-y;
	}
	else
	{
		y=r.bottom;
		h=r.top-y;
	}
}



void __fastcall TFormMain::CopyMap(bool cut)
{
	int i,j,x,y,w,h,pp;

	GetSelection(nameSelection,x,y,w,h);

	if(w&&h&&cut) SetUndo();

	pp=0;

	for(i=0;i<h;++i)
	{
		for(j=0;j<w;++j)
		{
			nameCopy[pp]=nameTable[y*nameTableWidth+x+j];
			attrCopy[pp]=AttrGet(x+j,y,false,false);

			if(cut&&SpeedButtonTiles->Down) nameTable[y*nameTableWidth+x+j]=nullTile;

			++pp;
		}

		++y;
	}

	nameCopyWidth=w;
	nameCopyHeight=h;

	DWORD Len=2*sizeof(int)+sizeof(nameCopy)+sizeof(attrCopy);

	HGLOBAL hDst=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,Len);

	if (hDst)
	{
		LPSTR gDst=(LPSTR)GlobalLock(hDst);

		if (gDst)
		{
			if (OpenClipboard(Handle))
			{
				EmptyClipboard();

				*((int*)&gDst[0*sizeof(int)])=nameCopyWidth;
				*((int*)&gDst[1*sizeof(int)])=nameCopyHeight;
				memcpy(&gDst[8],nameCopy,sizeof(nameCopy));
				memcpy(&gDst[8+sizeof(nameCopy)],attrCopy,sizeof(attrCopy));

				GlobalUnlock(hDst);

				SetClipboardData(CF_NAM,hDst);
				CloseClipboard();
			}
		}
	}

	if(cut) {
		UpdateNameTable(-1,-1,true);
		if(FormNavigator->Visible)
		{
            GetSelection(nameSelection,x,y,w,h);
			FormNavigator->DrawRange(x,y,w,h,false);
			FormNavigator->UpdateLines(false);
		}
	}  //FormNavigator->Draw(false,false);}
}



void __fastcall TFormMain::CopyMapCodeASM(void)
{
	char str[65536],buf[1024];
	int i,j,x,y,w,h;
	x=0;y=0;

	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		GetSelection(nameSelection,x,y,w,h);
		LabelStats->Caption="Selected tilenames (w"+IntToStr(w)+"x"+IntToStr(h)+")\nput on clipboard as ASM text.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
	}
	else
	{
		w=nameTableWidth;
		h=nameTableHeight;
		LabelStats->Caption="Map put on clipboard as ASM text.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
	}


	strcpy(str,"");

	for(i=0;i<h;i++)
	{
		if(byte1->Checked) strcat(str,"\t.byte ");
		if(db1->Checked) strcat(str,"\t.db ");


		for(j=0;j<w;j++)
		{
			sprintf(buf,"$%2.2x%c",nameTable[(y+i)*nameTableWidth+x+j],j<w-1?',':'\n');
			strcat(str,buf);
		}
	}

	Clipboard()->SetTextBuf(str);
}



void __fastcall TFormMain::CopyMapCodeC(bool rle)
{
	char str[65536],buf[1024];
	unsigned char src[65536],dst[65536];
	int i,j,x,y,w,h,ptr,size;
	x=0;y=0;
	bool tmp=false;
	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		GetSelection(nameSelection,x,y,w,h);
		tmp=true;
	}
	else
	{
		w=nameTableWidth;
		h=nameTableHeight;
	}
	if(!rle)
	{
		sprintf(str,"const unsigned char nametable[%i*%i]={\n",w,h);

		for(i=0;i<h;i++)
		{
			strcat(str,"\t");

			for(j=0;j<w;j++)
			{
				sprintf(buf,"0x%2.2x",nameTable[(y+i)*nameTableWidth+x+j]);

				strcat(str,buf);

				if(i*w+j<w*h-1) strcat(str,",");
			}

			strcat(str,"\n");
		}
	}
	else
	{
		ptr=0;

		for(i=0;i<h;i++) for(j=0;j<w;j++) src[ptr++]=nameTable[(y+i)*nameTableWidth+x+j];

		size=encode_rle(src,ptr,dst);

		if(size<0)
		{
			Application->MessageBox("No unused tiles found, can't be encoded with RLE","Error",MB_OK);
			return;
		}

		sprintf(str,"const unsigned char nametable[%i]={\n",size);

		for(i=0;i<size;++i)
		{
			sprintf(buf,"0x%2.2x",dst[i]);
			strcat(str,buf);

			if(i<size-1) strcat(str,",");
			if(((i&15)==15)||(i==size-1)) strcat(str,"\n");
		}
	}

	strcat(str,"};\n\n");
	Clipboard()->SetTextBuf(str);

	if(tmp)
	{
		LabelStats->Caption="Selected names (w"+IntToStr(w)+"x"+IntToStr(h)+")\nput on clipboard as C text.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;

	}
	else
	{
		LabelStats->Caption="Map put on clipboard as C text.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
    }
}




void __fastcall TFormMain::CopyMetaSpriteCodeC(bool hflip)
{
	char str[65536],buf[1024];
	int i,j,x,y,w,h;

	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		GetSelection(nameSelection,x,y,w,h);

		sprintf(str,"const unsigned char metasprite[]={\n");

		if(!hflip)
		{
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					sprintf(buf,"\t%s,%s,0x%2.2x,%i,\n",num_to_3char(j*8),num_to_3char(i*8),nameTable[(y+i)*nameTableWidth+x+j],AttrGet(x+j,y+i,false,false));
					strcat(str,buf);
				}
			}
		}
		else
		{
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					sprintf(buf,"\t%s,%s,0x%2.2x,%i|OAM_FLIP_H,\n",num_to_3char(j*8),num_to_3char(i*8),nameTable[(y+i)*nameTableWidth+x+(w-1-j)],AttrGet(x+(w-1-j),y+i,false,false));
					strcat(str,buf);
				}
			}
		}

		strcat(str,"\t128\n};\n\n");

		Clipboard()->SetTextBuf(str);
	}
	else
	{
		LabelStats->Caption="WARNING: Needs a map selection. No action was taken.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
    }
}



void __fastcall TFormMain::PasteMap(void)
{
	int i,j,x,y,w,h,pp;

	OpenClipboard(Handle);

	HGLOBAL hClipBuf = GetClipboardData(CF_NAM);

	if (hClipBuf)
	{
		LPSTR gSrc = (LPSTR)GlobalLock(hClipBuf);

		if (gSrc != NULL)
		{
			nameCopyWidth =*((int*)&gSrc[0*sizeof(int)]);
			nameCopyHeight=*((int*)&gSrc[1*sizeof(int)]);
			memcpy(nameCopy,&gSrc[8],sizeof(nameCopy));
			memcpy(attrCopy,&gSrc[8+sizeof(nameCopy)],sizeof(attrCopy));

			GlobalUnlock(gSrc);
		}
	}

	CloseClipboard();

	if(nameCopyHeight<1||nameCopyWidth<1) return;

	GetSelection(nameSelection,x,y,w,h);
	int bufY = y;

	if(w&&h) SetUndo();

	pp=0;

	for(i=0;i<nameCopyHeight;i++)
	{
		for(j=0;j<nameCopyWidth;j++)
		{
			if(x+j<nameTableWidth&&y<nameTableHeight)
			{
				if(nameCopy[pp]!=nullTile||!ingorepastingnulltiles1->Checked)
				{
					if(SpeedButtonTiles->Down) nameTable[y*nameTableWidth+x+j]=nameCopy[pp];
					if(SpeedButtonPal->Down) AttrSet(x+j,y,attrCopy[pp],false);
				}
			}
			pp++;
		}
		y++;
	}

	//FormNavigator->Draw(false,false);
	if(FormNavigator->Visible) {
		FormNavigator->DrawRange(x,bufY,nameCopyWidth,nameCopyHeight,false);
		FormNavigator->UpdateLines(false);
	}
	UpdateNameTable(-1,-1,true);
}



void __fastcall TFormMain::FillMap(bool bUseNull)
{
	int i,j,x,y,w,h;
	int tx,ty,tw,th;
	int tile;
	GetSelection(nameSelection,x,y,w,h);
	GetSelection(chrSelection,tx,ty,tw,th);

	int bufY=y;

	if(w&&h) SetUndo();

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{

			tile = tileActive+(j%tw)+((i%th)*16);

			tile = bUseNull ? nullTile:tile;
			if(SpeedButtonTiles->Down) nameTable[y*nameTableWidth+x+j]=tile;
			if(SpeedButtonPal->Down) AttrSet(x+j,y,palActive,false);
		}
		y++;
	}

	UpdateNameTable(-1,-1,true);
    if(FormNavigator->Visible)
		{
			FormNavigator->DrawRange(x,bufY,w,h,false);
			FormNavigator->UpdateLines(false);
		}

	//FormNavigator->Draw(false,false);
}



void __fastcall TFormMain::OpenAll(AnsiString name)
{
	if(OpenCHR(name+".chr"))
	{
		SaveDialogChr->FileName=name;
	}
	else
	{
		if(OpenCHR(name+".bin")) SaveDialogChr->FileName=name;
	}

	if(OpenNameTable(name+".nam"))
	{
		SaveDialogName->FileName=name;
	}
	else
	{
		if(OpenNameTable(name+".rle"))
		{
			SaveDialogName->FileName=name;
		}
		else
		{
			if(OpenNameTable(name+".map"))
			{
				SaveDialogMap->FileName=name;
			}
		}
	}

	if(OpenPalette(name+".pal"))
	{
		SaveDialogPal->FileName=name;
	}

	if(OpenMetaSprites(name+".msb"))
	{
		SaveDialogMetaSpriteBank->FileName=name;
	}
	if(OpenTprop(name+".tprop"))
    {
		SaveDialogTprop->FileName=name;
	}
}



void __fastcall TFormMain::UpdateStats(void)
{
	if (holdStats==true) {cueStats=true; return;}

	bool bClone = ( bBufCtrl &&  !bBufShift && bBufAlt);
	//bool bSwap	= (!bBufCtrl && !bBufShift && !bBufAlt);     //commented out to suppress warning
	bool bMove	= ( bBufCtrl && !bBufShift && !bBufAlt);

	//AnsiString str;
	//AnsiString tmp;
	int i,j,vx,vy,cnt,tile,off,col,sel,frame;

	cueStats=false;
	throttleOK=false;
	FormCaption = Application->Title;
	//first do the window titlebar
	strStat=FormCaption;
	AppendStr(strStat,"  ");

	if(nameTableWidth==32&&nameTableHeight==30)
	{
		//str+="[std nametable]";
		TabSheetName->Caption="[F1] Screen";
		AppendStr(strStat,"[standard screen]");
	}
	else
	{
		TabSheetName->Caption="[F1] Map";
		vx=nameTableViewX;
		vy=nameTableViewY;

		if(vx<0) vx=0;
		if(vy<0) vy=0;

		AppendStr(strStat,"[map "+IntToStr(nameTableWidth)
			+"x"+IntToStr(nameTableHeight)+"]"
			+"["+IntToStr(vx)+","+IntToStr(vy)+"]");
	}

	frame=GetNameTableFrame();

	if(frame>=0) AppendStr(strStat," [frame "+IntToStr(frame)+"]");



	//Filename in titlebar
	AppendStr(strStat," "+SaveDialogSession->FileName);
	if(unsavedChanges==true) AppendStr(strStat,"*");

	Caption=strStat;

	//-----
	//now do the status bar
	//-----
	if (GetKeyState(VK_CAPITAL)&&(!SpeedButtonTypeIn->Down))
		{cueStats=true;return;}

	bAllowLockMessage=true;

	strStat="---";
	if(spriteHover<0)
	if(PageControlEditor->ActivePage==TabSheetSprite)
	if(ListBoxSpriteList->Items->Count)
	{
		//if (str=="---") str="";
		//else str+="\t\t\t";

		strStat=IntToStr(ListBoxSpriteList->Items->Count)+" object(s) in metasprite "
			+IntToStr(metaSpriteActive)+".";
		tmpStat=strStat+"\t";
	}
	else {tmpStat="Metasprite slot is vacant.\t"; strStat=tmpStat;}

	if(tileXC>=0&&tileYC>=0)
	{
		//if (SpeedButtonMarqTile->Down)
		//{
			tile=tileViewTable[tileYC*16+tileXC];
			cnt=0;
			int selcnt=0;
			for(i=0;i<nameTableWidth*nameTableHeight;i++) if(nameTable[i]==tile) ++cnt;

			sel=0;

			for(i=0;i<256;i++) if(chrSelected[tileViewTable[i]])
			{
				++sel;
				for(j=0;j<nameTableWidth*nameTableHeight;j++) if(nameTable[j]==tileViewTable[i]) ++selcnt;
			}

			strStat="";
			strStat+=tmpStat
				+"Tile: $"+IntToHex(tile,2)
				+"\tEntries: "+IntToStr(cnt)
				+"\tSelected: "+IntToStr(sel)
				+"\tEntries in selection: "+IntToStr(selcnt);
		//}
		
		if (SpeedButtonMarqTile->Down) strStat+="\nSelect: click\tBox-select: shift-drag\t\tMulti-select: ctrl-click\t\tSwitch mode: shift+D";
		if (SpeedButtonDrawTile->Down) strStat+="\nPen: click\tEyedropper: right-click\tBucket: ctrl-click\nBox select: shift-drag\t\tSwitch mode: shift+D";



	}
	if(bImageNameAccepted||bImageTileAccepted)
	{
		strStat="Drag: ";
		if(bClone)		strStat+="CLONE.\tCopies selection to destination.\n\t\t\tModifiers: [Ctrl] Move content.  [Ctrl+Alt] Clone.";
		else if(bMove)  strStat+="MOVE.\t\tMoves selection to destination; leaving 'null tiles' behind.\n\t\t\tModifiers: [Ctrl] Move content.  [Ctrl+Alt] Clone";
		else   			strStat+="SWAP.\t\tSwaps selection with destination.\n\t\t\tModifiers: [Ctrl] Move content.  [Ctrl+Alt] Clone.\n\n\t\t\tIf Diagonal swaps overlap, the content swapped back reorders to fit vacant space.";

	}
	
	if(nameXC>=0&&nameXC<nameTableWidth&&nameYC>=0&&nameYC<nameTableHeight)
	{
			off=nameYC*nameTableWidth+nameXC;
			int isScreen = 0;
			if  (nameTableWidth*nameTableHeight == 32*30) isScreen = 32*30;
			strStat="XY: "+IntToStr(nameXC)+","+IntToStr(nameYC)
				+" \tOff: $"+IntToHex(off,4)
				+"\tName: $"+IntToHex(nameTable[off],2)
				+"\tAtXY: "+IntToStr(nameXC/2)+","+IntToStr(nameYC/2)
				+"\tAtOff: $"+IntToHex( nameYC/4*(nameTableWidth/4) + nameXC/4+isScreen,4)    //nameTableWidth*nameTableHeight
				+"."+IntToStr((nameXC&2)+(nameYC&2)*2)
				+"="+IntToStr(AttrGet(nameXC,nameYC,false,false))
				+"\t($"+IntToHex(attrTable[nameYC/4*(nameTableWidth/4)+nameXC/4],2)+")"
				+"\tMetaSpr:"+IntToStr(metaSpriteActive);
			if(nameSelection.left>=0)
				strStat+="\tWH:"+IntToStr(nameSelection.right-nameSelection.left)
				+","+IntToStr(nameSelection.bottom-nameSelection.top);
			if (SpeedButtonMarqTile->Down&&nameSelection.left>=0) //&&!bOutsideSel
				strStat+="\nMove selection: [Shift+Rightdrag]\t\tSwap: [Right-drag]\t\t\tFill selection: [F]\nMove content:  [Ctrl+Rightdrag]\t\tClone: [Ctrl+Alt+Rightdrag]";

			else if (SpeedButtonMarqTile->Down) strStat+="\nPlace: click\tFind tile: right-click \tSelect: shift-drag\tQuickpaste: V+click\tSwitch mode: shift+D";
			if (SpeedButtonDrawTile->Down) strStat+="\nPen: click\tEyedropper: right-click\tBucket: ctrl-click\tSelect: shift-drag\tSwitch mode: shift+D";

			//if(nameSelection.left>=0) str+="\nTip:\t[F] to fill selection.";

	}

	if(palHover>=0)
	{
		strStat="Pal:"+IntToStr(palHover)+"\tEntry:"
			+IntToStr(palColHover)
			+"\tAdr:"+IntToHex(0x3f00+palHover*4+palColHover,4)
			+"\tColour:$"+IntToHex(bgPal[palBank*16+palHover*4+palColHover],2)
			+"\nTip:\tC-click / V-click a subpalette to copy/paste. Shift-C/Shift-V to copy/paste the whole set."
			+"\n\tV-click repeatedly to cycle pastes from a clipboarded palette set.";
			//+ " v key: " + BoolToString(clickV);
	}

	if(colHover>=0)
	{
		col=colHover;

		//if(col==0x0d||col==0x1d) col=0x0f;

		strStat="Colour: $"+IntToHex(col,2);
		if(safeColsOnly)
		{
			switch(col)    //fg todo: make this optional
			{
				case 0x0d:
				case 0x1d:
				case 0x0e:
				case 0x1e:
				case 0x2e:
				case 0x3e:
				case 0x1f:
				case 0x2f:
				case 0x3f:
				strStat+=" -> $0F\t\t\t\t\t";
				break;
				default: strStat+="\t\t\t\t\t\t";
			}
		}
		strStat+="Click: Pick.\tAlt-click: Pick backdrop.\nCtrl-click: Ripple-pick subpalette.\t\t\tCtrl-rightclick: Ripple-skip through subpalette.\nCtrl+Shift-click: Ripple-pick whole set.\t\tCtrl+Shift-rightclick: Ripple-skip through whole set.\nShift-click: Pick and step to next subpal.\t\tShift-rightclick: Skip to next subpal.";
		}
	if(spriteHover>=0)
	{
		off=metaSpriteActive*64*4+spriteHover*4;

		strStat="Sprite:"+IntToStr(spriteHover)
			+"\tX:"+IntToStr(metaSprites[off+3]-spriteGridX)
			+"\tY:"+IntToStr(metaSprites[off]-spriteGridY)
			+"\tTile:$"+IntToHex(metaSprites[off+1],2)
			+"\tPal:"+IntToStr(metaSprites[off+2]&3);

		if(metaSprites[off+2]&OAM_FLIP_H) strStat+=" HFlip";
		if(metaSprites[off+2]&OAM_FLIP_V) strStat+=" VFlip";

	}

	if(CHREditorHover>=0)
	{
		strStat="Pen: click\tBucket: ctrl-click\tEyedropper: Right-click\nIncrement mode ink: shift-click\tDecrement mode ink: shift-alt-click.";
	}
	if(bigTilesHover>=0&&PageControlEditor->ActivePage==TabSheetTile)
	{
		if (SpeedButtonMarqTile->Down) strStat="[Layout mode]\tSwitch mode: Shift+D\nSelect: click\t\tBox-select: shift-drag";
		if (SpeedButtonDrawTile->Down) strStat="[Draw mode]\t\tSwitch mode: Shift+D\nPen: click\t\tBucket: ctrl-click\nSample: right-click\tNC-Bucket: ctrl+alt+click\nBox select: shift-drag";

		if(tileXC>=0&&tileYC>=0&&SpeedButtonMarqTile->Down)
		{
			tile=tileYC*16+tileXC;
			cnt=0;
			int selcnt=0;
			for(i=0;i<nameTableWidth*nameTableHeight;i++) if(nameTable[i]==tile) ++cnt;

			sel=0;

			for(i=0;i<256;i++) if(chrSelected[i])
			{
				++sel;
				for(j=0;j<nameTableWidth*nameTableHeight;j++) if(nameTable[j]==i) ++selcnt;
			}
			strStat+="\nTile: $"+IntToHex(tile,2)
				+"\t\tEntries: "+IntToStr(cnt)
				+"\nSelected: "+IntToStr(sel)
				+"\t\tEntries in selection: "+IntToStr(selcnt);
		}
	}
	LabelStats->Caption=strStat;

	strStat="";
	tmpStat="";
}



void __fastcall TFormMain::SetUndo(void)
{
	//BROKE STUDIO
	int i;
	for(i=0;i<256;i++) metaSpriteNamesUndo[i]=metaSpriteNames[i];
	//

	//FG: these were added because of the new toggle behaviour of sort/clear actions

		MCHRRemoveDoubles->Checked=false;
		MCHRRemoveUnused->Checked=false;

		MCHRRemoveDoubles->Caption="&Remove duplicates";
		MCHRRemoveUnused->Caption="Re&move unused";

		MCHRFreqSort->Checked=false;
		MCHRDensitySort->Checked=false;

		MCHRFreqSort->Caption="Sort by &frequency";
		MCHRDensitySort->Caption="Sort by d&ensity";

		MCHRFindUnused->Checked=false;
		MCHRFindDoubles->Checked=false;

		MCHRFindUnused->Caption="Find &unused";
        MCHRFindDoubles->Caption="Find &duplicates";

	unsavedChanges=true;
	Savesession1->Enabled=true;
	UpdateStats();
	//


	undoNameTableWidth	=	nameTableWidth;
	undoNameTableHeight =	nameTableHeight;

	undoSpriteGridX		=	spriteGridX;
	undoSpriteGridY     =   spriteGridY;

	memcpy(bgPalUndo,bgPal,sizeof(bgPal));
	memcpy(undoChr,chr,8192);
	memcpy(undoNameTable,nameTable,name_size());
	memcpy(undoAttrTable,attrTable,attr_size());
	memcpy(undoMetaSprites,metaSprites,sizeof(undoMetaSprites));

	memcpy(tilePropertiesUndo,tileProperties,sizeof(tilePropertiesUndo));
	memcpy(propConditionalUndo,propConditional,sizeof(propConditionalUndo));
	memcpy(propConditionUndo,propCondition,sizeof(propConditionUndo));

	//chr bank stuff
	undo_chrBanks = chrBanks;
	undo_chrBankLabels = chrBankLabels;

	memcpy(undo_chrA_id,chrA_id,16);
	memcpy(undo_chrB_id,chrB_id,16);

	size_t tmpSize = chrBanks*4096;
	size_t tmpSize_undo = undo_chrBanks*4096;
	if (tmpSize > tmpSize_undo) undo_chrBank = (char*) realloc(undo_chrBank, tmpSize);
	memcpy(undo_chrBank, chrBank, tmpSize);
    


}



void __fastcall TFormMain::Undo(void)
{
    //BROKE STUDIO
	int i,tmp;
	AnsiString tmpAnsi;

	for(i=0;i<256;i++)
	{
		tmpAnsi=metaSpriteNames[i];
		metaSpriteNames[i]=metaSpriteNamesUndo[i];
		metaSpriteNamesUndo[i]=tmpAnsi;
	}

	//figure out which is larger to copy the right size.
	int tmp_AttrSize = (nameTableWidth+3)/4*((nameTableHeight+3)/4);
	int tmp_UndoAttrSize = (undoNameTableWidth+3)/4*((undoNameTableHeight+3)/4);
	int tmp_MaxAttr = tmp_AttrSize > tmp_UndoAttrSize
					? tmp_AttrSize : tmp_UndoAttrSize;

	int tmp_NameSize = nameTableWidth*nameTableHeight;
	int tmp_UndoNameSize = undoNameTableWidth*undoNameTableHeight;
	int tmp_MaxName = tmp_NameSize > tmp_UndoNameSize
					? tmp_NameSize : tmp_UndoNameSize;

	mem_exchange(bgPalUndo,bgPal,sizeof(bgPal));
	mem_exchange(undoChr,chr,8192);

	mem_exchange(undoNameTable,nameTable,tmp_MaxName);     //name_size()
	mem_exchange(undoAttrTable,attrTable,tmp_MaxAttr);     //attr_size()

	mem_exchange(undoMetaSprites,metaSprites,sizeof(undoMetaSprites));

	mem_32exchange(tilePropertiesUndo,tileProperties,sizeof(tilePropertiesUndo));
	mem_exchange(propConditionalUndo,propConditional,sizeof(propConditionalUndo));
	swap_bool_2d(propConditionUndo, propCondition, 4, 8);
	//mem_exchange(propConditionUndo,propCondition,sizeof(propConditionUndo));


	tmp=nameTableWidth;		nameTableWidth=undoNameTableWidth;	undoNameTableWidth=tmp;
	tmp=nameTableHeight;	nameTableHeight=undoNameTableHeight;	undoNameTableHeight=tmp;

	tmp=spriteGridX;        spriteGridX=undoSpriteGridX;	undoSpriteGridX=tmp;
	tmp=spriteGridY;        spriteGridY=undoSpriteGridY;    undoSpriteGridY=tmp;

	//chr bank stuff
	tmp=chrBanks;			chrBanks=undo_chrBanks;			undo_chrBanks=tmp;

	tmp_chrBankLabels	= chrBankLabels;
	chrBankLabels		= undo_chrBankLabels;
	undo_chrBankLabels	= tmp_chrBankLabels;

	mem_32exchange(undo_chrA_id	,chrA_id	,sizeof(chrA_id));
	mem_32exchange(undo_chrB_id	,chrB_id	,sizeof(chrB_id));



	size_t tmpSize = max(chrBanks*4096,undo_chrBanks*4096);

	if (tmpSize > chrBanks*4096)
		chrBank = (char*)realloc(chrBank,tmpSize);
	if (tmpSize > undo_chrBanks*4096)
		undo_chrBank = (char*)realloc(undo_chrBank,tmpSize);

	mem_exchange(undo_chrBank,	chrBank	,tmpSize);



	//sanitize line drawing buffer
	memset (arr_linePlotCHR,0,sizeof(arr_linePlotCHR2));
	memset (arr_linePlotCHR2,0,sizeof(arr_linePlotCHR2));
	memcpy (chrBuf, chr, 8192);
	lineUpX= -1;
	lineUpY= -1;

	UpdateAll();
}


void __fastcall TFormMain::SetTile(int tile)
{
	int i;
	bool b=FormCHREditor->btn2x2mode->Down;
	int m=b?2:1;
	int n=m;
	if(b)  // done at right or wrong level? This turns some of the latter steps redundancies but keeping them will allow for an easy reversal if i change my mind. 
	{
		if(tile%16==15) tile--;
		if(tile/16==15) tile-=16;
	}
	tileActive=tile;
	chrSelection.left=tile&15;
	chrSelection.top=tile/16;

	//these only have meaningful effect if b is true since they serve to
	//revert its effect in corner cases. If this is not desireable in the future
	//they can be wrapped in the b condition, similar to what is done below for
	//tileSelRect.

	if(!((tileActive+1)&0x0F))    m=1;
	if(tileActive>=0xF0)		  n=1;

	chrSelection.right=chrSelection.left+m;
	chrSelection.bottom=chrSelection.top+n;

	for(i=0;i<256;++i) chrSelected[i]=0;


	if(b){
		//these are used by "selected only"
		chrSelected[tile]=1;
		if(m>1)chrSelected[tile+1]=1;
		if(n>1)chrSelected[tile+16]=1;
		if(m>1&&n>1)chrSelected[tile+17]=1;
		//chr editor draw tools operate on these.
		tileSelRectWdt=(tileActive&15)<0x0F?2:1;
		tileSelRectHgt=tileActive<0xF0?2:1;
	}
	else
	{
		chrSelected[tile]=1;
		tileSelRectWdt=1;
		tileSelRectHgt=1;

	}
	chrSelectRect=true;
	cueUpdateTiles=true;
	cueUpdateNametable=true;
	//UpdateTiles(true);
}



void __fastcall TFormMain::SetBMPPalette(Graphics::TBitmap* bmp)
{
	PALETTEENTRY pal[16];
	int i,col;

	for(i=0;i<16;i++)
	{
		col=outPalette[bgPal[palBank*16+(i>>2)*4+(i&3)]];

		pal[i].peBlue =(col>>16)&0xff;
		pal[i].peGreen=(col>>8)&0xff;
		pal[i].peRed  =(col&0xff);
	}

	SetPaletteEntries(bmp->Palette,0,16,pal);
}





int fget_i32(FILE *file)
{
	unsigned char buf[4];

	fread(buf,4,1,file);

	return buf[0]|(buf[1]<<8)|(buf[2]<<16)|(buf[3]<<24);
}



bool fget_bool(FILE *file)
{
	return fgetc(file)?true:false;
}



AnsiString fget_ansistring(FILE *file)
{
	AnsiString str;
	char *temp;
	int len;

	len=fget_i32(file);

	temp=(unsigned char*)malloc(len+1);
	fread(temp,len,1,file);
	temp[len]=0;
	str=temp;
	free(temp);

	return str;
}



void nss_put_bytes(FILE *file,const char *name,unsigned char *data,int size)
{
	int i,byte,cnt;

	fprintf(file,"%s",name);

	byte=-1;
	cnt=0;

	for(i=0;i<size;++i)
	{
		if(byte!=data[i])
		{
			if(cnt>1) fprintf(file,"[%x]",cnt);

			byte=data[i];

			fprintf(file,"%2.2x",byte);

			cnt=1;
		}
		else
		{
			++cnt;
		}
	}

	if(cnt>1) fprintf(file,"[%x]",cnt);

	fprintf(file,"\n");
}
void nss_put_bools(FILE *file, const char *name, bool *data, int size) {
	int i, cnt;
	bool bit;

	fprintf(file, "%s", name);

	bit = false;
	cnt = 0;

	for (i = 0; i < size; ++i) {
		if (bit != data[i]) {
			if (cnt > 1) fprintf(file, "[%x]", cnt);

			bit = data[i];

			fprintf(file, "%1.1x", bit ? 1 : 0);

			cnt = 1;
		} else {
			++cnt;
		}
	}

	if (cnt > 1) fprintf(file, "[%x]", cnt);

	fprintf(file, "\n");
}
void nss_put_2D_bools(FILE *file, const char *name, bool data[][8], int rows, int cols) {
	int i, j, cnt;
	bool bit;

	fprintf(file, "%s", name);

    bit = false;
	cnt = 0;

	for (i = 0; i < cols; ++i) {
		for (j = 0; j < rows; ++j) {
			if (bit != data[j][i]) {
                if (cnt > 1) fprintf(file, "[%x]", cnt);

                bit = data[j][i];

				fprintf(file, "%1.1x", bit ? 1 : 0);

                cnt = 1;
            } else {
                ++cnt;
            }
        }
    }

    if (cnt > 1) fprintf(file, "[%x]", cnt);

    fprintf(file, "\n");
}


void nss_put_uint32s(FILE *file, const char *name, uint32_t *data, int size) {
	int i, cnt;
	uint32_t element;
	fprintf(file, "%s", name);
	size=size/4;
	element = 0xFFFFFFFF;
	cnt = 0;

	for(i=0;i<size;++i)
	{
		if(element!=data[i])
		{
			if(cnt>1) fprintf(file,"[%x]",cnt);

			element=data[i];

			fprintf(file,"%8.8x",element);

			cnt=1;
		}
		else
		{
			++cnt;
		}
	}

	if (cnt > 1) fprintf(file, "[%x]", cnt);

	fprintf(file, "\n");
}

bool nss_get_bytes(char* data,int size,const char* tag,unsigned char *dst,int dst_size)
{
	char c1,c2;
	int ptr,len,prev;

	len=strlen(tag);
	ptr=0;

	while (size - len > 0) //while(size)
	{
		if(!memcmp(data,tag,len))
		{
			data+=len;
			prev=0;

			while(ptr<dst_size)
			{
				c1=*data++;

				if(c1=='[')	//repeat tag
				{
					len=0;

					while(1)
					{
						c1=*data++;

						if(c1==']') break;

						if(c1>='0'&&c1<='9') c1=c1-'0'; else if(c1>='a'&&c1<='f') c1=c1-'a'+10; else return false;

						len=(len<<4)|c1;
					}

					while(len>1)
					{
						dst[ptr++]=prev;

						--len;
					}

					continue;
				}

				c2=*data++;

				if(c1>='0'&&c1<='9') c1=c1-'0'; else if(c1>='a'&&c1<='f') c1=c1-'a'+10; else return false;
				if(c2>='0'&&c2<='9') c2=c2-'0'; else if(c2>='a'&&c2<='f') c2=c2-'a'+10; else return false;

				prev=(c1<<4)+c2;

				dst[ptr++]=prev;
			}

			return true;
		}

		++data;
		--size;
	}

	return false;
}
void nss_get_2D_bools(const char* data, int size, const char* tag, bool dst[][8], int rows, int cols)
{
    int ptr, len;
    bool bit;

    len = strlen(tag);
    ptr = 0;

    while (size - len > 0) {
        if (!memcmp(data, tag, len)) {
            data += len;
            bit = false;
            ptr = 0;

            while (ptr < rows * cols) {
                char c = *data++;

                if (c == '[') {
                    len = 0;

                    while (1) {
                        c = *data++;

                        if (c == ']') break;

                        if (c >= '0' && c <= '9') {
                            len = len * 16 + (c - '0');
                        } else if (c >= 'a' && c <= 'f') {
                            len = len * 16 + (c - 'a' + 10);
                        } else {
                            return;
                        }
                    }

                    for (int i = 0; i < len - 1; i++) {
                        int col = ptr / rows;
                        int row = ptr % rows;
                        dst[row][col] = bit;
                        ptr++;
                    }
                } else {
                    bit = (c == '1');
                    int col = ptr / rows;
                    int row = ptr % rows;
                    dst[row][col] = bit;
                    ptr++;
                }
            }

            return;
        }

        data++;
        size--;
    }
}
bool nss_get_bools(char* data, int size, const char* tag, bool* dst, int dst_size) {
	int tag_len = strlen(tag);
	int ptr= 0;

	while (size >= tag_len) {
		if (!memcmp(data, tag, tag_len)) {
			data += tag_len;

			bool bit = false;
			int cnt;// = 0;

			while (ptr < dst_size) {
				char c = *data++;

                if (c == '[') {
					int repeat_cnt = 0;

                    while (1) {
						c = *data++;

                        if (c == ']') {
                            break;
						}

                        if (c >= '0' && c <= '9') {
							repeat_cnt = (repeat_cnt << 4) | (c - '0');
						} else if (c >= 'a' && c <= 'f') {
                            repeat_cnt = (repeat_cnt << 4) | (c - 'a' + 10);
                        } else {
                            return false;
                        }
                    }

                    while (repeat_cnt-- > 1) {
                        dst[ptr++] = bit;
                    }

                    continue;
                }

                if (c == '0') {
                    bit = false;
                } else if (c == '1') {
                    bit = true;
                } else {
                    return false;
                }

                dst[ptr++] = bit;
            }

            return true;
		}

        ++data;
        --size;
    }

    return false;
}

std::vector<std::string> deserializeVector(const char* text, const char* prefix) {
    std::vector<std::string> result;
    const size_t prefixLen = strlen(prefix);
    const char* start = strstr(text, prefix);
	if (start != NULL) {
        start += prefixLen;
        const char* end = start;
		while (*end != '\0' && *end != '\n') {
            if (*end == ',') {
                result.push_back(std::string(start, end - start));
                start = end + 1;
            } else if (*end == '=') {
                break;
			}
            ++end;
        }
        // Add the last element (if any) to the vector.
        if (start != end) {
            result.push_back(std::string(start, end - start));
        } else {
            result.push_back("(missing)");
        }
    } else {
        result.push_back("(missing)");
    }
    return result;
}


int hexstr_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return 10 + c - 'a';
    } else if (c >= 'A' && c <= 'F') {
        return 10 + c - 'A';
    } else {
		return -1; // invalid hexadecimal digit
	}
}

bool nss_get_uint32s(const char* data, int size, const char* tag, uint32_t* dst, int dst_size) {
	int len = strlen(tag);
	int i = 0, ptr = 0;
	uint32_t prev, byte = 0xFFFFFFFF;
	bClearUint32Array=false;
	bool notag=false;
	while (i < size) {
		if (!memcmp(data + i, tag, len))
		{
			i += len;
			ptr = 0;
			prev = 0xFFFFFFFF;

			while (ptr < dst_size) {
				char c1 = data[i++];

				if (c1 == '[') {  // repeat tag
					int repeat_count = 0;

					while (true) {
						char c2 = data[i++];
						if (c2 == ']') break;

						if (c2 >= '0' && c2 <= '9') {
							repeat_count = (repeat_count << 4) | (c2 - '0');
						} else if (c2 >= 'a' && c2 <= 'f') {
							repeat_count = (repeat_count << 4) | (c2 - 'a' + 10);
						} else {


							return false;

						}
					}

					if (byte == 0xFFFFFFFF) {
						// missing first byte

						return false;
					}

					for (int j = 0; j < repeat_count-1; j++) {
						dst[ptr++] = byte;
					}

					byte = 0xFFFFFFFF;

					continue;
				}

				char c2 = data[i++];
				char c3 = data[i++];	char c4 = data[i++];
				char c5 = data[i++];	char c6 = data[i++];
				char c7 = data[i++];	char c8 = data[i++];

				if (c1 >= '0' && c1 <= '9') {c1 -= '0';}
				else if (c1 >= 'a' && c1 <= 'f') {c1 = c1 - 'a' + 10;
				} else {return false;}

				if (c2 >= '0' && c2 <= '9') {c2 -= '0';}
				else if (c2 >= 'a' && c2 <= 'f') {c2 = c2 - 'a' + 10;}
				else {return false;}

				if (c3 >= '0' && c3 <= '9') {c3 -= '0';}
				else if (c3 >= 'a' && c3 <= 'f') {c3 = c3 - 'a' + 10;
				} else {return false;}

				if (c4 >= '0' && c4 <= '9') {c4 -= '0';}
				else if (c4 >= 'a' && c4 <= 'f') {c4 = c4 - 'a' + 10;}
				else {return false;}

				if (c5 >= '0' && c5 <= '9') {c5 -= '0';}
				else if (c5 >= 'a' && c5 <= 'f') {c5 = c5 - 'a' + 10;
				} else {return false;}

				if (c6 >= '0' && c6 <= '9') {c6 -= '0';}
				else if (c6 >= 'a' && c6 <= 'f') {c6 = c6 - 'a' + 10;}
				else {return false;}

				if (c7 >= '0' && c7 <= '9') {c7 -= '0';}
				else if (c7 >= 'a' && c7 <= 'f') {c7 = c7 - 'a' + 10;
				} else {return false;}

				if (c8 >= '0' && c8 <= '9') {c8 -= '0';}
				else if (c8 >= 'a' && c8 <= 'f') {c8 = c8 - 'a' + 10;}
				else {return false;}

				int tmp_el = (c1 << 4) | c2;
				tmp_el = (tmp_el << 4) | c4;
				tmp_el = (tmp_el << 4) | c5;
				tmp_el = (tmp_el << 4) | c6;
				tmp_el = (tmp_el << 4) | c7;
				byte = (tmp_el << 4) | c8;

				//byte = (c1 << 4) | c2;


				if ((byte != prev) || prev == 0xFFFFFFFF) {
					if (ptr == dst_size) {
						// not enough space in dst

						return false;
					}
					dst[ptr++] = byte;

					prev = byte;

				}
			}

			return true;
		}
		else
		{
			notag=true;
		}
		i++;
	}
	if(notag){
			  bClearUint32Array=true;
	}//Application->MessageBox("Opening old (pre nexxt 1.0) session.","Warning",MB_OK);
	return false;
}

char* nss_get_str(char* data,int size,const char* tag)
{
	static char str[16384];
	char c;
	int ptr,len;

	len=strlen(tag);
	ptr=0;

	while(size)
	{
		if(!memcmp(data,tag,len))
		{
			data+=len;

			while(size)
			{
				c=*data++;
				if(c<32) break;

				str[ptr++]=c;
			}

			str[ptr]=0;

			return str;
		}

		++data;
		--size;
	}

	return NULL;
}



int nss_get_int(char* data,int size,const char* tag)
{
	char c;
	int len,n,sign;

	sign=1;

	len=strlen(tag);

	while(size)
	{
		if(!memcmp(data,tag,len))
		{
			data+=len;
			n=0;

			c=*data;

			if(c=='-')
			{
				sign=-1;
				*data++;
			}

			while(size)
			{
				c=*data++;
				if(c<'0'||c>'9') break;

				n=n*10+(c-'0');

				--size;
			}

			return n*sign;
		}

		++data;
		--size;
	}

	return 0;
}



bool nss_get_bool(char* data,int size,const char* tag)
{
	return nss_get_int(data,size,tag)?true:false;
}



bool __fastcall TFormMain::LoadSession1x(AnsiString filename)
{
	FILE *file;
	unsigned char temp[16];
	int i,j;

	file=fopen(filename.c_str(),"rb");

	if(!file) return false;

	//signature

	fread(temp,sizeof(sessionIDStr),1,file);

	if(memcmp(temp,sessionIDStr,sizeof(sessionIDStr)))
	{
		fclose(file);
		Application->MessageBox("Session data does not match current version of the program or has been corrupted","Error",MB_OK);

		return false;
	}

	//arrays

	fread(chr      ,sizeof(chr)      ,1,file);
	fread(chrCopy  ,sizeof(chrCopy)  ,1,file);
	fread(nameTable,sizeof(nameTable),1,file);
	fread(attrTable,sizeof(attrTable),1,file);
	fread(nameCopy ,sizeof(nameCopy) ,1,file);
	fread(attrCopy ,sizeof(attrCopy) ,1,file);
	fread(undoChr  ,sizeof(undoChr)  ,1,file);
	fread(undoNameTable,sizeof(undoNameTable),1,file);
	fread(undoAttrTable,sizeof(undoAttrTable),1,file);

	//palette

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			bgPal[palBank*16+i*4+j]=fget_i32(file);
		}
	}

	//screen buttons state

	SpeedButtonTiles    ->Down=fget_bool(file);
	SpeedButtonChecker  ->Down=fget_bool(file);
	SpeedButtonChrBank1 ->Down=fget_bool(file);
	SpeedButtonChrBank2 ->Down=fget_bool(file);
	SpeedButtonGridAll  ->Down=fget_bool(file);
	SpeedButtonGridTile ->Down=fget_bool(file);
	SpeedButtonGridAtr  ->Down=fget_bool(file);
	SpeedButtonGridBlock->Down=fget_bool(file);
	SpeedButtonPal      ->Down=fget_bool(file);

	//variables

	bgPalCur  =fget_i32(file);
	palActive =fget_i32(file);
	tileActive=fget_i32(file);
	bankActive=fget_i32(file);
	ppuMask   =fget_i32(file);

	nameSelection.Left  =fget_i32(file);
	nameSelection.Right =fget_i32(file);
	nameSelection.Top   =fget_i32(file);
	nameSelection.Bottom=fget_i32(file);
	nameCopyWidth       =fget_i32(file);
	nameCopyHeight      =fget_i32(file);

	chrSelection.Left  =fget_i32(file);
	chrSelection.Right =fget_i32(file);
	chrSelection.Top   =fget_i32(file);
	chrSelection.Bottom=fget_i32(file);
	chrCopyWidth       =fget_i32(file);
	chrCopyHeight      =fget_i32(file);
	chrCopyRect        =fget_bool(file);
	chrSelectRect      =fget_bool(file);

	for(i=0;i<256;i++) chrSelected[i]=fget_bool(file);


	//save dialogs settings

	SaveDialogChr ->FilterIndex=fget_i32(file);
	SaveDialogName->FilterIndex=fget_i32(file);

	SaveDialogChr ->FileName=fget_ansistring(file);
	SaveDialogName->FileName=fget_ansistring(file);
	SaveDialogPal ->FileName=fget_ansistring(file);

	fclose(file);

	return true;
}



bool __fastcall TFormMain::LoadSession2x(AnsiString filename)
{
	FILE *file;
	unsigned char temp[16];
	int i,j;

	file=fopen(filename.c_str(),"rb");

	if(!file) return false;

	//signature

	fread(temp,sizeof(sessionIDStr),1,file);

	//arrays

	fread(chr      ,sizeof(chr)      ,1,file);
	fread(chrCopy  ,sizeof(chrCopy)  ,1,file);
	fread(nameTable,sizeof(nameTable),1,file);
	fread(attrTable,sizeof(attrTable),1,file);
	fread(nameCopy ,sizeof(nameCopy) ,1,file);
	fread(attrCopy ,sizeof(attrCopy) ,1,file);
	fread(undoChr  ,sizeof(undoChr)  ,1,file);
	fread(undoNameTable,sizeof(undoNameTable),1,file);
	fread(undoAttrTable,sizeof(undoAttrTable),1,file);
	fread(metaSprites  ,sizeof(metaSprites)  ,1,file);

	//palette

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			bgPal[palBank*16+i*4+j]=fget_i32(file);
		}
	}

	//screen buttons state

	SpeedButtonTiles    ->Down=fget_bool(file);
	SpeedButtonChecker  ->Down=fget_bool(file);
	SpeedButtonSelTiles ->Down=fget_bool(file);
	SpeedButtonChrBank1 ->Down=fget_bool(file);
	SpeedButtonChrBank2 ->Down=fget_bool(file);
	SpeedButtonGridAll  ->Down=fget_bool(file);
	SpeedButtonGridTile ->Down=fget_bool(file);
	SpeedButtonGridAtr  ->Down=fget_bool(file);
	SpeedButtonGridBlock->Down=fget_bool(file);
	SpeedButtonPal      ->Down=fget_bool(file);
	SpeedButtonTypeIn   ->Down=fget_bool(file);
	SpeedButtonFrameAll ->Down=fget_bool(file);
	SpeedButtonFrameSelected->Down=fget_bool(file);
	SpeedButtonFrameNone    ->Down=fget_bool(file);
	SpeedButtonSpriteSnap   ->Down=fget_bool(file);
	SpeedButtonSprite8x16   ->Down=fget_bool(file);

	SpeedButtonSpriteBank->Enabled=SpeedButtonSprite8x16->Down;

	//variables

	bgPalCur  =fget_i32(file);
	palActive =fget_i32(file);
	tileActive=fget_i32(file);
	bankActive=fget_i32(file);
	ppuMask   =fget_i32(file);

	metaSpriteActive=fget_i32(file);
	spriteActive=fget_i32(file);
	spriteGridX =fget_i32(file);
	spriteGridY =fget_i32(file);

	nameSelection.Left  =fget_i32(file);
	nameSelection.Right =fget_i32(file);
	nameSelection.Top   =fget_i32(file);
	nameSelection.Bottom=fget_i32(file);
	nameCopyWidth       =fget_i32(file);
	nameCopyHeight      =fget_i32(file);

	chrSelection.Left  =fget_i32(file);
	chrSelection.Right =fget_i32(file);
	chrSelection.Top   =fget_i32(file);
	chrSelection.Bottom=fget_i32(file);
	chrCopyWidth       =fget_i32(file);
	chrCopyHeight      =fget_i32(file);
	chrCopyRect        =fget_bool(file);
	chrSelectRect      =fget_bool(file);

	for(i=0;i<256;i++) chrSelected[i]=fget_bool(file);

	//save dialogs settings

	SaveDialogChr ->FilterIndex=fget_i32(file);
	SaveDialogName->FilterIndex=fget_i32(file);

	SaveDialogChr ->FileName=fget_ansistring(file);
	SaveDialogName->FileName=fget_ansistring(file);
	SaveDialogPal ->FileName=fget_ansistring(file);
	SaveDialogMetaSpriteBank->FileName=fget_ansistring(file);

	fclose(file);

	return true;
}



bool __fastcall TFormMain::LoadSessionText(AnsiString filename)
{
	FILE *file;
	char *text;
	int i,j,size;

	file=fopen(filename.c_str(),"rb");

	if(!file) return false;

	fseek(file,0,SEEK_END);
	size=ftell(file);
	fseek(file,0,SEEK_SET);

	text=(char*)malloc(size+1);
	text[size]=0;

	fread(text,size,1,file);
	fclose(file);

	//palette

	nss_get_bytes(text,size,"Palette=",bgPal,sizeof(bgPal));

	//screen buttons state

	SpeedButtonTiles    ->Down=nss_get_bool(text,size,"BtnTiles=");
	SpeedButtonChecker  ->Down=nss_get_bool(text,size,"BtnChecker=");
	SpeedButtonSelTiles ->Down=nss_get_bool(text,size,"BtnSelTiles=");
	SpeedButtonChrBank1 ->Down=nss_get_bool(text,size,"BtnChrBank1=");
	SpeedButtonChrBank2 ->Down=nss_get_bool(text,size,"BtnChrBank2=");
	SpeedButtonGridAll  ->Down=nss_get_bool(text,size,"BtnGridAll=");
	SpeedButtonGridTile ->Down=nss_get_bool(text,size,"BtnGridTile=");
	SpeedButtonGridAtr  ->Down=nss_get_bool(text,size,"BtnGridAtr=");
	SpeedButtonGridBlock->Down=nss_get_bool(text,size,"BtnGridBlock=");
	SpeedButtonGridScreen->Down=nss_get_bool(text,size,"BtnGridScr=");

	SpeedButtonPal      ->Down=nss_get_bool(text,size,"BtnPal=");
	SpeedButtonTypeIn   ->Down=nss_get_bool(text,size,"BtnTypeIn=");
	SpeedButtonFrameAll ->Down=nss_get_bool(text,size,"BtnFrameAll=");
	SpeedButtonFrameSelected->Down=nss_get_bool(text,size,"BtnFrameSelected=");
	SpeedButtonFrameNone    ->Down=nss_get_bool(text,size,"BtnFrameNone=");
	SpeedButtonSpriteSnap   ->Down=nss_get_bool(text,size,"BtnSpriteSnap=");
	SpeedButtonSprite8x16   ->Down=nss_get_bool(text,size,"BtnSprite8x16=");
	ButtonForceView   ->Down=nss_get_bool(text,size,"BtnForce8x16=");

	MImportBestOffsets               ->Checked=nss_get_bool(text,size,"MenuBestOffsets=");
	MImportLossy                     ->Checked=nss_get_bool(text,size,"MenuLossy=");
	MImportThreshold                 ->Checked=nss_get_bool(text,size,"MenuThreshold=");
	MImportNoColorData               ->Checked=nss_get_bool(text,size,"MenuNoColorData=");
	MPutSelectionToMetaspriteAutoInc ->Checked=nss_get_bool(text,size,"MenuMetaSprAutoInc=");
	MPutSelectionToMetaspriteSkipZero->Checked=nss_get_bool(text,size,"MenuMetaSprSkipZero=");
	MPutSelectionToMetaspriteMerge   ->Checked=nss_get_bool(text,size,"MenuMetaSprMerge=");
	MSaveIncName                     ->Checked=nss_get_bool(text,size,"MenuSaveIncName=");
	MSaveIncAttr                     ->Checked=nss_get_bool(text,size,"MenuSaveIncAttr=");
	MSaveRLE                         ->Checked=nss_get_bool(text,size,"MenuSaveRLE=");

	SpeedButtonSpriteBank->Enabled=SpeedButtonSprite8x16->Down;

	//variables

	bgPalCur  =nss_get_int(text,size,"VarBgPalCur=");
	palActive =nss_get_int(text,size,"VarPalActive=");
	tileActive=nss_get_int(text,size,"VarTileActive=");
	bankActive=nss_get_int(text,size,"VarBankActive=");
	ppuMask   =nss_get_int(text,size,"VarPPUMask=");

	//conditional initialization to useful presets
	if(bgPalCur==3) bgPalCurOld=0; else bgPalCurOld=3;
	if(palActive==1) palActiveOld=0;  else palActiveOld=1;

	tileViewTagBuf  =nss_get_int(text,size,"VarTileViewTagBuf=");

	ppuMaskSet[0]   =nss_get_int(text,size,"VarPPUMaskSet0=");
	ppuMaskSet[1]   =nss_get_int(text,size,"VarPPUMaskSet1=");
	ppuMaskSet[2]   =nss_get_int(text,size,"VarPPUMaskSet2=");
	ppuMaskSet[3]   =nss_get_int(text,size,"VarPPUMaskSet3=");
	
	palBank   =nss_get_int(text,size,"VarPalBank=");

	metaSpriteActive=nss_get_int(text,size,"VarMetaSpriteActive=");
	spriteActive=nss_get_int(text,size,"VarSpriteActive=");
	spriteGridX =nss_get_int(text,size,"VarSpriteGridX=");
	spriteGridY =nss_get_int(text,size,"VarSpriteGridY=");

	nameTableWidth      =nss_get_int(text,size,"VarNameW=");
	nameTableHeight     =nss_get_int(text,size,"VarNameH=");
	nameTableViewX      =nss_get_int(text,size,"VarNameViewX=");
	nameTableViewY      =nss_get_int(text,size,"VarNameViewY=");
	nameSelection.Left  =nss_get_int(text,size,"VarNameSelectionL=");
	nameSelection.Right =nss_get_int(text,size,"VarNameSelectionR=");
	nameSelection.Top   =nss_get_int(text,size,"VarNameSelectionT=");
	nameSelection.Bottom=nss_get_int(text,size,"VarNameSelectionB=");
	nameCopyWidth       =nss_get_int(text,size,"VarNameCopyW=");
	nameCopyHeight      =nss_get_int(text,size,"VarNameCopyH=");

	chrSelection.Left  =nss_get_int(text,size,"VarCHRSelectionL=");
	chrSelection.Right =nss_get_int(text,size,"VarCHRSelectionR=");
	chrSelection.Top   =nss_get_int(text,size,"VarCHRSelectionT=");
	chrSelection.Bottom=nss_get_int(text,size,"VarCHRSelectionB=");
	chrCopyWidth       =nss_get_int(text,size,"VarCHRCopyW=");
	chrCopyHeight      =nss_get_int(text,size,"VarCHRCopyH=");
	chrCopyRect        =nss_get_bool(text,size,"VarCHRCopyRect=");
	chrSelectRect      =nss_get_bool(text,size,"VarCHRSelectRect=");

	//arrays
	//nss_get_bytes(text,size,"ppuMaskSet=",ppuMaskSet,sizeof(ppuMaskSet));
	nss_get_bytes(text,size,"VarCHRSelected=",chrSelected,sizeof(chrSelected));

	//working pal is loaded before everything else. 
	nss_get_bytes(text,size,"PalUndo="    ,bgPalUndo    ,sizeof(bgPalUndo));

	nss_get_bytes(text,size,"CHRMain="    ,chr          ,sizeof(chr));
	nss_get_bytes(text,size,"CHRCopy="    ,chrCopy      ,sizeof(chrCopy));
	nss_get_bytes(text,size,"CHRUndo="    ,undoChr      ,sizeof(undoChr));

	nss_get_bytes(text,size,"NameTable="  ,nameTable    ,name_size());
	nss_get_bytes(text,size,"NameCopy="   ,nameCopy     ,name_size());
	nss_get_bytes(text,size,"NameUndo="   ,undoNameTable,name_size());

	nss_get_bytes(text,size,"AttrTable="  ,attrTable    ,attr_size());
	nss_get_bytes(text,size,"AttrCopy="   ,attrCopy     ,attr_size());
	nss_get_bytes(text,size,"AttrUndo="   ,undoAttrTable,attr_size());

	nss_get_bytes(text,size,"MetaSprites=",metaSprites,sizeof(metaSprites));

	nss_get_bytes(text,size,"TilePropConditionType=",propConditional,sizeof(propConditional));
	nss_get_bytes(text,size,"TilePropConditionTypeUndo=",propConditionalUndo,sizeof(propConditional));

	if(CheckTag(text,size, "32_TileCollisionProperties="))
		nss_get_uint32s(text,size,"32_TileCollisionProperties=",tileProperties,sizeof(tileProperties));
	else memset(tileProperties,0,sizeof(tileProperties));

	if(CheckTag(text,size, "32_TileCollisionPropertiesUndo="))
		nss_get_uint32s(text,size,"32_TileCollisionPropertiesUndo=",tilePropertiesUndo,sizeof(tileProperties));
	else memset(tilePropertiesUndo,0,sizeof(tileProperties));

	//bool* tmp_propCondition = reinterpret_cast<bool*>(propCondition);
	//bool* tmp_propConditionUndo = reinterpret_cast<bool*>(propConditionUndo);
	nss_get_2D_bools(text,size,"PropertyConditions=",propCondition,4,8);
	nss_get_2D_bools(text,size,"PropertyConditionsUndo=",propConditionUndo,4,8);

	//Checkpoint stuff
	nss_get_bytes(text,size,"Checkpoint_Palette="  ,bgPalCheckpoint 		,sizeof(bgPal));
	nss_get_bytes(text,size,"Checkpoint_PalUndo="  ,bgPalUndoCheckPoint  ,sizeof(bgPalUndo));

	nss_get_bytes(text,size,"Checkpoint_CHRMain="  ,checkpointChr          ,sizeof(chr));
	nss_get_bytes(text,size,"Checkpoint_CHRUndo="  ,undoCheckpointChr      ,sizeof(undoChr));

	nss_get_bytes(text,size,"Checkpoint_NameTable=",checkpointNameTable    ,name_size());
	nss_get_bytes(text,size,"Checkpoint_NameUndo=" ,undoCheckpointNameTable,name_size());

	nss_get_bytes(text,size,"Checkpoint_AttrTable=",checkpointAttrTable    ,attr_size());
	nss_get_bytes(text,size,"Checkpoint_AttrUndo=" ,undoCheckpointAttrTable,attr_size());

	nss_get_bytes(text,size,"Checkpoint_MetaSprites=",checkpointMetaSprites,sizeof(metaSprites));



	if(CheckTag(text,size, "Checkpoint_32_TileCollisionProperties="))
		nss_get_uint32s(text,size,"Checkpoint_32_TileCollisionProperties=",checkpointTileProperties,sizeof(tileProperties));
	else  memset(checkpointTileProperties,0,sizeof(checkpointTileProperties));

	if(CheckTag(text,size, "Checkpoint_32_TileCollisionPropertiesUndo="))
		nss_get_uint32s(text,size,"Checkpoint_32_TileCollisionPropertiesUndo=",checkpointTilePropertiesUndo,sizeof(tileProperties));
	else  memset(checkpointTilePropertiesUndo,0,sizeof(checkpointTileProperties));




	nss_get_2D_bools(text,size,"Checkpoint_PropertyConditions=",checkpointPropCondition,4,8);
	nss_get_2D_bools(text,size,"Checkpoint_PropertyConditionsUndo=",checkpointPropConditionUndo,4,8);

	nss_get_bytes(text,size,"Checkpoint_TilePropConditionType=",checkpointPropConditional,sizeof(propConditional));
	nss_get_bytes(text,size,"Checkpoint_TilePropConditionTypeUndo=",checkpointPropConditionalUndo,sizeof(propConditional));


	//chr bank stuff
	chrBanks					=nss_get_int(text,size,"VarCHRBanks=");
	undo_chrBanks               =nss_get_int(text,size,"VarCHRBanksUndo=");
	checkpoint_chrBanks         =nss_get_int(text,size,"VarCheckpoint_CHRBanks=");
	undo_checkpoint_chrBanks    =nss_get_int(text,size,"VarCheckpoint_CHRBanksUndo=");

	bool bGenerateCHRBanks = ((chrBanks==0)
		||(undo_chrBanks==0)
		||(checkpoint_chrBanks==0)
		||(undo_checkpoint_chrBanks==0)
		||(!CheckTag(text,size, "CHRBank="))   //extra check since there are "in-between" version files in the wild
		||(!CheckTag(text,size, "CHR_A_ID="))  //another extra check.
		)? true:false;


	if(chrBanks==0)
		chrBanks=4;
	if(undo_chrBanks==0)
		undo_chrBanks=4;
	if(checkpoint_chrBanks==0)
		checkpoint_chrBanks=4;
	if(undo_checkpoint_chrBanks==0)
		undo_checkpoint_chrBanks=4;

	size_t tmpSize;
	tmpSize = sizeof(chrBanks*4096);
	if (tmpSize > sizeof(chrBank)) chrBank = (char*) realloc(chrBank, tmpSize);
	tmpSize = sizeof(undo_chrBanks*4096);
	if (tmpSize > sizeof(undo_chrBank)) undo_chrBank = (char*) realloc(undo_chrBank, tmpSize);
	tmpSize = sizeof(checkpoint_chrBanks*4096);
	if (tmpSize > sizeof(checkpoint_chrBank)) checkpoint_chrBank = (char*) realloc(checkpoint_chrBank, tmpSize);
	tmpSize = sizeof(undo_checkpoint_chrBanks*4096);
	if (tmpSize > sizeof(undo_checkpoint_chrBank)) undo_checkpoint_chrBank = (char*) realloc(undo_checkpoint_chrBank, tmpSize);


	if(bGenerateCHRBanks){    //make a generic setup
		memcpy(chrBank,chr,4096*2);
		memcpy(undo_chrBank,undoChr,4096*2);
		memcpy(checkpoint_chrBank,checkpointChr,4096*2);
		memcpy(undo_checkpoint_chrBank,undoCheckpointChr,4096*2);


		for(i = 0; i < 16; i++) {
			chrA_id[i] = i;
			chrB_id[i] = i + 16;
			undo_chrA_id[i] = i;
			undo_chrB_id[i] = i + 16;
			checkpoint_chrA_id[i] = i;
			checkpoint_chrB_id[i] = i + 16;
			undo_checkpoint_chrA_id[i] = i;
			undo_checkpoint_chrB_id[i] = i + 16;
		}


		chrBankLabels.clear();
		undo_chrBankLabels.clear();
		checkpoint_chrBankLabels.clear();
		undo_checkpoint_chrBankLabels.clear();


		for(unsigned int k=0;k<chrBanks;k++){chrBankLabels.push_back("Unlabeled");}
		for(unsigned int k=0;k<undo_chrBanks;k++){undo_chrBankLabels.push_back("Unlabeled");}
		for(unsigned int k=0;k<checkpoint_chrBanks;k++){checkpoint_chrBankLabels.push_back("Unlabeled");}
		for(unsigned int k=0;k<undo_checkpoint_chrBanks;k++){undo_checkpoint_chrBankLabels.push_back("Unlabeled");}
	}
                                 


	else{
		nss_get_bytes(text,size,"CHRBank=",chrBank,sizeof(chrBank));
		nss_get_bytes(text,size,"CHRBankUndo=",undo_chrBank,sizeof(undo_chrBank));
		nss_get_bytes(text,size,"Checkpoint_CHRBank=",checkpoint_chrBank,sizeof(checkpoint_chrBank));
		nss_get_bytes(text,size,"Checkpoint_CHRBankUndo=",undo_checkpoint_chrBank,sizeof(undo_checkpoint_chrBank));

		nss_get_uint32s(text,size,"CHR_A_ID=",chrA_id,sizeof(chrA_id));
		nss_get_uint32s(text,size,"CHR_B_ID=",chrB_id,sizeof(chrA_id));
		nss_get_uint32s(text,size,"CHR_A_IDUndo=",undo_chrA_id,sizeof(chrA_id));
		nss_get_uint32s(text,size,"CHR_A_IDUndo=",undo_chrB_id,sizeof(chrA_id));

		nss_get_uint32s(text,size,"Checkpoint_CHR_A_ID=",checkpoint_chrA_id,sizeof(chrA_id));
		nss_get_uint32s(text,size,"Checkpoint_CHR_B_ID=",checkpoint_chrB_id,sizeof(chrA_id));
		nss_get_uint32s(text,size,"Checkpoint_CHR_A_IDUndo=",undo_checkpoint_chrA_id,sizeof(chrA_id));
		nss_get_uint32s(text,size,"Checkpoint_CHR_B_IDUndo=",undo_checkpoint_chrB_id,sizeof(chrA_id));

        WorkCHRToBankCHR();


		//make extra sure these tags exist, getting the vectors right is crucial to avoid errors.

		if(CheckTag(text,size, "CHRBankLabels=")) chrBankLabels = deserializeVector(text, "CHRBankLabels=");
		else {
			chrBankLabels.clear();
			for(unsigned int k=0;k<chrBanks;k++){chrBankLabels.push_back("Unlabeled");}
		}
		if(CheckTag(text,size, "CHRBankLabelsUndo=")) undo_chrBankLabels = deserializeVector(text, "CHRBankLabelsUndo=");
		else{
			for(unsigned int k=0;k<undo_chrBanks;k++){undo_chrBankLabels.push_back("Unlabeled");}
			undo_chrBankLabels.clear();
		}

		if(CheckTag(text,size, "Checkpoint_CHRBankLabels=")) checkpoint_chrBankLabels = deserializeVector(text, "Checkpoint_CHRBankLabels=");
		else{
			for(unsigned int k=0;k<checkpoint_chrBanks;k++){checkpoint_chrBankLabels.push_back("Unlabeled");}
			checkpoint_chrBankLabels.clear();
		}

		if(CheckTag(text,size, "Checkpoint_CHRBankLabelsUndo=")) undo_checkpoint_chrBankLabels = deserializeVector(text, "Checkpoint_CHRBankLabelsUndo=");
		else{
            nss_get_bytes(text,size,"Checkpoint_CHRBankUndo=",undo_checkpoint_chrBank,sizeof(undo_checkpoint_chrBank));
			undo_checkpoint_chrBankLabels.clear();
		}
	}
	tmp_chrBankLabels.clear();


	//get collision labels
	for (i = 0; i <8; i++) {
		char label[20];
		sprintf(label, "propertyCHRlabel%i=", i);
		propCHRlabel[i] = nss_get_str(text,size,label);
		if(propCHRlabel[i]=="")
			{
				propCHRlabel[i]="Label"+IntToStr(i);
			}
	}


	//BROKE STUDIO
	for(i=0;i<256;i++)
	{
		if(metaSprites[i*64*4]<255)
		{
			char label[14];
			sprintf(label, "MetaSprite%i=", i);
			metaSpriteNames[i] = nss_get_str(text,size,label);
			if(metaSpriteNames[i]=="")
			{
				metaSpriteNames[i]="Metasprite"+IntToStr(i);
			}
        }
	}
	metaSpriteBankName=nss_get_str(text,size,"MetaSpriteBankName=");
	if(metaSpriteBankName=="")
	{
        metaSpriteBankName="spr";
    }
	//

	//save dialogs settings

	SaveDialogChr ->FilterIndex=nss_get_int(text,size,"FilterCHR=");
	SaveDialogName->FilterIndex=nss_get_int(text,size,"FilterName=");

	SaveDialogChr ->FileName=nss_get_str(text,size,"FileNameCHR=");
	SaveDialogName->FileName=nss_get_str(text,size,"FileNameName=");
	SaveDialogPal ->FileName=nss_get_str(text,size,"FileNamePal=");
	SaveDialogMetaSpriteBank->FileName=nss_get_str(text,size,"FileNameMetaSpriteBank=");
	SaveDialogTprop->FileName=nss_get_str(text,size,"FileNameTileProps=");

	//final preparations
	SpeedButtonPalBankB->Down=false; MPalB->Checked=false;
	SpeedButtonPalBankC->Down=false; MPalC->Checked=false;
	SpeedButtonPalBankD->Down=false; MPalD->Checked=false;
	SpeedButtonPalBankA->Down=false; MPalA->Checked=false;

	if(palBank==1) 		{SpeedButtonPalBankB->Down=true; MPalB->Checked=true;}
	else if(palBank==2) {SpeedButtonPalBankC->Down=true; MPalC->Checked=true;}
	else if(palBank==3) {SpeedButtonPalBankD->Down=true; MPalD->Checked=true;}
	else 		   		{SpeedButtonPalBankA->Down=true; MPalA->Checked=true;}
	if (ppuMask!=ppuMaskSet[palBank])
    //if they differ, this means we've loaded an old nss. NEXXT proceeds to clone the entry to reconstruct the behaviour from old versions.
	{
		for (int i = 0; i <= 3; i++) ppuMaskSet[i]=ppuMask;
	}
	ppuMask=ppuMaskSet[palBank];

	return true;
}



bool __fastcall TFormMain::LoadSession(AnsiString filename)
{
	FILE *file;
	unsigned char temp[16];
	int i,j,ver;
	bool r;

	file=fopen(filename.c_str(),"rb");

	if(!file) return false;

	fread(temp,sizeof(sessionIDStr),1,file);
	fclose(file);

	ver=0;

	if(!memcmp(temp,"NESSTses",8)) ver=1;//load legacy binary format
	if(!memcmp(temp,"NSTses00",8)) ver=2;//load old binary format
	if(!memcmp(temp,sessionIDStr,8)) ver=3;//current text format

	switch(ver)
	{
	case 1: r=LoadSession1x(filename); break;
	case 2: r=LoadSession2x(filename); break;
	case 3: r=LoadSessionText(filename); break;
	}

	if(r)
	{
		SaveDialogSession->FileName=filename;

		nameXC=-1;
		nameYC=-1;
		tileXC=-1;
		tileYC=-1;
		palHover=-1;
		palColHover=-1;
		colHover=-1;

		UpdateRGBM();
		palette_calc();
		DrawPalettes();
		UpdateTiles(true);
		UpdateNameTable(-1,-1,true);
		//if (FormNavigator->Visible) FormNavigator->Draw(true,true);

		return true;
	}

	Application->MessageBox("Unknown or corrupted session data format","Error",MB_OK);

	return false;
}



void __fastcall TFormMain::SaveSession(AnsiString filename)
{
	FILE *file;
	int i,j;
	unsigned char pal[16];

	file=fopen(filename.c_str(),"wt");

	if(!file)
	{
		Application->MessageBox("Can't save session","Error",MB_OK);
		return;
	}

	//signature

	fprintf(file,"%s\n\n",sessionIDStr);

	//screen buttons state

	fprintf(file,"BtnTiles=%i\n"        ,SpeedButtonTiles        ->Down?1:0);
	fprintf(file,"BtnChecker=%i\n"      ,SpeedButtonChecker      ->Down?1:0);
	fprintf(file,"BtnSelTiles=%i\n"     ,SpeedButtonSelTiles     ->Down?1:0);
	fprintf(file,"BtnChrBank1=%i\n"     ,SpeedButtonChrBank1     ->Down?1:0);
	fprintf(file,"BtnChrBank2=%i\n"     ,SpeedButtonChrBank2     ->Down?1:0);
	fprintf(file,"BtnGridAll=%i\n"      ,SpeedButtonGridAll      ->Down?1:0);
	fprintf(file,"BtnGridTile=%i\n"     ,SpeedButtonGridTile     ->Down?1:0);
	fprintf(file,"BtnGridAtr=%i\n"      ,SpeedButtonGridAtr      ->Down?1:0);
	fprintf(file,"BtnGridBlock=%i\n"    ,SpeedButtonGridBlock    ->Down?1:0);

	fprintf(file,"BtnGridScr=%i\n"    ,SpeedButtonGridScreen    ->Down?1:0);


	fprintf(file,"BtnPal=%i\n"          ,SpeedButtonPal          ->Down?1:0);
	fprintf(file,"BtnTypeIn=%i\n"       ,SpeedButtonTypeIn       ->Down?1:0);
	fprintf(file,"BtnFrameAll=%i\n"     ,SpeedButtonFrameAll     ->Down?1:0);
	fprintf(file,"BtnFrameSelected=%i\n",SpeedButtonFrameSelected->Down?1:0);
	fprintf(file,"BtnFrameNone=%i\n"    ,SpeedButtonFrameNone    ->Down?1:0);
	fprintf(file,"BtnSpriteSnap=%i\n"   ,SpeedButtonSpriteSnap   ->Down?1:0);
	fprintf(file,"BtnSprite8x16=%i\n"   ,SpeedButtonSprite8x16   ->Down?1:0);
	fprintf(file,"BtnForce8x16=%i\n"	,ButtonForceView         ->Down?1:0);

	fprintf(file,"MenuBestOffsets=%i\n"    ,MImportBestOffsets->Checked?1:0);
	fprintf(file,"MenuLossy=%i\n"          ,MImportLossy->Checked?1:0);
	fprintf(file,"MenuThreshold=%i\n"      ,MImportThreshold->Checked?1:0);
	fprintf(file,"MenuNoColorData=%i\n"    ,MImportNoColorData->Checked?1:0);
	fprintf(file,"MenuMetaSprAutoInc=%i\n" ,MPutSelectionToMetaspriteAutoInc->Checked?1:0);
	fprintf(file,"MenuMetaSprSkipZero=%i\n",MPutSelectionToMetaspriteSkipZero->Checked?1:0);
	fprintf(file,"MenuMetaSprMerge=%i\n"   ,MPutSelectionToMetaspriteMerge->Checked?1:0);
	fprintf(file,"MenuSaveIncName=%i\n"    ,MSaveIncName->Checked?1:0);
	fprintf(file,"MenuSaveIncAttr=%i\n"    ,MSaveIncAttr->Checked?1:0);
	fprintf(file,"MenuSaveRLE=%i\n"        ,MSaveRLE->Checked?1:0);

	//variables
	fprintf(file,"VarTileViewTagBuf=%i\n"   ,tileViewTagBuf);

	fprintf(file,"VarBgPalCur=%i\n"  ,bgPalCur);
	fprintf(file,"VarPalActive=%i\n" ,palActive);
	fprintf(file,"VarTileActive=%i\n",tileActive);
	fprintf(file,"VarBankActive=%i\n",bankActive);
	fprintf(file,"VarPPUMask=%i\n"   ,ppuMask);
	
	fprintf(file,"VarPPUMaskSet0=%i\n"   ,ppuMaskSet[0]);
	fprintf(file,"VarPPUMaskSet1=%i\n"   ,ppuMaskSet[1]);
	fprintf(file,"VarPPUMaskSet2=%i\n"   ,ppuMaskSet[2]);
	fprintf(file,"VarPPUMaskSet3=%i\n"   ,ppuMaskSet[3]);

	fprintf(file,"VarPalBank=%i\n"   ,palBank);

	fprintf(file,"VarMetaSpriteActive=%i\n",metaSpriteActive);
	fprintf(file,"VarSpriteActive=%i\n"    ,spriteActive);
	fprintf(file,"VarSpriteGridX=%i\n"     ,spriteGridX);
	fprintf(file,"VarSpriteGridY=%i\n"     ,spriteGridY);

	fprintf(file,"VarNameW=%i\n"         ,nameTableWidth);
	fprintf(file,"VarNameH=%i\n"         ,nameTableHeight);
	fprintf(file,"VarNameViewX=%i\n"     ,nameTableViewX);
	fprintf(file,"VarNameViewY=%i\n"     ,nameTableViewY);
	fprintf(file,"VarNameSelectionL=%i\n",nameSelection.Left);
	fprintf(file,"VarNameSelectionR=%i\n",nameSelection.Right);
	fprintf(file,"VarNameSelectionT=%i\n",nameSelection.Top);
	fprintf(file,"VarNameSelectionB=%i\n",nameSelection.Bottom);
	fprintf(file,"VarNameCopyW=%i\n"     ,nameCopyWidth);
	fprintf(file,"VarNameCopyH=%i\n"     ,nameCopyHeight);

	fprintf(file,"VarCHRSelectionL=%i\n",chrSelection.Left);
	fprintf(file,"VarCHRSelectionR=%i\n",chrSelection.Right);
	fprintf(file,"VarCHRSelectionT=%i\n",chrSelection.Top);
	fprintf(file,"VarCHRSelectionB=%i\n",chrSelection.Bottom);
	fprintf(file,"VarCHRCopyW=%i\n"     ,chrCopyWidth);
	fprintf(file,"VarCHRCopyH=%i\n"     ,chrCopyHeight);
	fprintf(file,"VarCHRCopyRect=%i\n"  ,chrCopyRect?1:0);
	fprintf(file,"VarCHRSelectRect=%i\n",chrSelectRect?1:0);

	fprintf(file,"VarCHRBanks=%i\n"     			,chrBanks);
	fprintf(file,"VarCHRBanksUndo=%i\n"     		,undo_chrBanks);
	fprintf(file,"VarCheckpoint_CHRBanks=%i\n"     	,checkpoint_chrBanks);
	fprintf(file,"VarCheckpoint_CHRBanksUndo=%i\n"  ,undo_checkpoint_chrBanks);


	nss_put_bytes(file,"\n\nVarCHRSelected=",chrSelected,sizeof(chrSelected));
	//arrays

	nss_put_bytes(file,"\n\nPalette="  ,bgPal        ,sizeof(bgPal));
	nss_put_bytes(file,"\n\nPalUndo="  ,bgPalUndo    ,sizeof(bgPalUndo));

	nss_put_bytes(file,"\n\nCHRMain="  ,chr          ,sizeof(chr));
	nss_put_bytes(file,"\n\nCHRCopy="  ,chrCopy      ,sizeof(chrCopy));
	nss_put_bytes(file,"\n\nCHRUndo="  ,undoChr      ,sizeof(undoChr));

	nss_put_bytes(file,"\n\nNameTable=",nameTable    ,name_size());
	nss_put_bytes(file,"\n\nNameCopy=" ,nameCopy     ,name_size());
	nss_put_bytes(file,"\n\nNameUndo=" ,undoNameTable,name_size());

	nss_put_bytes(file,"\n\nAttrTable=",attrTable    ,attr_size());
	nss_put_bytes(file,"\n\nAttrCopy=" ,attrCopy     ,attr_size());
	nss_put_bytes(file,"\n\nAttrUndo=" ,undoAttrTable,attr_size());

	nss_put_bytes(file,"\n\nMetaSprites=",metaSprites,sizeof(metaSprites));

	nss_put_uint32s(file,"\n\n32_TileCollisionProperties=",tileProperties,sizeof(tileProperties));
	nss_put_uint32s(file,"\n\n32_TileCollisionPropertiesUndo=",tilePropertiesUndo,sizeof(tilePropertiesUndo));

	//bool* tmp_propCondition = reinterpret_cast<bool*>(propCondition);
	//bool* tmp_propConditionUndo = reinterpret_cast<bool*>(propConditionUndo);
	nss_put_2D_bools(file,"\n\nPropertyConditions=",propCondition,4,8);
	nss_put_2D_bools(file,"\n\nPropertyConditionsUndo=",propConditionUndo,4,8);

	nss_put_bytes(file,"\n\nTilePropConditionType=",propConditional,sizeof(propConditional));
	nss_put_bytes(file,"\n\nTilePropConditionTypeUndo=",propConditionalUndo,sizeof(propConditional));

	/* //uncomment this when CHR bank editor is ready
	nss_put_bytes(file,"\n\nCHRBank="  		,chrBank        ,chrBanks*4096);
	nss_put_bytes(file,"\n\nCHRBankUndo="   ,undo_chrBank  	,undo_chrBanks*4096);

	nss_put_uint32s(file,"\n\nCHR_A_ID="  ,chrA_id  	,sizeof(chrA_id));
	nss_put_uint32s(file,"\n\nCHR_B_ID="  ,chrB_id  	,sizeof(chrB_id));
	nss_put_uint32s(file,"\n\nCHR_A_IDUndo="  ,undo_chrA_id  	,sizeof(undo_chrA_id));
	nss_put_uint32s(file,"\n\nCHR_B_IDUndo="  ,undo_chrB_id  	,sizeof(undo_chrB_id));


	std::string serializedVector = "";
	for (std::vector<std::string>::const_iterator iter = chrBankLabels.begin(); iter != chrBankLabels.end(); ++iter) {
		serializedVector += *iter;
		if (iter + 1 != chrBankLabels.end()) {
			serializedVector += ",";
		}
	}
	fprintf(file,"\n\nCHRBankLabels=%s",serializedVector.c_str());

	serializedVector.clear();
	for (std::vector<std::string>::const_iterator iter = undo_chrBankLabels.begin(); iter != undo_chrBankLabels.end(); ++iter) {
		serializedVector += *iter;
		if (iter + 1 != undo_chrBankLabels.end()) {
			serializedVector += ",";
		}
	}
	fprintf(file,"\n\nCHRBankLabelsUndo=%s",serializedVector.c_str());

	//remember to also uncomment checkpoint equivalent
    */


	//checkpoint data

	nss_put_bytes(file,"\n\nCheckpoint_Palette="  ,bgPalCheckpoint 		,sizeof(bgPal));
	nss_put_bytes(file,"\n\nCheckpoint_PalUndo="  ,bgPalUndoCheckPoint  ,sizeof(bgPalUndo));

	nss_put_bytes(file,"\n\nCheckpoint_CHRMain="  ,checkpointChr          ,sizeof(chr));
	nss_put_bytes(file,"\n\nCheckpoint_CHRUndo="  ,undoCheckpointChr      ,sizeof(undoChr));

	nss_put_bytes(file,"\n\nCheckpoint_NameTable=",checkpointNameTable    ,name_size());
	nss_put_bytes(file,"\n\nCheckpoint_NameUndo=" ,undoCheckpointNameTable,name_size());

	nss_put_bytes(file,"\n\nCheckpoint_AttrTable=",checkpointAttrTable    ,attr_size());
	nss_put_bytes(file,"\n\nCheckpoint_AttrUndo=" ,undoCheckpointAttrTable,attr_size());

	nss_put_bytes(file,"\n\nCheckpoint_MetaSprites=",checkpointMetaSprites,sizeof(metaSprites));

	nss_put_uint32s(file,"\n\nCheckpoint_32_TileCollisionProperties=",checkpointTileProperties,sizeof(tileProperties));
	nss_put_uint32s(file,"\n\nCheckpoint_32_TileCollisionPropertiesUndo=",checkpointTilePropertiesUndo,sizeof(tilePropertiesUndo));

	nss_put_2D_bools(file,"\n\nCheckpoint_PropertyConditions=",checkpointPropCondition,4,8);
	nss_put_2D_bools(file,"\n\nCheckpoint_PropertyConditionsUndo=",checkpointPropConditionUndo,4,8);

	nss_put_bytes(file,"\n\nCheckpoint_TilePropConditionType=",checkpointPropConditional,sizeof(propConditional));
	nss_put_bytes(file,"\n\nCheckpoint_TilePropConditionTypeUndo=",checkpointPropConditionalUndo,sizeof(propConditional));

	/* //uncomment this when chr bank editor is ready
	nss_put_bytes(file,"\n\nCheckpoint_CHRBank="  ,checkpoint_chrBank        		,checkpoint_chrBanks*4096);
	nss_put_bytes(file,"\n\nCheckpoint_CHRBankUndo="  ,undo_checkpoint_chrBank  	,undo_checkpoint_chrBanks*4096);

	nss_put_uint32s(file,"\n\nCheckpoint_CHR_A_ID="  ,checkpoint_chrA_id  		,sizeof(checkpoint_chrA_id));
	nss_put_uint32s(file,"\n\nCheckpoint_CHR_B_ID="  ,checkpoint_chrB_id  		,sizeof(checkpoint_chrB_id));
	nss_put_uint32s(file,"\n\nCheckpoint_CHR_A_IDUndo="  ,undo_checkpoint_chrA_id ,sizeof(undo_checkpoint_chrA_id));
	nss_put_uint32s(file,"\n\nCheckpoint_CHR_B_IDUndo="  ,undo_checkpoint_chrB_id ,sizeof(undo_checkpoint_chrB_id));

	serializedVector.clear();
	for (std::vector<std::string>::const_iterator iter = checkpoint_chrBankLabels.begin(); iter != checkpoint_chrBankLabels.end(); ++iter) {
		serializedVector += *iter;
		if (iter + 1 != checkpoint_chrBankLabels.end()) {
			serializedVector += ",";
		}
	}
	fprintf(file,"\n\nCheckpoint_CHRBankLabels=%s",serializedVector.c_str());

	serializedVector.clear();
	for (std::vector<std::string>::const_iterator iter = undo_checkpoint_chrBankLabels.begin(); iter != undo_checkpoint_chrBankLabels.end(); ++iter) {
		serializedVector += *iter;
		if (iter + 1 != undo_checkpoint_chrBankLabels.end()) {
			serializedVector += ",";
		}
	}

	fprintf(file,"\n\nCheckpoint_CHRBankLabelsUndo=%s\n",serializedVector.c_str());
    */
	for(i=0;i<8;i++)
	{
		fprintf(file,"propertyCHRlabel%i=%s\n",i,propCHRlabel[i]);
	}
	
	//BROKE STUDIO
	for(i=0;i<256;i++)
	{
		if(metaSprites[i*64*4]<255)
		{
			fprintf(file,"MetaSprite%i=%s\n",i,metaSpriteNames[i]);
		}
	}
	fprintf(file,"MetaSpriteBankName=%s\n",metaSpriteBankName);
	//



	//save dialogs settings

	fprintf(file,"\nFilterCHR=%i\n" ,SaveDialogChr ->FilterIndex);
	fprintf(file,"FilterName=%i\n",SaveDialogName->FilterIndex);

	fprintf(file,"FileNameCHR=%s\n"           ,SaveDialogChr           ->FileName.c_str());
	fprintf(file,"FileNameName=%s\n"          ,SaveDialogName          ->FileName.c_str());
	fprintf(file,"FileNamePal=%s\n"           ,SaveDialogPal           ->FileName.c_str());
	fprintf(file,"FileNameMetaSpriteBank=%s\n",SaveDialogMetaSpriteBank->FileName.c_str());
	fprintf(file,"FileNameTileProps=%s\n"	  ,SaveDialogTprop->FileName.c_str());



	fclose(file);

	unsavedChanges=false;
	Savesession1->Enabled=false;

	UpdateStats();
}
/*
void __fastcall TFormMain::FindFirstDuplicate(int tileID, int &firstDuplicateTile, bool &bDidFind)
{
	int i,j;
	bool dupe;
	bDidFind=false;
	for(i=0;i<256;++i)
	{
	used=false;
	for(j=0;j<nameTableWidth*nameTableHeight;++j)
		{
			if(nameTable[j]==i) {dupe=true; break;}
		}
		if(!dupe) firstDuplicateTile=i; {bDidFind=true; return;}
	}
}
*/

void __fastcall TFormMain::FindFirstUnused(int &firstUnusedTile, bool &bDidFind)
{
	int i,j;
	bool used;
	bDidFind=false;
	for(i=0;i<256;++i)
	{
	used=false;
	for(j=0;j<nameTableWidth*nameTableHeight;++j)
		{
			if(nameTable[j]==i) {used=true; break;}
		}
		if(!used) firstUnusedTile=i; {bDidFind=true; return;}
	}

}
void __fastcall TFormMain::SaveConfig()
{
	BlockDrawing(true);

	FILE *file;

	file=fopen((globalDir+"preferences.cfg").c_str(),"wt");

	if(!file)
	{
		Application->MessageBox("Couldn't save preferences to file","Warning",MB_OK);
		return;
	}

	fprintf(file,"=========================================================\n");
	fprintf(file,"Preferences for %s\n",Application->Title.c_str());
	fprintf(file,"=========================================================\n\n\n");

	fprintf(file,"\nRadio options (for each group, only 1 will be checked):\n");
	fprintf(file,"=========================================================\n");


	fprintf(file,"\nText export Asm Syntax:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,".byte=%i\n"				,byte1				->Checked?1:0);
	fprintf(file,".db=%i\n"					,db1				->Checked?1:0);

	fprintf(file,"\nSprite Metadata header/terminator:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"None=%i\n"			,Noterminator1		->Checked?1:0);
	fprintf(file,"Sprite count=%i\n"	,Spritecountheader1	->Checked?1:0);
	fprintf(file,"N flag=%i\n"			,Nflagterminator1	->Checked?1:0);
	fprintf(file,"$FF=%i\n"				,FFterminator1		->Checked?1:0);
	fprintf(file,"Single $00=%i\n"		,Single00terminator1->Checked?1:0);
	fprintf(file,"Double $00=%i\n"		,Double00terminator1->Checked?1:0);

	fprintf(file,"\nASCII base offset (default is -0x20 in classic NESST):\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"-0x20=%i\n"					,MASCIIneg20h		->Checked?1:0);
	fprintf(file,"-0x30=%i\n"					,MASCIIneg30h		->Checked?1:0);
	fprintf(file,"-0x40=%i\n"					,MASCIIneg40h		->Checked?1:0);

	fprintf(file,"\nWorkspace arrangement - Spritelist:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Spritelist Left=%i\n"					,Sprlistl1			->Checked?1:0);
	fprintf(file,"Spritelist Center=%i\n"				,Sprlistc1			->Checked?1:0);

	fprintf(file,"\nWorkspace arrangement - CHR tools:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"CHR tools Top=%i\n"					,CHReditortoolbartop->Checked?1:0);
	fprintf(file,"CHR tools Bottom=%i\n"				,CHReditortoolbarbottom->Checked?1:0);

	fprintf(file,"\nMenu - Open/Save Binary Actions:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"OSBA in File menu=%i\n"				,iRadioOpenSave<=1?1:0);
	fprintf(file,"OSBA in type menus=%i\n"				,iRadioOpenSave==2?1:0);
	fprintf(file,"OSBA in both=%i\n"					,iRadioOpenSave>=3?1:0);


	fprintf(file,"\nScreen grid rules - Navigator:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Navigator Always=%i\n"				,AlwaysNavigator1		->Checked?1:0);
	fprintf(file,"Navigator MouseOver=%i\n"				,MouseNavigator1		->Checked?1:0);
	fprintf(file,"Navigator Mouse or Button=%i\n"		,MouseButtonNavigator1	->Checked?1:0);
	fprintf(file,"Navigator Button=%i\n"				,ButtonNavigator1		->Checked?1:0);
	fprintf(file,"Navigator Never=%i\n"					,NeverNavigator1		->Checked?1:0);

	fprintf(file,"\nScreen grid rules - Main canvas:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Main canvas Always=%i\n"				,AlwaysCanvas1		->Checked?1:0);
	fprintf(file,"Main canvas MouseOver=%i\n"			,MouseCanvas1		->Checked?1:0);
	fprintf(file,"Main canvas Mouse or Button=%i\n"		,MouseButtonCanvas1	->Checked?1:0);
	fprintf(file,"Main canvas Button=%i\n"				,ButtonCanvas1		->Checked?1:0);
	fprintf(file,"Main canvas Never=%i\n"				,NeverCanvas1		->Checked?1:0);


	fprintf(file,"\nIncrease/Decrease ink behaviour:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Cap min/max=%i\n"						,IncDecCap1			->Checked?1:0);
	fprintf(file,"Wrap around=%i\n"						,IncDecWraparound1	->Checked?1:0);

	fprintf(file,"\nIncrease/Decrease ink application:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Per click=%i\n"						,IncDecPerclick1	->Checked?1:0);
	fprintf(file,"Over distance=%i\n"					,OverDistance1		->Checked?1:0);

	fprintf(file,"\nIncrease/Decrease ink flow:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Quickest=%i\n"						,IncDecFlow1		->Checked?1:0);
	fprintf(file,"Quick=%i\n"							,IncDecFlow2		->Checked?1:0);
	fprintf(file,"Medium=%i\n"							,IncDecFlow3		->Checked?1:0);
	fprintf(file,"Slow=%i\n"							,IncDecFlow4		->Checked?1:0);
	fprintf(file,"Slowest=%i\n"							,IncDecFlow5		->Checked?1:0);

	fprintf(file,"\nStartup settings part 1:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"GUI Scale x2=%i\n"			,prefStartScale2x		?1:0);
	fprintf(file,"GUI Scale x3=%i\n"			,prefStartScale3x	   	?1:0);
	fprintf(file,"GUI Scale x4=%i\n"			,prefStartScale4x	  	?1:0);

	fprintf(file,"Preselected colour 0 =%i\n"	,prefStartCol0		?1:0);
	fprintf(file,"Preselected colour 1 =%i\n"	,prefStartCol1	   	?1:0);
	fprintf(file,"Preselected colour 2 =%i\n"	,prefStartCol2	  	?1:0);
	fprintf(file,"Preselected colour 3 =%i\n"	,prefStartCol3	  	?1:0);

	fprintf(file,"Preselected subpalette 0 =%i\n"	,prefStartSubpal0	?1:0);
	fprintf(file,"Preselected subpalette 1 =%i\n"	,prefStartSubpal1	?1:0);
	fprintf(file,"Preselected subpalette 2 =%i\n"	,prefStartSubpal2	?1:0);
	fprintf(file,"Preselected subpalette 3 =%i\n"	,prefStartSubpal3	?1:0);

	//checkbox options
	fprintf(file,"\n\nCheckbox options:\n");
	fprintf(file,"=========================================================\n");

    fprintf(file,"\nEditor bitmask options:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Apply to pen=%i\n"					,Applytopen1		->Checked?1:0);
	fprintf(file,"Apply to rotate=%i\n"					,Applytorotate1		->Checked?1:0);
	fprintf(file,"Apply to mirror=%i\n"					,Applytomirror1		->Checked?1:0);
	fprintf(file,"Apply to nudge=%i\n"					,Applytonudge1		->Checked?1:0);
	fprintf(file,"Apply to paste=%i\n"					,Applytopaste1		->Checked?1:0);

	fprintf(file,"\nMetasprite text export:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Ask for metasprite name=%i\n"			,AskMetaName1		->Checked?1:0);
	fprintf(file,"Ask for metasprite bank name=%i\n"	,AskBankName1		->Checked?1:0);

	fprintf(file,"\nFind/Remove/Sort options:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Sort tiles on removal=%i\n"			,sortonremoval1		->Checked?1:0);
	fprintf(file,"Force: look on active tab only=%i\n"			,ForceActiveTab1	->Checked?1:0);
	fprintf(file,"Include NT/map=%i\n"					,IncludeNametables1	->Checked?1:0);
	fprintf(file,"Include metasprites=%i\n"			,IncludeMetasprites1->Checked?1:0);

	fprintf(file,"\nMisc. Grids and guides options:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Yellow metasprite scanline warning=%i\n"	,bWarnMsprYellow	?1:0);
	fprintf(file,"Orange metasprite scanline warning=%i\n"	,bWarnMsprOrange	?1:0);
	fprintf(file,"Red metasprite scanline warning=%i\n"		,bWarnMsprRed		?1:0);
	fprintf(file,"Autoshow grid while dragging=%i\n"		,AutoViewDragMode1->Checked?1:0);

	fprintf(file,"\nMisc. Editor options:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Autostore last used colour=%i\n"	,AutostoreLastUsed->Checked	?1:0);


	fprintf(file,"\nWorkspace options part 2:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Snap forms to monitor edge=%i\n"		,bSnapToScreen?1:0);
	fprintf(file,"Lightbox mode alpha=%i\n"				,iGlobalAlpha);

	fprintf(file,"\nNametable binary export:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Include nametable=%i\n"				,MSaveIncName		->Checked?1:0);
	fprintf(file,"Include attributes=%i\n"				,MSaveIncAttr		->Checked?1:0);
	fprintf(file,"Perform RLE compression=%i\n"			,MSaveRLE			->Checked?1:0);

	fprintf(file,"\nBMP import:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Best offsets=%i\n"				,MImportBestOffsets	->Checked?1:0);
	fprintf(file,"Lossy import=%i\n"				,MImportLossy		->Checked?1:0);
	fprintf(file,"Tile threshold=%i\n"				,MImportThreshold	->Checked?1:0);
	fprintf(file,"Without colour data=%i\n"			,MImportNoColorData	->Checked?1:0);

	fprintf(file,"\nPalette text export:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Include filename in palette label=%i\n"			,bExportPalFilename?1:0);
	fprintf(file,"Include set in palette label=%i\n"				,bExportPalSet?1:0);


	fprintf(file,"\nAdvanced preferences:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Force 0x0f=%i\n"						,SafeColours		->Checked?1:0);
	fprintf(file,"Force shared BG colour=%i\n"		   	,SharedBGcol		->Checked?1:0);

	fprintf(file,"\nStartup settings part 2:\n");
	fprintf(file,"---------------------------------------------------------\n");
	fprintf(file,"Show CHR editor=%i\n"				,prefStartShowCHR		?1:0);
	fprintf(file,"Show Metasprite manager=%i\n"		,prefStartShowMM		?1:0);
	fprintf(file,"Show grid=%i\n"					,prefStartGridShow		?1:0);
	fprintf(file,"Grid x1=%i\n"						,prefStartGrid1			?1:0);
	fprintf(file,"Grid x2=%i\n"						,prefStartGrid2			?1:0);
	fprintf(file,"Grid x4=%i\n"						,prefStartGrid4			?1:0);
	fprintf(file,"Grid 32x30=%i\n"					,prefStartGrid32x30		?1:0);
	fprintf(file,"Grid CHR Pixels=%i\n"				,prefStartGridPixelCHR	?1:0);

	fclose(file);
	BlockDrawing(false);

}

bool __fastcall TFormMain::LoadConfig()
{
	FILE *file;
	char *text;
	int i,j,size;

	file=fopen((globalDir+"preferences.cfg").c_str(),"rb");

	if(!file) return false;

	BlockDrawing(true);

	fseek(file,0,SEEK_END);
	size=ftell(file);
	fseek(file,0,SEEK_SET);

	text=(char*)malloc(size+1);
	text[size]=0;

	fread(text,size,1,file);
	fclose(file);


	
	byte1					->Checked=nss_get_bool(text,size,".byte=");
	db1						->Checked=nss_get_bool(text,size,".db=");

	Noterminator1			->Checked=nss_get_bool(text,size,"None=");
	Spritecountheader1		->Checked=nss_get_bool(text,size,"Sprite count=");
	Nflagterminator1		->Checked=nss_get_bool(text,size,"N flag=");
	FFterminator1			->Checked=nss_get_bool(text,size,"$FF=");
	Single00terminator1		->Checked=nss_get_bool(text,size,"Single $00=");
	Double00terminator1		->Checked=nss_get_bool(text,size,"Double $00=");
	
	MASCIIneg20h			->Checked=nss_get_bool(text,size,"-0x20=");
	MASCIIneg30h			->Checked=nss_get_bool(text,size,"-0x30=");
	MASCIIneg40h			->Checked=nss_get_bool(text,size,"-0x40=");

	Sprlistl1				->Checked=nss_get_bool(text,size,"Spritelist Left=");
	Sprlistc1				->Checked=nss_get_bool(text,size,"Spritelist Center=");
	CHReditortoolbartop		->Checked=nss_get_bool(text,size,"CHR tools Top=");
	CHReditortoolbarbottom	->Checked=nss_get_bool(text,size,"CHR tools Bottom=");

	if(Sprlistl1->Checked) GroupBoxMetaSprite->Align=alRight;
	if(Sprlistc1->Checked) GroupBoxMetaSprite->Align=alLeft;

	//CHR EDITOR fixed in its own createForm event.

	bool tmpRadioOpenSave1=nss_get_bool(text,size,"OSBA in File menu=");
	bool tmpRadioOpenSave2=nss_get_bool(text,size,"OSBA in type menus=");
	bool tmpRadioOpenSave3=nss_get_bool(text,size,"OSBA in both=");
	if(tmpRadioOpenSave1) iRadioOpenSave=1;
	if(tmpRadioOpenSave2) iRadioOpenSave=2;
	if(tmpRadioOpenSave3) iRadioOpenSave=3;

	AlwaysNavigator1		->Checked=nss_get_bool(text,size,"Navigator Always=");
	MouseNavigator1         ->Checked=nss_get_bool(text,size,"Navigator MouseOver=");
	MouseButtonNavigator1   ->Checked=nss_get_bool(text,size,"Navigator Mouse or Button=");
	ButtonNavigator1        ->Checked=nss_get_bool(text,size,"Navigator Button=");
	NeverNavigator1         ->Checked=nss_get_bool(text,size,"Navigator Never=");

	AlwaysCanvas1			->Checked=nss_get_bool(text,size,"Main canvas Always=");
	MouseCanvas1         	->Checked=nss_get_bool(text,size,"Main canvas MouseOver=");
	MouseButtonCanvas1   	->Checked=nss_get_bool(text,size,"Main canvas Mouse or Button=");
	ButtonCanvas1        	->Checked=nss_get_bool(text,size,"Main canvas Button=");
	NeverCanvas1         	->Checked=nss_get_bool(text,size,"Main canvas Never=");

	IncDecCap1				->Checked=nss_get_bool(text,size,"Cap min/max=");
	IncDecWraparound1		->Checked=nss_get_bool(text,size,"Wrap around=");

	IncDecPerclick1			->Checked=nss_get_bool(text,size,"Per click=");
	OverDistance1			->Checked=nss_get_bool(text,size,"Over distance=");

	IncDecFlow1				->Checked=nss_get_bool(text,size,"Quickest=");
	IncDecFlow2				->Checked=nss_get_bool(text,size,"Quick=");
	IncDecFlow3				->Checked=nss_get_bool(text,size,"Medium=");
	IncDecFlow4				->Checked=nss_get_bool(text,size,"Slow=");
	IncDecFlow5				->Checked=nss_get_bool(text,size,"Slowest=");

	Applytopen1				->Checked=nss_get_bool(text,size,"Apply to pen=");
	Applytorotate1			->Checked=nss_get_bool(text,size,"Apply to rotate=");
	Applytomirror1			->Checked=nss_get_bool(text,size,"Apply to mirror=");
	Applytonudge1			->Checked=nss_get_bool(text,size,"Apply to nudge=");
	Applytopaste1			->Checked=nss_get_bool(text,size,"Apply to paste=");

	AskMetaName1			->Checked=nss_get_bool(text,size,"Ask for metasprite name=");
	AskBankName1			->Checked=nss_get_bool(text,size,"Ask for metasprite bank name=");

	sortonremoval1			->Checked=nss_get_bool(text,size,"Sort tiles on removal=");
	ForceActiveTab1			->Checked=nss_get_bool(text,size,"Force: look on active tab only=");
	IncludeNametables1		->Checked=nss_get_bool(text,size,"Include NT/map=");
	IncludeMetasprites1		->Checked=nss_get_bool(text,size,"Include metasprites=");

	MSaveIncName			->Checked=nss_get_bool(text,size,"Include nametable=");
	MSaveIncAttr			->Checked=nss_get_bool(text,size,"Include attributes=");
	MSaveRLE				->Checked=nss_get_bool(text,size,"Perform RLE compression=");

	SafeColours				->Checked=nss_get_bool(text,size,"Force 0x0f=");
	SharedBGcol				->Checked=nss_get_bool(text,size,"Force shared BG colour=");
	sharedCol0=SharedBGcol->Checked;

	bWarnMsprYellow                  =nss_get_bool(text,size,"Yellow metasprite scanline warning=");
	bWarnMsprOrange                  =nss_get_bool(text,size,"Orange metasprite scanline warning=");
	bWarnMsprRed                     =nss_get_bool(text,size,"Red metasprite scanline warning=");
	AutoViewDragMode1		->Checked=nss_get_bool(text,size,"Autoshow grid while dragging=");

	AutostoreLastUsed		->Checked=nss_get_bool(text,size,"Autostore last used colour=");

	bSnapToScreen					 =nss_get_bool(text,size,"Snap forms to monitor edge=");
	iGlobalAlpha					 =nss_get_int(text,size,"Lightbox mode alpha=");
	if(iGlobalAlpha<140) iGlobalAlpha=140;
	if(iGlobalAlpha>220) iGlobalAlpha=220;

	prefStartShowCHR		=nss_get_bool(text,size,"Show CHR editor=");
	prefStartShowMM			=nss_get_bool(text,size,"Show Metasprite manager=");

	bExportPalFilename		=nss_get_bool(text,size,"Include filename in palette label=");
	bExportPalSet           =nss_get_bool(text,size,"Include set in palette label=");

	SpeedButtonGridTile		->Down=nss_get_bool(text,size,"Grid x1=");
	SpeedButtonGridAtr		->Down=nss_get_bool(text,size,"Grid x2=");
	SpeedButtonGridBlock	->Down=nss_get_bool(text,size,"Grid x4=");
	SpeedButtonGridAll		->Down=nss_get_bool(text,size,"Show grid=");
	SpeedButtonGridScreen   ->Down=nss_get_bool(text,size,"Grid 32x30=");
	CHRpixelgrid1			->Checked=nss_get_bool(text,size,"Grid CHR Pixels=");

	//SpeedButtonGridAll->Caption=SpeedButtonGridAll->Down?"on":"off";

	prefStartGrid1 		= SpeedButtonGridTile->Down;
	prefStartGrid2 		= SpeedButtonGridAtr->Down;
	prefStartGrid4 		= SpeedButtonGridBlock->Down;
	prefStartGridShow 	= SpeedButtonGridAll->Down;
	prefStartGrid32x30   =SpeedButtonGridScreen->Down;
	prefStartGridPixelCHR=CHRpixelgrid1->Checked;

	prefStartScale2x	=nss_get_bool(text,size,"GUI Scale x2=");
	prefStartScale3x    =nss_get_bool(text,size,"GUI Scale x3=");
	prefStartScale4x    =nss_get_bool(text,size,"GUI Scale x4=");

	prefStartCol0       =nss_get_bool(text,size,"Preselected colour 0 =");
	prefStartCol1       =nss_get_bool(text,size,"Preselected colour 1 =");
	prefStartCol2       =nss_get_bool(text,size,"Preselected colour 2 =");
	prefStartCol3       =nss_get_bool(text,size,"Preselected colour 3 =");

	prefStartSubpal0    =nss_get_bool(text,size,"Preselected subpalette 0 =");
	prefStartSubpal1    =nss_get_bool(text,size,"Preselected subpalette 1 =");
	prefStartSubpal2    =nss_get_bool(text,size,"Preselected subpalette 2 =");
	prefStartSubpal3    =nss_get_bool(text,size,"Preselected subpalette 3 =");



	BlockDrawing(false);
	return true;
}

void __fastcall TFormMain::UpdateRGBM(void)
{
	SpeedButtonMaskB->Caption="B"+IntToStr((ppuMask&0x80?1:0));
	SpeedButtonMaskG->Caption="G"+IntToStr((ppuMask&0x40?1:0));
	SpeedButtonMaskR->Caption="R"+IntToStr((ppuMask&0x20?1:0));
	SpeedButtonMaskM->Caption="M"+IntToStr((ppuMask&0x01?1:0));

}



bool __fastcall TFormMain::MouseTypeIn(int X,int Y)
{
	if(SpeedButtonTypeIn->Down)
	{
		nameSelection.left  =nameTableViewX+X/(8*uiScale);
		nameSelection.top   =nameTableViewY+Y/(8*uiScale);
		nameSelection.right =nameSelection.left+1;
		nameSelection.bottom=nameSelection.top +1;

		UpdateNameTable(-1,-1,true);
        if(FormNavigator->Visible)
		{
			//FormNavigator->DrawRange(nameSelection.left,nameSelection.top,nameSelection.right,nameSelection.bottom);
			FormNavigator->UpdateLines(true);
		}

		//FormNavigator->Draw(false,false);
		UpdateStats();

		return true;
	}

	return false;
}



void __fastcall TFormMain::NameTableTypeIn(int tile)
{
	int dx,dy;

	if(nameSelection.left>=0&&nameSelection.right>=0)
	{
		dx=nameSelection.left;
		dy=nameSelection.top;

		SetUndo();

		if(SpeedButtonTiles->Down) nameTable[dy*nameTableWidth+dx]=(tile+typeInFontOffset)&255;

		if(SpeedButtonPal->Down) AttrSet(dx,dy,palActive,false);

		++nameSelection.left;

		if(nameSelection.left>=nameTableWidth) nameSelection.left=nameTableWidth-1;
		if(nameSelection.left<0) nameSelection.left=0;

		nameSelection.right =nameSelection.left+1;
		nameSelection.bottom=nameSelection.top+1;

		UpdateNameTable(dx,dy,false);
		//FormNavigator->Draw(false,false); //bookmark: monitor for performance
        if(FormNavigator->Visible)
		{
			FormNavigator->DrawRange(dx,dy,dx+1,dy+1,false);
			FormNavigator->UpdateLines(true);
		}
		UpdateStats();


	}
}



void __fastcall TFormMain::DrawMetaSprite(TImage *img,int spr_id,int scale,bool grid,bool frame_all,bool frame_none)
{
	int i,j,x,y,cy,cnt,tile,attr,pp,hcol,vcol,bank;
	char str[128];
	TColor frame;
	unsigned char *d1;
	bool sel[64];
	int check[128];

	//clear

	img->Picture->Bitmap->Canvas->Brush->Style=bsSolid;
	img->Picture->Bitmap->Canvas->Brush->Color=(TColor)outPalette[bgPal[palBank*16+0]];
	img->Picture->Bitmap->Canvas->FillRect(TRect(0,0,128*scale,128*scale));

	//draw grid

	if(grid)
	{
		for(i=0;i<img->Width;i+=8*scale)
		{
			d1=(unsigned char*)img->Picture->Bitmap->ScanLine[i];

			for(j=0;j<img->Width;j+=2)
			{
				hcol=(spriteGridY*scale==i)?128:64;

				*d1+++=hcol;
				*d1+++=hcol;
				*d1+++=hcol;

				d1+=3;
			}

			for(j=1;j<img->Height;j+=2)
			{
				d1=(unsigned char*)img->Picture->Bitmap->ScanLine[j]+i*3;

				vcol=(spriteGridX*scale==i)?128:64;

				*d1+++=vcol;
				*d1+++=vcol;
				*d1+++=vcol;
			}

			*d1=*d1;//to prevent warning
		}
	}

	//draw sprites

	pp=spr_id*64*4+63*4;

	memset(check,0,sizeof(check));

	for(i=63;i>=0;--i)//reverse order to make proper sprites drawing priority
	{
		y   =metaSprites[pp+0];
		tile=metaSprites[pp+1];
		attr=metaSprites[pp+2];
		x   =metaSprites[pp+3];

		if(y<255)
		{
			frame=frame_all?clGray:clBlack;


			if(i<ListBoxSpriteList->Items->Count)
				if(!frame_none&&(ListBoxSpriteList->Selected[i]))
				frame=clWhite; //clMenu if we need the distinction
			if(!frame_none&&(spriteActive==i)) frame=clWhite;

			DrawSpriteTile(img->Picture,x,y,tile,attr,frame,scale);

			cy=y;

			for(j=0;j<(SpeedButtonSprite8x16->Down?16:8);++j)
			{
				if(cy>=-63&&cy<128) ++check[cy];

				++cy;
			}
		}

		pp-=4;
	}

	for(i=0;i<128;++i)
	{
		if(check[i]>4 && bWarnMsprYellow)
		{
			DrawSpriteDot(img->Picture,1,img->Height/scale/2-64+i,(TColor)0x00daff,scale);
			DrawSpriteDot(img->Picture,126,img->Height/scale/2-64+i,(TColor)0x00daff,scale);
		}
		if(check[i]>7 && bWarnMsprOrange)
		{
			DrawSpriteDot(img->Picture,1,img->Height/scale/2-64+i,(TColor)0x0090f0,scale);
			DrawSpriteDot(img->Picture,2,img->Height/scale/2-64+i,(TColor)0x0090f0,scale);
			DrawSpriteDot(img->Picture,126,img->Height/scale/2-64+i,(TColor)0x0090f0,scale);
			DrawSpriteDot(img->Picture,125,img->Height/scale/2-64+i,(TColor)0x0090f0,scale);
		}
		if(check[i]>8 && bWarnMsprRed)
		{
			DrawSpriteDot(img->Picture,1,img->Height/scale/2-64+i,(TColor)0x2222e0,scale);
			DrawSpriteDot(img->Picture,2,img->Height/scale/2-64+i,(TColor)0x2222e0,scale);
			DrawSpriteDot(img->Picture,3,img->Height/scale/2-64+i,(TColor)0x2222e0,scale);
			DrawSpriteDot(img->Picture,126,img->Height/scale/2-64+i,(TColor)0x2222e0,scale);
			DrawSpriteDot(img->Picture,125,img->Height/scale/2-64+i,(TColor)0x2222e0,scale);
			DrawSpriteDot(img->Picture,124,img->Height/scale/2-64+i,(TColor)0x2222e0,scale);
		}
	}

	img->Repaint();
}


void __fastcall TFormMain::DrawMetaSpriteExport(TPicture *img,int spr_id,int scale,bool grid,bool frame_all,bool frame_none)
{
	int i,j,x,y,cy,cnt,tile,attr,pp,hcol,vcol,bank;
	char str[128];
	TColor frame;
	unsigned char *d1;
	bool sel[64];
	int check[128];

	//clear

	img->Bitmap->Canvas->Brush->Style=bsSolid;
	img->Bitmap->Canvas->Brush->Color=(TColor)outPalette[bgPal[palBank*16+0]];
	img->Bitmap->Canvas->FillRect(TRect(0,0,128*scale,128*scale));

	//draw grid
	/*
	if(grid)
	{
		for(i=0;i<img->Width;i+=8*scale)
		{
			d1=(unsigned char*)img->Bitmap->ScanLine[i];

			for(j=0;j<img->Width;j+=2)
			{
				hcol=(spriteGridY*scale==i)?128:64;

				*d1+++=hcol;
				*d1+++=hcol;
				*d1+++=hcol;

				d1+=3;
			}

			for(j=1;j<img->Height;j+=2)
			{
				d1=(unsigned char*)img->Bitmap->ScanLine[j]+i*3;

				vcol=(spriteGridX*scale==i)?128:64;

				*d1+++=vcol;
				*d1+++=vcol;
				*d1+++=vcol;
			}

			*d1=*d1;//to prevent warning
		}
	} */

	//draw sprites

	pp=spr_id*64*4+63*4;

	memset(check,0,sizeof(check));

	for(i=63;i>=0;--i)//reverse order to make proper sprites drawing priority
	{
		y   =metaSprites[pp+0];
		tile=metaSprites[pp+1];
		attr=metaSprites[pp+2];
		x   =metaSprites[pp+3];

		if(y<255)
		{
			frame=frame_all?clGray:clBlack;


			if(i<ListBoxSpriteList->Items->Count)
				if(!frame_none&&(ListBoxSpriteList->Selected[i]))
				frame=clWhite; //clMenu if we need the distinction
			if(!frame_none&&(spriteActive==i)) frame=clWhite;
			DrawSpriteTile(img,x,y,tile,attr,frame,scale);

			cy=y;

			for(j=0;j<(SpeedButtonSprite8x16->Down?16:8);++j)
			{
				if(cy>=-63&&cy<64) ++check[cy];

				++cy;
			}
		}

		pp-=4;
	}

	/*for(i=0;i<128;++i)
	{
		if(check[i]>8)
		{
			DrawSpriteDot(img,1,img->Height/scale/2-64+i,(TColor)0x0000ff,scale);
			DrawSpriteDot(img,126,img->Height/scale/2-64+i,(TColor)0x0000ff,scale);
		}
	}

	//img->Repaint(); */
}

//BROKE STUDIO
void __fastcall TFormMain::UpdateMetaSpriteLabel(void)
{
	LabelMetaSprite->Caption=IntToStr(metaSpriteActive)+": "+metaSpriteNames[metaSpriteActive];
}
//

void __fastcall TFormMain::UpdateMetaSprite(void)
{
	int i,j,x,y,cnt,tile,attr,pp,hcol,vcol,bank;
	char str[128];
	TColor frame;
	unsigned char *d1;
	bool sel[64];
	cueUpdateMetasprite=false;
	DrawMetaSprite(ImageMetaSprite,metaSpriteActive,uiScale,SpeedButtonSpriteGrid->Down,SpeedButtonFrameAll->Down,SpeedButtonFrameNone->Down);

	//update list

	pp=metaSpriteActive*64*4;
	cnt=0;

	for(i=0;i<64;++i)
	{
		if(metaSprites[pp]<255) ++cnt;

		pp+=4;
	}

	if(spriteActive>cnt-1) spriteActive=cnt-1;

	if(spriteActive<0) spriteActive=0;

	for(i=0;i<64;++i)
	{
		if(i<ListBoxSpriteList->Items->Count) sel[i]=ListBoxSpriteList->Selected[i]; else sel[i]=false;
	}

	sel[spriteActive]=true;

	ListBoxSpriteList->Clear();

	pp=metaSpriteActive*64*4;

	for(i=0;i<64;++i)
	{
		if(metaSprites[pp]<255)
		{
			x   =metaSprites[pp+3]-spriteGridX;
			y   =metaSprites[pp+0]-spriteGridY;
			tile=metaSprites[pp+1];
			bank=metaSprites[pp+1]&1;

			if(SpeedButtonSprite8x16->Down) tile&=0xfe;

			sprintf(str,"%2.2x: X%c%3.3i Y%c%3.3i Tile $%2.2x Pal %i ",i,x<0?'-':' ',abs(x),y<0?'-':' ',abs(y),tile,metaSprites[pp+2]&3);

			if(SpeedButtonSprite8x16->Down) strcat(str,!bank?"A ":"B ");

			if(metaSprites[pp+2]&OAM_FLIP_V) strcat(str,"V");
			if(metaSprites[pp+2]&OAM_FLIP_H) strcat(str,"H");
			if(metaSprites[pp+2]&OAM_PRIO)	 strcat(str,"P");
			if(metaSprites[pp+2]&OAM_B4) 	 strcat(str,"4");
			if(metaSprites[pp+2]&OAM_B3) 	 strcat(str,"3");
			if(metaSprites[pp+2]&OAM_B2) 	 strcat(str,"2");
			ListBoxSpriteList->Items->Add(str);

			ListBoxSpriteList->Selected[ListBoxSpriteList->Items->Count-1]=sel[i];
		}

		pp+=4;
	}

	ListBoxSpriteList->ItemIndex=spriteActive;

	//LabelMetaSprite->Caption="Metasprite "+IntToStr(metaSpriteActive);
	//^disabled to accomodate the below merge
    //BROKE STUDIO
	UpdateMetaSpriteLabel();   //might want to move to MM update routine?
	//


	cueUpdateMM=true;

}



void squeeze_sprites(void)
{
	int i,j,k,pp;

	pp=0;

	for(i=0;i<256;++i)
	{
		for(j=0;j<63;++j)
		{
			if(metaSprites[pp+j*4]==255)
			{
				memcpy(&metaSprites[pp+j*4],&metaSprites[pp+j*4+4],64*4-j*4-4);

				for(k=0;k<4;++k) metaSprites[pp+63*4+k]=255;
			}
		}

		pp+=64*4;
	}

	pp=metaSpriteActive*64*4+spriteActive*4;

	while(pp>=4)
	{
		if(metaSprites[pp]==255) --spriteActive; else break;

		pp-=4;
	}
}



void __fastcall TFormMain::MoveSprite(int dx,int dy)
{
	int i,off;

	if(SpeedButtonSpriteSnap->Down)
	{
		dx*=8;
		dy*=8;
	}

	for(i=0;i<ListBoxSpriteList->Items->Count;++i)
	{
		if(ListBoxSpriteList->Selected[i])
		{
			off=metaSpriteActive*64*4+i*4;

			if(metaSprites[off]<255)
			{
				if(SpeedButtonSpriteSnap->Down)
				{
					if(dy!=0)
					{
						if(metaSprites[off+0]&7) metaSprites[off+0]&=~7; 
						else metaSprites[off+0]+=dy;
						if (metaSprites[off+0]==255) metaSprites[off+0]==248; //Y must not be 255 since this is handled as nonexistence
					}

					if(dx!=0)
					{
						if(metaSprites[off+3]&7) metaSprites[off+3]&=~7; else metaSprites[off+3]+=dx;
					}
				}
				else
				{
					if(!(metaSprites[off+0]+dy==255)) metaSprites[off+0]+=dy;
					else
					{
                        LabelStats->Caption="WARNING: Cannot move sprite(s) into Ypos 255 (this value signifies nonexistence in NEXXT).";
						StatusUpdateWaiter->Enabled=true;
						holdStats=true;
                    }
					metaSprites[off+3]+=dx;
				}
			}
		}
	}

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}




int __fastcall TFormMain::GetSpriteID(int x,int y)
{
	int i,pp,hgt;

	if(SpeedButtonSprite8x16->Down) hgt=16; else hgt=8;

	pp=metaSpriteActive*64*4;

	x/=uiScale;
	y/=uiScale;

	for(i=0;i<64;++i)
	{
		if(y>=metaSprites[pp]&&y<metaSprites[pp]+hgt&&x>=metaSprites[pp+3]&&x<metaSprites[pp+3]+8) return i;

		pp+=4;
	}

	return -1;
}



void __fastcall TFormMain::SpriteSnap(int id)
{
	int x,y,off;

	off=metaSpriteActive*64*4+id*4;

	if(metaSprites[off]<255)
	{
		x=metaSprites[off+3];
		y=metaSprites[off+0];

		if((x&7)<4) x&=~7; else x=(x&~7)+8;
		if((y&7)<4) y&=~7; else y=(y&~7)+8;

		metaSprites[off+3]=x;
		metaSprites[off+0]=y;
	}
}



void __fastcall TFormMain::SelectSprite(int id,bool multi)
{
	int i,off;

	spriteActive=id;

	if(!multi)
	{
		for(i=0;i<ListBoxSpriteList->Items->Count;++i)
		{
			ListBoxSpriteList->Selected[i]=(i==id)?true:false;
		}
	}

	if(spriteActive>=0)
	{
		off=metaSpriteActive*64*4+spriteActive*4;

		if(metaSprites[off]<255)
		{
			SelectTile   (metaSprites[off+1]);
			SelectPalette(metaSprites[off+2]&3);

			UpdateTiles(true);
		}
	}

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}



void __fastcall TFormMain::SelectTile(int id)
{
	int i;

	tileActive=id;

	chrSelection.left  =tileActive&15;
	chrSelection.top   =tileActive/16;
	chrSelection.right =chrSelection.left+1;
	chrSelection.bottom=chrSelection.top +1;

	for(i=0;i<256;++i) chrSelected[i]=0;

	chrSelected[tileActive]=1;
	chrSelectRect=true;
    if(FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();
}



void __fastcall TFormMain::SelectPalette(int id)
{
	palActive=id;
	DrawPalettes();
}



bool __fastcall TFormMain::OpenMetaSprites(AnsiString name)
{
	FILE *file;
	int size;
	unsigned char data[2];

	file=fopen(name.c_str(),"rb");

	if(!file) return false;

	fseek(file,0,SEEK_END);
	size=ftell(file);
	fseek(file,0,SEEK_SET);

	if(size!=256*64*4+2) return false;

	fread(data,2,1,file);
	fread(metaSprites,256*64*4,1,file);
	fclose(file);

	spriteGridX=data[0];
	spriteGridY=data[1];

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;

	return true;
}



void __fastcall TFormMain::FindDoublesUnused(bool unused)
{
	int i,j,cnt,used;
	bool forceActive = ForceActiveTab1->Checked;
	bool sweepNT = IncludeNametables1->Checked;
	bool sweepMS = IncludeMetasprites1->Checked;

	cnt=0;

	for(i=0;i<256;++i)
	{
		chrSelected[i]=0;

		if(unused)
		{
			used=0;

			if((forceActive)||!(sweepNT||sweepMS)) //if no sweep checkbox is set, also default to active.
			{
				if(PageControlEditor->ActivePage==TabSheetSprite)
					{for(j=0;j<256*64*4;j+=4) if(metaSprites[j+1]==i && metaSprites[j]!=0xFF) ++used;}
				else
					{for(j=0;j<nameTableWidth*nameTableHeight;++j)
					if(nameTable[j]==i) ++used;}
			}
			else
			{
				if(sweepNT) {
					for(j=0;j<nameTableWidth*nameTableHeight;++j) if(nameTable[j]==i) ++used;}
				if(sweepMS) {
					for(j=0;j<256*64*4;j+=4) if(metaSprites[j+1]==i && metaSprites[j]!=0xFF) ++used;}
			}

			if(!used)
			{
				chrSelected[ViewTablePos(i)]=1;
				++cnt;
			}
		}
		else
		{
			for(j=0;j<256;++j)
			{
				//skip comparisons against self & against empty tiles
				if(i!=j&&GetTileHasContents(chr+bankActive+(j<<4)))
				{
					if(!memcmp(chr+bankActive+(j<<4),chr+bankActive+(i<<4),16))
					{
						chrSelected[ViewTablePos(i)]=1;
						++cnt;
					}
				}
			}
			/*
			//original NESST routine. Finds doubles of the current active tile.
			if(!memcmp(chr+bankActive+(tileActive<<4),chr+bankActive+(i<<4),16))
			{
				chrSelected[i]=1;
				++cnt;
			}
			*/
		}
	}

	if(cnt)
	{
		chrSelectRect=false;
		nameSelection.left=-1;
		nameSelection.top=-1;

		UpdateTiles(true);
		UpdateNameTable(-1,-1,true);
		//FormNavigator->Draw(false,false);
		if(openByFileDone) {
			if (FormNavigator->Visible) FormNavigator->Draw(true,true);
			if (FormCHRbit->Visible)	FormCHRbit->UpdateBitButtons();
		}
		UpdateStats();
	}
}



void __fastcall TFormMain::RemoveDoublesUnused(bool unused)
{
	int i,j,k,pp,x,y,w,h,used;
	bool clear[256],skip[256];

	bool forceActive = ForceActiveTab1->Checked;
	bool sweepNT = IncludeNametables1->Checked;
	bool sweepMS = IncludeMetasprites1->Checked;

	SetUndo();

	GetSelection(chrSelection,x,y,w,h);

	for(i=0;i<256;++i)
	{
		clear[i]=false;
		skip[i]=false;
	}

	//these allow nonselected tiles to be skipped.
	if(chrSelectRect)
	{
		if(w>1||h>1)
		{
			for(i=0;i<16;++i)
			{
				for(j=0;j<16;++j)
				{
					skip[i*16+j]=(i>=y&&i<y+h&&j>=x&&j<x+w)?false:true;
				}
			}
		}
	}
	else
	{
		for(i=0;i<256;++i) skip[i]= chrSelected[i]?false:true;
	}

	for(i=0;i<256;++i)
	{
		if(!clear[i]&&!skip[i])
		{
			if(unused)
			{
				used=0;

				if((forceActive)||!(sweepNT||sweepMS)) //if no sweep checkbox is set, also default to active.
				{
					if(PageControlEditor->ActivePage==TabSheetSprite)
						{for(j=0;j<256*64*4;j+=4) if(metaSprites[j+1]==i && metaSprites[j]!=0xFF) ++used;}
					else
						{for(j=0;j<nameTableWidth*nameTableHeight;++j)
							if(nameTable[j]==i) ++used;}
				}
				else
				{
					if(sweepNT) {
						for(j=0;j<nameTableWidth*nameTableHeight;++j) if(nameTable[j]==i) ++used;}
					if(sweepMS) {
						for(j=0;j<256*64*4;j+=4) //added an exception for sprites with an Y of FF since that´s used to imply nonexistence. 
							if(metaSprites[j+1]==i && metaSprites[j]!=0xFF) ++used;}
				}

				if(!used) clear[i]=true;
			}
			else  //action is for doubles
			{
				for(j=i+1;j<256;j++)
				{
					if(!memcmp(&chr[bankActive+i*16],&chr[bankActive+j*16],16)&&!skip[j])
					{
						clear[j]=true;

						for(k=0;k<nameTableWidth*nameTableHeight;k++) if(nameTable[k]==j) nameTable[k]=i;

						for(k=0;k<256*64*4;k+=4) if(metaSprites[k+1]==j) metaSprites[k+1]=i;
					}
				}
			}
		}
	}

	//now remove
	pp=0;
	int prep_p=0;
	if (sortonremoval1->Checked)
	{
		for(i=0;i<256;++i)
		{
			if(!clear[i]&&!skip[i])
			{
				while(skip[pp/16]) pp+=16;

				for(j=0;j<nameTableWidth*nameTableHeight;++j) if(nameTable[j]==i) nameTable[j]=pp/16;

				for(j=0;j<256*64*4;j+=4) if(metaSprites[j+1]==i) metaSprites[j+1]=pp/16;

				for(j=0;j<16;++j)
				{
					chr[bankActive+pp]=chr[bankActive+i*16+j];

					++pp;
				}
				tileProperties[(bankActive/16)+prep_p]=tileProperties[(bankActive/16)+i];
				++prep_p;
			}
		}
		for(;pp<4096;pp++) if(!skip[pp/16])			chr[bankActive+pp]=0;
		for(;prep_p<256;prep_p++) if(!skip[prep_p]) tileProperties[(bankActive/16)+prep_p]=0;
	}
	else {
		for(;pp<4096;pp++) if(clear[pp/16]&&!skip[pp/16]){
			chr[bankActive+pp]=0;
		}
		for(;prep_p<256;prep_p++) if(clear[prep_p]&&!skip[prep_p]){
			tileProperties[(bankActive/16)+prep_p]=0;
		}
	}

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
    if(FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();
	if (FormNavigator->Visible) FormNavigator->Draw(false,false);
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}



bool __fastcall TFormMain::OverwritePrompt(AnsiString filename)
{
	if(FileExists(filename)) if(Application->MessageBox(("File "+filename+" already exists. Overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES) return false;

	return true;
}



void __fastcall TFormMain::SetLatestCHR(int offset,int size)
{
	AnsiString str;

	CHRLatestOffset=offset;
	CHRLatestSize=size;

	str="Latest ";

	switch(size)
	{
	case 1024: str+="(1K"; break;
	case 2048: str+="(2K"; break;
	case 4096: str+="(4K"; break;
	case 8192: str+="(8K"; break;
	default: str+="(Selection"; break;
	}

	str+=" from tile $"+IntToHex(CHRLatestOffset/16,2)+")";

	MCHRSaveLatest->Caption=str;
}



void __fastcall TFormMain::UpdateAll(void)
{
	bool normal;

	if(nameTableWidth>=32&&nameTableHeight>=30) normal=true; else normal=false;
    Saveasscreen32x301->Enabled=normal;
	MSaveNameTableBIN->Enabled=normal;

	if(nameTableWidth==32&&nameTableHeight==30) normal=true; else normal=false;
	MSaveNameTableASM->Enabled=normal;
	MSaveNameTableC  ->Enabled=normal;

	if(FormSetSize)
	{
		FormSetSize->NewWidth =nameTableWidth;
		FormSetSize->NewHeight=nameTableHeight;

		FormSetSize->RadioButtonNormal->Checked=normal;
		FormSetSize->RadioButtonUser  ->Checked=!normal;
	}
	FormMain->AlphaBlendValue=iGlobalAlpha;
    FormMain->ScreenSnap=bSnapToScreen;

	pal_validate();

	CorrectView();
	DrawPalettes();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
    FormBankCHR->Update();
	FormCHRbit->UpdateBitButtons();

	FormPropConditions->SetConditions();
	cueUpdateMetasprite=true;
	cueStats=true;
}



void __fastcall TFormMain::CorrectView(void)
{
	if(nameTableWidth<32)
	{
		nameTableViewX=-(32-nameTableWidth)/2;
	}
	else
	{
		if(nameTableViewX>=nameTableWidth-32) nameTableViewX=nameTableWidth-32;
		if(nameTableViewX<0) nameTableViewX=0;
	}

	if(nameTableHeight<32)
	{
		nameTableViewY=-(32-nameTableHeight)/2;
	}
	else
	{

		if(nameTableViewY>=nameTableHeight-30) nameTableViewY=nameTableHeight-30;  //was 32, but led to a bug where you couldn´ scroll to the last portion of the map in some cases.
        if(nameTableViewY<0) nameTableViewY=0;
	}
}



void __fastcall TFormMain::FlipMetaSprites(bool flip_h,bool flip_v)
{
	int i,off,sx,sy,xs,xe,ys,ye,cx,cy,hwdt,hhgt;

	SetUndo();
	
	xs= 16834;
	xe=-16384;
	ys= 16384;
	ye=-16384;

	hwdt=4;
	hhgt=!SpeedButtonSprite8x16->Down?4:8;

	for(i=0;i<ListBoxSpriteList->Items->Count;++i)
	{
		if(ListBoxSpriteList->Selected[i])
		{
			off=metaSpriteActive*64*4+i*4;

			if(metaSprites[off]<255)
			{
				sx=metaSprites[off+3]+hwdt;
				sy=metaSprites[off+0]+hhgt;

				if(sx<xs) xs=sx;
				if(sx>xe) xe=sx;
				if(sy<ys) ys=sy;
				if(sy>ye) ye=sy;
			}
		}
	}

	cx=xs+(xe-xs)/2;
	cy=ys+(ye-ys)/2;

	for(i=0;i<ListBoxSpriteList->Items->Count;++i)
	{
		if(ListBoxSpriteList->Selected[i])
		{
			off=metaSpriteActive*64*4+i*4;

			if(metaSprites[off]<255)
			{
				if(flip_h)
				{
					metaSprites[off+2]^=OAM_FLIP_H;

					if(xs!=xe)
					{
						sx=metaSprites[off+3]-cx+hwdt;

						sx=-sx;

						metaSprites[off+3]=sx-hwdt+cx;
					}
				}

				if(flip_v)
				{
					metaSprites[off+2]^=OAM_FLIP_V;

					if(ys!=ye)
					{
						sy=metaSprites[off+0]-cy+hhgt;

						sy=-sy;

						metaSprites[off+0]=sy-hhgt+cy;
					}
				}
			}
		}
	}

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}



void __fastcall TFormMain::MovePaletteCursor(int off)
{
	int ptr,col,h,b;

	SetUndo();

	ptr=palBank*16+palActive*4+bgPalCur;

	col=bgPal[ptr];

	h=col&0x0f;
	b=(col>>4)&3;

	if(b==0&&h==15) h=13;

	if(off<-1||off>1)
	{
		if(off<0)
		{
			--b;

			if(h==13&&b==1) b=0;
		}
		else
		{
			++b;

			if(h==13&&b==1) b=2;
		}
	}
	else
	{
		if(off<0) --h; else ++h;
	}

	if(h<0) h=13;
	if(h>13) h=0;
	if(b<0) b=3;
	if(b>3) b=0;

	col=h|(b<<4);
	bgPal[ptr]=col;

	UpdateAll();
}



void __fastcall TFormMain::MetaSpriteCopy(void)
{
	memcpy(metaSpriteCopy,&metaSprites[metaSpriteActive*64*4],sizeof(metaSpriteCopy));
 	//BROKE STUDIO
	strcpy(metaSpriteNameCopy, metaSpriteNames[metaSpriteActive].c_str());

	DWORD Len=sizeof(metaSpriteCopy)+sizeof(metaSpriteNameCopy);
	//
	//DWORD Len=sizeof(metaSpriteCopy);

	HGLOBAL hDst=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,Len);

	if (hDst)
	{
		LPSTR gDst=(LPSTR)GlobalLock(hDst);

		if (gDst)
		{
			if (OpenClipboard(Handle))
			{
				EmptyClipboard();

				memcpy(gDst,metaSpriteCopy,sizeof(metaSpriteCopy));
                //BROKE STUDIO
				memcpy(gDst+sizeof(metaSpriteCopy),metaSpriteNameCopy,sizeof(metaSpriteNameCopy));
				//
				GlobalUnlock(hDst);

				SetClipboardData(CF_META,hDst);
				CloseClipboard();
			}
		}
	}
}



void __fastcall TFormMain::MetaSpritePaste(void)
{
	SetUndo();
	
	OpenClipboard(Handle);

	HGLOBAL hClipBuf = GetClipboardData(CF_META);

	if (hClipBuf)
	{
		LPSTR gSrc = (LPSTR)GlobalLock(hClipBuf);

		if (gSrc != NULL)
		{
			memcpy(metaSpriteCopy,gSrc,sizeof(metaSpriteCopy));
			//BROKE STUDIO
			//if (sizeof(gSrc) > sizeof(metaSpriteCopy)) {  //maybe add this in case the clipboard content comes from an older version)
				memcpy(metaSpriteNameCopy,gSrc+sizeof(metaSpriteCopy),sizeof(metaSpriteNameCopy));
			//}

			//
			GlobalUnlock(gSrc);
		}
	}

	CloseClipboard();

	if(metaSpriteCopy[0]<255)
	{
		memcpy(&metaSprites[metaSpriteActive*64*4],metaSpriteCopy,sizeof(metaSpriteCopy));
		//BROKE STUDIO
		int count=0;
		for (int i = 0; i < 256; i++) {
			AnsiString trimmedName = metaSpriteNames[i].Trim();
			if (trimmedName == metaSpriteNameCopy) {
			count++;
			} else {
				int underscoreIndex = trimmedName.LastDelimiter("__");
				if (underscoreIndex > 0) {
					AnsiString baseName = trimmedName.SubString(1, underscoreIndex - 1);
					if (baseName == metaSpriteNameCopy) {
						AnsiString suffix = trimmedName.SubString(underscoreIndex + 1, trimmedName.Length() - underscoreIndex);
						try {
							int suffixValue = suffix.ToInt();
							if (suffixValue >= 1 && suffixValue <= 1024) {
						 count++;
							}
						} catch (...) {
						// Ignore any errors parsing the suffix as an integer.
						}
					}
				}
			}
		}


		metaSpriteNames[metaSpriteActive] = AnsiString(metaSpriteNameCopy)+"__"+IntToStr(count);
		//
		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	}
}



void __fastcall TFormMain::PaletteCopy(void)
{
	cf_palRotation=0;  //reset subpalette paste rotation

	DWORD Len=16;

	HGLOBAL hDst=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,Len);

	if (hDst)
	{
		LPSTR gDst=(LPSTR)GlobalLock(hDst);

		if (gDst)
		{
			if (OpenClipboard(Handle))
			{
				EmptyClipboard();

				memcpy(gDst,&bgPal[palBank*16],16);

				GlobalUnlock(hDst);

				SetClipboardData(CF_PAL,hDst);
				CloseClipboard();
			}
		}
	}
}



void __fastcall TFormMain::PalettePaste(void)
{
	OpenClipboard(Handle);

	HGLOBAL hClipBuf=GetClipboardData(CF_PAL);

	if (hClipBuf)
	{
		LPSTR gSrc=(LPSTR)GlobalLock(hClipBuf);

		if (gSrc!=NULL)
		{
			memcpy(&bgPal[palBank*16],gSrc,16);

			GlobalUnlock(gSrc);
		}
	}

	CloseClipboard();
	UpdateAll();
}



bool __fastcall TFormMain::IsBlockDrawing(void)
{
	return BlockDrawingFlag;
}



void __fastcall TFormMain::BlockDrawing(bool block)
{
	if(block)
	{
		BlockDrawingFlag=block;
	}
	else
	{
		TimerBlock->Enabled=false;
		TimerBlock->Interval=DrawTimerDuration;
		TimerBlock->Enabled=true;
	}
}

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormPaint(TObject *Sender)
{

	UpdateAll();

}
//---------------------------------------------------------------------------

int FormOriginalWidth;
int FormOriginalHeight;
int PanelEditAreaOriginalWidth;
int PanelEditAreaOriginalHeight;
int PanelToolbarOriginalWidth;
int PanelToolbarOriginalHeight;
int PanelTilesetOriginalHeight;
int GroupBoxTilesOriginalHeight;
int PageControlEditorOriginalHeight;
int GroupBoxMetaSpriteOriginalWidth;
int PanelSpriteViewOriginalHeight;

int GroupBoxLayoutOriginalLeft;
int GroupBoxViewOriginalLeft;
int GroupBoxLayoutOriginalTop;
int GroupBoxViewOriginalTop;


int LayBtnWdt;
int LayBtnHgt;
int LayBtnTop;
int LayBtnLeft;

int LayTxt1Top;	
int LayTxt1Left;	
int LayTxt2Top;	
int LayTxt3Top;
int GridTxtLeft;	
int TypeInOffsTop;
int TypeInOffsLeft;

int Grid1Left;
int Grid2Left;
int Grid4Left;

int BtnMarqLeft;
int BtnMarqTop;
int	BtnMarqWdt;
int	BtnMarqHgt;
int BtnDrawLeft;
int BtnDrawTop;
int	BtnDrawHgt;
int BtnDrawWdt;

int GroupSprListWdt;
int GroupSprListHgt;
int GroupMetaSprWdt;
int GroupMetaSprHgt;
int GroupMetaSprLeft;
int BtnSprGridLeft;
int BtnClrMetSprLeft;
int BtnSprSnapLeft;
int BtnSpr8x16Left;

int BtnForceViewLeft;
int BtnTileViewLeft;

int BtnSprHFlipLeft;
int BtnSprVFlipLeft;
int BtnSprRotLeft;

int BtnClrMetSprWdt;
int BtnSprSnapWdt;
int BtnSpr8x16Wdt;
int PnlSprToolbarWdt;


void __fastcall TFormMain::UpdateMenu(void)
{
	bool bShowNew 	 =  (iRadioOpenSave<=1 || iRadioOpenSave>=3);
	bool bShowClassic =  (iRadioOpenSave==2 || iRadioOpenSave>=3);

	CHR1->Visible			=	bShowNew ? true:false;
	Canvas1->Visible		=	bShowNew ? true:false;
	Palettes1->Visible		=	bShowNew ? true:false;
	Metaspritebank1->Visible=	bShowNew ? true:false;

	MOpenNameTable->Visible		=	bShowClassic ? true:false;
	MSaveMap->Visible			=	bShowClassic ? true:false;
	MSaveNameTableBIN->Visible	=	bShowClassic ? true:false;
	MSaveNameTableASM->Visible	=	bShowClassic ? true:false;
	MSaveNameTableC->Visible    =	bShowClassic ? true:false;
	Options2->Visible           =	bShowClassic ? true:false;
	N17->Visible              	=	bShowClassic ? true:false;

	MCHROpen->Visible           =	bShowClassic ? true:false;
	MCHRSave->Visible           =	bShowClassic ? true:false;
	N1->Visible                 =	bShowClassic ? true:false;

	MOpenMetaSpriteBank->Visible=	bShowClassic ? true:false;
	MSaveMetaSpriteBank->Visible=	bShowClassic ? true:false;
	N23->Visible                =	bShowClassic ? true:false;

	MPaletteOpen->Visible 	=	bShowClassic ? true:false;
	MPaletteSave->Visible 	=	bShowClassic ? true:false;


}

void __fastcall TFormMain::UpdateUIScale(void)
{
	MView2x->Checked=(uiScale==2)?true:false;
	MView3x->Checked=(uiScale==3)?true:false;
	MView4x->Checked=(uiScale==4)?true:false;

	if(uiScale>2 && PageControlEditor->ActivePage==TabSheetTile)
	{	GroupBoxTileControl->Visible=true;}
	else
	{	GroupBoxTileControl->Visible=false;	}

	Width =FormOriginalWidth -512-256+256*uiScale+128*uiScale;
	Height=FormOriginalHeight-512+256*uiScale;

	PanelEditArea->Width =PanelEditAreaOriginalWidth -512+256*uiScale;
	PanelToolbar ->Width =PanelToolbarOriginalWidth  -256+128*uiScale;
	PanelTileset ->Height=PanelTilesetOriginalHeight -256+128*uiScale;
	GroupBoxTiles->Height=GroupBoxTilesOriginalHeight-256+128*uiScale;
	PageControlEditor ->Height=PageControlEditorOriginalHeight-512+256*uiScale;
	GroupBoxMetaSprite->Width=GroupBoxMetaSpriteOriginalWidth -256+128*uiScale;
	PanelSpriteView   ->Height=PanelSpriteViewOriginalHeight  -256+128*uiScale;

	GroupBoxLayout->Left=GroupBoxLayoutOriginalLeft -256+128*uiScale;
	GroupBoxView->Left=GroupBoxViewOriginalLeft -256+128*uiScale;
	GroupBoxLayout->Top=GroupBoxLayoutOriginalTop -256+128*uiScale;
	GroupBoxView->Top=GroupBoxViewOriginalTop -256+128*uiScale;

	ButtonTileView->Left=BtnTileViewLeft -256+128*uiScale;
	ButtonForceView->Left=BtnForceViewLeft -256+128*uiScale;

	ImageName      ->Width =256*uiScale;
	ImageName      ->Height=256*uiScale;
	ImageTiles     ->Width =128*uiScale;
	ImageTiles     ->Height=128*uiScale;
	ImageBigTiles  ->Width =256*uiScale;
	ImageBigTiles  ->Height=256*uiScale;
	ImageMetaSprite->Width =128*uiScale;
	ImageMetaSprite->Height=128*uiScale;

	ImageName      ->Picture->Bitmap->SetSize(ImageName      ->Width,ImageName      ->Height);
	BufBmpName	   ->SetSize			   	 (ImageName      ->Width,ImageName      ->Height);

	BufBmpTiles    ->SetSize			   	 (ImageTiles     ->Width,ImageTiles     ->Height);

	ImageBigTiles  ->Picture->Bitmap->SetSize(ImageTiles     ->Width,ImageTiles     ->Height);
	ImageMetaSprite->Picture->Bitmap->SetSize(ImageMetaSprite->Width,ImageMetaSprite->Height);

	if(PageControlEditor->ActivePage!=TabSheetTile)
	{
		ImageTiles     ->Picture->Bitmap->SetSize(ImageTiles     ->Width,ImageTiles     ->Height);
		UpdateNameTable(-1,-1,true);
		UpdateTiles(true);
	}
	else
	{
		//i don´t like this fix, (having to update these in sequence), but it works.
		//mean to replace it for a sounder update routine eventually.

		//PageControlEditor->ActivePage=TabSheetName;   //temporarily pretend this tab is open
		bForceNameScale=true;
		UpdateNameTable(-1,-1,false);
		UpdateTiles(false);
		bForceNameScale=false;

		//PageControlEditor->ActivePage=TabSheetTile;
        ImageTiles     ->Picture->Bitmap->SetSize(ImageName     ->Width*2,ImageName     ->Height*2);
		if (uiScale!=2) ImageTiles->Stretch = true;
	}


	if(uiScale==1)
	{
		GroupBoxPal->Visible=false;
        GroupBoxSpriteList->Visible		=false;
		GroupBoxLayout->Align=alTop;
		GroupBoxView->Align=alBottom;
		GroupBoxView->Height=83;
		GroupBoxView->Width=139;
		GroupBoxLayout->Height=83;
		GroupBoxLayout->Width=139;

		int compactHgt=4;
		//SpeedButtonTiles->Width	=LayBtnWdt;
		SpeedButtonTiles->Height	=LayBtnHgt-compactHgt;
		SpeedButtonPal->Height	    =LayBtnHgt-compactHgt;
		SpeedButtonTypeIn->Height	=LayBtnHgt-compactHgt;
		SpeedButtonSelTiles->Height	=LayBtnHgt-compactHgt;
		SpeedButtonChecker->Height	=LayBtnHgt-compactHgt;
		SpeedButtonGridAll->Height	=LayBtnHgt-compactHgt;
		SpeedButtonGridTile->Height =LayBtnHgt-compactHgt;
		SpeedButtonGridAtr->Height  =LayBtnHgt-compactHgt;
		SpeedButtonGridBlock->Height=LayBtnHgt-compactHgt;
		SpeedButtonGridScreen->Height=LayBtnHgt-compactHgt;

		SpeedButtonTiles->Top		=LayBtnTop;
		SpeedButtonPal->Top	    	=LayBtnTop+LayBtnHgt+1-compactHgt;
		SpeedButtonTypeIn->Top		=LayBtnTop+(LayBtnHgt+1)*2-compactHgt*2;
		SpeedButtonSelTiles->Top	=LayBtnTop;
		SpeedButtonChecker->Top		=LayBtnTop+LayBtnHgt+1-compactHgt;
		SpeedButtonGridAll->Top		=LayBtnTop+(LayBtnHgt+1)*2-compactHgt*2;
		SpeedButtonGridTile->Top 	=LayBtnTop+(LayBtnHgt+1)*2-compactHgt*2;
		SpeedButtonGridAtr->Top  	=LayBtnTop+(LayBtnHgt+1)*2-compactHgt*2;
		SpeedButtonGridBlock->Top	=LayBtnTop+(LayBtnHgt+1)*2-compactHgt*2;
		SpeedButtonGridScreen->Top  =LayBtnTop+(LayBtnHgt+1)*2-compactHgt*2;
		//SpeedButtonTiles->Left		=LayBtnLeft;

		LabelApplyPatterns->Top		=LayTxt1Top-2;
		LabelApplyAttr->Top			=LayTxt2Top-compactHgt*1-2;
		LabelTypeIn->Top			=LayTxt3Top-compactHgt*2-2;
		LabelSelOnly->Top			=LayTxt1Top-2;
		LabelAttrChecker->Top		=LayTxt2Top-compactHgt*1-2;
		//LabelGrid->Top				=LayTxt3Top-compactHgt*2-2;

		//LabelApplyPatterns->Left	=LayTxt1Left;
		//Label3->Left				=GridTxtLeft;
		StaticTextFontOffset->Top	=TypeInOffsTop-compactHgt*2-2;
		StaticTextFontOffset->Left	=TypeInOffsLeft;

		SpeedButtonGridTile->Left	=Grid1Left;
		SpeedButtonGridAtr->Left	=Grid2Left;
		SpeedButtonGridBlock->Left	=Grid4Left;

		SpeedButtonMarqTile->Left	=TextNullTile->Left;
		SpeedButtonMarqTile->Top	=TextNullTile->Top+16;
		SpeedButtonDrawTile->Left	=TextNullTile->Left+32;
		SpeedButtonDrawTile->Top	=TextNullTile->Top+16;

		SpeedButtonMarqTile->Width	=BtnMarqWdt-4;
		SpeedButtonMarqTile->Height	=BtnMarqHgt-2;

		SpeedButtonDrawTile->Width	=BtnDrawWdt-6;
		SpeedButtonDrawTile->Height	=BtnDrawHgt-2;


		//GroupBoxSpriteList->Width		=GroupSprListWdt;
		//GroupBoxSpriteList->Height		=GroupSprListHgt;
		GroupBoxMetaSprite->Width		=256;
		GroupBoxMetaSprite->Height		=256;
		GroupBoxMetaSprite->Left		=0;
		SpeedButtonSpriteGrid->Left		=BtnSprGridLeft-15;
		SpeedButtonClearMetaSprite->Left=BtnClrMetSprLeft-15;
		SpeedButtonSpriteSnap->Left		=BtnSprSnapLeft-15;
		SpeedButtonSprite8x16->Left 	=BtnSpr8x16Left-15;

		SpeedButtonClearMetaSprite->Width=BtnClrMetSprWdt-5;
		SpeedButtonSpriteSnap->Width	 =BtnSprSnapWdt-5;
		SpeedButtonSprite8x16->Width 	 =BtnSpr8x16Wdt-5;

		SpeedButtonMetaSpriteHFlip->Left=BtnSprHFlipLeft-3;
		SpeedButtonMetaSpriteVFlip->Left=BtnSprVFlipLeft-3;
		SpeedButtonMetaSpriteRotate->Left=BtnSprRotLeft-3;

		PanelSpriteToolbar->Width		=252;
	}
	else
	{


		GroupBoxPal->Visible=true;
		GroupBoxLayout->Align=alLeft;

		GroupBoxView->Align=alRight;

		GroupBoxView->Height=94;
		GroupBoxView->Width=139;
		GroupBoxLayout->Height=94;
		GroupBoxLayout->Width=139;

		///SpeedButtonTiles->Width			=LayBtnWdt;
		SpeedButtonTiles->Height	=LayBtnHgt;
		SpeedButtonPal->Height	    =LayBtnHgt;
		SpeedButtonTypeIn->Height	=LayBtnHgt;
		SpeedButtonSelTiles->Height	=LayBtnHgt;
		SpeedButtonChecker->Height	=LayBtnHgt;
		SpeedButtonGridAll->Height	=LayBtnHgt;
		SpeedButtonGridTile->Height =LayBtnHgt;
		SpeedButtonGridAtr->Height  =LayBtnHgt;
		SpeedButtonGridBlock->Height=LayBtnHgt;
		SpeedButtonGridScreen->Height=LayBtnHgt;

		SpeedButtonTiles->Top		=LayBtnTop;
		SpeedButtonPal->Top	    	=LayBtnTop+LayBtnHgt+1;
		SpeedButtonTypeIn->Top		=LayBtnTop+(LayBtnHgt+1)*2;
		SpeedButtonSelTiles->Top	=LayBtnTop;
		SpeedButtonChecker->Top		=LayBtnTop+LayBtnHgt+1;
		SpeedButtonGridAll->Top		=LayBtnTop+(LayBtnHgt+1)*2;
		SpeedButtonGridTile->Top 	=LayBtnTop+(LayBtnHgt+1)*2;
		SpeedButtonGridAtr->Top  	=LayBtnTop+(LayBtnHgt+1)*2;
		SpeedButtonGridBlock->Top	=LayBtnTop+(LayBtnHgt+1)*2;
		SpeedButtonGridScreen->Top  =LayBtnTop+(LayBtnHgt+1)*2;

		LabelApplyPatterns->Top		=LayTxt1Top;
		LabelApplyAttr->Top			=LayTxt2Top;
		LabelTypeIn->Top			=LayTxt3Top;
		LabelSelOnly->Top			=LayTxt1Top;
		LabelAttrChecker->Top		=LayTxt2Top;
		//LabelGrid->Top				=LayTxt3Top;

		StaticTextFontOffset->Top	=TypeInOffsTop;
		StaticTextFontOffset->Left	=TypeInOffsLeft;

		SpeedButtonGridTile->Left		=Grid1Left;
		SpeedButtonGridAtr->Left		=Grid2Left;
		SpeedButtonGridBlock->Left		=Grid4Left;

		SpeedButtonMarqTile->Left		=BtnMarqLeft;
		SpeedButtonMarqTile->Top		=BtnMarqTop;
		SpeedButtonDrawTile->Left		=BtnDrawLeft;
		SpeedButtonDrawTile->Top		=BtnDrawTop;

		SpeedButtonMarqTile->Width		=BtnMarqWdt;
		SpeedButtonMarqTile->Height		=BtnMarqHgt;

		SpeedButtonDrawTile->Width		=BtnDrawWdt;
		SpeedButtonDrawTile->Height		=BtnDrawHgt;



		//GroupBoxSpriteList->Width		=GroupSprListWdt;
		//GroupBoxSpriteList->Height		=GroupSprListHgt;
		GroupBoxSpriteList->Visible		=true;
		GroupBoxMetaSprite->Width		=(GroupMetaSprWdt/2)*uiScale;
		if(uiScale==4) GroupBoxMetaSprite->Width=GroupBoxMetaSprite->Width-64;
		//rightmost tile of the canvas gets cropped out.
		//-32 gets all in but i don¨t want to bother with making a special case
		//for the sprite list + buttons right now.
		
		GroupBoxMetaSprite->Height		=GroupMetaSprHgt;
		GroupBoxMetaSprite->Left		=GroupMetaSprLeft;
		SpeedButtonSpriteGrid->Left		=BtnSprGridLeft;
		SpeedButtonClearMetaSprite->Left=BtnClrMetSprLeft;
		SpeedButtonSpriteSnap->Left		=BtnSprSnapLeft;
		SpeedButtonSprite8x16->Left 	=BtnSpr8x16Left;
		ButtonForceView->Left			=BtnSpr8x16Left;
		SpeedButtonMetaSpriteHFlip->Left=BtnSprHFlipLeft;
		SpeedButtonMetaSpriteVFlip->Left=BtnSprVFlipLeft;
		SpeedButtonMetaSpriteRotate->Left=BtnSprRotLeft;

		SpeedButtonClearMetaSprite->Width=BtnClrMetSprWdt;
		SpeedButtonSpriteSnap->Width	 =BtnSprSnapWdt;
		SpeedButtonSprite8x16->Width 	 =BtnSpr8x16Wdt;
		PanelSpriteToolbar->Width		=PnlSprToolbarWdt;
	}
	//UpdateAll(); //did this originally, but because of the fix above, at least this cuts out some redundance.
	pal_validate();

	CorrectView();
	DrawPalettes();

	cueUpdateMetasprite=true;
	cueStats=true;

}

void __fastcall TFormMain::FormCreate(TObject *Sender)
{
	FILE *file;
	int i,pp;
	unsigned char buf[192];
	AnsiString dir,name,spr;

	CF_CHR=RegisterClipboardFormat("NESST_CF_CHR");
	CF_NAM=RegisterClipboardFormat("NESST_CF_NAM");
	CF_META=RegisterClipboardFormat("NESST_CF_META");
	CF_PAL=RegisterClipboardFormat("NESST_CF_PAL");
	CF_SUBPAL=RegisterClipboardFormat("NESST_CF_SUBPAL");

	for(i=0;i<4;i++) bBrushMask[i]=true;

	spr="Ctrl + M: Create metasprite from selection\nDrop tile: Add sprite\nLeft click: Select sprite\nRight drag: Move sprite\nCtrl + left drag: Move anchor";
	LabelMetaSpriteHint->Caption=spr;

	FormCaption=Application->Title; //Caption;

	BlockDrawing(false);
    TimerNTstrip->Enabled=false;
	palBank=0;

	memcpy(bgPal,&bgPalDefault[16],sizeof(bgPal));


	nameTableWidth=32;
	nameTableHeight=30;

	nameTableViewX=0;
	nameTableViewY=0;

	//BROKE STUDIO
	for(i=0;i<256;i++)
	{
		metaSpriteNames[i]="Metasprite"+IntToStr(i);
	}

	//

	for(unsigned int j=0;j<chrBanks;j++){chrBankLabels.push_back("Unlabeled");}
	for(unsigned int j=0;j<undo_chrBanks;j++){undo_chrBankLabels.push_back("Unlabeled");}
	for(unsigned int j=0;j<checkpoint_chrBanks;j++){checkpoint_chrBankLabels.push_back("Unlabeled");}
	for(unsigned int j=0;j<undo_checkpoint_chrBanks;j++){undo_checkpoint_chrBankLabels.push_back("Unlabeled");}

	ImageName->Picture=new TPicture();
	ImageName->Picture->Bitmap=new Graphics::TBitmap();
	ImageName->Picture->Bitmap->PixelFormat=pf24bit;

	ImageTiles->Picture=new TPicture();
	ImageTiles->Picture->Bitmap=new Graphics::TBitmap();
	ImageTiles->Picture->Bitmap->PixelFormat=pf24bit;

	ImageMetaSprite->Picture=new TPicture();
	ImageMetaSprite->Picture->Bitmap=new Graphics::TBitmap();
	ImageMetaSprite->Picture->Bitmap->PixelFormat=pf24bit;

	ImageBigTiles->Picture=new TPicture();
	ImageBigTiles->Picture->Bitmap=new Graphics::TBitmap();
	ImageBigTiles->Picture->Bitmap->PixelFormat=pf24bit;


	BufBmpName=new Graphics::TBitmap();
	BufBmpName->PixelFormat=pf4bit;
	BufBmpTiles=new Graphics::TBitmap();
	BufBmpTiles->PixelFormat=pf4bit;

	LayBtnWdt		   =SpeedButtonTiles->Width;
	LayBtnHgt		   =SpeedButtonTiles->Height;
	LayBtnTop		   =SpeedButtonTiles->Top;
	LayBtnLeft		   =SpeedButtonTiles->Left;

	LayTxt1Top			=LabelApplyPatterns->Top;
	LayTxt1Left			=LabelApplyPatterns->Left;
	LayTxt2Top			=LabelApplyAttr->Top;
	LayTxt3Top			=LabelTypeIn->Top;
	//GridTxtLeft			=LabelGrid->Left;
	TypeInOffsTop		=StaticTextFontOffset->Top;
	TypeInOffsLeft		=StaticTextFontOffset->Left;

	Grid1Left			=SpeedButtonGridTile->Left;
	Grid2Left			=SpeedButtonGridAtr->Left;
	Grid4Left			=SpeedButtonGridBlock->Left;

	BtnMarqLeft			=SpeedButtonMarqTile->Left;
	BtnMarqTop			=SpeedButtonMarqTile->Top;
	BtnMarqWdt			=SpeedButtonMarqTile->Width;
	BtnMarqHgt			=SpeedButtonMarqTile->Height;
	BtnDrawLeft			=SpeedButtonDrawTile->Left;
	BtnDrawTop			=SpeedButtonDrawTile->Top;
	BtnDrawWdt			=SpeedButtonDrawTile->Width;
	BtnDrawHgt			=SpeedButtonDrawTile->Height;

	GroupSprListWdt		=GroupBoxSpriteList->Width;
	GroupSprListHgt		=GroupBoxSpriteList->Height;
	GroupMetaSprWdt		=GroupBoxMetaSprite->Width;
	GroupMetaSprHgt		=GroupBoxMetaSprite->Height;
	GroupMetaSprLeft	=GroupBoxMetaSprite->Left;

	FormOriginalWidth          =Width;
	FormOriginalHeight         =Height;
	PanelEditAreaOriginalWidth =PanelEditArea->Width;
	PanelEditAreaOriginalHeight=PanelEditArea->Height;
	PanelToolbarOriginalWidth  =PanelToolbar->Width;
	PanelToolbarOriginalHeight =PanelToolbar->Height;
	PanelTilesetOriginalHeight =PanelTileset->Height;
	GroupBoxTilesOriginalHeight=GroupBoxTiles->Height;
	PageControlEditorOriginalHeight=PageControlEditor->Height;
	GroupBoxMetaSpriteOriginalWidth=GroupBoxMetaSprite->Width;
	PanelSpriteViewOriginalHeight=PanelSpriteView->Height;

	GroupBoxLayoutOriginalLeft=GroupBoxLayout->Left;
	GroupBoxLayoutOriginalTop=GroupBoxLayout->Top;
	GroupBoxViewOriginalLeft=GroupBoxView->Left;
	GroupBoxViewOriginalTop=GroupBoxView->Top;

	BtnTileViewLeft		=ButtonTileView->Left;
	
	BtnSprGridLeft		=SpeedButtonSpriteGrid->Left;
	BtnClrMetSprLeft	=SpeedButtonClearMetaSprite->Left;
	BtnSprSnapLeft		=SpeedButtonSpriteSnap->Left;
	BtnSpr8x16Left	 	=SpeedButtonSprite8x16->Left;
	BtnForceViewLeft	=ButtonForceView->Left;
	BtnSprHFlipLeft		=SpeedButtonMetaSpriteHFlip->Left;
	BtnSprVFlipLeft		=SpeedButtonMetaSpriteVFlip->Left;
	BtnSprRotLeft		=SpeedButtonMetaSpriteRotate->Left;

	BtnClrMetSprWdt		=SpeedButtonClearMetaSprite->Width;
	BtnSprSnapWdt		=SpeedButtonSpriteSnap->Width;
	BtnSpr8x16Wdt		=SpeedButtonSprite8x16->Width;
	PnlSprToolbarWdt	=PanelSpriteToolbar->Width;

	FormMain->Left=(Screen->Width-FormMain->Width)/2;
	FormMain->Top=(Screen->Height-FormMain->Height)/2;


	tileActive=0;
	bankActive=0;
	metaSpriteActive=0;

	spriteGridX=64;
	spriteGridY=64;
	undoSpriteGridX=64;
	undoSpriteGridY=64;
	checkpointSpriteGridX	   =  64;
	checkpointSpriteGridY      =  64;
	undoCheckpointSpriteGridX  =  64;
	undoCheckpointSpriteGridY  =  64;

	spriteActive=0;


	memset(nameTable  ,0  ,sizeof(nameTable));
	memset(attrTable  ,0  ,sizeof(attrTable));
	memset(chr        ,0  ,sizeof(chr));
	memset(chrCopy    ,0  ,sizeof(chrCopy));
	memset(propCopy	  ,0  ,sizeof(propCopy));
	memset(metaSprites,255,sizeof(metaSprites));
	memset(metaSpriteCopy,255,sizeof(metaSpriteCopy));
	memset(metaSpritesBuf,255,sizeof(metaSpritesBuf));
	memset(checkpointMetaSprites,255,sizeof(checkpointMetaSprites));

	propCHRlabel[0] ="solid";
	propCHRlabel[1] ="dangerous";
	propCHRlabel[2] ="unlabeled2";
	propCHRlabel[3] ="unlabeled3";
	propCHRlabel[4] ="unlabeled4";
	propCHRlabel[5] ="unlabeled5";
	propCHRlabel[6] ="escape bit";
	propCHRlabel[7] ="global condition";

	dir=ParamStr(0).SubString(0,ParamStr(0).LastDelimiter("\\/"));
    globalDir=dir;
	file=fopen((dir+"nes.pal").c_str(),"rb");

	if(file)
	{
		if(get_file_size(file)==192)
		{
			Externalnespal1->Checked=true;
			fread(buf,192,1,file);
			fclose(file);

			pp=0;

			for(i=0;i<64;i++)
			{
				basePalette[i]=(buf[pp+2]<<16)|(buf[pp+1]<<8)|buf[pp];
				pp+=3;
			}
		}
	}
	else
	{
		NESSTclassic1->Checked=true;       //todo: maybe allow for a few internal palette preference choices.
		pp=0;

		for(i=0;i<64;i++)
		{
			basePalette[i]=(palette[pp+2]<<16)|(palette[pp+1]<<8)|palette[pp];
			pp+=3;
		}
	}

	ppuMask=0;
	palette_calc();

	nameSelection.left=-1;
	nameSelection.top=-1;
	nameCopyWidth=-1;
	nameCopyHeight=-1;

	chrSelection.left=0;
	chrSelection.right=0;
	chrSelection.right=1;
	chrSelection.bottom=1;
	chrCopyWidth=-1;
	chrCopyHeight=-1;
	chrCopyRect=true;

	for(i=0;i<256;i++) chrSelected[i]=0;

	chrSelected[tileActive]=1;
	chrSelectRect=true;
	//create 8x16 vs 8x8 indirection table
	int num;


	if(((SpeedButtonSprite8x16->Down==true)&&(ButtonForceView->Down==true))){
		SetViewTable(1);
		GroupBoxTiles->Caption="Tileset (8x16 sprite mode)";
	}
	else for(i=0;i<256;i++) {tileViewTable[i]=i;}

	//parameterize this in config sometime?
	SetLatestCHR(0,1024*4); //changed from 8k to 4k for now

	tileXC=-1;
	tileYC=-1;
	nameXC=-1;
	nameYC=-1;
	palHover=-1;
	palColHover=-1;
	colHover=-1;
	spriteHover=-1;
	spriteDrag=-1;
	CHREditorHover=-1;

	dir=reg_load_str(regWorkingDirectory,"");
	openByFileDone=false;

	if(ParamStr(1)!="")
	{
		name=RemoveExt(ParamStr(1));

		if(!ImportBMP(name+".bmp",false,false,-1,256,false,false))
		{
			if(!LoadSession(name+".nss"))
			{
				OpenAll(name);
			}
		}

		dir=ParamStr(1);

	//bandaid for bug with yet unknown cause:
	//1 user had a file that'd load improperly when opened by association.
    //resetting nametable selection prevents it. 
	nameSelection.left=-1;
	nameSelection.top=-1;
	}

	SetCurrentDirectory(dir.c_str());
	openByFileDone=true;
	
	SetUndo();

	unsavedChanges=false;
	//Savesession1->Enabled=false;      //no reason not to let the user save if save knows to redirect to save as.
	SharedBGcol->Checked=true;
	SafeColours->Checked=true;

	
	//we've initialized some startup values beforehand, so if the attempted loadConfig fails,
	//we'll create a factory config.

	if(!LoadConfig())
	{
		uiScale=FAC_GUISCALE;
		bgPalCur=FAC_COL;
		palActive=FAC_PAL;
		typeInFontOffset=0;
		typeInASCIIOffset=32;
		prefStartScale2x=true;
		prefStartCol3=true;
		prefStartSubpal0=true;

		bWarnMsprYellow=true;
		bWarnMsprOrange=false;
		bWarnMsprRed=true;

		bExportPalFilename=true;
		bExportPalSet=true;

		iGlobalAlpha=FAC_ALPHA;
		iRadioOpenSave=1;
		bSnapToScreen=false;

		SaveConfig();
	}
	if(prefStartScale2x) 	 	uiScale = 2;
	else if(prefStartScale3x)	uiScale = 3;
	else if(prefStartScale4x)	uiScale = 4;
	else						uiScale = 2;

	if(prefStartCol0) 	 		bgPalCur = 0;
	else if(prefStartCol1)		bgPalCur = 1;
	else if(prefStartCol2)		bgPalCur = 2;
	else if(prefStartCol3)		bgPalCur = 3;
	else 						bgPalCur = 3;

	if(prefStartSubpal0) 	 	palActive = 0;
	else if(prefStartSubpal1)	palActive = 1;
	else if(prefStartSubpal2)	palActive = 2;
	else if(prefStartSubpal3)	palActive = 3;
	else 						palActive = 0;


	//relative init to sensible presets.
	if(bgPalCur==3) bgPalCurOld=0; else bgPalCurOld=3;
	if(palActive==1) palActiveOld=0;  else palActiveOld=1;

		
	metaSpriteBankName=RemoveExt(ExtractFileName(SaveDialogSession->FileName)); //
	UpdateUIScale();
}
//---------------------------------------------------------------------------

int check_palette_mouse(int X,int Y)
{
	int i,x,y,s;

	s=2;//uiScale;
	y=0;
	x=8*s;

	//check BG palettes

	for(i=0;i<4;i++)
	{
		if(Y>=y&&Y<y+10*s)
		{
			if(X>=x+ 0*s&&X<x+10*s) return 0x00|i;
			if(X>=x+10*s&&X<x+20*s) return 0x10|i;
			if(X>=x+20*s&&X<x+30*s) return 0x20|i;
			if(X>=x+30*s&&X<x+40*s) return 0x30|i;
		}

		x+=(64-8)*s;

		if(x>(64*s))
		{
			x=8*s;
			y=16*s;
		}
	}

	//check the main palette

	if(X>=0&&X<(16*8*s)&&Y>=(32*s)&&Y<(32*s+4*8*s))
	{
		return 0x100|(X/(8*s)+(Y-32*s)/(8*s)*16);
	}

	return -1;
}




void __fastcall TFormMain::PaintBoxPalMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int i,n,col,off,entry;


	n=check_palette_mouse(X,Y);

	if(Shift.Contains(ssLeft))
	{
		if(n>=0)
		{
			if(n&0x100)
			{
				SetUndo();

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
					UpdateAll();

				}
				else if(Shift.Contains(ssShift))
						{
							palActive++;
							palActive=palActive&3; UpdateAll();
						}
			}
			else
			{
				if((palActive!=n&3)&&(AutostoreLastUsed->Checked)) palActiveOld=palActive;
				palActive=n&3;

				Pal0->Checked=false;
				Pal1->Checked=false;
				Pal2->Checked=false;
				Pal3->Checked=false;

				if (palActive==0) Pal0->Checked=true;
				if (palActive==1) Pal1->Checked=true;
				if (palActive==2) Pal2->Checked=true;
				if (palActive==3) Pal3->Checked=true;

				if((bgPalCur!=n>>4)&&(AutostoreLastUsed->Checked)) bgPalCurOld=bgPalCur;
				bgPalCur=n>>4;

				Colour001->Checked=false;
				Colour011->Checked=false;
				Colour101->Checked=false;
				Colour111->Checked=false;

				if (bgPalCur==0) Colour001->Checked=true;
				if (bgPalCur==1) Colour011->Checked=true;
				if (bgPalCur==2) Colour101->Checked=true;
				if (bgPalCur==3) Colour111->Checked=true;



				if(clickC)
				{
					cf_palRotation=0;  //reset

					DWORD Len=4;
					HGLOBAL hDst=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,Len);

					if (hDst)
					{
						LPSTR gDst=(LPSTR)GlobalLock(hDst);
						if (gDst)
						{
							if (OpenClipboard(Handle))
							{
								EmptyClipboard();
								memcpy(gDst,&bgPal[palBank*16+palActive*4],4);

								SetClipboardData(CF_SUBPAL,hDst);
								CloseClipboard();
							}
						}
					}
				}


				if(clickV)
				{
					OpenClipboard(Handle);

					HGLOBAL hClipBuf=GetClipboardData(CF_SUBPAL);

					if (hClipBuf)
					{
						LPSTR gSrc=(LPSTR)GlobalLock(hClipBuf);

						if (gSrc!=NULL)
						{
							SetUndo();
							memcpy(&bgPal[palBank*16+palActive*4],gSrc,4);
							GlobalUnlock(gSrc);
						}
					}
					HGLOBAL hClipBuf2=GetClipboardData(CF_PAL);

					if (hClipBuf2)
					{
						LPSTR gSrc=(LPSTR)GlobalLock(hClipBuf2);

						if (gSrc!=NULL)
						{
							SetUndo();
							memcpy(&bgPal[palBank*16+palActive*4],gSrc+cf_palRotation,4);
							GlobalUnlock(gSrc);
							cf_palRotation+=4;
							if (cf_palRotation>12) cf_palRotation=0;
						}
					}
					CloseClipboard();
					UpdateAll();
				}
			}
		}
	}

	if(PageControlEditor->ActivePage==TabSheetSprite)
	{
		for(i=0;i<ListBoxSpriteList->Items->Count;++i)
		{
			if(ListBoxSpriteList->Selected[i])
			{
				off=metaSpriteActive*64*4+i*4;

				if(metaSprites[off]<255) metaSprites[off+2]=(metaSprites[off+2]&~3)+palActive;
			}
		}

		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	}

	if(Shift.Contains(ssRight))
	{
		if(n>=0)
		{
			if(n&0x100)
			{
				palDragColor=n&0xff;
				acceptPalDrag=false;
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
					UpdateAll();

				}
				else if(Shift.Contains(ssShift))
						{
							palActive++;
							palActive=palActive&3; UpdateAll();
						}
			}
			else
			{
			palDragColor=bgPal[palBank*16+(n&3)*4+(n>>4)];
			palDragVal=n;
			acceptPalDrag=true;

			}
			PaintBoxPal->BeginDrag(false,-1);
		}
	}

	pal_validate();

	DrawPalettes();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);

	FormNavigator->Draw(false,false);  //bookmark: maybe replace with colour remapper rather than redrawer. maybe change working format to 8bit indexed altogether. 
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;

}


int __fastcall TFormMain::GetNameTableFrame(void)
{
	AnsiString str;
	char strid[16];
	int frame;

	str=RemoveExt(OpenDialogName->FileName);
	str=str.SubString(str.Length()-3,4);

	strcpy(strid,str.c_str());

	if(strid[0]!='_') return -1;

	if(strid[1]<'0'||strid[1]>'9') return -1;
	if(strid[2]<'0'||strid[2]>'9') return -1;
	if(strid[3]<'0'||strid[3]>'9') return -1;

	frame=(strid[1]-'0')*100+(strid[2]-'0')*10+(strid[3]-'0');

	return frame;
}



void __fastcall TFormMain::ChangeNameTableFrame(int dir)
{
	unsigned char *buf;
	AnsiString filename;
	char namestr[1024];
	int frame;

	frame=GetNameTableFrame();

	if(frame<0) return;

	frame+=dir;

	if(frame<0) frame=0;
	if(frame>999) frame=999;

	filename=RemoveExt(OpenDialogName->FileName);
	filename=filename.SubString(1,filename.LastDelimiter("_"));

	sprintf(namestr,"%s%3.3i.nam",filename.c_str(),frame);

	buf=(unsigned char*)malloc(name_size()+attr_size());

	memcpy(buf,nameTable,name_size());
	memcpy(buf+name_size(),attrTable,attr_size());

	if(OpenNameTable(namestr))
	{
		//save previous frame

		save_data((RemoveExt(OpenDialogName->FileName)+".nam").c_str(),buf,name_size()+attr_size(),SAVE_FORMAT_BIN,false);

		//remember new file name

		OpenDialogName->FileName=RemoveExt(namestr);
		SaveDialogName->FileName=OpenDialogName->FileName;
		SaveDialogMap ->FileName=OpenDialogName->FileName;
	}

	free(buf);
}

//---------------------------------------------------------------------------


void __fastcall TFormMain::ImageTilesMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int i,off;
	int tx=X/(8*uiScale);
	int ty=Y/(8*uiScale);
	bool bQuick = FormLineDetails->btnQuick->Down;

	if(MCHRFindDoubles->Checked)
	{
		MCHRFindDoubles->Checked=false;
		MCHRFindDoubles->Caption="Find &duplicates";
	}

	if(MCHRFindUnused->Checked)
	{
		MCHRFindUnused->Checked=false;
		MCHRFindUnused->Caption="Find &unused";
	}

	//---

	//added here in addition to keydown and keyup.
    //there was a context that wouldm't register properly without.
	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;
	if (bBufCtrl || bBufShift || bBufAlt)
	{
		//this is for updating when drag has been accepted.
		if (bImageNameAccepted) cueUpdateNametableNoRepaint=true;
		else if (bImageTileAccepted) 	cueUpdateTiles=true;
		//this is for any other case. Probably redundant for now.

		//else{cueUpdateNametable=true; cueUpdateTiles=true;}  //include if there´s a legitimate use.
	}
	//---
	
	txDown=tx;    //used for relative positioning when dragging selection, as well as performing positive selections to the left/up.
	tyDown=ty;
	//int tile=tx+ty*16;
	bOutsideSel=false;
	//if(abs(chrSelection.left-chrSelection.right)>1||abs(chrSelection.top-chrSelection.bottom)>1)
		//{
			if((chrSelection.right<=tx)
				|(chrSelection.left>tx)
				|(chrSelection.bottom<=ty)
				|(chrSelection.top>ty))
					bOutsideSel=true;
			if(!chrSelectRect&&tileActive!=ty*16+tx) bOutsideSel=true;

		//}

	if(Sender==ImageTiles&&PageControlEditor->ActivePage==TabSheetTile) return;

	if(IsBlockDrawing()) return;

	if(!(X>=0&&X<(128*uiScale)&&Y>=0&&Y<(128*uiScale))) return;

	tileSelRectWdt=1;
	tileSelRectHgt=1;

	isLastClickedMetaSprite=false;
	isLastClickedSpriteList=false;
	isLastClickedNametable=false;
	isLastClickedTiles=true;
	

	//handle draw mode
	if(((SpeedButtonDrawTile->Down)&&(!Shift.Contains(ssShift)))
		||((SpeedButtonDrawTile->Down)&&((Shift.Contains(ssShift)&&Shift.Contains(ssCtrl)))))
	{
		if(!Shift.Contains(ssRight)) SetUndo();

		lineDrawing = ((btnLine->Down)&& (!Shift.Contains(ssCtrl)) );
		if(lineDrawing && !bQuick) FormLineDetails->btnMove->Enabled=true;


		if((!lineDrawing) || !bQuick){
			memcpy (chrBuf, chr, 8192); //preps a referencepoint for brushstrokes
			memcpy (chrBufFill, chr, 8192); //preps a reference point for inc/dec fill
		}
        if(bQuick || (Shift.Contains(ssCtrl)&&!bQuick))
		{
		   if(lineUpX<0) memcpy (chrBuf, chr, 8192);
		   else memcpy (chr, chrBuf, 8192);       //chrQuickLine
		   memcpy (chrBufFill, chr, 8192);
		}
        if(bQuick)
		{
			bOldLine=true;   //user wants to retouch the position of old line
			 if(lineUpX<0) lineUpX= X;
			 if(lineUpY<0) lineUpY= Y;
			lineDownX=lineUpX;
			lineDownY=lineUpY;
		}
        else{

			bOldLine=false;
			if(bBufVK_3) memcpy(arr_linePlotCHR2,arr_linePlotCHR,sizeof(arr_linePlotCHR2));
			else memset (arr_linePlotCHR2,0,sizeof(arr_linePlotCHR2));
			lineDownX=X;
			lineDownY=Y;
		}
        lineToolAlt=0;
		lineToolX=0;
        lineToolY=0;
		bForbidPaint=false;
		lineDrawing = ((btnLine->Down)&&(!Shift.Contains(ssCtrl)));
		if(!lineDrawing) {lineUpX= -1; lineUpY= -1;}
		if(FormLineDetails->CheckResetLineNudge){
			lineOffX=0;
			lineOffY=0;
		}
		if(FormLineDetails->btnResetLine->Down)
		{
			lineToolRoster=0;
			

			for (int i = 0; i < 3; i++) {
				if (lineRosterEnable[lineToolRoster]==false) lineToolRoster++;
			}
			if (lineToolRoster > 3) lineToolRoster = 3;
		}




		if(bSmudge ||(bBufCtrl && bBufAlt))   //smudge or noncontiguous fill
		{
			int py = (Y/uiScale)%8; 	//only used by fill.
			int px = (X/uiScale)%8; 	//which pixel in 8x1 sliver
			if(FormCHREditor->btnQuant->Down){px=px&14;py=py&14;}

			int pp=tileViewTable[tyDown*16+txDown]*16 +py;
			iSmudge=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);
		}
		ImageTilesMouseMove(Sender,Shift,X,Y);
	}
    else{lineUpX= -1; lineUpY= -1;}


	//handle layout mode (+ shift modifier in draw mode)
	if((SpeedButtonMarqTile->Down)||((Shift.Contains(ssShift))))    //&&(!Shift.Contains(ssCtrl))
	{
		//multi-select method
		if(Shift.Contains(ssCtrl)&&Shift.Contains(ssLeft))
		{
			bMultiSelectRemoveMode=chrSelected[Y/(8*uiScale)*16+X/(8*uiScale)];
			bool bTmp = bMultiSelectRemoveMode;
			chrSelected[Y/(8*uiScale)*16+X/(8*uiScale)]=Shift.Contains(ssLeft)?!bTmp:bTmp;
			//chrSelected[Y/(8*uiScale)*16+X/(8*uiScale)]=(!chrSelected[Y/(8*uiScale)*16+X/(8*uiScale)]);
			chrSelectRect=false;

			UpdateTiles(true);
			if(FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();
		}
		//tile + box select method
		else if(Shift.Contains(ssLeft)||(bOutsideSel))
		{

			SelectTile(ty*16+tx);
			bool b=FormCHREditor->btn2x2mode->Down;
			if(b)
			{
				if((tileActive&15)==15) tileActive--;
				if(tileActive>=0xF0) tileActive-=16;
				
			}
		    SetTile(tileActive);
			UpdateTiles(true);
			if(FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();
		}
		else if(Shift.Contains(ssRight)||(!bOutsideSel)){


			chrSelBuf.left		=	chrSelection.left;
			chrSelBuf.top		=	chrSelection.top;
			chrSelBuf.right		=	chrSelection.right;
			chrSelBuf.bottom  	=	chrSelection.bottom;

			destRect.left		=	chrSelection.left;
			destRect.top		=	chrSelection.top;
			destRect.right		=	chrSelection.right;
			destRect.bottom  	=	chrSelection.bottom;

		UpdateTiles(true);
        if(FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();
		}
				
		if(SpeedButtonTypeIn->Down)
		{
			NameTableTypeIn(tileViewTable[tileActive]);
		}
		else
		{
			int exceptionSize =FormCHREditor->btn2x2mode->Down? 2:1;
			if(   abs(chrSelection.left-chrSelection.right)!=exceptionSize
				||abs(chrSelection.top-chrSelection.bottom)!=exceptionSize
				||Shift.Contains(ssCtrl)
                ||Shift.Contains(ssShift)
				)
			{
				nameSelection.left=-1;
				nameSelection.top=-1;

				UpdateTiles(true);
			}
		}
		//---- QuickPaste for CHR
		if(Shift.Contains(ssLeft)) if(clickV)
		{
			PasteCHR();
			//UpdateStats();
			cueStats=true;
			return;
		}
		//----

		UpdateNameTable(-1,-1,true);
		if((FormNavigator->Visible)&&(SpeedButtonSelTiles->Down)) FormNavigator->Draw(false,false);
		//UpdateStats();
		cueStats=true;

		if(PageControlEditor->ActivePage==TabSheetName)
		{
			if(Shift.Contains(ssRight)&&!Shift.Contains(ssShift))
			{
				if(bOutsideSel) SetTile(ty*16+tx);
				ImageTiles->BeginDrag(false,-1);
			}
		}
		if(PageControlEditor->ActivePage==TabSheetTile&&Sender==ImageBigTiles)
		{
			if(Shift.Contains(ssRight)&&!Shift.Contains(ssShift))
			{
				if(bOutsideSel) SetTile(ty*16+tx);
				ImageBigTiles->BeginDrag(false,-1);
			}
		}
		if(PageControlEditor->ActivePage==TabSheetSprite)
		{
			if(Shift.Contains(ssRight)&&!Shift.Contains(ssShift))
			{
				if(bOutsideSel) SetTile(ty*16+tx);
				ImageTiles->BeginDrag(false,-1);
                
			}

			if(!Shift.Contains(ssRight))
			{
				if(spriteActive>=0)
				{
					off=metaSpriteActive*64*4+spriteActive*4;

					if(metaSprites[off]<255)
					{
						SetUndo();

						if(!SpeedButtonSprite8x16->Down)
						{
							metaSprites[off+1]=tileActive;
						}
						else
						{
							//metaSprites[off+1]=(tileActive&0xfe)|(metaSprites[off+1]&1);
							metaSprites[off+1]=(tileViewTable[tileActive]&0xfe)|(metaSprites[off+1]&1);
						}

						//UpdateMetaSprite();
						cueUpdateMetasprite=true;
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonGridAllClick(TObject *Sender)
{
	if(((TSpeedButton*)Sender)->GroupIndex==10)
	{
		//repurposed as auto grids on/off. structure kept for easy reversal.
		//SpeedButtonGridTile->Down=SpeedButtonGridAll->Down;
		//SpeedButtonGridAtr->Down=SpeedButtonGridAll->Down;
		//SpeedButtonGridBlock->Down=SpeedButtonGridAll->Down;

	   /*if(!SpeedButtonGridTile->Down&&!SpeedButtonGridAtr->Down&&!SpeedButtonGridBlock->Down)
		{
			SpeedButtonGridTile->Down=true;
			SpeedButtonGridBlock->Down=true;
		}*/
	}
	else
	{

		//SpeedButtonGridAll->Down=(SpeedButtonGridTile->Down||SpeedButtonGridAtr->Down||SpeedButtonGridBlock->Down);
	}
	//SpeedButtonGridAll->Caption=SpeedButtonGridAll->Down?"on":"off";

	//if(!SpeedButtonGridTile->Down||!SpeedButtonGridAtr->Down||!SpeedButtonGridBlock->Down) SpeedButtonGridAll->Down=false;
	//if(SpeedButtonGridTile->Down&&SpeedButtonGridAtr->Down&&SpeedButtonGridBlock->Down) SpeedButtonGridAll->Down=true;

	GridOnOff1->Checked=SpeedButtonGridAll->Down;
	GridOnOff2->Checked=SpeedButtonGridAll->Down;

	Screen32x301->Checked=SpeedButtonGridScreen->Down;


	Tilegrid1->Checked=SpeedButtonGridTile->Down;
	Tilegrid2->Checked=SpeedButtonGridAtr->Down;
	Tilegrid4->Checked=SpeedButtonGridBlock->Down;

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHROpenClick(TObject *Sender)
{
	if(!OpenDialogChr->Execute()) return;

	BlockDrawing(true);

	if(OpenCHR(OpenDialogChr->FileName))
	{
		OpenDialogChr->FileName=RemoveExt(OpenDialogChr->FileName);

		SaveDialogChr->FileName=OpenDialogChr->FileName;
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxNamePaint(TObject *Sender)
{
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
}
//---------------------------------------------------------------------------

bool __fastcall TFormMain::TestAutoDraw(void) //int nYC, int nXC
{
  //char freeDrawCHRbuf[16];
	int t=nameTable[nameYC*nameTableWidth+nameXC]; //?????

	int i,j,k;
	int usedOnce=0;				//used to see if we can draw on current tile
	int isTilenameVacant;			//1/2 condition to assign a new tilename to the place
	bool empty;			//2/2 condition to assign a new tilename to the place
	int tileExamined=255;

	for(j=0;j<nameTableWidth*nameTableHeight;++j)
		if(nameTable[j]==t) {++usedOnce; if (usedOnce>1) break;}

	for(j=0;j<256*64*4;j+=4) 
		if(metaSprites[j+1]==t) {++usedOnce; if (usedOnce>1) break;}

	if (usedOnce==1) return true;  //true
	else 	//if tile has only one usage, we can just draw on it as usual
	{
		/* todo: find a match between a proposed new tile and the existing set

		//memcpy (freeDrawCHRbuf, chr[t], 16)
		//-do the draw action to the freedrawbuf
		//-match buf with tileset. (find duplicate)

		*/

		


		for (i=0;i<256;++i)
		{


			empty=true;

			for (j=0;j<16;++j)
			{
				if (chr[bankActive+(tileExamined*16)+j]>0)
				{
					empty=false;
					break;
				}
			}

			if (empty==true) //examined tile has no content
			{
				isTilenameVacant=0; 	
				
				

				for(k=0;k<nameTableWidth*nameTableHeight;k++) // check BG usage
					{
						if (isTilenameVacant>0) break;
						if(nameTable[k]==tileExamined) ++isTilenameVacant;
					}

				for(k=0;k<256*64*4;k+=4) // check sprite usage
					{
						if (isTilenameVacant>0) break;
						if(metaSprites[k+1]==tileExamined) ++isTilenameVacant;
					}
				if (isTilenameVacant==0)
				{
					nameTable[nameYC*nameTableWidth+nameXC]=tileExamined;

					return true;
				}
			}
			--tileExamined;
		}

		//no acceptable solution found

		LabelStats->Caption="Warning: Could not find an available (empty and unused) tile.\nTip: removing metasprites, duplicates and unused tiles may help.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
		return false;
	}

}


void __fastcall TFormMain::ImageNameMouseDown(TObject *Sender, TMouseButton Button,
TShiftState Shift, int X, int Y)
{
	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;

	int i,nx,ny;
	if(IsBlockDrawing()) return;
	bOutsideSel=false;
	//mouseDraggedNTSel=false;
	//mouseDraggedTileSel=false;
	nx=X/(8*uiScale)+nameTableViewX;
	ny=Y/(8*uiScale)+nameTableViewY;
	nxDown=nx;    //used for relative positioning when dragging selection, as well as performing positive selections to the left/up.
	nyDown=ny;
	if(nameXC<0||nameXC>=nameTableWidth||nameYC<0||nameYC>=nameTableHeight) return;
    FormNavigator->Map1->Picture->Bitmap->Assign(bufBmp);
	isLastClickedMetaSprite=false;
	isLastClickedSpriteList=false;
	isLastClickedNametable=true;
	isLastClickedTiles=false;

	if(Shift.Contains(ssLeft)) if(MouseTypeIn(X,Y)) return;

	if(Shift.Contains(ssLeft)&&(clickV))
	{
        SetUndo();
		//set selection
		nameSelection.left  =nx;
		nameSelection.top   =ny;
		nameSelection.right =nameSelection.left;
		nameSelection.bottom=nameSelection.top;

		for(i=0;i<256;++i) chrSelected[i]=0;

		chrSelected[tileActive]=1;
		chrSelectRect=true;
		PasteMap();
		//deselect
		nameSelection.left=-1;
		nameSelection.top =-1;

		cueStats=true;
		//FormNavigator->Draw(false,false);
		FormNavigator->UpdateLines(false);

		return;
	}

	//draw mode stuff
	if(((SpeedButtonDrawTile->Down)&&(!Shift.Contains(ssShift)))
		||((SpeedButtonDrawTile->Down)&&((Shift.Contains(ssShift)&&Shift.Contains(ssCtrl)))))
	{
		if(!Shift.Contains(ssRight)) SetUndo();
		memcpy (chrBuf, chr, 8192); //preps a referencepoint for brushstrokes
		memcpy (chrBufFill, chr, 8192); //preps a reference point for inc/dec fill
		TimerNTstrip->Enabled=true;

		if(bSmudge ||(bBufCtrl && bBufAlt))   //smudge or noncontiguous fill
		{
			int py = (Y/uiScale)%8; 	//only used by fill.
			int px = (X/uiScale)%8; 	//which pixel in 8x1 sliver
			if(FormCHREditor->btnQuant->Down){px=px&14;py=py&14;}

			int pp=nameTable[nyDown*nameTableWidth+nxDown]*16 +py;
			iSmudge=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);
		}

		ImageNameMouseMove(Sender,Shift,X,Y);
		return;
	}

	if (Shift.Contains(ssRight)) { //prep context for inside/outside selection
		SelectTile(nameTable[nameYC*nameTableWidth+nameXC]);
		SelectPalette(AttrGet(nameXC,nameYC,false,false));
        UpdateTiles(true);
		if((nameSelection.right<=nx)
				|(nameSelection.left>nx)
				|(nameSelection.bottom<=ny)
				|(nameSelection.top>ny))
		{
					bOutsideSel=true;
					nameSelection.left=-1;
					nameSelection.top =-1;



		}
		else{
			if(!Shift.Contains(ssShift) && nameSelection.left != -1 && nameSelection.top != -1) ImageName->BeginDrag(false,-1);

			nameSelBuf.left		=	nameSelection.left;
			nameSelBuf.top		=	nameSelection.top;
			nameSelBuf.right	=	nameSelection.right;
			nameSelBuf.bottom  	=	nameSelection.bottom;
			//these are probably redundant
			destRect.left		=	nameSelection.left;
			destRect.top		=	nameSelection.top;
			destRect.right		=	nameSelection.right;
			destRect.bottom  	=	nameSelection.bottom;

			

		}
		UpdateNameTable(-1,-1,true);
		//FormNavigator->Draw(false,false);
		FormNavigator->UpdateLines(true);
	}

	else if(Shift.Contains(ssShift)&&Shift.Contains(ssLeft))   //begin selection
	{
		nameSelection.left  =nx;
		nameSelection.top   =ny;
		nameSelection.right =nameSelection.left+1;
		nameSelection.bottom=nameSelection.top +1;

		
		/*
		chrSelection.right =chrSelection.left+1;
		chrSelection.bottom=chrSelection.top +1;

		for(i=0;i<256;++i) chrSelected[i]=0;

		chrSelected[tileActive]=1;
		chrSelectRect=true;

		tileSelRectWdt=1;
		tileSelRectHgt=1;
        */
		UpdateTiles(true);
		UpdateNameTable(-1,-1,true);
		//FormNavigator->Draw(false,false);
		FormNavigator->UpdateLines(false);
	}

	else
	{
		if(Shift.Contains(ssLeft)) SetUndo();        //place tile

		ImageNameMouseMove(Sender,Shift,X,Y);

		if(nameSelection.left>=0||nameSelection.top>=0)      
		{
			nameSelection.left=-1;
			nameSelection.top =-1;

			UpdateNameTable(-1,-1,true);
            FormNavigator->UpdateLines(false);
			//int dx,dy,dw,dh;
			//GetSelection(chrSelection,dx,dy,dw,dh);
			//FormNavigator->DrawRange(dx,dy,dw,dh);
			//FormNavigator->Draw(false,false);
		}
	}




	cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageNameMouseMove(TObject *Sender, TShiftState Shift,
int X, int Y)
{
	int i,j,dx,dy,xc,yc;
	int xmin,ymin,xmax,ymax;

    int ix,iy; //pixel modifiers for thick pen
	int gx,gy,g; //guards for thick pen
	int iThick=FormCHREditor->btnThick->Down?4:1;

	TRect r;
	if(IsBlockDrawing()) return;



	xc=X/(8*uiScale);
	yc=Y/(8*uiScale);

	nameXC=xc+nameTableViewX;
	nameYC=yc+nameTableViewY;

	if(xc<0||xc>=viewPortWidth||yc<0||yc>=viewPortHeight||nameXC<0||nameXC>=nameTableWidth||nameYC<0||nameYC>=nameTableHeight)
	{
		nameXC=-1;
		nameYC=-1;

		cueStats=true;
		return;
	}

	if(Shift.Contains(ssLeft)) if(MouseTypeIn(X,Y)) return;
	if(Shift.Contains(ssLeft)) if(clickV)return;
	if(!FormMain->Active) return;

	///test
	if(Sender==ImageTiles&&PageControlEditor->ActivePage!=TabSheetTile) return;
	if(IsBlockDrawing()) return;

	if(
		(!Shift.Contains(ssShift)||(Shift.Contains(ssShift)&&Shift.Contains(ssCtrl)))
			&&(SpeedButtonDrawTile->Down&(X>=0&&X<(256*uiScale)&&Y>=0&&Y<(256*uiScale))))
	{
		int px,py,pp,extpp,mask,tempPal;

		if(Shift.Contains(ssLeft))
		{
			if((SpeedButtonAutocreate->Down==true)&&(!TestAutoDraw())) return;


			py = (Y/uiScale)%8; 	//only used by fill.
			px = (X/uiScale)%8; 	//which pixel in 8x1 sliver
			//pp = (Y/uiScale)%8 	+(X/(8*uiScale))*16 	+(Y/(8*uiScale))*256;
			//	^-which sliver		^-which column	 		^-which row
			if(FormCHREditor->btnQuant->Down){px=px&14;py=py&14;}
			for(int i=0; i<iThick; i++)
				{
                if(FormCHREditor->btnThick->Down)
				{
					if(bBrushMask[i]==false) continue;
					//todo: if introducing more brush sizes, use an xy array for their masks instead of this.
					if(i==0){ ix=0; iy=0;}
					if(i==1){ ix=1; iy=0;}
					if(i==2){ ix=0; iy=1;}
					if(i==3){ ix=1; iy=1;}

					py 	= (Y/uiScale)%8;      //which sliver
					px 	= (X/uiScale)%8;      //which pixel in 8x1 sliver

					if(FormCHREditor->btnQuant->Down){
						px=px&6;
						py=py&6;
					}
                    if(px+ix<8) px=px+ix;
					if(py+iy<8) py=py+iy;

					//"guard dog" for brush field crossing the tile border
					gx=((X/uiScale)%8); gx+=ix;
					gy=((Y/uiScale)%8); gy+=iy;

					g=7;

					if(!FormCHREditor->btnQuant->Down){
						if((i==1||3)&&gx>g) continue;
						if((i==2||3)&&gy>g) continue;
						//if((i==1||3)&&gx>g) {tx++; px-=7;}
						//if((i==2||3)&&gy>g) {ty++; py-=7;}
					}
				}

				extpp = nameTable[nameYC*nameTableWidth+nameXC]*16;  //the value held by nameTable[] may be altered by TestAutoDraw()
				pp = extpp + py;

				mask=128>>px;

				tempPal=(((chrBuf[pp]<<px)&128)>>7)|(((chrBuf[pp+8]<<px)&128)>>6);
				fillPal=(((chrBufFill[pp]<<px)&128)>>7)|(((chrBuf[pp+8]<<px)&128)>>6);
				if(Applytopen2->Checked){
					if(FormCHREditor->Protect0->Down && tempPal==0) continue;
					if(FormCHREditor->Protect1->Down && tempPal==1) continue;
					if(FormCHREditor->Protect2->Down && tempPal==2) continue;
					if(FormCHREditor->Protect3->Down && tempPal==3) continue;
				}
				if(IncDecPerclick1->Checked==false)
				{
					tmpContinousIncDecTimer++;
					if (tmpContinousIncDecTimer>continousIncDecDuration)
					{
						tmpContinousIncDecTimer=0;
						memcpy (chrBuf, chr, 8192);
					}

				}

				if(!Shift.Contains(ssCtrl))
				{
					if (penActive==0&&(!Shift.Contains(ssShift)))
					{
                        if(!bSmudge){
								if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
								if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
							}
							else{
								if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((iSmudge&1)<<7)>>px);
								if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((iSmudge&2)<<6)>>px);
							}
					}
					if (penActive==1&&(!Shift.Contains(ssShift)))
					{
						if(!Shift.Contains(ssAlt))tempPal++;
						else tempPal--;

						if(IncDecCap1->Checked==true)
						{
							if (tempPal >3) tempPal = 3;
							if (tempPal <0) tempPal = 0;
						}


						if(FormCHREditor->ButtonBitmaskLo->Down||!Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((tempPal&1)<<7)>>px);
						if(FormCHREditor->ButtonBitmaskHi->Down||!Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((tempPal&2)<<6)>>px);

					}
					if (penActive==2&&(!Shift.Contains(ssShift)))
					{
						if(!Shift.Contains(ssAlt))tempPal--;
						else tempPal++;

						if(IncDecCap1->Checked==true)
						{
							if (tempPal >3) tempPal = 3;
							if (tempPal <0) tempPal = 0;
						}

						if(FormCHREditor->ButtonBitmaskLo->Down||!Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((tempPal&1)<<7)>>px);
						if(FormCHREditor->ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((tempPal&2)<<6)>>px);
					}
				}
				else
				{
					FormCHREditor->Fill(Shift,px,py,extpp,true); //tempPal,fillPal
				}
			}   //<--this loop end works for drawing just one tile. This method (nametable painting) currently doesn´t allow brushes crossing tile borders

			//remove doubles to current tile
			if(SpeedButtonAutocreate->Down)       
			{
				bool clear[256];  int a;
				int curtile=nameTable[nameYC*nameTableWidth+nameXC];
				//for(i=curtile+1;i<256;i++)
				for(i=256; i-- > curtile+1;)
				{
					if(!memcmp(&chr[bankActive+curtile*16],&chr[bankActive+i*16],16))
					{
						//clear[i]=true;
						//for (int k=0;k<16;k++) chr[16*i+bankActive+k]=0;
						//for(j=0;j<nameTableWidth*nameTableHeight;j++) if(nameTable[j]==i) nameTable[j]=curtile;
						//for(j=0;j<256*64*4;j+=4) if(metaSprites[j+1]==i) metaSprites[j+1]=curtile;

						for (int k=0;k<16;k++) chr[16*curtile+bankActive+k]=0;
						for(j=0;j<nameTableWidth*nameTableHeight;j++) if(nameTable[j]==curtile) nameTable[j]=i;
						for(j=0;j<256*64*4;j+=4) if(metaSprites[j+1]==curtile) metaSprites[j+1]=i;
					}
				}
				//for(;a<4096;a++) if(clear[a/16]) chr[bankActive+a]=0;
			}
			
			//UpdateNameTable(-1,-1,true);
			//cueUpdateNametable=true;
			cueUpdateTiles=true;
			cueUpdateMetasprite=true;
			cueUpdateNTstrip=true;
			cueRemoveDoubles=true;
			//int nx=nameXC+nameTableViewX;
			//int ny=nameYC+nameTableViewY;

			int xl=(X/uiScale&0xFFFFFFF8)*uiScale;
			int yl=(Y/uiScale&0xFFFFFFF8)*uiScale;
			DrawTile(
				ImageName->Picture,xl,yl,
				nameTable[nameYC*nameTableWidth+nameXC],
				AttrGet(nameXC,nameYC,false,false),nameXC,nameYC,
				false,false,uiScale,false,false,false);
			//cueUpdateNT=true;
			//UpdateNameTable(-1,-1,true);

		}

		if(Shift.Contains(ssRight))
		{
            py = (Y/uiScale)%8; 	//only used by fill.
			px = (X/uiScale)%8; 	//which pixel in 8x1 sliver
			//pp = (Y/uiScale)%8 		+(X/(8*uiScale))*16 	+(Y/(8*uiScale))*256;
			//	^-which sliver		^-which column	 		^-which row
			extpp = nameTable[nameYC*nameTableWidth+nameXC]*16;
			pp = extpp + py;
			bgPalCur=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);
			FormMain->DrawPalettes();
		}
	cueStats=true;
	return;
	}

	//layout mode begins:

	if(Shift.Contains(ssShift))          
	{
		if(Shift.Contains(ssLeft))
		{



			if(nameXC<nxDown)   {nameSelection.left=nameXC+1-(nameXC<nameSelection.right?1:0);
				nameSelection.right=nxDown+1;
				}
			if(nameXC>=nxDown) {nameSelection.right =nameXC+(nameXC>=nameSelection.left?1:0);
				nameSelection.left=nxDown;
				}
			if(nameYC<nyDown)  {nameSelection.top=nameYC-(nameYC>=nameSelection.bottom ?1:0);
				nameSelection.bottom=nyDown+1;
			    }
			if(nameYC>=nyDown) {nameSelection.bottom=nameYC+(nameYC>=nameSelection.top ?1:0);
				nameSelection.top=nyDown;				  
				}
			//UpdateNameTable(-1,-1,true);
			//cueUpdateNametable=true;
			NameLinesTimer->Enabled=true;
			//TODO: bug. selection not persistent on Nav until mouse hovers
			mouseDraggedNTSel=true;
		}

		else if(Shift.Contains(ssRight))
		{
			if(bOutsideSel){
				nameSelection.left=-1;
				nameSelection.top =-1;
                NameLinesTimer->Enabled=true;
				//cueUpdateNametable=true;
				//UpdateNameTable(-1,-1,true);
			}
			else if(!Shift.Contains(ssCtrl))
			{
				nameSelection.left=nameSelBuf.left+nameXC-nxDown;
				nameSelection.right=nameSelBuf.right+nameXC-nxDown;
				nameSelection.top=nameSelBuf.top+nameYC-nyDown;
				nameSelection.bottom=nameSelBuf.bottom+nameYC-nyDown;

				int cAlignHeight = (viewPortHeight-nameTableHeight);
				int cAlignWidth = (viewPortWidth-nameTableWidth);
				if  (cAlignHeight<0) cAlignHeight=0;
				if  (cAlignWidth<0) cAlignWidth=0;
				

				for (i=0; i<32; i++)  //long enough loop - felt safer than while
				{
					if(nameSelection.left+nameTableViewX<0-cAlignWidth/2)   	{	nameSelection.left++;
													nameSelection.right++;}

					if( nameSelection.right-nameTableViewX>viewPortWidth-cAlignWidth/2)
												{	nameSelection.left--;
													nameSelection.right--;}

					if(nameSelection.top+nameTableViewY<0-cAlignHeight/2)   	{	nameSelection.top++;
													nameSelection.bottom++;}

					if(nameSelection.bottom-nameTableViewY>viewPortHeight-cAlignHeight/2)
												{	nameSelection.top--;
													nameSelection.bottom--;}
				}

				//cueUpdateNametable=true;
				NameLinesTimer->Enabled=true;
				cueUpdateTiles=true;
			}
		}
	    cueStats=true;
		return;
	}




	if(Shift.Contains(ssLeft)&&nameSelection.left<0&&nameSelection.top<0)
	{
		if(chrSelectRect)    //place tiles/attrs with chr box selection
		{
			for(i=0;i<chrSelection.bottom-chrSelection.top;++i)
			{
				for(j=0;j<chrSelection.right-chrSelection.left;++j)
				{
					dx=nameXC+j;
					dy=nameYC+i;

					if(dx>=0&&dx<nameTableWidth&&dy>=0&&dy<nameTableHeight)
					{
						if(SpeedButtonTiles->Down) nameTable[dy*nameTableWidth+dx]=tileViewTable[((chrSelection.top+i)<<4)+chrSelection.left+j];

						if(SpeedButtonPal->Down) AttrSet(dx,dy,palActive,false);

						UpdateNameTable(dx,dy,false);
					}
				}
			}
		}
		else    //place tiles / attrs with chr multi selection
		{
			xmin=16;
			ymin=16;
			xmax=0;
			ymax=0;

			//determine area
			for(i=0;i<16;++i)
			{
				for(j=0;j<16;++j)
				{
					if(!chrSelected[i*16+j]) continue;

					if(j<xmin) xmin=j;
					if(j>xmax) xmax=j;
					if(i<ymin) ymin=i;
					if(i>ymax) ymax=i;
				}
			}
			//place tiles
			for(i=ymin;i<=ymax;++i)
			{
				for(j=xmin;j<=xmax;++j)
				{
					if(!chrSelected[i*16+j]) continue;

					dx=nameXC+j-xmin;
					dy=nameYC+i-ymin;

					if(dx>=0&&dx<nameTableWidth&&dy>=0&&dy<nameTableHeight)
					{
						if(SpeedButtonTiles->Down) nameTable[dy*nameTableWidth+dx]=tileViewTable[i*16+j];

						if(SpeedButtonPal->Down) AttrSet(dx,dy,palActive,false);

						UpdateNameTable(dx,dy,false);
					}
				}
			}
		}

		UpdateNameTable(nameXC,nameYC,true);
	}
	

	else if(Shift.Contains(ssRight))
	{


			SelectTile(tileViewTable[nameTable[nameYC*nameTableWidth+nameXC]]);
			SelectPalette(AttrGet(nameXC,nameYC,false,false));

			UpdateTiles(true);
			UpdateNameTable(-1,-1,true);
			if(FormNavigator->Visible) FormNavigator->Draw(false,false);

	}

	//if (!(X % (4*uiScale))||!(Y % (4*uiScale))) //obsolete.
	cueStats=true;

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MOpenNameTableClick(TObject *Sender)
{
	BlockDrawing(true);
	
	if(OpenDialogName->Execute())
	{
		if(OpenNameTable(OpenDialogName->FileName))
		{
			OpenDialogName->FileName=RemoveExt(OpenDialogName->FileName);
			SaveDialogName->FileName=OpenDialogName->FileName;
			SaveDialogMap ->FileName=OpenDialogName->FileName;
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveNameTableCClick(TObject *Sender)
{
	AnsiString name,filter;
	unsigned char* buf;
	int size;
    
	BlockDrawing(true);

	SaveDialogName->FileName=RemoveExt(SaveDialogName->FileName);
	SaveDialogName->Title="Save nametable as C header";

	filter="C header (*.h)|*.h|Any file (*.*)|*.*";

	SaveDialogName->Filter=filter;

	if(SaveDialogName->Execute())
	{
		name=RemoveExt(SaveDialogName->FileName)+".h";

		if(FileExists(name))
		{
			if(Application->MessageBox(("File "+name+" already exists. Overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES)
			{
				BlockDrawing(false);
				return;
			}
		}

		buf=(unsigned char*)malloc(name_size()+attr_size());
		size=0;

		if(MSaveIncName->Checked)
		{
			memcpy(buf,nameTable,name_size());
			size+=name_size();
		}

		if(MSaveIncAttr->Checked)
		{
			memcpy(buf+size,attrTable,attr_size());
			size+=attr_size();
		}

		name=save_data(name.c_str(),buf,size,SAVE_FORMAT_H,MSaveRLE->Checked);

		free(buf);

		if(name!="") Application->MessageBox(name.c_str(),"Error",MB_OK);
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveNameTableBINClick(TObject *Sender)
{
	AnsiString name,filter;
	unsigned char* buf;
	int size; int f;
	bool b=false;

	BlockDrawing(true);

	SaveDialogName->FileName=RemoveExt(SaveDialogName->FileName);
	SaveDialogName->Title="Save as screen (nametable)";

	if(!MSaveRLE->Checked) {filter="Nametable (*.nam)|*.nam|Binary (*.bin)|*.bin|Assembly text (*.asm)|.asm|CA65 text (*.s)|*.s|C header (*.h)|*.h|RLE packed Nametable (*.nrle)|*.nrle|RLE asm text(*.asm)|.asm|RLE CA65 text(*.s)|*.s|RLE C header(*.h)|*.h";
		filter+="||Any file (*.*)|*.*";
	}
	else {
		filter="RLE packed Nametable (*.nrle)|*.nrle|RLE asm text(*.asm)|.asm|RLE CA65 text(*.s)|*.s|RLE C header(*.h)|*.h";
    	SaveDialogName->Title="Save as NESlib RLE packed screen (nametable)";
	}


	SaveDialogName->Filter=filter;

	if(SaveDialogName->Execute())
	{
		name=RemoveExt(SaveDialogName->FileName);

		if(!MSaveRLE->Checked){
			switch(SaveDialogName->FilterIndex)
			{
				case 1: name+=".nam";				f=SAVE_FORMAT_BIN; break;
				case 2: name+=".bin"; 				f=SAVE_FORMAT_BIN; break;
				case 3: name+=".asm";				f=SAVE_FORMAT_ASM; break;
				case 4: name+=".s";					f=SAVE_FORMAT_ASM; break;
				case 5: name+=".h";					f=SAVE_FORMAT_H;   break;
				case 6: name+=".nrle";	b=true;		f=SAVE_FORMAT_BIN; break;
				case 7: name+=".asm";	b=true;		f=SAVE_FORMAT_ASM; break;
				case 8: name+=".s";		b=true;		f=SAVE_FORMAT_ASM; break;
				case 9: name+=".h";		b=true;     f=SAVE_FORMAT_H;   break;

				default: name+=!MSaveRLE->Checked?".nam":".nrle"; f=SAVE_FORMAT_BIN;
			}
		}
		else {
             switch(SaveDialogName->FilterIndex)
			{
				case 1: name+=".nrle";	b=true;		f=SAVE_FORMAT_BIN; break;
				case 2: name+=".asm";	b=true;		f=SAVE_FORMAT_ASM; break;
				case 3: name+=".s";		b=true;		f=SAVE_FORMAT_ASM; break;
				case 4: name+=".h";		b=true;     f=SAVE_FORMAT_H;   break;

				default: name+=!MSaveRLE->Checked?".nam":".nrle"; f=SAVE_FORMAT_BIN;
		}}

		if(FileExists(name))
		{
			if(Application->MessageBox(("File "+name+" already exists. Overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES)
			{
				BlockDrawing(false);
				return;
			}
		}

		buf=(unsigned char*)malloc(1024);   //1 .nam
		size=0;
		int pp=0;
		int x = nameTableViewX;
        int y = nameTableViewY;
		int h = 30, w = 32;
		//if(nameTableHeight<=30) y=-1;

		if(nameTableHeight<32)
		{
			y = y + (32-nameTableHeight)/2;
		}
		int yBuf = y;

		if(MSaveIncName->Checked)
		{
			//memcpy(buf,nameTable,name_size());
			for(int i=0;i<h;i++)
			{
				for(int j=0;j<w;j++)
				{
					buf[pp]=nameTable[y*nameTableWidth+x+j];

				pp++;
				}
			y++;
			}
			size+=960;
		}

		if(MSaveIncAttr->Checked)
		{
			//memcpy(buf+size,attrTable,attr_size());
			y=yBuf;
			pp=0;
			for(int i=0;i<h;i++)
			{
				for(int j=0;j<w;j+=4)
				{
					//pp=y/4*((nameTableWidth+3)/4)+(x+j)/4;
					buf[size+pp]=attrTable[y/4*((nameTableWidth+3)/4)+(x+j)/4];
					//attrTable[y*nameTableWidth+x+j]
					//AttrGet(x+j,y,false,true);
					//attrTable[y/4*((nameTableWidth+3)/4)+(x+j)/4];
					//   //

				pp++;
				}
			y+=4;

			}
			size+=64;
		}

        if (f==SAVE_FORMAT_ASM) {
			if (byte1->Checked) {byte1bool=true; db1bool=false;}
			if (db1->Checked)   {byte1bool=false; db1bool=true;}
		}
		name=save_data(name.c_str(),buf,size,f,(MSaveRLE->Checked||b));

		free(buf);

		if(name!="") Application->MessageBox(name.c_str(),"Error",MB_OK);
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPaletteOpenClick(TObject *Sender)
{
	BlockDrawing(true);

	if(OpenDialogPal->Execute())
	{
		if(OpenPalette(OpenDialogPal->FileName))
		{
			OpenDialogPal->FileName=RemoveExt(OpenDialogPal->FileName);

			SaveDialogPal->FileName=OpenDialogPal->FileName;
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPaletteSaveClick(TObject *Sender)
{
	FILE *file;
	unsigned char pal[16];
	int i;
	AnsiString name;

	BlockDrawing(true);

	if(SaveDialogPal->Execute())
	{
		name=RemoveExt(SaveDialogPal->FileName)+".pal";

		if(!OverwritePrompt(name))
		{
			BlockDrawing(false);
			return;
		}

		pal_validate();

		for(i=0;i<4;i++)
		{
			pal[i+ 0]=bgPal[palBank*16+0*4+i];
			pal[i+ 4]=bgPal[palBank*16+1*4+i];
			pal[i+ 8]=bgPal[palBank*16+2*4+i];
			pal[i+12]=bgPal[palBank*16+3*4+i];
		}

		file=fopen(name.c_str(),"wb");

		if(file)
		{
			fwrite(pal,16,1,file);
			fclose(file);
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::PaintBoxPalPaint(TObject *Sender)
{
	DrawPalettes();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHREditorClick(TObject *Sender)
{
	FormCHREditor->Visible^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{

bool bDoUpdate;

	bBufCtrl=Shift.Contains(ssCtrl)?true:false;     //these are chiefly used for the new drag mode actions, but could be used to condense and detangle many mouse events.
	bBufShift=Shift.Contains(ssShift)?true:false;   //idea - let these help status readouts in finer context?
	bBufAlt=Shift.Contains(ssAlt)?true:false;
	if(Key==VK_OEM_3) bBufVK_3=true;



	if (bBufCtrl || bBufShift || bBufAlt)
	{
		//this is for updating when drag has been accepted.
		if (bImageNameAccepted) cueUpdateNametableNoRepaint=true;
		else if (bImageTileAccepted) 	cueUpdateTiles=true;
		//this is for any other case. Probably redundant for now.

		//else{cueUpdateNametable=true; cueUpdateTiles=true;}  //include if there´s a legitimate use.
	}


	if(!Shift.Contains(ssCtrl)&&!Shift.Contains(ssShift))
	{
		bool b=false;
		if(Key==VK_F1) {FormMain->PageControlEditor->ActivePageIndex=0; b=true;}
		if(Key==VK_F2) {FormMain->PageControlEditor->ActivePageIndex=1; b=true;}
		if(Key==VK_F3) {FormMain->PageControlEditor->ActivePageIndex=2; b=true;}
		if (b) {FormMain->PageControlEditorChange(Sender);}
        //*/

		if(Key==VK_PAUSE)
		{
			FormMain->AlphaBlend^=true;
			if (!Shift.Contains(ssShift))
			{
				FormCHREditor->AlphaBlend^=true;
				FormManageMetasprites->AlphaBlend^=true;
			}
		}


		if(!SpeedButtonTypeIn->Down||(nameSelection.left<0&&nameSelection.top<0)){
        	if(PageControlEditor->ActivePage==TabSheetName)
			{
				if(Key=='T') {bIgnoreKey=true; TypeInModeOnOff1Click(TypeInModeOnOff1);}
			}
		}
		if(!SpeedButtonTypeIn->Down)
		{
			if(PageControlEditor->ActivePage==TabSheetSprite)
			{

			   if(Key=='H') FlipMetaSprites(true,false);
			   if(Key=='V') FlipMetaSprites(false,true);
			   if(Key=='P') SBPriorityToggle1Click(Sender);
			}

			else     //hotkeys that are disabled when in sprite editing mode
			{
                if(PageControlEditor->ActivePage==TabSheetName)
				{
					if(Key=='V') clickV=true;
					if(Key=='C') clickC=true;

					if(Key=='H') Tilegrid1Click(Screen32x301);
				}
					if(Key=='J') Tilegrid1Click(Tilegrid1);
					if(Key=='K') Tilegrid1Click(Tilegrid2);
					if(Key=='L') Tilegrid1Click(Tilegrid4);
				
			}
			if(PageControlEditor->ActivePage==TabSheetTile)
			{
				if(!lineDrawing)
				{
					if(Key=='5') {Protect0->Down^=true; FormCHREditor->Protect0->Down = Protect0->Down;}
					if(Key=='6') {Protect1->Down^=true; FormCHREditor->Protect1->Down = Protect1->Down;}
					if(Key=='7') {Protect2->Down^=true; FormCHREditor->Protect2->Down = Protect2->Down;}
					if(Key=='8') {Protect3->Down^=true; FormCHREditor->Protect3->Down = Protect3->Down;}
					if(Key=='V') FormCHREditor->MirrorVertical();
					if(Key=='R') FormCHREditor->SpeedButtonRotateCWClick(Sender);//{Flip90(false); Rotate4tiles(false);}
					if(Key=='G') {btnSmudge->Down=true; FormMain->btnSmudge->Down=true; bSmudge=true;}
                    if(Key=='B') {FormCHREditor->btnThick->Down^=true; btnThick->Down = FormCHREditor->btnThick->Down; }
					if(Key=='N') {FormCHREditor->btnLine->Down^=true; btnLine->Down = FormCHREditor->btnLine->Down; }
					if(Key=='U') {FormCHREditor->btnQuant->Down^=true; btnQuant->Down = FormCHREditor->btnQuant->Down; }
					if(Key=='M') FormBrush->Show();
					if(Key=='K') FormLineDetails->btnSmear->Down^=true;
					if(Key=='K') FormLineDetails->btnQuick->Down^=true;
				}
			}
			else{
				if(Key=='R') Red1Click(Red1);
				if(Key=='G') Red1Click(Green1);
				if(Key=='B') Red1Click(Blue1);
				if(Key=='M') Red1Click(Gray1);
            }
			//if(Key=='F' && !(nameSelection.left<0&&nameSelection.top<0)) FillMap();

			if(Key=='X') Toggletileset1Click(Toggletileset1);
			if(Key=='A') Attributes1Click(Attributes1);
			if(Key=='S') SelectedOnly1Click(SelectedOnly1);
			if(Key=='W') ApplyTiles1Click(ApplyTiles1);
			if(Key=='E') ApplyAttributes1Click(ApplyAttributes1);
			if(Key=='D') Tilegrid1Click(GridOnOff1);
            
		}
	}
	//*/
	if(!(nameSelection.left<0&&nameSelection.top<0))     //Photoshop alias
	{
		if(Shift.Contains(ssAlt))
		{
			if(Key==VK_BACK) FillMap(false);
		}

	}
	if(Shift.Contains(ssCtrl))
	{
		if((Key=='1')&&(uiScale!=1)) {uiScale=1;   UpdateUIScale();}
	}

	if(SpeedButtonTypeIn->Down&&PageControlEditor->ActivePage==TabSheetName)
	{
		switch(Key)
			{
			case VK_ESCAPE:
				SpeedButtonTypeIn->Down=false;
				StaticTextFontOffset->Visible=false;
				TypeInModeOnOff1->Checked=false;
				break;
			}
		if(nameSelection.left>=0)
		{
			switch(Key)
			{
			case VK_BACK:
			case VK_LEFT:   --nameSelection.left; break;

			case VK_RIGHT:  ++nameSelection.left; break;

			case VK_UP:    	--nameSelection.top;  break;

			case VK_RETURN: nameSelection.left=returnCarriagePos;
							if(!Shift.Contains(ssShift)) ++nameSelection.top;
							else --nameSelection.top;
							break;

			case VK_DOWN:  	++nameSelection.top;  break;
			}

			if(nameSelection.left<0) nameSelection.left=0;
			if(nameSelection.left>=nameTableWidth) nameSelection.left=nameTableWidth-1;
			if(nameSelection.top<0) nameSelection.top=0;
			if(nameSelection.top>=nameTableHeight) nameSelection.top=nameTableHeight-1;

			nameSelection.right =nameSelection.left+1;
			nameSelection.bottom=nameSelection.top +1;

            //bookmark: should these be moved or cued?
			UpdateNameTable(-1,-1,false);
			FormNavigator->Draw(false,false);
			cueStats=true;
		}

		return;
	}
	else
	{
		if(Key==VK_ESCAPE) {
			if( FormNavigator->Active) {
				FormNavigator->Close();
			}
		}
    }

	if(!Shift.Contains(ssCtrl))
	{
		if(Key==VK_OEM_4||Key==VK_OEM_COMMA) {SpeedButtonPrevMetaSpriteClick(Sender);cueStats=true;}// [
		if(Key==VK_OEM_6||Key==VK_OEM_PERIOD) {SpeedButtonNextMetaSpriteClick(Sender);cueStats=true;}// ]
		if(Key=='Q') FormCHREditor->Show();
		if(Key==VK_NUMPAD7) FormCHREditor->TileChange(-1,-1);
		if(Key==VK_NUMPAD8) FormCHREditor->TileChange( 0,-1);
		if(Key==VK_NUMPAD9) FormCHREditor->TileChange(+1,-1);

		if(Key==VK_NUMPAD4) FormCHREditor->TileChange(-1,0);
		if(Key==VK_NUMPAD5) MCHREditorClick(Sender);
		if(Key==VK_NUMPAD6) FormCHREditor->TileChange(+1,0);

		if(Key==VK_NUMPAD1) FormCHREditor->TileChange(-1,+1);
		if(Key==VK_NUMPAD2) FormCHREditor->TileChange( 0,+1);
		if(Key==VK_NUMPAD3) FormCHREditor->TileChange(+1,+1);
	}

	if(PageControlEditor->ActivePage==TabSheetName)
	{
		if(Shift.Contains(ssCtrl))
		{
			if(Key==VK_OEM_4) {ChangeNameTableFrame(-1); cueStats=true;}// [
			if(Key==VK_OEM_6) {ChangeNameTableFrame(1); cueStats=true;}// ]

		}

		if(!Shift.Contains(ssAlt))
		{
			if(!Shift.Contains(ssCtrl))
			{
				bDoUpdate=false;
				if(Key==VK_LEFT)  {nameTableViewX-=4; bDoUpdate=true;}
				if(Key==VK_RIGHT) {nameTableViewX+=4; bDoUpdate=true;}
				if(Key==VK_UP)    {nameTableViewY-=4; bDoUpdate=true;}
				if(Key==VK_DOWN)  {nameTableViewY+=4; bDoUpdate=true;}

				if(bDoUpdate){
					CorrectView();
					UpdateNameTable(-1,-1,true);
					FormNavigator->Draw(false,false);
					cueStats=true;
					}
			}
			else
			{
				if(Key==VK_LEFT)  NameTableScrollLeft (Shift.Contains(ssShift));
				if(Key==VK_RIGHT) NameTableScrollRight(Shift.Contains(ssShift));
				if(Key==VK_UP)    NameTableScrollUp   (Shift.Contains(ssShift));
				if(Key==VK_DOWN)  NameTableScrollDown (Shift.Contains(ssShift));

			}
		}
		else
		{
			if(nameSelection.left>=0)
			{
				bDoUpdate=false;
				if(Key==VK_LEFT)
				{
					if(nameSelection.left>0)
					{
						--nameSelection.left;
						--nameSelection.right;
						bDoUpdate=true;
					}
				}

				if(Key==VK_RIGHT)
				{
					if(nameSelection.right<nameTableWidth)
					{
						++nameSelection.left;
						++nameSelection.right;
						bDoUpdate=true;
					}
				}

				if(Key==VK_UP)
				{
					if(nameSelection.top>0)
					{
						--nameSelection.top;
						--nameSelection.bottom;
						bDoUpdate=true;
					}
				}

				if(Key==VK_DOWN)
				{
					if(nameSelection.bottom<nameTableHeight)
					{
						++nameSelection.top;
						++nameSelection.bottom;
						bDoUpdate=true;
					}
				}

				if(bDoUpdate) {
					UpdateNameTable(-1,-1,false);
					FormNavigator->Draw(false,false);
					cueStats=true;
					}
			}
		}
	}
	else
	{
        int sprMov;
		if(Shift.Contains(ssShift)) sprMov=8; else sprMov=1;
		if(Key==VK_LEFT)  MoveSprite(-sprMov, 0);
		if(Key==VK_RIGHT) MoveSprite( sprMov, 0);
		if(Key==VK_UP)    MoveSprite( 0,-sprMov);
		if(Key==VK_DOWN)  MoveSprite( 0, sprMov);
		cueStats=true;
	}

	if (GetKeyState(VK_CAPITAL)&&(!SpeedButtonTypeIn->Down))
	{
		if (bAllowLockMessage) {
			AnsiString lockbuf;
			lockbuf=LabelStats->Caption;
			lockbuf+="\nStats locked. Press [CAPS LOCK] to unlock.";
			LabelStats->Caption=lockbuf;
			bAllowLockMessage=false;
			cueStats=true;
		}
	}
	


	//cueStats=true;

}

//---------------------------------------------------------------------------



void __fastcall TFormMain::ImageTilesDblClick(TObject *Sender)
{
	/*   //old behaviour - left in case i change my mind.
	if(SpeedButtonDrawTile->Down)
	{
	if (FormCHREditor->Visible) return;
	}
	FormCHREditor->Show();
	*/
	if(SpeedButtonDrawTile->Down) return;
	if(SpeedButtonTypeIn->Down) return;

	FormCHREditor->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
	delete BufBmpName;
	delete BufBmpTiles;
	free (chrBank);
    //delete[] chrBankNames;

	char path[MAX_PATH];
	int len;

	GetCurrentDirectory(len,path);
	reg_save_str(regWorkingDirectory,path);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageNameMouseLeave(TObject *Sender)
{

	//TODO: look into maxing the selection on the given axis here
	//here´s some copypasta to remember variable names by
	/*
	if(mouseDraggedNTSel==true)
	{

	nameSelection.right =nameXC+(nameXC>=nameSelection.left?1:0);
	nameSelection.bottom=nameYC+(nameYC>=nameSelection.top ?1:0);
	UpdateNameTable(-1,-1,true);
	oldNameXC=nameXC;
	oldNameYC=nameYC;
	}
    */

	nameXC=-1;
	nameYC=-1;
	bMouseOverNam=false;	    //used by DrawTile to identify the target of auto screen grid.
	mouseDraggedNTSel=false;    //redundancies
	mouseDraggedTileSel=false;
	UpdateNameTable(-1,-1,true);
	//UpdateStats();
	cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageTilesMouseLeave(TObject *Sender)
{
	tileXC=-1;
	tileYC=-1;

	
		bDrawDestShadow=false;
		cueUpdateTiles=true;
	
	mouseDraggedNTSel=false; 	//redundancies
	mouseDraggedTileSel=false;
	
	//UpdateStats();
	cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageTilesMouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{
	int i,j,tx,ty;
	int xs,ys,wdt,hgt;
	int ba=bankActive;

	tx=X/(8*uiScale);
	ty=Y/(8*uiScale);


	int ix,iy; //pixel modifiers for thick pen
	int gx,gy,g; //guards for thick pen
	int iThick=FormCHREditor->btnThick->Down?4:1;

	if(Sender==ImageTiles&&PageControlEditor->ActivePage==TabSheetTile) return;
	if(IsBlockDrawing()) return;

	if(
		(!Shift.Contains(ssShift)||(Shift.Contains(ssShift)&&Shift.Contains(ssCtrl)))
			&&(SpeedButtonDrawTile->Down&(X>=0&&X<(128*uiScale)&&Y>=0&&Y<(128*uiScale))))
	{
		int px,py,pp,extpp,mask,tempPal;

		py = (Y/uiScale)%8; 	//also used separately by fill.
		px = (X/uiScale)%8; 	//which pixel in 8x1 sliver
		//pp = py			 		+(X/(8*uiScale))*16 	+(Y/(8*uiScale))*256;
		//	^-which sliver		^-which column	 		^-which row

		extpp = tileViewTable[(X/(8*uiScale)) +(Y/(8*uiScale))*16]*16+ba;  //used for fill
		pp	   = extpp+py;  //used for pen

		if(FormCHREditor->btnQuant->Down){px=px&14;pp=pp&0x1FFE;}
		mask=128>>px;


		if(Shift.Contains(ssLeft))
		{

        if(lineDrawing)
		{
				 FormCHREditor->Line(Shift,lineDownX,lineDownY,X,Y,1);
		}
		else
		{
            //if there´s a selection larger than 1x1, only allow drawing within it.
			if(abs(chrSelection.left-chrSelection.right)>1||abs(chrSelection.top-chrSelection.bottom)>1)
			{
				if(chrSelectRect
					&((chrSelection.right<=tx)
					|(chrSelection.left>tx)
					|(chrSelection.bottom<=ty)
					|(chrSelection.top>ty)))
					return;
			}	

			for(int i=0; i<iThick; i++)
			 {


				if(FormCHREditor->btnThick->Down)
				{
					if(bBrushMask[i]==false) continue;
                    //todo: if introducing more brush sizes, use an xy array for the mask instead of this. 
					if(i==0){ ix=0; iy=0;}
					if(i==1){ ix=1; iy=0;}
					if(i==2){ ix=0; iy=1;}
					if(i==3){ ix=1; iy=1;}

					py 	= (Y/uiScale)%8;      //which sliver
					px 	= (X/uiScale)%8;      //which pixel in 8x1 sliver
					tx	= X/(8*uiScale);      //which column
					ty	= Y/(8*uiScale);	  //which row

                    if(FormCHREditor->btnQuant->Down){
						px=px&6;
						py=py&6;
					}

					if(px+ix<8) px=px+ix;
					if(py+iy<8) py=py+iy;

					//"guard dog" for brush field crossing the tile border
					gx=((X/uiScale)%8); gx+=ix;
					gy=((Y/uiScale)%8); gy+=iy;

					g=7;

					if(!FormCHREditor->btnQuant->Down){
						//if((i==1||3)&&gx>g) continue;
						//if((i==2||3)&&gy>g) continue;
						if((i==1||3)&&gx>g) {tx++; px-=7;}
						if((i==2||3)&&gy>g) {ty++; py-=7;}
					}


					//pp = py+tx*16+ty*256; //selects to which char to operate.
					pp = tileViewTable[tx+ty*16]*16+ba+py;
					mask=128>>px;         //used for selecting bits in char.

				}



			tempPal=(((chrBuf[pp]<<px)&128)>>7)|(((chrBuf[pp+8]<<px)&128)>>6);
			fillPal=(((chrBufFill[pp]<<px)&128)>>7)|(((chrBuf[pp+8]<<px)&128)>>6);
			if(Applytopen2->Checked){
					if(FormCHREditor->Protect0->Down && tempPal==0) continue;
					if(FormCHREditor->Protect1->Down && tempPal==1) continue;
					if(FormCHREditor->Protect2->Down && tempPal==2) continue;
					if(FormCHREditor->Protect3->Down && tempPal==3) continue;
			}
			if(IncDecPerclick1->Checked==false)
			{
				tmpContinousIncDecTimer++;
				if (tmpContinousIncDecTimer>continousIncDecDuration)
				{
					tmpContinousIncDecTimer=0;
					memcpy (chrBuf, chr, 8192);
				}

			}

			if(!Shift.Contains(ssCtrl))
			{
				if (penActive==0&&(!Shift.Contains(ssShift)))
				{
                    if(!bSmudge){
								if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
								if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
							}
							else{
								if(ButtonBitmaskLo->Down||!FormMain->Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((iSmudge&1)<<7)>>px);
								if(ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((iSmudge&2)<<6)>>px);
							}

					//if(FormCHREditor->ButtonBitmaskLo->Down||!Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((bgPalCur&1)<<7)>>px);
					//if(FormCHREditor->ButtonBitmaskHi->Down||!Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((bgPalCur&2)<<6)>>px);
				}
				if (penActive==1&&(!Shift.Contains(ssShift)))
				{
					if(!Shift.Contains(ssAlt))tempPal++;
					else tempPal--;

					if(IncDecCap1->Checked==true)
					{
						if (tempPal >3) tempPal = 3;
						if (tempPal <0) tempPal = 0;
					}


					if(FormCHREditor->ButtonBitmaskLo->Down||!Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((tempPal&1)<<7)>>px);
					if(FormCHREditor->ButtonBitmaskHi->Down||!Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((tempPal&2)<<6)>>px);

				}
					if (penActive==2&&(!Shift.Contains(ssShift)))
				{
					if(!Shift.Contains(ssAlt))tempPal--;
					else tempPal++;

					if(IncDecCap1->Checked==true)
					{
						if (tempPal >3) tempPal = 3;
						if (tempPal <0) tempPal = 0;
					}

					if(FormCHREditor->ButtonBitmaskLo->Down||!Applytopen1->Checked) chr[pp]=(chr[pp]&~mask)|(((tempPal&1)<<7)>>px);
					if(FormCHREditor->ButtonBitmaskHi->Down||!FormMain->Applytopen1->Checked) chr[pp+8]=(chr[pp+8]&~mask)|(((tempPal&2)<<6)>>px);

				}

			}
			else
			{
				FormCHREditor->Fill(Shift,px,py,extpp,false); //tempPal,fillPal
			}
			}
            }
			//Draw();

			//UpdateNameTable(-1,-1,true);
			//cueUpdateNametable=true;
			cueUpdateTiles=true;
			cueUpdateMetasprite=true;
			cueUpdateNTstrip=true;
		}

		if(Shift.Contains(ssRight))
		{
			bgPalCur=(((chr[pp]<<px)&128)>>7)|(((chr[pp+8]<<px)&128)>>6);
			FormMain->DrawPalettes();
		}

	//recalc stuff for the status readout.
	tx=X/(8*uiScale);
	ty=Y/(8*uiScale);
	tileXC=tx;
	tileYC=ty;
	cueStats=true;
	return;
	}


	if(SpeedButtonTypeIn->Down) return;

	if(Shift.Contains(ssLeft)&&!(Shift.Contains(ssShift)||Shift.Contains(ssCtrl)))
	{
        //this enables the user to "scan" tiles with left click + drag
		if(X>=0&&X<(128*uiScale)&&Y>=0&&Y<(128*uiScale))
		{
			tx=X/(8*uiScale);
			ty=Y/(8*uiScale);
			
			SetTile(tx+ty*16);
			cueUpdateNametable=true;
			
			cueCHRdrawAll=true;
		}
		else return; 
	}

	if((!Shift.Contains(ssShift))||(X>=0&&X<(128*uiScale)&&Y>=0&&Y<(128*uiScale)))
	{
		tx=X/(8*uiScale);
		ty=Y/(8*uiScale);
		tileXC=tx;
		tileYC=ty;

		if(Shift.Contains(ssShift)&&!Shift.Contains(ssCtrl))
		{
			//drag selection
			if(Shift.Contains(ssRight)&&Shift.Contains(ssShift)&&!bOutsideSel)
			{
		
				chrSelection.left=chrSelBuf.left+tx-txDown;
				chrSelection.right=chrSelBuf.right+tx-txDown;
				chrSelection.top=chrSelBuf.top+ty-tyDown;
				chrSelection.bottom=chrSelBuf.bottom+ty-tyDown;

				for (i=0; i<16; i++)  //long enough loop - felt safer than while
				{
					if(chrSelection.left<0)   	{	chrSelection.left++;
													chrSelection.right++;}
					if(chrSelection.right>0x10)	{	chrSelection.left--;
													chrSelection.right--;}
					if(chrSelection.top<0)   	{	chrSelection.top++;
													chrSelection.bottom++;}
					if(chrSelection.bottom>0x10){	chrSelection.top--;
													chrSelection.bottom--;}
				}
				cueUpdateTiles=true;
				cueUpdateNametable=true;


			}

			//box selection
			if(Shift.Contains(ssLeft))
				{
					mouseDraggedTileSel=true;
					mouseDraggedNTSel=true;


					if(tx<txDown) {chrSelection.left=tx+1-(tx<chrSelection.right?1:0);
								  chrSelection.right=txDown+1;
								  }
					if(tx>=txDown) {chrSelection.right =tx+(tx>=chrSelection.left?1:0);
								  chrSelection.left=txDown;

								  }

					if(ty<tyDown)  {chrSelection.top=ty-(ty>=chrSelection.bottom ?1:0);
								   chrSelection.bottom=tyDown+1;

								   }
					if(ty>=tyDown) {chrSelection.bottom=ty+(ty>=chrSelection.top ?1:0);
								   chrSelection.top=tyDown;
								  
								   }
				   tileActive=chrSelection.top*16+chrSelection.left;



					//note: this is also performed in mousedown
					int exceptionSize =FormCHREditor->btn2x2mode->Down? 2:1;
					if(   abs(chrSelection.left-chrSelection.right)!=exceptionSize
						||abs(chrSelection.top-chrSelection.bottom)!=exceptionSize)
                    {
						nameSelection.left=-1;
						nameSelection.top =-1;

						UpdateNameTable(-1,-1,true);  //bookmark: should this be cued?
					}
					

				
					
		}  //
		for(i=0;i<256;i++) chrSelected[i]=false;

					//some of this is probably unwarranted now that there´s no inverted selection weirdness anymore. Keeping it for redundancy.
					//--------------
					xs=chrSelection.left<chrSelection.right ?chrSelection.left:chrSelection.right;
					ys=chrSelection.top <chrSelection.bottom?chrSelection.top :chrSelection.bottom;

					//first check for negatives, later on we derive absolutes.
					wdt=(chrSelection.right -chrSelection.left);
					hgt=(chrSelection.bottom-chrSelection.top);

					//these are used by the new scroll/wrap tile routines, as well as the new drag-swap.
					tileSelRectWdt=abs(wdt);
					tileSelRectHgt=abs(hgt);

					//overwrite. similar to std::max except i did this instead.

					if (wdt<0)tileSelRectWdt=0;
					if (hgt<0)tileSelRectHgt=0;

					//these are used for the rest of nesst vanilla code below
					wdt=abs(wdt);
					hgt=abs(hgt);

					
					for(i=0;i<hgt;i++)
						{
						for(j=0;j<wdt;j++)
						{
							chrSelected[(i+ys)*16+j+xs]=true;
						}
					}
					chrSelectRect=true;
		}

		//multi-select by dragging
		if(Shift.Contains(ssCtrl)&&(Shift.Contains(ssLeft)||Shift.Contains(ssRight)))
		{
			bool bTmp = bMultiSelectRemoveMode;
			chrSelected[ty*16+tx]=Shift.Contains(ssLeft)?!bTmp:bTmp;
			chrSelectRect=false;

			if(SpeedButtonSelTiles->Down) UpdateNameTable(-1,-1,true);
		}
	 }
	 else
	 {
		tileXC=-1;
		tileYC=-1;
	 }

	 if (wdt!=oldCHRRectWdt || hgt!=oldCHRRectHgt)
	 {
		UpdateTiles(false);

	 }

	 FormCHRbit->BitBtnTimer->Enabled=true;
	 cueStats=true;

	 oldCHRRectWdt=wdt;
	 oldCHRRectHgt=hgt;


}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MCHRClearClick(TObject *Sender)
{
	int i;

   //if(Application->MessageBox("Do you really want to clear 8K CHR?","Confirm",MB_YESNO)==ID_YES)
   //{
		SetUndo();

		for(i=0;i<8192;i++) chr[i]=0;

		UpdateTiles(true);
		UpdateNameTable(-1,-1,true);
		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
		LabelStats->Caption="A/B Tilesets cleared! Undo set.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
	//}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonChrBank1Click(TObject *Sender)
{
	bankActive=SpeedButtonChrBank1->Down?0:4096;
	FormBankCHR->btnA->Down=SpeedButtonChrBank1->Down;
	FormBankCHR->btnB->Down=SpeedButtonChrBank2->Down;

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRRemoveDoublesClick(TObject *Sender)
{
	//FG: this, along with the other remove/sort actions, were modified to toggle between do/undo for the sake of fluidity.
	if(MCHRRemoveDoubles->Checked)
	{
		Undo();
		MCHRRemoveDoubles->Checked=false;
		MCHRRemoveDoubles->Caption="&Remove duplicates";
	}
	else
	{
		RemoveDoublesUnused(false);
		MCHRRemoveDoubles->Checked=true;
		MCHRRemoveDoubles->Caption="Undo &remove duplicates";
		LabelStats->Caption="Duplicates removed. \n\nTip: Press again to undo.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPalMouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{
	int n;

	palHover=-1;
	palColHover=-1;
	colHover=-1;

	n=check_palette_mouse(X,Y);

	if(n>=0)
	{
		if(n&0x100)
		{
			colHover=n&0xff;
		}
		else
		{
			palHover=n&3;
			palColHover=n>>4;
		}
	}

	//UpdateStats();
	cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPalMouseLeave(TObject *Sender)
{
	palHover=-1;
	palColHover=-1;
	colHover=-1;
	//UpdateStats();
	cueStats=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MCHRInterleaveClick(TObject *Sender)
{
	InterleaveCHR(true);

	LabelStats->Caption="CHR Interleaved.";
	StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRDeinterleaveClick(TObject *Sender)
{
	InterleaveCHR(false);

	LabelStats->Caption="CHR De-interleaved.";
	StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSwapColorsClick(TObject *Sender)
{
	bool swap[256*2];
	int i,j,k,pp,col,bit;
	int ib, is, bankOff;
	unsigned char paltemp[4];



	memcpy (chrBuf, chr, 4096*2);    //prep buffer so we can perform previews
	memcpy (palBuf, bgPal, 4*16);

	FormSwapColors->ShowModal();

	memcpy (chr, chrBuf, 4096*2);  // restore.
	memcpy (bgPal, palBuf, 4*16);

	if(FormSwapColors->Swap)
	{
		SetUndo();

		for(i=0;i<256*2;i++) swap[i]=false;

		if(FormSwapColors->Selection)
		{
			for(i=0;i<256;i++) swap[(bankActive/4096*256)+i]=chrSelected[i];
		}
		else
		{
			if(FormSwapColors->WholeCHR)
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
						col=FormSwapColors->Map[col];
						chr[pp]=(chr[pp]&~bit)|((col&1)<<k);
						chr[pp+8]=(chr[pp+8]&~bit)|((col>>1)<<k);
					}
					pp++;
				}
			}
		}

		if(FormSwapColors->RemapPalette)
		{
			if(FormSwapColors->RadioPalOne->Checked) {ib=iSwap_WhichSubpal; is=1; bankOff=palBank;}
					else if(FormSwapColors->RadioPalAll->Checked) {ib=0; is=4*4; bankOff=0;}
					else {ib=0;is=4;bankOff=palBank;}
					for(i=ib;i<is+ib;i++)
					{
						for(j=0;j<4;j++) paltemp[j]=bgPal[bankOff*16+i*4+FormSwapColors->Map[j]];
						for(j=0;j<4;j++) bgPal[bankOff*16+i*4+j]=paltemp[j];
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
    pal_validate();

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	if(FormNavigator->Visible) FormNavigator->Draw(false,false);
	DrawPalettes();
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;


}

//---------------------------------------------------------------------------

void __fastcall TFormMain::MAddOffsetClick(TObject *Sender)
{
	int i,j,x,y,w,h,off;

	FormNameOffset->Mode=0;
	FormNameOffset->ShowModal();

	if(FormNameOffset->MakeOffset)
	{
		if(FormNameOffset->SelectedOnly)
		{
			x=-1;

			if(nameSelection.left>=0&&nameSelection.top>=0)
			{
				x=nameSelection.left;
				y=nameSelection.top;
				w=nameSelection.right-nameSelection.left;
				h=nameSelection.bottom-nameSelection.top;
			}
		}
		else
		{
			x=0;
			y=0;
			w=nameTableWidth;
			h=nameTableHeight;
		}

		if(x>=0)
		{
			SetUndo();

			for(i=0;i<h;++i)
			{
				for(j=0;j<w;++j)
				{
					off=(y+i)*nameTableWidth+(x+j);

					if(nameTable[off]>=FormNameOffset->From&&nameTable[off]<=FormNameOffset->To) nameTable[off]+=FormNameOffset->Offset;
				}
			}

			UpdateNameTable(-1,-1,true);
			FormNavigator->Draw(false,false);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMaskBClick(TObject *Sender)
{
	ppuMask^=0x80;
	ppuMaskSet[palBank]=ppuMask;
	UpdateRGBM();
	palette_calc();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	DrawPalettes();
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMaskGClick(TObject *Sender)
{
	ppuMask^=0x40;
	ppuMaskSet[palBank]=ppuMask;
	UpdateRGBM();
	palette_calc();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	DrawPalettes();
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMaskRClick(TObject *Sender)
{
	ppuMask^=0x20;
	ppuMaskSet[palBank]=ppuMask;
	UpdateRGBM();
	palette_calc();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	DrawPalettes();
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMaskMClick(TObject *Sender)
{
	ppuMask^=0x01;
	ppuMaskSet[palBank]=ppuMask;
	UpdateRGBM();
	palette_calc();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	DrawPalettes();
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MExportNametableBMPClick(TObject *Sender)
{
	TPicture *picture;
	Graphics::TBitmap *bmp;
	int i,j,x,y;
	AnsiString str;

	BlockDrawing(true);
	
	str=RemoveExt(SaveDialogName->FileName);

	if(str=="") str="nametable";

	SaveDialogImage->FileName=str+".bmp";

	if(SaveDialogImage->Execute())
	{
		picture=new TPicture();
		bmp=new Graphics::TBitmap();
		bmp->SetSize(nameTableWidth*8,nameTableHeight*8);
		bmp->PixelFormat=pf4bit;
		SetBMPPalette(bmp);
		picture->Bitmap=bmp;

		y=0;

		for(i=0;i<nameTableHeight;i++)
		{
			x=0;

			for(j=0;j<nameTableWidth;j++)
			{
				DrawExportTile16(picture,x,y,nameTable[i*nameTableWidth+j],AttrGet(j,i,false,false),j,i,false);

				x+=8;
			}

			y+=8;
		}

		picture->SaveToFile(SaveDialogImage->FileName);

		delete bmp;
		delete picture;
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MExportTilesetBMPClick(TObject *Sender)
{
	TPicture *picture;
	Graphics::TBitmap *bmp;
	int i,x,y;
	AnsiString str;

	BlockDrawing(true);

	str=RemoveExt(SaveDialogChr->FileName);

	if(str=="") str="tileset";

	SaveDialogImage->FileName=str+".bmp";

	if(SaveDialogImage->Execute())
	{
		picture=new TPicture();
		bmp=new Graphics::TBitmap();
		bmp->SetSize(128,128);
		bmp->PixelFormat=pf4bit;
		SetBMPPalette(bmp);
		picture->Bitmap=bmp;

		x=0;
		y=0;

		for(i=0;i<256;i++)
		{
			DrawExportTile16(picture,x,y,i,palActive,-1,-1,false);

			x+=8;

			if(x>=128)
			{
				x=0;
				y+=8;
			}
		}

		//test
		unsigned short picFormat;
		unsigned int cbHandle;
		HPALETTE hPal;

		picture->SaveToClipboardFormat(picFormat, cbHandle, hPal);
		Clipboard()->SetAsHandle(picFormat, cbHandle);

		picture->SaveToFile(SaveDialogImage->FileName);

		delete bmp;
		delete picture;
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::SpeedButtonCheckerClick(TObject *Sender)
{
	Attributes1->Checked = SpeedButtonChecker->Down;
	UpdateNameTable(-1,-1,true);
	if(FormNavigator->Visible) FormNavigator->Draw(false,false);
	/*LabelStats->Caption=LabelStats->Caption
		+"\n\nCheck!";
	StatusUpdateWaiter->Enabled=true;
	holdStats=true;*/
}
//---------------------------------------------------------------------------




void __fastcall TFormMain::MSaveNameTableASMClick(TObject *Sender)
{
	AnsiString name,filter;
	unsigned char* buf;
	int size;

	BlockDrawing(true);

	SaveDialogName->FileName=RemoveExt(SaveDialogName->FileName);
	SaveDialogName->Title="Save nametable as assembly code";

	filter="Assembly code (*.asm)|*.asm|CA65 assembly code (*.s)|*.s|Any file (*.*)|*.*";

	SaveDialogName->Filter=filter;

	if(SaveDialogName->Execute())
	{
		name=RemoveExt(SaveDialogName->FileName)+(SaveDialogName->FilterIndex!=2?".asm":".s");

		if(FileExists(name))
		{
			if(Application->MessageBox(("File "+name+" already exists. Overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES)
			{
				BlockDrawing(false);
				return;
			}
		}

		buf=(unsigned char*)malloc(name_size()+attr_size());
		size=0;

		if(MSaveIncName->Checked)
		{
			memcpy(buf,nameTable,name_size());
			size+=name_size();
		}

		if(MSaveIncAttr->Checked)
		{
			memcpy(buf+size,attrTable,attr_size());
			size+=attr_size();
		}

		//hotfix that could probably be refactored away, but this works.
		if (byte1->Checked) {byte1bool=true; db1bool=false;}
		if (db1->Checked)   {byte1bool=false; db1bool=true;}

		name=save_data(name.c_str(),buf,size,SAVE_FORMAT_ASM,MSaveRLE->Checked);

		free(buf);

		if(name!="") Application->MessageBox(name.c_str(),"Error",MB_OK);
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRFillNumbersClick(TObject *Sender)
{
	int i,j,k,pp;
	int x,y,w,h;
	bool skip[256]={false};
	bool doWhole;
	int cnt=0;
	
	//these allow nonselected tiles to be skipped.
	GetSelection(chrSelection,x,y,w,h);
	if(chrSelectRect)
	{
		if(w>1||h>1)
		{
			for(i=0;i<16;++i)
			{
				for(j=0;j<16;++j)
				{
					skip[i*16+j]=(i>=y&&i<y+h&&j>=x&&j<x+w)?false:true;
				}
			}
		}
	}
	else
	{
		for(i=0;i<256;++i)
		{
			skip[i]= chrSelected[i]?false:true;
			if (chrSelected[i]) cnt++;
		}
	}

	if (cnt>0 || (chrSelectRect && (w>1||h>1))) doWhole=false;
	else doWhole=true;



	//process
	//if(Application->MessageBox("Do you want to fill current table with numbers?","Confirm",MB_YESNO)==ID_YES)
	//{


		SetUndo();






		int tile=0;

		//pp=tileViewTable[tile];

		for(i=0;i<16;i++)
		{
			for(j=0;j<16;j++)
			{
				if(!doWhole && skip[j+i*16]) {tile++; continue;}
				for(k=0;k<16;k++)
				{
					int y=tileViewTable[j+i*16]/16;
					int x=tileViewTable[j+i*16]&15;

					chr[tileViewTable[tile]*16+bankActive+k]
						=
						 (smallnums_data[(y<<4)+k]&0xf0)
						|(smallnums_data[(x<<4)+k]&0x0f);
				}
				tile++;
			}
		}

		//memcpy(&chr[4096],chr,4096);

		UpdateTiles(true);
		UpdateNameTable(-1,-1,true);
		FormNavigator->Draw(false,false);
		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	//}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRFindDoublesClick(TObject *Sender)
{
	if(MCHRFindDoubles->Checked)
	{
		for(int i=0;i<256;++i) chrSelected[i]=0;
		SelectTile(tileActive);
		MCHRFindDoubles->Checked=false;
		MCHRFindDoubles->Caption="Find &duplicates";
		UpdateAll();
	}
	else
	{
		FindDoublesUnused(false);
		MCHRFindDoubles->Checked=true;
		MCHRFindDoubles->Caption="Deselect Find &duplicates";

		LabelStats->Caption="Duplicates found. \n\nTip: Press again to deselect.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
	}
}
//---------------------------------------------------------------------------



int           *import_name;
unsigned char *import_chr;
int *import_chr_sort;
int *import_chr_freq;

unsigned char *import_chr_temp;
unsigned char *import_bmp;
int           *import_stat;

int import_chr_size;
int import_name_size;

int import_bmp_width;
int import_bmp_height;

int import_name_width;
int import_name_height;

int import_stat_size;



//compare two tiles by counting number of matching pixels

inline int tile_lossy_compare( unsigned char* tile1ext, unsigned char* tile2ext)
{
	int i,j,col11,col21,col12,col22,mask,match;

	int col0match,col1match,col2match,col3match;
	int rowMatch[8] = {0};
	int columnMatch[8] = {0};

	//match=0;
	int weight0=1;
	int weight1=1;
	int weight2=1;
	int weight3=1;

	// CPU cycles attempt 1 - precompute mask values for row loop
	int maskTable[8];
	for (i = 0, mask = 0x01; i < 8; ++i, mask <<= 1)
	{
		maskTable[i] = mask;
	}
	// CPU cycles savings attempt 2 - copy tiles to compare to local memory, to reduce global access
	char tile1[16];
	char tile2[16];
	memcpy (tile1,tile1ext,16);
	memcpy (tile2,tile2ext,16);

	for(i=0;i<8;++i)  //look at a byte (8x1 sliver)
	{
		//old - checks bitplanes
		/*
		col11=tile1[i];
		col21=tile2[i];
		col12=tile1[i+8];
		col22=tile2[i+8];
		*/

		//new checks indexes. seems worse on test materials, possibly bugged. disabled for now.

		col0match  = ~(( ~tile1[i]  & ~tile1[i+8])  		  ^ ( ~tile2[i] & ~tile2[i+8]));
		col1match  = ~((  tile1[i]  & (tile1[i] ^ tile1[i+8])) ^ (  tile2[i]  & (tile2[i] ^ tile2[i+8])));
		col2match  = ~((  tile1[i+8]& (tile1[i] ^ tile1[i+8])) ^ (  tile2[i+8]& (tile2[i] ^ tile2[i+8])));
		col3match  = ~((  tile1[i]  &  tile1[i+8])  			  ^ (  tile2[i] &  tile2[i+8]));

		
		//mask=0x01;
		match=0;
		for(j=0;j<8;++j) //score each bit of a sliver.
		{
			mask = maskTable[j];
			//old - checks bitplanes

			//if(!((col11^col21)&mask)) ++match;
			//if(!((col12^col22)&mask)) ++match;

			// new, disabled

			if (col0match & mask) ++match;
			if (col1match & mask) ++match;
			if (col2match & mask) ++match;
			if (col3match & mask) ++match;


			//mask<<=1;
		}

		rowMatch[i]=match*match;
	}
	int rowScore=0;
	for(i=0;i<8;i++)
	{
		rowScore+=rowMatch[i];
	}
	//rowScore=rowScore*rowScore;
	//score columns next
	//..................



	//mask=0x01;

	for(j=0;j<8;++j)
	{
	match=0;

		for(i=0;i<8;++i)
		{
			mask = maskTable[i];
			//col0
			match+=((~(( ~tile1[i]  & ~tile1[i+8])
					 ^ ( ~tile2[i]  & ~tile2[i+8])))&mask);

			//col1
			match  += ((~((  tile1[i]  & (tile1[i] ^ tile1[i+8]))
						^ (  tile2[i]  & (tile2[i] ^ tile2[i+8]))))&mask);

			//col2
			match  += ((~((  tile1[i+8]& (tile1[i] ^ tile1[i+8]))
						^ (  tile2[i+8]& (tile2[i] ^ tile2[i+8]))))&mask);

			//col3
			match  += ((~((  tile1[i]  &  tile1[i+8])
						^ (  tile2[i] &  tile2[i+8])))&mask);

		}
	//mask<<=1;

	columnMatch[i]=match*match;
	}
	int columnScore=0;
	for(i=0;i<8;i++)
	{
		columnScore+=columnMatch[i];
	}
	//columnScore=columnScore*columnScore;

	int finalScore=columnScore+rowScore;
	return finalScore;     //match
}

class Thread_lossyTileComp : public TThread{
	private:
		int start_tile;
		int end_tile;
		int tile_count;
		int* max_array;
		int* tile1_array;
		int* tile2_array;
		int thread_index;
		int local_max;
		int local_tile1;
		int local_tile2;
	public:
		Thread_lossyTileComp(int start, int end, int count, int* max_arr,  int* tile1_arr,  int* tile2_arr, int index)
		: TThread(false), start_tile(start), end_tile(end),
		tile_count(count), max_array(max_arr), tile1_array(tile1_arr), tile2_array(tile2_arr), thread_index(index), local_max(0), local_tile1(0), local_tile2(0)
		{}

	void __fastcall Execute(){
		for (int i = start_tile; i < end_tile - 1; ++i){
			for (int j = i + 1; j < tile_count; ++j){
				int n = tile_lossy_compare(&import_chr[i * 16], &import_chr[j * 16]) * 65536 / import_stat[j];

				if (n > local_max){
					local_max = n;
					local_tile1 = i;
					local_tile2 = j;
				}
			}
		}
		//output array entries to pick a final winner once all threads are done
		max_array[thread_index] = local_max;
		tile1_array[thread_index] = local_tile1;
		tile2_array[thread_index] = local_tile2;
	}
};

int __fastcall TFormMain::ImportConvertTiles(int wdt,int hgt,int xoff,int yoff,int thresh)
{
	unsigned char tile[16];
	int i,j,k,l,x,y,c1,c2,col,pp,ptr,tile_count,pixel_count;
	bool add;

	memset(import_chr ,0,import_chr_size);
	memset(import_name,0,import_name_size);

	ptr=0;
	tile_count=0;

	for(i=0;i<hgt/8;++i)
	{
		for(j=0;j<wdt/8;++j)
		{
			memset(tile,0,sizeof(tile));

			pp=0;

			y=((i<<3)-yoff);

			pixel_count=0;

			for(k=0;k<8;++k)
			{
				x=((j<<3)-xoff);

				for(l=0;l<8;++l)
				{
					if(x<0) x+=wdt;
					if(x>wdt) x-=wdt;
					if(y<0) y+=hgt;
					if(y>hgt) y-=hgt;

					col=import_bmp[y*import_bmp_width+x];

					c1=(col&1)?(1<<(7-l)):0;
					c2=(col&2)?(1<<(7-l)):0;

					tile[pp+0]|=c1;
					tile[pp+8]|=c2;

					if(c1|c2) ++pixel_count;

					++x;
				}

				++pp;
				++y;
			}

			add=false;


			if(thresh<0||(!pixel_count||pixel_count>thresh)) add=true;
			if(pixel_count==0 || pixel_count<=thresh)
			{
				import_name[i*import_name_width+j]=nullTile;
				add=false;
			}


			if(add)
			{
				for(k=0;k<tile_count;++k)  // look for match in existing tiles
				{
					if(memcmp(tile,&import_chr[k*16],16)==0)
					{
						import_name[i*import_name_width+j]=k;
						add=false; //found match, don't lay down new chr tile.
						break;
					}
				}
			}


			if(add)
			{
				if(tile_count == nullTile) {ptr+=16; ++tile_count;} //skip ahead preserving the null tile.


				memcpy(&import_chr[ptr],tile,16);
				ptr+=16;
				import_name[i*import_name_width+j]=tile_count;

				++tile_count;

				if(ptr>=import_chr_size) break;
			}
		}
	}

	return tile_count;
}


struct matchStruct {
	unsigned char match;
	int tile1;
	int tile2;
};

enum {
	IMPORT_NORMAL=0,
	IMPORT_SQUEEZE,
	IMPORT_MATCH
};

bool __fastcall TFormMain::ImportBMP(AnsiString filename,int mode,bool shift,int thresh,int maxtiles,bool noAttr, bool noPal)
{
	FILE *file;
	unsigned char *data;
	int i,j,k,l,x,y,n,size,wdt,hgt,pp,ps,off,ptr,bpp;
	int cnt,fi,min,max,id,rx,gx,bx,pitch;
	int tile_count,tiles_min,tile1,tile2,shift_x,shift_y,best_x,best_y;
	int attr[4];
	AnsiString str;

	int infoheader;

	file=fopen(filename.c_str(),"rb");

	if(!file) return false;

	size=get_file_size(file);
	data=(unsigned char*)malloc(size);
	fread(data,size,1,file);
	fclose(file);

	infoheader=data[0x0e]; //added because some bitmaps may have nonstandard length headers.
	bpp=data[28];

	if(data[0]!='B'||data[1]!='M'||(bpp!=4&&bpp!=8)||data[29]!=0||read_dword(&data[30]))
	{
		Application->MessageBox("Wrong BMP format, only uncompressed 4- or 8bit images supported","Error",MB_OK);
		free(data);
		return false;
	}

	off=read_dword(&data[10]);
	wdt=read_dword(&data[18]);
	hgt=read_dword(&data[22]);

	if(wdt<8||wdt>8*NAME_MAX_WIDTH||labs(hgt)<8||labs(hgt)>8*NAME_MAX_HEIGHT)
	{
		Application->MessageBox(("BMP should not be smaller than 8x8 or larger than "+IntToStr(NAME_MAX_WIDTH*8)+"x"+IntToStr(NAME_MAX_HEIGHT)+" pixels").c_str(),"Error",MB_OK);
		free(data);
		return false;
	}

	//find similar colors in the NES palette

	if(!noPal) //switch for skipping overwrite of palette
	{
		for(i=0;i<16;i++)
		{
			min=0x01000000;
			id=0;

			for(j=0;j<64;j++)
			{
				//should be up to user. pal_validate() also defaults black to $0F according to config.
				//if(j==0x1d||j==0x2d) continue;

				bx=((outPalette[j]>>16)&255)-data[i*4+infoheader+0x0e+0]; //54 was
				gx=((outPalette[j]>>8 )&255)-data[i*4+infoheader+0x0e+1]; //55
				rx=((outPalette[j]    )&255)-data[i*4+infoheader+0x0e+2]; //56
				//swapped assignment order from rx,gx,bx to bx,gx,rx since that's what bmp:s do.

				//old
				//fi=30*rx*rx+59*gx*gx+11*bx*bx;

				//new
				int redmean=(((outPalette[j])&255) + data[i*4+infoheader+0x0e+2])/2;
				if(redmean<128) fi=2*rx*rx + 4*gx*gx + 3*bx*bx;
				else            fi=3*rx*rx + 4*gx*gx + 2*bx*bx;

				if(fi<min)
				{
					min=fi;
					id=j;
				}
			}

			if(id==0x20) id=0x30;

			if((i!=4&&i!=8&&i!=12) || sharedCol0==false ) bgPal[palBank*16+(i>>2)*4+(i&3)]=id;
		}

		pal_validate();
	}

	//put bitmap into an array to make it easier to work with

	import_bmp_width =wdt;
	import_bmp_height=hgt;

	import_bmp=(unsigned char*)malloc(import_bmp_width*import_bmp_height*sizeof(unsigned char));

	memset(import_bmp,0,import_bmp_width*import_bmp_height*sizeof(unsigned char));

	switch(bpp)
	{
	case 4:
		{
			pitch=wdt&~7;

			if(wdt&7) pitch+=8;

			pitch>>=1;

			for(i=0;i<hgt;++i)
			{
				ps=off+(hgt-1-i)*pitch;

				for(j=0;j<wdt;++j)
				{
					if(!(j&1))
					{
						import_bmp[i*import_bmp_width+j]=data[ps]>>4;
					}
					else
					{
						import_bmp[i*import_bmp_width+j]=data[ps++]&15;
					}
				}
			}
		}
		break;

	case 8:
		{
			pitch=wdt&~3;

			if(wdt&3) pitch+=4;

			for(i=0;i<hgt;++i)
			{
				ps=off+(hgt-1-i)*pitch;

				for(j=0;j<wdt;++j) import_bmp[i*import_bmp_width+j]=data[ps++];
			}
		}
		break;
	}

	free(data);

	//convert graphics into tiles

	if(wdt&7) wdt=(wdt&~7)+8;
	if(hgt&7) hgt=(hgt&~7)+8;

	import_name_width =wdt/8;
	import_name_height=hgt/8;

	import_name_size=import_name_width*import_name_height*sizeof(int);
	import_chr_size =import_bmp_width *import_bmp_height *sizeof(unsigned char);

	import_name=(int*)malloc(import_name_size);
	import_chr =(unsigned char*)malloc(import_chr_size);

	tiles_min=import_name_width*import_name_height;

	best_x=0;
	best_y=0;

	if(shift)
	{
		for(shift_y=0;shift_y<8;++shift_y)
		{
			for(shift_x=0;shift_x<8;++shift_x)
			{
				tile_count=ImportConvertTiles(wdt,hgt,shift_x,shift_y,thresh);

				if(tile_count<tiles_min)
				{
					tiles_min=tile_count;
					best_x=shift_x;
					best_y=shift_y;
				}
			}
		}
	}

	tile_count=ImportConvertTiles(wdt,hgt,best_x,best_y,thresh);
	importBMP_tile_count=tile_count; //for external display purposes
    importBMP_tile_count_reduced=tile_count;  //establishing baseline value before lossy compress
/*
		if(mode==IMPORT_NORMAL&&tile_count>maxtiles)
		{
			Application->MessageBox(("Too many unique tiles (256+"+IntToStr(tile_count-256)+"="+IntToStr(tile_count)+").\nExcess tiles will be put on pattern table B").c_str(),"Warning",MB_OK);
		}
*/

	if(mode==IMPORT_SQUEEZE)//&&(tile_count>maxtiles))
	{
		str=FormImportBMP->Caption;

		import_stat_size=tile_count*sizeof(int);
		import_stat=(int*)malloc(import_stat_size);

		//tile sorter begins here
		//if (FormLossyDetails->RadioFreq->Checked==true)
		//{
			FormImportBMP->Caption="Wait... ["+IntToStr(tile_count)+" tiles]";
			unsigned int temp=0;
			//import_chr_sort =(int*)malloc(import_chr_size/16);
			//import_chr_freq =(int*)malloc(import_chr_size/16);
			import_chr_sort =(int*)malloc(tile_count*sizeof(int));
			import_chr_freq =(int*)malloc(tile_count*sizeof(int));
			import_chr_temp =(unsigned char*)malloc(tile_count*16);


			memset(import_chr_freq,0,sizeof(import_chr_freq));


			//get use frequency
			for(i=0;i<import_name_height*import_name_width;i++)
				++import_chr_freq[import_name[i]];


			//list sort table
			for(i=0;i<tile_count;i++) import_chr_sort[i]=i;

			//sort chr assignment
			for(i=0;i<tile_count-1;i++)
			{
				for(j=0;j<tile_count-i-1;j++)
				{
					if(import_chr_freq[j]<import_chr_freq[j+1])
					{
						temp=import_chr_freq[j];
						import_chr_freq[j]=import_chr_freq[j+1];
						import_chr_freq[j+1]=temp;

						temp=import_chr_sort[j];
						import_chr_sort[j]=import_chr_sort[j+1];
						import_chr_sort[j+1]=temp;
					}
				}
			}

			//repair NT
			for(i=0;i<import_name_height*import_name_width;i++)
			{
				for(j=0;j<tile_count;j++)
				{
					if(import_chr_sort[j]==import_name[i])
					{
						import_name[i]=j;
						break;
					}
				}
			}

			//copy chr according to sort assignment
			memcpy(import_chr_temp,import_chr,tile_count*16);
			//memcpy(import_chr,import_chr_temp,sizeof(import_chr));

			for(i=0;i<tile_count;i++)
			{
				int index = import_chr_sort[i];
				memcpy(&import_chr[i*16],&import_chr_temp[index*16],16);
			}

			skip:


			free(import_chr_sort);
			free(import_chr_freq);
			free(import_chr_temp);

		//}
		//tile combinator begins here

        SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);

			int num_cpus = sysinfo.dwNumberOfProcessors;
			int num_threads = num_cpus*1.5; // number of threads to spawn. 1.5 seemed like a fine compromise.
			num_threads = 1>num_threads? 1:num_threads;
			num_threads = 24<=num_threads? 24:num_threads;

			const int tiles_per_thread = (tile_count-1)/num_threads + 1; // number of tiles each thread will handle
			int* max_array = new int[num_threads];
			int* tile1_array = new int[num_threads];
			int* tile2_array = new int[num_threads];

		//FormImportBMP->ButtonCancel->Enabled=false;
		FormImportBMP->ButtonSwap->Enabled=false;

		while(tile_count>maxtiles)
		{
			FormImportBMP->Caption="Wait... ["+IntToStr(tile_count)+" tiles]";

			//count tile use

			memset(import_stat,0,import_stat_size);
			for(i=0;i<import_name_width*import_name_height;++i) ++import_stat[import_name[i]];


			if(tile_count<32){
			//find a pair of tiles with max similarity

			max=0;
			tile1=0;
			tile2=0;


			for(i=0;i<tile_count-1;++i)
			{
				for(j=i+1;j<tile_count;++j)
				{
					n=tile_lossy_compare(&import_chr[i*16],&import_chr[j*16])*65536/import_stat[j];

					if(n>max)
					{
						max=n;
						tile1=i;
						tile2=j;
					}
				}
			}

			}

			else{
			//test begin-----


			std::vector<Thread_lossyTileComp*> thread_list;

			//int tiles_per_thread = tile_count / num_threads;
			for (int i = 0; i < num_threads; ++i)
			{
				int start_tile = i * tiles_per_thread;
				int end_tile = (i + 1) * tiles_per_thread;
				if (i == num_threads - 1) end_tile = tile_count; // Last thread takes any remaining tiles

				Thread_lossyTileComp* thread = new Thread_lossyTileComp(start_tile, end_tile, tile_count, max_array, tile1_array, tile2_array, i);
				thread->Resume();
				thread_list.push_back(thread);
			}

		// Wait for threads to complete
		for (std::vector<Thread_lossyTileComp*>::iterator it = thread_list.begin(); it != thread_list.end(); ++it)
		{
			(*it)->WaitFor();
		}

		// Find the maximum value across all threads
		int global_max = 0;
		for (int i = 0; i < num_threads; ++i)
		{
			if (max_array[i] > global_max)
			{
				global_max = max_array[i];
				tile1 	   = tile1_array[i];
				tile2	   = tile2_array[i];
			}
		}

		//delete arrays removed from loop
			//test end.....
		}


			//remove one of tiles

			for(i=0;i<import_name_width*import_name_height;++i)
			{
				if(import_name[i]==tile2) import_name[i]=tile1;
				else if(import_name[i]>tile2) --import_name[i];
			}
			memcpy(&import_chr[tile2*16],&import_chr[tile2*16+16],(tile_count-tile2-1)*16);
			--tile_count;
		}
		delete[] max_array;
		delete[] tile1_array;
		delete[] tile2_array;
		free(import_stat);

		FormImportBMP->Caption=str;
		//FormImportBMP->ButtonCancel->Enabled=true;
		FormImportBMP->ButtonSwap->Enabled=true;

	}

	if(mode!=IMPORT_MATCH)
	{

		int tilecap = maxtiles<tile_count? maxtiles:tile_count;
		tile_count = tilecap;
		memcpy(chr+bankActive,import_chr,tile_count*16);

		for(i=0;i<import_name_width*import_name_height;++i)
		{
			n=import_name[i];

		   //	if(n>maxtiles-1) n=maxtiles-1; //old
			if(n>maxtiles-1) n=nullTile;
			nameTable[i]=n;
		}
	}

	if(mode==IMPORT_MATCH)
	{
		for(i=0;i<import_name_width*import_name_height;++i)
		{
			tile1=import_name[i];

			max=0;
			tile2=0;

			for(j=0;j<maxtiles;++j)
			{
				n=tile_lossy_compare(&import_chr[tile1*16],&chr[bankActive+j*16]);

				if(n>max)
				{
					max=n;
					tile2=j;
				}
			}

			nameTable[i]=tile2;
		}

	}

	free(import_chr);
	free(import_name);

	//try to set proper attributes

	if(mode!=IMPORT_MATCH)
	{
		nameTableWidth =import_name_width;
		nameTableHeight=import_name_height;

		for(i=0;i<import_bmp_height;i+=16)
		{
			for(j=0;j<import_bmp_width;j+=16)
			{
				for(k=0;k<4;++k) attr[k]=0;

				for(k=i;k<i+16;++k)
				{
					for(l=j;l<j+16;++l)
					{
						if(import_bmp[k*import_bmp_width+l]&3) ++attr[import_bmp[k*import_bmp_width+l]>>2];
					}
				}

				id=0;
				max=0;

				for(k=0;k<4;++k)
				{
					if(attr[k]>max)
					{
						max=attr[k];
						id=k;
					}
				}

				if(!noAttr) AttrSet(j>>3,i>>3,id,false);
			}
		}
	}
	free(import_bmp);
	importBMP_tile_count_reduced = tile_count; //for external display purposes
    nameTableWidth =import_name_width;
	nameTableHeight=import_name_height;
	return true;
}



void __fastcall TFormMain::MImportBMPNametableClick(TObject *Sender)
{
	int mode;

	BlockDrawing(true);
	
	OpenDialogImport->Title="Import BMP to canvas";
	OpenDialogImport->Filter="Windows bitmap files (*.bmp)|*.bmp|All files (*.*)|*.*";
	OpenDialogImport->DefaultExt="bmp";

	if(OpenDialogImport->Execute())
	{
		SetUndo();

		if(!MImportLossy->Checked) mode=IMPORT_NORMAL; else mode=IMPORT_SQUEEZE;

		if(ImportBMP(OpenDialogImport->FileName,mode,MImportBestOffsets->Checked,MImportThreshold->Checked?8:-1,256,MImportNoColorData->Checked,MImportNoColorData->Checked))
		{
			UpdateAll();
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MImportNESClick(TObject *Sender)
{
	FILE *file;
	unsigned char *data;

	BlockDrawing(true);
	
	OpenDialogImport->Title="Import NES file";
	OpenDialogImport->Filter="NES cartridge dump|*.nes|All files (*.*)|*.*";
	OpenDialogImport->DefaultExt="nes";

	if(OpenDialogImport->Execute())
	{
		FormBank->Caption="Import 8K CHR bank from NES file";
		FormBank->FileName=OpenDialogImport->FileName;
		FormBank->ShowModal();

		if(FormBank->OK)
		{
			file=fopen(OpenDialogImport->FileName.c_str(),"rb");

			if(file)
			{
				fseek(file,16+FormBank->PRG*16384+FormBank->Bank*8192,SEEK_SET);
				fread(chr,8192,1,file);
				fclose(file);
			}

			UpdateTiles(true);
			UpdateNameTable(-1,-1,true);
			FormNavigator->Draw(false,false);
			//UpdateMetaSprite();
			cueUpdateMetasprite=true;
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MExportNESClick(TObject *Sender)
{
	FILE *file;
	unsigned char *nes;
	int size;

	BlockDrawing(true);
	
	if(SaveDialogExportNES->Execute())
	{
		FormBank->Caption="Export 8K CHR bank to NES file";
		FormBank->FileName=SaveDialogExportNES->FileName;
		FormBank->ShowModal();

		if(FormBank->OK)
		{
			file=fopen(SaveDialogExportNES->FileName.c_str(),"rb");

			if(file)
			{
				fseek(file,0,SEEK_END);
				size=ftell(file);
				fseek(file,0,SEEK_SET);

				nes=(unsigned char*)malloc(size);
				fread(nes,size,1,file);
				fclose(file);

				file=fopen((SaveDialogExportNES->FileName+".bak").c_str(),"wb");

				if(!file)
				{
					free(nes);
					BlockDrawing(false);
					return;
				}

				fwrite(nes,size,1,file);
				fclose(file);

				memcpy(nes+16+FormBank->PRG*16384+FormBank->Bank*8192,chr,8192);

				file=fopen(SaveDialogExportNES->FileName.c_str(),"wb");

				if(file)
				{
					fwrite(nes,size,1,file);
					fclose(file);
				}
				
				free(nes);
			}
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MImportBMPTilesetClick(TObject *Sender)
{
	FILE *file;
	unsigned char *data;
	int i,j,k,l,size,wdt,hgt,pp,ps,off,ptr,bpp;
	unsigned char tile[16];

	BlockDrawing(true);

	//if(Sender==MImportBMPTileset)

	OpenDialogImport->Title="Import BMP as tileset";
	OpenDialogImport->Filter="Windows bitmap files (*.bmp)|*.bmp|All files (*.*)|*.*";
	OpenDialogImport->DefaultExt="bmp";

	if(OpenDialogImport->Execute())
    {
		file=fopen(OpenDialogImport->FileName.c_str(),"rb");

		if(file)
		{
			size=get_file_size(file);
			data=(unsigned char*)malloc(size);
			fread(data,size,1,file);
			fclose(file);



			if(data[0]==0x89&&data[1]==0x50&&data[2]==0x4E&&data[3]==0x47)
				{
					Application->MessageBox(
						"Please use 4bpp or 8bpp BMP.",
						"Error: PNG not supported",MB_OK);
					free(data);
					BlockDrawing(false);
					return;
				}


			bpp=data[28];

			if(data[0]!='B'||data[1]!='M'||(bpp!=4&&bpp!=8)||data[29]!=0||read_dword(&data[30]))
			{
				Application->MessageBox(
						"Wrong BMP format: only 4bpp & 8bpp files w/o RLE are supported",
						"Error",MB_OK);
				free(data);
				BlockDrawing(false);
				return;
			}

			off=read_dword(&data[10]);
			wdt=read_dword(&data[18]);
			hgt=read_dword(&data[22]);

			if(wdt>128||(hgt>128&&hgt<-128)||(wdt&7)||(hgt&7))
			{
				Application->MessageBox("BMP should be 128x128 pixels or smaller, & dimensions a multiple of 8","Error",MB_OK);
				free(data);
				BlockDrawing(false);
				return;
			}

			ptr=bankActive;

			if(!(wdt==128&&hgt==128)) ptr+=tileActive*16;
			
			for(i=0;i<hgt/8;i++)
			{
				for(j=0;j<wdt/8;j++)
				{
					memset(tile,0,sizeof(tile));
					pp=0;

					//test (not working right now)
					/*
					if(bpp==1)
					{
						for(k=0;k<8;k++)
						{
							ps=off+(hgt-1-i*8-k)*(wdt/4)+j;
							for(l=0;l<8;l++)
							{
								tile[pp]|=(data[ps])?1<<(7-l):0; 
								ps++;
							}
							pp++;
						}
					}
					*/
					if(bpp==4)
					{
						for(k=0;k<8;k++)
						{
							/*if(hgt>0) ps=off+(i*8+k)*128+j*4; else */
							ps=off+(hgt-1-i*8-k)*(wdt/2)+j*4;
							for(l=0;l<8;l++)
							{
								tile[pp]|=(data[ps]&(l&1?0x01:0x10))?1<<(7-l):0;
								tile[pp+8]|=(data[ps]&(l&1?0x02:0x20))?1<<(7-l):0;

								if(l&1) ps++;
							}

							pp++;
						}
					}
					else
					{
						for(k=0;k<8;k++)
						{
							/*if(hgt>0) ps=off+(i*8+k)*128+j*4; else */
							ps=off+(hgt-1-i*8-k)*wdt+j*8;
							for(l=0;l<8;l++)
							{
								tile[pp]|=(data[ps]&1)?1<<(7-l):0;
								tile[pp+8]|=(data[ps]&2)?1<<(7-l):0;

								ps++;
							}

							pp++;
						}
					}

					memcpy(&chr[ptr+j*16],tile,16);
				}

				ptr+=16*16;
			}

			free(data);
		}

		UpdateTiles(true);
		UpdateNameTable(-1,-1,true);
		FormNavigator->Draw(false,false);
		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSwapBanksClick(TObject *Sender)
{
	unsigned char temp[4096];
	uint32_t propt[512];



	memcpy(temp,chr,4096);
	memcpy(chr,chr+4096,4096);
	memcpy(chr+4096,temp,4096);

	memcpy(propt					,tileProperties				,256*sizeof(uint32_t));
	memcpy(tileProperties			,&tileProperties[256]		,256*sizeof(uint32_t));
	memcpy(&tileProperties[256]		,propt						,256*sizeof(uint32_t));

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	if (FormNavigator->Visible) FormNavigator->Draw(false,false);
	if (FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MOpenAllClick(TObject *Sender)
{
	BlockDrawing(true);

	if(OpenDialogAll->Execute())
	{
		OpenAll(RemoveExt(OpenDialogAll->FileName));
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveAllClick(TObject *Sender)
{
	BlockDrawing(true);

	SaveCHR(0,8192);

	if(SaveDialogName->FileName=="") SaveDialogName->FileName=SaveDialogChr->FileName;
	if(SaveDialogMap ->FileName=="") SaveDialogMap ->FileName=SaveDialogChr->FileName;

	if(nameTableWidth==32&&nameTableHeight==30)
	{
		MSaveNameTableBINClick(Sender);
	}
	else
	{
		MSaveMapClick(Sender);
	}

	if(SaveDialogPal->FileName=="") SaveDialogPal->FileName=SaveDialogName->FileName;

	MPaletteSaveClick(Sender);

	MSaveMetaSpriteBankClick(Sender);

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MLoadSessionClick(TObject *Sender)
{
	BlockDrawing(true);

	if(OpenDialogSession->Execute())
	{
		if(!LoadSession(OpenDialogSession->FileName)) Application->MessageBox("Can't load session","Error",MB_OK);
        else unsavedChanges=false;
	}

	UpdateAll();
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveSessionClick(TObject *Sender)
{
	BlockDrawing(true);

	if(SaveDialogSession->Execute())
	{
		SaveSession(SaveDialogSession->FileName);
		//unsavedChanges=false;  //this is now in SaveSession
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MExportPaletteBMPClick(TObject *Sender)
{
	TPicture *picture;
	Graphics::TBitmap *bmp;
	int i;
	unsigned char* dst;
	AnsiString str;

	BlockDrawing(true);
	
	str=RemoveExt(SaveDialogPal->FileName);

	if(str=="") str="palette";

	SaveDialogImage->FileName=str+".bmp";

	if(SaveDialogImage->Execute())
	{
		picture=new TPicture();
		bmp=new Graphics::TBitmap();
		bmp->SetSize(16,1);
		bmp->PixelFormat=pf4bit;
		SetBMPPalette(bmp);
		picture->Bitmap=bmp;

		dst=(unsigned char*)picture->Bitmap->ScanLine[0];

		for(i=0;i<16;i+=2) *dst++=((i+1)|(i<<4));

		picture->SaveToFile(SaveDialogImage->FileName);

		delete bmp;
		delete picture;
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonTypeInClick(TObject *Sender)
{

	EnableDisableTypeConflictShortcuts(false); //bool=true forces disable, unconditionally.
	bool TypeIn = SpeedButtonTypeIn->Down;
	if(TypeIn)
	{
		if(nameSelection.left>=0)
		{
			nameSelection.right =nameSelection.left+1;
			nameSelection.bottom=nameSelection.top +1;
			returnCarriagePos= nameSelection.left;
			UpdateNameTable(-1,-1,true);
		}
	}

	StaticTextFontOffset->Caption="$"+IntToHex(typeInFontOffset,2);

	StaticTextFontOffset->Visible=TypeIn;
	TypeInModeOnOff1->Checked=TypeIn;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormKeyPress(TObject *Sender, char &Key)
{
	if(SpeedButtonTypeIn->Down)
	{
		if(Key>=32 && !bIgnoreKey)
		{
			NameTableTypeIn(Key-typeInASCIIOffset);
            cueUpdateNametable=true;
		}
	}
	
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::PageControlEditorChange(TObject *Sender)
{
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
	ActiveControl=NULL;//to prevent ListBoxSpriteList grab focus while Nametable tab is active
	if(PageControlEditor->ActivePage==TabSheetTile)
	GroupBoxTiles->Caption="Screen (view only)";
	
	else
	{

		GroupBoxTiles->Caption="Tileset";
		if(PageControlEditor->ActivePage==TabSheetSprite)
		{
			if(GroupBoxSpriteList->Visible) ActiveControl=ListBoxSpriteList;
			Fill1->Enabled=false;
		}
		if(PageControlEditor->ActivePage==TabSheetName)
		{
			Fill1->Enabled=true;
		}
	}
	if(uiScale>1 && PageControlEditor->ActivePage==TabSheetTile)
	{	GroupBoxTileControl->Visible=true;}
	else
	{	GroupBoxTileControl->Visible=false;	}

	UpdateNameTable(-1,-1,true);        //true or false?
	if(FormNavigator->Visible)FormNavigator->Draw(false,false);
	UpdateTiles(true);  //true or false?
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageMetaSpriteDragOver(TObject *Sender,
TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	if(Source->InheritsFrom(__classid(TImage))) Accept=true; else Accept=false;
	bDrawDestShadow=false;
	cueUpdateTiles=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageMetaSpriteDragDrop(TObject *Sender,
TObject *Source, int X, int Y)
{
	int i,pp,tile;

	SetUndo();
	
	pp=metaSpriteActive*64*4;

	for(i=0;i<64;++i)
	{
		if(metaSprites[pp]==255)
		{
			tile=tileViewTable[tileActive];

			if(SpeedButtonSprite8x16->Down) tile=(tileViewTable[tileActive]&0xfe)+(bankActive?1:0);

			metaSprites[pp+0]=Y/uiScale;
			metaSprites[pp+1]=tile;
			metaSprites[pp+2]=palActive;
			metaSprites[pp+3]=X/uiScale;

			if(SpeedButtonSpriteSnap->Down) SpriteSnap(i);

			SelectSprite(i,false);

			break;
		}

		pp+=4;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageMetaSpriteEndDrag(TObject *Sender,
TObject *Target, int X, int Y)
{
	if(Target) cueUpdateMetasprite=true; //UpdateMetaSprite();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonPrevMetaSpriteClick(TObject *Sender)
{
	if(metaSpriteActive)
	{
		--metaSpriteActive;

		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
		//UpdateStats();
		cueStats=true;
	}
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonNextMetaSpriteClick(TObject *Sender)
{
	if(metaSpriteActive<255)
	{
		++metaSpriteActive;

		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
		//UpdateStats();
		cueStats=true;
	}
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonClearMetaSpriteClick(TObject *Sender)
{
	int i,off;
    SetUndo();
	//if(Application->MessageBox("Metasprite will be cleared out!","CLEAR METASPRITE: Are you sure?",MB_YESNO)==IDYES)
	//{
		off=metaSpriteActive*64*4;

		for(i=0;i<64*4;i+=4) metaSprites[off+i]=255;

		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	//}

	//todo: this if statement was done in preparation of new copy/cut/paste routines for individual sprites.
	//todo: fix problem with "Sender"
		//if(Sender==Cut1Click)LabelStats->Caption="MetaSprite cut.";
		//else LabelStats->Caption="MetaSprite cleared.";

		//StatusUpdateWaiter->Enabled=true;
		//holdStats=true;
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSpriteDelClick(TObject *Sender)
{
	int i,j,off,cnt;

	//if(Application->MessageBox("Sprite(s) will be deleted!","DELETE SPRITE(S): Are you sure?",MB_YESNO)==IDYES)
	//{
		SetUndo();

		cnt=0;

		for(i=0;i<ListBoxSpriteList->Items->Count;++i)
		{
			if(ListBoxSpriteList->Selected[i])
			{
				off=metaSpriteActive*64*4+i*4;

				for(j=0;j<4;++j) metaSprites[off+j]=255;

				++cnt;
			}
		}

		for(i=0;i<cnt;++i) squeeze_sprites();

		for(i=0;i<ListBoxSpriteList->Items->Count;++i) ListBoxSpriteList->Selected[i]=false;

		//UpdateMetaSprite();
		cueUpdateMetasprite=true;

		//todo: this if statement was done in preparation of new copy/cut/paste routines for individual sprites.
		//todo: fix problem with "sender"
		//if(Sender==Cut1Click)LabelStats->Caption="Sprite object(s) cut.";
		//else LabelStats->Caption="Sprite object(s) cleared.";

		//StatusUpdateWaiter->Enabled=true;
		//holdStats=true;

		isLastClickedMetaSprite=false;
		isLastClickedSpriteList=true;
	//}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ListBoxSpriteListClick(TObject *Sender)
{
	SelectSprite(ListBoxSpriteList->ItemIndex,true);
	isLastClickedMetaSprite=false;
	isLastClickedSpriteList=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonFrameSelectedClick(TObject *Sender)
{
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSpriteUpClick(TObject *Sender)
{
	unsigned char temp[4];
	int off=metaSpriteActive*64*4;
	int lastSelectedItem;


	if(!ListBoxSpriteList->Selected[0])
	{
		SetUndo();

		//get the _lowest_ selected item
		for(int i=ListBoxSpriteList->Items->Count-1;i>0;--i)
			if(ListBoxSpriteList->Selected[i]) {lastSelectedItem=i; break;}


		//do the up shift of one or multiple items
		for(int i=0;i<ListBoxSpriteList->Items->Count;++i)
		{
			if(ListBoxSpriteList->Selected[i])
			{
				memcpy( temp            ,&metaSprites[i*4-4+off],4);
				memcpy(&metaSprites[i*4-4+off],&metaSprites[i*4  +off],4);
				memcpy(&metaSprites[i*4  +off], temp            ,4);

			if (i<ListBoxSpriteList->Items->Count&&i>0) //second condition is a redundancy but we keep it
				ListBoxSpriteList->Selected[i-1]=ListBoxSpriteList->Selected[i];
			}
		}
		--spriteActive;
	ListBoxSpriteList->Selected[lastSelectedItem]=false;
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
	}
	/*
    //original routine
	off=metaSpriteActive*64*4+spriteActive*4;

	if(spriteActive>0)
	{
		if(metaSprites[off]<255)
		{
			memcpy( temp              ,&metaSprites[off-4],4);
			memcpy(&metaSprites[off-4],&metaSprites[off  ],4);
			memcpy(&metaSprites[off  ], temp              ,4);

			--spriteActive;

			//UpdateMetaSprite();
			cueUpdateMetasprite=true;
		}
	}

	SelectSprite(spriteActive,false);
	*/
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSpriteDownClick(TObject *Sender)
{
	unsigned char temp[4];
	int off=metaSpriteActive*64*4;
	int firstSelectedItem;
	int max = ListBoxSpriteList->Items->Count-1;

	if(!ListBoxSpriteList->Selected[max])
	{
		SetUndo();

		//get the _lowest_ selected item
		for(int i=0;i<max;++i)
			if(ListBoxSpriteList->Selected[i]) {firstSelectedItem=i; break;}


		//do the down shift of one or multiple items
		for(int i=max;i>=0;--i)
		{
			if(ListBoxSpriteList->Selected[i])
			{
				memcpy( temp            ,&metaSprites[i*4+off],4);
				memcpy(&metaSprites[i*4+off],&metaSprites[i*4+4+off],4);
				memcpy(&metaSprites[i*4+4+off], temp            ,4);

			if (i<max) //
				ListBoxSpriteList->Selected[i+1]=ListBoxSpriteList->Selected[i];
			}


		}
		++spriteActive;
		ListBoxSpriteList->Selected[firstSelectedItem]=false;
		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	}


	/*
	//original routine
	off=metaSpriteActive*64*4+spriteActive*4;

	if(spriteActive<63)
	{
	SetUndo();
		if(metaSprites[off+4]<255)
		{
			memcpy( temp              ,&metaSprites[off  ],4);
			memcpy(&metaSprites[off  ],&metaSprites[off+4],4);
			memcpy(&metaSprites[off+4], temp              ,4);

			++spriteActive;

			//UpdateMetaSprite();
			cueUpdateMetasprite=true;
		}
	}

	SelectSprite(spriteActive,false);*/
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ListBoxSpriteListKeyDown(TObject *Sender, WORD &Key,
TShiftState Shift)
{
int max = ListBoxSpriteList->Items->Count-1;

	if (!Shift.Contains(ssCtrl))
	{
		FormKeyDown(Sender,Key,Shift);
		Key=0;
	}
	else //quickfix for selecting multiple items downwards. i should probably refactor sprite selection at some point but this works.
	{
		if(Key==VK_DOWN&&Shift.Contains(ssShift))
		if(ListBoxSpriteList->Items->Count)
		if(!ListBoxSpriteList->Selected[max])
		{
			++spriteActive;
			ListBoxSpriteList->Selected[spriteActive]=true;

			Key=0;
			//UpdateMetaSprite();
			cueUpdateMetasprite=true;
		}
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageMetaSpriteMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int i,off;

	if(IsBlockDrawing()) return;

	if(Shift.Contains(ssLeft)&&!Shift.Contains(ssCtrl))
	{
		i=GetSpriteID(X,Y);

		if(i>=0)
		{
			SelectSprite(i,false);
		}
	}
	if(Shift.Contains(ssLeft)&&Shift.Contains(ssCtrl)&&!Shift.Contains(ssRight))
	{
       //begin dragging anchor
	   SetUndo();

	}

	if(Shift.Contains(ssRight)&&!Shift.Contains(ssLeft))
	{
		i=GetSpriteID(X,Y);

		if(i>=0)
		{
			SelectSprite(i,false);
		}

		off=metaSpriteActive*64*4+spriteActive*4;

		if(metaSprites[off]<255)
		{
			SetUndo();

			spriteDragX=metaSprites[off+3]-X/uiScale;
			spriteDragY=metaSprites[off+0]-Y/uiScale;
		}

		spriteDrag=i;
	}

	ImageMetaSpriteMouseMove(Sender,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageMetaSpriteMouseLeave(TObject *Sender)
{
	spriteHover=-1;
	spriteDrag=-1;
	//UpdateStats();
	cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageMetaSpriteMouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{
	int x,y,off;

	if(IsBlockDrawing()) return;

	if(Shift.Contains(ssRight)&&spriteDrag>=0)
	{
		off=metaSpriteActive*64*4+spriteDrag*4;

		if(metaSprites[off]<255)
		{
			x=X/uiScale+spriteDragX;
			y=Y/uiScale+spriteDragY;

			if(x<0)   x=0;
			if(x>120) x=120;
			if(y<0)   y=0;
			if(y>120) y=120;

			metaSprites[off+3]=x;
			metaSprites[off+0]=y;

			//UpdateMetaSprite();
			cueUpdateMetasprite=true;
		}
	}

	if(Shift.Contains(ssLeft)&&Shift.Contains(ssCtrl))
	{
		spriteGridX=(X/uiScale)&~7;
		spriteGridY=(Y/uiScale)&~7;

		//original was 8 but that's sort of a mistake.
		if(spriteGridX<0) spriteGridX=0;
		if(spriteGridY<0) spriteGridY=0;

		if(spriteGridX>=128) spriteGridX=128;
		if(spriteGridY>=128) spriteGridY=128;
		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	}

	spriteHover=GetSpriteID(X,Y);

	//UpdateStats();
	cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageMetaSpriteMouseUp(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(SpeedButtonSpriteSnap->Down)
	{
		SpriteSnap(spriteActive);

		//UpdateMetaSprite();
		cueUpdateMetasprite=true;

	}

	spriteDrag=-1;
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MOpenMetaSpriteBankClick(TObject *Sender)
{
	BlockDrawing(true);
	
	if(OpenDialogMetaSpriteBank->Execute())
	{
		if(OpenMetaSprites(OpenDialogMetaSpriteBank->FileName))
		{
			OpenDialogMetaSpriteBank->FileName=RemoveExt(OpenDialogMetaSpriteBank->FileName);

			SaveDialogMetaSpriteBank->FileName=OpenDialogMetaSpriteBank->FileName;
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveMetaSpriteBankClick(TObject *Sender)
{
	FILE *file;
	unsigned char data[2];
	AnsiString name;

	BlockDrawing(true);
	
	if(SaveDialogMetaSpriteBank->Execute())
	{
		name=RemoveExt(SaveDialogMetaSpriteBank->FileName)+".msb";

		if(!OverwritePrompt(name))
		{
			BlockDrawing(false);
			return;
		}

		file=fopen(name.c_str(),"wb");

		if(file)
		{
			data[0]=spriteGridX;
			data[1]=spriteGridY;

			fwrite(data,2,1,file);
			fwrite(metaSprites,256*64*4,1,file);
			fclose(file);
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MMetaSpritePutBankToClipboardCClick(TObject *Sender)
{
	char *str,buf[1024];
	char strBuf[256];

	int i,j,pp,off,size,str_size;
    bool ask=false;
	if(AskBankName1->Checked) ask=true;

	AnsiString name;
    //BROKE STUDIO
	//FG:	Name is already implied by session filename
	//FormName->EditName->Text=metaSpriteBankName;

	//
	if(ask)
	{
		FormName->Caption="Bank label";
		FormName->EditName->Text=RemoveExt(ExtractFileName(SaveDialogSession->FileName));
		FormName->ShowModal();
		name=RemoveExt(ExtractFileName(FormName->EditName->Text));
		//name=FormName->EditName->Text;
		FormName->Caption="Metasprite label"; //return to normal
	}
	else name=RemoveExt(ExtractFileName(SaveDialogSession->FileName));
	//BROKE STUDIO
	metaSpriteBankName=name;
	//
	str_size=65536;

	str=(char*)malloc(str_size);

	str[0]=0;

	for(i=0;i<256;++i)
	{
		off=i*64*4;

		if(metaSprites[off]<255)
		{
			//old implied method
			//sprintf(buf,"const unsigned char %s_%i_data[]={\n",name.c_str(),i);
			//BROKE STUDIO's method. This might need to be a preferences setting or something.
			sprintf(buf,"const unsigned char %s_%s_data[]={\n",name.c_str(),metaSpriteNames[i]);
			//
			int sprCnt = 0;
			int offBuf = off;
			for(int k=0;k<64;++k) if(metaSprites[offBuf]<255) {++sprCnt; offBuf+=4;}

			if(sprCnt>=10) sprintf(strBuf," %d", sprCnt);
			else 		   sprintf(strBuf,"  %d", sprCnt);
			strcat(strBuf,",\n");

			strcat(str,buf);   // new sprite declaration
			if(Spritecountheader1->Checked) strcat(str,strBuf); // add sprite count

			for(j=0;j<64;++j)			//lay down a line
			{
				if(metaSprites[off]==255) continue;

				if(j) strcat(str,",\n");
				if(!(j%4)) strcat(str,"\n");

				sprintf(buf,"\t%s,%s,0x%2.2x,%i",num_to_3char(metaSprites[off+3]-spriteGridX),num_to_3char(metaSprites[off+0]-spriteGridY),metaSprites[off+1],metaSprites[off+2]&3);

				strcat(str,buf);

				if(metaSprites[off+2]&OAM_B2) strcat(str,"|B2");
				if(metaSprites[off+2]&OAM_B3) strcat(str,"|B3");
				if(metaSprites[off+2]&OAM_B4) strcat(str,"|B4");
				if(metaSprites[off+2]&OAM_FLIP_H) strcat(str,"|OAM_FLIP_H");
				if(metaSprites[off+2]&OAM_FLIP_V) strcat(str,"|OAM_FLIP_V");
				if(metaSprites[off+2]&OAM_PRIO) strcat(str,"|OAM_PRIO");

				off+=4;
			}

			if(Nflagterminator1->Checked)	 strcat(str,",\n\t0x80\n\n};\n\n");
			if(FFterminator1->Checked)		 strcat(str,",\n\t0xff\n\n};\n\n");
			if(Single00terminator1->Checked) strcat(str,",\n\t0\n\n};\n\n");
			if(Double00terminator1->Checked) strcat(str,",\n\t0,0\n\n};\n\n");
			if(Spritecountheader1->Checked)  strcat(str,"\n\n};\n\n");

			//strcat(str,",\n\t128");
			//strcat(str,"\n\n};\n\n");
		}

		if((int)strlen(str)>=str_size/2)
		{
			str_size+=65536;
			str=(char*)realloc(str,str_size);
		}
	}

	sprintf(buf,"const unsigned char* const %s_list[]={\n\n",name.c_str());

	strcat(str,buf);

	pp=0;

	for(i=0;i<256;++i)
	{
		off=i*64*4;

		if(metaSprites[off]<255)
		{
			if(pp) strcat(str,",\n");
            //BROKE STUDIO
			sprintf(buf,"\t%s_%s_data",name.c_str(),metaSpriteNames[i]);
			//
            //same as earlier in this routine; this might need to be a preferences setting
			//sprintf(buf,"\t%s_%i_data",name.c_str(),i);

			strcat(str,buf);

			size=1;

			for(j=0;j<64;++j)
			{
				if(metaSprites[off]==255) break;

				off+=4;
				size+=4;
			}

			pp+=size;
		}
	}

	strcat(str,"\n\n};\n\n");

	Clipboard()->SetTextBuf(str);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MMetaSpritePutToClipboardCClick(TObject *Sender)
{
	char str[65536],buf[1024];
	int i,j,x,y,off;

	off=metaSpriteActive*64*4;
	bool ask=false;
	AnsiString name;
	if(AskMetaName1->Checked) ask=true;
	if(ask)
	{
        FormName->EditName->Text=RemoveExt(ExtractFileName(SaveDialogSession->FileName));
		FormName->ShowModal();
		name=RemoveExt(ExtractFileName(FormName->EditName->Text));
	}
	else name=RemoveExt(ExtractFileName(SaveDialogSession->FileName));


	sprintf(buf, "  %d", ListBoxSpriteList->Items->Count);
	strcat(buf,",\n");

	if(metaSprites[off]<255)
	{
		if(ask)sprintf(str,"const unsigned char %s[]={\n",name.c_str());
		else sprintf(str,"const unsigned char %s[]={\n",metaSpriteNames[metaSpriteActive]);
		if(Spritecountheader1->Checked)
		{
			strcat(str,"\t");
			strcat(str,buf);
		}
		for(i=0;i<64;++i)
		{
			if(i) strcat(str,",\n");

			if(metaSprites[off]==255) break;
			if(!(i%4)) strcat(str,"\n");
			sprintf(buf,"\t%s,%s,0x%2.2x,%i",num_to_3char(metaSprites[off+3]-spriteGridX),num_to_3char(metaSprites[off]-spriteGridY),metaSprites[off+1],metaSprites[off+2]&3);

			strcat(str,buf);

			if(metaSprites[off+2]&OAM_B2) strcat(str,"|B2");
			if(metaSprites[off+2]&OAM_B3) strcat(str,"|B3");
			if(metaSprites[off+2]&OAM_B4) strcat(str,"|B4");
			if(metaSprites[off+2]&OAM_FLIP_H) strcat(str,"|OAM_FLIP_H");
			if(metaSprites[off+2]&OAM_FLIP_V) strcat(str,"|OAM_FLIP_V");
			if(metaSprites[off+2]&OAM_PRIO) strcat(str,"|OAM_PRIO");
			off+=4;
		}

		if(Nflagterminator1->Checked)	 strcat(str,"\t0x80\n};");
		if(FFterminator1->Checked)		 strcat(str,"\t0xff\n};");
		if(Single00terminator1->Checked) strcat(str,"\t0\n};");
		if(Double00terminator1->Checked) strcat(str,"\t0,0\n};");
		if(Spritecountheader1->Checked)  strcat(str,"};");

		strcat(str,"\n\n");
		Clipboard()->SetTextBuf(str);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSpriteHFlipClick(TObject *Sender)
{
	FlipMetaSprites(true,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSpriteVFlipClick(TObject *Sender)
{
	FlipMetaSprites(false,true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMetaSpriteCopyClick(TObject *Sender)
{
	MetaSpriteCopy();
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMetaSpritePasteClick(TObject *Sender)
{
	MetaSpritePaste();
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMetaSpriteHFlipClick(TObject *Sender)
{
	int i,x,off;

	SetUndo();

	off=metaSpriteActive*64*4;

	for(i=0;i<64;++i)
	{
		if(metaSprites[off]==255) break;

		x=-8-(metaSprites[off+3]-spriteGridX);

		metaSprites[off+3]=spriteGridX+x;
		metaSprites[off+2]^=OAM_FLIP_H;

		off+=4;
	}

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMetaSpriteVFlipClick(TObject *Sender)
{
	int i,y,off;

	SetUndo();
	
	off=metaSpriteActive*64*4;

	for(i=0;i<64;++i)
	{
		if(metaSprites[off]==255) break;

		y=-8-(metaSprites[off+0]-spriteGridY);

		metaSprites[off+0]=spriteGridY+y;
		metaSprites[off+2]^=OAM_FLIP_V;

		off+=4;
	}

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRFindUnusedClick(TObject *Sender)
{
	if(MCHRFindUnused->Checked)
	{
		for(int i=0;i<256;++i) chrSelected[i]=0;
		SelectTile(tileActive);
		MCHRFindUnused->Checked=false;
		MCHRFindUnused->Caption="Find &unused";
        UpdateAll();
	}
	else
	{
		FindDoublesUnused(true);
		MCHRFindUnused->Checked=true;
		MCHRFindUnused->Caption="Deselect Find &unused";

	   LabelStats->Caption="Unused found. \n\nTip: Press again to Deselect.";
	   StatusUpdateWaiter->Enabled=true;
	   holdStats=true;


	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRRemoveUnusedClick(TObject *Sender)
{
	//FG: this, along with the other remove/sort actions, were modified to toggle between do/undo for the sake of fluidity.
	if(MCHRRemoveUnused->Checked)
	{
		Undo();
		MCHRRemoveUnused->Checked=false;
		MCHRRemoveUnused->Caption="Re&move unused";
	}
	else
	{
		RemoveDoublesUnused(true);
		MCHRRemoveUnused->Checked=true;
		MCHRRemoveUnused->Caption="Undo re&move unused";
    	LabelStats->Caption="Unused removed. \n\nTip: Press again to undo.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSprite8x16Click(TObject *Sender)
{
	SpeedButtonSpriteBank->Enabled=SpeedButtonSprite8x16->Down;

	//create 8x16 vs 8x8 indirection table

	int t;
	bool b=((SpeedButtonSprite8x16->Down==true)&&(ButtonForceView->Down==true));
	AnsiString str;

	if(b)
	{
		 if(Normal1->Checked) 				t = Normal1->Tag;
		 if(N8x16spritemode1->Checked)  	t = N8x16spritemode1->Tag;

		 if(Sortedbyfrequency1->Checked)  	t = Sortedbyfrequency1->Tag;
		 if(Sortedbydensity1->Checked)  	t = Sortedbydensity1->Tag;
		 if(Sortedbydetail1->Checked)  		t = Sortedbydetail1->Tag;
		 if(Sortedbyedge1->Checked)  		t = Sortedbyedge1->Tag;
		 if(Sortedbycurcol1->Checked)  		t = Sortedbycurcol1->Tag;

		 if(N4x12x21->Checked)  			t = N4x12x21->Tag;
		 if(N4x12x2topbottom1->Checked)  	t = N4x12x2topbottom1->Tag;
		 if(N16tiles4x41->Checked)  		t = N16tiles4x41->Tag;

		 tileViewTagBuf = t;
		 SetViewTable(1);
		 str="Tileset (8x16 sprite mode)";

	}
	else
	{
		if(Normal1->Checked) 			str = "Tileset";
		if(N8x16spritemode1->Checked)  	str = "Tileset (8x16 mode)";

		if(Sortedbyfrequency1->Checked) str = "Tileset (by frequency)";
		if(Sortedbydensity1->Checked)  	str = "Tileset (by density)";
		if(Sortedbydetail1->Checked)  	str = "Tileset (by detail)";
		if(Sortedbyedge1->Checked)  	str = "Tileset (by edge content)";
		if(Sortedbycurcol1->Checked)  	str = "Tileset (by selected colour)";

		if(N4x12x21->Checked)  			str = "Tileset (by 4x1 -> 2x2)";
		if(N4x12x2topbottom1->Checked)  str = "Tileset (by 4x1 -> 2x2 top-down)";
		if(N16tiles4x41->Checked)  		str = "Tileset (by 16x1 -> 4x4)";


		SetViewTable(tileViewTagBuf);
		//revert to the checked option
	}

	GroupBoxTiles->Caption=str;

	//UpdateMetaSprite();
	UpdateTiles(true);
	cueUpdateMetasprite=true;
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSpriteBankClick(TObject *Sender)
{
	int i,off;

	SetUndo();
	
	for(i=0;i<ListBoxSpriteList->Items->Count;++i)
	{
		if(ListBoxSpriteList->Selected[i])
		{
			off=metaSpriteActive*64*4+i*4;

			if(metaSprites[off]<255) metaSprites[off+1]^=1;
		}
	}

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
	isLastClickedMetaSprite=false;
	isLastClickedSpriteList=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSpriteGridClick(TObject *Sender)
{
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MMetaSpritePutToClipboardAsmClick(TObject *Sender)
{
	char str[65536],buf[1024];
	int i,j,x,y,off,sprCnt;
    bool ask=false;
	if(AskMetaName1->Checked) ask=true;
	AnsiString name;
	off=metaSpriteActive*64*4;
	sprCnt=ListBoxSpriteList->Items->Count;

    if(ask)
	{
        FormName->EditName->Text=RemoveExt(ExtractFileName(SaveDialogSession->FileName));
		FormName->ShowModal();
		name=RemoveExt(ExtractFileName(FormName->EditName->Text));
	}
	else name=RemoveExt(ExtractFileName(SaveDialogSession->FileName));
    //^Redundant by now, i expect. just waiting until all changes are done, and then i'll probably forget about this line.
	if(byte1->Checked)
		if(sprCnt>=10)	sprintf(buf, "\n\t.byte  %d", sprCnt);
		else            sprintf(buf, "\n\t.byte   %d", sprCnt);
	if(db1->Checked)
		if(sprCnt>=10)  sprintf(buf, "\n\t.db  %d", sprCnt);
		else		    sprintf(buf, "\n\t.db   %d", sprCnt);

	if(metaSprites[off]<255)
	{
		//sprintf(str,"metasprite:\n");
		if (ask) sprintf(str,"%s:\n", name.c_str());
		else sprintf(str,"%s:\n",metaSpriteNames[metaSpriteActive]);
		if(Spritecountheader1->Checked) strcat(str,buf);
		for(i=0;i<64;++i)
		{
			strcat(str,"\n");

			if(metaSprites[off]==255) break;

			if(!(i%4)) strcat(str,"\n");

			if(byte1->Checked) sprintf(buf,"\t.byte %s,%s,$%2.2x,%i",num_to_3char(metaSprites[off+3]-spriteGridX),num_to_3char(metaSprites[off]-spriteGridY),metaSprites[off+1],metaSprites[off+2]&3);
			if(db1->Checked)   sprintf(buf,"\t.db %s,%s,$%2.2x,%i",	 num_to_3char(metaSprites[off+3]-spriteGridX),num_to_3char(metaSprites[off]-spriteGridY),metaSprites[off+1],metaSprites[off+2]&3);

			strcat(str,buf);

			if(metaSprites[off+2]&OAM_B2) strcat(str,"|B2");
			if(metaSprites[off+2]&OAM_B3) strcat(str,"|B3");
			if(metaSprites[off+2]&OAM_B4) strcat(str,"|B4");
			if(metaSprites[off+2]&OAM_FLIP_H) strcat(str,"|OAM_FLIP_H");
			if(metaSprites[off+2]&OAM_FLIP_V) strcat(str,"|OAM_FLIP_V");
			if(metaSprites[off+2]&OAM_PRIO) strcat(str,"|OAM_PRIO");

			off+=4;
		}
		if(!(Spritecountheader1->Checked||Noterminator1->Checked))
		{
			if(byte1->Checked) strcat(str,"\n\t.byte ");
			if(db1->Checked) strcat(str,"\n\t.db ");
		}
		if(Nflagterminator1->Checked)	 strcat(str,"$80\n");
		if(FFterminator1->Checked)		 strcat(str,"$ff\n");
		if(Single00terminator1->Checked) strcat(str,"  0\n");
		if(Double00terminator1->Checked) strcat(str,"  0,  0\n");

		strcat(str,"\n\n");

		Clipboard()->SetTextBuf(str);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MMetaSpritePutBankToClipboardAsmClick(
TObject *Sender)
{
	char str[65536],buf[1024],hdrBuf[256];
	int i,j,off;
	bool ask=false;
	if(AskBankName1->Checked) ask=true;
	AnsiString name;

	if(ask)
	{
        FormName->EditName->Text=RemoveExt(ExtractFileName(SaveDialogSession->FileName));
		FormName->ShowModal();
		name=RemoveExt(ExtractFileName(FormName->EditName->Text));
	}
	else //name=SaveDialogSession->FileName;
	name=RemoveExt(ExtractFileName(SaveDialogSession->FileName));
	//
	str[0]=0;

	for(i=0;i<256;++i)
	{
		off=i*64*4;

		if(metaSprites[off]<255)
		{
			//sprintf(buf,"%s_%i_data:\n\n",name.c_str(),i);
			sprintf(buf,"%s_%s_data:\n\n",name.c_str(),metaSpriteNames[i]);
			strcat(str,buf); //lay down ID

			//in case we want a header byte
			int sprCnt = 0;
			int offBuf = off;
			for(int k=0;k<64;++k) if(metaSprites[offBuf]<255) {++sprCnt; offBuf+=4;}

			if(byte1->Checked)
				if(sprCnt>=10)	sprintf(hdrBuf, "\n\t.byte  %d\n", sprCnt);
				else            sprintf(hdrBuf, "\n\t.byte   %d\n", sprCnt);
			if(db1->Checked)
				if(sprCnt>=10)  sprintf(hdrBuf, "\n\t.db  %d", sprCnt);
				else		    sprintf(hdrBuf, "\n\t.db   %d", sprCnt);
			if(Spritecountheader1->Checked) strcat(str,hdrBuf);

			for(j=0;j<64;++j)
			{
				if(metaSprites[off]==255) continue;

				if(j) strcat(str,"\n");

				if(!(j%4)) strcat(str,"\n");

				if(byte1->Checked) sprintf(buf,"\t.byte %s,%s,$%2.2x,%i",num_to_3char(metaSprites[off+3]-spriteGridX),num_to_3char(metaSprites[off+0]-spriteGridY),metaSprites[off+1],metaSprites[off+2]&3);
				if(db1->Checked)   sprintf(buf,"\t.db %s,%s,$%2.2x,%i",num_to_3char(metaSprites[off+3]-spriteGridX),num_to_3char(metaSprites[off+0]-spriteGridY),metaSprites[off+1],metaSprites[off+2]&3);


				strcat(str,buf);

				/*
				//todo:
				//unfinished option to cast as binary written constant

				char undefBits = metaSprites[off+2]&(OAM_B2|OAM_B3|OAM_B4)

				if (undefBits)
				{
					strcat(str,"|%");
					strcat(str,IntToStrAsBin(undefBits);
				}
				*/

				if(metaSprites[off+2]&OAM_B2) strcat(str,"|B2");
				if(metaSprites[off+2]&OAM_B3) strcat(str,"|B3");
				if(metaSprites[off+2]&OAM_B4) strcat(str,"|B4");
				if(metaSprites[off+2]&OAM_FLIP_H) strcat(str,"|OAM_FLIP_H");
				if(metaSprites[off+2]&OAM_FLIP_V) strcat(str,"|OAM_FLIP_V");
				if(metaSprites[off+2]&OAM_PRIO) strcat(str,"|OAM_PRIO");

				off+=4;
			}
			if(!(Spritecountheader1->Checked||Noterminator1->Checked))
			{
				if(byte1->Checked) strcat(str,"\n\t.byte ");
				if(db1->Checked) strcat(str,"\n\t.db ");
			}
			if(Nflagterminator1->Checked)	 strcat(str,"$80\n");
			if(FFterminator1->Checked)		 strcat(str,"$ff\n");
			if(Single00terminator1->Checked) strcat(str,"  0\n");
			if(Double00terminator1->Checked) strcat(str,"  0,  0\n");

			strcat(str,"\n\n");
		}
	}

	strcat(str,(name+"_pointers:\n").c_str());

	for(i=0;i<256;++i)
	{
		off=i*64*4;

		if(metaSprites[off]<255)
		{
			strcat(str,"\n");

			//if(byte1->Checked) sprintf(buf,"\t.word %s_%i_data",name.c_str(),i);
			//if(db1->Checked)   sprintf(buf,"\t.dw %s_%i_data",name.c_str(),i);
			if(byte1->Checked) sprintf(buf,"\t.word %s_%s_data",name.c_str(),metaSpriteNames[i]);
			if(db1->Checked)   sprintf(buf,"\t.dw %s_%s_data",name.c_str(),metaSpriteNames[i]);

			strcat(str,buf);

			for(j=0;j<64;++j)
			{
				if(metaSprites[off]==255) break;

				off+=4;
			}
		}
	}

	strcat(str,"\n\n");

	Clipboard()->SetTextBuf(str);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StaticTextFontOffsetMouseDown(TObject *Sender,
TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssLeft)) prevMouseY=Y;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StaticTextFontOffsetMouseMove(TObject *Sender,
TShiftState Shift, int X, int Y)
{
	int delta;

	if(Shift.Contains(ssLeft))
	{
		delta=prevMouseY-Y;

		if(Shift.Contains(ssShift))
		{
			typeInFontOffset+=1*(delta); //hold shift to fine adjust
		}
		else
		{
			typeInFontOffset+=16*(delta/2); //original behaviour
		}


		if(typeInFontOffset<0x00) typeInFontOffset=0x00;
		if(typeInFontOffset>0xff) typeInFontOffset=0x00;   //was f0

		StaticTextFontOffset->Caption="$"+IntToHex(typeInFontOffset,2);

		prevMouseY=Y;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ImageTilesDragOver(TObject *Sender, TObject *Source,
int X, int Y, TDragState State, bool &Accept)
{
	bool b=FormCHREditor->btn2x2mode->Down;
	bool bSmallWdt=chrSelection.right-chrSelection.left<2?1:0;
	bool bSmallHgt=chrSelection.bottom-chrSelection.top<2?1:0;

	Accept=false;

	cueStats=true;

	if((PageControlEditor->ActivePage==TabSheetName
		||PageControlEditor->ActivePage==TabSheetSprite)
		&&Source==ImageTiles)
	{
		if(X>=0&&X<(128*uiScale)&&Y>=0&&Y<(128*uiScale))
		{
			tileXC=X/(8*uiScale);
			tileYC=Y/(8*uiScale);
		}
		Accept=true;
	}

	if(PageControlEditor->ActivePage==TabSheetTile&&Source==ImageBigTiles)
	{
		if(X>=0&&X<(256*uiScale)&&Y>=0&&Y<(256*uiScale))
		{
			tileXC=X/(16*uiScale);
			tileYC=Y/(16*uiScale);
		}
		Accept=true;
	}


	if (Accept==true)
	{
		bImageNameAccepted=false;
		bImageTileAccepted=true;
		if (!bOutsideSel&&(!bSmallWdt||!bSmallHgt))
		{
			destRect.left	=chrSelection.left		+tileXC-txDown;
			destRect.right	=chrSelection.right		+tileXC-txDown;
			destRect.top	=chrSelection.top		+tileYC-tyDown;
			destRect.bottom	=chrSelection.bottom	+tileYC-tyDown;

			for (int i=0; i<16; i++)  //long enough loop
				{
					if(destRect.left<0)   	{	destRect.left++;
												destRect.right++;}
					if(destRect.right>0x10)	{	destRect.left--;
												destRect.right--;}
					if(destRect.top<0)   	{	destRect.top++;
												destRect.bottom++;}
					if(destRect.bottom>0x10){	destRect.top--;
												destRect.bottom--;}
				}

		}
        //old behaviour. Now redundancies but let´s keep them.
		else if(b)
		{
            destRect.left=tileXC;
			destRect.top=tileYC;
			if(tileXC>=15) destRect.left=14; 
			if(tileYC>=15) destRect.top=14;
			destRect.right=tileXC+2;
			destRect.bottom=tileYC+2;
		}
		else
		{
			destRect.left=tileXC;
			destRect.top=tileYC;
			destRect.right=tileXC+1;
			destRect.bottom=tileYC+1;
		}
		bDrawDestShadow=true;
		cueUpdateTiles=true;
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageTilesDragDrop(TObject *Sender, TObject *Source,
int X, int Y)
{
	//If same src and dest, exit & don´t set Undo.
	if(destRect.left==chrSelection.left&&destRect.top==chrSelection.top) return;

	//REMOVE THIS when swaps work in 8x16 mode!!
   //	if(SpeedButtonSprite8x16->Down) return;


	int set = bankActive/16;
	uint32_t propt[sizeof(tileProperties)];
	int propt_el = 1*sizeof(uint32_t);


	unsigned char tempchr[16];
	int tile,ps,pd, ba, offset;


	bool b2x2 = FormCHREditor->btn2x2mode->Down;

	bool bForceActive = RepairActiveTab1->Checked;
	bool bNT = RepairNT1->Checked;
	bool bMS = RepairMSPR1->Checked;

	bool bClone = ( bBufCtrl && !bBufShift &&  bBufAlt);
	bool bSwap	= (!bBufCtrl && !bBufShift && !bBufAlt);
	bool bMove	= ( bBufCtrl && !bBufShift && !bBufAlt);

	int w=1;   //init to single tile
	int h=1;

	if (!bOutsideSel) //if grabbed from inside selection, retain size
	{
		w=destRect.right-destRect.left;
		h=destRect.bottom-destRect.top;
	}
	else if (b2x2) { //last case, 2x2 edit mode.
		w=2;
		h=2;
	}


	if (bForceActive) {  //overrules the other options if "force: work on active tab only" is set.
		bMS=PageControlEditor->ActivePage==TabSheetSprite?true:false;
		bNT=!bMS;
	}

	SetUndo();

	X/=(8*uiScale);
	Y/=(8*uiScale);

	if(X<0||X>=(8*uiScale)||Y<0||Y>=(8*uiScale)) return;


	tile=destRect.top*16+destRect.left;

	//ps=tileActive*16+bankActive;
	//pd=tile*16+bankActive;
	ps=tileActive;
	pd=tile;
	ba=bankActive;

	const int tw=16;   //tileset table width

	int xSource = tileViewTable[tileActive&(tw-1)];
	int ySource = tileViewTable[tileActive/tw];

	if(nullTile==tileActive)  //TODO_02_2023: check if correct assignment in 8x16 mode
	{
		nullTile=tile;
		TextNullTile->Caption="   null tile: $"+IntToHex(nullTile,2)+" ";
	}

	for(int sy=0; sy<h*tw; sy+=tw) {
		for(int sx=0; sx<w; sx++) {
			if (ySource>=Y) {
				if   (xSource>X) offset=sx+(sy);
				else 			 offset=(w-1)-sx+(sy);
			}
			else {
				if 	 (xSource>X) offset=sx+((h-1)*tw)-(sy);
				else 			 offset=(w-1)-sx+((h-1)*tw)-(sy);
			}


			//swap
			if(bSwap){
				//memcpy(tempchr			   , &chr[pd+offset]	,t);
				//memcpy(&chr		[pd+offset], &chr[ps+offset]	,t);
				//memcpy(&chr		[ps+offset], tempchr			,t);
				memcpy(tempchr			   , &chr[ba+tileViewTable[pd+offset]*16]	,16);
				memcpy(&chr		[ba+tileViewTable[pd+offset]*16], &chr[ba+tileViewTable[ps+offset]*16]	,16);
				memcpy(&chr		[ba+tileViewTable[ps+offset]*16], tempchr			,16);

				if(RepairProps->Checked==true){
					memcpy(propt			   ,&tileProperties[set+tileViewTable[pd+offset]]	,propt_el);
					memcpy(&tileProperties		[set+tileViewTable[pd+offset]], &tileProperties[set+tileViewTable[ps+offset]]	,propt_el);
					memcpy(&tileProperties		[set+tileViewTable[ps+offset]], propt			,propt_el);
				}


			}

			//move
			if(bMove){
				memcpy(&chr		[ba+tileViewTable[pd+offset]*16], &chr[ba+tileViewTable[ps+offset]*16]	,16);
				memcpy(&chr		[ba+tileViewTable[ps+offset]*16], &chr[ba+tileViewTable[nullTile]*16]		,16);

				if(RepairProps->Checked==true){
					memcpy(&tileProperties		[set+tileViewTable[pd+offset]], &tileProperties[set+tileViewTable[ps+offset]]	,propt_el);
					memcpy(&tileProperties		[set+tileViewTable[ps+offset]], &tileProperties[set+tileViewTable[nullTile]]	,propt_el);

				}
			}

			//clone
			if(bClone){
				memcpy(&chr		[ba+tileViewTable[pd+offset]*16], &chr[ba+tileViewTable[ps+offset]*16]	,16);

				if(RepairProps->Checked==true){
					memcpy(&tileProperties		[set+tileViewTable[pd+offset]], &tileProperties[set+tileViewTable[ps+offset]]	,propt_el);

				}
			}

			if(bNT && (bSwap || bMove)){
				for(int i=0;i<nameTableWidth*nameTableHeight;++i)  //nametable reindex
				{
					//if(nameTable[i]==tileActive+offset/t) nameTable[i]=tile+offset/t;
					//else if(nameTable[i]==tile+offset/t) nameTable[i]=tileActive+offset/t;
					if(nameTable[i]==tileViewTable[ps+offset]) nameTable[i]=tileViewTable[pd+offset];
					else if(nameTable[i]==tileViewTable[pd+offset]) nameTable[i]=tileViewTable[ps+offset];
				}
			}
			if(bMS && (bSwap || bMove)){
				for (int i = 0; i < 64*256; i++) //metasprite bank reindex
				{
					if		(metaSprites[(i*4)+1]==tileViewTable[ps+offset]) metaSprites[(i*4)+1]=tileViewTable[pd+offset];
					else if (metaSprites[(i*4)+1]==tileViewTable[pd+offset])	   metaSprites[(i*4)+1]=tileViewTable[ps+offset];
				}
			}
		}
	}


	//SetTile(tile);
	SelectTile(tile); //destination is the new active tile.
	chrSelection.right=chrSelection.left+w;  //restore selection width/height (SetTile clobbers it).
	chrSelection.bottom=chrSelection.top+h;
	UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave1KCursorClick(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(tileActive*16,1024);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave2KCursorClick(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(tileActive*16,2048);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave4KClick(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(0,4096);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave8KClick(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(0,8192);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSaveSelectionClick(TObject *Sender)
{
	FILE *file;
	int i,size;
	AnsiString name;

	BlockDrawing(true);
	
	if(SaveDialogChr->Execute())
	{
		name=RemoveExt(SaveDialogChr->FileName)+".chr";

		size=0;

		for(i=0;i<256;++i) if(chrSelected[i]) size+=16;

		file=fopen(name.c_str(),"rb");

		if(file)
		{
			fseek(file,0,SEEK_END);
			i=ftell(file);
			fclose(file);

			if(Application->MessageBox((size==i?"Overwrite?":"Previous file has different size! Overwrite?"),"Confirm",MB_YESNO)!=IDYES)
			{
				BlockDrawing(false);
				return;
			}
		}

		file=fopen(name.c_str(),"wb");

		if(file)
		{
			for(i=0;i<256;i++)
			{
				if(chrSelected[i]) fwrite(chr+i*16+bankActive,16,1,file);
			}

			fclose(file);

			SetLatestCHR(0,size);
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutSelectedBlockToClipboardCClick(TObject *Sender)
{
	CopyMapCodeC(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutSelectedBlockToClipboardASMClick(TObject *Sender)
{
	CopyMapCodeASM();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutMetaSpriteToClipboardCNoFlipClick(TObject *Sender)
{
	CopyMetaSpriteCodeC(false);	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutMetaSpriteToClipboardCHFlipClick(TObject *Sender)
{
	CopyMetaSpriteCodeC(true);	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSaveLatestClick(TObject *Sender)
{
	BlockDrawing(true);

	switch(CHRLatestSize)
	{
	case 1024: SaveCHR(CHRLatestOffset,1024); break;
	case 2048: SaveCHR(CHRLatestOffset,2048); break;
	case 4096: SaveCHR(CHRLatestOffset,4096); break;
	case 8192: SaveCHR(CHRLatestOffset,8192); break;
	default: MCHRSaveSelectionClick(Sender);
	}

	BlockDrawing(false);	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutPaletteToClipboardAssemblyClick(TObject *Sender)
{
	char str[1024],buf[1024];
	int i,j;

	pal_validate();

	sprintf(str,"\tlda #$3f\n\tsta $2006\n\tlda #$00\n\tsta $2006\n\tldx #$%2.2x\n\tstx $2007\n",bgPal[palBank*16+0]);

	for(i=0;i<4;i++)
	{
		if(i) strcat(str,"\tstx $2007\n");
		for(j=1;j<4;j++)
		{
			sprintf(buf,"\tlda #$%2.2x\n\tsta $2007\n",bgPal[palBank*16+i*4+j]);
			strcat(str,buf);
		}
	}

	Clipboard()->SetTextBuf(str);	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutPaletteToClipboardASMClick(TObject *Sender)
{
	char str[1024],buf[1024];
	int i,j;

	//if(byte1->Checked) strcpy(str,".byte ");
	//if(db1->Checked)   strcpy(str,".db ");
	//sprintf(str,"palette:\n");
	AnsiString name;
	AnsiString bank;
	name=RemoveExt(ExtractFileName(SaveDialogSession->FileName));

	switch (palBank) {
	case 0: bank="a"; break;
	case 1: bank="b"; break;
	case 2: bank="c"; break;
	case 3: bank="d"; break;

	default: bank="a";
	}

	if((bExportPalFilename)&&(bExportPalSet))
		sprintf(str,"palette_%s_%s:\n",name.c_str(),bank.c_str());
	else if (bExportPalFilename)
		sprintf(str,"palette_%s:\n",name.c_str());
	else if (bExportPalSet)
		sprintf(str,"palette_%s:\n",bank.c_str());
	else
		sprintf(str,"palette:\n");





	for(i=0;i<4;i++)
	{
		if(byte1->Checked) sprintf(buf,".byte ");
		if(db1->Checked)   sprintf(buf,".db ");
        strcat(str,buf);
		for(j=0;j<4;j++)
		{
			//sprintf(buf,"$%2.2x%c",bgPal[palBank*16+i*4+j],i*4+j<15?',':'\n');
			sprintf(buf,"$%2.2x%c",bgPal[palBank*16+i*4+j],j<3?',':'\n');
			strcat(str,buf);
		}
	}

	Clipboard()->SetTextBuf(str);	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutPaletteToClipboardCClick(TObject *Sender)
{
	char str[1024],buf[1024];
	int i,j;
	AnsiString name;
	AnsiString bank;
	name=RemoveExt(ExtractFileName(SaveDialogSession->FileName));

	switch (palBank) {
	case 0: bank="a"; break;
	case 1: bank="b"; break;
	case 2: bank="c"; break;
	case 3: bank="d"; break;

	default: bank="a";
	}

	
	//strcpy
	if((bExportPalFilename)&&(bExportPalSet))
		sprintf(str,"const unsigned char palette_%s_%s[16]={ ",name.c_str(),bank.c_str());
	else if (bExportPalFilename)
		sprintf(str,"const unsigned char palette_%s[16]={ ",name.c_str());
	else if (bExportPalSet)
		sprintf(str,"const unsigned char palette_%s[16]={ ",bank.c_str());
	else
		sprintf(str,"const unsigned char palette[16]={ ");

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			sprintf(buf,"0x%2.2x%c",bgPal[palBank*16+i*4+j],i*4+j<15?',':' ');

			strcat(str,buf);
		}
	}

	strcat(str,"};\n\n");

	Clipboard()->SetTextBuf(str);	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormMouseWheel(TObject *Sender, TShiftState Shift,
int WheelDelta, TPoint &MousePos, bool &Handled)
{
    
	bool l=btnLine->Down;
	bool t=FormCHREditor->TimerScrollEvent->Enabled;
	bool bCtrl=Shift.Contains(ssCtrl)?true:false;
	//bool bShift=Shift.Contains(ssShift)?true:false;
	bool bAlt=Shift.Contains(ssAlt)?true:false;

	int tmpD=WheelDelta/2;
	int absl;
	if(l &&(t==false))
	{
		if(!bCtrl){
			if(bAlt)   //just alt pressed
			{
				if(tmpD<0) lineToolAlt++;
				else lineToolAlt--;
			}
			else{     //nothing pressed
				lineToolAlt=0;
				lineToolX=0;
				lineToolY=0;
				if(tmpD<0) lineToolRoster--;
				else lineToolRoster++;

				for (int i = 0; i < 3; i++) {
					absl = abs(lineToolRoster);
					if (lineRosterEnable[absl]==false)
					{
						if(tmpD<0) lineToolRoster--;
						else lineToolRoster++;
					}
				}
				if (lineToolRoster > 3) lineToolRoster = 3;
				if (lineToolRoster < -3) lineToolRoster = -3;
			}
		}
		else  //ctrl is assumed
		{
           if(bAlt)   //ctrl + alt
			{
				if(tmpD<0) lineToolX++;
				else lineToolX--;
			}
			else     //just ctrl
			{
				if(tmpD<0) lineToolY++;
				else lineToolY--;
			}
		}
		ImageTilesMouseMove(Sender,Shift,CHR_mmX,CHR_mmY);
		FormLineDetails->Repaint();

		FormCHREditor->TimerScrollEvent->Enabled;

	}
	else
	{


	if(WheelDelta<0)
	{
		if(Shift.Contains(ssCtrl)) palActive=(palActive-1)&3;
		else
		{
			//if(Shift.Contains(ssShift)) nameTableViewX+=1;
			if(Shift.Contains(ssShift)) nameTableViewX+=1;

			else nameTableViewY+=1;
		}

	}
	else
	{
		//bgPalCur=(bgPalCur+1)&3;
		if(Shift.Contains(ssCtrl)) palActive=(palActive+1)&3;
		else
		{
			if(Shift.Contains(ssShift)) nameTableViewX-=1;
			else nameTableViewY-=1;
			//UpdateNameTable(-1,-1,true);
		}
	}

	if(WheelDelta<-120&&(!Shift.Contains(ssCtrl)))
	{
		 if(Shift.Contains(ssShift)) nameTableViewX+=7;
		 else nameTableViewY+=7;
	}
	if(WheelDelta>120&&(!Shift.Contains(ssCtrl)))
	{
		 if(Shift.Contains(ssShift)) nameTableViewX-=7;
		 else nameTableViewY-=7;
	}
    }
	pal_validate();
	CorrectView();
    
	DrawPalettes();
	//UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	cueUpdateTiles=true;
	//cueUpdateNametable=true;
	//FormNavigator->CorrectNT->Enabled=true;
	//FormNavigator->Draw(false,false);

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;

	//UpdateStats();
	cueStats=true;
	Handled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPutSelectedBlockToClipboardCRLEClick(
TObject *Sender)
{
	CopyMapCodeC(true);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MPutSelectionToMetaspriteClick(TObject *Sender)
{
	int i,j,x,y,w,h,sx,sy,tx,ty,ptr,spr,tile;
	bool bIsNT = false;
	int ox=64;      //middle of canvas
	int oy=64;
	bool is8x16 = SpeedButtonSprite8x16->Down;
	int ystep = is8x16? 16:8;
	int bank = SpeedButtonChrBank2->Down? 1:0;

	SetUndo();

	if(Anchor1->Checked)
		{
		ox=spriteGridX;
		oy=spriteGridY;
		}
	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		GetSelection(nameSelection,x,y,w,h);
		bIsNT = true;
	}
	else
	{
		GetSelection(chrSelection,x,y,w,h);
    }
		spr=0;
		ptr=metaSpriteActive*64*4;

		if(center2->Checked)	sy=oy-(h*8/2);
		if(top1->Checked)		sy=oy;
		if(bottom1->Checked)	sy=oy-(h*8);

		for(i=0;i<h;i++)
		{
			if(center1->Checked)	sx=ox-(w*8/2);
			if(left1->Checked)		sx=ox;
			if(right1->Checked)		sx=ox-(w*8);

			for(j=0;j<w;j++)
			{
				tx=x+j;
				ty=y+i;


				if (bIsNT) tile=nameTable[ty*nameTableWidth+tx];
				else 	   tile=tileViewTable[ty*16+tx];
				if(is8x16) tile=(tile & ~1)+bank;

				if(tile!=nullTile||!MPutSelectionToMetaspriteSkipZero->Checked)
				{
					if(MPutSelectionToMetaspriteMerge->Checked)
					{
						while(spr<64)
						{
							if(metaSprites[ptr]==255) break;
							ptr+=4;
							++spr;
						}
					}

					if(spr<64)
					{
						if(sy==255) metaSprites[ptr+0]=254; //safeguard against autodeleting sprite item
						else metaSprites[ptr+0]=sy;
						metaSprites[ptr+1]=tile;
						if (bIsNT)metaSprites[ptr+2]=AttrGet(tx,ty,false,false);
						else metaSprites[ptr+2]=palActive;
						metaSprites[ptr+3]=sx;

						ptr+=4;

						++spr;
					}
				}

				sx+=8;
			}

			if(is8x16) i++;
			sy+=ystep;
		}

		if(!MPutSelectionToMetaspriteMerge->Checked)
		{
			while(spr<64)
			{
				metaSprites[ptr]=255;
				ptr+=4;
				++spr;
			}
		}

		if(MPutSelectionToMetaspriteAutoInc->Checked) if(metaSpriteActive<255) ++metaSpriteActive;

		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	/*}
	else
	{
	   GetSelection(nameSelection,x,y,w,h);
	   LabelStats->Caption="WARNING: No metasprite was made:\nNeeds a selection on the map/nametable.";
	   StatusUpdateWaiter->Enabled=true;
	   holdStats=true;
	} */
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSpriteAllClick(TObject *Sender)
{
	int i;

	for(i=0;i<ListBoxSpriteList->Items->Count;++i) ListBoxSpriteList->Selected[i]=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSpriteNoneClick(TObject *Sender)
{
	int i;

	for(i=0;i<ListBoxSpriteList->Items->Count;++i) ListBoxSpriteList->Selected[i]=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPalDragOver(TObject *Sender, TObject *Source,
int X, int Y, TDragState State, bool &Accept)
{
	//UpdateStats();
	cueStats=true;
	if(Source==PaintBoxPal) Accept=true; else Accept=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPalDragDrop(TObject *Sender, TObject *Source,
int X, int Y)
{
	int n,entry;

	n=check_palette_mouse(X,Y);

	if(n>=0&&!(n&0x100))
	{
		SetUndo();

		entry=(n&3)*4+(n>>4);

		if(!(entry&3)) entry=0;

		if(acceptPalDrag)
		{
			bgPal[palBank*16+(palDragVal&3)*4+(palDragVal>>4)]=bgPal[palBank*16+entry];
			acceptPalDrag=false;
		}
		bgPal[palBank*16+entry]=palDragColor;

		pal_validate();

		DrawPalettes();
		UpdateTiles(true);
		UpdateNameTable(-1,-1,true);
		FormNavigator->Draw(false,false);
		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MView2xClick(TObject *Sender)
{
	if((Sender==MView2x)&&(uiScale==2)) return;
	if((Sender==MView3x)&&(uiScale==3)) return;
	if((Sender==MView4x)&&(uiScale==4)) return;

	if(Sender==MView2x) uiScale=2;
	if(Sender==MView3x) uiScale=3;
	if(Sender==MView4x) uiScale=4;

	UpdateUIScale();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MReplaceTileClick(TObject *Sender)
{
	int i,j,x,y,w,h,off;

	FormNameOffset->Mode=1;
	FormNameOffset->ShowModal();

	if(FormNameOffset->MakeOffset)
	{
		if(FormNameOffset->SelectedOnly)
		{
			x=-1;

			if(nameSelection.left>=0&&nameSelection.top>=0)
			{
				x=nameSelection.left;
				y=nameSelection.top;
				w=nameSelection.right-nameSelection.left;
				h=nameSelection.bottom-nameSelection.top;
			}
		}
		else
		{
			x=0;
			y=0;
			w=nameTableWidth;
			h=nameTableHeight;
		}

		if(x>=0)
		{
			SetUndo();

			for(i=0;i<h;++i)
			{
				for(j=0;j<w;++j)
				{
					off=(y+i)*nameTableWidth+(x+j);

					if(nameTable[off]==FormNameOffset->From) nameTable[off]=FormNameOffset->Offset;
				}
			}

			UpdateNameTable(-1,-1,true);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::toggleCheckClick(TObject *Sender)
{
	((TMenuItem*)Sender)->Checked^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRFreqSortClick(TObject *Sender)
{
	//FG: this, along with the other remove/sort actions, were modified to toggle between do/undo for the sake of fluidity.
	if(MCHRFreqSort->Checked)
	{
		Undo();
		MCHRFreqSort->Checked=false;

		MCHRFreqSort->Caption="Sort by &frequency";
	}
	else
	{
		bool forceActive = ForceActiveTab1->Checked;
		bool sweepNT = IncludeNametables1->Checked;
		bool sweepMS = IncludeMetasprites1->Checked;

		int i,j,temp;
		static int sort[256],freq[256];
		static unsigned char chrt[256*16];
		static uint32_t propt[sizeof(tileProperties)];
		SetUndo();

		memset(freq,0,sizeof(freq));

		if((forceActive)||!(sweepNT||sweepMS)) //if no sweep checkbox is set, also default to active.
		{
		   if(PageControlEditor->ActivePage==TabSheetSprite) {
                for(j=0;j<256*64*4;j+=4) if(metaSprites[j]!=0xFF) ++freq[metaSprites[j+1]];
			 }
		   else {
				for(i=0;i<nameTableHeight*nameTableWidth;++i) ++freq[nameTable[i]];
			}
		}
		else
		{
		   if(sweepNT) {
				for(i=0;i<nameTableHeight*nameTableWidth;++i) ++freq[nameTable[i]];
			}
		   if(sweepMS) {
				for(j=0;j<256*64*4;j+=4) if(metaSprites[j]!=0xFF) ++freq[metaSprites[j+1]];
			}
		}





		for(i=0;i<256;++i) sort[i]=i;

		for(i=0;i<256-1;++i)
		{
			for(j=0;j<256-i-1;++j)
			{
				if(freq[j]<freq[j+1])
				{
					temp=freq[j];
					freq[j]=freq[j+1];
					freq[j+1]=temp;

					temp=sort[j];
					sort[j]=sort[j+1];
					sort[j+1]=temp;
				}
			}
		}
		//repair NT
		for(i=0;i<nameTableWidth*nameTableHeight;++i)
		{
			for(j=0;j<256;++j)
			{
				if(sort[j]==nameTable[i])
				{
					nameTable[i]=j;
					break;
				}
			}
		}
		//repair MSP.
		for(int i=0;i<256*64*4;i+=4)
		{
			for(j=0;j<256;++j)
			{
				if(sort[j]==metaSprites[i+1]) {metaSprites[i+1]=j; break;}
			}
		}

		//perform chr sort & property sort

		memcpy(chrt,&chr[bankActive],sizeof(chrt));
		memcpy(propt,&tileProperties[bankActive/16],sizeof(propt));

		for(i=0;i<256;++i)
		{
			memcpy(&chr[bankActive+i*16],&chrt[sort[i]*16],16);
			memcpy(&tileProperties[(bankActive/16)+i],&propt[sort[i]],sizeof(uint32_t));
		}






		UpdateAll();
		MCHRFreqSort->Checked=true;
		MCHRFreqSort->Caption="Undo Sort by &frequency";
		LabelStats->Caption="Tiles sorted by Frequency. \n\nTip: Press again to undo.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;

	}



}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MNameTableNewClick(TObject *Sender)
{
	int x,y,pp,pal,mask;

	bool useNull;

		FormSetSize->ShowModal();
		if (!FormSetSize->Confirm) return;
		useNull = FormSetSize->CheckNullTile->Checked;

	
	//if(Application->MessageBox("Are you sure?","Confirm",MB_YESNO)!=IDYES) return;

	memcpy(tmpNameTable,nameTable,sizeof(nameTable));
	memcpy(tmpAttrTable,attrTable,sizeof(attrTable));

	SetUndo();

	int oldWdt=nameTableWidth;
	int oldHgt=nameTableHeight;


	nameTableWidth =FormSetSize->NewWidth;
	nameTableHeight=FormSetSize->NewHeight;


	int newWdt=nameTableWidth;
	int newHgt=nameTableHeight;

	bool isNarrow	= oldWdt > newWdt
					? true : false;

	bool isTall		= oldHgt < newHgt
					? true : false;

	ClearNametable(useNull);




	if(!FormSetSize->CheckBoxClear->Checked)
	{
		for(y=0;y<oldHgt;y++)
		{

			for(x=0;x<oldWdt;x++)
			{
				if(isNarrow && isTall) if (y==oldHgt-1 && x>=newWdt) continue;  //ignores an excess remainder on the following row.
				nameTable[y*newWdt+x]=tmpNameTable[y*oldWdt+x];

			}
		}

		for(y=0;y<oldHgt;y+=2)
		{
			for(x=0;x<oldWdt;x+=2)
			{
				if(isNarrow && isTall) if (y==oldHgt-1 && x>=newWdt) continue;
				pal=tmpAttrTable[y/4*((oldWdt+3)/4)+x/4];

				if(x&2) pal>>=2;
				if(y&2) pal>>=4;

				pp=y/4*((newWdt+3)/4)+x/4;

				mask=3;
				pal=pal&3;

				if(x&2)
				{
					pal<<=2;
					mask<<=2;
				}
				if(y&2)
				{
					pal<<=4;
					mask<<=4;
				}

				attrTable[pp]=(attrTable[pp]&(mask^255))|pal;
			}
		}

	}

	nameSelection.left=-1;
	nameSelection.top=-1;
	nameCopyWidth=-1;
	nameCopyHeight=-1;

	//SetUndo();
	UpdateAll();
	if(FormNavigator->Height > GetSystemMetrics(SM_CYFULLSCREEN)) FormNavigator->Height  = GetSystemMetrics(SM_CYFULLSCREEN) + 10;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveMapClick(TObject *Sender)
{
	unsigned char meta[4];
	unsigned char* buf; //for RLE compressing maps.
	unsigned char* dst; //^
	bool bRLE;
	AnsiString name,filter;
	FILE *file;

	BlockDrawing(true);

	//
				
	//
	if(SaveDialogMap->Execute())
	{
		if(SaveDialogMap->FilterIndex==2||MSaveRLE->Checked) {bRLE=true; name=RemoveExt(SaveDialogMap->FileName)+".mrle";}
		else {bRLE=false; name=RemoveExt(SaveDialogMap->FileName)+".map";}

		if(FileExists(name))
		{
			if(Application->MessageBox(("File "+name+" already exists. Overwrite?").c_str(),"Confirm",MB_YESNO)!=IDYES)
			{
				BlockDrawing(false);
				return;
			}
		}

		meta[0]=nameTableWidth&255;
		meta[1]=nameTableWidth/256;
		meta[2]=nameTableHeight&255;
		meta[3]=nameTableHeight/256;

		file=fopen(name.c_str(),"wb");

		if(file)
		{
			//
			if(bRLE){
				buf=(unsigned char*)malloc(name_size()+attr_size());
				int size=0;

				if(MSaveIncName->Checked)
				{
					memcpy(buf,nameTable,name_size());
					size+=name_size();
				}

				if(MSaveIncAttr->Checked)
				{
					memcpy(buf+size,attrTable,attr_size());
					size+=attr_size();
				}
				//pack buf as RLE to dst
				dst=(unsigned char*)malloc(size*2);
				size=encode_rle(buf,size,dst);
				//handle if compression is impossible.
				if(size<0){
					Application->MessageBox("No unused values found (tiles and/or attributes), can't be saved as RLE due to limitations of the format.","Error",MB_OK);
					free(dst);
					free(buf);
					fclose(file);
					BlockDrawing(false);
					return;
				}
				//fwrite dst as RLE here
				fwrite(dst,size,1,file);
				free(dst);
				free(buf);
			}
			//
			else{
				fwrite(nameTable,name_size(),1,file);
				fwrite(attrTable,attr_size(),1,file);
			}
			fwrite(meta,sizeof(meta),1,file);
			fclose(file);
		}
	
    }
	BlockDrawing(false);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SpeedButtonSpriteDupClick(TObject *Sender)
{
	int i,j,len,src_ptr,dst_ptr;


	
	dst_ptr=metaSpriteActive*64*4;
	len=ListBoxSpriteList->Items->Count;

	if(len)SetUndo();

	for(i=0;i<len;++i)
	{
		if(ListBoxSpriteList->Selected[i])
		{
			src_ptr=metaSpriteActive*64*4+i*4;

			while(1)
			{
				if(metaSprites[dst_ptr]==255) break;

				dst_ptr+=4;
			}

			if(dst_ptr<metaSpriteActive*64*4+64*4)
			{
				for(j=0;j<4;++j) metaSprites[dst_ptr+j]=metaSprites[src_ptr+j];
				dst_ptr+=4;
			}
		}
	}

	spriteActive=len;

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;

	for(i=0;i<ListBoxSpriteList->Items->Count;++i)
	{
		ListBoxSpriteList->Selected[i]=(i<len)?false:true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonPalBankAClick(TObject *Sender)
{
	MPalA->Checked=false;
	MPalB->Checked=false;
	MPalC->Checked=false;
	MPalD->Checked=false;

	if(SpeedButtonPalBankA->Down) {palBank=0; MPalA->Checked=true;}
	if(SpeedButtonPalBankB->Down) {palBank=1; MPalB->Checked=true;}
	if(SpeedButtonPalBankC->Down) {palBank=2; MPalC->Checked=true;}
	if(SpeedButtonPalBankD->Down) {palBank=3; MPalD->Checked=true;}

	ppuMask=ppuMaskSet[palBank];
	UpdateRGBM();
	pal_validate();
    palette_calc();

	DrawPalettes();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MCHRDensitySortClick(TObject *Sender)
{
	//FG: this, along with the other remove/sort actions, were modified to toggle between do/undo for the sake of fluidity.
	if(MCHRDensitySort->Checked)
	{
		Undo();
		MCHRDensitySort->Checked=false;
		MCHRDensitySort->Caption="Sort by d&ensity";
	}
	else
	{
		 int i,j,k,ptr,mask,temp;
		static int sort[256],freq[256];
		static unsigned char chrt[256*16];
		static uint32_t propt[sizeof(tileProperties)];
		SetUndo();

		for(i=0;i<256;++i)
		{
			sort[i]=i;
			freq[i]=0;

			ptr=i*16+bankActive;

			for(j=0;j<8;++j)
			{
				mask=128;

				for(k=0;k<8;++k)
				{
					if((chr[ptr]&mask)||(chr[ptr+8]&mask)) ++freq[i];

					mask>>=1;
				}

				++ptr;
			}
		}

		for(i=0;i<256-1;++i)
		{
			for(j=0;j<256-i-1;++j)
			{
				if(freq[j]<freq[j+1])
				{
					temp=freq[j];
					freq[j]=freq[j+1];
					freq[j+1]=temp;

					temp=sort[j];
					sort[j]=sort[j+1];
					sort[j+1]=temp;
				}
			}
		}
		//repair NT
		for(i=0;i<nameTableWidth*nameTableHeight;++i)
		{
			for(j=0;j<256;++j)
			{
				if(sort[j]==nameTable[i])
				{
					nameTable[i]=j;
					break;
				}
			}
		}
		//repair MSP.
		for(int i=0;i<256*64*4;i+=4)
		{
			for(j=0;j<256;++j)
			{
				if(sort[j]==metaSprites[i+1]) {metaSprites[i+1]=j; break;}
			}
		}


		memcpy(chrt,&chr[bankActive],sizeof(chrt));
		memcpy(propt,&tileProperties[bankActive/16],sizeof(propt));
		for(i=0;i<256;++i)
		{
			memcpy(&chr[bankActive+i*16],&chrt[sort[i]*16],16);
            memcpy(&tileProperties[(bankActive/16)+i],&propt[sort[i]],sizeof(uint32_t));
		}

		UpdateAll();
		MCHRDensitySort->Checked=true;
		MCHRDensitySort->Caption="Undo Sort by d&ensity";
		LabelStats->Caption="Tiles sorted by Density. \n\nTip: Press again to undo.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSelectTilesFromMapClick(TObject *Sender)
{
	int i,j,x,y,w,h,sx,sy,tx,ty,ptr,spr,tile;

	if(nameSelection.left<0||nameSelection.top<0) return;

	chrSelectRect=false;

	for(i=0;i<256;i++) chrSelected[i]=false;

	GetSelection(nameSelection,x,y,w,h);

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			tx=x+j;
			ty=y+i;

			tile=nameTable[ty*nameTableWidth+tx];


			chrSelected[ViewTablePos(tile)]=true;
		}
	}
    bOutsideSel=true;
	nameSelection.left=-1;
	nameSelection.top=-1;

	UpdateAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonMetaSpriteRotateClick(TObject *Sender)
{
	int i,x,y,off;

	SetUndo();
	
	off=metaSpriteActive*64*4;

	for(i=0;i<64;++i)
	{
		if(metaSprites[off]==255) break;

		y=-8-(metaSprites[off+3]-spriteGridX);
		x=(metaSprites[off+0]-spriteGridY);

		metaSprites[off+0]=spriteGridY+y;
		metaSprites[off+3]=spriteGridX+x;
		//metaSprites[off+2]^=(OAM_FLIP_H|OAM_FLIP_V);

		off+=4;
	}

	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MMetaSpriteManageClick(TObject *Sender)
{
	FormManageMetasprites->Visible^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MAddTileOffsetMetaSpritesClick(TObject *Sender)
{
	int i,j,off;

	FormNameOffset->Mode=0;
	FormNameOffset->ShowModal();

	if(FormNameOffset->MakeOffset)
	{
		SetUndo();

		for(i=0;i<256;++i)
		{
			for(j=0;j<64;++j)
			{
				off=i*64*4+j*4;

				if(metaSprites[off]<255) metaSprites[off+1]+=FormNameOffset->Offset;
			}
		}

		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MImportBMPMatchTilesClick(TObject *Sender)
{
	BlockDrawing(true);
	
	OpenDialogImport->Title="Import BMP to canvas; matched to existing tileset";
	OpenDialogImport->Filter="Windows bitmap files (*.bmp)|*.bmp|All files (*.*)|*.*";
	OpenDialogImport->DefaultExt="bmp";

	if(OpenDialogImport->Execute())
	{
		SetUndo();

		if(ImportBMP(OpenDialogImport->FileName,IMPORT_MATCH,0,0,256,false,false))
		{
			UpdateAll();
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRGenerate4x4Click(TObject *Sender)
{
	int i,j,k,bit,c1,c2,pp;

	if(Application->MessageBox("Do you really want to fill 4K CHR with chunks?","Confirm",MB_YESNO)==ID_YES)
	{
		SetUndo();

		pp=0;

		/*	for(i=0;i<16;i++)
		{
			for(j=0;j<16;j++)
			{
				bit=j|(i<<4);

				for(k=0;k<8;k++)
				{
					c1=0;
					c2=0;

					if(k<4)
					{
						if(bit&0x1) c1|=0x0f;
						if(bit&0x2) c2|=0x0f;
						if(bit&0x4) c1|=0xf0;
						if(bit&0x8) c2|=0xf0;
					}
					else
					{
						if(bit&0x10) c1|=0x0f;
						if(bit&0x20) c2|=0x0f;
						if(bit&0x40) c1|=0xf0;
						if(bit&0x80) c2|=0xf0;
					}

					chr[pp+0+k]=c1;
					chr[pp+8+k]=c2;
				}

				pp+=16;
			}
		}
		*/
		for(i=0;i<16;i++)
		{
			for(j=0;j<16;j++)
			{
				bit=j|(i<<4);

				for(k=0;k<8;++k)
				{
					c1=0;
					c2=0;

					if(k<4)
					{
						if(bit&0x01) c1|=0x03;
						if(bit&0x02) c1|=0x0c;
						if(bit&0x04) c1|=0x30;
						if(bit&0x08) c1|=0xc0;
					}
					else
					{
						if(bit&0x10) c1|=0x03;
						if(bit&0x20) c1|=0x0c;
						if(bit&0x40) c1|=0x30;
						if(bit&0x80) c1|=0xc0;
					}

					switch(i/4)
					{
					case 0: break;
					case 1: c2=c1; c1=0; break;
					case 2: c2=c1; c1=0xff; break;
					case 3: c2=0xff; break;
					}

					chr[pp+0+k]=c1;
					chr[pp+8+k]=c2;
				}

				pp+=16;
			}
		}
		memcpy(&chr[4096],chr,4096);

		UpdateTiles(true);
		UpdateNameTable(-1,-1,true);
		FormNavigator->Draw(false,false);
		//UpdateMetaSprite();
		cueUpdateMetasprite=true;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MImportBMPIntoNumberOfTilesClick(TObject *Sender)
{
	int mode;

	FormInputNumber->ShowModal();

	if(!FormInputNumber->Confirm) return;

	OpenDialogImport->Title="Import BMP file to canvas";
	OpenDialogImport->Filter="Windows bitmap files (*.bmp)|*.bmp|All files (*.*)|*.*";
	OpenDialogImport->DefaultExt="bmp";

	BlockDrawing(true);

	if(OpenDialogImport->Execute())
	{
		SetUndo();

		if(ImportBMP(OpenDialogImport->FileName,IMPORT_SQUEEZE,false,-1,FormInputNumber->Number,MImportNoColorData->Checked,MImportNoColorData->Checked))
		{
			UpdateAll();
		}
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPaletteResetGrayscaleClick(TObject *Sender)
{
	//if(Application->MessageBox("Reset palette?","Confirm",MB_YESNO)!=IDYES) return;
	SetUndo();
	memcpy(&bgPal[palBank*16],&bgPalDefault[((TMenuItem*)Sender)->Tag*16],16);

	DrawPalettes();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPaletteCopyClick(TObject *Sender)
{
	PaletteCopy();	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MPalettePasteClick(TObject *Sender)
{
	PalettePaste();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TimerBlockTimer(TObject *Sender)
{
	BlockDrawingFlag=false;
	TimerBlock->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave1KTile00Click(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(0x00*16,1024);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave1KTile40Click(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(0x40*16,1024);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave1KTile80Click(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(0x80*16,1024);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave1KTileC0Click(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(0xC0*16,1024);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave2KTile00Click(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(0x00*16,2048);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCHRSave2KTile80Click(TObject *Sender)
{
	BlockDrawing(true);
	SaveCHR(0x80*16,2048);
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MAddXYOffsetMetaSpritesClick(TObject *Sender)
{
	int i,j,off,from,to;

	FormMetaspriteOffset->ShowModal();

	if(FormMetaspriteOffset->MakeOffset)
	{
		SetUndo();

		if(FormMetaspriteOffset->SelectedOnly)
		{
			from=metaSpriteActive;
			to=from+1;
		}
		else
		{
			from=0;
			to=256;
		}

		for(i=from;i<to;++i)
		{
			off=i*4*64;

			for(j=0;j<64;++j)
			{
				if(metaSprites[off]<255)
				{
					metaSprites[off+3]+=FormMetaspriteOffset->OffsetX;
					metaSprites[off+0]+=FormMetaspriteOffset->OffsetY;

					off+=4;
				}
			}
		}

		//UpdateMetaSprite();
    	cueUpdateMetasprite=true;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::UndoRedo1Click(TObject *Sender)
{
	Undo();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Cut1Click(TObject *Sender)
{
	int i,off;
	AnsiString tmp;

	if(FormCHREditor->Active==true)
	{
		if(Sender==Cut1) {CopyCHR(true,true); tmp="CHR";}
		if(Sender==Copy1) CopyCHR(true,false);
		if(Sender==Paste1) PasteCHR();
	}
	else
	{
		if(PageControlEditor->ActivePage==TabSheetSprite&&isLastClickedMetaSprite)
		{
			if(Sender==Cut1)
			{
                    SetUndo();
					MetaSpriteCopy();

					off=metaSpriteActive*64*4;
					for(i=0;i<64*4;i+=4) metaSprites[off+i]=255;
					//UpdateMetaSprite();
					cueUpdateMetasprite=true;
					tmp="MetaSprite #"+num_to_3char(metaSpriteActive);
				}
				if(Sender==Copy1) MetaSpriteCopy() ;
				if(Sender==Paste1) MetaSpritePaste();

			}

		else if(PageControlEditor->ActivePage==TabSheetName||TabSheetTile
		||(PageControlEditor->ActivePage==TabSheetSprite&&!isLastClickedMetaSprite))
		{
			if(nameSelection.left<0&&nameSelection.top<0)
			{
				if(Sender==Cut1) {CopyCHR(true,true); tmp="CHR";}
				if(Sender==Copy1) CopyCHR(true,false);
				if(Sender==Paste1) PasteCHR();
			}
			else
			{
				if(Sender==Cut1) {CopyMap(true); tmp="Name(s)";}
				if(Sender==Copy1) CopyMap(false);
				if(Sender==Paste1) PasteMap();
			}
			if(PageControlEditor->ActivePage==TabSheetTile) UpdateNameTable(-1,-1,true);
		}
	}
    
	if ((Sender==Cut1)&&(holdStats==false))
	{
	LabelStats->Caption=LabelStats->Caption
		+"\n"
		+tmp
		+" was/were cut.";
	StatusUpdateWaiter->Enabled=true;
	holdStats=true;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Quit1Click(TObject *Sender)
{
	FormMain->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Tilegrid1Click(TObject *Sender)
{
	if(SpeedButtonTypeIn->Down) return;
	if(Sender==GridOnOff1||Sender==GridOnOff2)
	{
		SpeedButtonGridAll->Down^=true;
		GridOnOff1->Checked=SpeedButtonGridAll->Down;
		GridOnOff2->Checked=SpeedButtonGridAll->Down;
	}
	if(Sender==Screen32x301) {SpeedButtonGridScreen->Down^=true;
							Screen32x301->Checked=SpeedButtonGridScreen->Down;}

	if(Sender==Tilegrid1) {SpeedButtonGridTile->Down^=true;
							Tilegrid1->Checked=SpeedButtonGridTile->Down;}
	if(Sender==Tilegrid2) {SpeedButtonGridAtr->Down^=true;
							Tilegrid2->Checked=SpeedButtonGridAtr->Down;}
	if(Sender==Tilegrid4) {SpeedButtonGridBlock->Down^=true;
							Tilegrid4->Checked=SpeedButtonGridBlock->Down;}

	//((TMenuItem*)Sender)->Checked^=true;

	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	UpdateTiles(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Colour0Click(TObject *Sender)
{
	if(SpeedButtonTypeIn->Down) return;
	Colour001->Checked=false;
	Colour011->Checked=false;
	Colour101->Checked=false;
	Colour111->Checked=false;

	if((bgPalCurOld!=bgPalCur)&&(AutostoreLastUsed->Checked)) bgPalCurOld=bgPalCur;
	if(Sender==Colour001) {bgPalCur=0;}
	if(Sender==Colour011) {bgPalCur=1;}
	if(Sender==Colour101) {bgPalCur=2;}
	if(Sender==Colour111) {bgPalCur=3;}

	penActive=0;
	PenMode1->Checked=false;
	PenMode2->Checked=false;
	((TMenuItem*)Sender)->Checked^=true;
	//FormMain->
	UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::pal0Click(TObject *Sender)
{

	Pal0->Checked=false;
	Pal1->Checked=false;
	Pal2->Checked=false;
	Pal3->Checked=false;

	if((palActiveOld!=palActive)&&(AutostoreLastUsed->Checked)) palActiveOld=palActive;
	if(Sender==Pal0) {palActive=0;}
	if(Sender==Pal1) {palActive=1;}
	if(Sender==Pal2) {palActive=2;}
	if(Sender==Pal3) {palActive=3;}

	((TMenuItem*)Sender)->Checked^=true;

	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::PenModeClick(TObject *Sender)
{
	if(SpeedButtonTypeIn->Down) return;


	if(Sender==PenMode1) {penActive=1;}
	if(Sender==PenMode2) {penActive=2;}
	((TMenuItem*)Sender)->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SafeColoursClick(TObject *Sender)
{
	((TMenuItem*)Sender)->Checked^=true;
	safeColsOnly^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SharedBGcolClick(TObject *Sender)
{
	((TMenuItem*)Sender)->Checked^=true;
	sharedCol0^=true;
	UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Magnify1Click(TObject *Sender)
{
	//not in use or finished.. decided to wait
	/*
	system("magnify /lens")
	else
	{
	system("taskkill /F /IM "magnify.exe" /T")
	}
	*/
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ApplyTiles1Click(TObject *Sender)
{
   if (FormCHREditor->Active) return;
   if(SpeedButtonTypeIn->Down) return;

   //ApplyTiles1Click->Checked^=true;
   ((TMenuItem*)Sender)->Checked^=true;
   SpeedButtonTiles->Down^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ApplyAttributes1Click(TObject *Sender)
{
   if (FormCHREditor->Active) return;
   if(SpeedButtonTypeIn->Down) return;

   ((TMenuItem*)Sender)->Checked^=true;
   SpeedButtonPal->Down^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Toggletileset1Click(TObject *Sender)
{
	if(SpeedButtonTypeIn->Down) return;
	//These conditionals are necessary to bypass doubling up
	//on the speedbuttons behaving like radio buttons per default for this group
	if(SpeedButtonChrBank1->Down==true)SpeedButtonChrBank2->Down=true;
	else SpeedButtonChrBank1->Down=true;

	bankActive=SpeedButtonChrBank1->Down?0:4096;
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Attributes1Click(TObject *Sender)
{
	if (FormCHREditor->Active) return;
	if(SpeedButtonTypeIn->Down) return;
	((TMenuItem*)Sender)->Checked^=true;
	SpeedButtonChecker->Down^=true;
	SpeedButtonCheckerClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SelectedOnly1Click(TObject *Sender)
{

	if (FormCHREditor->Active) return;
 if(SpeedButtonTypeIn->Down) return;
 SpeedButtonSelTiles->Down^=true;
 SelectedOnly1->Checked=SpeedButtonSelTiles->Down;
 UpdateNameTable(-1,-1,true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MASCIIneg20hClick(TObject *Sender)
{
   MASCIIneg20h->Checked=false;
   MASCIIneg30h->Checked=false;
   MASCIIneg40h->Checked=false;

   if(Sender==MASCIIneg20h)	typeInASCIIOffset=32;
   if(Sender==MASCIIneg30h)	typeInASCIIOffset=48;
   if(Sender==MASCIIneg40h)	typeInASCIIOffset=64;

   ((TMenuItem*)Sender)->Checked^=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MPalAClick(TObject *Sender)
{
	MPalA->Checked=false;
	MPalB->Checked=false;
	MPalC->Checked=false;
	MPalD->Checked=false;
	SpeedButtonPalBankA->Down=false;
	SpeedButtonPalBankB->Down=false;
	SpeedButtonPalBankC->Down=false;
	SpeedButtonPalBankD->Down=false;

	if(Sender==MPalA) {palBank=0; SpeedButtonPalBankA->Down=true;}
	if(Sender==MPalB) {palBank=1; SpeedButtonPalBankB->Down=true;}
	if(Sender==MPalC) {palBank=2; SpeedButtonPalBankC->Down=true;}
	if(Sender==MPalD) {palBank=3; SpeedButtonPalBankD->Down=true;}

	((TMenuItem*)Sender)->Checked^=true;

	pal_validate();
	DrawPalettes();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::PutcurrenttableonclipboardasBMP1Click(
	  TObject *Sender)
{
	TPicture *picture;
	Graphics::TBitmap *bmp;
	int i,x,y;
	unsigned short picFormat;
	unsigned int cbHandle;
	//AnsiString str;

	BlockDrawing(true);

	picture=new TPicture();
	bmp=new Graphics::TBitmap();
	bmp->SetSize(128,128);


	bmp->PixelFormat=pf4bit;
	SetBMPPalette(bmp);
	picture->Bitmap=bmp;


	x=0;
	y=0;

	for(i=0;i<256;i++)
	{
		DrawExportTile16(picture,x,y,i,palActive,-1,-1,false);

		x+=8;

		if(x>=128)
		{
			x=0;
			y+=8;
		}
	}



	HPALETTE hPal;

	picture->SaveToClipboardFormat(picFormat, cbHandle, hPal);
	Clipboard()->SetAsHandle(picFormat, cbHandle);

	delete bmp;
	delete picture;

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MCopyMapAsBMP(TObject *Sender)
{
TPicture *picture;
	Graphics::TBitmap *bmp;
	int i,j,x,y,w,h,pp;
	int xOff=0;
	int yOff=0;


	unsigned short picFormat;
	unsigned int cbHandle;


	if(nameSelection.left>=0&&nameSelection.top>=0&&(Sender!=CopymapasBMP1))
	{
		GetSelection(nameSelection,x,y,w,h);

		BlockDrawing(true);

		picture=new TPicture();
		bmp=new Graphics::TBitmap();
		bmp->SetSize(w*8,h*8);
		bmp->PixelFormat=pf4bit;
		SetBMPPalette(bmp);
		picture->Bitmap=bmp;

		pp=0;

		for(i=0;i<h;++i)
		{
			for(j=0;j<w;++j)
			{
				nameCopy[pp]=nameTable[y*nameTableWidth+x+j];
				attrCopy[pp]=AttrGet(x+j,y,false,false);
				DrawExportTile16(
					picture,
					j*8,
					i*8,
					nameCopy[pp],
					attrCopy[pp],
					x+j,
					i,
					false
					);
			++pp;
		}
		++y;
	}

	}
	else
	{
		w=nameTableWidth;
		h=nameTableHeight;

        BlockDrawing(true);

		picture=new TPicture();
		bmp=new Graphics::TBitmap();
		bmp->SetSize((w/*-xOff*/)*8,(h/*-yOff*/)*8);
		bmp->PixelFormat=pf4bit;
		SetBMPPalette(bmp);
		picture->Bitmap=bmp;

		y=0;

		for(i=yOff;i<h;i++)
		{
			x=0;

			for(j=xOff;j<w;j++)
			{
				DrawExportTile16(
					picture,
					x,
					y,
					nameTable[i*w+j],
					AttrGet(j,i,false,false),
					j,
					i,
					false
					);
				x+=8;
			}
		y+=8;
		}
	}

	HPALETTE hPal;

	picture->SaveToClipboardFormat(picFormat, cbHandle, hPal);
	Clipboard()->SetAsHandle(picFormat, cbHandle);

	delete bmp;
	delete picture;


	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
	  if(Shift.Contains(ssLeft))
	  {
		prevMouseY=Y;
		prevMouseX=X;
	  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Red1Click(TObject *Sender)
{
	if(Sender==Red1) {
		SpeedButtonMaskRClick(Sender);
		}
	if(Sender==Blue1) {
		SpeedButtonMaskBClick(Sender);
		}
	if(Sender==Green1) {
		SpeedButtonMaskGClick(Sender);
		}
	if(Sender==Gray1) {
		SpeedButtonMaskMClick(Sender);
		}

	Red1->Checked=(ppuMask&0x20?true:false);
	Green1->Checked=(ppuMask&0x40?true:false);
	Blue1->Checked=(ppuMask&0x80?true:false);

	Gray1->Checked=(ppuMask&0x01?true:false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PerClickOrCont(TObject *Sender)
{

	IncDecPerclick1->Checked^=true;
	OverDistance1->Checked^=true;

	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::IncDecCap1Click(TObject *Sender)
{
	 IncDecCap1->Checked^=true;
	 IncDecWraparound1->Checked^=true;
}
//--------------------------------------------------------------------------


void __fastcall TFormMain::IncDecFlow1Click(TObject *Sender)
{
	//IncDecPerclick1->Checked=false;
	IncDecFlow1->Checked=false;
	IncDecFlow2->Checked=false;
	IncDecFlow3->Checked=false;
	IncDecFlow4->Checked=false;
	IncDecFlow5->Checked=false;

	((TMenuItem*)Sender)->Checked=true;

	if(Sender==IncDecFlow1) {continousIncDecDuration=1;}
	if(Sender==IncDecFlow2) {continousIncDecDuration=3;}
	if(Sender==IncDecFlow3) {continousIncDecDuration=5;}
	if(Sender==IncDecFlow4) {continousIncDecDuration=7;}
	if(Sender==IncDecFlow5) {continousIncDecDuration=9;}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Setcheckpoint1Click(TObject *Sender)
{
	size_t tmpSize;


	for(int i=0;i<256;i++)
	{
		metaSpriteNamesCheckpoint[i]	=metaSpriteNames[i];
		metaSpriteNamesCheckpointUndo[i]=metaSpriteNamesUndo[i];
	}

     

	///put current state in checkpoint buffer
	memcpy(bgPalCheckpoint,bgPal,sizeof(bgPal));
	memcpy(checkpointChr,chr,8192);
	memcpy(checkpointNameTable,nameTable,name_size());
	memcpy(checkpointAttrTable,attrTable,attr_size());
	memcpy(checkpointMetaSprites,metaSprites,sizeof(checkpointMetaSprites));

	memcpy(checkpointTileProperties,tileProperties,sizeof(checkpointTileProperties));
	memcpy(checkpointPropConditional,propConditional,sizeof(checkpointPropConditional));
	memcpy(checkpointPropCondition,propCondition,sizeof(checkpointPropCondition));

	checkpointNameTableWidth   =  nameTableWidth;
	checkpointNameTableHeight  =  nameTableHeight;

	checkpointSpriteGridX	   =  spriteGridX;
	checkpointSpriteGridY      =  spriteGridY;

	//chr bank stuff pt1

	checkpoint_chrBanks = chrBanks;
	checkpoint_chrBankLabels = chrBankLabels;

	memcpy(checkpoint_chrA_id,chrA_id,16);
	memcpy(checkpoint_chrB_id,chrB_id,16);

	tmpSize = chrBanks*4096;
	if (tmpSize > checkpoint_chrBanks*4096) checkpoint_chrBank = (char*) realloc(checkpoint_chrBank, tmpSize);
	memcpy(checkpoint_chrBank, chrBank, tmpSize);



	//put undo buffer into "undo checkpoint" buffer
	memcpy(bgPalUndoCheckPoint,bgPalUndo,sizeof(bgPal));
	memcpy(undoCheckpointChr,undoChr,8192);
	memcpy(undoCheckpointNameTable,undoNameTable,name_size());
	memcpy(undoCheckpointAttrTable,undoAttrTable,attr_size());
	memcpy(undoCheckpointMetaSprites,undoMetaSprites,sizeof(checkpointMetaSprites));

	memcpy(checkpointTilePropertiesUndo,tilePropertiesUndo,sizeof(checkpointTilePropertiesUndo));
	memcpy(checkpointPropConditionalUndo,propConditionalUndo,sizeof(checkpointPropConditionalUndo));
	memcpy(checkpointPropConditionUndo,propConditionUndo,sizeof(checkpointPropConditionUndo));


	undoCheckpointNameTableWidth  = undoNameTableWidth;
	undoCheckpointNameTableHeight = undoNameTableHeight;

	undoCheckpointSpriteGridX  =  undoSpriteGridX;
	undoCheckpointSpriteGridY  =  undoSpriteGridY;

	//chr bank stuff pt2
	undo_checkpoint_chrBanks = undo_chrBanks;
	undo_checkpoint_chrBankLabels = undo_chrBankLabels;

	memcpy(undo_checkpoint_chrA_id,undo_chrA_id,16);
	memcpy(undo_checkpoint_chrB_id,undo_chrB_id,16);

	tmpSize = undo_chrBanks*4096;
	if (tmpSize > undo_checkpoint_chrBanks*4096) undo_checkpoint_chrBank = (char*) realloc(undo_checkpoint_chrBank, tmpSize);
	memcpy(undo_checkpoint_chrBank, undo_chrBank, tmpSize);


}

//---------------------------------------------------------------------------

void __fastcall TFormMain::Reverttocheckpoint1Click(TObject *Sender)
{
	int i,tmp;
	size_t tmpSize;
	int tmp_maxAttr, tmp_maxName;

	int tmp_AttrSize	,	tmp_checkpointAttrSize;
	int tmp_NameSize	, 	tmp_checkpointNameSize;


	AnsiString tmpAnsi;

	for(i=0;i<256;i++)
	{
		tmpAnsi=metaSpriteNames[i];
		metaSpriteNames[i]=metaSpriteNamesCheckpoint[i];
		metaSpriteNamesCheckpoint[i]=tmpAnsi;

		tmpAnsi=metaSpriteNamesUndo[i];
		metaSpriteNames[i]=metaSpriteNamesCheckpointUndo[i];
		metaSpriteNamesCheckpointUndo[i]=tmpAnsi;
	}




	//swap current and checkpoint states
	tmp_AttrSize = (nameTableWidth+3)/4*((nameTableHeight+3)/4);
	tmp_checkpointAttrSize = (checkpointNameTableWidth+3)/4*((checkpointNameTableHeight+3)/4);
	tmp_maxAttr = tmp_AttrSize > tmp_checkpointAttrSize
					? tmp_AttrSize : tmp_checkpointAttrSize;

	tmp_NameSize = nameTableWidth*nameTableHeight;
	tmp_checkpointNameSize = checkpointNameTableWidth*checkpointNameTableHeight;
	tmp_maxName = tmp_NameSize > tmp_checkpointNameSize
					? tmp_NameSize : tmp_checkpointNameSize;

	mem_exchange(bgPalCheckpoint,bgPal,sizeof(bgPal));
	mem_exchange(checkpointChr,chr,8192);
	mem_exchange(checkpointNameTable,nameTable,tmp_maxName);   //name_size()
	mem_exchange(checkpointAttrTable,attrTable,tmp_maxAttr);   //attr_size()
	mem_exchange(checkpointMetaSprites,metaSprites,sizeof(checkpointMetaSprites));

	mem_32exchange(checkpointTileProperties,tileProperties,sizeof(tileProperties));
	mem_exchange(checkpointPropConditional,propConditional,sizeof(propConditional));
	swap_bool_2d(checkpointPropCondition, propCondition, 4, 8);
	//mem_exchange(checkpointPropCondition,propCondition,sizeof(propCondition));




	tmp = nameTableWidth;   nameTableWidth = checkpointNameTableWidth;		checkpointNameTableWidth=tmp;
	tmp = nameTableHeight;  nameTableHeight = checkpointNameTableHeight;    checkpointNameTableHeight=tmp;

	tmp = spriteGridX;   spriteGridX = checkpointSpriteGridX;	 checkpointSpriteGridX=tmp;
	tmp = spriteGridY;   spriteGridY = checkpointSpriteGridY;    checkpointSpriteGridY=tmp;

	//chr bank stuff pt 1
	tmp=chrBanks;			chrBanks=checkpoint_chrBanks;			checkpoint_chrBanks=tmp;
	
	tmp_chrBankLabels	= chrBankLabels;               //vector swap
	chrBankLabels		= checkpoint_chrBankLabels;
	checkpoint_chrBankLabels	= tmp_chrBankLabels;

	mem_32exchange(checkpoint_chrA_id	,chrA_id	,sizeof(chrA_id));
	mem_32exchange(checkpoint_chrB_id	,chrB_id	,sizeof(chrB_id));

	tmpSize = max(chrBanks*4096,checkpoint_chrBanks*4096);

	if (tmpSize > chrBanks*4096)
		chrBank = (char*)realloc(chrBank,tmpSize);
	if (tmpSize > checkpoint_chrBanks*4096)
		checkpoint_chrBank = (char*)realloc(checkpoint_chrBank,tmpSize);

	mem_exchange(checkpoint_chrBank,	chrBank	,tmpSize);

	//swap current and checkpoint UNDO states
	//------------
	tmp_AttrSize = (undoNameTableWidth+3)/4*((undoNameTableHeight+3)/4);
	tmp_checkpointAttrSize = (undoCheckpointNameTableWidth+3)/4*((undoCheckpointNameTableHeight+3)/4);
	tmp_maxAttr = tmp_AttrSize > tmp_checkpointAttrSize
					? tmp_AttrSize : tmp_checkpointAttrSize;

	tmp_NameSize = undoNameTableWidth * undoNameTableHeight;
	tmp_checkpointNameSize = undoCheckpointNameTableWidth * undoCheckpointNameTableHeight;
	tmp_maxName = tmp_NameSize > tmp_checkpointNameSize
					? tmp_NameSize : tmp_checkpointNameSize;

	mem_exchange(bgPalUndoCheckPoint,bgPalUndo,sizeof(bgPal));
	mem_exchange(undoCheckpointChr,undoChr,8192);
	mem_exchange(undoCheckpointNameTable,undoNameTable,tmp_maxName);      //name_size()
	mem_exchange(undoCheckpointAttrTable,undoAttrTable,tmp_maxAttr);    //attr_size()
	mem_exchange(undoCheckpointMetaSprites,undoMetaSprites,sizeof(checkpointMetaSprites));

	mem_32exchange(checkpointTilePropertiesUndo,tilePropertiesUndo,sizeof(tilePropertiesUndo));
	mem_exchange(checkpointPropConditionalUndo,propConditionalUndo,sizeof(propConditionalUndo));
	swap_bool_2d(checkpointPropConditionUndo, propConditionUndo, 4, 8);
	//mem_exchange(checkpointPropConditionUndo,propConditionUndo,sizeof(propConditionUndo));


	tmp = undoNameTableWidth;   undoNameTableWidth = undoCheckpointNameTableWidth;		undoCheckpointNameTableWidth=tmp;
	tmp = undoNameTableHeight;  undoNameTableHeight = undoCheckpointNameTableHeight;    undoCheckpointNameTableHeight=tmp;

	tmp = undoSpriteGridX;  undoSpriteGridX = undoCheckpointSpriteGridX;	undoCheckpointSpriteGridX=tmp;
	tmp = undoSpriteGridY;  undoSpriteGridY = undoCheckpointSpriteGridY;    undoCheckpointSpriteGridY=tmp;

	//chr bank stuff pt 2
	tmp=undo_chrBanks;			undo_chrBanks=undo_checkpoint_chrBanks;			undo_checkpoint_chrBanks=tmp;

	tmp_chrBankLabels	= undo_chrBankLabels;               //vector swap
	undo_chrBankLabels		= undo_checkpoint_chrBankLabels;
	undo_checkpoint_chrBankLabels	= tmp_chrBankLabels;

	mem_32exchange(undo_checkpoint_chrA_id	,undo_chrA_id	,sizeof(chrA_id));
	mem_32exchange(undo_checkpoint_chrB_id	,undo_chrB_id	,sizeof(chrB_id));

	tmpSize = max(undo_chrBanks*4096,undo_checkpoint_chrBanks*4096);

	if (tmpSize > undo_chrBanks*4096)
		undo_chrBank = (char*)realloc(undo_chrBank,tmpSize);
	if (tmpSize > undo_checkpoint_chrBanks*4096)
		undo_checkpoint_chrBank = (char*)realloc(undo_checkpoint_chrBank,tmpSize);

	mem_exchange(undo_checkpoint_chrBank,	undo_chrBank	,tmpSize);








	//sanitize line drawing buffer
    memset (arr_linePlotCHR,0,sizeof(arr_linePlotCHR2));
	memset (arr_linePlotCHR2,0,sizeof(arr_linePlotCHR2));
	memcpy (chrBuf, chr, 8192);
	lineUpX= -1;
	lineUpY= -1;


	UpdateAll();


}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SpeedButtonSubpalCopyClick(TObject *Sender)
{
   cf_palRotation=0;  //reset subpalette paste rotation

   DWORD Len=4;
   HGLOBAL hDst=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,Len);

   if (hDst)
   {
		LPSTR gDst=(LPSTR)GlobalLock(hDst);
		if (gDst)
		{
			if (OpenClipboard(Handle))
			{
				EmptyClipboard();
				memcpy(gDst,&bgPal[palBank*16+palActive*4],4);

				SetClipboardData(CF_SUBPAL,hDst);
				CloseClipboard();
			}
		}
   }


}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSubpalPasteClick(TObject *Sender)
{


   OpenClipboard(Handle);

   HGLOBAL hClipBuf=GetClipboardData(CF_SUBPAL);

   if (hClipBuf)
   {
		LPSTR gSrc=(LPSTR)GlobalLock(hClipBuf);

		if (gSrc!=NULL)
		{
			SetUndo();

			memcpy(&bgPal[palBank*16+palActive*4],gSrc,4);


			GlobalUnlock(gSrc);
		}
   }

   CloseClipboard();
   UpdateAll();

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
		clickV=false;
		clickC=false;
		bIgnoreKey=false;
		bBufCtrl=Shift.Contains(ssCtrl)?true:false;
		bBufShift=Shift.Contains(ssShift)?true:false;
		bBufAlt=Shift.Contains(ssAlt)?true:false;

		bBufVK_3=false;
        if(PageControlEditor->ActivePage==TabSheetTile)
		{
			if(Key=='G') {btnSmudge->Down=false; FormCHREditor->btnSmudge->Down=false; bSmudge=false;}
		}
		if (bBufCtrl || bBufShift || bBufAlt)
		{
			//this is for updating when drag has been accepted.
			if (bImageNameAccepted) cueUpdateNametableNoRepaint=true;
			else if (bImageTileAccepted) 	cueUpdateTiles=true;
			//this is for any other case. Probably redundant for now.

			//else{cueUpdateNametable=true; cueUpdateTiles=true;}  //include if there´s a legitimate use.
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SelectAll1Click(TObject *Sender)
{
	int i;
	if(PageControlEditor->ActivePage==TabSheetSprite)
	{
		for(i=0;i<ListBoxSpriteList->Items->Count;++i) ListBoxSpriteList->Selected[i]=true;
        UpdateAll();
		return;
	}

	if(nameSelection.left>=0||nameSelection.top>=0
		|| (isLastClickedNametable==true && isLastClickedTiles==false))
	{
		if(	(nameSelection.right-nameSelection.left>=32
			&& nameSelection.bottom-nameSelection.top>=30)
			&& !(	 nameSelection.left  ==0
				  && nameSelection.top   ==0
				  && nameSelection.right ==nameTableWidth
				  && nameSelection.bottom==nameTableHeight)
			)
		{
			nameSelection.left  =0;
			nameSelection.top   =0;
			nameSelection.right =nameTableWidth;
			nameSelection.bottom=nameTableHeight;
		}
		else
        {
			nameSelection.left  =nameTableViewX;
			nameSelection.top   =nameTableViewY;
			nameSelection.right =nameTableViewX+32;
			nameSelection.bottom=nameTableViewY+30;

            for (int i=0; i<32; i++)  //long enough loop
			// I need to special-case if the canvas is smaller than the viewport.
				{
					if(nameSelection.left<0)   	{	nameSelection.left++;
												nameSelection.right++;}
					if(nameSelection.right>nameTableWidth)
											{	nameSelection.left--;
												nameSelection.right--;}
					if(nameSelection.top<0)   	{	nameSelection.top++;
												nameSelection.bottom++;}
					if(nameSelection.bottom>nameTableHeight)
											{	nameSelection.top--;
												nameSelection.bottom--;}
				}


		}

	}
	else
	{
		for(i=0;i<256;i++) chrSelected[i]=true;
		chrSelection.top	= 0;
		chrSelection.left	= 0;
		chrSelection.right	= 16;
		chrSelection.bottom	= 16;
        if(MCHRFindDoubles->Checked)
		{
			MCHRFindDoubles->Checked=false;
			MCHRFindDoubles->Caption="Find &duplicates";
		}

		if(MCHRFindUnused->Checked)
		{
			MCHRFindUnused->Checked=false;
			MCHRFindUnused->Caption="Find &unused";
		}
	}
	UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Deselect1Click(TObject *Sender)
{
	int i;
	if(PageControlEditor->ActivePage==TabSheetSprite)
	{
		for(i=0;i<ListBoxSpriteList->Items->Count;++i) ListBoxSpriteList->Selected[i]=false;
		UpdateAll();
		return;
	}
	if(nameSelection.left>=0||nameSelection.top>=0)
	{
        bOutsideSel=true;
		nameSelection.left  = -1;
		nameSelection.top   = -1;
		//nameSelection.right  = -1;
		//nameSelection.bottom   = -1;
	}
	else
	{
		for(i=0;i<256;i++) chrSelected[i]=false;
		chrSelected[0]=true;
		chrSelection.top	= 0;
		chrSelection.left	= 0;
		chrSelection.right	= 1;
		chrSelection.bottom	= 1;
	}
	if(MCHRFindDoubles->Checked)
	{
		MCHRFindDoubles->Checked=false;
		MCHRFindDoubles->Caption="Find &duplicates";
	}

	if(MCHRFindUnused->Checked)
	{
		MCHRFindUnused->Checked=false;
		MCHRFindUnused->Caption="Find &unused";
	}
	UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TextNullTileMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssLeft))
		{
			prevMouseY=Y;

			chrSelBuf.left		=	chrSelection.left;
			chrSelBuf.top		=	chrSelection.top;
			chrSelBuf.right		=	chrSelection.right;
			chrSelBuf.bottom  	=	chrSelection.bottom;

			chrSelection.left	=	nullTile%16;
			chrSelection.top   	=	nullTile/16;
			chrSelection.right 	=	chrSelection.left+1;
			chrSelection.bottom	=	chrSelection.top +1;
			chrSelectRect=true;
			UpdateTiles(true);
			UpdateNameTable(-1,-1,true);
			//UpdateStats();
			cueStats=true;

		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TextNullTileMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	  int i,delta;

	if(Shift.Contains(ssLeft))
	{
		delta=prevMouseY-Y;


		nullTile+=1*(delta/2);



		if(nullTile<0x00) nullTile=0xff;
		if(nullTile>0xff) nullTile=0x00;

		TextNullTile->Caption="   null tile: $"+IntToHex(nullTile,2)+" ";

		prevMouseY=Y;
		chrSelection.left  =nullTile%16;
		chrSelection.top   =nullTile/16;
		chrSelection.right =chrSelection.left+1;
		chrSelection.bottom=chrSelection.top +1;
		chrSelectRect=true;

		UpdateTiles(true);
		//i think these were unneccessary
		//UpdateNameTable(-1,-1,true);
		//FormNavigator->Draw(false);

		cueStats=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TextNullTileMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	   chrSelection.left	=	chrSelBuf.left;
	   chrSelection.top   	=	chrSelBuf.top;
	   chrSelection.right 	=	chrSelBuf.right;
	   chrSelection.bottom	=	chrSelBuf.bottom;
	   chrSelectRect=true;
	   UpdateTiles(true);
	   UpdateNameTable(-1,-1,true);
	   FormNavigator->Draw(false,false);
	   //UpdateStats();
	   cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Setcarriagereturnpoint1Click(TObject *Sender)
{
	if(nameSelection.left>=0&&nameSelection.top>=0) returnCarriagePos= nameSelection.left;
	else Application->MessageBox
		("A selected map position is required.",
		"Set carriage return point",
		MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CopySelectionasBMP1Click(TObject *Sender)
{
	TPicture *picture;
	Graphics::TBitmap *bmp;
	int i,j,x,y,w,h,pp;
	AnsiString tmp;

	unsigned short picFormat;
	unsigned int cbHandle;

	if(nameSelection.left>=0&&nameSelection.top>=0)
		{GetSelection(nameSelection,x,y,w,h); tmp="name(s)";}
	else
		{GetSelection(chrSelection,x,y,w,h); tmp="character(s)";}
    BlockDrawing(true);

	picture=new TPicture();
	bmp=new Graphics::TBitmap();
	bmp->SetSize(w*8,h*8);
	bmp->PixelFormat=pf4bit;
	SetBMPPalette(bmp);
	picture->Bitmap=bmp;

	pp=0;

	if(nameSelection.left>=0&&nameSelection.top>=0)
	{

		for(i=0;i<h;++i)
		{
			for(j=0;j<w;++j)
			{
				nameCopy[pp]=nameTable[y*nameTableWidth+x+j];
				attrCopy[pp]=AttrGet(x+j,y,false,false);
				DrawExportTile16(
					picture,
					j*8,
					i*8,
					nameCopy[pp],
					attrCopy[pp],
					x+j,
					i,
					false
					);
			++pp;
		}
		++y;
	}

	}
	else
	{
		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				pp=bankActive+(x+j)*16+(y+i)*256;//+k;
				DrawExportTile16(picture,j*8,i*8,pp,palActive,-1,-1,true);
			}
		}
	}
	
	HPALETTE hPal;

	picture->SaveToClipboardFormat(picFormat, cbHandle, hPal);
	Clipboard()->SetAsHandle(picFormat, cbHandle);

	delete bmp;
	delete picture;


	BlockDrawing(false);


	LabelStats->Caption="Selected "+tmp+" put on clipboard as BMP.";
	StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Delete1Click(TObject *Sender)
{
//question - should this priority be uniform with how ctrl-x etc work?
if(PageControlEditor->ActivePage==TabSheetSprite)
{
	SpeedButtonSpriteDelClick(Sender);
	
    return;
}
if(nameSelection.left>=0&&nameSelection.top>=0) CopyMap(true);
else CopyCHR(false,true);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::StatusUpdateWaiterTimer(TObject *Sender)
{
	if(!openByFileDone) return;
	StatusUpdateWaiter->Enabled=false;
	if (cueStats==true) UpdateStats();


	holdStats=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ThrottleTimer(TObject *Sender)
{
	if(!openByFileDone) return;
	throttleOK = true;
	throttleOKselRect = true;
	throttleOKtileSelRect = true;
	
	if (cueStats==true&&holdStats==false) UpdateStats();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageNameMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	UpdateNameTable(-1,-1,true);
	//FormNavigator->Draw(false,false);
	int dx = nameTableViewX <  0 ?  0  : nameTableViewX;
	int dy = nameTableViewY <  0 ?  0  : nameTableViewY;
	int dw = nameTableWidth < 32 ?  nameTableWidth   : 32;
	int dh = nameTableHeight< 32 ?  nameTableHeight  : 32;

	FormNavigator->DrawRange(dx,dy,dw,dh,false);
	//FormNavigator->CueLinesTimer->Enabled=true;
    FormNavigator->UpdateLines(false);
	mouseDraggedNTSel=false;
	mouseDraggedTileSel=false;
	TimerNTstrip->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageTilesMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//draw line stuff
    bool bSmear = FormLineDetails->btnSmear->Down;
  	//bool bMove  = FormLineDetails->btnMove->Down;
	//bool bReuse = FormLineDetails->btnReuse->Down;
	bool bQuick = FormLineDetails->btnQuick->Down;

		lineUpX=X;
		lineUpY=Y;
		mvOriginX=0;
		mvOriginY=0;
		lineDownX=0;
		lineDownY=0;
	if(lineDrawing){
		FormCHREditor->SpeedButtonHFlip->Enabled=true;
		FormCHREditor->SpeedButtonVFlip->Enabled=true;
		FormCHREditor->SpeedButtonRotateCW->Enabled=true;
		FormCHREditor->SpeedButtonRotateCCW->Enabled=true;

		FormLineDetails->btnMove->Down=false;
	}
	else{lineUpX= -1; lineUpY= -1;}

		
	if(bSmear || bQuick){memcpy (arr_linePlotCHR2,arr_linePlotCHR,sizeof(arr_linePlotCHR2));}

	FormLineDetails->btnMove->Enabled=false;
	lineDrawing = false;
	
	isMovingLine= false;
	bForbidPaint = false;




	//tileset stuff
	mouseDraggedNTSel=false;
	mouseDraggedTileSel=false;


	UpdateNameTable(-1,-1,true);
	if(FormNavigator->Visible) FormNavigator->Draw(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TypeInModeOnOff1Click(TObject *Sender)
{
  SpeedButtonTypeIn->Down^=true;
  SpeedButtonTypeInClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ForceActiveTab1Click(TObject *Sender)
{
	((TMenuItem*)Sender)->Checked^=true;
	MCHRRemoveDoubles->Checked=false;
	MCHRRemoveUnused->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::IncludeNametables1Click(TObject *Sender)
{
   ((TMenuItem*)Sender)->Checked^=true;
	MCHRRemoveDoubles->Checked=false;
	MCHRRemoveDoubles->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::IncludeMetasprites1Click(TObject *Sender)
{
   ((TMenuItem*)Sender)->Checked^=true;
    MCHRRemoveDoubles->Checked=false;
	MCHRRemoveDoubles->Checked=false;
   //todo: maybe add a warning if both options are unchecked that default behaviour in this case is same as "force active".
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Savesession1Click(TObject *Sender)
{
	AnsiString str = SaveDialogSession->FileName;

	BlockDrawing(true);

	if(str!="session") SaveSession(SaveDialogSession->FileName);
	else if(SaveDialogSession->Execute()) SaveSession(SaveDialogSession->FileName);

	
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::sortonremoval1Click(TObject *Sender)
{
	sortonremoval1->Checked^=true;
	MCHRRemoveDoubles->Checked=false;
	MCHRRemoveDoubles->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Noterminator1Click(TObject *Sender)
{
   ((TMenuItem*)Sender)->Checked^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (unsavedChanges==true)
	{
		if(Application->MessageBox("Unsaved work will be lost!","QUIT: Are you sure?",MB_YESNO)==IDNO) Action = caNone;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Externalnespal1Click(TObject *Sender)
{
	int i,pp;
	FILE *file;
	unsigned char buf[192];
	AnsiString dir, name;

	dir=ParamStr(0).SubString(0,ParamStr(0).LastDelimiter("\\/"));

	file=fopen((dir+"nes.pal").c_str(),"rb");
	if(file)
	{
		if(get_file_size(file)==192)
		{
			Externalnespal1->Checked=true;
			fread(buf,192,1,file);
			fclose(file);

			pp=0;

			for(i=0;i<64;i++)
			{
				basePalette[i]=(buf[pp+2]<<16)|(buf[pp+1]<<8)|buf[pp];
				pp+=3;
			}
		}
	}
	else
	{
		Application->MessageBox("Couldn´t find ext pal.\n Palette set to NESST classic.","Error",MB_OK);
		NESSTclassic1->Checked=true;       //todo: maybe allow for a few internal palette preference choices.
		pp=0;

		for(i=0;i<64;i++)
		{
			basePalette[i]=(palette[pp+2]<<16)|(palette[pp+1]<<8)|palette[pp];
			pp+=3;
		}
	}

	ppuMask=0;
	palette_calc();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::NESSTclassic1Click(TObject *Sender)
{
	 int pp,i;
	 NESSTclassic1->Checked=true;       //todo: maybe allow for a few internal palette preference choices.
		pp=0;

		for(i=0;i<64;i++)
		{
			basePalette[i]=(palette[pp+2]<<16)|(palette[pp+1]<<8)|palette[pp];
			pp+=3;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ClipMetaSpriteAsBMP1Click(TObject *Sender)
{
	TPicture *picture;
	Graphics::TBitmap *bmp;
	BlockDrawing(true);

	picture=new TPicture();
	bmp=new Graphics::TBitmap();
	bmp->SetSize(128,128);
	bmp->PixelFormat=pf24bit; //todo: make the draw routine work for 4bpp instead
	SetBMPPalette(bmp);
	picture->Bitmap=bmp;

	//(TPicture *img,int spr_id,int scale,bool grid,bool frame_all,bool frame_none)
	DrawMetaSpriteExport(picture,metaSpriteActive,1,SpeedButtonSpriteGrid->Down,SpeedButtonFrameAll->Down,SpeedButtonFrameNone->Down);

	unsigned short picFormat;
	unsigned int cbHandle;
	HPALETTE hPal;

	try
	{
		picture->SaveToClipboardFormat(picFormat, cbHandle, hPal);
		Clipboard()->SetAsHandle(picFormat, cbHandle);
	}
	__finally
	{
		delete bmp;
		delete picture;
    }
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CopyMetasToCB(int i)
{
	TPicture *picture;
			Graphics::TBitmap *bmp;

			picture=new TPicture();
			bmp=new Graphics::TBitmap();
			bmp->SetSize(128,128);
			bmp->PixelFormat=pf24bit; //todo: make the draw routine work for 4bpp instead
			SetBMPPalette(bmp);
			picture->Bitmap=bmp;

			unsigned short picFormat;
			unsigned int cbHandle;
			HPALETTE hPal;

			try
			{
				DrawMetaSpriteExport(picture,i,1,SpeedButtonSpriteGrid->Down,SpeedButtonFrameAll->Down,SpeedButtonFrameNone->Down);

				picture->SaveToClipboardFormat(picFormat, cbHandle, hPal);
				Clipboard()->SetAsHandle(picFormat, cbHandle);
			}
			__finally
			{
				delete bmp;
				delete picture;
			}
}

void __fastcall TFormMain::CopyBankasBMP1Click(TObject *Sender)
{
	int i,off;
	int total=0;
	int cnt=0;

	for(i=0;i<256;++i)
	{
		off=i*64*4;

		if(metaSprites[off]<255)
		{
			++total;
		}
	}
	if(total>10)
	if(Application->MessageBox(("There are more than 10 metasprites to process.\nIt takes ~0.2s to process each."),"Confirm",MB_OKCANCEL)!=IDOK) return;
	Screen->Cursor = crHourGlass;
	for(i=0;i<256;++i)
	{
		off=i*64*4;

		if(metaSprites[off]<255)
		{
			++cnt;
			//if(Application->MessageBox("Keep putting metasprites to stack?","Confirm",MB_YESNO)!=IDYES) return;
			//templabel:
			//if(CBThrottle->OnTimer==false) goto templabel;

			Sleep(250);
			BlockDrawing(true);
			CopyMetasToCB(i);
			BlockDrawing(false);
		}
	}
	Screen->Cursor = crDefault;
	LabelStats->Caption=num_to_3char(cnt)+"/"+num_to_3char(total)
			+" metasprite(s) copied to the clipboard stack.\n"
			+"\tTo retrieve them, press win+v.";
	StatusUpdateWaiter->Enabled=true;
			holdStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SaveMetasAsBMP(int i, AnsiString str)
{

			AppendStr(str,+".bmp");

			TPicture *picture;
			Graphics::TBitmap *bmp;

			picture=new TPicture();
			bmp=new Graphics::TBitmap();
			bmp->SetSize(128,128);
			bmp->PixelFormat=pf24bit; //todo: make the draw routine work for 4bpp instead
			SetBMPPalette(bmp);
			picture->Bitmap=bmp;

			try
			{
				DrawMetaSpriteExport(picture,i,1,SpeedButtonSpriteGrid->Down,SpeedButtonFrameAll->Down,SpeedButtonFrameNone->Down);

				picture->SaveToFile(str);
			}
			__finally
			{
				delete bmp;
				delete picture;
			}
}


void __fastcall TFormMain::MetaspritebankasBMPSequence1Click(TObject *Sender)
{
	int i,cnt,off;
	AnsiString str,buf;

	cnt=0;

	for(i=0;i<256;++i)
	{
		off=i*64*4;
		if(metaSprites[off]<255) ++cnt;
	}
	if (cnt<1)
	{
		LabelStats->Caption="WARNING: No metasprite was found:\nNeeds at least 1 metasprite in list.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
		return;
	}

	BlockDrawing(true);
	str=RemoveExt(SaveDialogImage->FileName);

	if(str=="") str="sequence";

	SaveDialogImage->FileName=str+".bmp";
	buf=str;

	if(SaveDialogImage->Execute())

	for(int i=0;i<256;++i)
	{
		int off=i*64*4;

		if(metaSprites[off]<255)
		{
        	str=buf;
			if (i<100) AppendStr(str,"0");
			if (i<10)  AppendStr(str,"0");
			AppendStr(str,IntToStr(i));


			SaveMetasAsBMP(i,str);
		}
	}
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MetaspriteasBMP1Click(TObject *Sender)
{
	BlockDrawing(true);
	AnsiString str=RemoveExt(SaveDialogImage->FileName);

	if(str=="") str="metasprite";

	SaveDialogImage->FileName=str+".bmp";

	if(SaveDialogImage->Execute())

	SaveMetasAsBMP(metaSpriteActive,str);

	BlockDrawing(false);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SBPriorityToggle1Click(TObject *Sender)
{
	int i,off;
	bool tmp=true;
	SetUndo();
	for(i=0;i<ListBoxSpriteList->Items->Count;++i)
	{
		if(ListBoxSpriteList->Selected[i])
		{
			off=metaSpriteActive*64*4+i*4;

			if(metaSprites[off]<255)
			{

				if(Sender==SBPriorityToggle1) {metaSprites[off+2]^=OAM_PRIO; tmp=false;}
				if(Sender==SBB4) {metaSprites[off+2]^=OAM_B4; tmp=false;}
				if(Sender==SBB3) {metaSprites[off+2]^=OAM_B3; tmp=false;}   
				if(Sender==SBB2) {metaSprites[off+2]^=OAM_B2; tmp=false;}
				if(tmp) {metaSprites[off+2]^=OAM_PRIO; tmp=false;} //else it must be Prio bit
			}
		}
	}
		//note: NEXXT currently has no way to emulate the sort of clipping effect that may happen.
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Sprlistl1Click(TObject *Sender)
{
	GroupBoxMetaSprite->Align=alRight;
	((TMenuItem*)Sender)->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Sprlistc1Click(TObject *Sender)
{
	GroupBoxMetaSprite->Align=alLeft;
	((TMenuItem*)Sender)->Checked=true;
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::Fill1Click(TObject *Sender)
{
	if(!(PageControlEditor->ActivePage==TabSheetName)) return; //redunancy
	if(!(nameSelection.left<0&&nameSelection.top<0)) FillMap(false);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Generalsettings1Click(TObject *Sender)
{
  FormPreferences->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MetaSpriteTimerTimer(TObject *Sender)
{
     if(!openByFileDone) return;
	if (cueUpdateMetasprite==true)
	{
		if(PageControlEditor->ActivePage==TabSheetSprite)UpdateMetaSprite();
		if(FormManageMetasprites)
			if(FormManageMetasprites->Visible)
				{
				//DrawMetaSprite(FormManageMetasprites->ImageMetaSprite,metaSpriteActive,1,true,false,true);
				FormManageMetasprites->UpdateActive();
				}
	 }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Drawontilesetmodeonoff1Click(TObject *Sender)
{
	if(SpeedButtonTypeIn->Down) return;
	if(SpeedButtonDrawTile->Down) {SpeedButtonMarqTile->Down=true; cueStats=true; return;}
	SpeedButtonDrawTile->Down^=true;
	cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageBigTilesMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   bbigTileMouseDown = true;
   TimerNTstrip->Enabled=true;
   ImageTilesMouseDown(Sender,Button,Shift,X/2,Y/2);

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ImageBigTilesMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   bigTilesHover=0; //this can be repurposed later for identifying pixels and memory
					  //for now it just acts to give stats intel.
   cueStats=true;
   ImageTilesMouseMove(Sender,Shift,X/2,Y/2);

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageBigTilesMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bbigTileMouseDown = false;
	TimerNTstrip->Enabled=false;

    bool bSmear = FormLineDetails->btnSmear->Down;
  	//bool bMove  = FormLineDetails->btnMove->Down;
	//bool bReuse = FormLineDetails->btnReuse->Down;
	bool bQuick = FormLineDetails->btnQuick->Down;

		lineUpX=X/2;
		lineUpY=Y/2;
		mvOriginX=0;
		mvOriginY=0;
		lineDownX=0;
		lineDownY=0;
	if(lineDrawing){
		FormCHREditor->SpeedButtonHFlip->Enabled=true;
		FormCHREditor->SpeedButtonVFlip->Enabled=true;
		FormCHREditor->SpeedButtonRotateCW->Enabled=true;
		FormCHREditor->SpeedButtonRotateCCW->Enabled=true;

		FormLineDetails->btnMove->Down=false;
	}
	else{lineUpX= -1; lineUpY= -1;}

		
	if(bSmear || bQuick){memcpy (arr_linePlotCHR2,arr_linePlotCHR,sizeof(arr_linePlotCHR2));}

	FormLineDetails->btnMove->Enabled=false;
	lineDrawing = false;
	
	isMovingLine= false;
	bForbidPaint = false;



	UpdateNameTable(-1,-1,true);
	if (FormNavigator->Visible) FormNavigator->Draw(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageBigTilesMouseLeave(TObject *Sender)
{
	bigTilesHover=-1;
	cueStats=true;
	ImageTilesMouseLeave(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageBigTilesDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
	ImageTilesDragDrop(Sender,Source,X/2,Y/2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageBigTilesDragOver(TObject *Sender,
	  TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	Accept=false;


	ImageTilesDragOver(Sender, Source, X, Y, State, Accept);

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TimerNTstripTimer(TObject *Sender)
{
	 if(!openByFileDone) return;
	if (cueUpdateNTstrip==true)
	{
		UpdateNameStrip(false,0);
		UpdateNameStrip(true,3);
		UpdateNameStrip(false,7);
		UpdateNameStrip(true,11);
		UpdateNameStrip(false,15);
		UpdateNameStrip(true,19);
		UpdateNameStrip(false,23);
		UpdateNameStrip(true,27);
		cntNTstrip++;
		if (cntNTstrip>=4)
		{
			cueUpdateNTstrip==false;
			cntNTstrip=0;
			//if(PageControlEditor->ActivePage==TabSheetTile?true:false) ImageTiles->Repaint();
			//else ImageName->Repaint();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnCHReditClick(TObject *Sender)
{
	FormCHREditor->Visible^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LabelMetaSpriteDblClick(TObject *Sender)
{
	FormName->Caption="Rename metasprite";
	FormName->EditName->Text=metaSpriteNames[metaSpriteActive];
	FormName->ShowModal();
	if(bKeyEscape) return;

	metaSpriteNames[metaSpriteActive]=FormName->EditName->Text;
	UpdateMetaSpriteLabel();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Renameallmetasprites1Click(TObject *Sender)
{
	FormName->Caption="Rename all metasprites";
	FormName->EditName->Text=RemoveExt(ExtractFileName(SaveDialogSession->FileName));
	FormName->ShowModal();
	if(bKeyEscape) return;
	for(int i=0;i<256;i++) metaSpriteNames[i]=FormName->EditName->Text+IntToStr(i);
	FormName->Caption="Metasprite name"; //return to normal
	UpdateMetaSpriteLabel();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonAutocreateClick(TObject *Sender)
{
   //SpeedButtonAutocreate->Down^=true;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Clearemphasis1Click(TObject *Sender)
{
	   ppuMask=0;
	   UpdateRGBM();
	   palette_calc();
	   UpdateTiles(true);
	   UpdateNameTable(-1,-1,true);
	   DrawPalettes();
	   cueUpdateMetasprite=true;
	   LabelStats->Caption="PPU mask cleared!";
	   StatusUpdateWaiter->Enabled=true;
	   holdStats=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Putemphasissetonallpalettesets1Click(TObject *Sender)
{
	for (int i = 0; i <= 3; i++) ppuMaskSet[i]=ppuMask;
	LabelStats->Caption="PPU mask cloned to all palette sets!";
	StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnHuePlusClick(TObject *Sender)
{

   int n,entry=palActive*4;
   SetUndo();
   for (int i = 1; i <= 3; i++) {
	  n=bgPal[palBank*16+entry+i];
	  if (((n&0x0F)>0x00)&&((n&0x0F)<0x0C)) bgPal[palBank*16+entry+i]+=1;
	  else if ((n&0x0F)==0x0C) bgPal[palBank*16+entry+i]-=11;
	}
   UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnHueMinusClick(TObject *Sender)
{
  int n,entry=palActive*4;

  SetUndo();
  for (int i = 1; i <= 3; i++) {
	  n=bgPal[palBank*16+entry+i];
	  if (((n&0x0F)>0x01)&&((n&0x0F)<=0x0C)) bgPal[palBank*16+entry+i]-=1;
	  else if ((n&0x0F)==0x01) bgPal[palBank*16+entry+i]+=11;
	}
   UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnHueTiltMinusClick(TObject *Sender)
{
	int n,entry=palActive*4;
	SetUndo();

	n=bgPal[palBank*16+entry+1];
	if (((n&0x0F)>0x01)&&((n&0x0F)<=0x0C)) bgPal[palBank*16+entry+1]-=1;
	else if ((n&0x0F)==0x01) bgPal[palBank*16+entry+1]+=11;

	n=bgPal[palBank*16+entry+3];
	if (((n&0x0F)>0x00)&&((n&0x0F)<0x0C)) bgPal[palBank*16+entry+3]+=1;
	else if ((n&0x0F)==0x0C) bgPal[palBank*16+entry+3]-=11;
   UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnHueTiltPlusClick(TObject *Sender)
{
	int n,entry=palActive*4;
	SetUndo();

	n=bgPal[palBank*16+entry+3];
	if (((n&0x0F)>0x01)&&((n&0x0F)<=0x0C)) bgPal[palBank*16+entry+3]-=1;
	else if ((n&0x0F)==0x01) bgPal[palBank*16+entry+3]+=11;

	n=bgPal[palBank*16+entry+1];
	if (((n&0x0F)>0x00)&&((n&0x0F)<0x0C)) bgPal[palBank*16+entry+1]+=1;
	else if ((n&0x0F)==0x0C) bgPal[palBank*16+entry+1]-=11;
   UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSubpalCopyMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="Button [copy subpalette]:\tPuts a single subpalette on the clipboard.\n\nTips:\t-Holding [C] and clicking on a subpalette is equivalent.\n\t-[Shift-C] puts entire subpalette set on the clipboard.";
	//StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSubpalCopyMouseLeave(TObject *Sender)
{
   LabelStats->Caption="---";
   holdStats=false;
   if(uiScale>1 && PageControlEditor->ActivePage==TabSheetTile)
	{	GroupBoxTileControl->Visible=true;}
	else
	{	GroupBoxTileControl->Visible=false;	}

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonSubpalPasteMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="Button [paste subpalette]:\tPastes to the active subpalette from the clipboard.\n\nTips:\t-Holding [V] and clicking on a subpalette is equivalent.\n\t-[Shift-V] pastes entire subpalette set.";
	//StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnBrightPlusClick(TObject *Sender)
{

	int n,entry=palActive*4;

	SetUndo();
	for (int i = 1; i <= 3; i++) {
	  n=bgPal[palBank*16+entry+i];
	  if ((n&0xF0)<0x30) bgPal[palBank*16+entry+i]+=16;
	  else if (((n&0xF0)==0x30)&&((n&0x0F)!=0x00)) bgPal[palBank*16+entry+i]=0x20;
	  //else if ((n&0x0F)==0x0C) bgPal[palBank*16+entry+i]-=11;
	}
   UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnBrightMinusClick(TObject *Sender)
{
	int n,entry=palActive*4;

	SetUndo();
	for (int i = 1; i <= 3; i++) {
	  n=bgPal[palBank*16+entry+i];
	  if ((n&0xF0)>=0x10) bgPal[palBank*16+entry+i]-=16;
	  else if ((n&0xF0)<0x10) bgPal[palBank*16+entry+i]=0x0F;
	}
   UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnHueMinusMouseEnter(TObject *Sender)
{
	AnsiString str;
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	if(Sender==btnHueMinus)
		str="Button [hue shift left]:\tShifts the hue of the Active Subpalette left. Grays are ignored.";
	if(Sender==btnHuePlus)
		str="Button [hue shift right]:\tShifts the hue of the Active Subpalette right. Grays are ignored.";
	if(Sender==btnHueTiltMinus)
		str="Button [hue tilt]:\tTilts the hue of the Active Subpalette (<-#1; #3->). Grays are ignored.";
	if(Sender==btnHueTiltPlus)
		str="Button [hue tilt]:\tTilts the hue of the Active Subpalette (<-#3; #1->). Grays are ignored.";
	if(Sender==btnBrightMinus)
		str="Button [value shift down]:\tShifts the value of the Active Subpalette down.";
	if(Sender==btnBrightPlus)
		str="Button [value shift up]:\tShifts the value of the Active Subpalette up.";

	str+="\n\nTips:\t-Set a checkpoint [Shift+Ctrl-Z] before you start manipulating palettes with these buttons.\n\t-Revert to last checkpoint with [Shift-Z].";

	LabelStats->Caption=str;
	//StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMaskBMouseEnter(TObject *Sender)
{
   if(uiScale<3) GroupBoxTileControl->Visible=false;
   LabelStats->Caption="PPUMASK buttons: These set the bits for [R]ed, [G]reen, [B]lue emphasis modes and the [M]onochrome mode.\n\nTips:\t-[Ctrl-E]clears the PPUMASK of the Active Subpalette Set.\n\t-[Shift+Ctrl+E]clones the current PPUMASK to all sets.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonAutocreateMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="If in Draw mode, when active, drawing freely on the screen or map \nautomatically asigns vacant tiles to the canvas.\nFreedraw + autocreation is best done sparingly when working with a tile budget.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMarqTileMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="The standard editing mode for the main window.\n\nOn screen/map: select [Shift+click or drag], place [click], sample [r click], seek [r drag], quickpaste [v+click].\nOn the tileset: single select [click], box select [shift+click], multi select [ctrl+click], swap [r drag].";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonDrawTileMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="Draw mode allows you to draw on the screen / tileset w. the mouse tools normally available in the CHR Editor.\n\nTips:\t-This is not intended to replace a full-fledged canvas editor like Aseprite or Pro Motion, but can be useful.\n\t-Be aware that free drawing in excess leads to huge tilecounts.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnCHReditMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="Opens CHR editor. It´s the main utility for detailed tile work.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TextNullTileMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="The 'null tile' is the tile that replaces deleted/cut content on the map.\n[Double click] to set the null tile to the current active tile.\n[Click+hold] to highlight current null tile. [Click+drag up/down] to change null tile.\nYou may also [r click+drag] a tile on the tileset to swap the null tile, along with its contents.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonChrBank1MouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="When down, the screen/map references to the main (A) tileset, which is shown and editable.\n\n[X] toggles between main (A) and spare (B) tilesets.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonChrBank2MouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="When down, the screen/map references to the spare (B) tileset, which is shown and editable.\n\n[X] toggles between main (A) and spare (B) tilesets.";
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SpeedButtonSelTilesMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="[S] Hides all non-selected tiles.\n\n Good for identifying the position and amount of selected tiles on the screen/map";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonCheckerMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="[A] Disables tile view and instead shows where attributes are placed.\n\nGood for when attribute boundaries are unclear or for identifying 'rogue' attributes\nthat may increase compressed data sizes in some game engines.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonGridAllMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="[D] turns grid visibility on/off.\n\n[H,J,K,L] toggle grid resolutions.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonGridTileMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="These buttons [H: 32x30, J: 1x1, K: 2x2, L: 4x4] toggle grid resolutions.\n\nTip:\t[D] turns grid visibility on/off.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonTilesMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="[W] Toggles placing tiles on/off when clicking on the screen/map canvas.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonPalMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="[E] Toggles placing palette attributes on/off when clicking on the screen/map canvas.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonTypeInMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="[T] Turns on 'Type In' mode, where you place tiles with your keyboard; each key corresponding to\nits ASCII value; +/- offset. [Esc] exits this mode. [Ctrl-T] toggles mode on/off. [Shift+Ctrl+T] Sets return point.\nTips:\t-If you select [shift+click] a cell before entering type in mode, a return [Enter] will reference it.\n\t-[Shift+Enter] acts as a negative return, moving to beginning of line and 1 line up.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StaticTextFontOffsetMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="[Drag] to change ASCII offset by tileset row.\n[Shift+Drag] to adjust fine offset.\n\nYou may also check the Edit menu for setting an ASCII 'base offset', which applies independently from this control.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonPalBankAMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
	LabelStats->Caption="This button group switches between 4 subpalette sets.\You may use [Shift + 5-8] to do this more quickly.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CHReditortoolbartopClick(TObject *Sender)
{
	FormCHREditor->GroupBox2->Align=alTop;
	FormCHREditor->GroupBox1->Align=alBottom;
	((TMenuItem*)Sender)->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CHReditortoolbarbottomClick(TObject *Sender)
{
	FormCHREditor->GroupBox2->Align=alBottom;
	FormCHREditor->GroupBox1->Align=alTop;
	((TMenuItem*)Sender)->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::NTtimerTimer(TObject *Sender)
{
	if(!openByFileDone) return;

	if(cueUpdateNametable==true) {
		UpdateNameTable(-1,-1,true);
		if(FormNavigator->Visible) FormNavigator->CueChunkDraw->Enabled=true; //FormNavigator->CueDrawTimer->Enabled=true;   //performance thief when scaled. todo: divide and conquer.
		}
	else if(cueUpdateNametableNoRepaint==true)
	{
		UpdateNameTable(-1,-1,false);
		//FormNavigator->Draw(false);
		cueUpdateNametableNoRepaint=false;
		cueUpdateNametable=false;
	}
	cueUpdateNametable=false;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TileTimerTimer(TObject *Sender)
{
	 if(!openByFileDone) return;
	if(cueUpdateTiles==true) UpdateTiles(false);
	cueUpdateTiles=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CHRtimerTimer(TObject *Sender)
{
	if(!openByFileDone) return;
	if(cueCHRdraw==true) FormCHREditor->Draw(false);
	if(cueCHRdrawAll==true) FormCHREditor->Draw(true);
	cueCHRdraw=false;
	cueCHRdrawAll=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2x2tileeditmode1Click(TObject *Sender)
{
	if(SpeedButtonTypeIn->Down) return;
	FormCHREditor->btn2x2mode->Down^=true;
	FormCHREditor->btn2x2modeClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::clearprotection1Click(TObject *Sender)
{
	FormCHREditor->Protect0->Down=false;
	FormCHREditor->Protect1->Down=false;
	FormCHREditor->Protect2->Down=false;
	FormCHREditor->Protect3->Down=false;

	Protect0->Down=false;
	Protect1->Down=false;
	Protect2->Down=false;
	Protect3->Down=false;



}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Invertprotection1Click(TObject *Sender)
{
	FormCHREditor->Protect0->Down^=true;
	FormCHREditor->Protect1->Down^=true;
	FormCHREditor->Protect2->Down^=true;
	FormCHREditor->Protect3->Down^=true;

	Protect0->Down^=true;
	Protect1->Down^=true;
	Protect2->Down^=true;
	Protect3->Down^=true;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SaveDialogMapTypeChange(TObject *Sender)
{
	//if(SaveDialogMap->FilterIndex==2) SaveDialogMap->FileName+=".rle";
	//else SaveDialogMap->FileName+=".map";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Lightboxmodetransparentform1Click(TObject *Sender)
{
	FormMain->AlphaBlend^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LightboxmodeCHRtransparent1Click(TObject *Sender)
{
	FormCHREditor->AlphaBlend^=true;
	//FormManageMetasprites->AlphaBlend^=true;
	FormCHREditor->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::VisitWebClick(TObject *Sender)
{
	ShellExecute(NULL, "open", "https://frankengraphics.itch.io/nexxt", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Recallcolour1Click(TObject *Sender)
{
    if(SpeedButtonTypeIn->Down) return;
	int tmp;

	//bool b=(FormCHREditor->Active||FormBrush->Active||SpeedButtonDrawTile->Down);



		tmp=bgPalCur;
		bgPalCur=bgPalCurOld;
		bgPalCurOld=tmp;

		Colour001->Checked=false;
		Colour011->Checked=false;
		Colour101->Checked=false;
		Colour111->Checked=false;

		if (bgPalCur==0) Colour001->Checked=true;
		if (bgPalCur==1) Colour011->Checked=true;
		if (bgPalCur==2) Colour101->Checked=true;
		if (bgPalCur==3) Colour111->Checked=true;


	/*
	//maybe explore: find a good context where this can be done without feeling disruptive.
	if(!b)
	{
	   tmp=palActive;
	   palActive=palActiveOld;
	   palActiveOld=tmp;

	   Pal0->Checked=false;
	   Pal1->Checked=false;
	   Pal2->Checked=false;
	   Pal3->Checked=false;

	   if (palActive==0) Pal0->Checked=true;
	   if (palActive==1) Pal1->Checked=true;
	   if (palActive==2) Pal2->Checked=true;
	   if (palActive==3) Pal3->Checked=true;
	}
    */
	UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TogglePenBrush1Click(TObject *Sender)
{
	if(SpeedButtonTypeIn->Down) return;
	FormCHREditor->btnThick->Down^=true;
	TogglePenBrush1->Checked=FormCHREditor->btnThick->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Quantized1Click(TObject *Sender)
{
    if(SpeedButtonTypeIn->Down) return;
	FormCHREditor->btnQuant->Down^=true;
	Quantized1->Checked=FormCHREditor->btnQuant->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Brushmask1Click(TObject *Sender)
{
	FormBrush->Visible^=true;	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::About1Click(TObject *Sender)
{
	AboutBox->Show();	
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Swaptablesinselection1Click(TObject *Sender)
{
	
	unsigned char temp[4096];
	uint32_t propt[512];

	int xTiles;  //number of tiles to process
	int yTiles;
	int pp = tileActive*16;
	int ta = tileActive;

	int i,j;
	int i16;
	SetUndo();

	if(chrSelectRect)
	{
		xTiles=tileSelRectWdt;
		yTiles=tileSelRectHgt;

		int x32=xTiles*sizeof(uint32_t);

		for (i=0; i < yTiles; i++) memcpy(temp+pp+i*256		,chr+pp+i*256		,xTiles*16);
		for (i=0; i < yTiles; i++) memcpy(chr+pp+i*256		,chr+4096+pp+i*256 ,xTiles*16);
		for (i=0; i < yTiles; i++) memcpy(chr+4096+pp+i*256	,temp+pp+i*256		,xTiles*16);

		for (i=0; i < yTiles; i++) {
				memcpy(&propt[ta+i*16]				,&tileProperties[ta+i*16]		,x32);}
		for (i=0; i < yTiles; i++)
				memcpy(&tileProperties[ta+i*16]		,&tileProperties[256+ta+i*16] 	,x32);
		for (i=0; i < yTiles; i++)
				memcpy(&tileProperties[256+ta+i*16]	,&propt[ta+i*16]					,x32);

	}
	else
	{
    	for(i=0;i<256;i++)
		{
			if(chrSelected[i])
			{
				memcpy(temp				,chr+i*16		,16);
				memcpy(chr+i*16			,chr+4096+i*16	,16);
				memcpy(chr+4096+i*16	,temp			,16);

				memcpy(propt					,&tileProperties[i]		,sizeof(uint32_t));
				memcpy(&tileProperties[i]		,&tileProperties[256+i]	,sizeof(uint32_t));
				memcpy(&tileProperties[256+i]	,propt					,sizeof(uint32_t));
			}
		}
	}
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	if (FormNavigator->Visible) FormNavigator->Draw(false,false);
	if (FormCHRbit->Visible) FormCHRbit->UpdateBitButtons();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------





void __fastcall TFormMain::ImageTilesEndDrag(TObject *Sender, TObject *Target,
      int X, int Y)
{
	bDrawDestShadow=false;
    bImageNameAccepted=false;
	bImageTileAccepted=false;
	cueUpdateTiles=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ImageNameDragOver(TObject *Sender, TObject *Source,
	  int X, int Y, TDragState State, bool &Accept)
{
	if(!FormMain->Active) return;
	Accept=false;
	if(PageControlEditor->ActivePage==TabSheetName&&Source==ImageName) if(!bOutsideSel) Accept=true;
	X=X/(8*uiScale);
	Y=Y/(8*uiScale);
	if(Accept==true)
	{
		bImageNameAccepted=true;
		bImageTileAccepted=false;
		if (!bOutsideSel)  //decided at mouse down-time
		{


			destRect.left	=nameSelection.left		+X-nxDown;
			destRect.right	=nameSelection.right	+X-nxDown;
			destRect.top	=nameSelection.top		+Y-nyDown;
			destRect.bottom	=nameSelection.bottom	+Y-nyDown;

			int cAlHgt = (viewPortHeight-nameTableHeight)/2;
			int cAlWdt = (viewPortWidth-nameTableWidth)/2;
			if  ( cAlHgt<0)cAlHgt=0;
			if  (cAlWdt<0) cAlWdt=0;

			for (int i=0; i<32; i++)  //long enough loop
			// I need to special-case if the canvas is smaller than the viewport.
				{
					if(destRect.left<0+cAlWdt)   	{	destRect.left++;
												destRect.right++;}
					if(destRect.right>viewPortWidth-cAlWdt)
											{	destRect.left--;
												destRect.right--;}
					if(destRect.top<0+cAlHgt)   	{	destRect.top++;
												destRect.bottom++;}
					if(destRect.bottom>viewPortHeight-cAlHgt)
											{	destRect.top--;
												destRect.bottom--;}
				}

		}
		else
		{
			destRect.left=X;
			destRect.top=X;
			destRect.right=Y+1;
			destRect.bottom=Y+1;
		}
		bDrawDestShadow=true;

		//cueUpdateNametableNoRepaint=true;
		cueUpdateTiles=false;
		NameLinesTimer->Enabled=true;
		//UpdateNameTable(-1,-1,true);
        cueStats=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageNameEndDrag(TObject *Sender, TObject *Target,
      int X, int Y)
{
	bDrawDestShadow=false;
	bImageNameAccepted=false;
	bImageTileAccepted=false;
	//cueUpdateNametable=true;
	UpdateNameTable(-1,-1,true);
	//FormNavigator->Draw(false,false);
	FormNavigator->UpdateLines(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageNameDragDrop(TObject *Sender, TObject *Source,
	  int X, int Y)
{
	bool bClone = ( bBufCtrl &&  !bBufShift && bBufAlt);
	bool bSwap	= (!bBufCtrl && !bBufShift && !bBufAlt);
	bool bMove	= ( bBufCtrl && !bBufShift && !bBufAlt);
	bool bOverride = false;

	X/=(8*uiScale);
	Y/=(8*uiScale);
	if(X<0||X>=(16*uiScale)||Y<0||Y>=(16*uiScale)) return;
    if(!FormMain->Active) return;
	int w=1;
	int h=1;


	if (!bOutsideSel) //if grabbed from inside selection, retain size. ImageNameDragDrop is only call while this is true, but in case a context changes it..
	{
		w=destRect.right-destRect.left;
		h=destRect.bottom-destRect.top;
	}
	//these are used by the preserve all tilenames; try preserve order method.
	unsigned char tempName;
	unsigned char tempAttr;

	//this array is used by the preserve structure; overwrite tilenames method.

	unsigned char tempTable[sizeof(w*h)];

	int dst = (destRect.top  + nameTableViewY ) * nameTableWidth
			 +  destRect.left+ nameTableViewX;



	int xSrc = nameSelection.left;//+ cAlWdt;//;
	int ySrc = nameSelection.top;//+ cAlHgt;// ;
	int xDst = destRect.left+nameTableViewX;
	int yDst = destRect.top+nameTableViewY;


	int src	= (ySrc  ) * nameTableWidth
			+ xSrc   ;

	bool bDiagonal=(xSrc!=xDst && ySrc!=yDst)?true:false;

	if(dst==src) return;
	if((SpeedButtonPal->Down==false) && (SpeedButtonTiles->Down==false))
		{bOverride=true;}

	//begin swap
	SetUndo();



	//--method 1: the "preserve all tilenames" fashioned from the CHR swap routine,
	//-----------
	// but if moving diagonally, structure is not intact which is important for the nametable.
	//diagonals therefore use a slightly better method below.
    //method 2 could replace this with minor modification now that it's in, but don't fix what works. 
	
	int offset;  	//gets loaded with an expression for sorting order.

	//bool xOdd = xDst%2?1:0;  //not in use.
	//bool yOdd = yDst%2?1:0;

	//----select sorting method
	for(int sy=0; sy<h*nameTableWidth; sy+=nameTableWidth) {
		for(int sx=0; sx<w; sx++) {
			if (ySrc==yDst) {
				if      (xSrc>xDst)  {offset=sx+(sy);}  //dir: east
				else if (xSrc==xDst) {return;}		   	//reduncancy; center; added for the note.
				else    			 {offset=((w-1))-sx+((h-1)*nameTableWidth)-(sy);} 		// west
			}
			else if (ySrc>yDst) {
				if      (xSrc==xDst) {offset=sx+(sy); }  //dir: north
				else if (xSrc>xDst)  {offset=sx+(sy); }  //dir: north-east
				else 	    		 {offset=((w-1))-sx+(sy);}   	//dir: north-west
			}
			else {
				if 	    (xSrc==xDst) {offset=sx+((h-1)*nameTableWidth)-(sy); } //south
				else if (xSrc>xDst) {offset=sx+((h-1)*nameTableWidth)-(sy); }  //south-east
				else    {offset=((w-1))-sx+((h-1)*nameTableWidth)-(sy); }      //south-west
			}

		 //-swap 1 tile for another.
		 if(!bDiagonal && (SpeedButtonTiles->Down || bOverride))  //the extra conditionals are because the loop does an extra row & line for attributes.
		 {
			//swap
			if(bSwap){
				memcpy(&tempName				   , &nameTable[dst+offset],1);
				memcpy(&nameTable	[dst+offset], &nameTable[src+offset] ,1);
				memcpy(&nameTable	[src+offset]  , &tempName			 ,1);
			}
			//move
			if(bMove){
				//memcpy(&tempName				   , &nameTable[dst+offset],1);
				memcpy(&nameTable	[dst+offset], &nameTable[src+offset] ,1);
				memcpy(&nameTable	[src+offset]  , &nullTile			 ,1);
			}
			//clone
			if(bClone){
				//memcpy(&tempName				   , &nameTable[dst+offset],1);
				memcpy(&nameTable	[dst+offset], &nameTable[src+offset] ,1);
				//memcpy(&nameTable	[src+offset]  , &tempName			 ,1);
			}

		 }
		}
	}


	if(bDiagonal  && (SpeedButtonTiles->Down || bOverride)){
	//--method 2: a simple copypaste swap with buffer.
	//----------
	//It seems to preserve the structure better in some cases.
	int offset;
	//-swap rows of tilenames

		if(bSwap){
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameCopy[src+offset]	, &nameTable[src+offset],w);
			}
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameTable	[src+offset], &nameTable[dst+offset],w);
			}
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameTable	[dst+offset], &nameCopy[src+offset]	,w);
			}
		}
		if(bClone){
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameTable	[dst+offset], &nameTable[src+offset],w);
			}
		}
		if(bMove)
		{
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameCopy[dst+offset]	, &nameTable[src+offset],w);
			}
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				//memcpy(&nameTable	[src+offset], nullTile	,w);
				for(int j = 0; j < w; j++) nameTable[src+offset+j]=nullTile;
			}
			for (int i = 0; i < h; i++) {
				if (ySrc>yDst) offset=i*nameTableWidth;
				if (ySrc<yDst) offset=(h-1-i)*nameTableWidth;
				if (ySrc==yDst) return; //redundancy
				memcpy(&nameTable	[dst+offset], &nameCopy[dst+offset],w);
			}
		}
	}

	//swap attributes
	if(SpeedButtonPal->Down||bOverride)
	{
	  int i,j,xo,yo;//,pp;
	  int n=1;

	  for (i=0;i<h;i++){
			for(j=0;j<w;++j){
				if (ySrc>yDst) yo=i;	if (ySrc<=yDst) yo=(h-n-i);
				if (xSrc>xDst) xo=j;	if (xSrc<=xDst) xo=(w-n-j);
				AttrSet(xo, yo, AttrGet(xSrc+xo,ySrc+yo,false,false),true);
			}
	  }
	  if(bSwap)
	  {
		for (i=0;i<h;i++){
			for(j=0;j<w;++j){
				if (ySrc>yDst) yo=i;	if (ySrc<=yDst) yo=(h-n-i);
				if (xSrc>xDst) xo=j;	if (xSrc<=xDst) xo=(w-n-j);
				AttrSet(xSrc+xo, ySrc+yo, AttrGet(xDst+xo,yDst+yo,false,false),false);
			}
		}
	  }
	  for (i=0;i<h;i++){
			for(j=0;j<w;++j){
				if (ySrc>yDst) yo=i;	if (ySrc<=yDst) yo=(h-n-i);
				if (xSrc>xDst) xo=j;	if (xSrc<=xDst) xo=(w-n-j);
				AttrSet(xDst+xo, yDst+yo, AttrGet(xo,yo,true,false),false);
			}
		}
	  
	}
    int dx,dy,dw,dh;
	FormNavigator->Map1->Picture->Bitmap->Assign(bufBmp);           
	GetSelection(nameSelection,dx,dy,dw,dh);
	FormNavigator->DrawRange(dx,dy,dw,dh,false);

	nameSelection.left 		= destRect.left 	+ nameTableViewX;
	nameSelection.top 		= destRect.top 		+ nameTableViewY;
	nameSelection.right 	= destRect.right 	+ nameTableViewX;
	nameSelection.bottom 	= destRect.bottom 	+ nameTableViewY;

	GetSelection(nameSelection,dx,dy,dw,dh);
	FormNavigator->DrawRange(dx,dy,dw,dh,false);


}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ImageBigTilesDblClick(TObject *Sender)
{
	//if(PageControlEditor->ActivePage==TabSheetTile)
	ImageTilesDblClick(Sender);	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CHRpixelgrid1Click(TObject *Sender)
{
    TRect r;
	r.left=0;	r.top=0;	r.right=272+132;	r.Bottom=272+132;

	FormCHREditor->PaintBoxChr->Canvas->Brush->Color=FormCHREditor->GroupBox1->Color;
	FormCHREditor->PaintBoxChr->Canvas->FillRect(r);
	FormCHREditor->Draw(true);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MapNavigator1Click(TObject *Sender)
{
	//FormNavigator->Draw();
	//FormNavigator->FormActivate(Sender);

	if (FormNavigator->WindowState==wsMinimized) FormNavigator->WindowState=wsNormal;
	else FormNavigator->Visible^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonGridScreenMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;
 LabelStats->Caption="[H] Toggles a screen-sized (32x30 tiles) grid resolution on/off.\n\nTip:\t[D] turns grid visibility on/off.";
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ImageNameMouseEnter(TObject *Sender)
{
	bMouseOverNam=true;
	UpdateNameTable(-1,-1,true);
	cueStats=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::AlwaysNavigator1Click(TObject *Sender)
{
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::TextNullTileDblClick(TObject *Sender)
{
	nullTile=tileActive;
    TextNullTile->Caption="   null tile: $"+IntToHex(nullTile,2)+" ";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MFileClick(TObject *Sender)
{
   UpdateMenu();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::includenames1Click(TObject *Sender)
{
	MSaveIncName->Checked=includenames1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveIncNameClick(TObject *Sender)
{
   includenames1->Checked=MSaveIncName->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveIncAttrClick(TObject *Sender)
{
   includeattributes1->Checked=MSaveIncAttr->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::includeattributes1Click(TObject *Sender)
{
   MSaveIncAttr->Checked=includeattributes1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::forceNESlibRLEpacking1Click(TObject *Sender)
{
  MSaveRLE->Checked=forceNESlibRLEpacking1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MSaveRLEClick(TObject *Sender)
{
  forceNESlibRLEpacking1->Checked=MSaveRLE->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::NewNEXXTinstance1Click(TObject *Sender)
{

	ShellExecute(NULL, "Open", ParamStr(0).c_str(), "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Inverttileselection1Click(TObject *Sender)
{
	for(int i=0;i<256;++i) chrSelected[i]^=1;
    chrSelectRect=false;
    nameSelection.left=-1;
	nameSelection.top=-1;
	UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PPUdump1Click(TObject *Sender)
{
	FILE *file;
	unsigned char *tmp;
	int size;
    AnsiString ext;



	BlockDrawing(true);
	
	OpenDialogImport->Title="Import PPU RAM dump file";
	OpenDialogImport->Filter="NES PPU dump|*.bin|Nintendulator PPU dump|*.ppumem|All files (*.*)|*.*";
	OpenDialogImport->DefaultExt="bin";

	if(OpenDialogImport->Execute())
	{
		//FormBank->Caption="Import 8K CHR bank from NES file";
		//FormBank->FileName=OpenDialogImport->FileName;
		//FormBank->ShowModal();

		//if(FormBank->OK)

			file=fopen(OpenDialogImport->FileName.c_str(),"rb");
			ext=GetExt(OpenDialogImport->FileName);

			if(file)
			{
				fseek(file,0,SEEK_END);
				size=ftell(file);


				if (size<=0x4000 ) //full ppu address range dump or up until first mirror of palette ram.
				{
					 if (size>=0x2000 || ext=="ppumem") 	//patterns
					 {
						fseek(file,0,SEEK_SET);
						fread(chr,8192,1,file);
					 }
					 if (size>=0x3000 || ext=="ppumem") 	//nametables
					 {
						nameTableWidth=64;
						nameTableHeight=60;
						tmp=(unsigned char*)malloc(0x1000);
						//load them as map.
						fseek(file,0x2000,SEEK_SET);
						fread(tmp,0x1000,1,file);
                        int v = 64;
						for (int i = 0; i < 30; i++) {
							memcpy(nameTable		   +i*v ,tmp			+i*32	,32);
							memcpy(nameTable		+32+i*v ,tmp +1024		+i*32	,32);
							memcpy(nameTable +960*2	   +i*v ,tmp +1024*2	+i*32	,32);
							memcpy(nameTable +960*2	+32+i*v ,tmp +1024*3	+i*32	,32);
						}
						for (int y = 0; y < 30; y+=2) {
							for (int x = 0; x < 32; x+=2) {
							//2 upper screens' attributes
							memcpy(attrTable+		(y/4*((nameTableWidth+3)/4)+x/4),tmp+960        +(y/4*((32+3)/4)+x/4),1);
							memcpy(attrTable+8+		(y/4*((nameTableWidth+3)/4)+x/4),tmp+960+1024   +(y/4*((32+3)/4)+x/4),1);
							//2 lower screens' attributes -put them in tmp to adjust later
							memcpy(attrCopy+128+	(y/4*((nameTableWidth+3)/4)+x/4),tmp+960+1024*2 +(y/4*((32+3)/4)+x/4),1);
							memcpy(attrCopy+128+8+	(y/4*((nameTableWidth+3)/4)+x/4),tmp+960+1024*3 +(y/4*((32+3)/4)+x/4),1);


						}
                        //correct lower screen attributes one half attribute strip up.
						for(int y=0;y<30;y++)
						{
							for(int x=0;x<64;x++)
							{
								AttrSet(x,30+y,AttrGet(x,32+y,true,false),false);
								
							}
						}


					 }

					 //palettes
					 if (ext=="ppumem")
					 {
						fseek(file,0x3100,SEEK_SET);
						fread(bgPal,32,1,file);
					 }
					 else if (size>=0x3F20)
					 {
						fseek(file,0x3F00,SEEK_SET);
						fread(bgPal,32,1,file);
					 }

					 //PPU OAM goes here when there´s something actually using raw oam.
					 /*
						 if (ext=="ppumem")
						 {
							fseek(file,0x3000,SEEK_SET);
							fread(ppuOAM,256,1,file);
						 }
                     */

				}

				fclose(file);
			}
			UpdateAll();
		}
	}
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::NameLinesTimerTimer(TObject *Sender)
{
     if(!openByFileDone) return;

	NameLinesTimer->Enabled=false;
	TimerNTstrip->Enabled=false;
 	cueUpdateNametable=false;
	

    bool bBigTile=(PageControlEditor->ActivePage==TabSheetTile?true:false)&&!bForceNameScale;
	TRect r;


	ImageName->Picture->Bitmap->Assign(BufBmpName);
	if(nameSelection.left>=0&&nameSelection.top>=0)
	{
		r.left  =nameSelection.left  -nameTableViewX;
		r.right =nameSelection.right -nameTableViewX;
		r.top   =nameSelection.top   -nameTableViewY;
		r.bottom=nameSelection.bottom-nameTableViewY;

		if(!bBigTile) DrawSelection(ImageName,r,uiScale,false,false);
        if(FormNavigator->Visible) FormNavigator->UpdateLines(true);
	}
}
//---------------------------------------------------------------------------




void __fastcall TFormMain::ImportBitmap1Click(TObject *Sender)
{
	bool swap[256*2];
	int i,j,k,pp,col,bit;
	int ib, is, bankOff;
	unsigned char paltemp[4];

	bool bNoAttr;
	bool bNoPal;
	bool bDensityThres;
	bool bMaxTiles;
	bool bLossy;
	bool bBestOffsets;
	int mode;
	int iMT;
	int iPT;

	BlockDrawing(true);

	OpenDialogImport->Title="Import BMP";
	OpenDialogImport->Filter="Windows bitmap files (*.bmp)|*.bmp|All files (*.*)|*.*";
	OpenDialogImport->DefaultExt="bmp";

	if(OpenDialogImport->Execute())
	{
		SetUndo();
		memcpy (chrBuf, chr, 4096*2);    //prep buffer so we can perform previews
		memcpy (palBuf, bgPal, 4*16);
		memcpy (tmpNameTable, nameTable, NAME_MAX_SIZE);
		memcpy (tmpAttrTable, attrTable, ATTR_MAX_SIZE);
		buf_nameWidth = nameTableWidth;
		buf_nameHeight = nameTableHeight;
		
		FormImportBMP->CheckLossy->Checked=false;
		FormImportBMP->ShowModal();

		if(!FormImportBMP->Swap)
		{
			memcpy (chr, chrBuf, 4096*2);  // restore.
			memcpy (bgPal, palBuf, 4*16);
			memcpy (nameTable, tmpNameTable, NAME_MAX_SIZE);
			memcpy (attrTable, tmpAttrTable, ATTR_MAX_SIZE);
			nameTableWidth = buf_nameWidth;
			nameTableHeight =buf_nameHeight;
		}
		if(FormImportBMP->Swap && !FormImportBMP->CheckBox1->Checked)
		{
			memcpy (chr, chrBuf, 4096*2);  // restore.
			memcpy (bgPal, palBuf, 4*16);
			memcpy (nameTable, tmpNameTable, NAME_MAX_SIZE);
			memcpy (attrTable, tmpAttrTable, ATTR_MAX_SIZE);
			nameTableWidth = buf_nameWidth;
			nameTableHeight =buf_nameHeight;




			bNoAttr = FormImportBMP->CheckNoAttr->Checked;
			bNoPal = FormImportBMP->CheckNoPal->Checked;
			bDensityThres =FormImportBMP->CheckDensityThres->Checked;
			bLossy = FormImportBMP->CheckLossy->Checked;
			bBestOffsets = FormImportBMP->CheckBestOffsets->Checked;
			bMaxTiles = FormImportBMP->CheckMaxTiles->Checked;
			iMT = StrToInt(FormImportBMP->EditMaxTiles->Text);
			iPT = StrToInt(FormImportBMP->EditPxThres->Text);
			if(bLossy) mode=IMPORT_SQUEEZE;
			else mode=IMPORT_NORMAL;
			if(FormImportBMP->RadioMatched->Checked) mode=IMPORT_MATCH;

			//perform final import.
			if(ImportBMP(OpenDialogImport->FileName,mode,bBestOffsets,bDensityThres?iPT:-1,bMaxTiles?iMT:256,bNoAttr,bNoPal))
			{

			//perform palette+pattern swaps
            for(i=0;i<256*2;i++) swap[i]=false;
			if(FormImportBMP->Selection)
			{
				for(i=0;i<256;i++) swap[(bankActive/4096*256)+i]=chrSelected[i];
			}
			else
			{
				if(FormImportBMP->WholeCHR)
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
							col=FormImportBMP->Map[col];
							chr[pp]=(chr[pp]&~bit)|((col&1)<<k);
							chr[pp+8]=(chr[pp+8]&~bit)|((col>>1)<<k);
						}
						pp++;
					}
				}
			}

			if(FormImportBMP->RemapPalette)
			{
				if(FormImportBMP->RadioPalOne->Checked) {ib=iSwap_WhichSubpal; is=1; bankOff=palBank;}
					else if(FormImportBMP->RadioPalAll->Checked) {ib=0; is=4*4; bankOff=0;}
					else {ib=0;is=4;bankOff=palBank;}
					for(i=ib;i<is+ib;i++)
					{
						for(j=0;j<4;j++) paltemp[j]=bgPal[bankOff*16+i*4+FormImportBMP->Map[j]];
						for(j=0;j<4;j++) bgPal[bankOff*16+i*4+j]=paltemp[j];
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
			            /*
                pal_validate();
                
                			UpdateTiles(true);
                			UpdateNameTable(-1,-1,true);
                			FormNavigator->Draw(false,false);
                			DrawPalettes();
                			//UpdateMetaSprite();
                			cueUpdateMetasprite=true;    
            */
		}
	}
	}
	pal_validate();
    CorrectView();
	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	DrawPalettes();
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
	FormMain->CorrectView();
	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Findclosestmatch1Click(TObject *Sender)
{
		int n,j;
		int max=0;
		int	tile1=0;
		int	tile2=0;
		int tile3=0;

		for(j=0;j<255;j++)
		{
			n=tile_lossy_compare(&chr[bankActive+(tileActive*16)],&chr[bankActive+(j*16)]);

			if(n>max && tileActive!=j)
			{
				max=n;
				tile1=tile2;
				tile2=tile3;
				tile3=j;
			}
		}
	   chrSelected[ViewTablePos(tile1)]=1;
	   chrSelected[ViewTablePos(tile2)]=1;
	   chrSelected[ViewTablePos(tile3)]=1;

	   //turn off rect selection; instead displaying multi selection
	   chrSelectRect=false;
	   nameSelection.left=-1;
	   nameSelection.top=-1;

	   //update visuals
	   UpdateTiles(true);
	   UpdateNameTable(-1,-1,true);
	   if(FormNavigator->Visible) FormNavigator->UpdateLines(true);
	   UpdateStats();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SortbyDifference1Click(TObject *Sender)
{
	if(SortbyDifference1->Checked)
	{
		Undo();
		MCHRDensitySort->Checked=false;
		MCHRDensitySort->Caption="Sort by d&ifference";
	}
	else
	{
		int i,j,k,ptr,mask,temp;
		static int sort[256],freq[256];
		static unsigned char chrt[256*16];
        static uint32_t propt[sizeof(tileProperties)];
		SetUndo();

		for(i=0;i<256;++i)
		{
			sort[i]=i;
			freq[i]=0;

			ptr=i*16+bankActive;

			//tiles that have contents get a base point over tiles that don't.
			//this tidies up solid tiles with a single colour over empty tiles.
			freq[i]+=GetTileHasContents(&chr[ptr]);

			//vertical
			freq[i]+=count_diff_bytes(&chr[ptr],8,0xFF);
			freq[i]+=count_diff_bytes(&chr[ptr+8],8,0xFF);

			//horizontal
			for(j=0;j<16;++j)
			{

				//horizontal difference
				freq[i]+=count_diff_bits(chr[ptr]);
				++ptr;
			}
		}

		for(i=0;i<256-1;++i)
		{
			for(j=0;j<256-i-1;++j)
			{
				if(freq[j]<freq[j+1])
				{
					temp=freq[j];
					freq[j]=freq[j+1];
					freq[j+1]=temp;

					temp=sort[j];
					sort[j]=sort[j+1];
					sort[j+1]=temp;
				}
			}
		}
		//repair NT
		for(i=0;i<nameTableWidth*nameTableHeight;++i)
		{
			for(j=0;j<256;++j)
			{
				if(sort[j]==nameTable[i])
				{
					nameTable[i]=j;
					break;
				}
			}
		}
		//repair MSP.
		for(int i=0;i<256*64*4;i+=4)
		{
			for(j=0;j<256;++j)
			{
				if(sort[j]==metaSprites[i+1]) {metaSprites[i+1]=j; break;}
			}
		}


		memcpy(chrt,&chr[bankActive],sizeof(chrt));
		memcpy(propt,&tileProperties[bankActive/16],sizeof(propt));
		for(i=0;i<256;++i)
		{
			memcpy(&chr[bankActive+i*16],&chrt[sort[i]*16],16);
			memcpy(&tileProperties[(bankActive/16)+i],&propt[sort[i]],sizeof(uint32_t));
		}

		UpdateAll();
		MCHRDensitySort->Checked=true;
		MCHRDensitySort->Caption="Undo Sort by d&ifference";
		LabelStats->Caption="Tiles sorted by bitwise difference. \n\nTip: Press again to undo.";
		StatusUpdateWaiter->Enabled=true;
		holdStats=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Swapattributes1Click(TObject *Sender)
{
	//unsigned char paltemp[16*4];

	//we can also use ATTR_MAX_SIZE. In that case, remember to update the loop down below.
	int tmp_AttrSize = (nameTableWidth+3)/4*((nameTableHeight+3)/4);

	memcpy (palBuf, bgPal, 4*16);                      //buffer
	memcpy (tmpAttrTable, attrTable, tmp_AttrSize);
	memcpy (metaSpritesBuf, metaSprites,256*64*4);

	FormSwapAttributes->ShowModal();

	memcpy (bgPal, palBuf, 4*16);                      //restore
	memcpy (attrTable, tmpAttrTable, tmp_AttrSize);
	memcpy (metaSprites, metaSpritesBuf, 256*64*4);

	if(FormSwapAttributes->Swap)
	{
	   SetUndo();

		if(FormSwapColors->RemapPalette)
		{

			bool perform[4];
			for (int i = 0; i < 4; i++) perform[i]=false;

			if ((FormSwapAttributes->RadioPalAll->Checked) | (FormSwapAttributes->RadioPalOne->Checked)) {
				for (int i = 0; i < 4; i++) perform[i]=true;
				if (FormSwapAttributes->RadioPalOne->Checked) perform[palBank]=false;

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
				GetSelection(nameSelection,x,y,w,h);

				for(int i=0;i<h;i+=2)
				{
					for(int j=0;j<w;j+=2)
					{
						int tmp = FormSwapAttributes->Map[AttrGet(x+j,y+i,false,false)];
						AttrSet(x+j, y+i, tmp, false);

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


		}

	}
	pal_validate();

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);
	DrawPalettes();
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Clearrogueattributes1Click(TObject *Sender)
{
	int count=0;
	SetUndo();
	for(int x=0; x<nameTableWidth; x+=2){
		for(int y=0; y<nameTableHeight; y+=2){

			for(int i=0; i<2; i++){
				for(int j=0; j<2; j++){
					if (GetTileHasContents(&nameTable[(y+j)*nameTableWidth+x+i])) goto skip;
					}
				}

		if( palActive!=AttrGet(x,y,false,false)){
			count++;
			AttrSet(x, y, palActive, false);
        }
		skip:
		}
	}

	UpdateNameTable(-1,-1,true);
	FormNavigator->Draw(false,false);

	if(count==0) {
		LabelStats->Caption="No rogue attributes found.";
	}
	else {
		LabelStats->Caption=IntToStr(count)+" attribute cells set to current subpalette.";
	}

	StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Normal1Click(TObject *Sender)
{
	AnsiString str;
	bool b = PageControlEditor->ActivePage==TabSheetTile?true:false;
	int a = ((TMenuItem*)Sender)->Tag;

	tileViewTagBuf = a; //used to recall from the metasprite editor.
	SetViewTable(a);

	if (a==0) {
		ButtonTileView->Down=false;

		if(b) {
			TabSheetTile->Caption="[F3] Tiles";
			GroupBoxTiles->Caption="Screen (view only)";
		}
		else {
			GroupBoxTiles->Caption="Tileset";
		}
	}
	else{
		//str=((TMenuItem*)Sender)->Caption;
		ButtonTileView->Down=true;
		if(b) {
			TabSheetTile->Caption="[F3] Tiles <O>";
			GroupBoxTiles->Caption="Screen (view only)";
		}
		else {
			str="Tileset ("+((TMenuItem*)Sender)->Caption+")";
			RemoveCharacter(str.c_str(), '&');
			GroupBoxTiles->Caption=str;
		}
	}
	SetCheckMenuItem(((TMenuItem*)Tilesetmode1),a);         //view menu correction
	SetCheckPopupMenuItem(((TPopupMenu*)TileViewPop),a);    //popup menu correction
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SetViewTable(int tag)
{
	//create 8x16 vs 8x8 indirection table

	int num;
	//reset
	for(int i=0;i<256;i++) {tileViewTable[i]=i;}

	if(tag==1){
		for(int i=0;i<16;i++)
		{
			num=(i/2)*32+(i&1);
			for(int j=0;j<16;j++)
			{
				tileViewTable[i*16+j]=num;

				num+=2;
			}
		}
	}
	else if(tag==10) SetViewTable_SortFreqency();
	else if(tag==11) SetViewTable_SortDensity();
	else if(tag==12) SetViewTable_SortDetail();
	else if(tag==13) SetViewTable_SortEdgeDetail();
	else if(tag==14) SetViewTable_SortActiveColour();



	else if(tag==20) //4x1 -> 2x2 ; left to right blocks
	{
		SetViewTable_Destrip(2,true);
	}
	else if(tag==21) //4x1 -> 2x2 ; top down blocks
	{
		SetViewTable_Destrip(2,false);
	}
	else if(tag==22) //16x1 -> 4x4
	{
		SetViewTable_Destrip(4,true);
	}



	//normal / default
	else for(int i=0;i<256;i++) {tileViewTable[i]=i;}

	if(tag>0) ButtonTileView->Down=false;
	else ButtonTileView->Down=true;

	//UpdateMetaSprite();
	UpdateTiles(true);
	cueUpdateMetasprite=true;
	isLastClickedMetaSprite=true;
	isLastClickedSpriteList=false;


}

void __fastcall TFormMain::SetViewTable_Destrip(int block, bool reorder)
{
	 //block 2 means result is expected 2x2
	 //block 4 means result is expected 4x4
	 //the only other option that would make sense is 8 (64x1-->8x8)
	 int num;
	 int row = 16;
	 char tmp[256];
	 for(int x=0;x<16;x++){
		num=(x/block)*(block*row)+(x&(block-1));
		for(int y=0;y<16;y++){
			tmp[y*16+x]=num;
			num+=block;
		}
	 }


	 //arranges blocks left to right.

	 if(reorder){
		for(int x=0;x<16;x+=block){
			for(int y=0;y<16;y+=block){
				for(int bx=0;bx<block;bx++){
					for(int by=0;by<block;by++){
						tileViewTable[(x+by)*16+y+bx]=tmp[(y+by)*16+x+bx];
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < 256; i++) {
		   tileViewTable[i]=tmp[i];
		}
	}
}

void __fastcall TFormMain::SetViewTable_SortFreqency(void)
{
	 bool forceActive = ForceActiveTab1->Checked;
		bool sweepNT = IncludeNametables1->Checked;
		bool sweepMS = IncludeMetasprites1->Checked;

		int i,j,temp;
		int freq[256];

		memset(freq,0,sizeof(freq));

		if((forceActive)||!(sweepNT||sweepMS)) //if no sweep checkbox is set, also default to active.
		{
		   if(PageControlEditor->ActivePage==TabSheetSprite) {
                for(j=0;j<256*64*4;j+=4) if(metaSprites[j]!=0xFF) ++freq[metaSprites[j+1]];
			 }
		   else {
				for(i=0;i<nameTableHeight*nameTableWidth;++i) ++freq[nameTable[i]];
			}
		}
		else
		{
		   if(sweepNT) {
				for(i=0;i<nameTableHeight*nameTableWidth;++i) ++freq[nameTable[i]];
			}
		   if(sweepMS) {
				for(j=0;j<256*64*4;j+=4) if(metaSprites[j]!=0xFF) ++freq[metaSprites[j+1]];
			}
		}


		for(i=0;i<256;++i) tileViewTable[i]=i;

		for(i=0;i<256-1;++i)
		{
			for(j=0;j<256-i-1;++j)
			{
				if(freq[j]<freq[j+1])
				{
					temp=freq[j];
					freq[j]=freq[j+1];
					freq[j+1]=temp;

					temp=tileViewTable[j];
					tileViewTable[j]=tileViewTable[j+1];
					tileViewTable[j+1]=temp;
				}
			}
		}
}

void __fastcall TFormMain::SetViewTable_SortDensity(void)
{
	int i,j,k,ptr,mask,temp;
	int freq[256];



	for(i=0;i<256;++i)
	{
		tileViewTable[i]=i;
		freq[i]=0;

		ptr=i*16+bankActive;

		for(j=0;j<8;++j)
		{
			mask=128;

			for(k=0;k<8;++k)
			{
				if((chr[ptr]&mask)||(chr[ptr+8]&mask)) ++freq[i];

				mask>>=1;
			}

				++ptr;
		}
	}

	for(i=0;i<256-1;++i)
		{
		for(j=0;j<256-i-1;++j)
		{
			if(freq[j]<freq[j+1])
			{
				temp=freq[j];
				freq[j]=freq[j+1];
				freq[j+1]=temp;

				temp=tileViewTable[j];
				tileViewTable[j]=tileViewTable[j+1];
				tileViewTable[j+1]=temp;
			}
		}
	}
}

void __fastcall TFormMain::SetViewTable_SortDetail(void)
{
	int i,j,k,ptr,mask,temp;
	int freq[256];

	for(i=0;i<256;++i)
	{
		tileViewTable[i]=i;
		freq[i]=0;

		ptr=i*16+bankActive;

		//tiles that have contents get a base point over tiles that don't.
		//this tidies up solid tiles with a single colour over empty tiles.
		freq[i]+=GetTileHasContents(&chr[ptr]);

		//vertical
		freq[i]+=count_diff_bytes(&chr[ptr],8,0xFF);
		freq[i]+=count_diff_bytes(&chr[ptr+8],8,0xFF);

		//horizontal
		for(j=0;j<16;++j)
		{

			//horizontal difference
			freq[i]+=count_diff_bits(chr[ptr]);
			++ptr;
		}
	}

	for(i=0;i<256-1;++i)
	{
		for(j=0;j<256-i-1;++j)
		{
			if(freq[j]<freq[j+1])
			{
				temp=freq[j];
				freq[j]=freq[j+1];
				freq[j+1]=temp;

				temp=tileViewTable[j];
				tileViewTable[j]=tileViewTable[j+1];
				tileViewTable[j+1]=temp;
			}
		}
	}

}

void __fastcall TFormMain::SetViewTable_SortEdgeDetail(void)
{
   int i,j,k,ptr,mask,temp;
   int	freq[256];

	for(i=0;i<256;++i)
	{
		tileViewTable[i]=i;
		freq[i]=0;

		ptr=i*16+bankActive;

		//tiles that have contents get a base point over tiles that don't.
		//this tidies up solid tiles with a single colour over empty tiles.
		freq[i]+=GetTileHasContents(&chr[ptr]);

		//gives a relatively strong score for difference: max 56
		//vertical
		freq[i]+=count_diff_bytes(&chr[ptr],8,0x81);
		freq[i]+=count_diff_bytes(&chr[ptr+8],8,0x81);

		//horizontal difference
		freq[i]+=count_diff_bits(chr[ptr]);
		freq[i]+=count_diff_bits(chr[ptr+7]);
		freq[i]+=count_diff_bits(chr[ptr+8]);
		freq[i]+=count_diff_bits(chr[ptr+15]);


		//

		//gives a weaker modification for solidity. max 28
		int imask=128;

		for(k=0;k<8;++k)
		{
			if((chr[ptr]&imask)||(chr[ptr+8]&imask)) ++freq[i];
			if((chr[ptr+7]&imask)||(chr[ptr+15]&imask)) ++freq[i];

			if(((chr[ptr+1]&0x81)&imask)||((chr[ptr+9]&0x81)&imask)) ++freq[i];
			if(((chr[ptr+2]&0x81)&imask)||((chr[ptr+10]&0x81)&imask)) ++freq[i];
			if(((chr[ptr+3]&0x81)&imask)||((chr[ptr+11]&0x81)&imask)) ++freq[i];
			if(((chr[ptr+4]&0x81)&imask)||((chr[ptr+12]&0x81)&imask)) ++freq[i];
			if(((chr[ptr+5]&0x81)&imask)||((chr[ptr+13]&0x81)&imask)) ++freq[i];
			if(((chr[ptr+6]&0x81)&imask)||((chr[ptr+14]&0x81)&imask)) ++freq[i];

				imask>>=1;
			}
	}


   for(i=0;i<256-1;++i)
	{
		for(j=0;j<256-i-1;++j)
		{
			if(freq[j]<freq[j+1])
			{
				temp=freq[j];
				freq[j]=freq[j+1];
				freq[j+1]=temp;

				temp=tileViewTable[j];
				tileViewTable[j]=tileViewTable[j+1];
				tileViewTable[j+1]=temp;
			}
		}
	}

}

void __fastcall TFormMain::SetViewTable_SortActiveColour(void)
{
	int i,j,k,ptr,mask,temp;
	int freq[256];



	for(i=0;i<256;++i)
	{
		tileViewTable[i]=i;
		freq[i]=0;

		ptr=i*16+bankActive;

		for(j=0;j<8;++j)
		{
			mask=128;

			for(k=0;k<8;++k)
			{
				switch(bgPalCur){
				case 0:
					if(((~chr[ptr])&mask)&&((~chr[ptr+8])&mask)) ++freq[i];
					break;
				case 1:
					if((chr[ptr]&mask)&&((~chr[ptr+8])&mask)) ++freq[i];
					break;
				case 2:
					if(((~chr[ptr])&mask)&&(chr[ptr+8]&mask)) ++freq[i];
					break;
				case 3:
					if((chr[ptr]&mask)&&(chr[ptr+8]&mask)) ++freq[i];
					break;
				}

				mask>>=1;
			}

				++ptr;
		}
	}

	for(i=0;i<256-1;++i)
		{
		for(j=0;j<256-i-1;++j)
		{
			if(freq[j]<freq[j+1])
			{
				temp=freq[j];
				freq[j]=freq[j+1];
				freq[j+1]=temp;

				temp=tileViewTable[j];
				tileViewTable[j]=tileViewTable[j+1];
				tileViewTable[j+1]=temp;
			}
		}
	}

}




void __fastcall TFormMain::ButtonTileViewMouseEnter(TObject *Sender)
{
	LabelStats->Caption="View mode:\t Lets you view and edit the tileset in arrangements\nfreed from physical memory layout.";
	StatusUpdateWaiter->Enabled=true;
	holdStats=true;

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonTileViewMouseLeave(TObject *Sender)
{
	LabelStats->Caption="---";
	holdStats=false;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonTileViewClick(TObject *Sender)
{
	TPoint p = Mouse->CursorPos;
	int x= p.x; //+ ButtonTileView->Left+12;
	int y= p.y; //+ ButtonTileView->Top+8;
	TileViewPop->Popup(x,y);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Mirror1Click(TObject *Sender)
{
	int x,y,w,h;
	int i,j,k,l;
	int pw,ph;
	int ps; //source
	int pd; //dest
	int pp;
	int hMid,wMid;
	//int wOdd;
	//int hOdd;
	int tmp;    //sliver
	char tmpTile[16]; //tile
	char midMask; //midtile bitmask
	char vMask[16];	//used by vertical masks


	int fLen=16;
	int fOff=0;

	//used by colour masks
	//char mask[16]; //colour protect mask
	/*
	bool b0 = FormCHREditor->Protect0->Down;
	bool b1 = FormCHREditor->Protect1->Down;
	bool b2 = FormCHREditor->Protect2->Down;
	bool b3 = FormCHREditor->Protect3->Down;
	*/

	//apply bitplane masks
	if (!(FormCHREditor->ButtonBitmaskLo->Down||FormCHREditor->ButtonBitmaskHi->Down))
		{if(Applytopaste1->Checked) return;}
	if (!FormCHREditor->ButtonBitmaskLo->Down&&Applytopaste1->Checked) {fLen-=8; fOff=8;}
	if (!FormCHREditor->ButtonBitmaskHi->Down&&Applytopaste1->Checked) {fLen-=8;}

	int tag = ((TMenuItem*)Sender)->Tag;



	if(chrSelectRect)
	{
		SetUndo();
		GetSelection(chrSelection,x,y,w,h);
		//if (w%2!=0) wOdd=1; else wOdd=0;
		//if (h%2!=0) hOdd=1; else hOdd=0;

		if((tag==0) || (tag==2)) //Horz mirror
		{
			for(i=0;i<h;i++){
				for(j=0;j<w;j++){
					for(k=0;k<16;k++){
						ps=bankActive + tileViewTable[(x+j) + (y+i)*16]*16 + k;
						chrCopy[ps]=chr[ps];
					}
				}
			}

			//mirror pixels horz
			for (i = 0; i < h; i++){
				for (j = 0; j < w; j++){
					for(k=0;k<16;k++){
						tmp=0;
						ps=bankActive + tileViewTable[(x+j) + (y+i)*16]*16 + k;
						for(l=0;l<8;l++){
							tmp|=(chrCopy[ps]&(128>>l))?1<<l:0;
						}
						chrCopy[ps]=tmp;
					}
				}
			}

			//paste in reverse horz order;

			ph=0;
			for(i=0;i<h;i++)   ////2-hOdd    //(i=h-1;i>=h/2+hOdd;i--)
			{
				pw=0;
				for(j=w-1;j>=w/2;j--)  //+wOdd;
				{
					if(x+j<16&&y+i<16)  //safety for making sure selection isn´t larger than table.
					{
						if((j==0) || (pw==w/2)) midMask=0x0F; else midMask=0xFF;
						ps=bankActive + tileViewTable[(x+pw) + (y+i)*16]*16;
						for(k=0;k<fLen;k++)  //pasting tile
						{
							pd=bankActive + tileViewTable[(x+j) + (y+ph)*16]*16 + k;
							chr[pd+fOff]=(chr[pd+fOff]&(~midMask));
							chr[pd+fOff]=chr[pd+fOff]|(chrCopy[ps+k+fOff]&midMask);
						}
					}
				pw++;
				}
			ph++;
			}
		}

		if((tag==1) || (tag==2)) //Vert mirror
		{
			//make buffer
			for(i=0;i<h;i++){
				for(j=0;j<w;j++){
					for(k=0;k<16;k++){
						ps=bankActive + tileViewTable[(x+j) + (y+i)*16]*16 + k;
						chrCopy[ps]=chr[ps];
					}
				}
			}

			//mirror pixels vert
			for (i = 0; i < h; i++){
				for (j = 0; j < w; j++){
					
					pp=bankActive + tileViewTable[(x+j) + (y+i)*16]*16;

					for(k=0;k<16;k++){
						tmpTile[k]=chrCopy[pp];
						pp++;
					}
					for(k=0;k<16;k++){
						pp--;
						chrCopy[pp]=tmpTile[k];
					}
				}
			}

			//paste in reverse vert order;

			ph=0;
			for(i=h-1;i>=h/2;i--)
			{
				pw=0;
				for(j=0;j<w;j++)            
				{
					if(x+j<16&&y+i<16)  //safety for making sure selection isn´t larger than table.
					{
						if((i==0) || (ph==h/2)) midMask=0x00; else midMask=0xFF;
						for(k=0;k<4;k++) {vMask[k]=midMask;  vMask[k+8]=midMask;}
						for(k=4;k<8;k++) {vMask[k]=0xFF;     vMask[k+8]=0xFF;}

						ps=bankActive + tileViewTable[(x+j) + (y+ph)*16]*16;
						for(k=0;k<fLen;k++)  //pasting tile
						{


							pd=bankActive + tileViewTable[(x+j) + (y+i)*16]*16 + k;
							chr[pd+fOff]=(chr[pd+fOff]&(~vMask[k+fOff]));
							chr[pd+fOff]=chr[pd+fOff]|(chrCopy[ps+k+fOff]&vMask[k+fOff]);
						}
					}
				}
			ph++;
			}
		}
	}
	else //multiselect behaviour. if we want that.
	{

	}

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	if (FormNavigator->Visible) FormNavigator->Draw(false,false);
	cueUpdateMetasprite=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Linetooldetails1Click(TObject *Sender)
{
	FormLineDetails->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLineMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//if(Shift.Contains(ssRight))
	if(!FormLineDetails->Visible) FormLineDetails->Show();

		
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLineClick(TObject *Sender)
{
	if(Sender==btnLine)FormCHREditor->btnLine->Down = btnLine->Down;
	if(Sender==btnQuant)FormCHREditor->btnQuant->Down = btnQuant->Down;
	if(Sender==btnThick)FormCHREditor->btnThick->Down = btnThick->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButton1UpClick(TObject *Sender)
{
	if(Sender==SpeedButton1Left)
		FormCHREditor->ScrollHorz(true);
	if(Sender==SpeedButton1Right)
		FormCHREditor->ScrollHorz(false);
	if(Sender==SpeedButton1Up)
		FormCHREditor->ScrollVert(false);
	if(Sender==SpeedButton1Down)
		FormCHREditor->ScrollVert(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButton1UpMouseEnter(TObject *Sender)
{
	AnsiString str;
	bool b =(FormBrush->Active|FormCHREditor->Active);
	if (b)	str="[WASD] Nudges pattern of the selected tile(s) ";
	else str="Nudges pattern of the selected tile(s) ";
	if(Sender==SpeedButton1Up) str+="up.";
	if(Sender==SpeedButton1Down) str+="down.";
	if(Sender==SpeedButton1Left) str+="left.";
	if(Sender==SpeedButton1Right) str+="right.";
	str +="\nWraparound occurs at the seams of the selection.";
	FormMain->LabelStats->Caption=str;	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Protect0MouseEnter(TObject *Sender)
{
	int n;
	AnsiString str;
	if(Sender==Protect0) n=0;
	if(Sender==Protect1) n=1;
	if(Sender==Protect2) n=2;
	if(Sender==Protect3) n=3;
	str="Protects pixels of colour "+IntToStr(n)+" from being drawn over."
		+"\n\n[Click] to Toggle.\t[Ctrl+click] to enable all but."
		+"\n[Shift + click] to enable all.\t[Alt+click] to 'solo'.";
	FormMain->LabelStats->Caption=str;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Protect0MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) //set everyone else
	{
			Protect0->Down=true;    //bc click applies after, this one is inverted
			Protect1->Down=true;
			Protect2->Down=true;
			Protect3->Down=true;
	}
	if(Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) //clear everyone else
	{
		if(Protect0->Down && (Protect1->Down||Protect2->Down||Protect3->Down))
		{
			Protect0->Down=false; //force always down, except if others are already up

		}
		Protect1->Down=false;
		Protect2->Down=false;
		Protect3->Down=false;
	}
	if(Shift.Contains(ssShift))
	{

		Protect1->Down=false;
		Protect2->Down=false;
		Protect3->Down=false;
		if(!Protect0->Down)
		{
			Protect0->Down=true; //force always up

		}
	}	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonHFlipClick(TObject *Sender)
{
	if(!lineDrawing)FormCHREditor->MirrorHorizontal();	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonHFlipMouseEnter(TObject *Sender)
{
	AnsiString str;
	str="[";
	//if (!b) str+="Shift+";      //nope.
	if(Sender==SpeedButtonHFlip) str+="H] ";
	if(Sender==SpeedButtonVFlip) str+="V] ";
	if(Sender==SpeedButtonRotateCCW) str+="L] ";
	if(Sender==SpeedButtonRotateCW) str+="R] ";


	str+="Flips the pattern of the selected tile(s) ";

	if(Sender==SpeedButtonHFlip) str+="\nhorizontally.";
	if(Sender==SpeedButtonVFlip) str+="\nvertically.";
	if(Sender==SpeedButtonRotateCCW) str+="\ncounter clockwise.";
	if(Sender==SpeedButtonRotateCW) str+="\nclockwise.";

	if(FormCHREditor->btn2x2mode->Down) str +="\nIn 2x2 mode; tiles also swap places so long as 2x2 tiles are box selected.";
	FormMain->LabelStats->Caption=str;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonVFlipClick(TObject *Sender)
{
	if(!lineDrawing)FormCHREditor->MirrorVertical();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonRotateCCWClick(TObject *Sender)
{
	if(!lineDrawing){
	FormCHREditor->Flip90(true);
	FormCHREditor->Rotate4tiles(true);

	FormCHREditor->Draw(true);

	UpdateNameTable(-1,-1,true);
	UpdateTiles(false);
	UpdateMetaSprite();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonRotateCWClick(TObject *Sender)
{
	if(!lineDrawing){
	FormCHREditor->Flip90(false);
	FormCHREditor->Rotate4tiles(false);
	FormCHREditor->Draw(true);

	UpdateNameTable(-1,-1,true);
	UpdateTiles(false);
	UpdateMetaSprite();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonBitmaskLoMouseEnter(TObject *Sender)
{
	AnsiString str;
	if(Sender==ButtonBitmaskLo && ButtonBitmaskLo->Down)  str="Disable editing the Low bitplane.";
	if(Sender==ButtonBitmaskLo && !ButtonBitmaskLo->Down) str="Enable editing the Low bitplane.";
	if(Sender==ButtonBitmaskHi && ButtonBitmaskHi->Down)  str="Disable editing the High bitplane.";
	if(Sender==ButtonBitmaskHi && !ButtonBitmaskHi->Down) str="Enable editing the High bitplane.";

	str+="\nNES patterns are stored in 2 bitplanes.\nIt can sometimes be useful to edit them separately.\nSee [Menu->Draw->Bitmask options...] for options.";
	FormMain->LabelStats->Caption=str;	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonDoWrapMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="When down, the +/- buttons will wrap pixels\nat max colour around to min,or vise versa.\n\nWhen up, the buttons cap at max / min value.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CHRIncMouseEnter(TObject *Sender)
{
	AnsiString str;
	if(Sender==CHRInc) {str="Increments pixel value in the selected tile(s).";

		str+="\n\nWhen a subpalette is ordered dark to bright,\nthis will make tiles brighter.";
		}
	if(Sender==CHRDec) {str="Decrements pixel value in the selected tile(s).";

		str+="\n\nWhen a subpalette is ordered dark to bright,\nthis will make tiles darker.";
		}
	FormMain->LabelStats->Caption=str;		
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::CHRIncClick(TObject *Sender)
{
	 FormCHREditor->CHRIncDec(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnQuantMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="[U] Makes pen (etc) valid only for even coordinates.\n\nTip:\tWith brush, it's great for creating dither,\n\t'chunky pixels' or striped line patterns.";
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::btnThickMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssRight))	FormBrush->Show();

	FormMain->TogglePenBrush1->Checked=btnThick->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnThickMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="[B] Toggles between Pen and Brush tool \non the [left mouse button].\nTip:\tRight-click this button or press [Shift-F4] \n\tto open brush shape editor.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnSmudgeMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Uses the clicked pixel's colour when drawing.\nGood for defining borders/clusters.\nHolding [G] enables smudge momentarily.\nLifting [G] always releases this button.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnSmudgeClick(TObject *Sender)
{
	bSmudge=btnSmudge->Down; FormCHREditor->btnSmudge->Down=btnSmudge->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonDoWrapClick(TObject *Sender)
{
	FormCHREditor->SpeedButtonDoWrap->Down=SpeedButtonDoWrap->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Protect0Click(TObject *Sender)
{
	FormCHREditor->Protect0->Down=Protect0->Down;
	FormCHREditor->Protect1->Down=Protect1->Down;
	FormCHREditor->Protect2->Down=Protect2->Down;
	FormCHREditor->Protect3->Down=Protect3->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonBitmaskLoClick(TObject *Sender)
{
	FormCHREditor->ButtonBitmaskHi->Down= ButtonBitmaskHi->Down;
	FormCHREditor->ButtonBitmaskLo->Down= ButtonBitmaskLo->Down;

    ButtonBitmaskLoMouseEnter(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::CHRBankSelector1Click(TObject *Sender)
{
	if (FormBankCHR->WindowState==wsMinimized) FormNavigator->WindowState=wsNormal;
	else FormBankCHR->Visible^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CHRcollision1Click(TObject *Sender)
{
	FormCHRbit->Show();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Savepropertiescurrentset1Click(TObject *Sender)
{
   BlockDrawing(true);
   SaveTprop(tileActive,256);
   BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Savepropertiesbothsets1Click(TObject *Sender)
{
  BlockDrawing(true);
  SaveTprop(tileActive,512);
  BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Loadproperties1Click(TObject *Sender)
{
	if(!OpenDialogTprop->Execute()) return;

	BlockDrawing(true);

	if(OpenTprop(OpenDialogTprop->FileName))
	{
		OpenDialogTprop->FileName=RemoveExt(OpenDialogTprop->FileName);

		SaveDialogTprop->FileName=OpenDialogTprop->FileName;
	}

	BlockDrawing(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Swappatterncolours1Click(TObject *Sender)
{

	bool swap[256*2];
	int i,j,k,pp,col,bit;
	int ib, is, bankOff;
	unsigned char paltemp[4];

	memcpy (chrBuf, chr, 4096*2);    //prep buffer so we can perform previews
	//memcpy (palBuf, bgPal, 4*16);

	FormSwapPatternColour->ShowModal();

	memcpy (chr, chrBuf, 4096*2);  // restore.
	//memcpy (bgPal, palBuf, 4*16);

	if(FormSwapPatternColour->Swap)
	{
		SetUndo();

		for(i=0;i<256*2;i++) swap[i]=false;

		if(FormSwapPatternColour->Selection)
		{
			for(i=0;i<256;i++) swap[(bankActive/4096*256)+i]=chrSelected[i];
		}
		else
		{
			if(FormSwapPatternColour->WholeCHR)
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
						col=FormSwapPatternColour->Map[col];
						chr[pp]=(chr[pp]&~bit)|((col&1)<<k);
						chr[pp+8]=(chr[pp+8]&~bit)|((col>>1)<<k);
					}
					pp++;
				}
			}
		}
	}
    pal_validate();

	UpdateTiles(true);
	UpdateNameTable(-1,-1,true);
	if(FormNavigator->Visible) FormNavigator->Draw(false,false);
	DrawPalettes();
	//UpdateMetaSprite();
	cueUpdateMetasprite=true;
	
}
//---------------------------------------------------------------------------






void __fastcall TFormMain::SpeedButtonMarqTileMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bDrawMarqSwitch = SpeedButtonMarqTile->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonMarqTileMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	if(bDrawMarqSwitch) SpeedButtonDrawTile->Down^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonDrawTileMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   bDrawMarqSwitch = SpeedButtonDrawTile->Down;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SpeedButtonDrawTileMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if(bDrawMarqSwitch) SpeedButtonMarqTile->Down^=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBoxPalMouseEnter(TObject *Sender)
{
	if(uiScale<3) GroupBoxTileControl->Visible=false;	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageBigTilesMouseEnter(TObject *Sender)
{
	if(uiScale>1 && PageControlEditor->ActivePage==TabSheetTile)
	{	GroupBoxTileControl->Visible=true;}
	else
	{	GroupBoxTileControl->Visible=false;	}	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLineMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="[N] Toggles between Pen and Line tool \non the [left mouse button].\nTip:\tClicking or pressing[Shift-F5] \n\topens the Line toolbox.";	
}
//---------------------------------------------------------------------------

