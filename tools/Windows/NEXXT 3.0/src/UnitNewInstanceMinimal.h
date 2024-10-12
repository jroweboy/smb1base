//---------------------------------------------------------------------------

#ifndef UnitNewInstanceMinimalH
#define UnitNewInstanceMinimalH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormNewInstanceMinimal : public TForm
{
__published:	// IDE-managed Components
	TButton *btnOK;
	TGroupBox *GroupBox1;
	TSpeedButton *BtnWdtInc;
	TEdit *EditWidth;
	TSpeedButton *BtnWdtDec;
	TUpDown *UpDownWidth;
	TSpeedButton *BtnHgtInc;
	TEdit *EditHeight;
	TSpeedButton *BtnHgtDec;
	TUpDown *UpDownHeight;
	TGroupBox *GroupBox2;
	TButton *Button1;
	TRadioButton *RadioNew;
	TRadioButton *RadioClone;
	TSpeedButton *Btn32x30;
	TSpeedButton *BtnThisSession;
	TGroupBox *GroupBoxIfNew;
	TCheckBox *chkInheritSubpal;
	TCheckBox *chkInheritCHR;
	TCheckBox *chkInheritMap;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnWdtIncClick(TObject *Sender);
	void __fastcall BtnWdtDecClick(TObject *Sender);
	void __fastcall BtnHgtIncClick(TObject *Sender);
	void __fastcall BtnHgtDecClick(TObject *Sender);
	void __fastcall BtnThisSessionClick(TObject *Sender);
	void __fastcall Btn32x30Click(TObject *Sender);
	void __fastcall chkInheritMapClick(TObject *Sender);
	void __fastcall RadioCloneClick(TObject *Sender);
	void __fastcall RadioNewClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormNewInstanceMinimal(TComponent* Owner);
	void __fastcall SizeEnableDisable(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormNewInstanceMinimal *FormNewInstanceMinimal;
//---------------------------------------------------------------------------
#endif
