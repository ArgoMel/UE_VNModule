#include "Save/VNSaveGame.h"

UVNSaveGame::UVNSaveGame()
{
	SlotName = FString(TEXT("Slot1"));
	UserIndex = 0;

	SavedRowNumber = 0;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DialogInfo(TEXT(
		"/Game/VisualNovel/Data/DT_DialogInfo.DT_DialogInfo"));
	if (DT_DialogInfo.Succeeded())
	{
		DialogInfoTable = DT_DialogInfo.Object;
	}
}
