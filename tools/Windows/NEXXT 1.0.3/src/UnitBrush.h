//---------------------------------------------------------------------------

#ifndef UnitBrushH
#define UnitBrushH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormBrush : public TForm
{
__published:	// IDE-managed Components
	TPaintBox *PaintBoxMask;
	TTimer *BrushmaskTimer;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall PaintBoxMaskPaint(TObject *Sender);
	void __fastcall PaintBoxMaskMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall PaintBoxMaskMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall BrushmaskTimerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormBrush(TComponent* Owner);
	void __fastcall DrawCell(int,int,int);
	void __fastcall Draw();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBrush *FormBrush;
//---------------------------------------------------------------------------
#endif
