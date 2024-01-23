#include "HUD/VisualNovelHUD.h"
#include "HUD/DialogHUD.h"
#include "VisualNovelGameInstance.h"

AVisualNovelHUD::AVisualNovelHUD()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget>	WBP_DialogHUD(TEXT(
		"/Game/VisualNovel/BluePrints/HUD/WBP_DialogHUD.WBP_DialogHUD_C"));
	if (WBP_DialogHUD.Succeeded())
	{
		mUIClass = WBP_DialogHUD.Class;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase>	SW_Continue(TEXT(
		"/Script/Engine.SoundWave'/Game/VisualNovel/Sounds/SFX/SW_Continue.SW_Continue'"));
	if (SW_Continue.Succeeded())
	{
		mLogButtonSound = SW_Continue.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundMix>	SCM_Sounds(TEXT(
		"/Game/VisualNovel/Sounds/SoundClass/SCM_Sounds.SCM_Sounds"));
	if (SCM_Sounds.Succeeded())
	{
		mSoundMix = SCM_Sounds.Object;
	}
	for (int32 i = 0; i < (int32)ESoundKind::Max; ++i)
	{
		const FString name = EnumToFString<ESoundKind>((ESoundKind)i);
		const FString string = FString::Printf(TEXT(
			"/Game/VisualNovel/Sounds/SoundClass/SC_%s.SC_%s"), *name, *name);
		ConstructorHelpers::FObjectFinder<USoundClass>	soundClass(*string);
		if (soundClass.Succeeded())
		{
			mSoundClasses.Emplace(soundClass.Object);
		}
	}
}

void AVisualNovelHUD::BeginPlay()
{
	Super::BeginPlay();
	mGameInstance = GetWorld()->GetGameInstance<UVisualNovelGameInstance>();

	if (IsValid(mUIClass))
	{
		mDialogWidget = CreateWidget<UDialogHUD>(GetWorld(), mUIClass);
		if (IsValid(mDialogWidget))
		{
			mDialogWidget->AddToViewport();
		}
	}

	UGameplayStatics::SetBaseSoundMix(GetWorld(), mSoundMix);
	for (int32 i = 0; i < (int32)ESoundKind::Max; ++i)
	{
		SetVolume(mGameInstance->Volumes[i], i);
	}
}

AVisualNovelHUD* AVisualNovelHUD::GetVNHUD_Implementation()
{
	return this;
}

void AVisualNovelHUD::NextDialog()
{
	mDialogWidget->NextDialog();
}

void AVisualNovelHUD::SetVolume(float value, int32 index)
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), mSoundMix, mSoundClasses[index], value);
	mGameInstance->Volumes[index] = value;
}
