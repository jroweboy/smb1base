//---------------------------------------------------------------------------

#ifndef UnitEscapeTilesH
#define UnitEscapeTilesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormEscapeTiles : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TSpeedButton *btnMMC2;
	TSpeedButton *btnPreset1;
	TSpeedButton *btnPreset2;
	TGroupBox *GroupBox2;
	TCheckBox *chkTile1;
	TCheckBox *chkTile2;
	TCheckBox *chkTile3;
	TCheckBox *chkTile4;
	TComboBox *ComboBox1;
	TComboBox *ComboBox2;
	TComboBox *ComboBox3;
	TComboBox *ComboBox4;
	TComboBox *ComboBox5;
	TComboBox *ComboBox6;
	TComboBox *ComboBox7;
	TComboBox *ComboBox8;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TGroupBox *GroupBox3;
	TCheckBox *chkDoublePair;
	TCheckBox *chkBG;
	TCheckBox *chkSpr;
	TSpeedButton *btnStore;
	TLabel *Label5;
	TSpeedButton *btnModeOnOff;
	TGroupBox *GroupBox4;
	TRadioButton *RadioDoublePair1st;
	TRadioButton *RadioDoublePair2nd;
	TGroupBox *GroupBox5;
	TRadioButton *RadioUse1stChecked;
	TRadioButton *RadioUseActive;
	TRadioButton *RadioUseA;
	TRadioButton *RadioUseB;
	TRadioButton *RadioUseC;
	TRadioButton *RadioUseD;
	TGroupBox *GroupBox6;
	TRadioButton *RadioUse1stChecked_mt;
	TRadioButton *RadioUseActive_mt;
	TRadioButton *RadioUseA_mt;
	TRadioButton *RadioUseB_mt;
	TRadioButton *RadioUseC_mt;
	TRadioButton *RadioUseD_mt;
	TRadioButton *RadioUseSame_mt;
	TLabel *Label6;
	TLabel *Label7;
	TGroupBox *GroupBox7;
	TRadioButton *Radio1stLine;
	TRadioButton *RadioEveryLine;
	TRadioButton *RadioLastLine;
	TLabel *Label8;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnModeOnOffClick(TObject *Sender);
	void __fastcall chkTile1Click(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall ComboBox5Change(TObject *Sender);
	void __fastcall btnMMC2Click(TObject *Sender);
	void __fastcall chkDoublePairClick(TObject *Sender);
	void __fastcall chkBGClick(TObject *Sender);
	void __fastcall chkSprClick(TObject *Sender);
	void __fastcall btnStoreClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TFormEscapeTiles(TComponent* Owner);
    void __fastcall SetStartConditions(void);
	void __fastcall UpdateConditions(bool allowUpdate);



};
//---------------------------------------------------------------------------
extern PACKAGE TFormEscapeTiles *FormEscapeTiles;
//---------------------------------------------------------------------------
#endif
