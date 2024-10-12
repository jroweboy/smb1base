//---------------------------------------------------------------------------

#ifndef UnitMTpropsH
#define UnitMTpropsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormMTprops : public TForm
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
	TPaintBox *PaintBox1;
	TLabel *Label5;
	TSpeedButton *btnC0;
	TSpeedButton *btnC1;
	TSpeedButton *btnC2;
	TSpeedButton *btnC3;
	TSpeedButton *btnC4;
	TSpeedButton *btnC5;
	TSpeedButton *btnC6;
	TSpeedButton *btnC7;
	TMaskEdit *MaskEdit1;
	TTimer *BitBtnTimer;
	TPopupMenu *PopupMenu1;
	TGroupBox *GroupBox1;
	TSpeedButton *btnDirect;
	TSpeedButton *btn1x1meta;
	TSpeedButton *btn2x1meta;
	TSpeedButton *btn2x2meta;
	TSpeedButton *btn4x2meta;
	TSpeedButton *btn4x4meta;
	void __fastcall btn0labelClick(TObject *Sender);
	void __fastcall PaintBox1Paint(TObject *Sender);
	void __fastcall btn1labelClick(TObject *Sender);
	void __fastcall btn2labelClick(TObject *Sender);
	void __fastcall btn3labelClick(TObject *Sender);
	void __fastcall btn4labelClick(TObject *Sender);
	void __fastcall btn5labelClick(TObject *Sender);
	void __fastcall btn6labelClick(TObject *Sender);
	void __fastcall btn7labelClick(TObject *Sender);
	void __fastcall btn0MouseEnter(TObject *Sender);
	void __fastcall btn0MouseLeave(TObject *Sender);
	void __fastcall btn0MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btn0Click(TObject *Sender);
	void __fastcall btn1x1metaClick(TObject *Sender);
	void __fastcall btn2x1metaClick(TObject *Sender);
	void __fastcall btn2x2metaClick(TObject *Sender);
	void __fastcall btn4x2metaClick(TObject *Sender);
	void __fastcall btn4x4metaClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDeactivate(TObject *Sender);
	void __fastcall btnC0MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall MaskEdit1MouseEnter(TObject *Sender);
	void __fastcall MaskEdit1MouseLeave(TObject *Sender);
	void __fastcall MaskEdit1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall MaskEdit1Enter(TObject *Sender);
	void __fastcall MaskEdit1Exit(TObject *Sender);
	void __fastcall MaskEdit1KeyPress(TObject *Sender, char &Key);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnDirectClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    void __fastcall HexToClip(void);
    void __fastcall ClipToHexEdit(void);
	void __fastcall UpdateBitButtons_tileClick(bool);
	void __fastcall UpdateBitButtons_metasetClick(void);
	void __fastcall UpdateBitButtons_metatileClick(void);

	__fastcall TFormMTprops(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMTprops *FormMTprops;
//---------------------------------------------------------------------------
#endif
