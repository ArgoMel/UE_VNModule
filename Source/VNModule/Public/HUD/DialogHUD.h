#pragma once
#include "../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "DialogHUD.generated.h"

class UChoiceButton;
//class UChoiceButton;

UCLASS()
class VNMODULE_API UDialogHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UDialogHUD(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct();

private:
	TObjectPtr<UTextBlock> mCharacterNameText;
	TObjectPtr<UTextBlock> mDialogText;
	TObjectPtr<UImage> mBGImage;
	TObjectPtr<UImage> mLeftSpriteImage;
	TObjectPtr<UImage> mRightSpriteImage;
	TObjectPtr<UBorder> mDialogBorder;
	//TObjectPtr<UChoiceButton>	mChoiceButtonWidget;

	TSubclassOf<UUserWidget>	mChoiceButtonClass;
	TObjectPtr<UChoiceButton>	mChoiceButtonWidget;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> mShakeAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> mFadeBordersAnim;

	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	FTimerHandle mLetterTimer;
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	FString mCurDialogText;
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	int32 mRowNumber;
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	int32 mLetterIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	bool mDialogFinished;
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	bool mCanSkipDialog;
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	bool mDisableLMB;

	UPROPERTY(BlueprintReadWrite, Category = "Choice")
	int32 mButtonIndex;

public:
	UFUNCTION(BlueprintCallable)
	FDialogInfo GetDTInfo();
	UFUNCTION(BlueprintCallable)
	void SetHUDElements(FDialogInfo dialogInfo);
	UFUNCTION(BlueprintCallable)
	void SetLetterByLetter();
	UFUNCTION(BlueprintCallable)
	void DialogLogic();
	UFUNCTION(BlueprintCallable)
	void ClearDialog();
	UFUNCTION(BlueprintCallable)
	void SetNextDialogRowIndex(int32 index);
	UFUNCTION(BlueprintCallable)
	void RefreshData();
	UFUNCTION(BlueprintCallable)
	void SkipDialog();
	UFUNCTION(BlueprintCallable)
	void ContinueDialog();
	UFUNCTION(BlueprintCallable)
	void PlayVisualFX(EVisualFX visualFX);
	UFUNCTION(BlueprintCallable)
	void ToggleBorders(bool bordersOn);
	UFUNCTION(BlueprintCallable)
	void BordersOn();

	UFUNCTION(BlueprintNativeEvent, Category = "Choice")
	void CreateChoices();
	void CreateChoices_Implementation();

	UFUNCTION(BlueprintCallable , Category = "Input")
	void NextDialog();
};