//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"
#include "UnitMain.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//--------------------------------------------------------------------- 
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TAboutBox::FormShow(TObject *Sender)
{
	ProductName->Caption=Application->Title;
	StaticText1->Caption="Build date: April 18th, 2023.\n\n\n\n\n\n\nDeveloped by FrankenGraphics.\nBased on NESST; developed by Shiru.\n\nThis software is Public Domain.\n";
	StaticText1->Width=200;
	StaticText1->Height=146;
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::OKButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::btnItchClick(TObject *Sender)
{
	ShellExecute(NULL, "open", "https://frankengraphics.itch.io/nexxt", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::btnCommunityClick(TObject *Sender)
{
  ShellExecute(NULL, "open", "https://frankengraphics.itch.io/nexxt/community", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::btnShiruClick(TObject *Sender)
{
ShellExecute(NULL, "open", "https://shiru.untergrund.net/index.shtml", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::btnTwitterClick(TObject *Sender)
{
	ShellExecute(NULL, "open", "https://twitter.com/FrankenGraphics", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::btnPatreonClick(TObject *Sender)
{
	ShellExecute(NULL, "open", "https://www.patreon.com/frankengraphics", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::Button1Click(TObject *Sender)
{
	ShellExecute(NULL, "open", "https://mastodon.art/@FrankenGraphics", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

