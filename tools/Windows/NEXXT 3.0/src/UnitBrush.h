//---------------------------------------------------------------------------

#ifndef UnitBrushH
#define UnitBrushH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>


//---------------------------------------------------------------------------
class TFormBrush : public TForm
{
__published:	// IDE-managed Components
	TPaintBox *PaintBoxMask;
	TTimer *BrushmaskTimer;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TSpeedButton *SpeedButton5;
	TSpeedButton *SpeedButton6;
	TSpeedButton *SpeedButton7;
	TSpeedButton *SpeedButton8;
	TSpeedButton *SpeedButton9;
	TSpeedButton *SpeedButton10;
	TSpeedButton *SpeedButton11;
	TSpeedButton *SpeedButton12;
	TSpeedButton *SpeedButton13;
	TSpeedButton *SpeedButton14;
	TSpeedButton *SpeedButton15;
	TSpeedButton *SpeedButton16;
	TSpeedButton *SpeedButton17;
	TSpeedButton *SpeedButton18;
	TSpeedButton *SpeedButton19;
	TSpeedButton *SpeedButton20;
	TSpeedButton *SpeedButton21;
	TSpeedButton *SpeedButton22;
	TSpeedButton *SpeedButton23;
	TSpeedButton *SpeedButton24;
	TSpeedButton *SpeedButton25;
	TSpeedButton *SpeedButton26;
	TSpeedButton *SpeedButton27;
	TGroupBox *GroupBox3;
	TSpeedButton *SpeedButton28;
	TSpeedButton *SpeedButton30;
	TLabel *Label1;
	TGroupBox *GroupBox4;
	TSpeedButton *SpeedButton29;
	TSpeedButton *SpeedButton31;
	TLabel *Label2;
	TSpeedButton *SpeedButton32;
	TSpeedButton *SpeedButton33;
	TSpeedButton *SpeedButton34;
	TSpeedButton *SpeedButton35;
	TSpeedButton *SpeedButton36;
	TSpeedButton *SpeedButton37;
	TLabel *LabelXY;
	TTimer *StatTimer;
	TPopupMenu *PopupMenu1;
	TMenuItem *Squares1;
	TMenuItem *Filledsquares281;
	TMenuItem *Filledsquares6121;
	TMenuItem *Filledsquares10161;
	TMenuItem *Outlinedsquares281;
	TMenuItem *Outlinedsquares10161;
	TMenuItem *Circles1;
	TMenuItem *N1;
	TMenuItem *Circles281;
	TMenuItem *Circles10161;
	TMenuItem *Circlesoutlined281;
	TMenuItem *Circlesoutlined10161;
	TMenuItem *Diamonds1;
	TMenuItem *Diamondsfilled391;
	TMenuItem *Diamondsfilled10161;
	TMenuItem *Diamondsoutlined391;
	TMenuItem *Diamondsoutlined10161;
	TMenuItem *Rightangleswedges1;
	TMenuItem *Wedges1;
	TMenuItem *N901;
	TMenuItem *N90wedgesfilled6121;
	TMenuItem *N90wedgesfilled10161;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *N90wedgesoutlined281;
	TMenuItem *N90wedgesoutlined10161;
	TSpeedButton *SpeedButton38;
	TSpeedButton *SpeedButton39;
	TLabel *Label3;
	TSpeedButton *SpeedButton40;
	TSpeedButton *SpeedButton41;
	TLabel *Label5;
	TSpeedButton *SpeedButton42;
	TPopupMenu *PopupMenu2;
	TMenuItem *Fromsolidpixels1;
	TMenuItem *Fromcolour01;
	TMenuItem *Frombitplane11;
	TMenuItem *Fromcolour11;
	TMenuItem *Fromcolour12;
	TMenuItem *Fromcolour21;
	TMenuItem *Fromcolour31;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *N8;
	TMenuItem *Circlesjaggy391;
	TMenuItem *Circlesjaggy10161;
	TMenuItem *N9;
	TMenuItem *Diamondsjaggy391;
	TMenuItem *Diamondsjaggy10161;
	TMenuItem *Squaresoutlined7131;
	TMenuItem *Circlesfilled7131;
	TMenuItem *Circlesoutlined7131;
	TMenuItem *Circlesjaggy7131;
	TMenuItem *Diamondsfilled7131;
	TMenuItem *Diamondsoutlined7131;
	TMenuItem *Diamondsjaggy3131;
	TMenuItem *N904545wedgesoutline7131;
	TMenuItem *Roundedsquares1;
	TMenuItem *Roundedsquares4101;
	TMenuItem *Roundedsquaresfilled7131;
	TMenuItem *Roundedsquares10161;
	TMenuItem *N10;
	TMenuItem *Roundedsquaresfilled4101;
	TMenuItem *Roundedsquaresoutlined4101;
	TMenuItem *Roundedsquaresoutlined10161;
	TMenuItem *N11;
	TMenuItem *Roundedsquaresjaggy4101;
	TMenuItem *Roundedsquaresjaggy7131;
	TMenuItem *Roundedsquaresjaggy10161;
	TMenuItem *Linesat7angles8x81;
	TMenuItem *Linesat7angles16x161;
	TMenuItem *N12;
	TMenuItem *Linesat7anglesjaggy8x81;
	TMenuItem *Linesat7anglesjaggy16x161;
	TSpeedButton *SpeedButton43;
	TSpeedButton *SpeedButton44;
	TPopupMenu *PopupMenu3;
	TMenuItem *Currentset2;
	TMenuItem *Doubleset2;
	TMenuItem *Currentsinglebrush2;
	TMenuItem *N13;
	TTimer *TimerRotate1;
	TTimer *TimerRestoreCaption;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall PaintBoxMaskPaint(TObject *Sender);
	void __fastcall PaintBoxMaskMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall PaintBoxMaskMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall BrushmaskTimerTimer(TObject *Sender);
	void __fastcall SpeedButton3Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall SpeedButton30Click(TObject *Sender);
	void __fastcall SpeedButton28Click(TObject *Sender);
	void __fastcall SpeedButton31Click(TObject *Sender);
	void __fastcall SpeedButton29Click(TObject *Sender);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SpeedButton24MouseLeave(TObject *Sender);
	void __fastcall SpeedButton24MouseEnter(TObject *Sender);
	void __fastcall SpeedButton25MouseEnter(TObject *Sender);
	void __fastcall SpeedButton27MouseEnter(TObject *Sender);
	void __fastcall SpeedButton26MouseEnter(TObject *Sender);
	void __fastcall GroupBox2MouseEnter(TObject *Sender);
	void __fastcall SpeedButton32MouseEnter(TObject *Sender);
	void __fastcall SpeedButton30MouseEnter(TObject *Sender);
	void __fastcall SpeedButton28MouseEnter(TObject *Sender);
	void __fastcall SpeedButton31MouseEnter(TObject *Sender);
	void __fastcall SpeedButton29MouseEnter(TObject *Sender);
	void __fastcall SpeedButton3MouseEnter(TObject *Sender);
	void __fastcall StatTimerTimer(TObject *Sender);
	void __fastcall PaintBoxMaskMouseLeave(TObject *Sender);
	void __fastcall SpeedButton32Click(TObject *Sender);
	void __fastcall SpeedButton35Click(TObject *Sender);
	void __fastcall SpeedButton34Click(TObject *Sender);
	void __fastcall SpeedButton33Click(TObject *Sender);
	void __fastcall SpeedButton25Click(TObject *Sender);
	void __fastcall SpeedButton24Click(TObject *Sender);
	void __fastcall SpeedButton26Click(TObject *Sender);
	void __fastcall SpeedButton27Click(TObject *Sender);
	void __fastcall SpeedButton36Click(TObject *Sender);
	void __fastcall SpeedButton37Click(TObject *Sender);
	void __fastcall SpeedButton23Click(TObject *Sender);
	void __fastcall SpeedButton39MouseEnter(TObject *Sender);
	void __fastcall SpeedButton41MouseEnter(TObject *Sender);
	void __fastcall SpeedButton38MouseEnter(TObject *Sender);
	void __fastcall SpeedButton40MouseEnter(TObject *Sender);
	void __fastcall SpeedButton42Click(TObject *Sender);
	void __fastcall Fromsolidpixels1Click(TObject *Sender);
	void __fastcall Filledsquares281Click(TObject *Sender);
	void __fastcall Outlinedsquares281Click(TObject *Sender);
	void __fastcall Circles281Click(TObject *Sender);
	void __fastcall Circlesoutlined281Click(TObject *Sender);
	void __fastcall Circlesjaggy391Click(TObject *Sender);
	void __fastcall Diamondsoutlined391Click(TObject *Sender);
	void __fastcall Diamondsjaggy391Click(TObject *Sender);
	void __fastcall Diamondsfilled391Click(TObject *Sender);
	void __fastcall N901Click(TObject *Sender);
	void __fastcall N90wedgesoutlined281Click(TObject *Sender);
	void __fastcall Roundedsquaresfilled4101Click(TObject *Sender);
	void __fastcall Roundedsquares4101Click(TObject *Sender);
	void __fastcall Roundedsquaresjaggy4101Click(TObject *Sender);
	void __fastcall Linesat7angles8x81Click(TObject *Sender);
	void __fastcall Linesat7anglesjaggy8x81Click(TObject *Sender);
	void __fastcall Currentsinglebrush1Click(TObject *Sender);
	void __fastcall SpeedButton44Click(TObject *Sender);
	void __fastcall SpeedButton43Click(TObject *Sender);
	void __fastcall TimerRotate1Timer(TObject *Sender);
	void __fastcall PaintBoxMaskMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall PaintBoxMaskMouseEnter(TObject *Sender);
	void __fastcall TimerRestoreCaptionTimer(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TFormBrush(TComponent* Owner);
	void __fastcall DrawCell(int,int,int);
	void __fastcall Draw();
	void __fastcall ToggleSpeedButtonByTag(TGroupBox* groupBox, int iTagToToggle, int iGroupIndex, int mode);
    void __fastcall UpdatePaintBoxModulo();
	void __fastcall SetBrushAnchor();
    void __fastcall ChangePreset(int step);
	void __fastcall ShiftLeft(void);
	void __fastcall ShiftRight(void);
	void __fastcall ShiftUp(void);
	void __fastcall ShiftDown(void);
    void __fastcall MakeRotBuffer(void);
	int  __fastcall GetOctant(double angle_rad);
    int  __fastcall GetHextant(double angle_rad);
	int  __fastcall GetSeg24(double angle_rad);
	void __fastcall Rotate3x3(int steps);
	void __fastcall Rotate5x5(int inner, int outer);
	void __fastcall Rotate7x7(int inner, int mid, int outer);
	void __fastcall TurnBitmask_90(size_t d, bool dir);
	void __fastcall TurnBufmask_90(size_t d, bool dir);
	void __fastcall TurnBitmask_45(int cd, bool dir);
	void __fastcall TurnBitmask_180(char* data, int w, int h);
	void __fastcall RetouchBitmask(void);
	void __fastcall SoftenBitmask(void);
	void __fastcall FillBrushMask(int x,int y);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBrush *FormBrush;
//---------------------------------------------------------------------------
#endif
