//---------------------------------------------------------------------------

#ifndef UnitPipelineHelperH
#define UnitPipelineHelperH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormPipelineHelper : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TCheckBox *CheckBox1;
	TLabel *Label1;
	TGroupBox *GroupBox2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TLabel *Label2;
	TCheckBox *CheckBox2;
	TGroupBox *GroupBox3;
	TCheckBox *chkAutoExpMetatilesBMP;
	TGroupBox *GroupBox4;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TSpeedButton *btnType;
	TSpeedButton *btnName;
	TOpenDialog *OpenDialogPath;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox4;
	TSpeedButton *btnAsset;
	TRadioButton *RadioButton3;
	TSpeedButton *btnRefreshCHRlink;
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall CheckBox2Click(TObject *Sender);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall SpeedButton4Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall CheckBox4Click(TObject *Sender);
	void __fastcall CheckBox1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall CheckBox2MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall CheckBox2MouseEnter(TObject *Sender);
	void __fastcall CheckBox1MouseEnter(TObject *Sender);
	void __fastcall CheckBox1MouseLeave(TObject *Sender);
	void __fastcall CheckBox4MouseEnter(TObject *Sender);
	void __fastcall chkAutoExpMetatilesBMPMouseEnter(TObject *Sender);
	void __fastcall RadioButton1MouseEnter(TObject *Sender);
	void __fastcall RadioButton2MouseEnter(TObject *Sender);
	void __fastcall btnAssetMouseEnter(TObject *Sender);
	void __fastcall btnNameMouseEnter(TObject *Sender);
	void __fastcall btnTypeMouseEnter(TObject *Sender);
	void __fastcall RadioButton3MouseEnter(TObject *Sender);
	void __fastcall btnRefreshCHRlinkMouseEnter(TObject *Sender);
	void __fastcall btnRefreshCHRlinkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormPipelineHelper(TComponent* Owner);
    void __fastcall UpdateUI(void);
	void __fastcall HandleLoadedLinkedMode(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPipelineHelper *FormPipelineHelper;
//---------------------------------------------------------------------------
#endif
