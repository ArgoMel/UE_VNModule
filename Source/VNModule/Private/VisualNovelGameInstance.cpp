#include "VisualNovelGameInstance.h"

UVisualNovelGameInstance::UVisualNovelGameInstance()
{
	Language = ELanguage::Korean;

	AutoModeDuration = 4.f;
	ResetAutoModeDuration = AutoModeDuration;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DialogInfo(TEXT(
		"/Game/VisualNovel/Data/DT_DialogInfo.DT_DialogInfo"));
	if (DT_DialogInfo.Succeeded())
	{
		mDialogInfoTable = DT_DialogInfo.Object;
	}
}

void UVisualNovelGameInstance::Init()
{
	Super::Init();
}

const FDialogInfo* UVisualNovelGameInstance::FindDialogInfoData(const FName& name)
{
	return mDialogInfoTable->FindRow<FDialogInfo>(name, TEXT(""));
}
