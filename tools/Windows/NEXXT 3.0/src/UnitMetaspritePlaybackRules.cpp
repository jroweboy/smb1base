//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMetaspritePlaybackRules.h"
#include "UnitManageMetasprites.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMetaspritePlaybackRules *FormMetaspritePlaybackRules;
//---------------------------------------------------------------------------
__fastcall TFormMetaspritePlaybackRules::TFormMetaspritePlaybackRules(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMetaspritePlaybackRules::RadioButton1Click(TObject *Sender)
{
	TextA();

    FormManageMetasprites->RadioStart->Caption="start";

}
//---------------------------------------------------------------------------

void __fastcall TFormMetaspritePlaybackRules::TextA(void)
{
	StaticText1->Caption="Animation tags are designed to fit in the upper 2 bits of a duration byte (at your discretion), to keep animation data small.\n\nIn this mode, one of the 4 possible bitcodes is dedicated to explicit start markers to loop back to.\n\nA 'call' on the other hand is a generic bitcode used to execute animation driven behaviour code.Using the call tag either assumes either using its state as an escape character in the animation table, or is reliant on object/game state external to the animation data.";
}
//---------------------------------------------------------------------------
void __fastcall TFormMetaspritePlaybackRules::TextB(void)
{
	StaticText1->Caption="Animation tags are designed to fit in the upper 2 bits of a duration byte (at your discretion), to keep animation data small.\n\nIn this mode, the start of an animation is implied by either being first in the list, or by following a previous loop tag.Static frames should contain a loop tag to not include them in the next animation.\n\nThe omission of a start tag enables 2 generic 'call' bitcodes fitting into the animation bytecode; which may be enough for some games that want a simple scheme for animation driven behaviour.\n\nAlternately, by not using animation calls at all, you can free 1 bit to double the range of possible durations that fit in a single byte.";
}
//---------------------------------------------------------------------------
void __fastcall TFormMetaspritePlaybackRules::TextC(void)
{
	StaticText1->Caption="Duration caps are useful to make sure a duration fits in your intended animation bytecode format.\n\n63 is suitable for when the 2 highest bits are reserved for flags,\n127 when there's just one, and 255 when you want to make sure the duration at least fits within a single byte (you may want to ignore exporting tags and instead supplement a separate table of loop runlengths).\nNEXXT will playback animations with the cap in mind, and export the data accordingly.\n\nTip: there are ways to fit a wider range of durations in a smaller set. for instance, every value above 60 could signify an extra second. Or, you can use double keyframes. Or, maybe you don't need precision down to odd frames, in which case you can double-time at export.";
}
//---------------------------------------------------------------------------


void __fastcall TFormMetaspritePlaybackRules::RadioButton2Click(
      TObject *Sender)
{
	TextB();
	UpdateCallText();
}
//---------------------------------------------------------------------------
void __fastcall TFormMetaspritePlaybackRules::FormShow(TObject *Sender)
{
	if(RadioButton1->Checked) TextA();
	else TextB();
}
//---------------------------------------------------------------------------
void __fastcall TFormMetaspritePlaybackRules::RadioDontCapClick(
      TObject *Sender)
{
   TextC();
}
//---------------------------------------------------------------------------
void __fastcall TFormMetaspritePlaybackRules::chkReplaceCall1MouseEnter(
      TObject *Sender)
{
   StaticText1->Caption="Instead of a generic call tag to use however you want programmatically without any NEXXT playback behaviour tied to it,\nthis option turns it into a command of the type chosen in the listbox.\n\n<Revert> makes the animation play pendulum-style; back and forth.\n<Seconds> multiplies the specified duration by 60 (or 50 in PAL mode).\n<Half secs> multiplies the specified duration by 30 (or 25 in PAL mode).";
}
//---------------------------------------------------------------------------


void __fastcall TFormMetaspritePlaybackRules::RadioButton2MouseEnter(
      TObject *Sender)
{
   TextB();
}
//---------------------------------------------------------------------------

void __fastcall TFormMetaspritePlaybackRules::RadioButton1MouseEnter(
      TObject *Sender)
{
   TextA();
}
//---------------------------------------------------------------------------


void __fastcall TFormMetaspritePlaybackRules::UpdateCallText(void){
	 AnsiString str;
	if (RadioButton1->Checked) str="start";
	else if(chkReplaceCall2->Checked){
	   if(ComboBoxCall2->ItemIndex==0) str="revert";
	   if(ComboBoxCall2->ItemIndex==1) str="seconds";
	   if(ComboBoxCall2->ItemIndex==2) str="h. secs";

	}
	else str="call2";
	FormManageMetasprites->RadioStart->Caption=str;


	if(chkReplaceCall1->Checked){
	   if(ComboBoxCall1->ItemIndex==0) str="revert";
	   if(ComboBoxCall1->ItemIndex==1) str="seconds";
	   if(ComboBoxCall1->ItemIndex==2) str="h. secs";

	}
	else str="call";

	FormManageMetasprites->RadioCall->Caption=str;

}
void __fastcall TFormMetaspritePlaybackRules::ComboBoxCall1Change(
      TObject *Sender)
{
   UpdateCallText();
}
//---------------------------------------------------------------------------

void __fastcall TFormMetaspritePlaybackRules::ComboBoxCall2Change(
      TObject *Sender)
{
   UpdateCallText();
}
//---------------------------------------------------------------------------

void __fastcall TFormMetaspritePlaybackRules::FormCreate(TObject *Sender)
{
   UpdateCallText();
}
//---------------------------------------------------------------------------

void __fastcall TFormMetaspritePlaybackRules::chkReplaceCall1Click(
      TObject *Sender)
{
   UpdateCallText();
}
//---------------------------------------------------------------------------

void __fastcall TFormMetaspritePlaybackRules::chkReplaceCall2Click(
      TObject *Sender)
{
	UpdateCallText();	
}
//---------------------------------------------------------------------------

void __fastcall TFormMetaspritePlaybackRules::FormKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	FormMain->FormKeyDown(Sender, Key, Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetaspritePlaybackRules::FormKeyPress(TObject *Sender,
      char &Key)
{
		FormMain->FormKeyPress(Sender, Key);
}
//---------------------------------------------------------------------------

void __fastcall TFormMetaspritePlaybackRules::FormKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
   FormMain->FormKeyUp(Sender, Key, Shift);	
}
//---------------------------------------------------------------------------

