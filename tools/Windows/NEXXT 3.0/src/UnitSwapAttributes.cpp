//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "UnitMain.h"
#include "UnitSwapAttributes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "UnitSwapColors"
#pragma resource "*.dfm"
TFormSwapAttributes *FormSwapAttributes;
//---------------------------------------------------------------------------
__fastcall TFormSwapAttributes::TFormSwapAttributes(TComponent* Owner)
	: TFormSwapColors(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapAttributes::FormShow(TObject *Sender)
{
	if(FormMain->ConfirmNameSelection()) RadioButtonSelection->Enabled=true;
	else{
		RadioButton4K->Checked=true;
		RadioButtonSelection->Enabled=false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSwapAttributes::CheckBox1Click(TObject *Sender)
{
	extern int nameTableWidth;
	extern int nameTableHeight;
	extern unsigned char bgPal[];
	extern unsigned char palBuf[];
	extern unsigned char attrTable[];
	extern unsigned char tmpAttrTable[];
	extern unsigned char metaSprites[];
    extern unsigned char metaSpritesBuf[];
	int tmp_AttrSize = (nameTableWidth+3)/4*((nameTableHeight+3)/4);

	if(!CheckBox1->Checked)
	{
		memcpy (bgPal, palBuf, 4*16);
		memcpy (attrTable,tmpAttrTable , tmp_AttrSize);
		memcpy (metaSprites,metaSpritesBuf,256*64*4);
		
	}
	if(RadioSpritesNone->Checked) memcpy (metaSprites,metaSpritesBuf,256*64*4);



	PreviewSwap();
}
//---------------------------------------------------------------------------


