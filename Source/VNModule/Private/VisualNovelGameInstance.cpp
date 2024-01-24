#include "VisualNovelGameInstance.h"
#include "Save/VNSaveSetting.h"

UVisualNovelGameInstance::UVisualNovelGameInstance()
{
	DialogSpeed = 0.05f;

	Language = ELanguage::Korean;

	AutoModeDuration = 4.f;

	FontSize = 24;

	Volumes.Init(1.f, (int32)ESoundKind::Max);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DialogInfo(TEXT(
		"/Game/VisualNovel/Data/DT_DialogInfo.DT_DialogInfo"));
	if (DT_DialogInfo.Succeeded())
	{
		mCurDialogInfoTable = DT_DialogInfo.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DisplayName(TEXT(
		"/Game/VisualNovel/Data/DT_DisplayName.DT_DisplayName"));
	if (DT_DisplayName.Succeeded())
	{
		mDisplayName = DT_DisplayName.Object;
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
	mSaveSetting= Cast<UVNSaveSetting>(UGameplayStatics::CreateSaveGameObject(UVNSaveSetting::StaticClass()));
	LoadSetting();
}

const FDialogInfo* UVisualNovelGameInstance::FindDialogInfoData(const FName& name)
{
	return mCurDialogInfoTable->FindRow<FDialogInfo>(name, TEXT(""));
}

const FDisplayName* UVisualNovelGameInstance::FindDisplayNameData(const FName& name)
{
	return mDisplayName->FindRow<FDisplayName>(name, TEXT(""));
}

const FRichTextStyleRow* UVisualNovelGameInstance::FindTextStyleData(const FName& name)
{
	return mTextStyleTable->FindRow<FRichTextStyleRow>(name, TEXT(""));
}

void UVisualNovelGameInstance::SaveSetting()
{
	mSaveSetting->Volumes = Volumes;
	mSaveSetting->Language = Language;
	mSaveSetting->FontName = FontName;
	mSaveSetting->FontSize = FontSize;
	mSaveSetting->DialogSpeed = DialogSpeed;
	mSaveSetting->AutoModeDuration = AutoModeDuration;
	UGameplayStatics::SaveGameToSlot(mSaveSetting, mSaveSetting->SlotName, mSaveSetting->UserIndex);
}

void UVisualNovelGameInstance::LoadSetting()
{
	UVNSaveSetting* load = Cast<UVNSaveSetting>(UGameplayStatics::LoadGameFromSlot(
		mSaveSetting->SlotName, mSaveSetting->UserIndex));
	if (!IsValid(load))
	{
		UE_LOG(LogTemp, Warning, TEXT(
			"no save data , %s, %i"), *mSaveSetting->SlotName, mSaveSetting->UserIndex);
		return;
	}
	Volumes = load->Volumes;
	Language = load->Language;
	FontName = load->FontName;
	FontSize = load->FontSize;
	DialogSpeed = load->DialogSpeed;
	AutoModeDuration = load->AutoModeDuration;
}