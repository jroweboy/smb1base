//---------------------------------------------------------------------------

#ifndef UnitTileBucketExtrasH
#define UnitTileBucketExtrasH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormTileBucketExtras : public TForm
{
__published:	// IDE-managed Components
	TCheckBox *CheckAlignToSel;
	TCheckBox *CheckSubpaletteAware;
	TGroupBox *GroupBox6;
	TSpeedButton *btnSetMaxReach;
	TTrackBar *TrkReach;
	TCheckBox *chkAutoCustom;
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TrkReachChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CheckAlignToSelMouseEnter(TObject *Sender);
	void __fastcall CheckSubpaletteAwareMouseEnter(TObject *Sender);
	void __fastcall btnSetMaxReachMouseEnter(TObject *Sender);
	void __fastcall chkAutoCustomMouseEnter(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormTileBucketExtras(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTileBucketExtras *FormTileBucketExtras;
//---------------------------------------------------------------------------
#endif
