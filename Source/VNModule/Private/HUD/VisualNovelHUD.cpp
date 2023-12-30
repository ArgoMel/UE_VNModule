#include "HUD/VisualNovelHUD.h"
#include "HUD/DialogHUD.h"

AVisualNovelHUD::AVisualNovelHUD()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget>	WBP_DialogHUD(TEXT(
		"/Game/VisualNovel/BluePrints/HUD/WBP_DialogHUD.WBP_DialogHUD_C"));
	if (WBP_DialogHUD.Succeeded())
	{
		m_UIClass = WBP_DialogHUD.Class;
	}
}

void AVisualNovelHUD::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(m_UIClass))
	{
		m_DialogWidget = CreateWidget<UDialogHUD>(GetWorld(), m_UIClass);
		if (IsValid(m_DialogWidget))
		{
			m_DialogWidget->AddToViewport();
		}
	}
}

void AVisualNovelHUD::NextDialog()
{
	m_DialogWidget->NextDialog();
}
