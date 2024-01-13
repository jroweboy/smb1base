//----------------------------------------------------------------------------
#ifndef AboutH
#define AboutH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TAboutBox : public TForm
{
__published:
	TPanel *Panel1;
	TLabel *ProductName;
	TButton *OKButton;
	TStaticText *StaticText1;
	TButton *btnItch;
	TButton *btnCommunity;
	TButton *btnTwitter;
	TButton *btnPatreon;
	TButton *btnShiru;
	TButton *Button1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall btnItchClick(TObject *Sender);
	void __fastcall btnCommunityClick(TObject *Sender);
	void __fastcall btnShiruClick(TObject *Sender);
	void __fastcall btnTwitterClick(TObject *Sender);
	void __fastcall btnPatreonClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:
public:
	virtual __fastcall TAboutBox(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TAboutBox *AboutBox;
//----------------------------------------------------------------------------
#endif    
