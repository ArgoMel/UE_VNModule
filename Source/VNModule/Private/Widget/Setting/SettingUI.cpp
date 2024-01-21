#include "Widget/Setting/SettingUI.h"
#include "HUD/VisualNovelHUD.h"
#include "HUD/DialogHUD.h"
//#include "Interface/HUDInterface.h"
#include "VisualNovelGameInstance.h"

void USettingUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	for (int32 i = 0; i < (int32)ELanguage::Max;++i)
	{
		Language_CB->AddOption(EnumToFString<ELanguage>((ELanguage)i));
	}
	Language_CB->SetSelectedOption(Language_CB->GetOptionAtIndex(0));
}

void USettingUI::NativeConstruct()
{
	Super::NativeConstruct();

	mApplyButton = Cast<UButton>(GetWidgetFromName(TEXT("Apply_Button")));
	mCloseButton = Cast<UButton>(GetWidgetFromName(TEXT("Close_Button")));

	mApplyButton->OnClicked.AddDynamic(this, &USettingUI::ApplyButtonClicked);
	mCloseButton->OnClicked.AddDynamic(this, &USettingUI::CloseButtonClicked);
	Language_CB->OnSelectionChanged.AddDynamic(this,&USettingUI::LanguageCBChanged);

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(controller))
	{
		FInputModeUIOnly inputMode;
		inputMode.SetWidgetToFocus(GetCachedWidget());
		controller->SetInputMode(inputMode);
	}
}

void USettingUI::ApplyButtonClicked()
{
	CloseButtonClicked();
}

void USettingUI::CloseButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	APlayerController* controller = GetOwningPlayer();
	if (!IsValid(controller))
	{
		UE_LOG(LogTemp, Warning, TEXT("no controller"));
		return;
	}
	AVisualNovelHUD* vnHUD = GetVNHUD();
	UDialogHUD* dialogHUD = vnHUD->GetDialogWidget();
	FInputModeGameAndUI inputMode;
	inputMode.SetWidgetToFocus(dialogHUD->GetCachedWidget());
	controller->SetInputMode(inputMode);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::PlaySound2D(GetWorld(), vnHUD->GetLogButtonSound());
}

void USettingUI::LanguageCBChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UVisualNovelGameInstance* gameInst=GetWorld()->GetGameInstance<UVisualNovelGameInstance>();
	if(!IsValid(gameInst))
	{
		UE_LOG(LogTemp, Warning, TEXT("No UVisualNovelGameInstance"));
		return;
	}
	gameInst->Language = (ELanguage)Language_CB->FindOptionIndex(SelectedItem);
	AVisualNovelHUD* vnHUD = GetVNHUD();
	UDialogHUD* dialogHUD = vnHUD->GetDialogWidget();
	dialogHUD->SkipDialog();
	if(!dialogHUD->IsDialogFinished())
	{
		dialogHUD->ClearDialog();
	}
}

AVisualNovelHUD* USettingUI::GetVNHUD()
{
	APlayerController* controller = GetOwningPlayer();
	if (!IsValid(controller))
	{
		UE_LOG(LogTemp, Warning, TEXT("no controller"));
		return nullptr;
	}
	//AHUD* hud = controller->GetHUD();
	//if (hud->GetClass()->ImplementsInterface(UHUDInterface::StaticClass()))
	//{
	//	return nullptr;
	//}
	//AVisualNovelHUD* vnHUD = Cast<IHUDInterface>(hud)->Execute_GetVNHUD(hud);
	return Cast<AVisualNovelHUD>(controller->GetHUD());
}
