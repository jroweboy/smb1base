//---------------------------------------------------------------------------

#ifndef UnitBankCHRH
#define UnitBankCHRH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormBankCHR : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox1;
	TImage *Image1;
	TImage *Image2;
	TSpeedButton *btnA;
	TSpeedButton *btnB;
	TSpeedButton *btn4k;
	TSpeedButton *btn1k;
	TSpeedButton *btn2k;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TLabel *Label1;
	TLabel *Label2;
	TSpeedButton *Insert1;
	TSpeedButton *Remove1;
	TSpeedButton *Duplicate1;
	TSpeedButton *Up1;
	TSpeedButton *Down1;
	TSpeedButton *Clear1;
	TSpeedButton *btn512b;
	TSpeedButton *btn256b;
	TTimer *DrawTimer;
	TSpeedButton *Copy1;
	TSpeedButton *Paste1;
	TSpeedButton *SpeedButton1;
	TTimer *OpenByFileAssociationMakeListTimer;
	TSpeedButton *btnC;
	TSpeedButton *btnD;
	TSpeedButton *btnFree;
	TSpeedButton *SpeedButton5;
	TSpeedButton *SpeedButton2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Insert1Click(TObject *Sender);
	void __fastcall btnAClick(TObject *Sender);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Image2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btn4kClick(TObject *Sender);
	void __fastcall btn2kClick(TObject *Sender);
	void __fastcall Image1MouseEnter(TObject *Sender);
	void __fastcall Image1MouseLeave(TObject *Sender);
	void __fastcall Image2MouseLeave(TObject *Sender);
	void __fastcall Image2MouseEnter(TObject *Sender);
	void __fastcall Image2MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall DrawTimerTimer(TObject *Sender);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall Clear1Click(TObject *Sender);
	void __fastcall Up1Click(TObject *Sender);
	void __fastcall Down1Click(TObject *Sender);
	void __fastcall Remove1Click(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall ListBox1MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall OpenByFileAssociationMakeListTimerTimer(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Copy1Click(TObject *Sender);
	void __fastcall Paste1Click(TObject *Sender);
	void __fastcall Insert1MouseEnter(TObject *Sender);
	void __fastcall Clear1MouseEnter(TObject *Sender);
	void __fastcall Duplicate1MouseEnter(TObject *Sender);
	void __fastcall Up1MouseEnter(TObject *Sender);
	void __fastcall Down1MouseEnter(TObject *Sender);
	void __fastcall SpeedButton1MouseEnter(TObject *Sender);
	void __fastcall Copy1MouseEnter(TObject *Sender);
	void __fastcall Paste1MouseEnter(TObject *Sender);
	void __fastcall Remove1MouseEnter(TObject *Sender);
	void __fastcall Insert1MouseLeave(TObject *Sender);
	void __fastcall btn4kMouseEnter(TObject *Sender);
	void __fastcall btn2kMouseEnter(TObject *Sender);
	void __fastcall btn1kMouseEnter(TObject *Sender);
	void __fastcall btn512bMouseEnter(TObject *Sender);
	void __fastcall btn256bMouseEnter(TObject *Sender);
	void __fastcall SpeedButton5Click(TObject *Sender);
	void __fastcall SpeedButton5MouseEnter(TObject *Sender);
	void __fastcall btnFreeClick(TObject *Sender);
	void __fastcall btnFreeMouseEnter(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall SpeedButton2MouseEnter(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormBankCHR(TComponent* Owner);
    void __fastcall Update(void);
	void __fastcall Draw(void);
	void __fastcall MakeList(bool bSelectTop, bool bInit);


	};
//---------------------------------------------------------------------------
extern PACKAGE TFormBankCHR *FormBankCHR;
//---------------------------------------------------------------------------
#endif
