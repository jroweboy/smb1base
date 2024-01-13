//---------------------------------------------------------------------------

#ifndef UnitLineDetailsH
#define UnitLineDetailsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormLineDetails : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TSpeedButton *btnTaper2;
	TSpeedButton *btnTaperIn;
	TSpeedButton *btnTaperOut;
	TGroupBox *GroupBox2;
	TSpeedButton *btnDots;
	TSpeedButton *btnDashes;
	TTrackBar *TrkDash;
	TLabel *Label1;
	TGroupBox *GroupBox3;
	TSpeedButton *btnRosterLine;
	TSpeedButton *btnRosterCurve;
	TSpeedButton *btnRosterKnee;
	TSpeedButton *btnRosterAngle;
	TSpeedButton *btnResetLine;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *LineToolIndicator;
	TLabel *LabelDashLen;
	TCheckBox *CheckEnableBrush;
	TGroupBox *GroupBox4;
	TSpeedButton *btnSmear;
	TSpeedButton *btnQuick;
	TGroupBox *GroupBox5;
	TSpeedButton *SpeedButton3;
	TCheckBox *CheckResetLineNudge;
	TLabel *Label4;
	TSpeedButton *btnMove;
	void __fastcall btnRosterLineClick(TObject *Sender);
	void __fastcall btnRosterLineMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TrkDashChange(TObject *Sender);
	void __fastcall btnTaperInClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SpeedButton3Click(TObject *Sender);
	void __fastcall btnTaperInMouseEnter(TObject *Sender);
	void __fastcall btnTaperInMouseLeave(TObject *Sender);
	void __fastcall btnTaperOutMouseEnter(TObject *Sender);
	void __fastcall btnTaper2MouseEnter(TObject *Sender);
	void __fastcall CheckEnableBrushMouseEnter(TObject *Sender);
	void __fastcall btnSmearMouseEnter(TObject *Sender);
	void __fastcall btnMoveMouseEnter(TObject *Sender);
	void __fastcall SpeedButton3MouseEnter(TObject *Sender);
	void __fastcall CheckResetLineNudgeMouseEnter(TObject *Sender);
	void __fastcall btnDotsMouseEnter(TObject *Sender);
	void __fastcall btnDashesMouseEnter(TObject *Sender);
	void __fastcall Label1MouseEnter(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormLineDetails(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLineDetails *FormLineDetails;
//---------------------------------------------------------------------------
#endif
