#include "Widget/Setting/SettingUI.h"
#include "HUD/VisualNovelHUD.h"
#include "HUD/DialogHUD.h"
//#include "Interface/HUDInterface.h"
#include "VisualNovelGameInstance.h"

constexpr int32 MinFontSize = 10;
constexpr int32 MaxFontSize = 40;
constexpr float MinAutoDuration=1.f;
constexpr float MaxAutoDuration=10.f;

USettingUI::USettingUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mPreviewFont = FString(TEXT("Sample 예시"));
}

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

	if(!IsValid(mGameInstance))
	{
		return;
	}
	TArray<FName> fontNames = mGameInstance->GetFontNames();
	for (auto& fontName: fontNames)
	{
		Font_CB->AddOption(fontName.ToString());
	}
	Font_CB->SetSelectedOption(mGameInstance->FontName.ToString());
}

void USettingUI::NativeConstruct()
{
	Super::NativeConstruct();
	mVolumeTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("MasterVolumeValue_Text"))));
	mVolumeTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("MusicVolumeValue_Text"))));
	mVolumeTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("SFXVolumeValue_Text"))));
	mVolumeTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("VoiceVolumeValue_Text"))));
	mApplyButton = Cast<UButton>(GetWidgetFromName(TEXT("Apply_Button")));
	mCloseButton = Cast<UButton>(GetWidgetFromName(TEXT("Close_Button")));
	mAutoDecreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("DecDuration_Button")));
	mAutoIncreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("IncDuration_Button")));
	mFontSizeDecreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("DecFontSize_Button")));
	mFontSizeIncreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("IncFontSize_Button")));
	mCountdownDurationText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CountdownDuration_Text")));
	mDialogSpeedValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DialogSpeedValue_Text")));
	mFontSizeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("FontSize_Text")));
	mPreviewDialogText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PreviewDialog_Text")));
	mDialogSpeedSlider = Cast<USlider>(GetWidgetFromName(TEXT("DialogSpeed_Slider")));
	mVolumeSliders.Add(Cast<USlider>(GetWidgetFromName(TEXT("MasterVolume_Slider"))));
	mVolumeSliders.Add(Cast<USlider>(GetWidgetFromName(TEXT("MusicVolume_Slider"))));
	mVolumeSliders.Add(Cast<USlider>(GetWidgetFromName(TEXT("SFXVolume_Slider"))));
	mVolumeSliders.Add(Cast<USlider>(GetWidgetFromName(TEXT("VoiceVolume_Slider"))));

	mApplyButton->OnClicked.AddDynamic(this, &USettingUI::ApplyButtonClicked);
	mCloseButton->OnClicked.AddDynamic(this, &USettingUI::CloseButtonClicked);
	mAutoDecreaseButton->OnClicked.AddDynamic(this, &USettingUI::AutoDecreaseButtonClicked);
	mAutoIncreaseButton->OnClicked.AddDynamic(this, &USettingUI::AutoIncreaseButtonClicked);
	mFontSizeDecreaseButton->OnClicked.AddDynamic(this, &USettingUI::FontSizeDecreaseButtonClicked);
	mFontSizeIncreaseButton->OnClicked.AddDynamic(this, &USettingUI::FontSizeIncreaseButtonClicked);
	//settext 할때 델리게이트가 해제된다. 텍스트만 그런듯
	//mCountdownDurationText->TextDelegate.BindDynamic(this, &USettingUI::SetAutoDurationText);
	mDialogSpeedSlider->OnValueChanged.AddDynamic(this,&USettingUI::DialogSpeedSliderChanged);
	mDialogSpeedSlider->OnMouseCaptureEnd.AddDynamic(this,&USettingUI::DialogSpeedSliderEnd);
	mVolumeSliders[(int32)ESoundKind::Master]->OnValueChanged.AddDynamic(
		this, &USettingUI::MasterVolumeSliderChanged);
	mVolumeSliders[(int32)ESoundKind::Music]->OnValueChanged.AddDynamic(
		this, &USettingUI::MusicVolumeSliderChanged);
	mVolumeSliders[(int32)ESoundKind::SFX]->OnValueChanged.AddDynamic(
		this, &USettingUI::SFXVolumeSliderChanged);
	mVolumeSliders[(int32)ESoundKind::Voice]->OnValueChanged.AddDynamic(
		this, &USettingUI::VoiceVolumeSliderChanged);
	Language_CB->OnSelectionChanged.AddDynamic(this,&USettingUI::LanguageCBChanged);
	Font_CB->OnSelectionChanged.AddDynamic(this,&USettingUI::FontCBChanged);

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(controller))
	{
		FInputModeUIOnly inputMode;
		inputMode.SetWidgetToFocus(GetCachedWidget());
		controller->SetInputMode(inputMode);
	}

	Language_CB->SetSelectedOption(Language_CB->GetOptionAtIndex((int32)mGameInstance->Language));
	SetAutoDurationText();
	float displayValue = mDialogSpeedSlider->GetMinValue() / mGameInstance->DialogSpeed;
	mDialogSpeedSlider->SetValue(displayValue);
	DialogSpeedSliderChanged(displayValue);
	FontCBChanged(mGameInstance->FontName.ToString(),ESelectInfo::Type::Direct);
	for (int32 i = 0; i < (int32)ESoundKind::Max; ++i)
	{
		mVolumeSliders[i]->SetValue(mGameInstance->Volumes[i]);
	}
}

