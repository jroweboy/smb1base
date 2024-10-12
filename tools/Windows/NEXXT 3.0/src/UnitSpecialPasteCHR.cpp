//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSpecialPasteCHR.h"
#include "UnitMain.h"
#include "UnitCHREditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSpecialPasteCHR *FormSpecialPasteCHR;
//---------------------------------------------------------------------------
__fastcall TFormSpecialPasteCHR::TFormSpecialPasteCHR(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSpecialPasteCHR::RadioSolidsMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Pastes solid colours (1-3) from clipboard on top of target.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioCol0MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Pastes the backdrop colour (0) from clipboard on top of target.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioCustomMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Pastes chosen colours (chosen with toggle buttons) on top of target.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioAddMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Adds the colour index from clipboard to target.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioAdd1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Adds the colour index from clipboard to target,\nbut balances it with an offset of -1.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioAdd2MouseEnter(TObject *Sender)
{
    FormMain->LabelStats->Caption="Adds the colour index from clipboard to target,\nbut balances it with an offset of -2 (average for 2bpp graphics).";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioAdd3MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Adds the colour index from clipboard to target,\nbut balances it with an offset of -3.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioOnTopMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Pastes clipboard contents 'on top' of target.\nThis is functionally the same as pasting 'solids'";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioBitORMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Merges the bitplanes of clipboard and target in a bitwise OR filter.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioBitANDMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Merges the bitplanes of clipboard and target in a bitwise AND filter.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioBitNANDClick(TObject *Sender)
{
	FormMain->LabelStats->Caption="Merges the bitplanes of clipboard and target in a bitwise NAND filter.";	
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioKeepSimMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Discards dissimilar colour indexes and keeps the matching pixels.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioKeepDiffMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Discards similar colour indexes and keeps the difference.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioKeepMaskMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Uses the colour mask from mode A to discard pixels not matching the mask criteria; keeping the rest.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioRemoveMaskMouseEnter(
	  TObject *Sender)
{
	FormMain->LabelStats->Caption="Uses the colour mask from mode A to discard pixels matching the mask criteria; keeping the rest.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::CheckUseMaskBMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="Borrows the colour mask from mode A and applies it to the algorithm chosen in mode B.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::CheckUseMaskCMouseEnter(
      TObject *Sender)
{
     FormMain->LabelStats->Caption="Borrows the colour mask from mode A and applies it to the algorithm chosen in mode C.";

}
//---------------------------------------------------------------------------



void __fastcall TFormSpecialPasteCHR::RadioSubMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Subtracts the colour index from target with clipboard contents.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioSub1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Subtracts the colour index from target with clipboard contents,\nbut balances it with an offset of +1.";

}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioSub2MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Subtracts the colour index from target with clipboard contents,\nbut balances it with an offset of +2 (average for 2bpp graphics).";

}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioSub3Click(TObject *Sender)
{
	FormMain->LabelStats->Caption="Subtracts the colour index from target with clipboard contents,\nbut balances it with an offset of +3.";

}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioAsPatternMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="The clipboard contents are interpreted as a normal pattern.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::RadioAsSilhouetteMouseEnter(
      TObject *Sender)
{
	FormMain->LabelStats->Caption="The clipboard contents are interpreted as a silhouette in the current active colour.\nInterpretation depends on the colour mask from Mode A.";
}
//---------------------------------------------------------------------------


void __fastcall TFormSpecialPasteCHR::RadioAsInvSilhouetteMouseEnter(
	  TObject *Sender)
{
   FormMain->LabelStats->Caption="The clipboard contents are interpreted as an inverted silhouette in the current active colour.\nInterpretation depends on the colour mask from Mode A.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::CheckSubmaskMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="When using subtractive paste with an offset, the submask protects the background colour automatically.\nThis tends to make it more useful on average.";
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::FormKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if(FormCHREditor->Visible) FormCHREditor->FormKeyDown(Sender,Key,Shift);
	else					   FormMain->FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::FormKeyPress(TObject *Sender,
	  char &Key)
{
	if(FormCHREditor->Visible) FormCHREditor->FormKeyPress(Sender,Key);
	else					   FormMain->FormKeyPress(Sender,Key);
}
//---------------------------------------------------------------------------

void __fastcall TFormSpecialPasteCHR::FormKeyUp(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
   if(FormCHREditor->Visible)  FormCHREditor->FormKeyUp(Sender,Key,Shift);
   else					      FormMain->FormKeyUp(Sender,Key,Shift);
}
//---------------------------------------------------------------------------

