//---------------------------------------------------------------------------

#ifndef UnitSwapAttributesH
#define UnitSwapAttributesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "UnitSwapColors.h"
//---------------------------------------------------------------------------
class TFormSwapAttributes : public TFormSwapColors
{
__published:	// IDE-managed Components
	TRadioButton *RadioSpritesAll;
	TRadioButton *RadioSpritesNone;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSwapAttributes(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSwapAttributes *FormSwapAttributes;
//---------------------------------------------------------------------------
#endif
