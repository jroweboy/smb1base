//---------------------------------------------------------------------------

#ifndef UnitNavigatorH
#define UnitNavigatorH
//---------------------------------------------------------------------------


#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TFormNavigator : public TForm
{
__published:	// IDE-managed Components
	TImage *Map1;
	TTimer *ResizeTimer;
	TTimer *CueDrawTimer;
	TTimer *CueLinesTimer;
	TTimer *CorrectNT;
	TTimer *CueChunkDraw;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Map1DblClick(TObject *Sender);
	void __fastcall ResizeTimerTimer(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight,
          bool &Resize);
	void __fastcall CueDrawTimerTimer(TObject *Sender);
	void __fastcall Map1MouseEnter(TObject *Sender);
	void __fastcall Map1MouseLeave(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Map1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Map1MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall Map1EndDrag(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall Map1DragOver(TObject *Sender, TObject *Source, int X, int Y,
          TDragState State, bool &Accept);
	void __fastcall Map1DragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall CueLinesTimerTimer(TObject *Sender);
	void __fastcall CorrectNTTimer(TObject *Sender);
	void __fastcall CueChunkDrawTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormNavigator(TComponent* Owner);

	void __fastcall Draw(bool,bool);
	void __fastcall DrawRange(int tx,int ty,int tw,int th, bool repaint);
	void __fastcall UpdateLines(bool getBuffer);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormNavigator *FormNavigator;
//---------------------------------------------------------------------------
#endif
