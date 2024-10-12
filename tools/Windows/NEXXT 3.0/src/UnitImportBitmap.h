//---------------------------------------------------------------------------

#ifndef UnitImportBitmapH
#define UnitImportBitmapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "UnitSwapColors.h"
#include "UnitLossyDetails.h"
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormImportBMP : public TFormSwapColors
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox6;
	TCheckBox *CheckBestOffsets;
	TCheckBox *CheckLossy;
	TCheckBox *CheckDensityThres;
	TCheckBox *CheckNoAttr;
	TGroupBox *GroupBox7;
	TRadioButton *RadioAsMap;
	TRadioButton *RadioMatched;
	TGroupBox *GroupBox8;
	TEdit *EditPxThres;
	TUpDown *UpDown1;
	TCheckBox *CheckMaxTiles;
	TEdit *EditMaxTiles;
	TUpDown *UpDown2;
	TCheckBox *CheckNoPal;
	TLabel *Label1;
	TButton *Button9;
	TSpeedButton *BtnWdtInc;
	TSpeedButton *SpeedButton1;
	void __fastcall CheckBestOffsetsClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall BtnWdtIncClick(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormImportBMP(TComponent* Owner);
	__fastcall PreviewImport(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormImportBMP *FormImportBMP;
//---------------------------------------------------------------------------
#endif
