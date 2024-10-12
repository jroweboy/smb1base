//---------------------------------------------------------------------------

#ifndef MetatileManagerH
#define MetatileManagerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMetatileEditor : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TImage *Image1;
	TLabel *Label2;
	TImage *Image2;
	TGroupBox *GroupBox2;
	TSpeedButton *Insert1;
	TSpeedButton *Remove1;
	TSpeedButton *Duplicate1;
	TSpeedButton *Up1;
	TSpeedButton *Down1;
	TSpeedButton *Clear1;
	TSpeedButton *Copy1;
	TSpeedButton *Paste1;
	TSpeedButton *SpeedButton1;
	TGroupBox *GroupBox3;
	TPageControl *PageControl1;
	TTabSheet *TabSheet2x2;
	TTabSheet *TabSheet4x4;
	TListBox *ListBox1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton5;
	TSpeedButton *SpeedButton6;
	TSpeedButton *SpeedButton4;
	TSpeedButton *SpeedButton7;
	TSpeedButton *SpeedButton8;
	TSpeedButton *SpeedButton9;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label4;
	TListBox *ListBox2;
	TSpeedButton *SpeedButton10;
	TLabel *Label5;
private:	// User declarations
public:		// User declarations
	__fastcall TMetatileEditor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMetatileEditor *MetatileEditor;
//---------------------------------------------------------------------------
#endif
