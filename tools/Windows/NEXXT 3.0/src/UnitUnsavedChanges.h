//---------------------------------------------------------------------------

#ifndef UnitUnsavedChangesH
#define UnitUnsavedChangesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormUnsavedChanges : public TForm
{
__published:	// IDE-managed Components
	TStaticText *StaticText1;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
private:	// User declarations
public:		// User declarations
	__fastcall TFormUnsavedChanges(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormUnsavedChanges *FormUnsavedChanges;
//---------------------------------------------------------------------------
#endif
