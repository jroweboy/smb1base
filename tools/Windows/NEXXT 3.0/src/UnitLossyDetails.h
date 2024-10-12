//---------------------------------------------------------------------------

#ifndef UnitLossyDetailsH
#define UnitLossyDetailsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormLossyDetails : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox4;
	TCheckBox *CheckBox5;
	TCheckBox *CheckBox6;
	TTrackBar *TrackBar1;
	TTrackBar *TrackBar2;
	TTrackBar *TrackBar3;
	TTrackBar *TrackBar4;
	TTrackBar *TrackBar5;
	TTrackBar *TrackBar6;
	TGroupBox *GroupBox3;
	TButton *Button1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton1;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TGroupBox *GroupBox4;
	TEdit *Edit1;
	TUpDown *UpDown1;
	TGroupBox *GroupBox5;
	TGroupBox *GroupBox6;
	TButton *Button2;
	TGroupBox *GroupBox8;
	TCheckBox *CheckBox7;
	TCheckBox *CheckBox8;
	TGroupBox *GroupBox7;
	TTrackBar *TrackBar7;
	TLabel *Label1;
	TLabel *Label2;
	TSpeedButton *SpeedButton4;
	TCheckBox *CheckBox9;
	TCheckBox *CheckBox10;
	void __fastcall RadioFreqClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormLossyDetails(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLossyDetails *FormLossyDetails;
//---------------------------------------------------------------------------
#endif
