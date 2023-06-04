//---------------------------------------------------------------------------

#ifndef UnitMainH
#define UnitMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <TIME.H>


//---------------------------------------------------------------------------

#define REG_SECTION "Software\\Shiru\\NES Screen Tool\\"

#define NAME_MAX_WIDTH		4096
#define NAME_MAX_HEIGHT		4096
#define NAME_MAX_SIZE		(NAME_MAX_WIDTH*NAME_MAX_HEIGHT)
#define ATTR_MAX_SIZE		((NAME_MAX_WIDTH/4)*(NAME_MAX_HEIGHT/4))

#define OAM_B2		4
#define OAM_B3		8
#define OAM_B4		16
#define OAM_PRIO	32
#define OAM_FLIP_H	64
#define OAM_FLIP_V	128

const int viewPortWidth=32;
const int viewPortHeight=32;

const int 	FAC_GUISCALE=2;
const int 	FAC_INKFLOW=3;

const int		FAC_COL=3;
const int		FAC_PAL=0;

const bool 	FAC_GRIDSHOW=false;
const bool	FAC_GRID1=false;
const bool	FAC_GRID2=true;
const bool	FAC_GRID4=false;

const bool	FAC_CHRSHOW=false;
const bool	FAC_MMSHOW=false;

const int	FAC_ALPHA=180;

const bool 	FAC_0F=true;
const bool 	FAC_SHARED=true;





enum {
	SAVE_FORMAT_BIN=0,
	SAVE_FORMAT_H,
	SAVE_FORMAT_ASM
};

const char regWorkingDirectory[]="WorkDir";
const char colBlack=0x0f;

const char sessionIDStr[8]="NSTssTXT";

const float emphasis[8][3]={//from nintech.txt
	{100.0,100.0,100.0},
	{ 74.3, 91.5,123.9},
	{ 88.2,108.6, 79.4},
	{ 65.3, 98.0,101.9},
	{127.7,102.6, 90.5},
	{ 97.9, 90.8,102.3},
	{100.1, 98.7, 74.1},
	{ 75.0, 75.0, 75.0}
};



//---------------------------------------------------------------------------

