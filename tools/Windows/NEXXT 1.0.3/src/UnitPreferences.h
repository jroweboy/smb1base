//----------------------------------------------------------------------------
#ifndef UnitPreferencesH
#define UnitPreferencesH
//----------------------------------------------------------------------------
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
//----------------------------------------------------------------------------
class TFormPreferences : public TForm
{
__published:
	TPanel *Panel1;
	TPanel *Panel2;
	TButton *OKBtn;
	TButton *CancelBtn;
	TButton *HelpBtn;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TGroupBox *RGroupScale;
	TRadioButton *RadioScale2x;
	TRadioButton *RadioScale3x;
	TRadioButton *RadioScale4x;
	TGroupBox *RGroupColour;
	TRadioButton *RadioCol0;
	TRadioButton *RadioCol1;
	TRadioButton *RadioCol2;
	TRadioButton *RadioCol3;
	TGroupBox *RGroupSubpal;
	TRadioButton *RadioSubpal0;
	TRadioButton *RadioSubpal1;
	TRadioButton *RadioSubpal2;
	TRadioButton *RadioSubpal3;
	TGroupBox *RGroupGrid;
	TRadioButton *RadioGridHide;
	TRadioButton *RadioGridShow;
	TCheckBox *CheckGrid1;
	TCheckBox *CheckGrid2;
	TCheckBox *CheckGrid4;
	TCheckBox *CheckGridPixelCHR;
	TGroupBox *GroupShowWindow;
	TCheckBox *CheckShowCHREditor;
	TCheckBox *CheckShowMetaspriteManager;
	TTabSheet *TabSheet2;
	TGroupBox *GroupBox3;
	TCheckBox *CheckAutostoreLastUsed;
	TGroupBox *GroupBitmask;
	TCheckBox *CheckBitmaskPen;
	TCheckBox *CheckBitmaskMirror;
	TCheckBox *CheckBitmaskRotate;
	TCheckBox *CheckBitmaskNudge;
	TCheckBox *CheckBitmaskPaste;
	TGroupBox *GroupRules;
	TCheckBox *CheckRules0F;
	TCheckBox *CheckRulesSharedBG;
	TGroupBox *RGroupASCIIBase;
	TRadioButton *RadioASCIIneg20;
	TRadioButton *RadioASCIIneg30;
	TRadioButton *RadioASCIIneg40;
	TGroupBox *GroupFindUnused;
	TCheckBox *CheckFindUnusedForce;
	TCheckBox *CheckFindUnusedName;
	TCheckBox *CheckFindUnusedMeta;
	TGroupBox *GroupRemoveFound;
	TCheckBox *CheckRemoveFoundSort;
	TGroupBox *RGroupInkLimit;
	TRadioButton *RadioInkLimitCap;
	TRadioButton *RadioInkLimitWrap;
	TGroupBox *RGroupInkBehaviour;
	TRadioButton *RadioInkBehaviourClick;
	TRadioButton *RadioInkBehaviourDistance;
	TGroupBox *RGroupInkFlow;
	TRadioButton *RadioInkFlowQuickest;
	TRadioButton *RadioInkFlowQuick;
	TRadioButton *RadioInkFlowMedium;
	TRadioButton *RadioInkFlowSlow;
	TRadioButton *RadioInkFlowSlowest;
	TTabSheet *TabSheet3;
	TGroupBox *GroupBMPAsName;
	TCheckBox *CheckBMPBestOffsets;
	TCheckBox *CheckBMPLossy;
	TCheckBox *CheckBMPThres;
	TCheckBox *CheckBMPNoColour;
	TTabSheet *TabSheet4;
	TGroupBox *RGroupSpriteMetadata;
	TRadioButton *RadioNoHeader;
	TRadioButton *RadioSpriteCount;
	TRadioButton *RadioNflag;
	TRadioButton *RadioFFTerminator;
	TRadioButton *RadioSingle00;
	TRadioButton *RadioDouble00;
	TGroupBox *GroupAskName;
	TCheckBox *CheckAskSprName;
	TCheckBox *CheckAskBankName;
	TGroupBox *GroupAsmSyntax;
	TRadioButton *RadioAsmByte;
	TRadioButton *RadioAsmDb;
	TGroupBox *GroupBox1;
	TCheckBox *CheckIncludeNames;
	TCheckBox *CheckIncludeAttributes;
	TCheckBox *CheckRLECompress;
	TTabSheet *TabSheet5;
	TGroupBox *GroupBox4;
	TCheckBox *CheckAutoshowDrag;
	TGroupBox *GroupBox5;
	TCheckBox *CheckMsprYellow;
	TCheckBox *CheckMsprOrange;
	TCheckBox *CheckMsprRed;
	TGroupBox *GroupBox6;
	TRadioButton *RadioNavScrAlways;
	TRadioButton *RadioNavScrMouse;
	TRadioButton *RadioNavScrMB;
	TRadioButton *RadioNavScrButton;
	TRadioButton *RadioNavScrNever;
	TGroupBox *GroupBox7;
	TRadioButton *RadioMainScrAlways;
	TRadioButton *RadioMainScrMouse;
	TRadioButton *RadioMainScrMB;
	TRadioButton *RadioMainScrButton;
	TRadioButton *RadioMainScrNever;
	TTabSheet *TabSheet6;
	TGroupBox *RGroupWorkspace;
	TRadioButton *RadioSprlistLeft;
	TRadioButton *RadioSprlistCenter;
	TGroupBox *GroupBox2;
	TRadioButton *RadioToolTop;
	TRadioButton *RadioToolBottom;
	TGroupBox *GroupBox8;
	TGroupBox *GroupBox9;
	TTrackBar *TrackBarAlpha;
	TCheckBox *CheckGrid32x30;
	TCheckBox *CheckFormToMonitor;
	TGroupBox *GroupBox10;
	TRadioButton *RadioOpenSave1;
	TRadioButton *RadioOpenSave2;
	TRadioButton *RadioOpenSave3;
	TGroupBox *GroupBox11;
	TCheckBox *CheckExportPalFilename;
	TCheckBox *CheckExportPalSet;
	void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall HelpBtnClick(TObject *Sender);
private:
public:
	virtual __fastcall TFormPreferences(TComponent* AOwner);
	void __fastcall GetStartPreferences();
};
//----------------------------------------------------------------------------
extern PACKAGE TFormPreferences *FormPreferences;
//----------------------------------------------------------------------------
#endif    
