#pragma once
#include "../../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "SettingUI.generated.h"

class AVisualNovelHUD;
class UVisualNovelGameInstance;

UCLASS()
class VNMODULE_API USettingUI : public UUserWidget
{
	GENERATED_BODY()
public:
	USettingUI(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	TObjectPtr<UButton> mApplyButton;
	TObjectPtr<UButton> mCloseButton;
	TObjectPtr<UButton> mAutoDecreaseButton;
	TObjectPtr<UButton> mAutoIncreaseButton;
	TObjectPtr<UButton> mFontSizeDecreaseButton;
	TObjectPtr<UButton> mFontSizeIncreaseButton;
	TObjectPtr<UTextBlock> mCountdownDurationText;
	TObjectPtr<UTextBlock> mDialogSpeedValueText;
	TObjectPtr<UTextBlock> mFontSizeText;
	TObjectPtr<UTextBlock> mPreviewDialogText;
	TObjectPtr<UTextBlock> mMasterVolumeText;
	TObjectPtr<UTextBlock> mMusicVolumeText;
	TObjectPtr<UTextBlock> mSFXVolumeText;
	TObjectPtr<UTextBlock> mVoiceVolumeText;
	TObjectPtr<USlider> mDialogSpeedSlider;
	TObjectPtr<USlider> mMasterVolumeSlider;
	TObjectPtr<USlider> mMusicVolumeSlider;
	TObjectPtr<USlider> mSFXVolumeSlider;
	TObjectPtr<USlider> mVoiceVolumeSlider;

	TObjectPtr<UVisualNovelGameInstance>	mGameInstance;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> Language_CB;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> Font_CB;

	UPROPERTY(BlueprintReadWrite, Category = "Font")
	FString mPreviewFont;

protected:
	UFUNCTION()
	void ApplyButtonClicked();
	UFUNCTION()
	void CloseButtonClicked();
	UFUNCTION()
	void AutoDecreaseButtonClicked();
	UFUNCTION()
	void AutoIncreaseButtonClicked();
	UFUNCTION()
	void FontSizeDecreaseButtonClicked();
	UFUNCTION()
	void FontSizeIncreaseButtonClicked();
	UFUNCTION()
	void DialogSpeedSliderChanged(float value);
	UFUNCTION()
	void DialogSpeedSliderEnd();
	UFUNCTION()
	void MasterVolumeSliderChanged(float value);
	UFUNCTION()
	void MusicVolumeSliderChanged(float value);
	UFUNCTION()
	void SFXVolumeSliderChanged(float value);
	UFUNCTION()
	void VoiceVolumeSliderChanged(float value);
	UFUNCTION()
	void LanguageCBChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void FontCBChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable)
	void SetAutoDurationText();
	UFUNCTION(BlueprintCallable)
	void ApplyFontSize();

	UFUNCTION(BlueprintPure)
	FString CalculateFontSize();
	UFUNCTION(BlueprintPure)
	AVisualNovelHUD* GetVNHUD();
};
