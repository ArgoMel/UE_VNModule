#include "Widget/Setting/SettingUI.h"
#include "HUD/VisualNovelHUD.h"
#include "HUD/DialogHUD.h"
//#include "Interface/HUDInterface.h"
#include "VisualNovelGameInstance.h"

constexpr float MinAutoDuration=1.f;
constexpr float MaxAutoDuration=10.f;

void USettingUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mGameInstance = GetWorld()->GetGameInstance<UVisualNovelGameInstance>();
}

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
	mAutoDecreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("DecDuration_Button")));
	mAutoIncreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("IncDuration_Button")));
	mCountdownDurationText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CountdownDuration_Text")));

	mApplyButton->OnClicked.AddDynamic(this, &USettingUI::ApplyButtonClicked);
	mCloseButton->OnClicked.AddDynamic(this, &USettingUI::CloseButtonClicked);
	mAutoDecreaseButton->OnClicked.AddDynamic(this, &USettingUI::AutoDecreaseButtonClicked);
	mAutoIncreaseButton->OnClicked.AddDynamic(this, &USettingUI::AutoIncreaseButtonClicked);
	//settext 할때 델리게이트가 해제된다. 얘만 그런듯
	//mCountdownDurationText->TextDelegate.BindDynamic(this, &USettingUI::SetAutoDurationText);
	Language_CB->OnSelectionChanged.AddDynamic(this,&USettingUI::LanguageCBChanged);

	SetAutoDurationText();

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

void USettingUI::AutoDecreaseButtonClicked()
{
	mGameInstance->ResetAutoModeDuration = 
		FMath::Clamp(mGameInstance->ResetAutoModeDuration-1,MinAutoDuration, MaxAutoDuration);
	mGameInstance->AutoModeDuration= mGameInstance->ResetAutoModeDuration;
	SetAutoDurationText();
}

void USettingUI::AutoIncreaseButtonClicked()
{
	mGameInstance->ResetAutoModeDuration =
		FMath::Clamp(mGameInstance->ResetAutoModeDuration + 1, MinAutoDuration, MaxAutoDuration);
	mGameInstance->AutoModeDuration = mGameInstance->ResetAutoModeDuration;
	SetAutoDurationText();
}

void USettingUI::SetAutoDurationText()
{
	FString string = FString::Printf(TEXT("%02i 초"), (int32)mGameInstance->ResetAutoModeDuration);
	mCountdownDurationText->SetText(FText::FromString(string));
}

void USettingUI::LanguageCBChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(!IsValid(mGameInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("No UVisualNovelGameInstance"));
		return;
	}
	mGameInstance->Language = (ELanguage)Language_CB->FindOptionIndex(SelectedItem);
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
