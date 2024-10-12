//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("UnitMain.cpp", FormMain);
USEFORM("About.cpp", AboutBox);
USEFORM("UnitCHREditor.cpp", FormCHREditor);
USEFORM("UnitSwapColors.cpp", FormSwapColors);
USEFORM("UnitNametableOffset.cpp", FormNameOffset);
USEFORM("UnitNESBank.cpp", FormBank);
USEFORM("UnitManageMetasprites.cpp", FormManageMetasprites);
USEFORM("UnitSetSize.cpp", FormSetSize);
USEFORM("UnitName.cpp", FormName);
USEFORM("UnitInputNumber.cpp", FormInputNumber);
USEFORM("UnitMetaspriteOffset.cpp", FormMetaspriteOffset);
USEFORM("UnitPreferences.cpp", FormPreferences);
USEFORM("UnitBrush.cpp", FormBrush);
USEFORM("UnitNavigator.cpp", FormNavigator);
USEFORM("UnitImportBitmap.cpp", FormImportBMP);
USEFORM("UnitLossyDetails.cpp", FormLossyDetails);
USEFORM("UnitSwapAttributes.cpp", FormSwapAttributes);
USEFORM("UnitLineDetails.cpp", FormLineDetails);
USEFORM("UnitBankCHR.cpp", FormBankCHR);
USEFORM("UnitCHRbit.cpp", FormCHRbit);
USEFORM("UnitPropertyConditions.cpp", FormPropConditions);
USEFORM("UnitMTprops.cpp", FormMTprops);
USEFORM("UnitPropID.cpp", FormPropID);
USEFORM("UnitSwapPatternColour.cpp", FormSwapPatternColour);
USEFORM("UnitMetatileEditor.cpp", FormMetatileTool);
USEFORM("UnitWarning.cpp", FormWarning);
USEFORM("UnitSelect2subpalSets.cpp", FormSelect2subpalSets);
USEFORM("UnitEmphasisPalette.cpp", FormEmphasisPalette);
USEFORM("UnitAttributeCheckerOptions.cpp", FormAttrChecker);
USEFORM("UnitPipelineHelper.cpp", FormPipelineHelper);
USEFORM("UnitScanlineWarnings.cpp", FormScanlineWarnings);
USEFORM("UnitBucketToolbox.cpp", FormBucketToolbox);
USEFORM("UnitSwapBanks.cpp", FormSwapBanks);
USEFORM("UnitNewInstance.cpp", FormNewInstance);
USEFORM("UnitSpecialPasteCHR.cpp", FormSpecialPasteCHR);
USEFORM("UnitSubpaletteLibrary.cpp", SubpaletteLibrary1);
USEFORM("UnitColourPicker.cpp", FormColourPicker);
USEFORM("UnitUnsavedChanges.cpp", FormUnsavedChanges);
USEFORM("UnitMetaspritePlaybackRules.cpp", FormMetaspritePlaybackRules);
USEFORM("UnitTileBucketExtras.cpp", FormTileBucketExtras);
USEFORM("UnitNewInstanceMinimal.cpp", FormNewInstanceMinimal);
USEFORM("UnitEscapeTiles.cpp", FormEscapeTiles);
USEFORM("UnitImportPPUDump.cpp", FormImportPPUDump);
//---------------------------------------------------------------------------
#include "FreeImage.h"
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "NEXXT studio 3.0";
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TFormCHREditor), &FormCHREditor);
		Application->CreateForm(__classid(TFormSwapColors), &FormSwapColors);
		Application->CreateForm(__classid(TFormNameOffset), &FormNameOffset);
		Application->CreateForm(__classid(TFormBank), &FormBank);
		Application->CreateForm(__classid(TFormSetSize), &FormSetSize);
		Application->CreateForm(__classid(TFormName), &FormName);
		Application->CreateForm(__classid(TFormManageMetasprites), &FormManageMetasprites);
		Application->CreateForm(__classid(TFormInputNumber), &FormInputNumber);
		Application->CreateForm(__classid(TFormMetaspriteOffset), &FormMetaspriteOffset);
		Application->CreateForm(__classid(TFormPreferences), &FormPreferences);
		Application->CreateForm(__classid(TFormBrush), &FormBrush);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TFormNavigator), &FormNavigator);
		Application->CreateForm(__classid(TFormImportBMP), &FormImportBMP);
		Application->CreateForm(__classid(TFormLossyDetails), &FormLossyDetails);
		Application->CreateForm(__classid(TFormSwapAttributes), &FormSwapAttributes);
		Application->CreateForm(__classid(TFormLineDetails), &FormLineDetails);
		Application->CreateForm(__classid(TFormBankCHR), &FormBankCHR);
		Application->CreateForm(__classid(TFormCHRbit), &FormCHRbit);
		Application->CreateForm(__classid(TFormPropConditions), &FormPropConditions);
		Application->CreateForm(__classid(TFormSwapPatternColour), &FormSwapPatternColour);
		Application->CreateForm(__classid(TFormMetatileTool), &FormMetatileTool);
		Application->CreateForm(__classid(TFormWarning), &FormWarning);
		Application->CreateForm(__classid(TFormMTprops), &FormMTprops);
		Application->CreateForm(__classid(TFormSelect2subpalSets), &FormSelect2subpalSets);
		Application->CreateForm(__classid(TFormEmphasisPalette), &FormEmphasisPalette);
		Application->CreateForm(__classid(TFormAttrChecker), &FormAttrChecker);
		Application->CreateForm(__classid(TFormPipelineHelper), &FormPipelineHelper);
		Application->CreateForm(__classid(TFormPropID), &FormPropID);
		Application->CreateForm(__classid(TFormScanlineWarnings), &FormScanlineWarnings);
		Application->CreateForm(__classid(TFormBucketToolbox), &FormBucketToolbox);
		Application->CreateForm(__classid(TFormSwapBanks), &FormSwapBanks);
		Application->CreateForm(__classid(TFormNewInstance), &FormNewInstance);
		Application->CreateForm(__classid(TFormSpecialPasteCHR), &FormSpecialPasteCHR);
		Application->CreateForm(__classid(TSubpaletteLibrary1), &SubpaletteLibrary1);
		Application->CreateForm(__classid(TFormMetaspritePlaybackRules), &FormMetaspritePlaybackRules);
		Application->CreateForm(__classid(TFormColourPicker), &FormColourPicker);
		Application->CreateForm(__classid(TFormUnsavedChanges), &FormUnsavedChanges);
		Application->CreateForm(__classid(TFormTileBucketExtras), &FormTileBucketExtras);
		Application->CreateForm(__classid(TFormNewInstanceMinimal), &FormNewInstanceMinimal);
		Application->CreateForm(__classid(TFormEscapeTiles), &FormEscapeTiles);
		Application->CreateForm(__classid(TFormImportPPUDump), &FormImportPPUDump);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
