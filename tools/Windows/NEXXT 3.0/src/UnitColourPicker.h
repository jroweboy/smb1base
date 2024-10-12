//---------------------------------------------------------------------------

#ifndef UnitColourPickerH
#define UnitColourPickerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormColourPicker : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TTimer *TimerPassePartout;
	TTimer *TimerHighlightSubpal;
	TTimer *TimerRefresh;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall Image1MouseLeave(TObject *Sender);
	void __fastcall TimerPassePartoutTimer(TObject *Sender);
	void __fastcall TimerHighlightSubpalTimer(TObject *Sender);
	void __fastcall FormMouseEnter(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormMouseLeave(TObject *Sender);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
	void __fastcall TimerRefreshTimer(TObject *Sender);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TFormColourPicker(TComponent* Owner);
    TColor __fastcall  HSLToRGB(float H, float S, float L);
    double __fastcall  ColourDistance(TColor c1, TColor c2);
	void __fastcall  DrawHSLColourWheel(Graphics::TBitmap *Bitmap);
	void __fastcall  DrawPassePartout(Graphics::TBitmap *Bitmap);
    void __fastcall DrawSubpalHighlight(Graphics::TBitmap *Bitmap);
	void __fastcall  DrawInvertButton(Graphics::TBitmap *Bitmap);
    void __fastcall  DrawMatteButton(Graphics::TBitmap *Bitmap);

	void __fastcall Draw(void);
	void __fastcall RefreshBuf(bool forceNoPasse);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormColourPicker *FormColourPicker;
//---------------------------------------------------------------------------
#endif
