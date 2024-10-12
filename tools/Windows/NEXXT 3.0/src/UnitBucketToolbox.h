//---------------------------------------------------------------------------

#ifndef UnitBucketToolboxH
#define UnitBucketToolboxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormBucketToolbox : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TSpeedButton *btnClassic;
	TSpeedButton *btnForgiving;
	TLabel *Label1;
	TSpeedButton *btnSides;
	TSpeedButton *btnSemis;
	TSpeedButton *btnDiags;
	TLabel *Label2;
	TSpeedButton *btnFields;
	TSpeedButton *btnGaps;
	TSpeedButton *btnBoth;
	TLabel *Label4;
	TSpeedButton *btnGapPen;
	TSpeedButton *SpeedButton12;
	TGroupBox *GroupBox3;
	TSpeedButton *SpeedButton13;
	TSpeedButton *SpeedButton14;
	TLabel *Label3;
	TSpeedButton *SpeedButton15;
	TSpeedButton *SpeedButton16;
	TGroupBox *GroupBox4;
	TSpeedButton *btn4way;
	TSpeedButton *btn8way;
	TLabel *Label5;
	TSpeedButton *btnFieldPen;
	TSpeedButton *SpeedButton10;
	TSpeedButton *btnCustomway;
	TGroupBox *GroupBox5;
	TSpeedButton *btnC_nw;
	TSpeedButton *btnSwitchDir;
	TSpeedButton *btnC_n;
	TSpeedButton *btnC_ne;
	TSpeedButton *btnC_w;
	TSpeedButton *btnC_e;
	TSpeedButton *btnC_sw;
	TSpeedButton *btnC_s;
	TSpeedButton *btnC_se;
	TLabel *Label6;
	TSpeedButton *btnSmartAll;
	TSpeedButton *btnSmartCustom;
	TSpeedButton *SpeedButton24;
	TSpeedButton *SpeedButton25;
	TSpeedButton *SpeedButton27;
	TSpeedButton *SpeedButton26;
	TSpeedButton *SpeedButton5;
	TSpeedButton *btnForceBuf;
	void __fastcall SpeedButton15MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnC_nwMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnC_nMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnC_neMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnC_wMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnC_eMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnC_swMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnC_sMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnC_seMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall SpeedButton5MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpeedButton27MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpeedButton26MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpeedButton25MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpeedButton24MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnClassicMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnForgivingMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnSwitchDirMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnClassicMouseEnter(TObject *Sender);
	void __fastcall btnForgivingMouseEnter(TObject *Sender);
	void __fastcall btn4wayMouseEnter(TObject *Sender);
	void __fastcall btn8wayMouseEnter(TObject *Sender);
	void __fastcall btnCustomwayMouseEnter(TObject *Sender);
	void __fastcall btnFieldPenMouseEnter(TObject *Sender);
	void __fastcall SpeedButton10MouseEnter(TObject *Sender);
	void __fastcall btnSidesMouseEnter(TObject *Sender);
	void __fastcall btnSemisMouseEnter(TObject *Sender);
	void __fastcall btnDiagsMouseEnter(TObject *Sender);
	void __fastcall btnFieldsMouseEnter(TObject *Sender);
	void __fastcall btnGapsMouseEnter(TObject *Sender);
	void __fastcall btnBothMouseEnter(TObject *Sender);
	void __fastcall btnSmartAllMouseEnter(TObject *Sender);
	void __fastcall btnSmartCustomMouseEnter(TObject *Sender);
	void __fastcall btnGapPenMouseEnter(TObject *Sender);
	void __fastcall SpeedButton12MouseEnter(TObject *Sender);
	void __fastcall btnForceBufMouseEnter(TObject *Sender);
	void __fastcall SpeedButton5MouseEnter(TObject *Sender);
	void __fastcall SpeedButton27MouseEnter(TObject *Sender);
	void __fastcall SpeedButton26MouseEnter(TObject *Sender);
	void __fastcall SpeedButton24MouseEnter(TObject *Sender);
	void __fastcall SpeedButton25MouseEnter(TObject *Sender);
	void __fastcall btnSwitchDirMouseEnter(TObject *Sender);
	void __fastcall btnC_nwMouseEnter(TObject *Sender);
	void __fastcall btnC_nMouseEnter(TObject *Sender);
	void __fastcall btnC_neMouseEnter(TObject *Sender);
	void __fastcall btnC_wMouseEnter(TObject *Sender);
	void __fastcall btnC_eMouseEnter(TObject *Sender);
	void __fastcall btnC_swMouseEnter(TObject *Sender);
	void __fastcall btnC_sMouseEnter(TObject *Sender);
	void __fastcall btnC_seMouseEnter(TObject *Sender);
	void __fastcall btnSidesClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormBucketToolbox(TComponent* Owner);
	void __fastcall GetBucketDirA(void);
    void __fastcall GetBucketDirB(void);
	void __fastcall PutBucketDirA(void);
	void __fastcall PutBucketDirB(void);
	void __fastcall PutBucketDirections(void);
	void __fastcall RotateCW(void);
	void __fastcall RotateCCW(void);
	void __fastcall ToggleFillMode(void);
	void __fastcall ToggleFloodMode(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBucketToolbox *FormBucketToolbox;
//---------------------------------------------------------------------------
#endif
