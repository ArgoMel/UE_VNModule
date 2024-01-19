#pragma once
#include "../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "DialogHUD.generated.h"

class UChoiceButton;
class UChoicesGridPanel;
class UVisualNovelGameInstance;

UCLASS()
class VNMODULE_API UDialogHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UDialogHUD(const FObjectInitializer& ObjectInitializer);
protected:
	//construct보다 더 빨리 실행된다.
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	TObjectPtr<UTextBlock> mCharacterNameText;
	TObjectPtr<UTextBlock> mDialogText;
	TObjectPtr<UTextBlock> mContinueText;
	TObjectPtr<UImage> mBGImage;
	TObjectPtr<UImage> mLeftSpriteImage;
	TObjectPtr<UImage> mRightSpriteImage;
	TObjectPtr<UBorder> mDialogBorder;
	TObjectPtr<UChoicesGridPanel>	mChoiceGridPanel;
	TObjectPtr<UThrobber>	mTypingThrobber;

	TSubclassOf<UUserWidget>	mChoiceButtonClass;
	TObjectPtr<UVisualNovelGameInstance>	mGameInstance;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> mShakeAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> mFadeBordersAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> mFadeContinueTextAnim;

	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	FTimerHandle mLetterTimer;
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	FString mCurDialogText;
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	EDialogState mDialogState;
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
	TArray<TObjectPtr<UChoiceButton>>	mChoiceButtonWidgets;
	UPROPERTY(BlueprintReadWrite, Category = "Choice")
	TObjectPtr<UChoiceButton>	mChoiceButtonWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Choice")
	int32 mButtonIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Choice")
	bool mIsChoiceTriggered;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound")
	TArray<TObjectPtr<USoundBase>>	mShakeSound;
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound")
    TObjectPtr<USoundBase>	mChoiceSound;
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound")
    TObjectPtr<USoundBase>	mSkipSound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase>	mBGSound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase>	mMusicSound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase>	mLetterSound;

public:
	UFUNCTION(BlueprintPure, Category = "Dialog")
	FDialogInfo GetDTInfo();
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void SetHUDElements(FDialogInfo dialogInfo);
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void SetLetterByLetter();
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void DialogLogic();
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void ClearDialog();
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void SetNextDialogRowIndex(int32 index);
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void RefreshData();
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void SkipDialog();
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void ContinueDialog(bool hasChoice,int32 selectedIndex=0);
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void ToggleBorders(bool bordersOn);
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void BordersOn();
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void ToggleDialogState(EDialogState state);
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void SetCharacterSettings(bool bIsLeftSpriteHighlighted);

	UFUNCTION(BlueprintNativeEvent, Category = "Choice")
	void CreateChoices();
	void CreateChoices_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Choice")
	void ClickChoice(int32 index);
	void ClickChoice_Implementation(int32 index);
	UFUNCTION(BlueprintCallable, Category = "Choice")
	void SelectChoiceRowIndex(int32 selectedIndex);
	
	UFUNCTION(BlueprintCallable, Category = "VFX")
	void PlayVisualFX(EVisualFX visualFX);

	UFUNCTION(BlueprintCallable , Category = "Input")
	void NextDialog();

	TArray<TObjectPtr<UChoiceButton>> GetChoiceButtons()
	{
		return mChoiceButtonWidgets;
	}
};