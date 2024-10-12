//---------------------------------------------------------------------------

#ifndef UnitNESBankH
#define UnitNESBankH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormBank : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBoxInfo;
	TImage *ImageCHR1;
	TImage *ImageCHR2;
	TButton *ButtonOK;
	TButton *ButtonCancel;
	TLabel *LabelInfo;
	TEdit *EditBank;
	TUpDown *UpDownBank;
	TLabel *Label1;
	TLabel *Label2;
	TGroupBox *GroupBox1;
	TSpeedButton *btnA;
	TSpeedButton *btnB;
	TSpeedButton *btnC;
	TSpeedButton *btnD;
	TGroupBox *GroupBox2;
	TSpeedButton *btnGray;
	TSpeedButton *btnPal0;
	TSpeedButton *btnPal1;
	TSpeedButton *btnPal2;
	TSpeedButton *btnPal3;
	TSpeedButton *btnDiffCheck;
	TSpeedButton *btnInverse;
	TTrackBar *TrackBar1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall EditBankChange(TObject *Sender);
	void __fastcall btnAClick(TObject *Sender);
	void __fastcall btnGrayClick(TObject *Sender);
	void __fastcall btnDiffCheckClick(TObject *Sender);
	void __fastcall TrackBar1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormBank(TComponent* Owner);
	void __fastcall DrawBank(TPicture*,unsigned char*,unsigned char*,bool);
	void __fastcall WrongFile(void);
	void __fastcall ShowBank(void);
	AnsiString FileName;
	int PRG,CHR,Bank;
	bool OK;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBank *FormBank;
//---------------------------------------------------------------------------
#endif
