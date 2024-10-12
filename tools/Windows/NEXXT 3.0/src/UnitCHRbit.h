//---------------------------------------------------------------------------

#ifndef UnitCHRbitH
#define UnitCHRbitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormCHRbit : public TForm
{
__published:	// IDE-managed Components
	TSpeedButton *btn0;
	TSpeedButton *btn1;
	TSpeedButton *btn2;
	TSpeedButton *btn3;
	TSpeedButton *btn4;
	TSpeedButton *btn5;
	TSpeedButton *btn6;
	TSpeedButton *btn7;
	TSpeedButton *btn0label;
	TSpeedButton *btn1label;
	TSpeedButton *btn2label;
	TSpeedButton *btn4label;
	TSpeedButton *btn3label;
	TSpeedButton *btn5label;
	TSpeedButton *btn6label;
	TSpeedButton *btn7label;
	TLabel *Label3;
	TLabel *Label4;
	TSpeedButton *btnHold;
	TMaskEdit *MaskEdit1;
	TPaintBox *PaintBox1;
	TTimer *BitBtnTimer;
	TLabel *Label5;
	TSpeedButton *btnC0;
	TSpeedButton *btnC1;
	TSpeedButton *btnC2;
	TSpeedButton *btnC3;
	TSpeedButton *btnC4;
	TSpeedButton *btnC5;
	TSpeedButton *btnC6;
	TSpeedButton *btnC7;
	TPopupMenu *PopupMenu1;
	TGroupBox *GroupBox1;
	TSpeedButton *btn1x1;
	TSpeedButton *btn2x1;
	TSpeedButton *btn2x2;
	TSpeedButton *btn4x2;
	TSpeedButton *btn4x4;
	TGroupBox *GroupBox2;
	TSpeedButton *btnShowScreen;
	TSpeedButton *btnShowCHR;
	TSpeedButton *btnShowMeta;
	TSpeedButton *btnShowNav;
	void __fastcall btn6labelClick(TObject *Sender);
	void __fastcall btn7labelClick(TObject *Sender);
	void __fastcall btn5labelClick(TObject *Sender);
	void __fastcall btn4labelClick(TObject *Sender);
	void __fastcall btn3labelClick(TObject *Sender);
	void __fastcall btn2labelClick(TObject *Sender);
	void __fastcall btn1labelClick(TObject *Sender);
	void __fastcall btn0labelClick(TObject *Sender);
	void __fastcall btn0Click(TObject *Sender);
	void __fastcall btn0MouseEnter(TObject *Sender);
	void __fastcall btn0MouseLeave(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall MaskEdit1KeyPress(TObject *Sender, char &Key);
	void __fastcall MaskEdit1Exit(TObject *Sender);
	void __fastcall PaintBox1Paint(TObject *Sender);
	void __fastcall MaskEdit1MouseEnter(TObject *Sender);
	void __fastcall MaskEdit1MouseLeave(TObject *Sender);
	void __fastcall BitBtnTimerTimer(TObject *Sender);
	void __fastcall btnC0MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnHoldClick(TObject *Sender);
	void __fastcall MaskEdit1Enter(TObject *Sender);
	void __fastcall MaskEdit1Click(TObject *Sender);
	void __fastcall FormDeactivate(TObject *Sender);
	void __fastcall btnHoldMouseEnter(TObject *Sender);
	void __fastcall btnHoldMouseLeave(TObject *Sender);
	void __fastcall btnShowCHRMouseEnter(TObject *Sender);
	void __fastcall btnShowScreenMouseEnter(TObject *Sender);
	void __fastcall btn0MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall MaskEdit1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall btn1x1Click(TObject *Sender);
	void __fastcall btn2x1Click(TObject *Sender);
	void __fastcall btn2x2Click(TObject *Sender);
	void __fastcall btn4x2Click(TObject *Sender);
	void __fastcall btn4x4Click(TObject *Sender);
	void __fastcall btn1x1MouseEnter(TObject *Sender);
	void __fastcall btn2x1MouseEnter(TObject *Sender);
	void __fastcall btn2x2MouseEnter(TObject *Sender);
	void __fastcall btn4x2MouseEnter(TObject *Sender);
	void __fastcall btn4x4MouseEnter(TObject *Sender);
	void __fastcall btnShowMetaMouseEnter(TObject *Sender);
	void __fastcall btnShowNavClick(TObject *Sender);
	void __fastcall btnShowMetaClick(TObject *Sender);
	void __fastcall btnShowScreenClick(TObject *Sender);
	void __fastcall btnShowCHRClick(TObject *Sender);
	void __fastcall btnShowNavMouseEnter(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormCHRbit(TComponent* Owner);
	void __fastcall UpdateBitButtons(bool bID_listClick);
    void __fastcall HexToClip(void);
    void __fastcall ClipToHexEdit(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCHRbit *FormCHRbit;
//---------------------------------------------------------------------------
#endif
