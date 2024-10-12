//---------------------------------------------------------------------------

#ifndef UnitAttributeCheckerOptionsH
#define UnitAttributeCheckerOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormAttrChecker : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TTrackBar *TrackBar1;
	TSpeedButton *SpeedButton5;
	TLabel *Label1;
	TLabel *Label2;
	TGroupBox *GroupBox3;
	TLabel *Label3;
	TLabel *Label4;
	TTrackBar *TrackBar2;
	TGroupBox *GroupBox4;
	TLabel *Label5;
	TSpeedButton *SpeedButton6;
	TSpeedButton *SpeedButton7;
	TPopupMenu *PopupMenu1;
	TMenuItem *Resetpresets1;
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TrackBar1Change(TObject *Sender);
	void __fastcall UpDown1Changing(TObject *Sender, bool &AllowChange);
	void __fastcall TrackBar2Change(TObject *Sender);
	void __fastcall SpeedButton6Click(TObject *Sender);
	void __fastcall SpeedButton7Click(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Resetpresets1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormAttrChecker(TComponent* Owner);
	void __fastcall ResetPresets(bool doRefresh);
	void __fastcall UpdateUI(void);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormAttrChecker *FormAttrChecker;
//---------------------------------------------------------------------------
#endif
