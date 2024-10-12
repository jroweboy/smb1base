//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <vector>
#include <algorithm>


#pragma hdrstop

#include "UnitMetatileEditor.h"
#include "UnitName.h"
#include "UnitMain.h"
#include "UnitMTprops.h"
#include "UnitNavigator.h"
#include "UnitCHREditor.h"
#include "UnitManageMetasprites.h"
#include "UnitLineDetails.h"
#include "UnitBucketToolbox.h"
#include "UnitBrush.h"
#include "math.h"
#include "UnitPropertyConditions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMetatileTool *FormMetatileTool;

AnsiString RemoveExt(AnsiString name)
{
	return ChangeFileExt(name,"");
}


inline int min(int a, int b) {
	return (a < b) ? a : b;
}
inline int max(int a, int b) {
	return (a > b) ? a : b;
}

inline bool CompareTempToSet(const char *wildcard_mask, const char *array1, const char *array2, size_t len, size_t id){
			for (size_t i = 0; i < len; i++) {
				if(array1[i] != array2[id + i]){
					if(wildcard_mask[i]==0)return false; //mismatch detected
				}
			}
			return true;
}
inline bool CompareBufToSet(const char *array1, size_t idBuf, const char *array2, size_t idSet, size_t len){
			for (size_t i = 0; i < len; i++) {
				if(array1[idBuf + i] != array2[idSet + i]){
					return false; //mismatch detected
				}
			}
			return true;
}

inline bool CompareTempToSet32(const uint32_t *array1, const uint32_t *array2, size_t len, size_t id){
			for (size_t i = 0; i < len; i++) {
				if(array1[i] != array2[id + i]){
					return false; //mismatch detected
				}
			}
			return true;
}

const int META_2x2 = 4*64; //4 tiles, 64 metas
const int META_4x4 = 16*64;
const int META_8x8 = 64*64;
AnsiString strListMT;

int prevSetCursorX= -1;
int prevSetCursorY= -1;

extern TRect nameSelection;

extern unsigned int iMapMatchCnt;
extern int prevmtClickID;

unsigned char tmp_nametable_match[NAME_MAX_SIZE];       //4096*4096

extern bool openByFileDone;
extern bool bKeyEscape;
extern bool holdStats;

extern bool cueUpdateNametable;
extern bool cueUpdateNametableNoRepaint;
extern bool cueUpdateNTstrip;

extern bool clickV;
extern bool clickC;

extern bool bBufCtrl;
extern bool bBufShift;
extern bool bBufAlt;

extern bool bDrawDestShadow;

extern bool bMtMultiSelectRemoveMode;
extern bool chrSelectRect;
extern TRect chrSelection;

extern char propConditional[];
extern bool propCondition[4][8];

extern unsigned char tileViewTable[]; //possibly not needed
extern unsigned int bankViewTable[];
extern int palActive;
extern int nullTile;
extern int tileActive;
extern int bankActive;

extern int CHRCollisionGranularityX;
extern int CHRCollisionGranularityY;
extern int MetaCollisionGranularityX;
extern int MetaCollisionGranularityY;

//extern uint32_t tileProperties[];
extern unsigned char *tileProperties;

extern int nameTableHeight;
extern int nameTableWidth;
extern unsigned char nameTable[];
extern char chrSelected[];
extern int mtPropsActive;
extern int32_t metatileSets_2x2;
extern int32_t metatileSets_4x4;
extern int32_t metatileSets_8x8;

extern std::vector<std::string> metatileSetLabels_2x2;
extern std::vector<std::string> metatileSetLabels_4x4;
extern std::vector<std::string> metatileSetLabels_8x8;

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

//----metatile unpacking space for merging outside sets.
extern int buf_metatileSets_2x2;
extern int buf_metatileSets_4x4;
extern int buf_metatileSets_8x8;


extern char *metatileBuf_2x2_id;
extern char *metatileBuf_2x2_pal;
extern char *metatileBuf_2x2_props;

extern char *metatileBuf_4x4_id;
extern char *metatileBuf_4x4_pal;
extern char *metatileBuf_4x4_props;

extern char *metatileBuf_8x8_id;
extern char *metatileBuf_8x8_pal;
extern char *metatileBuf_8x8_props;

AnsiString dialogTitle_AppendMetatiles;

int cnt_appendFromFileMatch;
int	cnt_appendFromFileUnique;

extern int iSetView;
extern int iSetViewOff;
extern int iListViewOff;
extern int mtClickID;
extern unsigned int highlight_mt;
extern int mtClickID_store_2x2;
extern int mtClickID_store_4x4;
extern int mtClickID_store_8x8;


extern TRect destRect;
TRect metaSelection_2x2;
TRect metaSelection_4x4;
TRect metaSelection_8x8;

extern int mxDown;  //metaset down
extern int myDown;


int metaDownXc=0;   //metatile down
int metaDownYc=0;

TRect setSelection;
TRect setCursor;
TRect metaCursor;

bool isSetCursor=false;
bool isMetaCursor=false;

bool setHover=false;
bool metaHover=false;

bool mtClickSent=false;

bool bSetOrClear;

extern bool metaSelectMulti;
extern unsigned char metaSelected_2x2[];
extern unsigned char metaSelected_4x4[];
extern unsigned char metaSelected_8x8[];

//maybe use these more in revision
unsigned char *currentTable_id = NULL;
unsigned char *currentTable_pal = NULL;
unsigned char *currentTable_props = NULL;
char *currentTable_content = NULL;
unsigned char *currentMetaSelected = NULL;
TRect *currentMetaSelection = NULL;

