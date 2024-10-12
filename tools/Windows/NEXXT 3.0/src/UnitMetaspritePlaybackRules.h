//---------------------------------------------------------------------------

#ifndef UnitMetaspritePlaybackRulesH
#define UnitMetaspritePlaybackRulesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormMetaspritePlaybackRules : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TGroupBox *GroupBox2;
	TStaticText *StaticText1;
	TGroupBox *GroupBox3;
	TRadioButton *RadioDontCap;
	TRadioButton *Radio63Cap;
	TRadioButton *Radio127Cap;
	TRadioButton *Radio255Cap;
	TCheckBox *chkReplaceCall1;
	TCheckBox *chkReplaceCall2;
	TComboBox *ComboBoxCall1;
	TComboBox *ComboBoxCall2;
	void __fastcall RadioButton1Click(TObject *Sender);
	void __fastcall RadioButton2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall RadioDontCapClick(TObject *Sender);
	void __fastcall chkReplaceCall1MouseEnter(TObject *Sender);
	void __fastcall RadioButton2MouseEnter(TObject *Sender);
	void __fastcall RadioButton1MouseEnter(TObject *Sender);
	void __fastcall ComboBoxCall1Change(TObject *Sender);
	void __fastcall ComboBoxCall2Change(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall chkReplaceCall1Click(TObject *Sender);
	void __fastcall chkReplaceCall2Click(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TFormMetaspritePlaybackRules(TComponent* Owner);
	void __fastcall TextA(void);
    void __fastcall TextB(void);
	void __fastcall TextC(void);
	void __fastcall UpdateCallText(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMetaspritePlaybackRules *FormMetaspritePlaybackRules;
//---------------------------------------------------------------------------
#endif
