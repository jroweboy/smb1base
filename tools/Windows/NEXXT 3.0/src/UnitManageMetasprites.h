//---------------------------------------------------------------------------

#ifndef UnitManageMetaspritesH
#define UnitManageMetaspritesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormManageMetasprites : public TForm
{
__published:	// IDE-managed Components
	TSpeedButton *SpeedButtonMoveUp;
	TSpeedButton *SpeedButtonMoveDown;
	TSpeedButton *SpeedButtonInsert;
	TSpeedButton *SpeedButtonRemove;
	TSpeedButton *SpeedButtonDuplicate;
	TImage *ImageMetaSprite;
	TSpeedButton *SpeedButtonCopy;
	TSpeedButton *SpeedButtonPaste;
	TEdit *durationNTSC;
	TUpDown *UpDownNTSC;
	TUpDown *UpDownPAL;
	TEdit *durationPAL;
	TCheckBox *chkLink;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TGroupBox *GroupBox1;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TRadioButton *RadioSkip;
	TRadioButton *RadioHold;
	TGroupBox *GroupBox2;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TSpeedButton *btnRelease;
	TRadioButton *PlayNTSC;
	TRadioButton *PlayPAL;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TCheckBox *CheckMoveLabels;
	TMaskEdit *MetaLabel;
	TCheckBox *CheckMoveDurations;
	TRadioButton *RadioStart;
	TRadioButton *RadioNone;
	TRadioButton *RadioLoop;
	TRadioButton *RadioCall;
	TGroupBox *GroupBox5;
	TListBox *ListBoxSprites;
	TSpeedButton *btnGrid;
	TSpeedButton *btnShowTags;
	TSpeedButton *btnShowNTSC;
	TSpeedButton *btnShowPAL;
	TSpeedButton *btnShowLabels;
	TSpeedButton *btnBox;
	TSpeedButton *btnWarn;
	TSpeedButton *btnSil;
	TLabel *Label9;
	TSpeedButton *btnShowCount;
	TSpeedButton *SpeedButton5;
	TSpeedButton *SpeedButton7;
	TTimer *TimerFrameTick;
	TCheckBox *chkAdjust;
	TRadioButton *RadioX1;
	TRadioButton *RadioX2;
	TRadioButton *RadioSteady;
	TLabel *Label8;
	TTimer *TimerSkipDelay;
	TGroupBox *GroupBox6;
	TLabel *Label10;
	TLabel *Label11;
	TEdit *EditStepDur;
	TUpDown *UpDownDur;
	TCheckBox *chkLoopDurationStep;
	TCheckBox *chkValidDuration;
	TLabel *Label13;
	TCheckBox *CheckMoveTags;
	TSpeedButton *btnZoom;
	TSpeedButton *SpeedButton6;
	TTimer *TimerPan;
	TLabel *Label7;
	TCheckBox *chkValidTag;
	TSpeedButton *btnShowOrder;
	TCheckBox *CheckMoveSprites;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ListBoxSpritesClick(TObject *Sender);
	void __fastcall SpeedButtonInsertClick(TObject *Sender);
	void __fastcall SpeedButtonRemoveClick(TObject *Sender);
	void __fastcall SpeedButtonMoveUpClick(TObject *Sender);
	void __fastcall SpeedButtonMoveDownClick(TObject *Sender);
	void __fastcall SpeedButtonDuplicateClick(TObject *Sender);
	void __fastcall SpeedButtonCopyClick(TObject *Sender);
	void __fastcall SpeedButtonPasteClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall MetaLabelKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall MetaLabelClick(TObject *Sender);
	void __fastcall MetaLabelEnter(TObject *Sender);
	void __fastcall MetaLabelExit(TObject *Sender);
	void __fastcall FormDeactivate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ListBoxSpritesDblClick(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall RadioStartClick(TObject *Sender);
	void __fastcall RadioCallClick(TObject *Sender);
	void __fastcall RadioLoopClick(TObject *Sender);
	void __fastcall RadioNoneClick(TObject *Sender);
	void __fastcall btnShowLabelsClick(TObject *Sender);
	void __fastcall btnGridClick(TObject *Sender);
	void __fastcall btnShowLabelsMouseLeave(TObject *Sender);
	void __fastcall SpeedButton3MouseEnter(TObject *Sender);
	void __fastcall SpeedButton4MouseEnter(TObject *Sender);
	void __fastcall SpeedButton5MouseEnter(TObject *Sender);
	void __fastcall SpeedButton2MouseEnter(TObject *Sender);
	void __fastcall SpeedButton1MouseEnter(TObject *Sender);
	void __fastcall btnShowLabelsMouseEnter(TObject *Sender);
	void __fastcall btnShowNTSCMouseEnter(TObject *Sender);
	void __fastcall btnShowPALMouseEnter(TObject *Sender);
	void __fastcall btnShowCountMouseEnter(TObject *Sender);
	void __fastcall btnShowTagsMouseEnter(TObject *Sender);
	void __fastcall RadioNoneMouseEnter(TObject *Sender);
	void __fastcall RadioStartMouseEnter(TObject *Sender);
	void __fastcall SpeedButton7Click(TObject *Sender);
	void __fastcall ListBoxSpritesDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
	void __fastcall TimerFrameTickTimer(TObject *Sender);
	void __fastcall SpeedButton3Click(TObject *Sender);
	void __fastcall SpeedButton4Click(TObject *Sender);
	void __fastcall ListBoxSpritesMouseEnter(TObject *Sender);
	void __fastcall durationNTSCKeyPress(TObject *Sender, char &Key);
	void __fastcall durationPALKeyPress(TObject *Sender, char &Key);
	void __fastcall durationPALClick(TObject *Sender);
	void __fastcall durationNTSCClick(TObject *Sender);
	void __fastcall durationNTSCExit(TObject *Sender);
	void __fastcall durationPALExit(TObject *Sender);
	void __fastcall durationNTSCEnter(TObject *Sender);
	void __fastcall durationPALEnter(TObject *Sender);
	void __fastcall ListBoxSpritesKeyPress(TObject *Sender, char &Key);
	void __fastcall ListBoxSpritesEnter(TObject *Sender);
	void __fastcall ListBoxSpritesExit(TObject *Sender);
	void __fastcall durationPALKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall durationNTSCKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall chkLinkMouseEnter(TObject *Sender);
	void __fastcall chkAdjustMouseEnter(TObject *Sender);
	void __fastcall RadioX1MouseEnter(TObject *Sender);
	void __fastcall RadioX2MouseEnter(TObject *Sender);
	void __fastcall RadioSteadyMouseEnter(TObject *Sender);
	void __fastcall btnReleaseClick(TObject *Sender);
	void __fastcall UpDownNTSCClick(TObject *Sender, TUDBtnType Button);
	void __fastcall UpDownPALClick(TObject *Sender, TUDBtnType Button);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall TimerSkipDelayTimer(TObject *Sender);
	void __fastcall SpeedButton5Click(TObject *Sender);
	void __fastcall EditStepDurExit(TObject *Sender);
	void __fastcall EditStepDurKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall EditStepDurKeyPress(TObject *Sender, char &Key);
	void __fastcall ImageMetaSpriteMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ImageMetaSpriteMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
	void __fastcall ImageMetaSpriteMouseEnter(TObject *Sender);
	void __fastcall ImageMetaSpriteMouseLeave(TObject *Sender);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
	void __fastcall SpeedButton6Click(TObject *Sender);
	void __fastcall ImageMetaSpriteMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TimerPanTimer(TObject *Sender);
	void __fastcall UpDownDurClick(TObject *Sender, TUDBtnType Button);
	void __fastcall ListBoxSpritesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ListBoxSpritesMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ListBoxSpritesMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
	void __fastcall ListBoxSpritesKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TFormManageMetasprites(TComponent* Owner);

	
	void __fastcall Update(void);
	void __fastcall UpdateActive(bool doBitmap);
	void __fastcall MetaLabelToClip(bool cut);
	void __fastcall ClipToMetaLabel(void);
	void __fastcall UpdateOneListItem(int id);
	void __fastcall AssignFrame(bool refetch);
	void __fastcall DisplayZoomLevel(void);
};


//---------------------------------------------------------------------------
extern PACKAGE TFormManageMetasprites *FormManageMetasprites;
//---------------------------------------------------------------------------
#endif
