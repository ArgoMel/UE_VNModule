#pragma once
#include "../../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "SettingUI.generated.h"

class AVisualNovelHUD;

UCLASS()
class VNMODULE_API USettingUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	TObjectPtr<UButton> mApplyButton;
	TObjectPtr<UButton> mCloseButton;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> Language_CB;

protected:
	UFUNCTION()
	void ApplyButtonClicked();
	UFUNCTION()
	void CloseButtonClicked();
	UFUNCTION()
	void LanguageCBChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintPure)
	AVisualNovelHUD* GetVNHUD();
};
