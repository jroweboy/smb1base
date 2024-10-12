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
	StaticText1->Caption="Build date: October 10th, 2024.\n\nDeveloped by FrankenGraphics.\nBased on NESST; developed by Shiru.\n\nThis software is Public Domain, excluding FreeImage library whose licence is included in the source.\n\nThanks to contributors Antoine Gohin (Broke Studio), jroweboy,\nand the NESdev community.\n\nSpecial thanks to Shiru, and to my patreon supporters, for without this work would not have come this far:\n\nBarry White, Colin Reed, Cornel, Howie Day, Jone, Margaret McNulty-Beldyk, Matt Roszak, Paul Preuss, Raftronaut, Rusty Gerard, an insignifant speck of dust, Ben Smith, Dan Fries, Deadeye, Infinite NES Lives LLC, Jacob Speicher, Joe's Computer Museum, Justin Orenich, Kacper Woźniak, Kalle Siukola, Lee Pfenninger, Matthew Klundt, Max Meiners, Michael Thompson, michael_emh, Ninja Dynamics, Pete Spicer, Pez Pengelly, ReJ aka Renaldas Zioma, RT, Scott Walters, Sean Robinson, W-, Amina, André Luís Baptista da Silva, BIG EVIL CORPORATION, Colin Kingfisher, Eric DeSantis, Frank Provo, Marc Moore, Maxim Sergeevich, Nicholas Berthiaume, NovaSquirrel, RetroNES Software, Yonghai Yu, zeta0134, zzox.";
	StaticText1->Width=300;
	StaticText1->Height=470;
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

