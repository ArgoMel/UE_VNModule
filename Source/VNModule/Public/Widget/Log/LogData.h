#pragma once
#include "../../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "LogData.generated.h"

UCLASS()
class VNMODULE_API ULogData : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

private:
	TObjectPtr<UTextBlock> mLogCharacterNameText;
	TObjectPtr<UTextBlock> mLogDialogText;
	TObjectPtr<UTextBlock> mHourText;
	TObjectPtr<UTextBlock> mMinuteText;
	TObjectPtr<UTextBlock> mSecondText;
	TObjectPtr<UImage> mSpriteImage;

public:
	UTextBlock* GetLogCharacterNameText()
	{
		return mLogCharacterNameText;
	}
	UTextBlock* GetLogDialogText()
	{
		return mLogDialogText;
	}
	UTextBlock* GetHourText()
	{
		return mHourText;
	}
	UTextBlock* GetMinuteText()
	{
		return mMinuteText;
	}
	UTextBlock* GetSecondText()
	{
		return mSecondText;
	}
	UImage* GetSpriteImage()
	{
		return mSpriteImage;
	}
};
