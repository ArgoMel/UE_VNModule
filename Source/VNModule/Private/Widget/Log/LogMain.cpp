#include "Widget/Log/LogMain.h"
#include "Widget/Log/LogGridPanel.h"
#include "HUD/VisualNovelHUD.h"
#include "HUD/DialogHUD.h"
#include "Interface/HUDInterface.h"

void ULogMain::NativeConstruct()
{
	Super::NativeConstruct();

	mLogGridPanel = Cast<ULogGridPanel>(GetWidgetFromName(TEXT("WBP_LogGridPanel")));
	mLogButton = Cast<UButton>(GetWidgetFromName(TEXT("Log_Button")));

	mLogButton->OnClicked.AddDynamic(this, &ULogMain::LogButtonClicked);

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(controller))
	{
		FInputModeUIOnly inputMode;
		inputMode.SetWidgetToFocus(GetCachedWidget());
		controller->SetInputMode(inputMode);
	}
}

void ULogMain::LogButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(controller))
	{
		UE_LOG(LogTemp, Warning, TEXT("no controller"));
		return;
	}
	AHUD* hud = controller->GetHUD();
	if (hud->GetClass()->ImplementsInterface(UHUDInterface::StaticClass()))
	{
		auto interfaceVariable = Cast<IHUDInterface>(hud);
		AVisualNovelHUD* vnHUD = interfaceVariable->Execute_GetVNHUD(hud);
		UDialogHUD* dialogHUD = vnHUD->GetDialogWidget();
		FInputModeGameAndUI inputMode;
		inputMode.SetWidgetToFocus(dialogHUD->GetCachedWidget());
		controller->SetInputMode(inputMode);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		UGameplayStatics::PlaySound2D(GetWorld(), vnHUD->GetLogButtonSound());
	}
}
