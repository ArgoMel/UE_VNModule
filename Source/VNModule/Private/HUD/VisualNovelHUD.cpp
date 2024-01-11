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
