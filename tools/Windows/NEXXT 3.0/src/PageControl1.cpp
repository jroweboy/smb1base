//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "PageControl1.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TPageControl1 *)
{
	new TPageControl1(NULL);
}
//---------------------------------------------------------------------------
__fastcall TPageControl1::TPageControl1(TComponent* Owner)
	: TPageControl(Owner)
{
}
//---------------------------------------------------------------------------
namespace Pagecontrol1
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TPageControl1)};
		RegisterComponents("Samples", classes, 0);
	}
}
//---------------------------------------------------------------------------