extern bool mmc2_startSet_use1st_mt;
extern bool mmc2_startSet_useActive_mt;
extern bool mmc2_startSet_useFixedSet_mt;
extern unsigned int mmc2_startSet_fixed_mt;
extern bool mmc2_startSet_asPrevious_mt;
extern bool mmc2_doublepair;
extern bool bMMC2Switch;
extern bool mmc2_DoublePair1stSet;
extern unsigned int current_switchTileTarget;
extern int mmc2_checked1;
extern bool mmc2_Doublepair_1stSet;
//---------------------------------------------------------------------------
__fastcall TFormMetatileTool::TFormMetatileTool(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::BuildMetas(int whichType, int reserveOff)
{


	bool match;
	int d, tmp;
    int set = bankActive/16;
	bool doPropConditions = FormPropConditions->chkMetas->Checked;
	bool doAlign = chkAlignScr->Checked;
	bool clearTmp = false;
	bool doPal = btnUseAttr->Down;
	if(whichType==2){d=2;}
	if(whichType==4){d=4;}
	if(whichType==8){d=8;}

	unsigned char *tmp_meta_id = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_nullcmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_meta_pal = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_palcmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_meta_props = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_propscmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_wildcard = (char*)calloc((d*d),sizeof(char));



	memset(tmp_meta_nullcmp,nullTile,d*d);
	memset(tmp_meta_palcmp,0,d*d);
	memset(tmp_meta_propscmp,0,d*d);

	//take content
	if(whichType==2){
		//memset(mtUsage_2x2,0,metatileSets_2x2*64*sizeof(uint32_t));

		for (int i = 0; i < metatileSets_2x2*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			mtContent_2x2[i]= memcmp(tmp_meta_nullcmp ,metatileSet_2x2_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(mtContent_2x2[i]==0) mtContent_2x2[i]= memcmp(tmp_meta_palcmp ,metatileSet_2x2_pal+(i*d*d) ,d*d);
			if(mtContent_2x2[i]==0) mtContent_2x2[i]= memcmp(tmp_meta_propscmp ,metatileSet_2x2_props+(i*d*d) ,d*d);
		}
	}

	if(whichType==4){
		//memset(mtUsage_4x4,0,metatileSets_4x4*64*sizeof(uint32_t));

		for (int i = 0; i < metatileSets_4x4*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			mtContent_4x4[i]= memcmp(tmp_meta_nullcmp ,metatileSet_4x4_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(mtContent_4x4[i]==0) mtContent_4x4[i]= memcmp(tmp_meta_palcmp ,metatileSet_4x4_pal+(i*d*d) ,d*d);
			if(mtContent_4x4[i]==0) mtContent_4x4[i]= memcmp(tmp_meta_propscmp ,metatileSet_4x4_props+(i*d*d) ,d*d);

		}
	}

	if(whichType==8){
		//memset(mtUsage_8x8,0,metatileSets_8x8*64*sizeof(uint32_t));

		for (int i = 0; i < metatileSets_8x8*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			mtContent_8x8[i]= memcmp(tmp_meta_nullcmp ,metatileSet_8x8_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(mtContent_8x8[i]==0) mtContent_8x8[i]= memcmp(tmp_meta_palcmp ,metatileSet_8x8_pal+(i*d*d) ,d*d);
			if(mtContent_8x8[i]==0) mtContent_8x8[i]= memcmp(tmp_meta_propscmp ,metatileSet_8x8_props+(i*d*d) ,d*d);

		}
	}



	//sample nametable contents
	int tmpD;
	bool skippass;
	unsigned char tmpProp;
	unsigned char tmpPal;
	for (int pass=0; pass < 2; pass++) {
	//we do 2 passes, first for whole metatiles, and then for half-metatiles
	for (int i=0; i < nameTableHeight ; i+=d) {
		if(pass==0) skippass=false;
		else	    skippass=true;
		//special rules for screen grid alignment, pt 1
		if ((i % 30 == 28) && d==4 && doAlign) {tmpD=2; clearTmp=true; skippass=!skippass;}
		else if((i % 30 == 24)&& d==8 && doAlign) {tmpD=6; clearTmp=true; skippass=!skippass;}
		else{tmpD = d; clearTmp=false;}

		if(clearTmp){
			memset(tmp_meta_id,nullTile,d*d);
			memset(tmp_meta_pal,0,d*d);
			memset(tmp_meta_props,0,d*d);

			if(d==4){
				memset(tmp_wildcard,0,(d*d)/2);
				memset(tmp_wildcard+((d*d)/2),1,(d*d)/2);
			}
			else if(d==8){
				memset(tmp_wildcard,0,((d*d)/4)*3);
				memset(tmp_wildcard+(((d*d)/4)*3),1,(d*d)/4);
			}
		}
		else{
			memset(tmp_wildcard,0,d*d);
		}

		for (int j = 0; j < nameTableWidth ; j+=d) {

			for (int y = 0; y < tmpD; y++) {
				for (int x = 0; x < d; x++) {

					tmp_meta_id[x + y*d] = nameTable[(i+y)*nameTableWidth+(j+x)];
					tmp_meta_pal[x + y*d] = FormMain->AttrGet(j+x,i+y, false, false);

					if(doPropConditions){
						int tn = nameTable[(i+y)*nameTableWidth+(j+x)];
						tmpProp = tileProperties[bankViewTable[set+tn]/16+tn];
						tmpPal = tmp_meta_pal[x + y*d];

						//1st pass for conditions - the bitwise condition.
						for(int bit=0;bit<8;bit++){
							if(propConditional[bit]==1 && ((tmpProp >> bit) & 1)){
								tmpProp = propCondition[tmpPal][bit]? tmpProp:tmpProp &= ~(1 << bit);
							}

						}
						//2nd pass for conditions - the 'all' condition.
						for(int bit=0;bit<8;bit++){

							//if a bit is set that has the 'all conditional', nullify properties if there isn't a palette match.
							if(propConditional[bit]==2 && ((tmpProp >> bit) & 1)){
								tmpProp = propCondition[tmpPal][bit]? tmpProp:0;
							}
						}

							tmp_meta_props[x + y*d] = tmpProp;
					}
					else{
						int tn = nameTable[(i+y)*nameTableWidth+(j+x)];
						tmp_meta_props[x + y*d] = tileProperties[bankViewTable[set+tn]/16+tn];
					}

				}
			}
			if(skippass) goto Next;
			//find a match
			if(whichType==2) {
				for (int m = reserveOff; m < (metatileSets_2x2*64); m++) {
					match = CompareTempToSet(tmp_wildcard,tmp_meta_id,metatileSet_2x2_id,d*d,m*d*d);
					if(match==true)
					{
						//does the palette data match too?
						match = CompareTempToSet(tmp_wildcard,tmp_meta_pal,metatileSet_2x2_pal,d*d,m*d*d);
						if((match==true) || (doPal==false))
						{
							match = CompareTempToSet(tmp_wildcard,tmp_meta_props,metatileSet_2x2_props,d*d,m*d*d);
							if(match==true) goto Next; //already in set, don't place
						}
					}

				}

				//no match, put in empty slot (potentially unused too)
				for ( int n = reserveOff; n < (64*metatileSets_2x2); n++) {
					if (mtContent_2x2[n]==0){
						memcpy(metatileSet_2x2_id+(n*d*d),		tmp_meta_id,d*d);
						memcpy(metatileSet_2x2_pal+(n*d*d),		tmp_meta_pal,d*d);
						memcpy(metatileSet_2x2_props+(n*d*d),	tmp_meta_props,d*d);

						//Update mtContent so we know this place is occupied.
						mtContent_2x2[n]= memcmp(tmp_meta_nullcmp ,tmp_meta_id ,d*d);
						//if mt has no contentful tile data, evaluate based on palettes instead.
						if(mtContent_2x2[n]==0)
						{
							mtContent_2x2[n]= memcmp(tmp_meta_palcmp ,metatileSet_2x2_pal+(i*d*d) ,d*d);
							if(mtContent_2x2[n]==0)
							{
								mtContent_2x2[n]= memcmp(tmp_meta_propscmp ,metatileSet_2x2_props+(i*d*d) ,d*d);
							}
						}
						goto Next;
					}
				}
			}

				if(whichType==4)
				{
					for (int m = reserveOff; m < (64*metatileSets_4x4); m++) {
						match = CompareTempToSet(tmp_wildcard,tmp_meta_id,metatileSet_4x4_id,d*d,m*d*d);
						if(match==true)
						{
							//does the palette data match too?
							match = CompareTempToSet(tmp_wildcard,tmp_meta_pal,metatileSet_4x4_pal,d*d,m*d*d);
							if((match==true) || (doPal==false))
							{
								match = CompareTempToSet(tmp_wildcard,tmp_meta_props,metatileSet_4x4_props,d*d,m*d*d);
								if(match==true) goto Next; //already in set, don't place
							}
						}

					}

					//no match, put in empty & unused slot
					for ( int n = reserveOff; n < (64*metatileSets_4x4); n++) {
						if (mtContent_4x4[n]==0){
							memcpy(metatileSet_4x4_id+(n*d*d),  tmp_meta_id	,d*d);
							memcpy(metatileSet_4x4_pal+(n*d*d),	tmp_meta_pal,d*d);
							memcpy(metatileSet_4x4_props+(n*d*d),	tmp_meta_props,d*d);

							mtContent_4x4[n]= memcmp(tmp_meta_nullcmp ,tmp_meta_id ,d*d);
							if(mtContent_4x4[n]==0)
							{
								mtContent_4x4[n]= memcmp(tmp_meta_palcmp ,metatileSet_4x4_pal+(i*d*d) ,d*d);
								if(mtContent_4x4[n]==0)
								{
									mtContent_4x4[n]= memcmp(tmp_meta_propscmp ,metatileSet_4x4_props+(i*d*d) ,d*d);
								}
							}
							goto Next;
						}
					}
				}

				if(whichType==8)
				{
					for (int m = reserveOff; m < metatileSets_8x8*64; m++) {
						match = CompareTempToSet(tmp_wildcard,tmp_meta_id,metatileSet_8x8_id,d*d,m*d*d);
						if(match==true)
						{
							//does the palette data match too?
							match = CompareTempToSet(tmp_wildcard,tmp_meta_pal,metatileSet_8x8_pal,d*d,m*d*d);
							if((match==true) || (doPal==false))
							{
								match = CompareTempToSet(tmp_wildcard,tmp_meta_props,metatileSet_8x8_props,d*d,m*d*d);
								if(match==true) goto Next; //already in set, don't place
							}
						}

				}
				//no match, put in empty & unused slot
					for ( int n = reserveOff; n < (64*metatileSets_8x8); n++) {
						if (mtContent_8x8[n]==0){
							memcpy(metatileSet_8x8_id+(n*d*d),tmp_meta_id,d*d);
							memcpy(metatileSet_8x8_pal+(n*d*d),	tmp_meta_pal,d*d);
							memcpy(metatileSet_8x8_props+(n*d*d),	tmp_meta_props,d*d);


							mtContent_8x8[n]= memcmp(tmp_meta_nullcmp ,tmp_meta_id ,d*d);
							if(mtContent_8x8[n]==0)
							{
								mtContent_8x8[n]= memcmp(tmp_meta_palcmp ,metatileSet_8x8_pal+(i*d*d) ,d*d);
								if(mtContent_8x8[n]==0)
								{
									mtContent_8x8[n]= memcmp(tmp_meta_propscmp ,metatileSet_8x8_props+(i*d*d) ,d*d);
								}
							}
							goto Next;
						}
					}
				}
			Next:
			} //width enclosure
			if(clearTmp)i-=2; //realign y-axis sampling cursor after half-metas have been processed.
	}         //height enclosure
	}
	free(tmp_meta_id);
	free(tmp_meta_nullcmp);

	free(tmp_meta_pal);
	free(tmp_meta_palcmp);

	free(tmp_meta_props);
	free(tmp_meta_propscmp);
	free(tmp_wildcard);
}

void __fastcall TFormMetatileTool::MakeList(bool bSelectTop, bool bInit)
{
	if(bInit){
		ListBox2x2->Clear();
		ListBox4x4->Clear();
		ListBox8x8->Clear();

	}

	AnsiString tmp;

	for(int i=0;i<metatileSets_2x2;i++)
	{
		tmp = metatileSetLabels_2x2.at(i).c_str();

		strListMT=IntToHex(i,3)+": \t"+tmp+"\t @ $"+IntToHex(i*META_2x2,6);

		if(bInit)		ListBox2x2->Items->Add(strListMT);
		else 			ListBox2x2->Items->Strings[i] = strListMT;
	}

	for(int i=0;i<metatileSets_4x4;i++)
	{

		tmp = metatileSetLabels_4x4.at(i).c_str();
		strListMT=IntToHex(i,3)+": \t"+tmp+"\t @ $"+IntToHex(i*META_4x4,6);

		if(bInit)		ListBox4x4->Items->Add(strListMT);
		else 			ListBox4x4->Items->Strings[i] = strListMT;
	}

	for(int i=0;i<metatileSets_8x8;i++)
	{

		tmp = metatileSetLabels_8x8.at(i).c_str();
		strListMT=IntToHex(i,3)+": \t"+tmp+"\t @ $"+IntToHex(i*META_8x8,6);

		if(bInit)		ListBox8x8->Items->Add(strListMT);
		else 			ListBox8x8->Items->Strings[i] = strListMT;
	}

	if(bSelectTop){
		ListBox2x2->Selected[0]=true;
		ListBox4x4->Selected[0]=true;
		ListBox8x8->Selected[0]=true;
}
}

void __fastcall TFormMetatileTool::Draw(void)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	int d,m,n; //dimensions, magnification1, magnification2.
	int metasOnRow=sqrt(iSetView);
	unsigned int tmpi_highlight,tile_highlight;


	if(bTab2x2){
		if (metatileSets_2x2<=1) Remove1->Enabled=false;
		else             Remove1->Enabled=true;
		d=2;
		m=4;
		n=2;
		Image1     ->Picture->Bitmap->SetSize(16*m,16*m);
		Image2     ->Picture->Bitmap->SetSize(16*n*metasOnRow,16*n*metasOnRow);
		currentMetaSelection = &metaSelection_2x2;
		currentMetaSelected = metaSelected_2x2;
	}
	if(bTab4x4){
		if (metatileSets_4x4<=1) Remove1->Enabled=false;
		else             Remove1->Enabled=true;
		d=4;
		m=2;
		n=1;
		Image1     ->Picture->Bitmap->SetSize(32*m,32*m);
		Image2     ->Picture->Bitmap->SetSize(32*n*metasOnRow,32*n*metasOnRow);
		currentMetaSelection = &metaSelection_4x4;
		currentMetaSelected = metaSelected_4x4;
	}
	if(bTab8x8){
		if (metatileSets_8x8<=1) Remove1->Enabled=false;
		else             Remove1->Enabled=true;
		d=8;
		m=1;
		n=1;
		Image1     ->Picture->Bitmap->SetSize(64*m,64*m);
		Image2     ->Picture->Bitmap->SetSize(64*n*metasOnRow,64*n*metasOnRow);
		currentMetaSelection = &metaSelection_8x8;
		currentMetaSelected = metaSelected_4x4;
	}

	int x=0;
	int y=0;
	unsigned char tile;
	unsigned int pal;
	int cgX=CHRCollisionGranularityX;
	int cgY=CHRCollisionGranularityY;
	unsigned int mt_cgX=MetaCollisionGranularityX;
	unsigned int mt_cgY=MetaCollisionGranularityY;

	//int highlight_tile;

	unsigned int tmpi, tmpi_pal; //temporary iteration cursor
	//set canvas; tiles

	bMMC2Switch=mmc2_Doublepair_1stSet? false:true;
	current_switchTileTarget=bankActive;

	for(int i=0;i<metasOnRow*metasOnRow;i++) //metas
	{
		if(mmc2_doublepair) if(!mmc2_startSet_asPrevious_mt) bMMC2Switch=mmc2_Doublepair_1stSet? false:true;
		if(mmc2_startSet_use1st_mt) current_switchTileTarget=mmc2_checked1;
		else if(mmc2_startSet_useActive_mt) current_switchTileTarget=bankActive;
		else if(mmc2_startSet_useFixedSet_mt) current_switchTileTarget=mmc2_startSet_fixed_mt;


		//DrawTile(TPicture *pic, int x, int y, int tile, int highlight_tile, int pal, int tx, int ty, bool sel, bool efficientTarget, int inputScale, bool bIsNav, bool isMeta, bool doubleWidth, bool banked)
		for(int k=0;k<d;k++) //y tiles ín meta
		{
			for(int j=0;j<d;j++) //x tiles ín meta
			{
				tmpi=k*d+j+(iSetViewOff+iListViewOff+i)*d*d;

				if(bTab2x2) tile= (metatileSet_2x2_id[tmpi]);
				if(bTab4x4) tile= (metatileSet_4x4_id[tmpi]);
				if(bTab8x8) tile= (metatileSet_8x8_id[tmpi]);


				//this is for metatile collision reference.
				tmpi_highlight=((k*d)/(d*mt_cgY))*(d*mt_cgY) + (j/mt_cgX)*mt_cgX + (iSetViewOff+iListViewOff+i)*d*d;
				if(bTab2x2) highlight_mt= (metatileSet_2x2_props[tmpi_highlight]);
				if(bTab4x4) highlight_mt= (metatileSet_4x4_props[tmpi_highlight]);
				if(bTab8x8) highlight_mt= (metatileSet_8x8_props[tmpi_highlight]);


				//these are for direct tile collision references.
				tmpi_highlight=((k*d)/(d*cgY))*(d*cgY) + (j/cgX)*cgX + (iSetViewOff+iListViewOff+i)*d*d;
				if(bTab2x2) tile_highlight= (metatileSet_2x2_id[tmpi_highlight]);
				if(bTab4x4) tile_highlight= (metatileSet_4x4_id[tmpi_highlight]);
				if(bTab8x8) tile_highlight= (metatileSet_8x8_id[tmpi_highlight]);



				if(btnUseAttr->Down==false) pal=palActive; //change this for paletted metas
				else{
					//Extract 16x16px attributes out of 8x8px attr mode.
					tmpi_pal=((k*d)/(d*2))*(d*2) + (j/2)*2 + (iSetViewOff+iListViewOff+i)*d*d;

					if(bTab2x2) pal= (metatileSet_2x2_pal[(tmpi_pal )]);    //& 0xfffffffc
					if(bTab4x4) pal= (metatileSet_4x4_pal[(tmpi_pal )]);
					if(bTab8x8) pal= (metatileSet_8x8_pal[(tmpi_pal )]);
				}
				int xOff = x + j*8;
				int yOff = y + k*8;
				//DrawTile(TPicture *pic,int x,int y,unsigned int tile,int pal,int tx,int ty,bool sel, bool efficientTarget, int inputScale, bool bIsNav, bool doubleWidth, bool banked)
				bool dodoubleline=false;
				int tdiv=8;
				if(d==2){tdiv=4;}
				if(d==4){tdiv=8;}
				if(d==8){tdiv=8; dodoubleline=true;}

				//when appropriate, tile_highlight isn't used - instead a conditional inDrawTile uses highlight_mt
				FormMain->DrawTile(Image2->Picture,xOff*n,yOff*n,tile,tile_highlight,pal,xOff/tdiv,yOff/tdiv,chrSelected[tile],false,n,false,true,dodoubleline,false,false);



			}

		}
		x+=8*d;
		if(x>=8*d*metasOnRow){	x=0; y+=8*d; }
	}


	//meta canvas
	x=0;
	y=0;
	tmpi=mtClickID*d*d;


	bMMC2Switch=mmc2_Doublepair_1stSet? false:true;
	current_switchTileTarget=bankActive;
	//if(mmc2_doublepair) if(!mmc2_startSet_asPrevious_mt) bMMC2Switch=mmc2_Doublepair_1stSet? false:true;
    if(mmc2_startSet_use1st_mt) current_switchTileTarget=mmc2_checked1;
	else if(mmc2_startSet_useActive_mt) current_switchTileTarget=bankActive;
    else if(mmc2_startSet_useFixedSet_mt) current_switchTileTarget=mmc2_startSet_fixed_mt;

	//DrawTile(TPicture *pic, int x, int y, int tile, int pal, int tx, int ty, bool sel, bool efficientTarget, int inputScale, bool bIsNav, bool doubleWidth, bool banked)
	for(int j=0;j<d;j++) //tiles ín meta
	{
		for(int k=0;k<d;k++) //tiles ín meta
		{

			if(bTab2x2) tile= (metatileSet_2x2_id[tmpi]);
			if(bTab4x4) tile= (metatileSet_4x4_id[tmpi]);
			if(bTab8x8) tile= (metatileSet_8x8_id[tmpi]);

			//this is for metatile collision reference.
			tmpi_highlight=((j*d)/(d*mt_cgY))*(d*mt_cgY) + (k/mt_cgX)*mt_cgX + (mtClickID)*d*d;
			if(bTab2x2) highlight_mt= (metatileSet_2x2_props[tmpi_highlight]);
			if(bTab4x4) highlight_mt= (metatileSet_4x4_props[tmpi_highlight]);
			if(bTab8x8) highlight_mt= (metatileSet_8x8_props[tmpi_highlight]);


			//these are for direct tile collision references.
			tmpi_highlight=((j*d)/(d*cgY))*(d*cgY) + (k/cgX)*cgX + (mtClickID)*d*d;
			if(bTab2x2) tile_highlight= (metatileSet_2x2_id[tmpi_highlight]);
			if(bTab4x4) tile_highlight= (metatileSet_4x4_id[tmpi_highlight]);
			if(bTab8x8) tile_highlight= (metatileSet_8x8_id[tmpi_highlight]);

			if(btnUseAttr->Down==false) pal=palActive; //change this for paletted metas
			else{
				//Extract 16x16px attributes out of 8x8px attr mode.
				tmpi_pal=((j*d)/(d*2))*(d*2) + (k/2)*2 + (mtClickID)*d*d;
				if(bTab2x2) pal= (metatileSet_2x2_pal[(tmpi_pal)]);
				if(bTab4x4) pal= (metatileSet_4x4_pal[(tmpi_pal)]);
				if(bTab8x8) pal= (metatileSet_8x8_pal[(tmpi_pal)]);
			}
			int xOff = k*8;
			int yOff = j*8;

			FormMain->DrawTile(Image1->Picture,xOff*m,yOff*m,tile,tile_highlight,pal,0,0,chrSelected[tile],				false,m,false,true,			  false,false,false);
			//FormMain->DrawTile(Image2->Picture,xOff*n,yOff*n,tile,tile_highlight,pal,xOff/tdiv,yOff/tdiv,chrSelected[tile],false,n,false,true,dodoubleline,false,false);


			tmpi++;
		}
	}
	
	//handle selection
	isSetCursor=false;
	//FormMain->DrawSelection(Image1,curSelection,1,true,false);

	//reminder: mtClickID = idY*metasOnRow + idX + iSetViewOff + iListViewOff;
	int tmpLo = iSetViewOff + iListViewOff;
	int tmpHi = iSetViewOff + iListViewOff + iSetView;
	if((mtClickID >= tmpLo) || (mtClickID < tmpHi)){
		FormMain->DrawSelection(Image2,setSelection,n,true,false);
	}


	if(metaSelectMulti)

	{
		TRect rect;
		for(int i=0;i<d*d;i++)
		{
			if(currentMetaSelected[i])
			{
				rect.Left=i&(d-1);
				rect.Right=rect.Left+1;
				rect.Top=i/d;
				rect.Bottom=rect.Top+1;

				FormMain->DrawSelection(Image1,rect,m,false,false);
			}
		}
	}
	else if(currentMetaSelection->left>=0 && currentMetaSelection->top>=0)
	{

		FormMain->DrawSelection(Image1,*currentMetaSelection,m,false,false);  //selection only meaningful on the big canvas.
	}
	Image1->Refresh();
	Image2->Refresh();


	if(!mtClickSent)
	{
		isSetCursor=true;
		if(metaHover)	FormMain->DrawSelection(Image1,metaCursor,m,true,false);
		if(setHover) FormMain->DrawSelection(Image2,setCursor,n,false,false);
		isMetaCursor=false;
	}
}

void __fastcall TFormMetatileTool::UpdateUI(bool cue)
{
	if(cue)
	{
		DrawTimer->Enabled=true;
        ListTimer->Enabled=true;
	}
	else{
		MakeList(false,false);
		Draw();
	}
}

void __fastcall TFormMetatileTool::FormCreate(TObject *Sender)
{
	this->Left=FormMain->Left - this->Width;
	this->Top=(Screen->Height-FormMain->Height)/4;


	Image1->Picture=new TPicture();
	Image1->Picture->Bitmap=new Graphics::TBitmap();
	Image1->Picture->Bitmap->PixelFormat=pf24bit;
	Image1->Stretch=true;

	Image2->Picture=new TPicture();
	Image2->Picture->Bitmap=new Graphics::TBitmap();
	Image2->Picture->Bitmap->PixelFormat=pf24bit;
	Image2->Stretch=true;

	mtClickID=0;
	mtClickID_store_2x2=0;
	mtClickID_store_4x4=0;
	mtClickID_store_8x8=0;

    metaSelection_2x2.left= -1;
	metaSelection_2x2.top= -1;
	metaSelection_4x4.left= -1;
	metaSelection_4x4.top= -1;
	metaSelection_8x8.left= -1;
	metaSelection_8x8.top= -1;

	if(openByFileDone) MakeList(true,true);
	else OpenByFileAssociationMakeListTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image2MouseEnter(TObject *Sender)
{
	setHover=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image2MouseLeave(TObject *Sender)
{
	setHover=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image1MouseEnter(TObject *Sender)
{
	metaHover=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image1MouseLeave(TObject *Sender)
{
	LabelPos->Caption="position";
	LabelTilePal->Caption="tile, pal";
	LabelProps->Caption="props";


	metaHover=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Insert1Click(TObject *Sender)
{
	int i,id, total;
	bool bDuplicate = ((TMenuItem*)Sender)->Tag==1?true:false;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;


	if(bTab2x2){
		id=ListBox2x2->ItemIndex;
		total=ListBox2x2->Items->Count-1;
	}
	else if(bTab4x4){
		id=ListBox4x4->ItemIndex;
		total=ListBox4x4->Items->Count-1;
	}
	else if(bTab8x8){
		id=ListBox8x8->ItemIndex;
		total=ListBox8x8->Items->Count-1;
	}
	else return;

	FormMain->SetUndo();

	//size_t oldSize, oldSize64;
	size_t newSize, newSize64;
	//expand Metatileset memory
	if(bTab2x2){
		//oldSize=metatileSets_2x2*META_2x2;
		//oldSize64=metatileSets_2x2*64;

		metatileSets_2x2++;

		newSize=metatileSets_2x2*META_2x2;
		newSize64=metatileSets_2x2*64;


		metatileSet_2x2_id  = (char*)realloc(metatileSet_2x2_id,newSize);
		metatileSet_2x2_pal = (char*)realloc(metatileSet_2x2_pal,newSize);
		metatileSet_2x2_props = (char*)realloc(metatileSet_2x2_props,newSize);

		mtContent_2x2 	    = (char*)realloc(mtContent_2x2,newSize64);
		//mtUsage_2x2 	    = (uint32_t*)realloc(mtUsage_2x2,newSize64*sizeof(uint32_t));
		/*
		memset(metatileSet_2x2_id +oldSize,nullTile,(newSize-oldSize));
		memset(metatileSet_2x2_pal +oldSize,0,(newSize-oldSize));
		memset(metatileSet_2x2_props +oldSize,0,(newSize-oldSize));

		memset(mtContent_2x2 +(oldSize64),0,(newSize64-oldSize64));
		*/
		//memset(mtUsage_2x2 +(oldSize64*sizeof(uint32_t)),0,(newSize64-oldSize64)*sizeof(uint32_t));

	}
	if(bTab4x4){
		//oldSize=metatileSets_4x4*META_4x4;
		//oldSize64=metatileSets_4x4*64;

		metatileSets_4x4++;

		newSize=metatileSets_4x4*META_4x4;
		newSize64=metatileSets_4x4*64;


		metatileSet_4x4_id = (char*)realloc(metatileSet_4x4_id,newSize);
		metatileSet_4x4_pal = (char*)realloc(metatileSet_4x4_pal,newSize);
		metatileSet_4x4_props = (char*)realloc(metatileSet_4x4_props,newSize);

		mtContent_4x4 	   = (char*)realloc(mtContent_4x4,newSize64);
		//mtUsage_4x4 	   = (uint32_t*)realloc(mtUsage_4x4,newSize64*sizeof(uint32_t));
		/*
		memset(metatileSet_4x4_id +oldSize,nullTile,(newSize-oldSize));
		memset(metatileSet_4x4_pal +oldSize,0,(newSize-oldSize));
		memset(metatileSet_4x4_props +oldSize,0,(newSize-oldSize));

		memset(mtContent_4x4 +(oldSize64),0,(newSize64-oldSize64));
		*/
		//memset(mtUsage_4x4 +(oldSize64*sizeof(uint32_t)),0,(newSize64-oldSize64)*sizeof(uint32_t));

	}
	if(bTab8x8){
		//oldSize=metatileSets_8x8*META_8x8;
		//oldSize64=metatileSets_8x8*64;

		metatileSets_8x8++;

		newSize=metatileSets_8x8*META_8x8;
		newSize64=metatileSets_8x8*64;


		metatileSet_8x8_id = (char*)realloc(metatileSet_8x8_id,newSize);
		metatileSet_8x8_pal = (char*)realloc(metatileSet_8x8_pal,newSize);
		metatileSet_8x8_props = (char*)realloc(metatileSet_8x8_props,newSize);

		mtContent_8x8 	   = (char*)realloc(mtContent_8x8,newSize64);
		//mtUsage_8x8 	   = (uint32_t*)realloc(mtUsage_8x8,newSize64*sizeof(uint32_t));
		/*
		memset(metatileSet_8x8_id +oldSize,nullTile,(newSize-oldSize));
		memset(metatileSet_8x8_pal +oldSize,0,(newSize-oldSize));
		memset(metatileSet_8x8_props +oldSize,0,(newSize-oldSize));

		memset(mtContent_8x8 +(oldSize64),0,(newSize64-oldSize64));
		*/
		//memset(mtUsage_8x8 +(oldSize64*sizeof(uint32_t)),0,(newSize64-oldSize64)*sizeof(uint32_t));
	}

	//push sets
	if(bTab2x2){
		metatileSetLabels_2x2.insert(metatileSetLabels_2x2.begin() + id + 1, metatileSetLabels_2x2[id]);
		for(i=total;i>id;--i)
		{
			memcpy(&metatileSet_2x2_id[i*META_2x2],&metatileSet_2x2_id[(i-1)*META_2x2],META_2x2);
			memcpy(&metatileSet_2x2_pal[i*META_2x2],&metatileSet_2x2_pal[(i-1)*META_2x2],META_2x2);
			memcpy(&metatileSet_2x2_props[i*META_2x2],&metatileSet_2x2_props[(i-1)*META_2x2],META_2x2);

		}
	}
	if(bTab4x4){
		metatileSetLabels_4x4.insert(metatileSetLabels_4x4.begin() + id + 1, metatileSetLabels_4x4[id]);
		for(i=total;i>id;--i)
		{
			memcpy(&metatileSet_4x4_id[i*META_4x4], &metatileSet_4x4_id[(i-1)*META_4x4],META_4x4);
			memcpy(&metatileSet_4x4_pal[i*META_4x4], &metatileSet_4x4_pal[(i-1)*META_4x4],META_4x4);
			memcpy(&metatileSet_4x4_props[i*META_4x4], &metatileSet_4x4_props[(i-1)*META_4x4],META_4x4);

		}
	}
	if(bTab8x8){
		metatileSetLabels_8x8.insert(metatileSetLabels_8x8.begin() + id + 1, metatileSetLabels_8x8[id]);
		for(i=total;i>id;--i)
		{
			memcpy(&metatileSet_8x8_id[i*META_8x8],&metatileSet_8x8_id[(i-1)*META_8x8],META_8x8);
			memcpy(&metatileSet_8x8_pal[i*META_8x8],&metatileSet_8x8_pal[(i-1)*META_8x8],META_8x8);
			memcpy(&metatileSet_8x8_props[i*META_8x8],&metatileSet_8x8_props[(i-1)*META_8x8],META_8x8);

		}
	}



	//insertion mode
	if(!bDuplicate)
	{
		if(bTab2x2){
			memset(&metatileSet_2x2_id[id*META_2x2],0,META_2x2);
			memset(&metatileSet_2x2_pal[id*META_2x2],0,META_2x2);
			memset(&metatileSet_2x2_props[id*META_2x2],0,META_2x2);
			metatileSetLabels_2x2[id]="Unlabeled";
		}
		if(bTab4x4){
			memset(&metatileSet_4x4_id[id*META_4x4],0,META_4x4);
			memset(&metatileSet_4x4_pal[id*META_4x4],0,META_4x4);
			memset(&metatileSet_4x4_props[id*META_4x4],0,META_4x4);

			metatileSetLabels_4x4[id]="Unlabeled";
		}
		if(bTab8x8){
			memset(&metatileSet_8x8_id[id*META_8x8],0,META_8x8);
			memset(&metatileSet_8x8_pal[id*META_8x8],0,META_8x8);
			memset(&metatileSet_8x8_props[id*META_8x8],0,META_8x8);

			metatileSetLabels_8x8[id]="Unlabeled";
		}
	}

	//MakeList(false,false);
	UpdateUI(true);
	if(bTab2x2) if (metatileSets_2x2>1) Remove1->Enabled=true;
	if(bTab4x4) if (metatileSets_4x4>1) Remove1->Enabled=true;
	if(bTab8x8) if (metatileSets_8x8>1) Remove1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Rename1Click(TObject *Sender)
{
	int id;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;

	if(bTab2x2){
		id=ListBox2x2->ItemIndex;
		FormName->Caption="Rename Metatile set label";
		FormName->EditName->Text=metatileSetLabels_2x2.at(id).c_str();
		FormName->ShowModal();
		if(bKeyEscape) return;
		metatileSetLabels_2x2.at(id)=FormName->EditName->Text.c_str();
	}
	if(bTab4x4){
		id=ListBox4x4->ItemIndex;
		FormName->Caption="Rename Metatile set label";
		FormName->EditName->Text=metatileSetLabels_4x4.at(id).c_str();
		FormName->ShowModal();
		if(bKeyEscape) return;
		metatileSetLabels_4x4.at(id)=FormName->EditName->Text.c_str();
	}
	if(bTab8x8){
		id=ListBox8x8->ItemIndex;
		FormName->Caption="Rename Metatile set label";
		FormName->EditName->Text=metatileSetLabels_8x8.at(id).c_str();
		FormName->ShowModal();
		if(bKeyEscape) return;
		metatileSetLabels_8x8.at(id)=FormName->EditName->Text.c_str();
	}
	MakeList(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Clear1Click(TObject *Sender)
{
	int id;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;

	FormMain->SetUndo();

    //todo: set attributes, props, bank too.
	if(bTab2x2){
		id=ListBox2x2->ItemIndex;
		memset(&metatileSet_2x2_id[id*META_2x2],0,META_2x2);
		memset(&metatileSet_2x2_pal[id*META_2x2],0,META_2x2);
		memset(&metatileSet_2x2_props[id*META_2x2],0,META_2x2);

	}
	if(bTab4x4){
		id=ListBox4x4->ItemIndex;
		memset(&metatileSet_4x4_id[id*META_4x4],0,META_4x4);
		memset(&metatileSet_4x4_pal[id*META_4x4],0,META_4x4);
		memset(&metatileSet_4x4_props[id*META_4x4],0,META_4x4);

	}
	if(bTab8x8){
		id=ListBox8x8->ItemIndex;
		memset(&metatileSet_8x8_id[id*META_8x8],0,META_8x8);
		memset(&metatileSet_8x8_pal[id*META_8x8],0,META_8x8);
		memset(&metatileSet_8x8_props[id*META_8x8],0,META_8x8);

	}
	UpdateUI(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Remove1Click(TObject *Sender)
{
	int id,total;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;


	if(bTab2x2){if (metatileSets_2x2<=1) return;}
	if(bTab4x4){if (metatileSets_4x4<=1) return;}
	if(bTab8x8){if (metatileSets_8x8<=1) return;}

	FormMain->SetUndo();

	if(bTab2x2){
		id=ListBox2x2->ItemIndex;
		total=ListBox2x2->Items->Count-1;
	}
	if(bTab4x4){
		id=ListBox4x4->ItemIndex;
		total=ListBox4x4->Items->Count-1;
	}
	if(bTab8x8){
		id=ListBox8x8->ItemIndex;
		total=ListBox8x8->Items->Count-1;
	}


	//push working sets
	/*
	for(int i=0;i<bankwin;i++)
	{
		if (id<=chrA_id[i]) chrA_id[i]-=bankwin;
		if (id<=chrB_id[i]) chrB_id[i]-=bankwin;
	}
    */

	//push banks
	for(int i=id;i<total;++i)
	{
		if(bTab2x2) {memcpy(&metatileSet_2x2_id[i*META_2x2]
							,&metatileSet_2x2_id[(i+1)*META_2x2]
							,META_2x2);}
		if(bTab4x4) {memcpy(&metatileSet_4x4_id[i*META_4x4]
							,&metatileSet_4x4_id[(i+1)*META_4x4]
							,META_4x4);}
		if(bTab8x8) {memcpy(&metatileSet_8x8_id[i*META_8x8]
							,&metatileSet_8x8_id[(i+1)*META_8x8]
							,META_8x8);}
	}

	if(bTab2x2) {
		metatileSetLabels_2x2.erase(metatileSetLabels_2x2.begin() + id);
		ListBox2x2->Items->Delete(id);
		ListBox2x2->ItemIndex = min(id,total-1);
		metatileSets_2x2--;
	}
	if(bTab4x4) {
		metatileSetLabels_4x4.erase(metatileSetLabels_4x4.begin() + id);
		ListBox4x4->Items->Delete(id);
		ListBox4x4->ItemIndex = min(id,total-1);
		metatileSets_4x4--;
	}
	if(bTab8x8) {
		metatileSetLabels_8x8.erase(metatileSetLabels_8x8.begin() + id);
		ListBox8x8->Items->Delete(id);
		ListBox8x8->ItemIndex = min(id,total-1);
		metatileSets_8x8--;
	}

	//MakeList(false,false);
	UpdateUI(true);
	if(bTab2x2) if (metatileSets_2x2<=1) Remove1->Enabled=false;
	if(bTab4x4) if (metatileSets_4x4<=1) Remove1->Enabled=false;
	if(bTab8x8) if (metatileSets_8x8<=1) Remove1->Enabled=false;

}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Down1Click(TObject *Sender)
{
	int id,total;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	if(bTab2x2){
		id=ListBox2x2->ItemIndex;
		total=ListBox2x2->Items->Count-1;
		if(id>=total) return;
		unsigned char tempChar_2x2[META_2x2];
		FormMain->SetUndo();
		//data
		memcpy(&tempChar_2x2						,&metatileSet_2x2_id[(id+1)*META_2x2],META_2x2);
		memcpy(&metatileSet_2x2_id[(id+1)*META_2x2]	,&metatileSet_2x2_id[id*META_2x2]	 ,META_2x2);
		memcpy(&metatileSet_2x2_id[id*META_2x2]		,&tempChar_2x2			  			 ,META_2x2);
		//list
		std::iter_swap(metatileSetLabels_2x2.begin()+id,metatileSetLabels_2x2.begin()+id+1);
		ListBox2x2->ItemIndex++;
	}
	if(bTab4x4){
		id=ListBox4x4->ItemIndex;
		total=ListBox4x4->Items->Count-1;
		if(id>=total) return;
		unsigned char tempChar_4x4[META_4x4];
		FormMain->SetUndo();
		//data
		memcpy(&tempChar_4x4						,&metatileSet_4x4_id[(id+1)*META_4x4],META_4x4);
		memcpy(&metatileSet_4x4_id[(id+1)*META_4x4]	,&metatileSet_4x4_id[id*META_4x4]	 ,META_4x4);
		memcpy(&metatileSet_4x4_id[id*META_4x4]		,&tempChar_4x4			  			 ,META_4x4);
		//list
		std::iter_swap(metatileSetLabels_4x4.begin()+id,metatileSetLabels_4x4.begin()+id+1);
		ListBox4x4->ItemIndex++;
	}
	if(bTab8x8){
		id=ListBox8x8->ItemIndex;
		total=ListBox8x8->Items->Count-1;
		if(id>=total) return;
		unsigned char tempChar_8x8[META_8x8];
		FormMain->SetUndo();
		//data
		memcpy(&tempChar_8x8						,&metatileSet_8x8_id[(id+1)*META_8x8],META_8x8);
		memcpy(&metatileSet_8x8_id[(id+1)*META_8x8]	,&metatileSet_8x8_id[id*META_8x8]	 ,META_8x8);
		memcpy(&metatileSet_8x8_id[id*META_8x8]		,&tempChar_8x8			  			 ,META_8x8);
		//list
		std::iter_swap(metatileSetLabels_8x8.begin()+id,metatileSetLabels_8x8.begin()+id+1);
		ListBox8x8->ItemIndex++;
	}

	UpdateUI(true);
	//MakeList(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Up1Click(TObject *Sender)
{
	int id;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	if(bTab2x2){
		id=ListBox2x2->ItemIndex;
		if(id<1) return;
		unsigned char tempChar_2x2[META_2x2];
		FormMain->SetUndo();
		//data
		memcpy(&tempChar_2x2						,&metatileSet_2x2_id[(id-1)*META_2x2],META_2x2);
		memcpy(&metatileSet_2x2_id[(id-1)*META_2x2]	,&metatileSet_2x2_id[id*META_2x2]	 ,META_2x2);
		memcpy(&metatileSet_2x2_id[id*META_2x2]		,&tempChar_2x2			  			 ,META_2x2);
		//list
		std::iter_swap(metatileSetLabels_2x2.begin()+id,metatileSetLabels_2x2.begin()+id-1);
		ListBox2x2->ItemIndex--;
	}
	if(bTab4x4){
		id=ListBox4x4->ItemIndex;
		if(id<1) return;
		unsigned char tempChar_4x4[META_4x4];
		FormMain->SetUndo();
		//data
		memcpy(&tempChar_4x4						,&metatileSet_4x4_id[(id-1)*META_4x4],META_4x4);
		memcpy(&metatileSet_4x4_id[(id-1)*META_4x4]	,&metatileSet_4x4_id[id*META_4x4]	 ,META_4x4);
		memcpy(&metatileSet_4x4_id[id*META_4x4]		,&tempChar_4x4			  			 ,META_4x4);
		//list
		std::iter_swap(metatileSetLabels_4x4.begin()+id,metatileSetLabels_4x4.begin()+id-1);
		ListBox4x4->ItemIndex--;
	}
	if(bTab8x8){
		id=ListBox8x8->ItemIndex;
		if(id<1) return;
		unsigned char tempChar_8x8[META_8x8];
		FormMain->SetUndo();
		//data
		memcpy(&tempChar_8x8						,&metatileSet_8x8_id[(id-1)*META_8x8],META_8x8);
		memcpy(&metatileSet_8x8_id[(id-1)*META_8x8]	,&metatileSet_8x8_id[id*META_8x8]	 ,META_8x8);
		memcpy(&metatileSet_8x8_id[id*META_8x8]		,&tempChar_8x8			  			 ,META_8x8);
		//list
		std::iter_swap(metatileSetLabels_8x8.begin()+id,metatileSetLabels_8x8.begin()+id-1);
		ListBox8x8->ItemIndex--;
	}

	UpdateUI(true);
	//MakeList(false,false);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::FormShow(TObject *Sender)
{
	DrawTimer->Enabled=true;
	FormMain->UpdateLabelApplyTiles();
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::PageControl1Change(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;

	if(bTab2x2){mtClickID = mtClickID_store_2x2;}
	if(bTab4x4){mtClickID = mtClickID_store_4x4;}
	if(bTab8x8){mtClickID = mtClickID_store_8x8;}

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::DrawTimerTimer(TObject *Sender)
{
	if(!openByFileDone) return;
	Draw();
	DrawTimer->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetView64Click(TObject *Sender)
{
   static int iSetViewOffPrev=0;
   static int iSetViewPrev=64;

   bool togglePrev=false;
	if(SBSetView64->Down && iSetView==64 && iSetViewOff==0) {
		if(iSetViewPrev==iSetView && iSetViewOffPrev==iSetViewOff) {SBSetViewA->Down=true;}
		else togglePrev=true;
	}
	if(SBSetViewA->Down && iSetView==16 && iSetViewOff==0) {
		if(iSetViewPrev==iSetView && iSetViewOffPrev==iSetViewOff) {SBSetViewB->Down=true;}
		else togglePrev=true;
	}
	if(SBSetViewB->Down && iSetView==16 && iSetViewOff==16) {
		if(iSetViewPrev==iSetView && iSetViewOffPrev==iSetViewOff) {SBSetViewC->Down=true;}
		else togglePrev=true;
	}
	if(SBSetViewC->Down && iSetView==16 && iSetViewOff==32) {
		if(iSetViewPrev==iSetView && iSetViewOffPrev==iSetViewOff) {SBSetViewD->Down=true;}
		else togglePrev=true;
	}
	if(SBSetViewD->Down && iSetView==16 && iSetViewOff==48) {
		if(iSetViewPrev==iSetView && iSetViewOffPrev==iSetViewOff) {SBSetView64->Down=true;}
		else togglePrev=true;
	}


	if(togglePrev){
		switch (iSetViewOffPrev) {

		case 0:
			if(iSetViewPrev==64) {SBSetView64->Down=true; break;}
			else                 {SBSetViewA->Down=true; break;}
		case 16:  SBSetViewB->Down=true; break;
		case 32:  SBSetViewC->Down=true; break;
		case 48:  SBSetViewD->Down=true; break;
		default:
			SBSetView64->Down=true;
		}

	}

	iSetViewOffPrev=iSetViewOff;
	iSetViewPrev=iSetView;

	if(SBSetView64->Down) {iSetView=64; iSetViewOff=0;}
	if(SBSetViewA->Down)  {iSetView=16; iSetViewOff=0;}
	if(SBSetViewB->Down)  {iSetView=16; iSetViewOff=16;}
	if(SBSetViewC->Down)  {iSetView=16; iSetViewOff=32;}
	if(SBSetViewD->Down)  {iSetView=16; iSetViewOff=48;}

   //iSetView=64;
   //iSetViewOff=0;
   DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetViewAClick(TObject *Sender)
{

   iSetView=16;
	iSetViewOff=0;

   DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetViewBClick(TObject *Sender)
{

	iSetView=16;
	iSetViewOff=16;

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetViewCClick(TObject *Sender)
{

	iSetView=16;
	iSetViewOff=32;
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetViewDClick(TObject *Sender)
{


	iSetView=16;
	iSetViewOff=48;

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------





void __fastcall TFormMetatileTool::ListTimerTimer(TObject *Sender)
{
	if(!openByFileDone) return;
	MakeList(false,false);
	ListTimer->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image2MouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;
	int f=0;
	int metasOnRow=sqrt(iSetView);
	int scale;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;

	if(bTab2x2){f=2; if (metasOnRow==4) scale=8*4; else scale=8*2;}
	if(bTab4x4){f=4; if (metasOnRow==4) scale=8*2; else scale=8*1;}
	if(bTab8x8){f=8; if (metasOnRow==4) scale=8*1; else scale=8*0.5;}

	//quantize steps & scale
	int tmpX=X/(f*scale);
	int tmpY=Y/(f*scale);
	int idX = tmpX;
	int idY = tmpY;
	tmpX=tmpX*(f);
	tmpY=tmpY*(f);
	mxDown=tmpX;  //used as origin for shift-selection
	myDown=tmpY;

	setSelection = TRect(tmpX, tmpY, tmpX+f, tmpY+f);
	mtClickSent=true;
	mtClickID = idY*metasOnRow + idX + iSetViewOff + iListViewOff;
	if(FormMTprops->Visible)FormMTprops->UpdateBitButtons_metasetClick();
	//Label6->Caption = IntToStr(mtClickID);

	if(mtClickID<256)GroupBox1->Caption = "Metatile "+IntToStr(mtClickID)+" [$"+IntToHex(mtClickID,2)+"]";
	else if(mtClickID<4096) GroupBox1->Caption = "Metatile "+IntToStr(mtClickID)+" [$"+IntToHex(mtClickID,3)+"]";
    else GroupBox1->Caption = "Metatile "+IntToStr(mtClickID)+" [$"+IntToHex(mtClickID,4)+"]";
	DrawTimer->Enabled=true;

	if(Shift.Contains(ssRight)&&Shift.Contains(ssShift))
	{
			iMapMatchCnt=0;
			FindMapMatch();
	}
	else if(Shift.Contains(ssRight)&&!Shift.Contains(ssShift))
	{

			if(prevmtClickID!=mtClickID) iMapMatchCnt=0;

			prevmtClickID=mtClickID;

			FindMapMatch();

			//begin drag
			TimerAsync->Enabled=true;
			Image2->BeginDrag(false,-1);
	}
	else{
		DisplayMT_usage(idX, idY);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image2MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;
	int f=0;
	int metasOnRow=sqrt(iSetView);
	int scale;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	//if(iSetView=16) metasOnRow=4;
	//if(iSetView=64) metasOnRow=4;

	if(bTab2x2){f=2; if (metasOnRow==4) scale=8*4; else scale=8*2;}
	if(bTab4x4){f=4; if (metasOnRow==4) scale=8*2; else scale=8*1;}
	if(bTab8x8){f=8; if (metasOnRow==4) scale=8*1; else scale=8*0.5;}

	//quantize steps & scale
	int tmpX=X/(f*scale);
	int tmpY=Y/(f*scale);
	int idX = tmpX;
	int idY = tmpY;
	tmpX=tmpX*(f);
	tmpY=tmpY*(f);





	if((Shift.Contains(ssShift))&&(Shift.Contains(ssLeft)))
	{
			if(tmpX<mxDown)   {setSelection.left=tmpX+f-(tmpX<setSelection.right?f:0);
				setSelection.right=mxDown+f;
				}
			if(tmpX>=mxDown) {setSelection.right =tmpX+(tmpX>=setSelection.left?f:0);
				setSelection.left=mxDown;
				}
			if(tmpY<myDown)  {setSelection.top=tmpY-(tmpY>=setSelection.bottom ?f:0);
				setSelection.bottom=myDown+f;
				}
			if(tmpY>=myDown) {setSelection.bottom=tmpY+(tmpY>=setSelection.top ?f:0);
				setSelection.top=myDown;
				}
        	setCursor = setSelection;
  }
  else	setCursor = TRect(tmpX, tmpY, tmpX+f, tmpY+f);

  if(prevSetCursorX!=tmpX || prevSetCursorY!=tmpY){
	DisplayMT_usage(idX,idY);
  }

  prevSetCursorX=tmpX;
  prevSetCursorY=tmpY;
  mtClickSent=false;

  DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::AppendFromNametable(void)
{

	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	int res = chkReserve1st->Checked==true?1:0;

	if(bTab2x2) BuildMetas(2,res);
	if(bTab4x4) BuildMetas(4,res);
	if(bTab8x8) BuildMetas(8,res);

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TFormMetatileTool::ListBox2x2Click(TObject *Sender)
{
	mtClickID -= iListViewOff;
	iListViewOff=(ListBox2x2->ItemIndex)*64;
	mtClickID += iListViewOff;

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::ListBox4x4Click(TObject *Sender)
{
   mtClickID -= iListViewOff;
   iListViewOff=(ListBox4x4->ItemIndex)*64;
   mtClickID += iListViewOff;

   DrawTimer->Enabled=true;
   
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::ListBox8x8Click(TObject *Sender)
{
   mtClickID -= iListViewOff;
   iListViewOff=(ListBox8x8->ItemIndex)*64;
   mtClickID += iListViewOff;

   DrawTimer->Enabled=true;

}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Rebuild1Click(TObject *Sender)
{
	FormMain->SetUndo();
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	int res = chkReserve1st->Checked==true?1:0;
	if(bTab2x2)
	{
		 memset(metatileSet_2x2_id,nullTile,64*4*metatileSets_2x2);
		 memset(metatileSet_2x2_pal,0,64*4*metatileSets_2x2);
		 memset(metatileSet_2x2_props,0,64*4*metatileSets_2x2);

		 memset(mtContent_2x2,0,64*metatileSets_2x2);
		 //memset(mtUsage_2x2,0,64*metatileSets_2x2*sizeof(uint32_t));
		 BuildMetas(2,res);
	}
	if(bTab4x4)
	{
		 memset(metatileSet_4x4_id,nullTile,64*16*metatileSets_4x4);
		 memset(metatileSet_4x4_pal,0,64*16*metatileSets_4x4);
		 memset(metatileSet_4x4_props,0,64*16*metatileSets_4x4);

		 memset(mtContent_4x4,0,64*metatileSets_4x4);
		 //memset(mtUsage_4x4,0,64*metatileSets_4x4*sizeof(uint32_t));
		 BuildMetas(4,res);
	}
	if(bTab8x8)
	{
		 memset(metatileSet_8x8_id,nullTile,64*64*metatileSets_8x8);
		 memset(metatileSet_8x8_pal,0,64*64*metatileSets_8x8);
		 memset(metatileSet_8x8_props,0,64*64*metatileSets_8x8);

		 memset(mtContent_8x8,0,64*metatileSets_8x8);
		 //memset(mtUsage_8x8,0,64*metatileSets_8x8*sizeof(uint32_t));
		 BuildMetas(8,res);
	}

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SpeedButton12Click(TObject *Sender)
{
	FormMTprops->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image1MouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(FormMain->IsBlockDrawing()) return;

	int xc;
	int yc;
	int d;

	bool bOutsideSel=false;

	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	int mtScale;

	if(bTab2x2)
	{
		d=2;
		mtScale=8;
		currentTable_id = metatileSet_2x2_id;
		currentTable_pal = metatileSet_2x2_pal;
		currentTable_props = metatileSet_2x2_props;
		currentMetaSelection = &metaSelection_2x2;
		currentMetaSelected = metaSelected_2x2;
	}
	if(bTab4x4)
	{
		d=4;
		mtScale=4;
		currentTable_id = metatileSet_4x4_id;
		currentTable_pal = metatileSet_4x4_pal;
		currentTable_props = metatileSet_4x4_props;
		currentMetaSelection = &metaSelection_4x4;
		currentMetaSelected = metaSelected_4x4;
	}
	if(bTab8x8)
	{
		d=8;
		mtScale=2;
		currentTable_id = metatileSet_8x8_id;
		currentTable_pal = metatileSet_8x8_pal;
		currentTable_props = metatileSet_8x8_props;
		currentMetaSelection = &metaSelection_8x8;
		currentMetaSelected = metaSelected_4x4;
	}

	xc=X/(8*mtScale);
	yc=Y/(8*mtScale);

	metaDownXc=xc;
	metaDownYc=yc;





   if (Shift.Contains(ssRight)) { //prep context for inside/outside selection
        FormMain->SelectTile(tileViewTable[currentTable_id[(mtClickID*d*d)+yc*d+xc]]);
		FormMain->SelectPalette(currentTable_pal[((yc*d)/(d*2))*(d*2) + (xc/2)*2 + (mtClickID)*d*d]);
		FormMain->UpdateTiles(true);
		FormMain->UpdateNameTable(-1,-1,true);
		if(FormNavigator->Visible) FormNavigator->Draw(false,false,false);


		if((currentMetaSelection->right<=xc)
				|(currentMetaSelection->left>xc)
				|(currentMetaSelection->bottom<=yc)
				|(currentMetaSelection->top>yc))
				{
					bOutsideSel=true;
					currentMetaSelection->left=-1;
					currentMetaSelection->top =-1;
				}


		else{
			//if(!Shift.Contains(ssShift) && currentMetaSelection.left != -1 && currentMetaSelection.top != -1) Image1->BeginDrag(false,-1);
			/*
			nameSelBuf.left		=	nameSelection.left;
			nameSelBuf.top		=	nameSelection.top;
			nameSelBuf.right	=	nameSelection.right;
			nameSelBuf.bottom  	=	nameSelection.bottom;
			//these are probably redundant
			destRect.left		=	nameSelection.left;
			destRect.top		=	nameSelection.top;
			destRect.right		=	nameSelection.right;
			destRect.bottom  	=	nameSelection.bottom;
			*/
			

		}
		//UpdateNameTable(-1,-1,true);
		//FormNavigator->Draw(false,false);
		//FormNavigator->UpdateLines(true);
	}

	else if(Shift.Contains(ssShift)&&Shift.Contains(ssLeft))   //begin selection
	{
		currentMetaSelection->left  =xc;
		currentMetaSelection->top   =yc;
		currentMetaSelection->right =currentMetaSelection->left+1;
		currentMetaSelection->bottom=currentMetaSelection->top +1;
		metaSelectMulti=false;
		Image1MouseMove(Sender,Shift,X,Y);
		//UpdateTiles(true);
		//UpdateNameTable(-1,-1,true);
		//FormNavigator->Draw(false,false);
		//FormNavigator->UpdateLines(false);
	}
	else if(Shift.Contains(ssCtrl)&&Shift.Contains(ssLeft))   //begin selection
	{
		bMtMultiSelectRemoveMode=currentMetaSelected[yc*d + xc];
		bool bTmp = bMtMultiSelectRemoveMode;
		currentMetaSelected[yc*d + xc]=Shift.Contains(ssLeft)?!bTmp:bTmp;


		metaSelectMulti=true;
	}
	else
	{
		//place behaviour.
		if(Shift.Contains(ssLeft)) {   //place tile
			FormMain->SetUndo();
			if(btnMap->Down)PrepMapEditBuffer(d, mtClickID*d*d);
		}
		Image1MouseMove(Sender,Shift,X,Y);
   }
	bOutsideSel=bOutsideSel; //suppress warning
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image1MouseMove(TObject *Sender,
	  TShiftState Shift, int X, int Y)
{
	if(FormMain->IsBlockDrawing()) return;


	int xc;
	int yc;
	int d;
	int set = bankActive/16;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	int mtScale;

	if(bTab2x2)
	{
		d=2;
		mtScale=8;
		currentTable_id = metatileSet_2x2_id;
		currentTable_pal = metatileSet_2x2_pal;
		currentTable_props = metatileSet_2x2_props;
		currentMetaSelection = &metaSelection_2x2;
		currentMetaSelected = metaSelected_2x2;
	}
	if(bTab4x4)
	{
		d=4;
		mtScale=4;
		currentTable_id = metatileSet_4x4_id;
		currentTable_pal = metatileSet_4x4_pal;
		currentTable_props = metatileSet_4x4_props;
		currentMetaSelection = &metaSelection_4x4;
		currentMetaSelected = metaSelected_4x4;
	}
	if(bTab8x8)
	{
		d=8;
		mtScale=2;
		currentTable_id = metatileSet_8x8_id;
		currentTable_pal = metatileSet_8x8_pal;
		currentTable_props = metatileSet_8x8_props;
		currentMetaSelection = &metaSelection_8x8;
		currentMetaSelected = metaSelected_8x8;
	}

	xc=X/(8*mtScale);
	yc=Y/(8*mtScale);
	//mtClickID is important here)

	if(X<0||Y<0||xc<0||xc>=d||yc<0||yc>=d) //redundancy.
	{
		return;
	}

	int labelIndex = mtClickID*d*d + yc*d + xc;
	LabelPos->Caption="x"+IntToStr(xc)+", y"+IntToStr(yc);
	LabelTilePal->Caption="$"+IntToHex(currentTable_id[labelIndex],2)+", "+IntToStr(currentTable_pal[labelIndex]);
	
	LabelProps->Caption="$"+IntToHex(currentTable_props[labelIndex],2);

	if(!FormMetatileTool->Active) return;

	//place tile
	if(Shift.Contains(ssLeft)&&!(Shift.Contains(ssShift)||Shift.Contains(ssCtrl))){

		if(chrSelectRect)    //place tiles/attrs with chr box selection
		{
			for(int i=0;i<chrSelection.bottom-chrSelection.top;++i)
			{
				for(int j=0;j<chrSelection.right-chrSelection.left;++j)
				{
					int dx=xc+j;
					int dy=yc+i;
					if(dx>=0&&dx<d&&dy>=0&&dy<d)
					{
						int tmpi = tileViewTable[((chrSelection.top+i)<<4)+chrSelection.left+j];

						if(btnTiles->Down)currentTable_id[(mtClickID*d*d)+dy*d+dx]=tmpi;
						if(btnAttr->Down){
							currentTable_pal[((dy*d)/(d*2))*(d*2) + (dx/2)*2 + (mtClickID)*d*d]=palActive;
						}
						if(btnProps->Down)currentTable_props[(mtClickID*d*d)+dy*d+dx]=tileProperties[bankViewTable[set+tmpi]/16 +tmpi];;
						if(btnMap->Down)AutoEditMap(d,dx,dy);
					}
				}
			}
		}
		//multi selection place
		else{
			int xmin=16;
			int ymin=16;
			int xmax=0;
			int ymax=0;

			//determine area
			for(int i=0;i<16;++i)
			{
				for(int j=0;j<16;++j)
				{
					if(!chrSelected[i*16+j]) continue;

					if(j<xmin) xmin=j;
					if(j>xmax) xmax=j;
					if(i<ymin) ymin=i;
					if(i>ymax) ymax=i;
				}
			}
			//place tiles
			for(int i=ymin;i<=ymax;++i)
			{
				for(int j=xmin;j<=xmax;++j)
				{
					if(!chrSelected[i*16+j]) continue;
					int dx=xc+j-xmin;
					int dy=yc+i-ymin;
					if(dx>=0&&dx<d&&dy>=0&&dy<d)
					{

					   	int tmpi = tileViewTable[i*16+j];

						if(btnTiles->Down)currentTable_id[(mtClickID*d*d)+dy*d+dx]=tmpi;
						if(btnAttr->Down){
							currentTable_pal[((dy*d)/(d*2))*(d*2) + (dx/2)*2 + (mtClickID)*d*d]=palActive;
						}
						if(btnProps->Down)currentTable_props[(mtClickID*d*d)+dy*d+dx]=tileProperties[bankViewTable[set+tmpi]/16 +tmpi];
						if(btnMap->Down)AutoEditMap(d,dx,dy);
					}
				}
			}
		}
	}

	//resize selection
	if(Shift.Contains(ssShift)&&!Shift.Contains(ssCtrl)){

		if(Shift.Contains(ssLeft)){

			if(xc<metaDownXc){
				currentMetaSelection->left=xc+1-(xc<currentMetaSelection->right?1:0);
				currentMetaSelection->right=metaDownXc+1;
			}
			if(xc>=metaDownXc){
				currentMetaSelection->right=xc+(xc>=currentMetaSelection->left?1:0);
				currentMetaSelection->left=metaDownXc;
			}
			if(yc<metaDownYc){
				currentMetaSelection->top=yc+1-(yc<currentMetaSelection->bottom?1:0);
				currentMetaSelection->bottom=metaDownYc+1;
			}
			if(yc>=metaDownYc){
				currentMetaSelection->bottom=yc+(yc>=currentMetaSelection->top?1:0);
				currentMetaSelection->top=metaDownYc;
			}
		}
		metaSelectMulti=false;
		for(int i=0;i<d*d;i++) currentMetaSelected[i]=false;

		int wdt=abs(currentMetaSelection->right -currentMetaSelection->left);
		int hgt=abs(currentMetaSelection->bottom-currentMetaSelection->top);

		for(int i=0;i<hgt;i++)
		{
			for(int j=0;j<wdt;j++)
			{
				currentMetaSelected[(i+currentMetaSelection->top)*d +j+currentMetaSelection->left]=true;
			}
		}

	}
    else
	 {
		xc=-1;
		yc=-1;
	 }


	//multi-select by dragging
	if(Shift.Contains(ssCtrl)&&(Shift.Contains(ssLeft)||Shift.Contains(ssRight)))
	{
		bool bTmp = bMtMultiSelectRemoveMode;
		currentMetaSelected[yc*d + xc]=Shift.Contains(ssLeft)?!bTmp:bTmp;
		metaSelectMulti=true;
	}


	//sample tile
	if(Shift.Contains(ssRight)){
			FormMain->SelectTile(tileViewTable[currentTable_id[(mtClickID*d*d)+yc*d+xc]]);
			FormMain->SelectPalette(currentTable_pal[((yc*d)/(d*2))*(d*2) + (xc/2)*2 + (mtClickID)*d*d]);
			FormMain->UpdateTiles(true);
			FormMain->UpdateNameTable(-1,-1,true);
			if(FormNavigator->Visible) FormNavigator->Draw(false,false,false);
	}
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::PageControl1Changing(TObject *Sender,
	  bool &AllowChange)
{

	//pocket current cursor before changing tab.
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;

	if(bTab2x2){mtClickID_store_2x2  = mtClickID;}
	if(bTab4x4){mtClickID_store_4x4  = mtClickID;}
	if(bTab8x8){mtClickID_store_8x8  = mtClickID;}

	metaSelection_2x2.left=-1;
	metaSelection_2x2.top=-1;
	metaSelection_4x4.left=-1;
	metaSelection_4x4.top=-1;
	metaSelection_8x8.left=-1;
	metaSelection_8x8.top=-1;

}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image2DragDrop(TObject *Sender,
	  TObject *Source, int X, int Y)
{
	FormMain->SetUndo();
	bBufCtrl	=(GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
	bBufShift 	=(GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bBufAlt		=(GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
	bool bClone = ( bBufCtrl && !bBufShift &&  bBufAlt);
	bool bSwap	= (!bBufCtrl && !bBufShift && !bBufAlt);
	bool bMove	= ( bBufCtrl && !bBufShift && !bBufAlt);
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	int d, scale;
	int metasOnRow=sqrt(iSetView);
	if(bTab2x2)
	{
	 d=2;
	 currentTable_id = metatileSet_2x2_id;
	 currentTable_pal = metatileSet_2x2_pal;
	 currentTable_props = metatileSet_2x2_props;
	 if (metasOnRow==4) scale=8*4; else scale=8*2;

	}

	if(bTab4x4){
		d=4;
		currentTable_id = metatileSet_4x4_id;
		currentTable_pal = metatileSet_4x4_pal;
		currentTable_props = metatileSet_4x4_props;
		if (metasOnRow==4) scale=8*2; else scale=8*1;
	}
	if(bTab8x8)
	{
		d=8;
		currentTable_id = metatileSet_8x8_id;
		currentTable_pal = metatileSet_8x8_pal;
		currentTable_props = metatileSet_8x8_props;
		if (metasOnRow==4) scale=8*1; else scale=8*0.5;
	}

	unsigned char *tmp_meta_id = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_pal = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_props = (char*)calloc((d*d),sizeof(char));

	//quantize steps & scale
	int tmpX=X/(d*scale);
	int tmpY=Y/(d*scale);
	int idX = tmpX;
	int idY = tmpY;
	tmpX=tmpX*(d);
	tmpY=tmpY*(d);

	//setSelection = TRect(tmpX, tmpY, tmpX+f, tmpY+f);
	//mtClickSent=true;
	int pd = (idY*metasOnRow + idX + iSetViewOff + iListViewOff)*d*d;
	int ps = mtClickID*d*d;


	//swap
			if(bSwap){
				//tile ID
				memcpy(tmp_meta_id			    , &currentTable_id[pd]	,d*d);
				memcpy(&currentTable_id	[pd]	, &currentTable_id[ps]	,d*d);
				memcpy(&currentTable_id		[ps], tmp_meta_id			,d*d);
				//pal attr
				memcpy(tmp_meta_pal			    , &currentTable_pal[pd]	,d*d);
				memcpy(&currentTable_pal   [pd]	, &currentTable_pal[ps]	,d*d);
				memcpy(&currentTable_pal	[ps], tmp_meta_pal			,d*d);
				//props
				memcpy(tmp_meta_props	 	    , &currentTable_props[pd]	,d*d);
				memcpy(&currentTable_props  [pd], &currentTable_props[ps]	,d*d);
				memcpy(&currentTable_props	[ps], tmp_meta_props			,d*d);

			}

			//move
			if(bMove){
				memset(tmp_meta_id,nullTile,d*d);
				memcpy(&currentTable_id		[pd], &currentTable_id[ps]	,d*d);
				memcpy(&currentTable_id		[ps], tmp_meta_id	,d*d);
				//pal attr
				memset(tmp_meta_pal,0,d*d);
				memcpy(&currentTable_pal 	[pd], &currentTable_pal[ps]	,d*d);
				memcpy(&currentTable_pal 	[ps], tmp_meta_pal	,d*d);
				//props
				memset(tmp_meta_props,0,d*d);
				memcpy(&currentTable_props 	[pd], &currentTable_props[ps],d*d);
				memcpy(&currentTable_props 	[ps], tmp_meta_props	,d*d);

			}

			//clone
			if(bClone){
				memcpy(&currentTable_id	[pd], &currentTable_id[ps]	,d*d);
				//pal attr
				memcpy(&currentTable_pal[pd], &currentTable_pal[ps]	,d*d);

				//props
				memcpy(&currentTable_props [pd], &currentTable_props[ps],d*d);

			}

	DrawTimer->Enabled=true;
    if(FormMTprops->Visible)FormMTprops->UpdateBitButtons_metasetClick();

	free(tmp_meta_id);
	free(tmp_meta_pal);
	free(tmp_meta_props);

}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image2DragOver(TObject *Sender,
	  TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	if(!FormMetatileTool->Active) return;
	bBufCtrl	=(GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
	bBufShift 	=(GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bBufAlt		=(GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
	bDrawDestShadow=true;
    int f=0;
	int metasOnRow=sqrt(iSetView);
	int scale;
    bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	if(bTab2x2){f=2; if (metasOnRow==4) scale=8*4; else scale=8*2;}
	if(bTab4x4){f=4; if (metasOnRow==4) scale=8*2; else scale=8*1;}
	if(bTab8x8){f=8; if (metasOnRow==4) scale=8*1; else scale=8*0.5;}

	//quantize steps & scale
	int tmpX=X/(f*scale);
	int tmpY=Y/(f*scale);
	tmpX=tmpX*(f);
	tmpY=tmpY*(f);


	destRect = TRect(tmpX, tmpY, tmpX+f, tmpY+f);
	Accept=true;
    DrawTimer->Enabled=true;

}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::FormKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	FormMain->FormKeyDown(Sender, Key, Shift);

	//old clone of formmain, not up to date, kept in case there was a change
	//i should reinstate for this form specifically.

	/*
	extern bool bIgnoreKey;
	extern bool lineDrawing;
	extern int iLinePresetIndex;
	extern bool bLinePreset_modeCoat[];
    extern bool bLinePreset_modeQuick[];
	extern bool bSmudge;
	extern int uiScale;
	extern int returnCarriagePos;
	extern bool cueStats;
	bool bDoUpdate;

	extern int nameTableViewX;
	extern int nameTableViewY;

    extern bool bAllowLockMessage;

	bBufCtrl=Shift.Contains(ssCtrl)?true:false;
	bBufShift=Shift.Contains(ssShift)?true:false;
	bBufAlt=Shift.Contains(ssAlt)?true:false;
	//FormMain->FormKeyDown(Sender,Key,Shift);

	if(!Shift.Contains(ssCtrl)&&!Shift.Contains(ssShift))
	{
		bool b=false;
		if(Key==VK_F1) {FormMain->PageControlEditor->ActivePageIndex=0; b=true;}
		if(Key==VK_F2) {FormMain->PageControlEditor->ActivePageIndex=1; b=true;}
		if(Key==VK_F3) {FormMain->PageControlEditor->ActivePageIndex=2; b=true;}
		if (b) {FormMain->PageControlEditorChange(Sender);}
		//

		if(Key==VK_PAUSE)
		{
			FormMain->AlphaBlend^=true;
			if (!Shift.Contains(ssShift))
			{
				FormCHREditor->AlphaBlend^=true;
				FormManageMetasprites->AlphaBlend^=true;
			}
		}


		if(!FormMain->SpeedButtonTypeIn->Down||(nameSelection.left<0&&nameSelection.top<0)){
			if(FormMain->PageControlEditor->ActivePage==FormMain->TabSheetName)
			{
				if(Key=='T') {bIgnoreKey=true; FormMain->TypeInModeOnOff1Click(FormMain->TypeInModeOnOff1);}
			}
		}
		if(!FormMain->SpeedButtonTypeIn->Down)
		{
			if(FormMain->PageControlEditor->ActivePage==FormMain->TabSheetSprite)
			{

			   if(Key=='H') FormMain->FlipMetaSprites(true,false,true);
			   if(Key=='V') FormMain->FlipMetaSprites(false,true,true);
			   if(Key=='P') FormMain->SBPriorityToggle1Click(Sender);
			}

			else     //hotkeys that are disabled when in sprite editing mode
			{
				if(FormMain->PageControlEditor->ActivePage==FormMain->TabSheetName)
				{
					if(Key=='V') clickV=true;
					if(Key=='C') clickC=true;

					if(Key=='H') FormMain->Tilegrid1Click(FormMain->Screen32x301);
					if(Key=='N') if(!lineDrawing){
						btnClonestamp->Down^=true;
						if(btnClonestamp->Down) 	FormMain->LabelStats->Caption="Metatile clonestamp ON.";
						else			   			   FormMain->LabelStats->Caption="Metatile clonestamp OFF.";
                        FormMain->UpdateLabelApplyTiles();
						FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;
					}

				}
					if(Key=='J') FormMain->Tilegrid1Click(FormMain->Tilegrid1);
					if(Key=='K') FormMain->Tilegrid1Click(FormMain->Tilegrid2);
					if(Key=='L') FormMain->Tilegrid1Click(FormMain->Tilegrid4);
            }
			if(FormMain->PageControlEditor->ActivePage==FormMain->TabSheetTile)
			{



				if(!lineDrawing)
				{
					if(Key=='T') {FormLineDetails->btnSmear->Down^=true;
						FormMain->LineCoating1->Checked=FormLineDetails->btnSmear->Down;
						int i = iLinePresetIndex;
						bLinePreset_modeCoat[i]=FormLineDetails->btnSmear->Down;
						if(FormLineDetails->btnSmear->Down) 	FormMain->LabelStats->Caption="Coat mode ON.";
						else									FormMain->LabelStats->Caption="Coat mode OFF.";
						FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;
					}
					if(Key=='K') {
						FormLineDetails->btnQuick->Down^=true;
						FormMain->LineQuickmultiline1->Checked=FormLineDetails->btnQuick->Down;
						int i = iLinePresetIndex;
						bLinePreset_modeQuick[i]=FormLineDetails->btnQuick->Down;
						if(FormLineDetails->btnQuick->Down) 	FormMain->LabelStats->Caption="Quick multiline ON.";
						else			   						FormMain->LabelStats->Caption="Quick multiline OFF.";
						FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;
					}

					if(Key=='H') FormCHREditor->MirrorHorizontal();
					if(Key=='V') FormCHREditor->MirrorVertical();
					if(Key=='R') FormCHREditor->SpeedButtonRotateCWClick(Sender);//{Flip90(false); Rotate4tiles(false);}
					if(Key=='G') {FormCHREditor->btnSmudge->Down=true; FormMain->btnSmudge->Down=true; bSmudge=true;}
					if(Key=='B') {FormCHREditor->btnThick->Down^=true; FormMain->btnThick->Down = FormCHREditor->btnThick->Down;
						if(FormMain->btnThick->Down) 	FormMain->LabelStats->Caption="Toggled to Brush mode.";
						else			   				FormMain->LabelStats->Caption="Toggled to Pen mode.";
						FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;
					}

					if(Key=='N') {FormCHREditor->btnLine->Down^=true; FormMain->btnLine->Down = FormCHREditor->btnLine->Down;
						if(FormMain->btnThick->Down) 	FormMain->LabelStats->Caption="Line mode ON.";
						else			   				FormMain->LabelStats->Caption="Line mode OFF.";
                        FormMain->UpdateLabelApplyTiles();
						FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;
					}

					if(Key=='U') {FormCHREditor->btnQuant->Down^=true; FormMain->btnQuant->Down = FormCHREditor->btnQuant->Down;
						if(FormMain->btnQuant->Down) 	FormMain->LabelStats->Caption="Quantized pixel placement ON.";
						else						   	FormMain->LabelStats->Caption="Quantized pixel placement OFF.";
						FormMain->StatusUpdateWaiter->Enabled=true; holdStats=true;

					}

					if(Key=='M') FormBrush->Visible^=true;

					if(Key=='F') FormBucketToolbox->ToggleFillMode();
					if(Key=='C') FormBucketToolbox->ToggleFloodMode();

				}
			}
			else{
                if(!lineDrawing)
				{
					if(Key=='R') FormMain->Red1Click(FormMain->Red1);
					if(Key=='G') FormMain->Red1Click(FormMain->Green1);
					if(Key=='B') FormMain->Red1Click(FormMain->Blue1);
					if(Key=='M') FormMain->Red1Click(FormMain->Gray1);
				}
			}
			//if(Key=='F' && !(nameSelection.left<0&&nameSelection.top<0)) FillMap();
			if(!lineDrawing)
			{
				if(Key=='X') FormMain->Toggletileset1Click(FormMain->Toggletileset1);
				if(Key=='A') FormMain->Attributes1Click(FormMain->Attributes1);
				if(Key=='S') FormMain->SelectedOnly1Click(FormMain->SelectedOnly1);
				if(Key=='W') FormMain->ApplyTiles1Click(FormMain->ApplyTiles1);
				if(Key=='D') FormMain->Tilegrid1Click(FormMain->GridOnOff1);
				if(Key=='E') FormMain->ApplyAttributes1Click(FormMain->ApplyAttributes1);
			}
            
		}
	}
	//
	if(!(nameSelection.left<0&&nameSelection.top<0))     //Photoshop alias
	{
		if(Shift.Contains(ssAlt))
		{
			if(Key==VK_BACK) FormMain->FillMap(false);
		}

	}
	if(Shift.Contains(ssCtrl))
	{
		if((Key=='1')&&(uiScale!=1)) {uiScale=1;   FormMain->UpdateUIScale();}
	}

	if(FormMain->SpeedButtonTypeIn->Down && FormMain->PageControlEditor->ActivePage==FormMain->TabSheetName)
	{
		switch(Key)
			{
			case VK_ESCAPE:
				FormMain->SpeedButtonTypeIn->Down=false;
				FormMain->StaticTextFontOffset->Visible=false;
				FormMain->TypeInModeOnOff1->Checked=false;
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
			FormMain->UpdateNameTable(-1,-1,false);
			FormNavigator->Draw(false,false,false);
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
		if(Key==VK_OEM_4||Key==VK_OEM_COMMA) {FormMain->SpeedButtonPrevMetaSpriteClick(Sender);cueStats=true;}// [
		if(Key==VK_OEM_6||Key==VK_OEM_PERIOD) {FormMain->SpeedButtonNextMetaSpriteClick(Sender);cueStats=true;}// ]

		
		if(Key==(int)MapVirtualKey(0x27, 1)) FormBrush->ChangePreset(-1);
		if(Key==(int)MapVirtualKey(0x28, 1)) FormBrush->ChangePreset(+1);
		if(Key==(int)MapVirtualKey(0x2B, 1)) FormBrush->ChangePreset(+7);


		if(Key=='Q') FormCHREditor->Show();
		if(Key==VK_NUMPAD7) FormCHREditor->TileChange(-1,-1);
		if(Key==VK_NUMPAD8) FormCHREditor->TileChange( 0,-1);
		if(Key==VK_NUMPAD9) FormCHREditor->TileChange(+1,-1);

		if(Key==VK_NUMPAD4) FormCHREditor->TileChange(-1,0);
		if(Key==VK_NUMPAD5) FormMain->MCHREditorClick(Sender);
		if(Key==VK_NUMPAD6) FormCHREditor->TileChange(+1,0);

		if(Key==VK_NUMPAD1) FormCHREditor->TileChange(-1,+1);
		if(Key==VK_NUMPAD2) FormCHREditor->TileChange( 0,+1);
		if(Key==VK_NUMPAD3) FormCHREditor->TileChange(+1,+1);
	}

	if(FormMain->PageControlEditor->ActivePage==FormMain->TabSheetName)
	{
		if(Shift.Contains(ssCtrl))
		{
			if(Key==VK_OEM_4) {FormMain->ChangeNameTableFrame(-1); cueStats=true;}// [
			if(Key==VK_OEM_6) {FormMain->ChangeNameTableFrame(1); cueStats=true;}// ]

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
					FormMain->CorrectView();
					FormMain->UpdateNameTable(-1,-1,true);
					FormNavigator->Draw(false,false,false);
					cueStats=true;
					}
			}
			else
			{
				if(Key==VK_LEFT)  FormMain->NameTableScrollLeft (Shift.Contains(ssShift));
				if(Key==VK_RIGHT) FormMain->NameTableScrollRight(Shift.Contains(ssShift));
				if(Key==VK_UP)    FormMain->NameTableScrollUp   (Shift.Contains(ssShift));
				if(Key==VK_DOWN)  FormMain->NameTableScrollDown (Shift.Contains(ssShift));

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
					FormMain->UpdateNameTable(-1,-1,false);
					FormNavigator->Draw(false,false,false);
					cueStats=true;
					}
			}
		}
	}
	else
	{
        int sprMov;
		if(Shift.Contains(ssShift)) sprMov=8; else sprMov=1;
		if(Key==VK_LEFT)  FormMain->MoveSprite(-sprMov, 0);
		if(Key==VK_RIGHT) FormMain->MoveSprite( sprMov, 0);
		if(Key==VK_UP)    FormMain->MoveSprite( 0,-sprMov);
		if(Key==VK_DOWN)  FormMain->MoveSprite( 0, sprMov);
		cueStats=true;
	}

	if (GetKeyState(VK_CAPITAL)&&(!FormMain->SpeedButtonTypeIn->Down))
	{
		if (bAllowLockMessage) {
			AnsiString lockbuf;
			lockbuf=FormMain->LabelStats->Caption;
			lockbuf+="\nStats locked. Press [CAPS LOCK] to unlock.";
			FormMain->LabelStats->Caption=lockbuf;
			bAllowLockMessage=false;
			cueStats=true;
		}
	}



	*/

}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

	FormMain->FormKeyUp(Sender, Key, Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::ListBox2x2DragOver(TObject *Sender,
	  TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	//this is meant for metatile drag events.
	//If i decide to add drag-n-drop rearrangement of sets in of themselves, that
	//needs to be appropriately handled.

	int tmpY=Y/ListBox2x2->ItemHeight;

	if(tmpY >= 0 && tmpY < (metatileSets_2x2))
	{
		ListBox2x2->ItemIndex = tmpY;
		iListViewOff=(ListBox2x2->ItemIndex)*64;
	}

	DrawTimer->Enabled=true;
	Accept = false;
	}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::ListBox4x4DragOver(TObject *Sender,
	  TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	int tmpY=Y/ListBox4x4->ItemHeight;

	if(tmpY >= 0 && tmpY < (metatileSets_4x4))
	{
		ListBox4x4->ItemIndex = tmpY;
		iListViewOff=(ListBox4x4->ItemIndex)*64;
	}

	DrawTimer->Enabled=true;
	Accept = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::ListBox8x8DragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
		int tmpY=Y/ListBox8x8->ItemHeight;

	if(tmpY >= 0 && tmpY < (metatileSets_8x8))
	{
		ListBox8x8->ItemIndex = tmpY;
		iListViewOff=(ListBox8x8->ItemIndex)*64;
	}

	DrawTimer->Enabled=true;
	Accept = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image2EndDrag(TObject *Sender,
      TObject *Target, int X, int Y)
{
	bDrawDestShadow=false;	
	TimerAsync->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::TimerAsyncTimer(TObject *Sender)
{
	bBufCtrl	=(GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
	bBufShift 	=(GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bBufAlt		=(GetAsyncKeyState(VK_MENU) & 0x8000) != 0;	
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Image1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	//int mtScale;
	int d;
	if(bTab2x2)
	{
		d=2;
		//mtScale=8;
		//currentTable_id = metatileSet_2x2_id;
		//currentTable_pal = metatileSet_2x2_pal;
		//currentTable_props = metatileSet_2x2_props;
		currentMetaSelection = &metaSelection_2x2;
	}
	if(bTab4x4)
	{
		d=4;
		//mtScale=4;
		//currentTable_id = metatileSet_4x4_id;
		//currentTable_pal = metatileSet_4x4_pal;
		//currentTable_props = metatileSet_4x4_props;
		currentMetaSelection = &metaSelection_4x4;
	}
	if(bTab8x8)
	{
		d=8;
		//mtScale=2;
		//currentTable_id = metatileSet_8x8_id;
		//currentTable_pal = metatileSet_8x8_pal;
		//currentTable_props = metatileSet_8x8_props;
		currentMetaSelection = &metaSelection_8x8;
	}

   if((currentMetaSelection->right - currentMetaSelection->left == d)
		&& (currentMetaSelection->bottom - currentMetaSelection->top == d))
		{
		currentMetaSelection->left= -1;
		currentMetaSelection->top= -1;

	    DrawTimer->Enabled=true;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmEveryInListClick(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char idVal = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;
	unsigned char palVal = bSetOrClear?(unsigned char)palActive:0;
	unsigned char propsVal = bSetOrClear?(unsigned char)mtPropsActive:0;

	if(bTab2x2)
	{
		 memset(metatileSet_2x2_id,idVal,64*4*metatileSets_2x2);
		 memset(metatileSet_2x2_pal,palVal,64*4*metatileSets_2x2);
		 memset(metatileSet_2x2_props,propsVal,64*4*metatileSets_2x2);
		 memset(mtContent_2x2,0,64*metatileSets_2x2);
		 //memset(mtUsage_2x2,0,64*metatileSets_2x2*sizeof(uint32_t));
	}
	if(bTab4x4)
	{
		 memset(metatileSet_4x4_id,idVal,64*16*metatileSets_4x4);
		 memset(metatileSet_4x4_pal,palVal,64*16*metatileSets_4x4);
		 memset(metatileSet_4x4_props,propsVal,64*16*metatileSets_4x4);

		 memset(mtContent_4x4,0,64*metatileSets_4x4);
		 //memset(mtUsage_4x4,0,64*metatileSets_4x4*sizeof(uint32_t));

	}
	if(bTab8x8)
	{
		 memset(metatileSet_8x8_id,idVal,64*64*metatileSets_8x8);
		 memset(metatileSet_8x8_pal,palVal,64*64*metatileSets_8x8);
		 memset(metatileSet_8x8_props,propsVal,64*64*metatileSets_8x8);

		 memset(mtContent_8x8,0,64*metatileSets_8x8);
		 //memset(mtUsage_8x8,0,64*metatileSets_8x8*sizeof(uint32_t));
	}

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmEveryAllClick(TObject *Sender)
{
	FormMain->SetUndo();

	unsigned char idVal = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;
	unsigned char palVal = bSetOrClear?(unsigned char)palActive:0;
	unsigned char propsVal = bSetOrClear?(unsigned char)mtPropsActive:0;


	memset(metatileSet_2x2_id,idVal,64*4*metatileSets_2x2);
	memset(metatileSet_2x2_pal,palVal,64*4*metatileSets_2x2);
	memset(metatileSet_2x2_props,propsVal,64*4*metatileSets_2x2);

	memset(mtContent_2x2,0,64*metatileSets_2x2);
	//memset(mtUsage_2x2,0,64*metatileSets_2x2*sizeof(uint32_t));

	memset(metatileSet_4x4_id,idVal,64*16*metatileSets_4x4);
	memset(metatileSet_4x4_pal,palVal,64*16*metatileSets_4x4);
	memset(metatileSet_4x4_props,propsVal,64*16*metatileSets_4x4);

	memset(mtContent_4x4,0,64*metatileSets_4x4);
	///memset(mtUsage_4x4,0,64*metatileSets_4x4*sizeof(uint32_t));


	memset(metatileSet_8x8_id,idVal,64*64*metatileSets_8x8);
	memset(metatileSet_8x8_pal,palVal,64*64*metatileSets_8x8);
	memset(metatileSet_8x8_props,propsVal,64*64*metatileSets_8x8);

	memset(mtContent_8x8,0,64*metatileSets_8x8);
	//memset(mtUsage_8x8,0,64*metatileSets_8x8*sizeof(uint32_t));


	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmPropsAllClick(TObject *Sender)
{
	FormMain->SetUndo();

	unsigned char propsVal = bSetOrClear?(unsigned char)mtPropsActive:0;


	memset(metatileSet_2x2_props,propsVal,64*4*metatileSets_2x2);
	memset(metatileSet_4x4_props,propsVal,64*16*metatileSets_4x4);
	memset(metatileSet_8x8_props,propsVal,64*64*metatileSets_8x8);

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmPropsThisListClick(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char propsVal = bSetOrClear?(unsigned char)mtPropsActive:0;

	if(bTab2x2)
	{
		 memset(metatileSet_2x2_props,propsVal,64*4*metatileSets_2x2);
	}
	if(bTab4x4)
	{
		 memset(metatileSet_4x4_props,propsVal,64*16*metatileSets_4x4);
	}
	if(bTab8x8)
	{
		 memset(metatileSet_8x8_props,propsVal,64*64*metatileSets_8x8);
	}

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmAttrAllClick(TObject *Sender)
{
	FormMain->SetUndo();

	unsigned char palVal = bSetOrClear?(unsigned char)palActive:0;

	memset(metatileSet_2x2_pal,palVal,64*4*metatileSets_2x2);
	memset(metatileSet_4x4_pal,palVal,64*16*metatileSets_4x4);
	memset(metatileSet_8x8_pal,palVal,64*64*metatileSets_8x8);

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmAttrThisListClick(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char palVal = bSetOrClear?(unsigned char)palActive:0;
	if(bTab2x2)
	{
		memset(metatileSet_2x2_pal,palVal,64*4*metatileSets_2x2);
	}
	if(bTab4x4)
	{
		memset(metatileSet_4x4_pal,palVal,64*16*metatileSets_4x4);
	}
	if(bTab8x8)
	{
		memset(metatileSet_8x8_pal,palVal,64*64*metatileSets_8x8);
	}
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rnNamesAllClick(TObject *Sender)
{
	FormMain->SetUndo();
	unsigned char idVal = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;
	memset(metatileSet_2x2_id,idVal,64*4*metatileSets_2x2);
	memset(metatileSet_4x4_id,idVal,64*16*metatileSets_4x4);
	memset(metatileSet_8x8_id,idVal,64*64*metatileSets_8x8);

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmNamesInListClick(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char idVal = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;

	if(bTab2x2)
	{
		memset(metatileSet_2x2_id,idVal,64*4*metatileSets_2x2);
	}
	if(bTab4x4)
	{
		memset(metatileSet_4x4_id,idVal,64*16*metatileSets_4x4);
	}
	if(bTab8x8)
	{
		memset(metatileSet_8x8_id,idVal,64*64*metatileSets_8x8);
	}
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmEveryOnSheetClick(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char idVal = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;
	unsigned char palVal = bSetOrClear?(unsigned char)palActive:0;
	unsigned char propsVal = bSetOrClear?(unsigned char)mtPropsActive:0;

	if(bTab2x2)
	{
		int offset_2x2 = (iSetViewOff+iListViewOff)*4;
		int len_2x2	   = iSetView*4;

		memset(&metatileSet_2x2_id[offset_2x2],idVal,len_2x2);
		memset(&metatileSet_2x2_pal[offset_2x2],palVal,len_2x2);
		memset(&metatileSet_2x2_props[offset_2x2],propsVal,len_2x2);

		memset(&mtContent_2x2[offset_2x2],0,len_2x2);
		//memset(&mtUsage_2x2[offset_2x2],0,len_2x2*sizeof(uint32_t));
	}
	if(bTab4x4)
	{
		int offset_4x4 = (iSetViewOff+iListViewOff)*16;
		int len_4x4	   = iSetView*16;

		memset(&metatileSet_4x4_id[offset_4x4],idVal,len_4x4);
		memset(&metatileSet_4x4_pal[offset_4x4],palVal,len_4x4);
		memset(&metatileSet_4x4_props[offset_4x4],propsVal,len_4x4);

		memset(&mtContent_4x4[offset_4x4],0,len_4x4);
		//memset(&mtUsage_4x4[offset_4x4],0,len_4x4*sizeof(uint32_t));
	}

	if(bTab8x8)
	{
		int offset_8x8 = (iSetViewOff+iListViewOff)*64;
		int len_8x8	   = iSetView*64;

		memset(&metatileSet_8x8_id[offset_8x8],idVal,len_8x8);
		memset(&metatileSet_8x8_pal[offset_8x8],palVal,len_8x8);
		memset(&metatileSet_8x8_props[offset_8x8],propsVal,len_8x8);

		memset(&mtContent_8x8[offset_8x8],0,len_8x8);
		//memset(&mtUsage_8x8[offset_8x8],0,len_8x8*sizeof(uint32_t));

	}
	DrawTimer->Enabled=true;

}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnMetaClearClick(TObject *Sender)
{
	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;
	bSetOrClear = ((TSpeedButton*)Sender)->Name=="btnMetaSet"?true:false;
	PopupMenuSetOrClear->Popup(x,y);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::onthissheet1Click(TObject *Sender)
{
    bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char val = bSetOrClear?(unsigned char)mtPropsActive:0;


	if(bTab2x2)
	{
		int offset_2x2 = (iSetViewOff+iListViewOff)*4;
		int len_2x2	   = iSetView*4;

		memset(&metatileSet_2x2_props[offset_2x2],val,len_2x2);
	}
	if(bTab4x4)
	{
		int offset_4x4 = (iSetViewOff+iListViewOff)*16;
		int len_4x4	   = iSetView*16;

		memset(&metatileSet_4x4_props[offset_4x4],val,len_4x4);
	}

	if(bTab8x8)
	{
		int offset_8x8 = (iSetViewOff+iListViewOff)*64;
		int len_8x8	   = iSetView*64;

		memset(&metatileSet_8x8_props[offset_8x8],val,len_8x8);
	}
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmAttrOnSheetClick(TObject *Sender)
{
   bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char val = bSetOrClear?(unsigned char)palActive:0;

	if(bTab2x2)
	{
		int offset_2x2 = (iSetViewOff+iListViewOff)*4;
		int len_2x2	   = iSetView*4;

		memset(&metatileSet_2x2_pal[offset_2x2],val,len_2x2);
	}
	if(bTab4x4)
	{
		int offset_4x4 = (iSetViewOff+iListViewOff)*16;
		int len_4x4	   = iSetView*16;

		memset(&metatileSet_4x4_pal[offset_4x4],val,len_4x4);
	}

	if(bTab8x8)
	{
		int offset_8x8 = (iSetViewOff+iListViewOff)*64;
		int len_8x8	   = iSetView*64;

		memset(&metatileSet_8x8_pal[offset_8x8],val,len_8x8);
	}
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmNameOnSheetClick(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char val = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;


	if(bTab2x2)
	{
		int offset_2x2 = (iSetViewOff+iListViewOff)*4;
		int len_2x2	   = iSetView*4;

		memset(&metatileSet_2x2_id[offset_2x2],val,len_2x2);
	}
	if(bTab4x4)
	{
		int offset_4x4 = (iSetViewOff+iListViewOff)*16;
		int len_4x4	   = iSetView*16;

		memset(&metatileSet_4x4_id[offset_4x4],val,len_4x4);
	}

	if(bTab8x8)
	{
		int offset_8x8 = (iSetViewOff+iListViewOff)*64;
		int len_8x8	   = iSetView*64;

		memset(&metatileSet_8x8_id[offset_8x8],val,len_8x8);
	}
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmEveryMetaClick(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char idVal = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;
	unsigned char palVal = bSetOrClear?(unsigned char)palActive:0;
	unsigned char propsVal = bSetOrClear?(unsigned char)mtPropsActive:0;

	if(bTab2x2)
	{
		int offset_2x2 = (mtClickID)*4;
		int len_2x2	   = 4;

		memset(&metatileSet_2x2_id[offset_2x2],idVal,len_2x2);
		memset(&metatileSet_2x2_pal[offset_2x2],palVal,len_2x2);
		memset(&metatileSet_2x2_props[offset_2x2],propsVal,len_2x2);

		memset(&mtContent_2x2[offset_2x2],0,len_2x2);
		//memset(&mtUsage_2x2[offset_2x2],0,len_2x2*sizeof(uint32_t));
	}
	if(bTab4x4)
	{
		int offset_4x4 = (mtClickID)*16;
		int len_4x4	   = 16;

		memset(&metatileSet_4x4_id[offset_4x4],idVal,len_4x4);
		memset(&metatileSet_4x4_pal[offset_4x4],palVal,len_4x4);
		memset(&metatileSet_4x4_props[offset_4x4],propsVal,len_4x4);

		memset(&mtContent_4x4[offset_4x4],0,len_4x4);
		///memset(&mtUsage_4x4[offset_4x4],0,len_4x4*sizeof(uint32_t));
	}

	if(bTab8x8)
	{
		int offset_8x8 = (mtClickID)*64;
		int len_8x8	   = 64;

		memset(&metatileSet_8x8_id[offset_8x8],idVal,len_8x8);
		memset(&metatileSet_8x8_pal[offset_8x8],palVal,len_8x8);
		memset(&metatileSet_8x8_props[offset_8x8],propsVal,len_8x8);

		memset(&mtContent_8x8[offset_8x8],0,len_8x8);
		//memset(&mtUsage_8x8[offset_8x8],0,len_8x8*sizeof(uint32_t));

	}
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmPropsInMetaClick(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	//unsigned char idVal = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;
	//unsigned char palVal = bSetOrClear?(unsigned char)palActive:0;
	unsigned char propsVal = bSetOrClear?(unsigned char)mtPropsActive:0;

	if(bTab2x2)
	{
		int offset_2x2 = (mtClickID)*4;
		int len_2x2	   = 4;

		memset(&metatileSet_2x2_props[offset_2x2],propsVal,len_2x2);
	}
	if(bTab4x4)
	{
		int offset_4x4 = (mtClickID)*16;
		int len_4x4	   = 16;

		memset(&metatileSet_4x4_props[offset_4x4],propsVal,len_4x4);


	}

	if(bTab8x8)
	{
		int offset_8x8 = (mtClickID)*64;
		int len_8x8	   = 64;

		memset(&metatileSet_8x8_props[offset_8x8],propsVal,len_8x8);

	}

	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmAttrInMetaClick(TObject *Sender)
{
    bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	//unsigned char idVal = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;
	unsigned char palVal = bSetOrClear?(unsigned char)palActive:0;
	//unsigned char propsVal = bSetOrClear?(unsigned char)mtPropsActive:0;

	if(bTab2x2)
	{
		int offset_2x2 = (mtClickID)*4;
		int len_2x2	   = 4;

		memset(&metatileSet_2x2_pal[offset_2x2],palVal,len_2x2);

	}
	if(bTab4x4)
	{
		int offset_4x4 = (mtClickID)*16;
		int len_4x4	   = 16;

		memset(&metatileSet_4x4_pal[offset_4x4],palVal,len_4x4);

	}

	if(bTab8x8)
	{
		int offset_8x8 = (mtClickID)*64;
		int len_8x8	   = 64;

		memset(&metatileSet_8x8_pal[offset_8x8],palVal,len_8x8);

	}
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::rmNameInMetaClick(TObject *Sender)
{
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;
	FormMain->SetUndo();

	unsigned char idVal = bSetOrClear?(unsigned char)tileActive:(unsigned char)nullTile;
	//unsigned char palVal = bSetOrClear?(unsigned char)palActive:0;
	//unsigned char propsVal = bSetOrClear?(unsigned char)mtPropsActive:0;

	if(bTab2x2)
	{
		int offset_2x2 = (mtClickID)*4;
		int len_2x2	   = 4;

		memset(&metatileSet_2x2_id[offset_2x2],idVal,len_2x2);
	}
	if(bTab4x4)
	{
		int offset_4x4 = (mtClickID)*16;
		int len_4x4	   = 16;

		memset(&metatileSet_4x4_id[offset_4x4],idVal,len_4x4);
	}

	if(bTab8x8)
	{
		int offset_8x8 = (mtClickID)*64;
		int len_8x8	   = 64;

		memset(&metatileSet_8x8_id[offset_8x8],idVal,len_8x8);
	}
	DrawTimer->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SpeedButton5Click(TObject *Sender)
{
	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;
	PopupMenuMore->Popup(x,y);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Append1Click(TObject *Sender)
{
	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;
	PopupMenuAppend->Popup(x,y);
}
//---------------------------------------------------------------------------


void __fastcall TFormMetatileTool::Fromnametablemap1Click(TObject *Sender)
{
	FormMain->SetUndo();
	AppendFromNametable();
}

//---------------------------------------------------------------------------
bool __fastcall TFormMetatileTool::AppendMetatilesFromFile(AnsiString filename, bool mt2,bool mt4,bool mt8)
{
	//some redundancies here - we're again checking if file exists, open and read it,
	//even though it is already pre-loaded by the preview. Just to be safe.
	bool bTab2x2 = PageControl1->ActivePage==FormMetatileTool->TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==FormMetatileTool->TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==FormMetatileTool->TabSheet8x8?true:false;
    int res = chkReserve1st->Checked==true?1:0;
	FILE *file;
	char *text;
	unsigned char temp[16];
	int i,j,filetype,size;
	bool r;

	file=fopen(OpenDialogAppend->FileName.c_str(),"rb");

	if(!file){
		return false;
	}

	fread(temp,sizeof(sessionIDStr),1,file);

	fseek(file,0,SEEK_END);
	size=ftell(file);
	fseek(file,0,SEEK_SET);

	text=(char*)malloc(size+1);
	text[size]=0;

	fread(text,size,1,file);
	fclose(file);

	filetype= 1;  //other

	if(!memcmp(temp,sessionIDStr,8)) filetype=2;    //session text file
	if(!memcmp(temp,metatiletxtIDstr,8)) filetype=3; //metasprite text file

	if (filetype==1) {
		OpenDialogAppend->Title=dialogTitle_AppendMetatiles;
		return false;
	}

	FormMain->ExtractMetatileAppendData(text,size,bTab2x2,bTab4x4,bTab8x8);

	FormMain->SetUndo();
	if(bTab2x2)MergeUniquesFromBuffer(2,res);
	if(bTab4x4)MergeUniquesFromBuffer(4,res);
	if(bTab8x8)MergeUniquesFromBuffer(8,res);

	return true;

}

//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Fromfile1Click(TObject *Sender)
{
  bool bTab2x2 = PageControl1->ActivePage==FormMetatileTool->TabSheet2x2?true:false;
  bool bTab4x4 = PageControl1->ActivePage==FormMetatileTool->TabSheet4x4?true:false;
  bool bTab8x8 = PageControl1->ActivePage==FormMetatileTool->TabSheet8x8?true:false;



  if(bTab2x2) {dialogTitle_AppendMetatiles="Open metatiles (2x2)";}
  if(bTab4x4) {dialogTitle_AppendMetatiles="Open metatiles (4x4)";}
  if(bTab8x8) {dialogTitle_AppendMetatiles="Open metatiles (8x8)";}


  if(bTab2x2) {OpenDialogAppend->Filter="All valid (*.mtt, *.nss, *.mtt2)|*.mtt;*.nss;*.mtt2;|Metatiles (*.mtt)|*.mtt|Session (*.nss)|*.nss|2x2 Metatiles|*.mtt2|Any (*.*)|*.*";}
  if(bTab4x4) {OpenDialogAppend->Filter="All valid (*.mtt, *.nss, *.mtt4)|*.mtt;*.nss;*.mtt4;|Metatiles (*.mtt)|*.mtt|Session (*.nss)|*.nss|4x4 Metatiles|*.mtt4|Any (*.*)|*.*";}
  if(bTab8x8) {OpenDialogAppend->Filter="All valid (*.mtt, *.nss, *.mtt8)|*.mtt;*.nss;*.mtt8;|Metatiles (*.mtt)|*.mtt|Session (*.nss)|*.nss|8x8 Metatiles|*.mtt8|Any (*.*)|*.*";}

  OpenDialogAppend->Title=dialogTitle_AppendMetatiles;
  if(!OpenDialogAppend->Execute()) return;

  FormMain->BlockDrawing(true);

  if(AppendMetatilesFromFile(OpenDialogAppend->FileName,bTab2x2,bTab4x4,bTab8x8))
  {
		OpenDialogAppend->FileName=RemoveExt(OpenDialogAppend->FileName);
        UpdateUI(true);
  }

  FormMain->BlockDrawing(false);

}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::OpenDialogAppendSelectionChange(
      TObject *Sender)
{

	if (OpenDialogAppend->FileName != ""){

		bool bTab2x2 = PageControl1->ActivePage==FormMetatileTool->TabSheet2x2?true:false;
		bool bTab4x4 = PageControl1->ActivePage==FormMetatileTool->TabSheet4x4?true:false;
		bool bTab8x8 = PageControl1->ActivePage==FormMetatileTool->TabSheet8x8?true:false;

		FILE *file;
		char *text;
		unsigned char temp[16];
		int i,j,filetype,size;
		bool r;

		file=fopen(OpenDialogAppend->FileName.c_str(),"rb");

		if(!file){
			OpenDialogAppend->Title=dialogTitle_AppendMetatiles;
			return;
		}

		fread(temp,sizeof(sessionIDStr),1,file);

		fseek(file,0,SEEK_END);
		size=ftell(file);
		fseek(file,0,SEEK_SET);

		text=(char*)malloc(size+1);
		text[size]=0;

		fread(text,size,1,file);
		fclose(file);

		filetype= 1;  //other

		if(!memcmp(temp,sessionIDStr,8)) filetype=2;    //session text file
		if(!memcmp(temp,metatiletxtIDstr,8)) filetype=3; //metasprite text file

		if (filetype==1) {
			OpenDialogAppend->Title=dialogTitle_AppendMetatiles;
			return;
		}
	//FormMain->SetUndo();
	FormMain->ExtractMetatileAppendData(text,size,bTab2x2,bTab4x4,bTab8x8);
	GenerateAppendMetatileTitle(bTab2x2,bTab4x4,bTab8x8);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMetatileTool::GenerateAppendMetatileTitle(bool mt2,bool mt4,bool mt8)
{

	AnsiString str_mt2="";
	AnsiString str_mt4="";
	AnsiString str_mt8="";
	int res = chkReserve1st->Checked==true?1:0;


	if(mt2){
		EvaluateOverlap(2,res);
		str_mt2="2x2 Match: "+IntToStr(cnt_appendFromFileMatch)+" Unique: "+IntToStr(cnt_appendFromFileUnique);
	}

	if(mt4){
		EvaluateOverlap(4,res);
		str_mt4="4x4 Match: "+IntToStr(cnt_appendFromFileMatch)+" Unique: "+IntToStr(cnt_appendFromFileUnique);
		FormMain->LabelStats->Caption=str_mt4;
	}

	if(mt8){
	   EvaluateOverlap(8,res);
	   str_mt8="8x8 Match: "+IntToStr(cnt_appendFromFileMatch)+" Unique: "+IntToStr(cnt_appendFromFileUnique);
	}
	if (mt2) dialogTitle_AppendMetatiles += str_mt2;
	if (mt4) dialogTitle_AppendMetatiles += str_mt4;
	if (mt8) dialogTitle_AppendMetatiles += str_mt8;
	OpenDialogAppend->Title=dialogTitle_AppendMetatiles;
}

void __fastcall TFormMetatileTool::EvaluateOverlap(int whichType, int reserveOff)
{
	bool match;
	cnt_appendFromFileMatch=0;
	cnt_appendFromFileUnique=0;
	bool doPal = btnUseAttr->Down;
	int d, tmp;
	//bool doPropConditions = FormPropConditions->chkMetas->Checked;
    int buf_size;
	if(whichType==2){d=2; buf_size=buf_metatileSets_2x2*64;}
	if(whichType==4){d=4; buf_size=buf_metatileSets_4x4*64;}
	if(whichType==8){d=8; buf_size=buf_metatileSets_8x8*64;}


	//take stock of contents in buffer
	unsigned char *tmp_meta_nullcmp = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_palcmp = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_propscmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *buf_mtContent = (char*)calloc((buf_size),sizeof(char));

	memset(tmp_meta_nullcmp,nullTile,d*d);
	memset(tmp_meta_palcmp,0,d*d);
	memset(tmp_meta_propscmp,0,d*d);

	if(whichType==2){
		for (int i = 0; i < metatileSets_2x2*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			buf_mtContent[i]= memcmp(tmp_meta_nullcmp ,metatileBuf_2x2_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_palcmp ,metatileBuf_2x2_pal+(i*d*d) ,d*d);
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_propscmp ,metatileBuf_2x2_props+(i*d*d) ,d*d);
		}
	}
	if(whichType==4){
		for (int i = 0; i < metatileSets_4x4*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			buf_mtContent[i]= memcmp(tmp_meta_nullcmp ,metatileBuf_4x4_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_palcmp ,metatileBuf_4x4_pal+(i*d*d) ,d*d);
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_propscmp ,metatileBuf_4x4_props+(i*d*d) ,d*d);
		}
	}
	if(whichType==8){
		for (int i = 0; i < metatileSets_8x8*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			buf_mtContent[i]= memcmp(tmp_meta_nullcmp ,metatileBuf_8x8_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_palcmp ,metatileBuf_8x8_pal+(i*d*d) ,d*d);
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_propscmp ,metatileBuf_8x8_props+(i*d*d) ,d*d);
		}
	}



	//do comparisons
	if(whichType==2) {

		for (int i = 0; i < (buf_metatileSets_2x2*64); i++) {

			if(buf_mtContent[i]>0) for (int m = reserveOff; m < (metatileSets_2x2*64); m++) {
				match = CompareBufToSet(metatileBuf_2x2_id,(i*d*d),metatileSet_2x2_id,m*d*d,d*d);
				if(match==true)
				{
					//does the palette data match too?
					match = CompareBufToSet(metatileBuf_2x2_pal,(i*d*d),metatileSet_2x2_pal,m*d*d,d*d);
					if((match==true) || (doPal==false))
					{
						match = CompareBufToSet(metatileBuf_2x2_props,(i*d*d),metatileSet_2x2_props,m*d*d,d*d);
						if(match==true) {cnt_appendFromFileMatch++; goto Next_2x2;} //already in set, don't place
					}
				}

			}
			//no match, increase count.
			cnt_appendFromFileUnique++;
			Next_2x2:
		}
	}
	if(whichType==4) {

		 for (int i = 0; i < (buf_metatileSets_4x4*64); i++) {
			if(buf_mtContent[i]>0) for (int m = 0; m < (metatileSets_4x4*64); m++) {
				match = CompareBufToSet(metatileBuf_4x4_id,(i*d*d),metatileSet_4x4_id,m*d*d,d*d);
				if(match==true)
				{
					//does the palette data match too?
					match = CompareBufToSet(metatileBuf_4x4_pal,i*d*d,metatileSet_4x4_pal,m*d*d,d*d);
					if((match==true) || (doPal==false))
					{
						match = CompareBufToSet(metatileBuf_4x4_props,i*d*d,metatileSet_4x4_props,m*d*d,d*d);
						if(match==true) {cnt_appendFromFileMatch++; goto Next_4x4;} //already in set, don't place
					}
				}

			}
			else{goto Next_4x4;}
			//no match, increase count.
			cnt_appendFromFileUnique++;
			Next_4x4:
		}
	}
   if(whichType==8) {

		for (int i = 0; i < (buf_metatileSets_8x8*64); i++) {
			if(buf_mtContent[i]>0) for (int m = reserveOff; m < (metatileSets_8x8*64); m++) {
				match = CompareBufToSet(metatileBuf_8x8_id,(i*d*d),metatileSet_8x8_id,m*d*d,d*d);
				if(match==true)
				{
					//does the palette data match too?
					match = CompareBufToSet(metatileBuf_8x8_pal,(i*d*d),metatileSet_8x8_pal,m*d*d,d*d);
					if((match==true) || (doPal==false))
					{
						match = CompareBufToSet(metatileBuf_8x8_props,(i*d*d),metatileSet_8x8_props,m*d*d,d*d);
						if(match==true) {cnt_appendFromFileMatch++; goto Next_8x8;} //already in set, don't place
					}
				}

			}
			//no match, increase count.
			cnt_appendFromFileUnique++;
			Next_8x8:
		}
	}

	free(tmp_meta_nullcmp);
	free(tmp_meta_palcmp);
	free(tmp_meta_propscmp);

	free(buf_mtContent);
}

void __fastcall TFormMetatileTool::MergeUniquesFromBuffer(int whichType, int reserveOff)
{
	bool match;
	cnt_appendFromFileMatch=0;
	cnt_appendFromFileUnique=0;
	bool doPal = btnUseAttr->Down;
	int d, tmp;
	//bool doPropConditions = FormPropConditions->chkMetas->Checked;
    int buf_size;
	if(whichType==2){d=2; buf_size=buf_metatileSets_2x2*64;}
	if(whichType==4){d=4; buf_size=buf_metatileSets_4x4*64;}
	if(whichType==8){d=8; buf_size=buf_metatileSets_8x8*64;}


	//take stock of contents in buffer
	unsigned char *tmp_meta_nullcmp = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_palcmp = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_propscmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *buf_mtContent = (char*)calloc((buf_size),sizeof(char));

	memset(tmp_meta_nullcmp,nullTile,d*d);
	memset(tmp_meta_palcmp,0,d*d);
	memset(tmp_meta_propscmp,0,d*d);

	if(whichType==2){
		//buffer content
		for (int i = 0; i < buf_metatileSets_2x2*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			buf_mtContent[i]= memcmp(tmp_meta_nullcmp ,metatileBuf_2x2_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_palcmp ,metatileBuf_2x2_pal+(i*d*d) ,d*d);
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_propscmp ,metatileBuf_2x2_props+(i*d*d) ,d*d);
		}
        //existing metatiles content
		for (int i = 0; i < metatileSets_2x2*64; i++) {
			mtContent_2x2[i]= memcmp(tmp_meta_nullcmp ,metatileSet_2x2_id+(i*d*d) ,d*d);
			if(mtContent_2x2[i]==0) mtContent_2x2[i]= memcmp(tmp_meta_palcmp ,metatileSet_2x2_pal+(i*d*d) ,d*d);
			if(mtContent_2x2[i]==0) mtContent_2x2[i]= memcmp(tmp_meta_propscmp ,metatileSet_2x2_props+(i*d*d) ,d*d);
		}
	}

	if(whichType==4){
		//buffer content
		for (int i = 0; i < buf_metatileSets_4x4*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			buf_mtContent[i]= memcmp(tmp_meta_nullcmp ,metatileBuf_4x4_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_palcmp ,metatileBuf_4x4_pal+(i*d*d) ,d*d);
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_propscmp ,metatileBuf_4x4_props+(i*d*d) ,d*d);
		}
        for (int i = 0; i < metatileSets_4x4*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			mtContent_4x4[i]= memcmp(tmp_meta_nullcmp ,metatileSet_4x4_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(mtContent_4x4[i]==0) mtContent_4x4[i]= memcmp(tmp_meta_palcmp ,metatileSet_4x4_pal+(i*d*d) ,d*d);
			if(mtContent_4x4[i]==0) mtContent_4x4[i]= memcmp(tmp_meta_propscmp ,metatileSet_4x4_props+(i*d*d) ,d*d);

		}

	}


	if(whichType==8){
		for (int i = 0; i < buf_metatileSets_8x8*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			buf_mtContent[i]= memcmp(tmp_meta_nullcmp ,metatileBuf_8x8_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_palcmp ,metatileBuf_8x8_pal+(i*d*d) ,d*d);
			if(buf_mtContent[i]==0) buf_mtContent[i]= memcmp(tmp_meta_propscmp ,metatileBuf_8x8_props+(i*d*d) ,d*d);
		}
        for (int i = 0; i < metatileSets_8x8*64; i++) {

			//mtContent is currently only concerned with 0 = no content, !0 = content.
			mtContent_8x8[i]= memcmp(tmp_meta_nullcmp ,metatileSet_8x8_id+(i*d*d) ,d*d);
			//if seemingly contentless, check for if all palettes are 0 to be extra sure.
			if(mtContent_8x8[i]==0) mtContent_8x8[i]= memcmp(tmp_meta_palcmp ,metatileSet_8x8_pal+(i*d*d) ,d*d);
			if(mtContent_8x8[i]==0) mtContent_8x8[i]= memcmp(tmp_meta_propscmp ,metatileSet_8x8_props+(i*d*d) ,d*d);

		}

	}



	//do comparisons
	if(whichType==2) {

		for (int i = 0; i < (buf_metatileSets_2x2*64); i++) {

			if(buf_mtContent[i]>0) for (int m = 0; m < (metatileSets_2x2*64); m++) {
				match = CompareBufToSet(metatileBuf_2x2_id,(i*d*d),metatileSet_2x2_id,m*d*d,d*d);
				if(match==true)
				{
					//does the palette data match too?
					match = CompareBufToSet(metatileBuf_2x2_pal,(i*d*d),metatileSet_2x2_pal,m*d*d,d*d);
					if((match==true) || (doPal==false))
					{
						match = CompareBufToSet(metatileBuf_2x2_props,(i*d*d),metatileSet_2x2_props,m*d*d,d*d);
						if(match==true) {cnt_appendFromFileMatch++; goto Next_2x2;} //already in set, don't place
					}
				}

			}
			else{goto Next_2x2;}
			//found meta but no match, put content
			cnt_appendFromFileUnique++;
			for ( int n = reserveOff; n < (64*metatileSets_2x2); n++) {
				if (mtContent_2x2[n]==0){
					memcpy(metatileSet_2x2_id+(n*d*d),		metatileBuf_2x2_id+(i*d*d),d*d);
					memcpy(metatileSet_2x2_pal+(n*d*d),		metatileBuf_2x2_pal+(i*d*d),d*d);
					memcpy(metatileSet_2x2_props+(n*d*d),	metatileBuf_2x2_props+(i*d*d),d*d);

					//Update mtContent so we know this place is occupied.
					mtContent_2x2[n]= memcmp(tmp_meta_nullcmp ,metatileBuf_2x2_id+(i*d*d),d*d);
					//if mt has no contentful tile data, evaluate based on palettes instead.
					if(mtContent_2x2[n]==0)
					{
						mtContent_2x2[n]= memcmp(tmp_meta_palcmp ,metatileSet_2x2_pal+(i*d*d) ,d*d);
						if(mtContent_2x2[n]==0)
						{
							mtContent_2x2[n]= memcmp(tmp_meta_propscmp ,metatileSet_2x2_props+(i*d*d) ,d*d);
						}
					}
				goto Next_2x2;
				}
			}
			Next_2x2:
		}
	}
	if(whichType==4) {

		 for (int i = 0; i < (buf_metatileSets_4x4*64); i++) {
			if(buf_mtContent[i]>0) for (int m = 0; m < (metatileSets_4x4*64); m++) {
				match = CompareBufToSet(metatileBuf_4x4_id,(i*d*d),metatileSet_4x4_id,m*d*d,d*d);
				if(match==true)
				{
					//does the palette data match too?
					match = CompareBufToSet(metatileBuf_4x4_pal,i*d*d,metatileSet_4x4_pal,m*d*d,d*d);
					if((match==true) || (doPal==false))
					{
						match = CompareBufToSet(metatileBuf_4x4_props,i*d*d,metatileSet_4x4_props,m*d*d,d*d);
						if(match==true) {cnt_appendFromFileMatch++; goto Next_4x4;} //already in set, don't place
					}
				}

			}
			else{goto Next_4x4;}
			//found meta but no match, put content
			cnt_appendFromFileUnique++;
            for ( int n = reserveOff; n < (64*metatileSets_4x4); n++) {
				if (mtContent_4x4[n]==0){
					memcpy(metatileSet_4x4_id+(n*d*d),		metatileBuf_4x4_id+(i*d*d),d*d);
					memcpy(metatileSet_4x4_pal+(n*d*d),		metatileBuf_4x4_pal+(i*d*d),d*d);
					memcpy(metatileSet_4x4_props+(n*d*d),	metatileBuf_4x4_props+(i*d*d),d*d);

					//Update mtContent so we know this place is occupied.
					mtContent_4x4[n]= memcmp(tmp_meta_nullcmp ,metatileBuf_4x4_id+(i*d*d),d*d);
					//if mt has no contentful tile data, evaluate based on palettes instead.
					if(mtContent_4x4[n]==0)
					{
						mtContent_4x4[n]= memcmp(tmp_meta_palcmp ,metatileSet_4x4_pal+(i*d*d) ,d*d);
						if(mtContent_4x4[n]==0)
						{
							mtContent_4x4[n]= memcmp(tmp_meta_propscmp ,metatileSet_4x4_props+(i*d*d) ,d*d);
						}
					}
				goto Next_4x4;
				}
			}
			Next_4x4:
		}
	}
   if(whichType==8) {
		for (int i = 0; i < (buf_metatileSets_8x8*64); i++) {
			if(buf_mtContent[i]>0) for (int m = reserveOff; m < (metatileSets_8x8*64); m++) {
				match = CompareBufToSet(metatileBuf_8x8_id,(i*d*d),metatileSet_8x8_id,m*d*d,d*d);
				if(match==true)
				{
					//does the palette data match too?
					match = CompareBufToSet(metatileBuf_8x8_pal,(i*d*d),metatileSet_8x8_pal,m*d*d,d*d);
					if((match==true) || (doPal==false))
					{
						match = CompareBufToSet(metatileBuf_8x8_props,(i*d*d),metatileSet_8x8_props,m*d*d,d*d);
						if(match==true) {cnt_appendFromFileMatch++; goto Next_8x8;} //already in set, don't place
					}
				}

			}
			else{goto Next_8x8;}
			//found meta but no match, put content
			cnt_appendFromFileUnique++;
            for ( int n = reserveOff; n < (64*metatileSets_8x8); n++) {
				if (mtContent_8x8[n]==0){
					memcpy(metatileSet_8x8_id+(n*d*d),		metatileBuf_8x8_id+(i*d*d),d*d);
					memcpy(metatileSet_8x8_pal+(n*d*d),		metatileBuf_8x8_pal+(i*d*d),d*d);
					memcpy(metatileSet_8x8_props+(n*d*d),	metatileBuf_8x8_props+(i*d*d),d*d);

					//Update mtContent so we know this place is occupied.
					mtContent_8x8[n]= memcmp(tmp_meta_nullcmp ,metatileBuf_8x8_id+(i*d*d),d*d);
					//if mt has no contentful tile data, evaluate based on palettes instead.
					if(mtContent_8x8[n]==0)
					{
						mtContent_8x8[n]= memcmp(tmp_meta_palcmp ,metatileSet_8x8_pal+(i*d*d) ,d*d);
						if(mtContent_8x8[n]==0)
						{
							mtContent_8x8[n]= memcmp(tmp_meta_propscmp ,metatileSet_8x8_props+(i*d*d) ,d*d);
						}
					}
				goto Next_8x8;
				}
			}
			Next_8x8:
		}
	}

	free(tmp_meta_nullcmp);
	free(tmp_meta_palcmp);
	free(tmp_meta_propscmp);

	free(buf_mtContent);

}



void __fastcall TFormMetatileTool::Copy1Click(TObject *Sender)
{
	FormMain->CopyMetatiles(false,true);	
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::PageControl1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Select active metatile format: 2x2, 4x4 or 8x8.\nA session may contain and use metatiles of all three formats.\nIt's up to your game which is more suitable for export.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::PageControl1MouseLeave(TObject *Sender)
{
	FormMain->LabelStats->Caption="---";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Copy1MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Copies the currently selected set (64 metatiles),\n or if viewing subsets a...d is toggled, the 16 currently viewed metatiles.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Paste1MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Pastes metatiles from clipboard; aligned to set if sizes match.\n\nTip:\t if you want to paste from an offset, click the set canvas and press ctrl+v instead.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Rename1MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Up1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Moves set up the list.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Down1MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Moves set down the list.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Duplicate1MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Duplicates current set as a new list item.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Remove1MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Removes current set + list item.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Clear1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Clears current set.\n\nTip:\tThere are more clear actions at the bottom right corner of this form.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Insert1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Inserts an empty set as a new list item.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnMapMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="When down, NEXXT will seek any matches of the metatile you're editing on the map\nand apply the changes there as well.\nWarning: can be slow, and possibly destructive. Saving regularly and reviewing changes is recommended.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnPropsMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="When down, editing collision properties of this metatile is permitted.\nThis includes any associated collision data from your tileset.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnAttrMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="When down, editing attribute properties is permitted.\nMetatiles store attributes at an 8x8 pixel granularity; \nhowever is currently only displaying it at 16x16 (NES native resolution).";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnTilesMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="When down, editing tiles of the metatile is permitted.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnUseAttrMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="If on, subpalette attributes are displayed.\nIf off, metatiles behave attribute-less and use whatever active subpalette is chosen.\nThis can help make smaller sets for 2x2 metas.\nSubpalette attribute information is also used to generare (build/rebuild/append) metatiles.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnClonestampMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Clonestamp [N while on map/nametable]:\nIf on, a metatile brush replaces the normal tile brush on the nametable/map canvas;\nso long as the metatile editor remains open.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnCloneSnapMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="If on, the metatile brush snaps to the nametable/map canvas grid.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SpeedButton12MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetViewAMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Show the 1st quarter of a metatile set.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetViewBMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Show the 2nd quarter of a metatile set.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetViewCMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Show the 3rd quarter of a metatile set.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetViewDMouseEnter(TObject *Sender)
{
 FormMain->LabelStats->Caption="Show the 4th quarter of a metatile set.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::SBSetView64MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="View the set item of 64 metatiles.\nTip:\tYou can have as many set items as you want, but you can only view up to 64 metatiles at a time.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Rebuild1MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Append1MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Sort1MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Find1MouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnMetaSetMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Set a portion of metatile data to a specific value.\nCan affect tiles, attributes or properties, or all three.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::btnMetaClearMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Clear a portion of metatile data.\nCan affect tiles, attributes or properties, or all three.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::chkAlignScrMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="If checked, generated metatiles take the 32x30 screen alignment into consideration.\nAdditionally if checked, or screen grid is active,\nbig metatile stamps placed with the metatile brush are truncated to fit.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::chkReserve1stMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="When generating metatiles, this option makes sure the 1st metatile is left empty/alone.";
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Paste1Click(TObject *Sender)
{
	FormMain->PasteMetatiles(true); //bool: align	
}
//---------------------------------------------------------------------------


void __fastcall TFormMetatileTool::PrepMapEditBuffer(int d, int meta_index)
{
	bool match;
	bool doPropConditions = FormPropConditions->chkMetas->Checked;
	bool doAlign = chkAlignScr->Checked;
	bool clearTmp = false;
	int set = bankActive/16;
	bool doPal = btnUseAttr->Down;

	unsigned char *tmp_meta_id = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_idcmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_meta_pal = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_palcmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_meta_props = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_propscmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_wildcard = (char*)calloc((d*d),sizeof(char));

	unsigned char *empty_table = (char*)calloc((d*d),sizeof(char));

	//int namesize=nameTableWidth*nameTableHeight;
	memset(tmp_nametable_match,0,sizeof(tmp_nametable_match));

	memset(empty_table, (unsigned char) nullTile, d*d);
	memcpy(tmp_meta_idcmp, &currentTable_id[meta_index],d*d);           // mtClickID*d*d
	memcpy(tmp_meta_palcmp, &currentTable_pal[meta_index],d*d);
	memcpy(tmp_meta_propscmp, &currentTable_props[meta_index],d*d);



	//converts to NES nametable style data. should be made conditional when there's more formats
	/*
	for(int k=0; k<d; k++){
		for(int j=0; j<d; j++){
			tmp_meta_palcmp[k*d + j] = 	tmp_meta_palcmp[((k*d)/(d*2))*(d*2) + (j/2)*2];
		}
	}
	*/

	//sample nametable contents
	int tmpD;
	for (int i=0; i < nameTableHeight ; i+=d) {

		//special rules for screen grid alignment, pt 1
		if ((i % 30 == 28) && d==4 && doAlign) {tmpD=2; clearTmp=true;}
		else if((i % 30 == 24)&& d==8 && doAlign) {tmpD=6; clearTmp=true;}
		else{tmpD = d; clearTmp=false;}

		if(clearTmp){
			memset(tmp_meta_id,nullTile,d*d);
			memset(tmp_meta_pal,0,d*d);
			memset(tmp_meta_props,0,d*d);

			if(d==4){
				memset(tmp_wildcard,0,(d*d)/2);
				memset(tmp_wildcard+((d*d)/2),1,(d*d)/2);
			}
			else if(d==8){
				memset(tmp_wildcard,0,((d*d)/4)*3);
				memset(tmp_wildcard+(((d*d)/4)*3),1,(d*d)/4);
			}
		}
		else{
			memset(tmp_wildcard,0,d*d);
		}
	
		for (int j = 0; j < nameTableWidth ; j+=d) {

			for (int y = 0; y < tmpD; y++) {
				for (int x = 0; x < d; x++) {

					tmp_meta_id[x + y*d] = nameTable[(i+y)*nameTableWidth+(j+x)];
					tmp_meta_pal[x + y*d] = FormMain->AttrGet(j+x,i+y, false, false);

					/*
					for(int k=0; k<d; k++){
						for(int j=0; j<d; j++){
							tmp_meta_pal[k*d + j] = 	tmp_meta_pal[((k*d)/(d*2))*(d*2) + (j/2)*2];
						}
					}
					*/


					if(doPropConditions){
						int tn =  nameTable[(i+y)*nameTableWidth+(j+x)];
						unsigned char tmpProp = (unsigned char)tileProperties[bankViewTable[set+tn]/16 + tn];
						unsigned char tmpPal = tmp_meta_pal[x + y*d];

						//1st pass for conditions - the bitwise condition.
						for(int bit=0;bit<8;bit++){
							if(propConditional[bit]==1 && ((tmpProp >> bit) & 1)){
								tmpProp = propCondition[tmpPal][bit]? tmpProp:tmpProp &= ~(1 << bit);
							}

						}
						//2nd pass for conditions - the 'all' condition.
						for(int bit=0;bit<8;bit++){

							//if a bit is set that has the 'all conditional', nullify properties if there isn't a palette match.
							if(propConditional[bit]==2 && ((tmpProp >> bit) & 1)){
								tmpProp = propCondition[tmpPal][bit]? tmpProp:0;
							}
						}

							tmp_meta_props[x + y*d] = tmpProp;
					}
					else{
						int tn = nameTable[(i+y)*nameTableWidth+(j+x)];
						tmp_meta_props[x + y*d] = (unsigned char)tileProperties[bankViewTable[set+tn]/16+tn];
					}

				}
			}

			//find a match

			match = CompareTempToSet(tmp_wildcard,tmp_meta_id,empty_table,d*d,0);
			if(match==true) goto Next;

			match = CompareTempToSet(tmp_wildcard,tmp_meta_id,tmp_meta_idcmp,d*d,0);
			if(match==false) goto Next;

				//does the palette data match too?
			match = CompareTempToSet(tmp_wildcard,tmp_meta_pal,tmp_meta_palcmp,d*d,0);
			if((match==false) && (doPal==true)) goto Next;

			   //	match = CompareTempToSet(tmp_wildcard,tmp_meta_props,tmp_meta_propscmp,d*d,d*d);
			   //	if(match==false) goto Next; //no match, don't place




			//match, set reference table slot.
			tmp_nametable_match[i*nameTableWidth +j]=1;

			Next:
			} //width enclosure
			if(clearTmp)i-=2; //realign y-axis sampling cursor after half-metas have been processed.
	}         //height enclosure

	free(tmp_meta_id);
	free(tmp_meta_idcmp);

	free(tmp_meta_pal);
	free(tmp_meta_palcmp);

	free(tmp_meta_props);
	free(tmp_meta_propscmp);
	free(tmp_wildcard);

	free(empty_table);
}

//---------------------------------------------------------------------------

int __fastcall TFormMetatileTool::AutoSelectMap(int d)
{
	bool doAlign = chkAlignScr->Checked;
	bool adjust=false;
	//int set = bankActive/16;
	unsigned int tmp_cnt=0;
	unsigned int matches=0;
	//1st pass - count matches
	for (int i=0; i < nameTableHeight ; i+=d) {

		//special rules for screen grid alignment, pt 1
		if ((i % 30 == 28) && d==4 && doAlign) {adjust=true;}
		else if((i % 30 == 24)&& d==8 && doAlign) {adjust=true;}
		else{adjust=false;}
		for (int j = 0; j < nameTableWidth ; j+=d) {

			if(d==4 && adjust) goto Next_0;
			if(d==8 && adjust) goto Next_0;
			if(tmp_nametable_match[i*nameTableWidth + j]==0) goto Next_0;

				matches++;

			Next_0:
		}
	if(adjust)i-=2;
	}


	//2nd pass - find match
	for (int i=0; i < nameTableHeight ; i+=d) {

		//special rules for screen grid alignment, pt 1
		if ((i % 30 == 28) && d==4 && doAlign) {adjust=true;}
		else if((i % 30 == 24)&& d==8 && doAlign) {adjust=true;}
		else{adjust=false;}
		for (int j = 0; j < nameTableWidth ; j+=d) {


			if(d==4 && adjust) goto Next;
			if(d==8 && adjust) goto Next;
			if(tmp_nametable_match[i*nameTableWidth + j]==0) goto Next;
			if(iMapMatchCnt==tmp_cnt) {

				nameSelection.left = j;
				nameSelection.right = j+d;
				nameSelection.top = i;
				nameSelection.bottom = i+d;

				cueUpdateNametable=true;
				return matches;
			}
			tmp_cnt++;
			Next:
			
		}
	if(adjust)i-=2;
	}
	cueUpdateNametable=true;
	return matches;
}


//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::AutoEditMap(int d, int tx, int ty)
{
	bool doAlign = chkAlignScr->Checked;
	bool adjust=false;
	//int set = bankActive/16;

	for (int i=0; i < nameTableHeight ; i+=d) {

		//special rules for screen grid alignment, pt 1
		if ((i % 30 == 28) && d==4 && doAlign) {adjust=true;}
		else if((i % 30 == 24)&& d==8 && doAlign) {adjust=true;}
		else{adjust=false;}
		for (int j = 0; j < nameTableWidth ; j+=d) {

			if(d==4 && adjust && ty>=2) goto Next;
			if(d==8 && adjust && ty>=6) goto Next;
			if(tmp_nametable_match[i*nameTableWidth + j]==0) goto Next;

			nameTable[(i+ty)*nameTableWidth + j+tx]=currentTable_id[(mtClickID*d*d)+ty*d+tx];

			if(FormMetatileTool->btnAttr) FormMain->AttrSet(j+tx,i+ty,currentTable_pal[(mtClickID*d*d) + ((ty*d)/(d*2))*(d*2) + (tx/2)*2],false);
			//else FormMain->AttrSet(tx,ty,palActive,false);

			//old - todo: test which is best.
			//note that the new behaviour uses useAttr as search criteria in the routine above this one.
			/*
			if(FormMetatileTool->btnUseAttr) FormMain->AttrSet(j+tx,i+ty,currentTable_pal[(mtClickID*d*d) + ((ty*d)/(d*2))*(d*2) + (tx/2)*2],false);
			else FormMain->AttrSet(tx,ty,palActive,false);
			*/
			//FormMain->UpdateNameTable(j+tx,i+ty,false);
			
			Next:
		}
	if(adjust)i-=2;
	}
	cueUpdateNametable=true;
}

void __fastcall TFormMetatileTool::Removeunused1Click(TObject *Sender)
{
	FormMain->SetUndo();
	bool match;
	bool doPropConditions = FormPropConditions->chkMetas->Checked;
	bool doAlign = chkAlignScr->Checked;
	bool clearTmp = false;
	bool bTab2x2 = PageControl1->ActivePage==FormMetatileTool->TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==FormMetatileTool->TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==FormMetatileTool->TabSheet8x8?true:false;
	int d;
	int mt_end;
	int set = bankActive/16;
	if(bTab2x2)
	{
		d=2;

		mt_end=64*metatileSets_2x2;
		currentTable_id = metatileSet_2x2_id;
		currentTable_pal = metatileSet_2x2_pal;
		currentTable_props = metatileSet_2x2_props;
		currentMetaSelection = &metaSelection_2x2;
		currentMetaSelected = metaSelected_2x2;
	}
	if(bTab4x4)
	{
		d=4;
		mt_end=64*metatileSets_4x4;
		currentTable_id = metatileSet_4x4_id;
		currentTable_pal = metatileSet_4x4_pal;
		currentTable_props = metatileSet_4x4_props;
		currentMetaSelection = &metaSelection_4x4;
		currentMetaSelected = metaSelected_4x4;
	}
	if(bTab8x8)
	{
		d=8;
		mt_end=64*metatileSets_8x8;
		currentTable_id = metatileSet_8x8_id;
		currentTable_pal = metatileSet_8x8_pal;
		currentTable_props = metatileSet_8x8_props;
		currentMetaSelection = &metaSelection_8x8;
		currentMetaSelected = metaSelected_4x4;
	}

	unsigned char *tmp_meta_id = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_idcmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_meta_pal = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_palcmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_meta_props = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_propscmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_wildcard = (char*)calloc((d*d),sizeof(char));

	memset(tmp_nametable_match,0,sizeof(tmp_nametable_match));

	int cnt=0;
	for(int index=0; index<mt_end; index++){
		memcpy(tmp_meta_idcmp, &currentTable_id[index*d*d],d*d);
		memcpy(tmp_meta_palcmp, &currentTable_pal[index*d*d],d*d);
		memcpy(tmp_meta_propscmp, &currentTable_props[index*d*d],d*d);

		//sample nametable contents
		int tmpD;
		for (int i=0; i < nameTableHeight ; i+=d) {

			//special rules for screen grid alignment, pt 1
			if ((i % 30 == 28) && d==4 && doAlign) {tmpD=2; clearTmp=true;}
			else if((i % 30 == 24)&& d==8 && doAlign) {tmpD=6; clearTmp=true;}
			else{tmpD = d; clearTmp=false;}

			if(clearTmp){
				memset(tmp_meta_id,nullTile,d*d);
				memset(tmp_meta_pal,0,d*d);
				memset(tmp_meta_props,0,d*d);

				if(d==4){
					memset(tmp_wildcard,0,(d*d)/2);
					memset(tmp_wildcard+((d*d)/2),1,(d*d)/2);
				}
				else if(d==8){
					memset(tmp_wildcard,0,((d*d)/4)*3);
					memset(tmp_wildcard+(((d*d)/4)*3),1,(d*d)/4);
				}
			}
			else{
				memset(tmp_wildcard,0,d*d);
			}

			for (int j = 0; j < nameTableWidth ; j+=d) {
				for (int y = 0; y < tmpD; y++) {
					for (int x = 0; x < d; x++) {

					tmp_meta_id[x + y*d] = nameTable[(i+y)*nameTableWidth+(j+x)];
					tmp_meta_pal[x + y*d] = FormMain->AttrGet(j+x,i+y, false, false);

					if(doPropConditions){
						int tn = nameTable[(i+y)*nameTableWidth+(j+x)];
						unsigned char tmpProp = (unsigned char)tileProperties[bankViewTable[set+tn]/16 +tn];
						unsigned char tmpPal = tmp_meta_pal[x + y*d];

						//1st pass for conditions - the bitwise condition.
						for(int bit=0;bit<8;bit++){
							if(propConditional[bit]==1 && ((tmpProp >> bit) & 1)){
								tmpProp = propCondition[tmpPal][bit]? tmpProp:tmpProp &= ~(1 << bit);
								}
							}
							//2nd pass for conditions - the 'all' condition.
							for(int bit=0;bit<8;bit++){

							//if a bit is set that has the 'all conditional', nullify properties if there isn't a palette match.
								if(propConditional[bit]==2 && ((tmpProp >> bit) & 1)){
									tmpProp = propCondition[tmpPal][bit]? tmpProp:0;
								}
							}
							tmp_meta_props[x + y*d] = tmpProp;
						}
						else{
							int tn = nameTable[(i+y)*nameTableWidth+(j+x)];
							tmp_meta_props[x + y*d] = (unsigned char)tileProperties[bankViewTable[set+tn]/16+tn];
						}
					}
				}

				//find a match

				match = CompareTempToSet(tmp_wildcard,tmp_meta_id,tmp_meta_idcmp,d*d,0);
				if(match){
					//does the palette data match too?
					match = CompareTempToSet(tmp_wildcard,tmp_meta_pal,tmp_meta_palcmp,d*d,0);
					if(match) goto SkipToNextMeta;
				}
			} //width enclosure
			if(clearTmp)i-=2; //realign y-axis sampling cursor after half-metas have been processed.
		}         //height enclosure
		//all positions exhausted, delete
		cnt++;
		memset(&currentTable_id[index*d*d],nullTile,d*d);
		memset(&currentTable_pal[index*d*d],0,d*d);
		memset(&currentTable_props[index*d*d],0,d*d);

		SkipToNextMeta:
	}             //metatile enclosure

	UpdateUI(false);
	AnsiString strD=IntToStr(d);
	AnsiString str=IntToStr(cnt)+" unused metatiles ("+strD+"x"+strD+") removed.";
	FormMain->LabelStats->Caption=str;
    FormMain->StatusUpdateWaiter->Enabled=true;
	holdStats=true;
	free(tmp_meta_id);
	free(tmp_meta_idcmp);

	free(tmp_meta_pal);
	free(tmp_meta_palcmp);

	free(tmp_meta_props);
	free(tmp_meta_propscmp);
	free(tmp_wildcard);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::Sort1Click(TObject *Sender)
{
	TPoint p = Mouse->CursorPos;
	int x= p.x;
	int y= p.y;
	PopupMenuMore->Popup(x,y);
}
//---------------------------------------------------------------------------


void __fastcall TFormMetatileTool::Associateothernssmetatileliststothissession1Click(
	  TObject *Sender)
{
	FormMain->SetUndo();
	bool match;
	bool bTab2x2 = PageControl1->ActivePage==FormMetatileTool->TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==FormMetatileTool->TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==FormMetatileTool->TabSheet8x8?true:false;
	int d;
	int res = chkReserve1st->Checked==true?1:0;
	int mt_end;
	if(bTab2x2)
	{
		d=2;

		mt_end=64*metatileSets_2x2;
		currentTable_id = metatileSet_2x2_id;
		currentTable_pal = metatileSet_2x2_pal;
		currentTable_props = metatileSet_2x2_props;
		currentTable_content = mtContent_4x4;

		currentMetaSelection = &metaSelection_2x2;
		currentMetaSelected = metaSelected_2x2;
	}
	if(bTab4x4)
	{
		d=4;
		mt_end=64*metatileSets_4x4;
		currentTable_id = metatileSet_4x4_id;
		currentTable_pal = metatileSet_4x4_pal;
		currentTable_props = metatileSet_4x4_props;
		currentTable_content = mtContent_4x4;

		currentMetaSelection = &metaSelection_4x4;
		currentMetaSelected = metaSelected_4x4;
	}
	if(bTab8x8)
	{
		d=8;
		mt_end=64*metatileSets_8x8;
		currentTable_id = metatileSet_8x8_id;
		currentTable_pal = metatileSet_8x8_pal;
		currentTable_props = metatileSet_8x8_props;
		currentTable_content = mtContent_8x8;
		currentMetaSelection = &metaSelection_8x8;
		currentMetaSelected = metaSelected_4x4;
	}

	unsigned char *tmp_meta_id = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_nullcmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_meta_pal = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_palcmp = (char*)calloc((d*d),sizeof(char));

	unsigned char *tmp_meta_props = (char*)calloc((d*d),sizeof(char));
	unsigned char *tmp_meta_propscmp = (char*)calloc((d*d),sizeof(char));

	memset(tmp_meta_nullcmp,nullTile,d*d);
	memset(tmp_meta_palcmp,0,d*d);
	memset(tmp_meta_propscmp,0,d*d);

	//take content
	for (int i = res; i < mt_end; i++) {

		//mtContent is currently only concerned with 0 = no content, !0 = content.
		currentTable_content[i]= memcmp(tmp_meta_nullcmp ,currentTable_id+(i*d*d) ,d*d)==0;
		//if seemingly contentless, check for if all palettes are 0 to be extra sure.
		if(currentTable_content[i]==0) currentTable_content[i]= memcmp(tmp_meta_palcmp ,currentTable_pal+(i*d*d) ,d*d);
		if(currentTable_content[i]==0) currentTable_content[i]= memcmp(tmp_meta_propscmp ,currentTable_props+(i*d*d) ,d*d);
	}

	int cnt=0;
	for(int i=res; i<mt_end; i++){
		if(currentTable_content[i]>0) continue;
		//sample nametable contents
		for (int j = res; j<mt_end ; j++) {
			if(currentTable_content[j]>0) continue;
            if(i==j) continue; //don't compare against self

			//find a match
			match = memcmp(currentTable_id+(i*d*d),currentTable_id+(j*d*d),d*d)==0;
			if(match){
				//does the palette data match too?
				match = memcmp(currentTable_pal+(i*d*d),currentTable_pal+(j*d*d),d*d)==0;
				if(match){
					//does the props data match too?
					match = memcmp(currentTable_props+(i*d*d),currentTable_props+(j*d*d),d*d)==0;
					if(match)
					{
						cnt++;
						memset(&currentTable_id[j*d*d],nullTile,d*d);
						memset(&currentTable_pal[j*d*d],0,d*d);
						memset(&currentTable_props[j*d*d],0,d*d);
						currentTable_content[j]=1; //don't iterate removed metas
					}
				}
			}
		}             //metatile enclosure
	}

	UpdateUI(false);
	AnsiString strD=IntToStr(d);
	AnsiString str=IntToStr(cnt)+" duplicate metatiles ("+strD+"x"+strD+") removed.";
	FormMain->LabelStats->Caption=str;
	FormMain->StatusUpdateWaiter->Enabled=true;
	holdStats=true;

	free(tmp_meta_id);
	free(tmp_meta_nullcmp);

	free(tmp_meta_pal);
	free(tmp_meta_palcmp);

	free(tmp_meta_props);
	free(tmp_meta_propscmp);
}
//---------------------------------------------------------------------------
 void __fastcall TFormMetatileTool::FindMapMatch(void)
{
	int d;
	bool bTab2x2 = PageControl1->ActivePage==FormMetatileTool->TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==FormMetatileTool->TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==FormMetatileTool->TabSheet8x8?true:false;
	//int mt_end;
	if(bTab2x2)
	{
		d=2;

		//mt_end=64*metatileSets_2x2;
		currentTable_id = metatileSet_2x2_id;
		currentTable_pal = metatileSet_2x2_pal;
		currentTable_props = metatileSet_2x2_props;
		currentTable_content = mtContent_4x4;

		currentMetaSelection = &metaSelection_2x2;
		currentMetaSelected = metaSelected_2x2;
	}
	if(bTab4x4)
	{
		d=4;
		//mt_end=64*metatileSets_4x4;
		currentTable_id = metatileSet_4x4_id;
		currentTable_pal = metatileSet_4x4_pal;
		currentTable_props = metatileSet_4x4_props;
		currentTable_content = mtContent_4x4;

		currentMetaSelection = &metaSelection_4x4;
		currentMetaSelected = metaSelected_4x4;
	}
	if(bTab8x8)
	{
		d=8;
		//mt_end=64*metatileSets_8x8;
		currentTable_id = metatileSet_8x8_id;
		currentTable_pal = metatileSet_8x8_pal;
		currentTable_props = metatileSet_8x8_props;
		currentTable_content = mtContent_8x8;
		currentMetaSelection = &metaSelection_8x8;
		currentMetaSelected = metaSelected_4x4;
	}



	//builds a table of map matches. Originally used for map autorepairs; we'll use it here too.
	PrepMapEditBuffer(d, mtClickID*d*d);
	unsigned int matches = AutoSelectMap(d);



	FormMain->LabelStats->Caption="Match "+IntToStr(iMapMatchCnt+1)+" of "+IntToStr(matches)+" selected.";
	FormMain->StatusUpdateWaiter->Enabled=true;
	holdStats=true;
	if(iMapMatchCnt+1 >=matches) iMapMatchCnt=0;
	else iMapMatchCnt++;
}
//---------------------------------------------------------------------------


void __fastcall TFormMetatileTool::Findfirstmapmatch1Click(TObject *Sender)
{
	FindMapMatch();
}

//---------------------------------------------------------------------------
int __fastcall TFormMetatileTool::GetMTMap_matches(int d)
{
   bool doAlign = chkAlignScr->Checked;
	bool adjust=false;
	//int set = bankActive/16;

	unsigned int matches=0;
	//1st pass - count matches
	for (int i=0; i < nameTableHeight ; i+=d) {

		//special rules for screen grid alignment, pt 1
		if ((i % 30 == 28) && d==4 && doAlign) {adjust=true;}
		else if((i % 30 == 24)&& d==8 && doAlign) {adjust=true;}
		else{adjust=false;}
		for (int j = 0; j < nameTableWidth ; j+=d) {

			if(d==4 && adjust) goto Next_0;
			if(d==8 && adjust) goto Next_0;
			if(tmp_nametable_match[i*nameTableWidth + j]==0) goto Next_0;

				matches++;

			Next_0:
		}
	if(adjust)i-=2;
	}
	return matches;
}
//---------------------------------------------------------------------------
void __fastcall TFormMetatileTool::DisplayMT_usage(int idX, int idY)
{
	int metasOnRow=sqrt(iSetView);
	int d;
	bool bTab2x2 = PageControl1->ActivePage==TabSheet2x2?true:false;
	bool bTab4x4 = PageControl1->ActivePage==TabSheet4x4?true:false;
	bool bTab8x8 = PageControl1->ActivePage==TabSheet8x8?true:false;

if(bTab2x2)
	{
		d=2;
		currentTable_id = metatileSet_2x2_id;
		currentTable_pal = metatileSet_2x2_pal;
		currentTable_props = metatileSet_2x2_props;
		currentTable_content = mtContent_4x4;

		currentMetaSelection = &metaSelection_2x2;
		currentMetaSelected = metaSelected_2x2;
	}
	if(bTab4x4)
	{
		d=4;
		currentTable_id = metatileSet_4x4_id;
		currentTable_pal = metatileSet_4x4_pal;
		currentTable_props = metatileSet_4x4_props;
		currentTable_content = mtContent_4x4;

		currentMetaSelection = &metaSelection_4x4;
		currentMetaSelected = metaSelected_4x4;
	}
	if(bTab8x8)
	{
		d=8;
		currentTable_id = metatileSet_8x8_id;
		currentTable_pal = metatileSet_8x8_pal;
		currentTable_props = metatileSet_8x8_props;
		currentTable_content = mtContent_8x8;

		currentMetaSelection = &metaSelection_8x8;
		currentMetaSelected = metaSelected_4x4;
	}

	int tmp_hoverID = idY*metasOnRow + idX + iSetViewOff + iListViewOff;
	 int activeMT_matches;
	PrepMapEditBuffer(d, tmp_hoverID*d*d);
	int hoverMT_Matches = GetMTMap_matches(d);
	if(mtClickID>=0){
		PrepMapEditBuffer(d, mtClickID*d*d);
		activeMT_matches = GetMTMap_matches(d);
	}

	AnsiString ansiHelp = "\nClick to select.\t\t\t\t\t\tShift-drag to select several (for copying/pasting).\nRight-click: select && step first match on map. \t\tShift+right-click: reset step to first match.\nRight-drag: Swap, Move (+ctrl)  or Clone (+ctrl+alt).";

	if(mtClickID>=0){
		FormMain->LabelStats->Caption="Hovered metatile (#"+IntToStr(tmp_hoverID)+") usage: "+IntToStr(hoverMT_Matches)+"\tSelected metatile (#"+IntToStr(mtClickID)+") usage: "+IntToStr(activeMT_matches)+ansiHelp;
	}
	else{
		FormMain->LabelStats->Caption="Hovered metatile (#"+IntToStr(tmp_hoverID)+") usage: "+IntToStr(hoverMT_Matches);
	}

	FormMain->StatusUpdateWaiter->Enabled=true;
	holdStats=true;
}

void __fastcall TFormMetatileTool::btnClonestampClick(TObject *Sender)
{
	if(btnClonestamp->Down)
		FormMain->LabelStats->Caption="Metatile clonestamp ON.";
	else  FormMain->LabelStats->Caption="Metatile clonestamp OFF.";
	FormMain->UpdateLabelApplyTiles();
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	FormMain->UpdateLabelApplyTiles();
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::FormHide(TObject *Sender)
{
	FormMain->UpdateLabelApplyTiles();	
}
//---------------------------------------------------------------------------

void __fastcall TFormMetatileTool::FormKeyPress(TObject *Sender, char &Key)
{
	FormMain->FormKeyPress(Sender, Key);
}
//---------------------------------------------------------------------------


