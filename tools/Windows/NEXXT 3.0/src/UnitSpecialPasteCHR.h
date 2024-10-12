//---------------------------------------------------------------------------

#ifndef UnitSpecialPasteCHRH
#define UnitSpecialPasteCHRH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSpecialPasteCHR : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TRadioButton *RadioAsPattern;
	TRadioButton *RadioAsSilhouette;
	TGroupBox *GroupBox2;
	TRadioButton *RadioSolids;
	TRadioButton *RadioCol0;
	TRadioButton *RadioCustom;
	TGroupBox *GroupBox3;
	TCheckBox *CheckUseMaskB;
	TGroupBox *GroupBox4;
	TCheckBox *CheckUseMaskC;
	TSpeedButton *btn0;
	TSpeedButton *btn1;
	TSpeedButton *btn2;
	TSpeedButton *btn3;
	TRadioButton *RadioAdd;
	TRadioButton *RadioKeepSim;
	TRadioButton *RadioSub;
	TRadioButton *RadioKeepDiff;
	TRadioButton *RadioKeepMask;
	TRadioButton *RadioRemoveMask;
	TRadioButton *RadioAsInvSilhouette;
	TRadioButton *RadioBitOR;
	TRadioButton *RadioBitAND;
	TRadioButton *RadioBitNAND;
	TRadioButton *RadioAdd1;
	TRadioButton *RadioAdd2;
	TRadioButton *RadioAdd3;
	TRadioButton *RadioSub1;
	TRadioButton *RadioSub2;
	TRadioButton *RadioSub3;
	TRadioButton *RadioBehind;
	TRadioButton *RadioOnTop;
	TCheckBox *CheckSubmask;
	void __fastcall RadioSolidsMouseEnter(TObject *Sender);
	void __fastcall RadioCol0MouseEnter(TObject *Sender);
	void __fastcall RadioCustomMouseEnter(TObject *Sender);
	void __fastcall RadioAddMouseEnter(TObject *Sender);
	void __fastcall RadioAdd1MouseEnter(TObject *Sender);
	void __fastcall RadioAdd2MouseEnter(TObject *Sender);
	void __fastcall RadioAdd3MouseEnter(TObject *Sender);
	void __fastcall RadioOnTopMouseEnter(TObject *Sender);
	void __fastcall RadioBitORMouseEnter(TObject *Sender);
	void __fastcall RadioBitANDMouseEnter(TObject *Sender);
	void __fastcall RadioBitNANDClick(TObject *Sender);
	void __fastcall RadioKeepSimMouseEnter(TObject *Sender);
	void __fastcall RadioKeepDiffMouseEnter(TObject *Sender);
	void __fastcall RadioKeepMaskMouseEnter(TObject *Sender);
	void __fastcall RadioRemoveMaskMouseEnter(TObject *Sender);
	void __fastcall CheckUseMaskBMouseEnter(TObject *Sender);
	void __fastcall CheckUseMaskCMouseEnter(TObject *Sender);
	void __fastcall RadioSubMouseEnter(TObject *Sender);
	void __fastcall RadioSub1MouseEnter(TObject *Sender);
	void __fastcall RadioSub2MouseEnter(TObject *Sender);
	void __fastcall RadioSub3Click(TObject *Sender);
	void __fastcall RadioAsPatternMouseEnter(TObject *Sender);
	void __fastcall RadioAsSilhouetteMouseEnter(TObject *Sender);
	void __fastcall RadioAsInvSilhouetteMouseEnter(TObject *Sender);
	void __fastcall CheckSubmaskMouseEnter(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSpecialPasteCHR(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSpecialPasteCHR *FormSpecialPasteCHR;
//---------------------------------------------------------------------------
#endif
