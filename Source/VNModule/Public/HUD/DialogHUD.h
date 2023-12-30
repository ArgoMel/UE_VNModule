#pragma once
#include "../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "DialogHUD.generated.h"

UCLASS()
class VNMODULE_API UDialogHUD : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct();

private:
	TObjectPtr<UTextBlock> mCharacterNameText;
	TObjectPtr<UTextBlock> mDialogText;
	TObjectPtr<UImage> mBGImage;
	TObjectPtr<UImage> mLeftSpriteImage;
	TObjectPtr<UImage> mRightSpriteImage;

protected:
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
	void NextDialog();
};