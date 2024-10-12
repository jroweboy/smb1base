//---------------------------------------------------------------------------

#ifndef UnitSubpaletteLibraryH
#define UnitSubpaletteLibraryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSubpaletteLibrary1 : public TForm
{
__published:	// IDE-managed Components
	TScrollBox *ScrollBox1;
	TPaintBox *PaintBox1;
	TGroupBox *GroupBox1;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
private:	// User declarations
public:		// User declarations
	__fastcall TSubpaletteLibrary1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSubpaletteLibrary1 *SubpaletteLibrary1;
//---------------------------------------------------------------------------
#endif
