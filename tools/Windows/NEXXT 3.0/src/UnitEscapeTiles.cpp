//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitEscapeTiles.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEscapeTiles *FormEscapeTiles;

extern bool mmc2_modeON;
extern bool mmc2_doublepair;
extern bool mmc2_affectBG;
extern bool mmc2_affectSpr;
extern int mmc2_checked1;
extern int mmc2_checked2;
extern int mmc2_unchecked1;
extern int mmc2_unchecked2;

extern int mmc2_checked1_tile;
extern int mmc2_checked2_tile;
extern int mmc2_unchecked1_tile;
extern int mmc2_unchecked2_tile;

extern int mmc2_currentEscapeTilePreset;

extern bool mmc2_Doublepair_1stSet;
extern bool mmc2_Doublepair_2ndSet;

extern bool mmc2_startSet_use1st;
extern bool mmc2_startSet_useActive;
extern bool mmc2_startSet_useFixedSet;
extern unsigned int mmc2_startSet_fixed;

extern bool mmc2_startSet_use1st_mt;
extern bool mmc2_startSet_useActive_mt;
extern bool mmc2_startSet_useFixedSet_mt;
extern unsigned int mmc2_startSet_fixed_mt;
extern bool mmc2_startSet_asPrevious_mt;


extern bool mmc2_startSet_use1st_preset_mt[2];
extern bool mmc2_startSet_useActive_preset_mt[2];
extern bool mmc2_startSet_useFixedSet_preset_mt[2];
extern unsigned int mmc2_startSet_fixed_preset_mt[2];
extern bool mmc2_startSet_asPrevious_preset_mt[2];

extern bool mmc2_spriteSwitch1st;
extern bool mmc2_spriteSwitchEvery;
extern bool mmc2_spriteSwitchLast;

extern bool mmc2_spriteSwitch1st_preset[2];
extern bool mmc2_spriteSwitchEvery_preset[2];
extern bool mmc2_spriteSwitchLast_preset[2];

extern int switchTile[4];
extern int switchTileTarget[4];
extern bool switchTileChecked[4];

extern bool		 	mmc2_doublepair_preset[2];
extern bool 		mmc2_affectBG_preset[2];
extern bool 		mmc2_affectSpr_preset[2];
extern bool 		mmc2_startSet_use1st_preset[2];
extern bool 		mmc2_startSet_useActive_preset[2];
extern bool 		mmc2_startSet_useFixedSet_preset[2];
extern unsigned int mmc2_startSet_fixed_preset[2];
extern bool 		mmc2_Doublepair_1stSet_preset[2];
extern bool 		mmc2_Doublepair_2ndSet_preset[2];

extern unsigned int		switchTile_preset[8];
extern unsigned int		switchTile_preset[8];
extern unsigned int		switchTile_preset[8];
extern unsigned int		switchTile_preset[8];

extern unsigned int		switchTileTarget_preset[8];
extern unsigned int		switchTileTarget_preset[8];
extern unsigned int		switchTileTarget_preset[8];
extern unsigned int		switchTileTarget_preset[8];

extern bool		switchTileChecked_preset[8];
extern bool		switchTileChecked_preset[8];
extern bool		switchTileChecked_preset[8];
extern bool		switchTileChecked_preset[8];

extern bool mmc2_loadFromSession;