class TFormMain : public TForm
{
	__published:	// IDE-managed Components
	TOpenDialog *OpenDialogChr;
	TMainMenu *MainMenu1;
	TMenuItem *MPatterns;
	TMenuItem *MCHROpen;
	TMenuItem *MCHRSave;
	TOpenDialog *OpenDialogName;
	TSaveDialog *SaveDialogName;
	TMenuItem *MNameTable;
	TMenuItem *MOpenNameTable;
	TMenuItem *MSaveNameTableC;
	TMenuItem *MSaveNameTableBIN;
	TMenuItem *MPalettes;
	TMenuItem *MPaletteOpen;
	TMenuItem *MPaletteSave;
	TOpenDialog *OpenDialogPal;
	TSaveDialog *SaveDialogPal;
	TMenuItem *MImport;
	TMenuItem *MCHREditor;
	TOpenDialog *OpenDialogImport;
	TMenuItem *MFile;
	TOpenDialog *OpenDialogAll;
	TMenuItem *MPutSelectedBlockToClipboard;
	TMenuItem *MCHRClear;
	TMenuItem *N1;
	TMenuItem *MCHRRemoveDoubles;
	TMenuItem *N2;
	TMenuItem *N3;
	TSaveDialog *SaveDialogChr;
	TMenuItem *MCHRInterleave;
	TMenuItem *MCHRDeinterleave;
	TMenuItem *MCHRSwapColors;
	TMenuItem *MAddOffset;
	TMenuItem *MExport;
	TMenuItem *MExportNametableBMP;
	TMenuItem *MExportTilesetBMP;
	TSaveDialog *SaveDialogImage;
	TMenuItem *N4;
	TMenuItem *MSaveNameTableASM;
	TMenuItem *MCHRFillNumbers;
	TMenuItem *MCHRFindDoubles;
	TMenuItem *N7;
	TMenuItem *N8;
	TMenuItem *MImportBMPNametable;
	TMenuItem *MImportNES;
	TMenuItem *MExportNES;
	TSaveDialog *SaveDialogExportNES;
	TMenuItem *MImportBMPTileset;
	TMenuItem *MCHRSwapBanks;
	TMenuItem *MOpenAll;
	TMenuItem *MSaveAll;
	TMenuItem *N9;
	TMenuItem *MSaveSession;
	TMenuItem *MLoadSession;
	TOpenDialog *OpenDialogSession;
	TSaveDialog *SaveDialogSession;
	TMenuItem *MExportPaletteBMP;
	TMenuItem *N10;
	TMenuItem *MMetaSprites;
	TMenuItem *MOpenMetaSpriteBank;
	TMenuItem *MSaveMetaSpriteBank;
	TMenuItem *MMetaSpritePutBankToClipboardC;
	TMenuItem *N11;
	TOpenDialog *OpenDialogMetaSpriteBank;
	TSaveDialog *SaveDialogMetaSpriteBank;
	TMenuItem *MMetaSpritePutToClipboardC;
	TMenuItem *MCHRFindUnused;
	TMenuItem *MCHRRemoveUnused;
	TMenuItem *MMetaSpritePutToClipboardAsm;
	TMenuItem *MMetaSpritePutBankToClipboardAsm;
	TMenuItem *MCHRSave1KCursor;
	TMenuItem *MCHRSave2KCursor;
	TMenuItem *MCHRSave4K;
	TMenuItem *MCHRSave8K;
	TMenuItem *MCHRSaveSelection;
	TMenuItem *MPutSelectedBlockToClipboardC;
	TMenuItem *MPutSelectedBlockToClipboardASM;
	TMenuItem *MPutMetaSpriteToClipboardCNoFlip;
	TMenuItem *MPutMetaSpriteToClipboardCHFlip;
	TMenuItem *N5;
	TMenuItem *N13;
	TMenuItem *MCHRSaveLatest;
	TMenuItem *MPutPaletteToClipboard;
	TMenuItem *MPutPaletteToClipboardAssembly;
	TMenuItem *MPutPaletteToClipboardASM;
	TMenuItem *MPutPaletteToClipboardC;
	TMenuItem *MPutSelectedBlockToClipboardCRLE;
	TMenuItem *N14;
	TMenuItem *MPutSelectionToMetasprite;
	TMenuItem *MView;
	TMenuItem *MView2x;
	TMenuItem *MView3x;
	TMenuItem *MView4x;
	TMenuItem *MReplaceTile;
	TPanel *PanelToolbar;
	TGroupBox *GroupBoxTiles;
	TGroupBox *GroupBoxPal;
	TPaintBox *PaintBoxPal;
	TSpeedButton *SpeedButtonPal;
	TLabel *LabelApplyAttr;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label8;
	TLabel *Label9;
	TSpeedButton *SpeedButtonMaskB;
	TSpeedButton *SpeedButtonMaskG;
	TSpeedButton *SpeedButtonMaskR;
	TSpeedButton *SpeedButtonMaskM;
	TPanel *PanelEditArea;
	TPageControl *PageControlEditor;
	TTabSheet *TabSheetName;
	TTabSheet *TabSheetTile;
	TImage *ImageName;
	TImage *ImageBigTiles;
	TTabSheet *TabSheetSprite;
	TGroupBox *GroupBoxSpriteList;
	TSpeedButton *SpeedButtonSpriteDel;
	TSpeedButton *SpeedButtonSpriteUp;
	TSpeedButton *SpeedButtonSpriteDown;
	TSpeedButton *SpeedButtonSpriteHFlip;
	TSpeedButton *SpeedButtonSpriteVFlip;
	TSpeedButton *SpeedButtonSpriteBank;
	TSpeedButton *SpeedButtonSpriteAll;
	TSpeedButton *SpeedButtonSpriteNone;
	TListBox *ListBoxSpriteList;
	TGroupBox *GroupBoxMetaSprite;
	TGroupBox *GroupBoxStats;
	TLabel *LabelStats;
	TPanel *PanelTileset;
	TImage *ImageTiles;
	TPanel *PanelTilesetTools;
	TSpeedButton *SpeedButtonTiles;
	TSpeedButton *SpeedButtonChecker;
	TSpeedButton *SpeedButtonTypeIn;
	TLabel *LabelTypeIn;
	TLabel *LabelAttrChecker;
	TLabel *LabelApplyPatterns;
	TStaticText *StaticTextFontOffset;
	TSpeedButton *SpeedButtonSelTiles;
	TSpeedButton *SpeedButtonGridAll;
	TSpeedButton *SpeedButtonGridTile;
	TSpeedButton *SpeedButtonGridAtr;
	TSpeedButton *SpeedButtonGridBlock;
	TLabel *LabelSelOnly;
	TLabel *Label10;
	TSpeedButton *SpeedButtonChrBank1;
	TSpeedButton *SpeedButtonChrBank2;
	TPanel *PanelSpriteToolbar;
	TSpeedButton *SpeedButtonPrevMetaSprite;
	TLabel *LabelMetaSprite;
	TSpeedButton *SpeedButtonNextMetaSprite;
	TSpeedButton *SpeedButtonSpriteGrid;
	TSpeedButton *SpeedButtonClearMetaSprite;
	TSpeedButton *SpeedButtonSpriteSnap;
	TSpeedButton *SpeedButtonMetaSpriteVFlip;
	TSpeedButton *SpeedButtonMetaSpriteHFlip;
	TSpeedButton *SpeedButtonMetaSpritePaste;
	TSpeedButton *SpeedButtonMetaSpriteCopy;
	TLabel *Label7;
	TSpeedButton *SpeedButtonFrameAll;
	TSpeedButton *SpeedButtonFrameSelected;
	TSpeedButton *SpeedButtonFrameNone;
	TSpeedButton *SpeedButtonSprite8x16;
	TPanel *PanelSpriteView;
	TImage *ImageMetaSprite;
	TMenuItem *MImportBestOffsets;
	TMenuItem *MImportLossy;
	TMenuItem *N15;
	TMenuItem *N16;
	TMenuItem *MImportThreshold;
	TMenuItem *MCHRFreqSort;
	TMenuItem *MPutSelectionToMetaspriteAutoInc;
	TMenuItem *N17;
	TMenuItem *MNameTableNew;
	TMenuItem *MSaveMap;
	TSaveDialog *SaveDialogMap;
	TSpeedButton *SpeedButtonSpriteDup;
	TSpeedButton *SpeedButtonPalBankA;
	TSpeedButton *SpeedButtonPalBankB;
	TSpeedButton *SpeedButtonPalBankC;
	TSpeedButton *SpeedButtonPalBankD;
	TMenuItem *MPutSelectionToMetaspriteSkipZero;
	TMenuItem *MCHRDensitySort;
	TMenuItem *N20;
	TMenuItem *MSelectTilesFromMap;
	TSpeedButton *SpeedButtonMetaSpriteRotate;
	TMenuItem *N22;
	TMenuItem *MMetaSpriteManage;
	TMenuItem *N23;
	TMenuItem *MAddTileOffsetMetaSprites;
	TMenuItem *MImportBMPMatchTiles;
	TMenuItem *N24;
	TMenuItem *MCHRGenerate4x4;
	TMenuItem *MPutSelectionToMetaspriteMerge;
	TMenuItem *MImportBMPIntoNumberOfTiles;
	TMenuItem *MImportNoColorData;
	TMenuItem *MPaletteReset;
	TMenuItem *MPaletteResetGrayscale;
	TMenuItem *MPaletteResetDefaultA;
	TMenuItem *MPaletteResetDefaultB;
	TMenuItem *MPaletteResetDefaultC;
	TMenuItem *MPaletteResetDefaultD;
	TMenuItem *MPaletteCopy;
	TMenuItem *MPalettePaste;
	TTimer *TimerBlock;
	TLabel *LabelMetaSpriteHint;
	TMenuItem *MAddXYOffsetMetaSprites;
	TMenuItem *MCHRSave1KTile00;
	TMenuItem *MCHRSave1KTile40;
	TMenuItem *MCHRSave1KTile80;
	TMenuItem *MCHRSave1KTileC0;
	TMenuItem *MCHRSave2KTile00;
	TMenuItem *MCHRSave2KTile80;
	TMenuItem *N26;
	TMenuItem *N27;
	TMenuItem *MSaveIncName;
	TMenuItem *MSaveIncAttr;
	TMenuItem *MSaveRLE;
	TMenuItem *Edit1;
	TMenuItem *UndoRedo1;
	TMenuItem *Setcheckpoint1;
	TMenuItem *Reverttocheckpoint1;
	TMenuItem *N28;
	TMenuItem *Window1;
	TMenuItem *Tilegrid1;
	TMenuItem *Tilegrid2;
	TMenuItem *Tilegrid4;
	TMenuItem *Attributes1;
	TMenuItem *Generate1;
	TMenuItem *SetEmphasis1;
	TMenuItem *Red1;
	TMenuItem *Green1;
	TMenuItem *Blue1;
	TMenuItem *Gray1;
	TMenuItem *Grid1;
	TMenuItem *asmetasprite1;
	TMenuItem *N29;
	TMenuItem *Copy1;
	TMenuItem *Cut1;
	TMenuItem *Paste1;
	TMenuItem *Metaspritetoclipboard1;
	TMenuItem *Banktoclipboard1;
	TMenuItem *N31;
	TMenuItem *Quit1;
	TMenuItem *N32;
	TMenuItem *Options1;
	TMenuItem *Options2;
	TMenuItem *options3;
	TMenuItem *Savesession1;
	TMenuItem *Draw1;
	TMenuItem *PenMode1;
	TMenuItem *Colour001;
	TMenuItem *Colour011;
	TMenuItem *Colour101;
	TMenuItem *Colour111;
	TMenuItem *Ink2;
	TMenuItem *Subpalette1;
	TMenuItem *Pal0;
	TMenuItem *Pal1;
	TMenuItem *Pal2;
	TMenuItem *Pal3;
	TMenuItem *PenMode2;
	TMenuItem *Advanced1;
	TMenuItem *SafeColours;
	TMenuItem *SharedBGcol;
	TMenuItem *ApplyTiles1;
	TMenuItem *ApplyAttributes1;
	TMenuItem *Toggletileset1;
	TMenuItem *SelectedOnly1;
	TMenuItem *ypeInoffsetat1;
	TMenuItem *MASCIIneg20h;
	TMenuItem *MASCIIneg30h;
	TMenuItem *MASCIIneg40h;
	TMenuItem *classicNESST1;
	TMenuItem *Paletteset1;
	TMenuItem *MPalA;
	TMenuItem *MPalB;
	TMenuItem *MPalC;
	TMenuItem *MPalD;
	TMenuItem *PutcurrenttableonclipboardasBMP1;
	TMenuItem *N35;
	TMenuItem *N12;
	TMenuItem *N36;
	TMenuItem *IncDecPerclick1;
	TMenuItem *IncDecFlow1;
	TMenuItem *IncDecWraparound1;
	TMenuItem *IncDecCap1;
	TMenuItem *N37;
	TMenuItem *IncDecFlow2;
	TMenuItem *IncDecFlow3;
	TMenuItem *IncDecFlow4;
	TMenuItem *IncDecFlow5;
	TMenuItem *OverDistance1;
	TMenuItem *IncDecwetness1;
	TMenuItem *GridOnOff1;
	TGroupBox *GroupBoxLayout;
	TGroupBox *GroupBoxView;
	TMenuItem *N38;
	TSpeedButton *SpeedButtonSubpalCopy;
	TSpeedButton *SpeedButtonSubpalPaste;
	TMenuItem *CopyasBMP1;
	TMenuItem *SelectAll1;
	TMenuItem *N21;
	TMenuItem *Deselect1;
	TStaticText *TextNullTile;
	TMenuItem *CopySelectionasBMP1;
	TMenuItem *Copyspecial1;
	TMenuItem *CopymapasBMP1;
	TMenuItem *CopytilesetasBMP1;
	TMenuItem *N30;
	TMenuItem *Setcarriagereturnpoint1;
	TMenuItem *TypeInModeOnOff1;
	TMenuItem *options4;
	TMenuItem *ingorepastingnulltiles1;
	TMenuItem *N34;
	TMenuItem *Applytopen1;
	TMenuItem *bitmaskoptions1;
	TMenuItem *Applytorotate1;
	TMenuItem *Applytomirror1;
	TMenuItem *Applytonudge1;
	TMenuItem *Applytopaste1;
	TMenuItem *N39;
	TMenuItem *N40;
	TMenuItem *Delete1;
	TTimer *StatusUpdateWaiter;
	TTimer *Throttle;
	TMenuItem *Options5;
	TMenuItem *IncludeNametables1;
	TMenuItem *IncludeMetasprites1;
	TMenuItem *ForceActiveTab1;
	TMenuItem *N41;
	TMenuItem *sortonremoval1;
	TMenuItem *exportoptions1;
	TMenuItem *Noterminator1;
	TMenuItem *Nflagterminator1;
	TMenuItem *Double00terminator1;
	TMenuItem *Spritecountheader1;
	TMenuItem *FFterminator1;
	TMenuItem *Single00terminator1;
	TMenuItem *asmsyntax1;
	TMenuItem *byte1;
	TMenuItem *db1;
	TMenuItem *N42;
	TMenuItem *AskMetaName1;
	TMenuItem *N43;
	TMenuItem *AskBankName1;
	TMenuItem *Generalsettings1;
	TMenuItem *N44;
	TMenuItem *Masterpalette1;
	TMenuItem *Externalnespal1;
	TMenuItem *NESSTclassic1;
	TMenuItem *N45;
	TMenuItem *ClipMetaSpriteAsBMP1;
	TMenuItem *CopyBankasBMP1;
	TMenuItem *xalignment1;
	TMenuItem *center1;
	TMenuItem *left1;
	TMenuItem *right1;
	TMenuItem *yalignment1;
	TMenuItem *center2;
	TMenuItem *top1;
	TMenuItem *bottom1;
	TMenuItem *N46;
	TMenuItem *N47;
	TMenuItem *MetaspriteasBMP1;
	TMenuItem *MetaspritebankasBMPSequence1;
	TMenuItem *origin1;
	TMenuItem *middleofcanvas1;
	TMenuItem *Anchor1;
	TSpeedButton *SBPriorityToggle1;
	TSpeedButton *SBB4;
	TSpeedButton *SBB3;
	TSpeedButton *SBB2;
	TMenuItem *N48;
	TMenuItem *Metaspriteeditorarrangement1;
	TMenuItem *Sprlistl1;
	TMenuItem *Sprlistc1;
	TMenuItem *IncDecbehaviour1;
	TMenuItem *Fill1;
	TMenuItem *N19;
	TTimer *Timer1;
	TTimer *MetaSpriteTimer;
	TSpeedButton *SpeedButtonMarqTile;
	TSpeedButton *SpeedButtonDrawTile;
	TMenuItem *Drawontilesetmodeonoff1;
	TSpeedButton *btnCHRedit;
	TTimer *TimerNTstrip;
	TSpeedButton *SpeedButtonAutocreate;
	TMenuItem *N25;
	TMenuItem *Renamecurrentmetasprite1;
	TMenuItem *Renameallmetasprites1;
	TMenuItem *Clearemphasis1;
	TMenuItem *N49;
	TMenuItem *Putemphasissetonallpalettesets1;
	TSpeedButton *btnHuePlus;
	TSpeedButton *btnHueMinus;
	TSpeedButton *btnHueTiltMinus;
	TSpeedButton *btnHueTiltPlus;
	TSpeedButton *btnBrightPlus;
	TSpeedButton *btnBrightMinus;
	TMenuItem *CHReditortoolbartop;
	TMenuItem *CHReditortoolbarbottom;
	TMenuItem *N50;
	TTimer *NTtimer;
	TTimer *TileTimer;
	TTimer *CHRtimer;
	TMenuItem *N2x2tileeditmode1;
	TMenuItem *N51;
	TMenuItem *N52;
	TMenuItem *Protectcolours1;
	TMenuItem *clearprotection1;
	TMenuItem *Invertprotection1;
	TMenuItem *N18;
	TMenuItem *Applytopen2;
	TMenuItem *Applytopaste2;
	TMenuItem *Lightboxmodetransparentform1;
	TMenuItem *LightboxmodeCHRtransparent1;
	TMenuItem *N54;
	TMenuItem *N53;
	TMenuItem *VisitWeb;
	TMenuItem *Recallcolour1;
	TMenuItem *N55;
	TMenuItem *AutostoreLastUsed;
	TMenuItem *Brushmask1;
	TMenuItem *N56;
	TMenuItem *TogglePenBrush1;
	TMenuItem *Quantized1;
	TMenuItem *Pen1;
	TMenuItem *Colour1;
	TMenuItem *About1;
	TMenuItem *N33;
	TMenuItem *Drag1;
	TMenuItem *RepairActiveTab1;
	TMenuItem *RepairNT1;
	TMenuItem *RepairMSPR1;
	TMenuItem *N57;
	TMenuItem *Swaptablesinselection1;
	TMenuItem *CHRpixelgrid1;
	TMenuItem *N58;
	TMenuItem *N59;
	TMenuItem *AutoViewDragMode1;
	TMenuItem *MapNavigator1;
	TMenuItem *GridRules1;
	TMenuItem *AlwaysNavigator1;
	TMenuItem *MouseNavigator1;
	TMenuItem *AlwaysCanvas1;
	TMenuItem *MouseCanvas1;
	TMenuItem *NeverNavigator1;
	TMenuItem *NeverCanvas1;
	TSpeedButton *SpeedButtonGridScreen;
	TMenuItem *Screen32x301;
	TMenuItem *inNavigator1;
	TMenuItem *onScreencanvas1;
	TMenuItem *MouseButtonNavigator1;
	TMenuItem *ButtonNavigator1;
	TMenuItem *N61;
	TMenuItem *N62;
	TMenuItem *ButtonCanvas1;
	TMenuItem *MouseButtonCanvas1;
	TMenuItem *GridOnOff2;
	TMenuItem *CHR1;
	TMenuItem *OpenCHR1;
	TMenuItem *Latest1;
	TMenuItem *N60;
	TMenuItem *N1Kfromcursor1;
	TMenuItem *N1Kfrom001;
	TMenuItem *N1Kfrom401;
	TMenuItem *N1Kfrom801;
	TMenuItem *N1KfromC01;
	TMenuItem *N63;
	TMenuItem *N2Kfromcursor1;
	TMenuItem *N2Kfrom001;
	TMenuItem *N2Kfrom801;
	TMenuItem *N64;
	TMenuItem *N4Kcurrenttable1;
	TMenuItem *N8Kbothtables1;
	TMenuItem *Selection1;
	TMenuItem *N65;
	TMenuItem *Palettes1;
	TMenuItem *Openascurrentset1;
	TMenuItem *Savecurrentset1;
	TMenuItem *Canvas1;
	TMenuItem *Opencanvasmapornametable1;
	TMenuItem *Saveasmapanysize1;
	TMenuItem *Saveasscreen32x301;
	TMenuItem *SavescreenasASM1;
	TMenuItem *SavescreenasCheader1;
	TMenuItem *options6;
	TMenuItem *includenames1;
	TMenuItem *includeattributes1;
	TMenuItem *forceNESlibRLEpacking1;
	TMenuItem *N66;
	TMenuItem *N67;
	TMenuItem *N69;
	TMenuItem *Metaspritebank1;
	TMenuItem *OpenBank1;
	TMenuItem *Savebank1;
	TMenuItem *N68;
	TMenuItem *NewNEXXTinstance1;
	TMenuItem *N70;
	TMenuItem *N71;
	TMenuItem *Inverttileselection1;
	TMenuItem *PPUdump1;
	TMenuItem *N6;
	TTimer *NameLinesTimer;
	TMenuItem *N72;
	TMenuItem *ImportBitmap1;
	TMenuItem *Findclosestmatch1;
	TMenuItem *N73;
	TMenuItem *SortbyDifference1;
	TMenuItem *Swapattributes1;
	TMenuItem *Clearrogueattributes1;
	TMenuItem *Tilesetmode1;
	TMenuItem *Normal1;
	TMenuItem *N8x16spritemode1;
	TMenuItem *Sortedbyfrequency1;
	TMenuItem *Sortedbydensity1;
	TMenuItem *Sortedbydetail1;
	TMenuItem *N4x12x21;
	TMenuItem *N16tiles4x41;
	TMenuItem *N74;
	TMenuItem *N75;
	TMenuItem *N4x12x2topbottom1;
	TMenuItem *Sortedbycurcol1;
	TMenuItem *UIScale1;
	TMenuItem *Sortedbyedge1;
	TPopupMenu *TileViewPop;
	TMenuItem *Normal2;
	TMenuItem *N8x161;
	TMenuItem *N76;
	TMenuItem *byFrequency1;
	TMenuItem *byDensity1;
	TMenuItem *byDetail1;
	TMenuItem *byEdge1;
	TMenuItem *byCurrentcolour1;
	TMenuItem *N77;
	TMenuItem *N4x12x22;
	TMenuItem *N4x12x2topdown1;
	TMenuItem *N16x14x41;
	TSpeedButton *ButtonTileView;
	TSpeedButton *ButtonForceView;
	TMenuItem *Mirror1;
	TMenuItem *Rearrange1;
	TMenuItem *Find1;
	TMenuItem *Mirrorselection1;
	TMenuItem *Verticalmirror1;
	TMenuItem *Biaxialmirror1;
	TMenuItem *Linetooldetails1;
	TGroupBox *GroupBoxTileControl;
	TSpeedButton *SpeedButtonHFlip;
	TSpeedButton *SpeedButtonVFlip;
	TSpeedButton *SpeedButtonRotateCCW;
	TSpeedButton *SpeedButtonRotateCW;
	TSpeedButton *CHRInc;
	TSpeedButton *CHRDec;
	TSpeedButton *SpeedButton1Up;
	TSpeedButton *SpeedButton1Down;
	TSpeedButton *SpeedButton1Left;
	TSpeedButton *SpeedButton1Right;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label12;
	TSpeedButton *SpeedButtonDoWrap;
	TSpeedButton *ButtonBitmaskLo;
	TSpeedButton *ButtonBitmaskHi;
	TLabel *Label13;
	TSpeedButton *Protect0;
	TSpeedButton *Protect1;
	TSpeedButton *Protect2;
	TSpeedButton *Protect3;
	TLabel *Label14;
	TSpeedButton *btnThick;
	TSpeedButton *btnQuant;
	TSpeedButton *btnLine;
	TSpeedButton *btnSmudge;
	TMenuItem *CHRBankSelector1;
	TMenuItem *CHRcollision1;
	TMenuItem *RepairProps;
	TMenuItem *N78;
	TSaveDialog *SaveDialogTprop;
	TMenuItem *ilecollisionpropertiestprop1;
	TMenuItem *Savepropertiescurrentset1;
	TMenuItem *Savepropertiesbothsets1;
	TMenuItem *Loadproperties1;
	TOpenDialog *OpenDialogTprop;
	TMenuItem *Swappatterncolours1;

	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall PaintBoxPalMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall ImageTilesMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall SpeedButtonGridAllClick(TObject *Sender);
	void __fastcall MCHROpenClick(TObject *Sender);
	void __fastcall PaintBoxNamePaint(TObject *Sender);
	void __fastcall ImageNameMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall ImageNameMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y);
	void __fastcall MOpenNameTableClick(TObject *Sender);
	void __fastcall MSaveNameTableCClick(TObject *Sender);
	void __fastcall MSaveNameTableBINClick(TObject *Sender);
	void __fastcall MPaletteOpenClick(TObject *Sender);
	void __fastcall MPaletteSaveClick(TObject *Sender);
	void __fastcall PaintBoxPalPaint(TObject *Sender);
	void __fastcall MCHREditorClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ImageTilesDblClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ImageNameMouseLeave(TObject *Sender);
	void __fastcall ImageTilesMouseLeave(TObject *Sender);
	void __fastcall ImageTilesMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y);
	void __fastcall MCHRClearClick(TObject *Sender);
	void __fastcall SpeedButtonChrBank1Click(TObject *Sender);
	void __fastcall MCHRRemoveDoublesClick(TObject *Sender);
	void __fastcall PaintBoxPalMouseMove(TObject *Sender, TShiftState Shift, int X,
	int Y);
	void __fastcall PaintBoxPalMouseLeave(TObject *Sender);
	void __fastcall MCHRInterleaveClick(TObject *Sender);
	void __fastcall MCHRDeinterleaveClick(TObject *Sender);
	void __fastcall MCHRSwapColorsClick(TObject *Sender);
	void __fastcall MAddOffsetClick(TObject *Sender);
	void __fastcall SpeedButtonMaskBClick(TObject *Sender);
	void __fastcall SpeedButtonMaskGClick(TObject *Sender);
	void __fastcall SpeedButtonMaskRClick(TObject *Sender);
	void __fastcall SpeedButtonMaskMClick(TObject *Sender);
	void __fastcall MExportNametableBMPClick(TObject *Sender);
	void __fastcall MExportTilesetBMPClick(TObject *Sender);
	void __fastcall SpeedButtonCheckerClick(TObject *Sender);
	void __fastcall MSaveNameTableASMClick(TObject *Sender);
	void __fastcall MCHRFillNumbersClick(TObject *Sender);
	void __fastcall MCHRFindDoublesClick(TObject *Sender);
	void __fastcall MImportBMPNametableClick(TObject *Sender);
	void __fastcall MImportNESClick(TObject *Sender);
	void __fastcall MExportNESClick(TObject *Sender);
	void __fastcall MImportBMPTilesetClick(TObject *Sender);
	void __fastcall MCHRSwapBanksClick(TObject *Sender);
	void __fastcall MOpenAllClick(TObject *Sender);
	void __fastcall MSaveAllClick(TObject *Sender);
	void __fastcall MLoadSessionClick(TObject *Sender);
	void __fastcall MSaveSessionClick(TObject *Sender);
	void __fastcall MExportPaletteBMPClick(TObject *Sender);
	void __fastcall SpeedButtonTypeInClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall PageControlEditorChange(TObject *Sender);
	void __fastcall ImageMetaSpriteDragOver(TObject *Sender, TObject *Source,
	int X, int Y, TDragState State, bool &Accept);
	void __fastcall ImageMetaSpriteDragDrop(TObject *Sender, TObject *Source,
	int X, int Y);
	void __fastcall ImageMetaSpriteEndDrag(TObject *Sender, TObject *Target, int X,
	int Y);
	void __fastcall SpeedButtonPrevMetaSpriteClick(TObject *Sender);
	void __fastcall SpeedButtonNextMetaSpriteClick(TObject *Sender);
	void __fastcall SpeedButtonClearMetaSpriteClick(TObject *Sender);
	void __fastcall SpeedButtonSpriteDelClick(TObject *Sender);
	void __fastcall ListBoxSpriteListClick(TObject *Sender);
	void __fastcall SpeedButtonFrameSelectedClick(TObject *Sender);
	void __fastcall SpeedButtonSpriteUpClick(TObject *Sender);
	void __fastcall SpeedButtonSpriteDownClick(TObject *Sender);
	void __fastcall ListBoxSpriteListKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
	void __fastcall ImageMetaSpriteMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall ImageMetaSpriteMouseLeave(TObject *Sender);
	void __fastcall ImageMetaSpriteMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y);
	void __fastcall ImageMetaSpriteMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall MOpenMetaSpriteBankClick(TObject *Sender);
	void __fastcall MSaveMetaSpriteBankClick(TObject *Sender);
	void __fastcall MMetaSpritePutBankToClipboardCClick(TObject *Sender);
	void __fastcall MMetaSpritePutToClipboardCClick(TObject *Sender);
	void __fastcall SpeedButtonSpriteHFlipClick(TObject *Sender);
	void __fastcall SpeedButtonSpriteVFlipClick(TObject *Sender);
	void __fastcall SpeedButtonMetaSpriteCopyClick(TObject *Sender);
	void __fastcall SpeedButtonMetaSpritePasteClick(TObject *Sender);
	void __fastcall SpeedButtonMetaSpriteHFlipClick(TObject *Sender);
	void __fastcall SpeedButtonMetaSpriteVFlipClick(TObject *Sender);
	void __fastcall MCHRFindUnusedClick(TObject *Sender);
	void __fastcall MCHRRemoveUnusedClick(TObject *Sender);
	void __fastcall SpeedButtonSprite8x16Click(TObject *Sender);
	void __fastcall SpeedButtonSpriteBankClick(TObject *Sender);
	void __fastcall SpeedButtonSpriteGridClick(TObject *Sender);
	void __fastcall MMetaSpritePutToClipboardAsmClick(TObject *Sender);
	void __fastcall MMetaSpritePutBankToClipboardAsmClick(TObject *Sender);
	void __fastcall StaticTextFontOffsetMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall StaticTextFontOffsetMouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y);
	void __fastcall ImageTilesDragOver(TObject *Sender, TObject *Source, int X,
	int Y, TDragState State, bool &Accept);
	void __fastcall ImageTilesDragDrop(TObject *Sender, TObject *Source, int X,
	int Y);
	void __fastcall MCHRSave1KCursorClick(TObject *Sender);
	void __fastcall MCHRSave2KCursorClick(TObject *Sender);
	void __fastcall MCHRSave4KClick(TObject *Sender);
	void __fastcall MCHRSave8KClick(TObject *Sender);
	void __fastcall MCHRSaveSelectionClick(TObject *Sender);
	void __fastcall MPutSelectedBlockToClipboardCClick(TObject *Sender);
	void __fastcall MPutSelectedBlockToClipboardASMClick(TObject *Sender);
	void __fastcall MPutMetaSpriteToClipboardCNoFlipClick(TObject *Sender);
	void __fastcall MPutMetaSpriteToClipboardCHFlipClick(TObject *Sender);
	void __fastcall MCHRSaveLatestClick(TObject *Sender);
	void __fastcall MPutPaletteToClipboardAssemblyClick(TObject *Sender);
	void __fastcall MPutPaletteToClipboardASMClick(TObject *Sender);
	void __fastcall MPutPaletteToClipboardCClick(TObject *Sender);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
	int WheelDelta, TPoint &MousePos, bool &Handled);
	void __fastcall MPutSelectedBlockToClipboardCRLEClick(TObject *Sender);
	void __fastcall MPutSelectionToMetaspriteClick(TObject *Sender);
	void __fastcall SpeedButtonSpriteAllClick(TObject *Sender);
	void __fastcall SpeedButtonSpriteNoneClick(TObject *Sender);
	void __fastcall PaintBoxPalDragOver(TObject *Sender, TObject *Source, int X,
	int Y, TDragState State, bool &Accept);
	void __fastcall PaintBoxPalDragDrop(TObject *Sender, TObject *Source, int X,
	int Y);
	void __fastcall MView2xClick(TObject *Sender);
	void __fastcall MReplaceTileClick(TObject *Sender);
	void __fastcall toggleCheckClick(TObject *Sender);
	void __fastcall MCHRFreqSortClick(TObject *Sender);
	void __fastcall MNameTableNewClick(TObject *Sender);
	void __fastcall MSaveMapClick(TObject *Sender);
	void __fastcall SpeedButtonSpriteDupClick(TObject *Sender);
	void __fastcall SpeedButtonPalBankAClick(TObject *Sender);
	void __fastcall MCHRDensitySortClick(TObject *Sender);
	void __fastcall MSelectTilesFromMapClick(TObject *Sender);
	void __fastcall SpeedButtonMetaSpriteRotateClick(TObject *Sender);
	void __fastcall MMetaSpriteManageClick(TObject *Sender);
	void __fastcall MAddTileOffsetMetaSpritesClick(TObject *Sender);
	void __fastcall MImportBMPMatchTilesClick(TObject *Sender);
	void __fastcall MCHRGenerate4x4Click(TObject *Sender);
	void __fastcall MImportBMPIntoNumberOfTilesClick(TObject *Sender);
	void __fastcall MPaletteResetGrayscaleClick(TObject *Sender);
	void __fastcall MPaletteCopyClick(TObject *Sender);
	void __fastcall MPalettePasteClick(TObject *Sender);
	void __fastcall TimerBlockTimer(TObject *Sender);
	void __fastcall MCHRSave1KTile00Click(TObject *Sender);
	void __fastcall MCHRSave1KTile40Click(TObject *Sender);
	void __fastcall MCHRSave1KTile80Click(TObject *Sender);
	void __fastcall MCHRSave1KTileC0Click(TObject *Sender);
	void __fastcall MCHRSave2KTile00Click(TObject *Sender);
	void __fastcall MCHRSave2KTile80Click(TObject *Sender);
	void __fastcall MAddXYOffsetMetaSpritesClick(TObject *Sender);
	void __fastcall UndoRedo1Click(TObject *Sender);
	void __fastcall Cut1Click(TObject *Sender);
	
	void __fastcall Quit1Click(TObject *Sender);
	void __fastcall Tilegrid1Click(TObject *Sender);
	void __fastcall Colour0Click(TObject *Sender);
	void __fastcall pal0Click(TObject *Sender);
	void __fastcall PenModeClick(TObject *Sender);
	void __fastcall SafeColoursClick(TObject *Sender);
	void __fastcall SharedBGcolClick(TObject *Sender);
	void __fastcall Magnify1Click(TObject *Sender);
	void __fastcall ApplyTiles1Click(TObject *Sender);
	void __fastcall ApplyAttributes1Click(TObject *Sender);
	void __fastcall Toggletileset1Click(TObject *Sender);
	void __fastcall Attributes1Click(TObject *Sender);

	void __fastcall SelectedOnly1Click(TObject *Sender);
	void __fastcall MASCIIneg20hClick(TObject *Sender);
	void __fastcall MPalAClick(TObject *Sender);
	void __fastcall PutcurrenttableonclipboardasBMP1Click(TObject *Sender);
	void __fastcall MCopyMapAsBMP(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Red1Click(TObject *Sender);
	void __fastcall PerClickOrCont(TObject *Sender);
	void __fastcall IncDecCap1Click(TObject *Sender);
	void __fastcall IncDecFlow1Click(TObject *Sender);
	void __fastcall Setcheckpoint1Click(TObject *Sender);
	void __fastcall Reverttocheckpoint1Click(TObject *Sender);
	void __fastcall SpeedButtonSubpalCopyClick(TObject *Sender);
	void __fastcall SpeedButtonSubpalPasteClick(TObject *Sender);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SelectAll1Click(TObject *Sender);
	void __fastcall Deselect1Click(TObject *Sender);
	void __fastcall TextNullTileMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall TextNullTileMouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y);
	void __fastcall TextNullTileMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall Setcarriagereturnpoint1Click(TObject *Sender);
	void __fastcall CopySelectionasBMP1Click(TObject *Sender);
	void __fastcall Delete1Click(TObject *Sender);
	void __fastcall StatusUpdateWaiterTimer(TObject *Sender);
	void __fastcall ThrottleTimer(TObject *Sender);
	void __fastcall ImageNameMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall ImageTilesMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall TypeInModeOnOff1Click(TObject *Sender);
	void __fastcall ForceActiveTab1Click(TObject *Sender);
	void __fastcall IncludeNametables1Click(TObject *Sender);
	void __fastcall IncludeMetasprites1Click(TObject *Sender);
	void __fastcall Savesession1Click(TObject *Sender);
	void __fastcall sortonremoval1Click(TObject *Sender);
	void __fastcall Noterminator1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Externalnespal1Click(TObject *Sender);
	void __fastcall NESSTclassic1Click(TObject *Sender);
	void __fastcall ClipMetaSpriteAsBMP1Click(TObject *Sender);
	void __fastcall CopyBankasBMP1Click(TObject *Sender);
	void __fastcall MetaspritebankasBMPSequence1Click(TObject *Sender);
	void __fastcall MetaspriteasBMP1Click(TObject *Sender);
	void __fastcall SBPriorityToggle1Click(TObject *Sender);
	void __fastcall Sprlistl1Click(TObject *Sender);
	void __fastcall Sprlistc1Click(TObject *Sender);
	void __fastcall Fill1Click(TObject *Sender);
	void __fastcall Generalsettings1Click(TObject *Sender);
	void __fastcall MetaSpriteTimerTimer(TObject *Sender);
	void __fastcall Drawontilesetmodeonoff1Click(TObject *Sender);
	void __fastcall ImageBigTilesMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ImageBigTilesMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall ImageBigTilesMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ImageBigTilesMouseLeave(TObject *Sender);
	void __fastcall ImageBigTilesDragDrop(TObject *Sender, TObject *Source, int X,
          int Y);
	void __fastcall ImageBigTilesDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
	void __fastcall TimerNTstripTimer(TObject *Sender);
	void __fastcall btnCHReditClick(TObject *Sender);
	void __fastcall LabelMetaSpriteDblClick(TObject *Sender);
	void __fastcall Renameallmetasprites1Click(TObject *Sender);
	void __fastcall SpeedButtonAutocreateClick(TObject *Sender);
	void __fastcall Clearemphasis1Click(TObject *Sender);
	void __fastcall Putemphasissetonallpalettesets1Click(TObject *Sender);
	void __fastcall btnHuePlusClick(TObject *Sender);
	void __fastcall btnHueMinusClick(TObject *Sender);
	void __fastcall btnHueTiltMinusClick(TObject *Sender);
	void __fastcall btnHueTiltPlusClick(TObject *Sender);
	void __fastcall SpeedButtonSubpalCopyMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonSubpalCopyMouseLeave(TObject *Sender);
	void __fastcall SpeedButtonSubpalPasteMouseEnter(TObject *Sender);
	void __fastcall btnBrightPlusClick(TObject *Sender);
	void __fastcall btnBrightMinusClick(TObject *Sender);
	void __fastcall btnHueMinusMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonMaskBMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonAutocreateMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonMarqTileMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonDrawTileMouseEnter(TObject *Sender);
	void __fastcall btnCHReditMouseEnter(TObject *Sender);
	void __fastcall TextNullTileMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonChrBank1MouseEnter(TObject *Sender);
	void __fastcall SpeedButtonChrBank2MouseEnter(TObject *Sender);
	void __fastcall SpeedButtonSelTilesMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonCheckerMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonGridAllMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonGridTileMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonTilesMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonPalMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonTypeInMouseEnter(TObject *Sender);
	void __fastcall StaticTextFontOffsetMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonPalBankAMouseEnter(TObject *Sender);
	void __fastcall CHReditortoolbartopClick(TObject *Sender);
	void __fastcall CHReditortoolbarbottomClick(TObject *Sender);
	void __fastcall NTtimerTimer(TObject *Sender);
	void __fastcall TileTimerTimer(TObject *Sender);
	void __fastcall CHRtimerTimer(TObject *Sender);
	void __fastcall N2x2tileeditmode1Click(TObject *Sender);
	void __fastcall clearprotection1Click(TObject *Sender);
	void __fastcall Invertprotection1Click(TObject *Sender);
	void __fastcall SaveDialogMapTypeChange(TObject *Sender);
	void __fastcall Lightboxmodetransparentform1Click(TObject *Sender);
	void __fastcall LightboxmodeCHRtransparent1Click(TObject *Sender);
	void __fastcall VisitWebClick(TObject *Sender);
	void __fastcall Recallcolour1Click(TObject *Sender);
	void __fastcall TogglePenBrush1Click(TObject *Sender);
	void __fastcall Quantized1Click(TObject *Sender);
	void __fastcall Brushmask1Click(TObject *Sender);
	void __fastcall About1Click(TObject *Sender);
	void __fastcall Swaptablesinselection1Click(TObject *Sender);
	void __fastcall ImageTilesEndDrag(TObject *Sender, TObject *Target, int X,
          int Y);
	void __fastcall ImageNameDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
	void __fastcall ImageNameEndDrag(TObject *Sender, TObject *Target, int X,
          int Y);
	void __fastcall ImageNameDragDrop(TObject *Sender, TObject *Source, int X,
          int Y);
	void __fastcall ImageBigTilesDblClick(TObject *Sender);
	void __fastcall CHRpixelgrid1Click(TObject *Sender);
	void __fastcall MapNavigator1Click(TObject *Sender);
	void __fastcall SpeedButtonGridScreenMouseEnter(TObject *Sender);
	void __fastcall ImageNameMouseEnter(TObject *Sender);
	void __fastcall AlwaysNavigator1Click(TObject *Sender);
	void __fastcall TextNullTileDblClick(TObject *Sender);
	void __fastcall MFileClick(TObject *Sender);
	void __fastcall includenames1Click(TObject *Sender);
	void __fastcall MSaveIncNameClick(TObject *Sender);
	void __fastcall MSaveIncAttrClick(TObject *Sender);
	void __fastcall includeattributes1Click(TObject *Sender);
	void __fastcall forceNESlibRLEpacking1Click(TObject *Sender);
	void __fastcall MSaveRLEClick(TObject *Sender);
	void __fastcall NewNEXXTinstance1Click(TObject *Sender);
	void __fastcall Inverttileselection1Click(TObject *Sender);
	void __fastcall PPUdump1Click(TObject *Sender);
	void __fastcall NameLinesTimerTimer(TObject *Sender);
	void __fastcall ImportBitmap1Click(TObject *Sender);
	void __fastcall Findclosestmatch1Click(TObject *Sender);
	void __fastcall SortbyDifference1Click(TObject *Sender);
	void __fastcall Swapattributes1Click(TObject *Sender);
	void __fastcall Clearrogueattributes1Click(TObject *Sender);
	void __fastcall Normal1Click(TObject *Sender);
	void __fastcall ButtonTileViewMouseEnter(TObject *Sender);
	void __fastcall ButtonTileViewMouseLeave(TObject *Sender);
	void __fastcall ButtonTileViewClick(TObject *Sender);
	void __fastcall Mirror1Click(TObject *Sender);
	void __fastcall Linetooldetails1Click(TObject *Sender);
	void __fastcall btnLineMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnLineClick(TObject *Sender);
	void __fastcall SpeedButton1UpClick(TObject *Sender);
	void __fastcall SpeedButton1UpMouseEnter(TObject *Sender);
	void __fastcall Protect0MouseEnter(TObject *Sender);
	void __fastcall Protect0MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall SpeedButtonHFlipClick(TObject *Sender);
	void __fastcall SpeedButtonHFlipMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonVFlipClick(TObject *Sender);
	void __fastcall SpeedButtonRotateCCWClick(TObject *Sender);
	void __fastcall SpeedButtonRotateCWClick(TObject *Sender);
	void __fastcall ButtonBitmaskLoMouseEnter(TObject *Sender);
	void __fastcall SpeedButtonDoWrapMouseEnter(TObject *Sender);
	void __fastcall CHRIncMouseEnter(TObject *Sender);
	void __fastcall CHRIncClick(TObject *Sender);
	void __fastcall btnQuantMouseEnter(TObject *Sender);
	void __fastcall btnThickMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnThickMouseEnter(TObject *Sender);
	void __fastcall btnSmudgeMouseEnter(TObject *Sender);
	void __fastcall btnSmudgeClick(TObject *Sender);
	void __fastcall SpeedButtonDoWrapClick(TObject *Sender);
	void __fastcall Protect0Click(TObject *Sender);
	void __fastcall ButtonBitmaskLoClick(TObject *Sender);
	void __fastcall CHRBankSelector1Click(TObject *Sender);
	void __fastcall CHRcollision1Click(TObject *Sender);
	void __fastcall Savepropertiescurrentset1Click(TObject *Sender);
	void __fastcall Savepropertiesbothsets1Click(TObject *Sender);
	void __fastcall Loadproperties1Click(TObject *Sender);
	void __fastcall Swappatterncolours1Click(TObject *Sender);
	void __fastcall SpeedButtonMarqTileMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpeedButtonMarqTileMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
	void __fastcall SpeedButtonDrawTileMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpeedButtonDrawTileMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
	void __fastcall PaintBoxPalMouseEnter(TObject *Sender);
	void __fastcall ImageBigTilesMouseEnter(TObject *Sender);
	void __fastcall btnLineMouseEnter(TObject *Sender);


