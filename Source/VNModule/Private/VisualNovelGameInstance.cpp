#include "VisualNovelGameInstance.h"

UVisualNovelGameInstance::UVisualNovelGameInstance()
{
	DialogSpeed = 0.05f;

	Language = ELanguage::Korean;

	AutoModeDuration = 4.f;
	ResetAutoModeDuration = AutoModeDuration;

	FontSize = 24;

	Volumes.Init(1.f, (int32)ESoundKind::Max);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DialogInfo(TEXT(
		"/Game/VisualNovel/Data/DT_DialogInfo.DT_DialogInfo"));
	if (DT_DialogInfo.Succeeded())
	{
		mDialogInfoTable = DT_DialogInfo.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TextStyle(TEXT(
		"/Game/VisualNovel/Data/DT_TextStyle.DT_TextStyle"));
	if (DT_TextStyle.Succeeded())
	{
		mTextStyleTable = DT_TextStyle.Object;
	}
}

void UVisualNovelGameInstance::Init()
{
	Super::Init();
	mFontNames = mTextStyleTable->GetRowNames();
}

const FDialogInfo* UVisualNovelGameInstance::FindDialogInfoData(const FName& name)
{
	return mDialogInfoTable->FindRow<FDialogInfo>(name, TEXT(""));
}

const FRichTextStyleRow* UVisualNovelGameInstance::FindTextStyleData(const FName& name)
{
	return mTextStyleTable->FindRow<FRichTextStyleRow>(name, TEXT(""));
}
