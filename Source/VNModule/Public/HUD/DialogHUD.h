#pragma once
#include "../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "DialogHUD.generated.h"

class UChoiceButton;
class UChoicesGridPanel;
class ULogMain;
class ULogData;
class USettingUI;
class UVisualNovelGameInstance;
class UVNSaveGame;

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
	TObjectPtr<UTextBlock> mTimeRemainingText;
	TObjectPtr<UTextBlock> mAutoSkipDurationText;
	TObjectPtr<UImage> mBGImage;
	TObjectPtr<UImage> mLeftSpriteImage;
	TObjectPtr<UImage> mRightSpriteImage;
	TObjectPtr<UBorder> mDialogBorder;
	TObjectPtr<UChoicesGridPanel>	mChoiceGridPanel;
	TObjectPtr<UThrobber>	mTypingThrobber;
	TObjectPtr<UButton>	mLogButton;
	TObjectPtr<UButton>	mAutoButton;
	TObjectPtr<UButton>	mSettingButton;
	TObjectPtr<UButton>	mSaveButton;
	TObjectPtr<UButton>	mLoadButton;

	TSubclassOf<UUserWidget>	mLogDataClass;
	TSubclassOf<UUserWidget>	mLogMainClass;
	TSubclassOf<UUserWidget>	mSettingUIClass;
	TSubclassOf<UUserWidget>	mChoiceButtonClass;

	TObjectPtr<UVisualNovelGameInstance>	mGameInstance;
	FSlateFontInfo mCurFont;

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
	bool mIsChoiceTriggered;

	UPROPERTY(BlueprintReadWrite, Category = "Log")
	TObjectPtr<ULogMain>	mLogMainWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Log")
	TObjectPtr<ULogData>	mLogDataWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Log")
	int32	mLogIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	FTimerHandle mAutoModeTimer;
	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	float mAutoModeDuration;
	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	bool mIsAutoModeOn;

	UPROPERTY(BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USettingUI>	mSettingUIWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Setting")
	TObjectPtr<UVNSaveGame>	mSaveGame;

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

protected:
	UFUNCTION()
	void LogButtonClicked();
	UFUNCTION()
	void AutoButtonClicked();
	UFUNCTION()
	void AutoButtonHovered();
	UFUNCTION()
	void AutoButtonUnHovered();
	UFUNCTION()
	void SettingButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void CreateWidgets();
	UFUNCTION(BlueprintCallable, Category = "Game")
	void InitGame();
	UFUNCTION(BlueprintCallable, Category = "Game")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "Game")
	void LoadGame();

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
	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void SwitchDataTable(UDataTable* dataTable);

	UFUNCTION(BlueprintNativeEvent, Category = "Choice")
	void CreateChoices();
	void CreateChoices_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Choice")
	void ClickChoice(int32 index);
	void ClickChoice_Implementation(int32 index);
	UFUNCTION(BlueprintCallable, Category = "Choice")
	void SelectChoiceRowIndex(int32 selectedIndex);

	UFUNCTION(BlueprintCallable, Category = "Log")
	void GenerateLogData();
	UFUNCTION(BlueprintCallable, Category = "Log")
	void SetLogData();
	UFUNCTION(BlueprintPure, Category = "Log")
	FString GetHour()
	{
		int32 hour = FDateTime::Now().GetHour();
		return FString::Printf(TEXT("%02i"), hour);
	}
	UFUNCTION(BlueprintPure, Category = "Log")
	FString GetMinute()
	{
		int32 minute = FDateTime::Now().GetMinute();
		return FString::Printf(TEXT("%02i"), minute);
	}
	UFUNCTION(BlueprintPure, Category = "Log")
	FString GetSecond()
	{
		int32 second = FDateTime::Now().GetSecond();
		return FString::Printf(TEXT("%02i"), second);
	}

	UFUNCTION(BlueprintNativeEvent, Category = "Auto")
	void AutoModeStart();
	void AutoModeStart_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Auto")
	void AutoModeCountdown();
	void AutoModeCountdown_Implementation();
	UFUNCTION(BlueprintCallable, Category = "Auto")
	void ClearAndResetAutoCountdown();
	UFUNCTION(BlueprintCallable, Category = "Auto")
	void ToggleTimeRemainingText();
	
	UFUNCTION(BlueprintCallable, Category = "VFX")
	void PlayVisualFX(EVisualFX visualFX);

	UFUNCTION(BlueprintCallable , Category = "Input")
	void NextDialog();

	void SetCharacterNameText();
	void SetFont(FSlateFontInfo font);
	bool IsDialogFinished()
	{
		return mDialogFinished;
	}
	void SetAutoModeDuration(float value)
	{
		mAutoModeDuration = value;
	}
	TArray<TObjectPtr<UChoiceButton>> GetChoiceButtons()
	{
		return mChoiceButtonWidgets;
	}
};