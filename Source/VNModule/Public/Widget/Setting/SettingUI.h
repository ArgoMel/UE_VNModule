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
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	TObjectPtr<UButton> mApplyButton;
	TObjectPtr<UButton> mCloseButton;
	TObjectPtr<UButton> mAutoDecreaseButton;
	TObjectPtr<UButton> mAutoIncreaseButton;
	TObjectPtr<UTextBlock> mCountdownDurationText;

	TObjectPtr<UVisualNovelGameInstance>	mGameInstance;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> Language_CB;

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
	//FText SetAutoDurationText();
	void SetAutoDurationText();
	UFUNCTION()
	void LanguageCBChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintPure)
	AVisualNovelHUD* GetVNHUD();
};