bool programmatic=false;
//---------------------------------------------------------------------------
__fastcall TFormEscapeTiles::TFormEscapeTiles(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::FormCreate(TObject *Sender)
{
	programmatic=true;
	ComboBox5->AddItem("Set A",NULL);
	ComboBox6->AddItem("Set A",NULL);
	ComboBox7->AddItem("Set A",NULL);
	ComboBox8->AddItem("Set A",NULL);

	ComboBox5->AddItem("Set B",NULL);
	ComboBox6->AddItem("Set B",NULL);
	ComboBox7->AddItem("Set B",NULL);
	ComboBox8->AddItem("Set B",NULL);

	ComboBox5->AddItem("Set C",NULL);
	ComboBox6->AddItem("Set C",NULL);
	ComboBox7->AddItem("Set C",NULL);
	ComboBox8->AddItem("Set C",NULL);

	ComboBox5->AddItem("Set D",NULL);
	ComboBox6->AddItem("Set D",NULL);
	ComboBox7->AddItem("Set D",NULL);
	ComboBox8->AddItem("Set D",NULL);

	AnsiString str;
	for(int i=0; i<256; i++){
		  str=IntToHex(i,2);
		  ComboBox1->AddItem(str,NULL);
		  ComboBox2->AddItem(str,NULL);
		  ComboBox3->AddItem(str,NULL);
		  ComboBox4->AddItem(str,NULL);
	}

	ComboBox1->ItemIndex=switchTile[0];
	ComboBox2->ItemIndex=switchTile[1];
	ComboBox3->ItemIndex=switchTile[2];
	ComboBox4->ItemIndex=switchTile[3];

	ComboBox5->ItemIndex=switchTileTarget[0]/4096;
	ComboBox6->ItemIndex=switchTileTarget[1]/4096;
	ComboBox7->ItemIndex=switchTileTarget[2]/4096;
	ComboBox8->ItemIndex=switchTileTarget[3]/4096;

	chkTile1->Checked=switchTileChecked[0];
	chkTile2->Checked=switchTileChecked[1];
	chkTile3->Checked=switchTileChecked[2];
	chkTile4->Checked=switchTileChecked[3];
	programmatic=false;
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::btnModeOnOffClick(TObject *Sender)
{
	btnModeOnOff->Caption=btnModeOnOff->Down? "mode on" : "mode off";
	mmc2_modeON=btnModeOnOff->Down;
	FormMain->menuMMC2mode->Checked=mmc2_modeON;
	UpdateConditions(false);
	FormMain->UpdateAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormEscapeTiles::SetStartConditions()
{
	if(mmc2_loadFromSession){
		ComboBox1->ItemIndex=switchTile[0];
		ComboBox2->ItemIndex=switchTile[1];
		ComboBox3->ItemIndex=switchTile[2];
		ComboBox4->ItemIndex=switchTile[3];

		ComboBox5->ItemIndex=switchTileTarget_preset[0];
		ComboBox6->ItemIndex=switchTileTarget_preset[1];
		ComboBox7->ItemIndex=switchTileTarget_preset[2];
		ComboBox8->ItemIndex=switchTileTarget_preset[3];

		chkTile1->Checked=switchTileChecked[0]?true:false;
		chkTile2->Checked=switchTileChecked[1]?true:false;
		chkTile3->Checked=switchTileChecked[2]?true:false;
		chkTile4->Checked=switchTileChecked[3]?true:false;

		RadioDoublePair2nd->Checked=mmc2_Doublepair_2ndSet;
		RadioDoublePair1st->Checked=mmc2_Doublepair_1stSet;

		chkDoublePair->Checked=mmc2_doublepair;
		chkBG->Checked=mmc2_affectBG;
		chkSpr->Checked=mmc2_affectSpr;

		//chk1stLine->Checked=!mmc2_8switch;

		if(mmc2_spriteSwitchLast) 			RadioLastLine->Checked=true;
		else if (mmc2_spriteSwitchEvery) 	RadioEveryLine->Checked=true;
		else                                Radio1stLine->Checked=true;

		if(mmc2_startSet_useFixedSet){
			if 		(mmc2_startSet_fixed==3) RadioUseD->Checked=true;
			else if (mmc2_startSet_fixed==2) RadioUseC->Checked=true;
			else if (mmc2_startSet_fixed==1) RadioUseB->Checked=true;
			else    RadioUseA->Checked=true;
		}
		else if (mmc2_startSet_useActive) RadioUseActive->Checked=true;
		else RadioUse1stChecked->Checked=true;

		if(mmc2_currentEscapeTilePreset==0)  		btnMMC2->Down=true;
		else if(mmc2_currentEscapeTilePreset==1)    btnPreset1->Down=true;
		else if(mmc2_currentEscapeTilePreset==1) btnPreset2->Down=true;

	}

	else if(mmc2_currentEscapeTilePreset==0){
		btnMMC2->Down=true;

		ComboBox1->ItemIndex=0xFE;
		ComboBox2->ItemIndex=0xFD;
		ComboBox3->ItemIndex=0xFC;
		ComboBox4->ItemIndex=0xFB;
		ComboBox5->ItemIndex=0;
		ComboBox6->ItemIndex=1;
		ComboBox7->ItemIndex=2;
		ComboBox8->ItemIndex=3;

		chkTile1->Checked=true;
		chkTile2->Checked=true;
		chkTile3->Checked=false;
		chkTile4->Checked=false;

		RadioDoublePair1st->Checked=true;
		chkDoublePair->Checked=true;
		chkBG->Checked=true;
		chkSpr->Checked=true;
		RadioUse1stChecked->Checked=true;
		Radio1stLine->Checked=true;

	}
	FormMain->menuMMC2mode->Checked=mmc2_modeON;
	btnModeOnOff->Down=mmc2_modeON;
	btnModeOnOff->Caption=btnModeOnOff->Down? "mode on" : "mode off";

	btnStore->Enabled= (mmc2_currentEscapeTilePreset>0)?true:false;
	if(mmc2_loadFromSession) UpdateConditions(false);
	mmc2_loadFromSession=false;
}
void __fastcall TFormEscapeTiles::UpdateConditions(bool allowUpdate)
{


	//special double-set behaviour

	int checked1 = -1;
	int checked2 = -1;
	int unchecked1 = -1;
	int unchecked2 = -1;
	int count=0;
	int checked1_tile;
	int checked2_tile;
	int unchecked1_tile;
	int unchecked2_tile;
	if(chkTile1->Checked) count++;
	if(chkTile2->Checked) count++;
	if(chkTile3->Checked) count++;
	if(chkTile4->Checked) count++;



		if (chkTile1->Checked) {
			checked1 = ComboBox5->ItemIndex*4096;
			checked1_tile = ComboBox1->ItemIndex;
		}
		if (chkTile2->Checked) {
			if (checked1 == -1) {
				checked1 = ComboBox6->ItemIndex*4096;
				checked1_tile = ComboBox2->ItemIndex;
			} else {
				checked2 = ComboBox6->ItemIndex*4096;
				checked2_tile = ComboBox2->ItemIndex;
			}
		}
		if (chkTile3->Checked) {
			if (checked1 == -1) {
				checked1 = ComboBox7->ItemIndex*4096;
				checked1_tile = ComboBox3->ItemIndex;
			} else {
				checked2 = ComboBox7->ItemIndex*4096;
				checked2_tile = ComboBox3->ItemIndex;
			}
		}
		if (chkTile4->Checked) {
			if (checked1 == -1) {
				checked1 = ComboBox8->ItemIndex*4096;
				checked1_tile = ComboBox4->ItemIndex;
			} else {
				checked2 = ComboBox8->ItemIndex*4096;
				checked2_tile = ComboBox4->ItemIndex;
			}
		}
		mmc2_checked1    = checked1;    //this one used both for double-pair and sometimes else.
		mmc2_checked2    = checked2;
        mmc2_checked1_tile    = checked1_tile;
		mmc2_checked2_tile    = checked2_tile;

		if(count==2){
		//unchecked
		if (!chkTile1->Checked) {
			unchecked1 = ComboBox5->ItemIndex*4096;
			ComboBox1->Enabled=false;
		}
		if (!chkTile2->Checked) {
			if (unchecked1 == -1) {
				unchecked1 = ComboBox6->ItemIndex*4096;
				ComboBox2->Enabled=false;
			} else {
				unchecked2 = ComboBox6->ItemIndex*4096;
				ComboBox2->Enabled=false;
			}
		}
		if (!chkTile3->Checked) {
			if (unchecked1 == -1) {
				unchecked1 = ComboBox7->ItemIndex*4096;
				ComboBox3->Enabled=false;
			} else {
				unchecked2 = ComboBox7->ItemIndex*4096;
				ComboBox3->Enabled=false;
			}
		}
		if (!chkTile4->Checked) {
			if (unchecked1 == -1) {
				unchecked1 = ComboBox8->ItemIndex*4096;
				ComboBox4->Enabled=false;
			} else {
				unchecked2 = ComboBox8->ItemIndex*4096;
				ComboBox4->Enabled=false;
			}
		}
		unchecked1_tile = checked1_tile;
		unchecked2_tile = checked2_tile;

		
		//ShowMessage(IntToStr(mmc2_checked1) +":"+ IntToStr(checked1) + "\n" + IntToStr(mmc2_checked2) +":"+ IntToStr(checked2) + "\n"+ IntToStr(mmc2_unchecked1) +":"+ IntToStr(unchecked1) +"\n"+ IntToStr(mmc2_unchecked2) +":"+ IntToStr(unchecked2) +"\n");

		mmc2_unchecked1  = unchecked1;
		mmc2_unchecked2  = unchecked2;




		mmc2_unchecked1_tile  = unchecked1_tile;
		mmc2_unchecked2_tile  = unchecked2_tile;
	mmc2_doublepair = chkDoublePair->Checked? true:false;
	}
	else {
		mmc2_doublepair = false;
		ComboBox1->Enabled=true;
		ComboBox2->Enabled=true;
		ComboBox3->Enabled=true;
		ComboBox4->Enabled=true;

	}


	switchTile[0]=ComboBox1->ItemIndex;
	switchTile[1]=ComboBox2->ItemIndex;
	switchTile[2]=ComboBox3->ItemIndex;
	switchTile[3]=ComboBox4->ItemIndex;

	switchTileTarget[0]=ComboBox5->ItemIndex*4096;
	switchTileTarget[1]=ComboBox6->ItemIndex*4096;
	switchTileTarget[2]=ComboBox7->ItemIndex*4096;
	switchTileTarget[3]=ComboBox8->ItemIndex*4096;

	switchTileChecked[0]=chkTile1->Checked;
	switchTileChecked[1]=chkTile1->Checked;
	switchTileChecked[2]=chkTile1->Checked;
	switchTileChecked[3]=chkTile1->Checked;

	mmc2_affectBG=chkBG->Checked;
	mmc2_affectSpr=chkSpr->Checked;

	mmc2_Doublepair_1stSet=RadioDoublePair1st->Checked;

	mmc2_startSet_use1st=RadioUse1stChecked->Checked;
	mmc2_startSet_useActive=RadioUseActive->Checked;
	mmc2_startSet_useFixedSet=(RadioUseA->Checked || RadioUseB->Checked || RadioUseC->Checked || RadioUseD->Checked)? true:false;

	if(RadioUseA->Checked) mmc2_startSet_fixed=0;
	if(RadioUseB->Checked) mmc2_startSet_fixed=1*4096;
	if(RadioUseC->Checked) mmc2_startSet_fixed=2*4096;
	if(RadioUseD->Checked) mmc2_startSet_fixed=3*4096;

	//mmc2_8switch=!chk1stLine->Checked;

	if(RadioLastLine->Checked) 			mmc2_spriteSwitchLast=true;
	else if (RadioEveryLine->Checked) 	mmc2_spriteSwitchEvery=true;
  	else                                mmc2_spriteSwitch1st=true;



	mmc2_startSet_use1st_mt=RadioUse1stChecked_mt->Checked;
	mmc2_startSet_useActive_mt=RadioUseActive_mt->Checked;
	mmc2_startSet_asPrevious_mt=RadioUseSame_mt->Checked;
	mmc2_startSet_useFixedSet_mt=(RadioUseA_mt->Checked || RadioUseB_mt->Checked || RadioUseC_mt->Checked || RadioUseD_mt->Checked)? true:false;

	if(RadioUseA_mt->Checked) mmc2_startSet_fixed_mt=0;
	if(RadioUseB_mt->Checked) mmc2_startSet_fixed_mt=1*4096;
	if(RadioUseC_mt->Checked) mmc2_startSet_fixed_mt=2*4096;
	if(RadioUseD_mt->Checked) mmc2_startSet_fixed_mt=3*4096;




	if(mmc2_currentEscapeTilePreset==0){
		FormMain->menuMMC2mode->Caption="Use switchtiles (MMC2/4; 512 tiles)";
	}
	else{
		FormMain->menuMMC2mode->Caption="Use switchtiles (preset %i; %i tiles)",mmc2_currentEscapeTilePreset,count;
	}
	//update if necessary
	if(btnModeOnOff->Down && allowUpdate) FormMain->UpdateAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::chkTile1Click(TObject *Sender)
{
	UpdateConditions(true);
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::ComboBox1Change(TObject *Sender)
{
   if(!programmatic) UpdateConditions(true);
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::ComboBox5Change(TObject *Sender)
{
   if(!programmatic) UpdateConditions(true);
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::btnMMC2Click(TObject *Sender)
{
  programmatic=true;

   TSpeedButton *btn = dynamic_cast<TSpeedButton*>(Sender);

	if (btn != NULL) {
		int tag = btn->Tag;
		btnStore->Enabled= (tag>0)?true:false;
		mmc2_currentEscapeTilePreset=tag;
	}
	int tmp2=mmc2_currentEscapeTilePreset-1;
	
	if(tmp2<0){


		ComboBox1->ItemIndex=0xFE;
		ComboBox2->ItemIndex=0xFD;
		ComboBox3->ItemIndex=0xFC;
		ComboBox4->ItemIndex=0xFB;
		ComboBox5->ItemIndex=0;
		ComboBox6->ItemIndex=1;
		ComboBox7->ItemIndex=2;
		ComboBox8->ItemIndex=3;

		chkTile1->Checked=true;
		chkTile2->Checked=true;
		chkTile3->Checked=false;
		chkTile4->Checked=false;

		RadioDoublePair1st->Checked=true;
		chkDoublePair->Checked=true;
		chkBG->Checked=true;
		chkSpr->Checked=true;
		RadioUse1stChecked->Checked=true;
		RadioUseActive_mt->Checked=true;
		//btnStore->Enabled=false;

		Radio1stLine->Checked=true;
	}
	else{
		ComboBox1->ItemIndex=switchTile_preset[tmp2*4+0];
		ComboBox2->ItemIndex=switchTile_preset[tmp2*4+1];
		ComboBox3->ItemIndex=switchTile_preset[tmp2*4+2];
		ComboBox4->ItemIndex=switchTile_preset[tmp2*4+3];

		ComboBox5->ItemIndex=switchTileTarget_preset[tmp2*4+0];
		ComboBox6->ItemIndex=switchTileTarget_preset[tmp2*4+1];
		ComboBox7->ItemIndex=switchTileTarget_preset[tmp2*4+2];
		ComboBox8->ItemIndex=switchTileTarget_preset[tmp2*4+3];

		chkTile1->Checked=switchTileChecked_preset[tmp2*4+0]?true:false;
		chkTile2->Checked=switchTileChecked_preset[tmp2*4+1]?true:false;
		chkTile3->Checked=switchTileChecked_preset[tmp2*4+2]?true:false;
		chkTile4->Checked=switchTileChecked_preset[tmp2*4+3]?true:false;

		RadioDoublePair2nd->Checked=mmc2_Doublepair_2ndSet_preset[tmp2];
		RadioDoublePair1st->Checked=mmc2_Doublepair_1stSet_preset[tmp2];

		chkDoublePair->Checked=mmc2_doublepair_preset[tmp2];
		chkBG->Checked=mmc2_affectBG_preset[tmp2];
		chkSpr->Checked=mmc2_affectSpr_preset[tmp2];

		//chk1stLine->Checked=!mmc2_8switch_preset[tmp2];

		if(mmc2_spriteSwitchLast_preset[tmp2]) 			RadioLastLine->Checked=true;
		else if (mmc2_spriteSwitchEvery_preset[tmp2]) 	RadioEveryLine->Checked=true;
		else                                			Radio1stLine->Checked=true;


		if(mmc2_startSet_useFixedSet_preset[tmp2]){
			if 		(mmc2_startSet_fixed_preset[tmp2]==3) RadioUseD->Checked=true;
			else if (mmc2_startSet_fixed_preset[tmp2]==2) RadioUseC->Checked=true;
			else if (mmc2_startSet_fixed_preset[tmp2]==1) RadioUseB->Checked=true;
			else    RadioUseA->Checked=true;
		}
		else if (mmc2_startSet_useActive_preset[tmp2]) RadioUseActive->Checked=true;
		else RadioUse1stChecked->Checked=true;



		 if(mmc2_startSet_useFixedSet_preset_mt[tmp2]){
			if 		(mmc2_startSet_fixed_preset_mt[tmp2]==3) RadioUseD_mt->Checked=true;
			else if (mmc2_startSet_fixed_preset_mt[tmp2]==2) RadioUseC_mt->Checked=true;
			else if (mmc2_startSet_fixed_preset_mt[tmp2]==1) RadioUseB_mt->Checked=true;
			else    RadioUseA_mt->Checked=true;
		}
		else if (mmc2_startSet_useActive_preset_mt[tmp2]) RadioUseActive_mt->Checked=true;
		else if (mmc2_startSet_asPrevious_preset_mt[tmp2]) RadioUseSame_mt->Checked=true;
		else RadioUse1stChecked_mt->Checked=true;

	}


  UpdateConditions(true);
  programmatic=false;


}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::chkDoublePairClick(TObject *Sender)
{
  UpdateConditions(true);
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::chkBGClick(TObject *Sender)
{
  UpdateConditions(true);
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::chkSprClick(TObject *Sender)
{
	UpdateConditions(true);	
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::btnStoreClick(TObject *Sender)
{
	FormMain->StorePreset_SwitchTiles(-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormEscapeTiles::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	FormMain->FormKeyDown(Sender, Key, Shift);	
}
//---------------------------------------------------------------------------

void __fastcall TFormEscapeTiles::FormKeyPress(TObject *Sender, char &Key)
{
	FormMain->FormKeyPress(Sender, Key);	
}
//---------------------------------------------------------------------------

void __fastcall TFormEscapeTiles::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	FormMain->FormKeyUp(Sender, Key, Shift);	
}
//---------------------------------------------------------------------------


