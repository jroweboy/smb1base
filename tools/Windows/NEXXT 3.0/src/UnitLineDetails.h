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
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormLineDetails : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TSpeedButton *btnTaperFromMid;
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
	TSpeedButton *btnRosterHyperline;
	TSpeedButton *btnRosterHyperbola;
	TSpeedButton *btnRosterHypercave;
	TSpeedButton *btnAutoSizeHyperY;
	TLabel *Label5;
	TLabel *Label6;
	TSpeedButton *btnRosterRectangle;
	TGroupBox *GroupBox6;
	TSpeedButton *btnPresetA;
	TSpeedButton *btnPresetB;
	TSpeedButton *btnPresetC;
	TSpeedButton *btnPresetD;
	TSpeedButton *btnRosterEllipse;
	TTimer *LinePresetSaveTimer;
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
	void __fastcall btnTaperFromMidMouseEnter(TObject *Sender);
	void __fastcall CheckEnableBrushMouseEnter(TObject *Sender);
	void __fastcall btnSmearMouseEnter(TObject *Sender);
	void __fastcall btnMoveMouseEnter(TObject *Sender);
	void __fastcall SpeedButton3MouseEnter(TObject *Sender);
	void __fastcall CheckResetLineNudgeMouseEnter(TObject *Sender);
	void __fastcall btnDotsMouseEnter(TObject *Sender);
	void __fastcall btnDashesMouseEnter(TObject *Sender);
	void __fastcall Label1MouseEnter(TObject *Sender);
	void __fastcall btnPresetAClick(TObject *Sender);
	void __fastcall btnDotsClick(TObject *Sender);
	void __fastcall btnDashesClick(TObject *Sender);
	void __fastcall btnTaperFromMidClick(TObject *Sender);
	void __fastcall btnQuickClick(TObject *Sender);
	void __fastcall CheckResetLineNudgeClick(TObject *Sender);
	void __fastcall btnResetLineClick(TObject *Sender);
	void __fastcall btnAutoSizeHyperYClick(TObject *Sender);
	void __fastcall LinePresetSaveTimerTimer(TObject *Sender);
	void __fastcall btnQuickMouseEnter(TObject *Sender);
	void __fastcall btnSmearClick(TObject *Sender);
	void __fastcall btnPresetAMouseEnter(TObject *Sender);
	void __fastcall btnPresetBMouseEnter(TObject *Sender);
	void __fastcall btnPresetCMouseEnter(TObject *Sender);
	void __fastcall btnPresetDMouseEnter(TObject *Sender);
	void __fastcall btnAutoSizeHyperYMouseEnter(TObject *Sender);
	void __fastcall btnResetLineMouseEnter(TObject *Sender);
	void __fastcall GroupBox3MouseEnter(TObject *Sender);
	void __fastcall btnRosterLineMouseEnter(TObject *Sender);
	void __fastcall btnRosterCurveMouseEnter(TObject *Sender);
	void __fastcall btnRosterKneeMouseEnter(TObject *Sender);
	void __fastcall btnRosterAngleMouseEnter(TObject *Sender);
	void __fastcall btnRosterRectangleMouseEnter(TObject *Sender);
	void __fastcall btnRosterEllipseMouseEnter(TObject *Sender);
	void __fastcall btnRosterHyperbolaMouseEnter(TObject *Sender);
	void __fastcall btnRosterHyperlineMouseEnter(TObject *Sender);
	void __fastcall btnRosterHypercaveMouseEnter(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TFormLineDetails(TComponent* Owner);
	void __fastcall GetPreset(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLineDetails *FormLineDetails;
//---------------------------------------------------------------------------
#endif
