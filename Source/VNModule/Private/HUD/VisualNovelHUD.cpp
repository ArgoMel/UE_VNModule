#include "HUD/VisualNovelHUD.h"
#include "HUD/DialogHUD.h"

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
}

void AVisualNovelHUD::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(mUIClass))
	{
		mDialogWidget = CreateWidget<UDialogHUD>(GetWorld(), mUIClass);
		if (IsValid(mDialogWidget))
		{
			mDialogWidget->AddToViewport();
		}
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
