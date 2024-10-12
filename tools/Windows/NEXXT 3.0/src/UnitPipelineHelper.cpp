//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitPipelineHelper.h"
#include "UnitMain.h"
#include "UnitWarning.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPipelineHelper *FormPipelineHelper;
extern AnsiString ansiFileNameAssociatedMetatiles;
extern AnsiString ansiFileNameAssociatedTileset;
extern bool bAssociateMetatiles;
extern bool bAssociateTileset;
extern bool bNotify_linkedmodewarning;

extern unsigned char chr[];
extern unsigned char undoChr[];
extern unsigned char undoCheckpointChr[];
extern unsigned char checkpointChr[];

extern unsigned char chr_LinkBuf[];
extern unsigned char undoChr_LinkBuf[];
extern unsigned char undoCheckpointChr_LinkBuf[];
extern unsigned char checkpointChr_LinkBuf[];

extern bool bIgnoreCheckBoxOnClick_LinkedCHR;

extern bool bLinkedCHRmode;

//---------------------------------------------------------------------------
__fastcall TFormPipelineHelper::TFormPipelineHelper(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::UpdateUI(void)
{
    CheckBox1->Checked=bAssociateMetatiles;
	CheckBox2->Checked=bAssociateTileset;

	btnRefreshCHRlink->Enabled=CheckBox4->Checked;

	if(ansiFileNameAssociatedMetatiles=="") Label1->Caption="No path selected";
	else Label1->Caption=ansiFileNameAssociatedMetatiles;

	if(ansiFileNameAssociatedTileset=="") {
		Label2->Caption="No path selected";
		if(bLinkedCHRmode){
			FormMain->ExitLinkedCHRmode();
			bIgnoreCheckBoxOnClick_LinkedCHR=true;
			CheckBox4->Checked=false;
			bIgnoreCheckBoxOnClick_LinkedCHR=false;
			btnRefreshCHRlink->Enabled=CheckBox4->Checked;
		}
	}
	else {
		Label2->Caption=ansiFileNameAssociatedTileset;
		if((CheckBox4->Checked==true) && (bLinkedCHRmode==false)){
			if(!FormMain->EnterLinkedCHRmode(ansiFileNameAssociatedTileset)){
				Application->MessageBox("Linked tileset mode has no path and will be turned off.\nCheck path and permissions to linked file","Warning: Pipeline helper",MB_OK);
				bIgnoreCheckBoxOnClick_LinkedCHR=true;
				CheckBox4->Checked=false;
				bIgnoreCheckBoxOnClick_LinkedCHR=false;
                btnRefreshCHRlink->Enabled=CheckBox4->Checked;
				return;
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::HandleLoadedLinkedMode(void)
{
    if(ansiFileNameAssociatedTileset=="") {
		Label2->Caption="No path selected";
    }
	bIgnoreCheckBoxOnClick_LinkedCHR=true;
	CheckBox4->Checked=bLinkedCHRmode;
	bIgnoreCheckBoxOnClick_LinkedCHR=false;

}
//---------------------------------------------------------------------------
void __fastcall TFormPipelineHelper::CheckBox1Click(TObject *Sender)
{
	bAssociateMetatiles=CheckBox1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFormPipelineHelper::CheckBox2Click(TObject *Sender)
{
	bAssociateTileset=CheckBox2->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFormPipelineHelper::SpeedButton2Click(TObject *Sender)
{
   ansiFileNameAssociatedMetatiles="";
   UpdateUI();
}
//---------------------------------------------------------------------------
void __fastcall TFormPipelineHelper::SpeedButton4Click(TObject *Sender)
{
  ansiFileNameAssociatedTileset="";
  UpdateUI();
}
//---------------------------------------------------------------------------
void __fastcall TFormPipelineHelper::FormShow(TObject *Sender)
{
	UpdateUI();	
}
//---------------------------------------------------------------------------
void __fastcall TFormPipelineHelper::FormPaint(TObject *Sender)
{
	UpdateUI();
}
//---------------------------------------------------------------------------
void __fastcall TFormPipelineHelper::SpeedButton1Click(TObject *Sender)
{
   int tag = ((TSpeedButton*)Sender)->Tag;
   if(tag==0)OpenDialogPath->Title="Associate metatiles to session";
   if(tag==1)OpenDialogPath->Title="Associate tileset to session";

   /*
   char tmp[MAXPATH];
   GetCurrentDirectory(MAXPATH,tmp);
   AnsiString workDir(tmp);
   */
   AnsiString workDir = ExtractFileDir(FormMain->SaveDialogSession->FileName);

   if(OpenDialogPath->Execute())
   {
		if(tag==0){


			ansiFileNameAssociatedMetatiles=FormMain->MakePathLocal(workDir,OpenDialogPath->FileName);

		}
		if(tag==1){

			ansiFileNameAssociatedTileset=FormMain->MakePathLocal(workDir,OpenDialogPath->FileName);
			if(CheckBox4->Checked) {
                FormMain->ExitLinkedCHRmode();
				CheckBox4Click(Sender);
			}
		}

   }

   UpdateUI();

}
//---------------------------------------------------------------------------
void __fastcall TFormPipelineHelper::CheckBox4Click(TObject *Sender)
{
	if(bIgnoreCheckBoxOnClick_LinkedCHR) return;



	//at this point, the checkbox has already changed - handling occurs after.
	if(bNotify_linkedmodewarning==true && (CheckBox4->Checked==true)){
		FormWarning->Caption="Confirm: Enter linked CHR mode.";
		FormWarning->StaticText1->Caption="Checking this button; you will view && edit the tileset contentsof another session file,\nif a valid path is present.\n\nWhen you save session, you will also save that other file.\nIt is recommended to make a backup of that file before you proceed.\n\nTo remind you, a '!' symbol is shown in the caption of the tileset canvas.\n\nThis provides a way to have one canonical tileset for many sessions,\nbut be aware that any session that has view/edit permission can overwrite the tileset of the linked-to session.\n\nUnchecking reverts to normal editing, viewing and saving behaviour,\nand this sessions' native tileset is visible once again.";


		if (FormWarning->ShowModal() == mrCancel){

			//
			if(FormWarning->CheckBox1->Checked==true) bNotify_linkedmodewarning=false; //user want to be notified?
			if(bNotify_linkedmodewarning==false) FormMain->SaveConfig();      //if not, save config.
			FormWarning->CheckBox1->Checked=false;                      //reset notification form


			bIgnoreCheckBoxOnClick_LinkedCHR=true;
			CheckBox4->Checked=false;
			bIgnoreCheckBoxOnClick_LinkedCHR=false;
			btnRefreshCHRlink->Enabled=CheckBox4->Checked;
			return;
		}
	}

	//fix config according to users' wishes.
	if(FormWarning->CheckBox1->Checked==true) bNotify_linkedmodewarning=false; //user want to be notified?
	if(bNotify_linkedmodewarning==false) FormMain->SaveConfig();      //if not, save config.
	FormWarning->CheckBox1->Checked=false;                      //reset notification form

	//at this point, a real change has occured. we can now check whether to perform a mode swap.
	if (CheckBox4->Checked) //try linking files.
	{
		if(!FormMain->EnterLinkedCHRmode(ansiFileNameAssociatedTileset)){
			Application->MessageBox("Check path and permissions to file","Error",MB_OK);
			bIgnoreCheckBoxOnClick_LinkedCHR=true;
			CheckBox4->Checked=false;
			bIgnoreCheckBoxOnClick_LinkedCHR=false;
			btnRefreshCHRlink->Enabled=CheckBox4->Checked;
			return;
		}
	}
	else   //this should only happen if we are already in linked mode.
	{
		FormMain->ExitLinkedCHRmode();
	}
    btnRefreshCHRlink->Enabled=CheckBox4->Checked;
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------



void __fastcall TFormPipelineHelper::CheckBox1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
		bAssociateMetatiles=CheckBox1->Checked;	
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::CheckBox2MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	bAssociateTileset=CheckBox2->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::CheckBox2MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Associated tileset flag:\tSets a flag in this session file to say that the path is associated with this session.\nA gamebound parser script may then use that flag to indicatethat the tileset of the other file \nshould be parsed instead of this one.\nThis flag has no effect on the editing or contents of the currently open session file in of itself.";

}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::CheckBox1MouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Associated metatiles flag:\tSets a flag in this session file to say that the path is associated with this session.\nA gamebound parser script may then use that flag to indicatethat the metatiles of the other file \nshould be parsed instead of this one.\nThis flag has no effect on the editing or contents of the currently open session file in of itself.";

}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::CheckBox1MouseLeave(TObject *Sender)
{
	FormMain->LabelStats->Caption="---";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::CheckBox4MouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Links the tilesets of the associated file to this file; if a valid path is present.\nWhile in linked mode, you're viewing and editing the tiles of the other file.\nWhen saving session, the tiles of the other file also get saved to that file.\nUnchecking exits linked mode, and you're again viewing/editing this files' tiles.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::chkAutoExpMetatilesBMPMouseEnter(
      TObject *Sender)
{
   FormMain->LabelStats->Caption="If checked, when saving session, you'll automatically also export the currently chosen metatile tab as a bitmap.\n";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::RadioButton1MouseEnter(
      TObject *Sender)
{
   FormMain->LabelStats->Caption="Auto-exported assets get saved to the same location as the session file.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::RadioButton2MouseEnter(
	  TObject *Sender)
{
   FormMain->LabelStats->Caption="Auto-exported assets get saved to a Project Subfolder; at the same location as the session file.\nIf no such folder exists, one will be created, either by type or name.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::btnAssetMouseEnter(TObject *Sender)
{
   FormMain->LabelStats->Caption="Project subfolder gets named after the asset type, e.g. 'Autoexport Metatiles'";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::btnNameMouseEnter(TObject *Sender)
{
	FormMain->LabelStats->Caption="Project subfolder gets named after the session file, e.g. 'Autoexport {Filename}'";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::btnTypeMouseEnter(TObject *Sender)
{
  FormMain->LabelStats->Caption="Project subfolder gets named after the filetype, e.g. 'Autoexport Bitmaps'";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::RadioButton3MouseEnter(
      TObject *Sender)
{
  FormMain->LabelStats->Caption="Auto-exported assets get saved to the Autoexports subfolder of this NEXXT installation.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::btnRefreshCHRlinkMouseEnter(
      TObject *Sender)
{
   FormMain->LabelStats->Caption="Exits and re-enters linked mode; refreshing the tileset from source.";
}
//---------------------------------------------------------------------------

void __fastcall TFormPipelineHelper::btnRefreshCHRlinkClick(
      TObject *Sender)
{
	FormMain->ExitLinkedCHRmode();
    FormMain->EnterLinkedCHRmode(ansiFileNameAssociatedTileset);

}
//---------------------------------------------------------------------------