void USettingUI::ApplyButtonClicked()
{
	CloseButtonClicked();
}

void USettingUI::CloseButtonClicked()
{
	mGameInstance->SaveSetting();
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
	mGameInstance->AutoModeDuration = 
		FMath::Clamp(mGameInstance->AutoModeDuration -1,MinAutoDuration, MaxAutoDuration);
	SetAutoDurationText();
}

void USettingUI::AutoIncreaseButtonClicked()
{
	mGameInstance->AutoModeDuration =
		FMath::Clamp(mGameInstance->AutoModeDuration + 1, MinAutoDuration, MaxAutoDuration);
	SetAutoDurationText();
}

void USettingUI::FontSizeDecreaseButtonClicked()
{
	mGameInstance->FontSize =
		FMath::Clamp(mGameInstance->FontSize - 1, MinFontSize, MaxFontSize);
	ApplyFontSize();
}

void USettingUI::FontSizeIncreaseButtonClicked()
{
	mGameInstance->FontSize =
		FMath::Clamp(mGameInstance->FontSize + 1, MinFontSize, MaxFontSize);
	ApplyFontSize();
}

void USettingUI::DialogSpeedSliderChanged(float value)
{
	FString string = FString::Printf(TEXT("%.2f 초"), value);
	mDialogSpeedValueText->SetText(FText::FromString(string));
	mGameInstance->DialogSpeed = mDialogSpeedSlider->GetMinValue() / value;
}

void USettingUI::MasterVolumeSliderChanged(float value)
{
	SetVolume(value,(int32)ESoundKind::Master);
}

void USettingUI::MusicVolumeSliderChanged(float value)
{
	SetVolume(value, (int32)ESoundKind::Music);
}

void USettingUI::SFXVolumeSliderChanged(float value)
{
	SetVolume(value, (int32)ESoundKind::SFX);
}

void USettingUI::VoiceVolumeSliderChanged(float value)
{
	SetVolume(value, (int32)ESoundKind::Voice);
}

void USettingUI::DialogSpeedSliderEnd()
{
	AVisualNovelHUD* vnHUD = GetVNHUD();
	UDialogHUD* dialogHUD = vnHUD->GetDialogWidget();
	if (!dialogHUD->IsDialogFinished())
	{
		dialogHUD->SetLetterByLetter();
	}
}

void USettingUI::SetAutoDurationText()
{
	FString string = FString::Printf(TEXT("%02i 초"), (int32)mGameInstance->AutoModeDuration);
	mCountdownDurationText->SetText(FText::FromString(string));
	AVisualNovelHUD* vnHUD = GetVNHUD();
	UDialogHUD* dialogHUD = vnHUD->GetDialogWidget();
	dialogHUD->SetAutoModeDuration(mGameInstance->AutoModeDuration);
}

void USettingUI::ApplyFontSize()
{
	mFontSizeText->SetText(FText::FromString(CalculateFontSize()));
	FSlateFontInfo fontInfo = mPreviewDialogText->GetFont();
	fontInfo.Size = mGameInstance->FontSize;
	mPreviewDialogText->SetFont(fontInfo);

	AVisualNovelHUD* vnHUD = GetVNHUD();
	UDialogHUD* dialogHUD = vnHUD->GetDialogWidget();
	dialogHUD->SetFont(fontInfo);
}

void USettingUI::SetVolume(float value, int32 index)
{
	GetVNHUD()->SetVolume(value, index);
	FString string = FString::Printf(TEXT("%i"), (int32)(value * 100));
	mVolumeTexts[index]->SetText(FText::FromString(string));
}

FString USettingUI::CalculateFontSize()
{
	return FString::Printf(TEXT("폰트 크기 : %02i"), mGameInstance->FontSize);
}

void USettingUI::LanguageCBChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	mGameInstance->Language = (ELanguage)Language_CB->FindOptionIndex(SelectedItem);
	AVisualNovelHUD* vnHUD = GetVNHUD();
	UDialogHUD* dialogHUD = vnHUD->GetDialogWidget();
	dialogHUD->SkipDialog();
	if(!dialogHUD->IsDialogFinished())
	{
		dialogHUD->ClearDialog();
	}
	FSlateFontInfo fontInfo = mPreviewDialogText->GetFont();
	dialogHUD->SetFont(fontInfo);
	dialogHUD->SetCharacterNameText();
}

void USettingUI::FontCBChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	mGameInstance->FontName= FName(*SelectedItem);
	FSlateFontInfo fontInfo = mGameInstance->FindTextStyleData(mGameInstance->FontName)->TextStyle.Font;
	mPreviewDialogText->SetFont(fontInfo);
	ApplyFontSize();
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
