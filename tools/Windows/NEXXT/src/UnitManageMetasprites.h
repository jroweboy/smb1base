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
//---------------------------------------------------------------------------
class TFormManageMetasprites : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBoxSprites;
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
	TCheckBox *CheckBox1;
	TLabel *Label1;
	TLabel *Label2;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TRadioButton *RadioButton3;
	TLabel *Label3;
	TGroupBox *GroupBox1;
	TLabel *Label4;
	TRadioButton *RadioButton4;
	TLabel *Label5;
	TLabel *Label6;
	TRadioButton *RadioButton5;
	TRadioButton *RadioButton6;
	TGroupBox *GroupBox2;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TSpeedButton *SpeedButton5;
	TSpeedButton *SpeedButton6;
	TRadioButton *PlayNTSC;
	TRadioButton *PlayPAL;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TCheckBox *CheckMoveName;
	TMaskEdit *MetaLabel;
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
private:	// User declarations
public:		// User declarations
	__fastcall TFormManageMetasprites(TComponent* Owner);

	void __fastcall Update(void);
	void __fastcall UpdateActive(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormManageMetasprites *FormManageMetasprites;
//---------------------------------------------------------------------------
#endif