private:	// User declarations
public:		// User declarations
	__fastcall TFormMain(TComponent* Owner);
	void __fastcall WorkCHRToBankCHR(void);
    void __fastcall BankCHRToWorkCHR(void);
	void __fastcall pal_validate();
	void __fastcall EnableDisableTypeConflictShortcuts(bool forceDisable);
	void __fastcall DrawPalettes(void);
	void __fastcall DrawCol(int,int,int,int,bool);
	void __fastcall DrawBGPal(int,int,int);
	void __fastcall DrawSelection(TImage*,TRect,int,bool,bool);
	void __fastcall DrawTile(TPicture*,int,int,unsigned int,int,int,int,bool,bool,int,bool,bool,bool);
	void __fastcall DrawTileChecker(TPicture*,int,int,int,int,int,int,bool,bool,int);
	void __fastcall DrawExportTile16(TPicture*,int,int,int,int,int,int,bool);
	void __fastcall UpdateTiles(bool);
	void __fastcall UpdateNameTable(int,int,bool);
	void __fastcall UpdateNameStrip(bool,int);
	void __fastcall CopyCHR(bool,bool);
	void __fastcall PasteCHR(void);
	void __fastcall NameTableScrollLeft(bool);
	void __fastcall NameTableScrollRight(bool);
	void __fastcall NameTableScrollUp(bool);
	void __fastcall NameTableScrollDown(bool);
	bool __fastcall OpenCHR(AnsiString);
    bool __fastcall OpenTprop(AnsiString);
	void __fastcall InterleaveCHR(bool);
	bool __fastcall OpenNameTable(AnsiString);
	bool __fastcall OpenPalette(AnsiString);
	void __fastcall CopyMap(bool);
	void __fastcall CopyMapCodeASM(void);
	void __fastcall CopyMapCodeC(bool rle);
	void __fastcall PasteMap(void);
	void __fastcall FillMap(bool bUseNull);
	void __fastcall GetSelection(TRect,int&,int&,int&,int&);
	void __fastcall OpenAll(AnsiString);
	void __fastcall UpdateStats(void);
	void __fastcall SetUndo(void);
	void __fastcall Undo(void);
	void __fastcall SetTile(int);
	void __fastcall CopyMetaSpriteCodeC(bool);
	void __fastcall SetBMPPalette(Graphics::TBitmap*);
	bool __fastcall LoadSession1x(AnsiString);
	bool __fastcall LoadSession2x(AnsiString filename);
	bool __fastcall LoadSessionText(AnsiString filename);
	bool __fastcall LoadSession(AnsiString);
	void __fastcall SaveSession(AnsiString);

	void __fastcall FindFirstUnused(int &firstUnusedTile, bool &bDidFind);
	void __fastcall SaveConfig();
	bool __fastcall LoadConfig();
	void __fastcall UpdateRGBM(void);
	bool __fastcall MouseTypeIn(int,int);
	void __fastcall NameTableTypeIn(int);
	void __fastcall UpdateMetaSpriteLabel(void);
	void __fastcall UpdateMetaSprite(void);
	void __fastcall DrawSpriteTile(TPicture*,int,int,int,int,TColor,int scale);
	
	void __fastcall MoveSprite(int,int);
	void __fastcall SelectSprite(int,bool);
	void __fastcall SelectTile(int);
	void __fastcall SelectPalette(int);
	bool __fastcall OpenMetaSprites(AnsiString);
	void __fastcall FindDoublesUnused(bool);
	void __fastcall RemoveDoublesUnused(bool);
	int  __fastcall GetSpriteID(int x,int y);
	void __fastcall SpriteSnap(int id);
	AnsiString __fastcall FilterIndexToExt(int index);
	bool __fastcall OverwritePrompt(AnsiString filename);
	bool __fastcall ImportBMP(AnsiString filename,int mode,bool shift,int thresh,int maxtiles,bool noAttr,bool noPal);
	void __fastcall SaveCHR(int offset,int size);
    void __fastcall SaveTprop(int offset,int size);
	void __fastcall SetLatestCHR(int offset,int size);
	void __fastcall DrawEmptyTile(TPicture *pic,int x,int y,int pal,int tx,int ty,bool sel,bool,int inputScale);
    void __fastcall UpdateMenu(void);
	void __fastcall UpdateUIScale(void);
	int  __fastcall ImportConvertTiles(int wdt,int hgt,int xoff,int yoff,int thresh);
	void __fastcall UpdateAll(void);
	void __fastcall ClearNametable(bool);
    bool __fastcall ConfirmNameSelection(void);
	int  __fastcall AttrGet(int x,int y, bool doBuffer, bool returnByte);
	void __fastcall AttrSet(int x,int y,int pal, bool doBuffer);
	void __fastcall CorrectView(void);
	void __fastcall FlipMetaSprites(bool flip_h,bool flip_v);
	void __fastcall MovePaletteCursor(int off);
	void __fastcall DrawMetaSprite(TImage *img,int spr_id,int scale,bool grid,bool frame_all,bool frame_none);

	void __fastcall DrawMetaSpriteExport(TPicture *img,int spr_id,int scale,bool grid,bool frame_all,bool frame_none);
	int __fastcall GetNameTableFrame(void);
	void __fastcall ChangeNameTableFrame(int dir);
	void __fastcall DrawSpriteDot(TPicture *pic,int x,int y,TColor color,int scale);
	void __fastcall MetaSpriteCopy(void);
	void __fastcall MetaSpritePaste(void);
	void __fastcall PaletteCopy(void);
	void __fastcall PalettePaste(void);
	bool __fastcall IsBlockDrawing(void);
	void __fastcall BlockDrawing(bool block);
	void __fastcall SetCheckpoint(void);
	void __fastcall RestoreCheckpoint(void);
	void __fastcall CopyMetasToCB(int i);
	void __fastcall SaveMetasAsBMP(int i, AnsiString str);
	bool __fastcall TestAutoDraw(void);             //int nYC, int nXC
	void __fastcall SetViewTable(int);
	void __fastcall SetViewTable_Destrip(int,bool);
	void __fastcall SetViewTable_SortFreqency(void);
	void __fastcall SetViewTable_SortDensity(void);
	void __fastcall SetViewTable_SortDetail(void);
	void __fastcall SetViewTable_SortEdgeDetail(void);
	void __fastcall SetViewTable_SortActiveColour(void);

	bool BlockDrawingFlag;
	
	//int nameTableWidth;
	//int nameTableHeight;
	AnsiString FormCaption;
	AnsiString globalDir;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
