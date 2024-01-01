#pragma once
#include "../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "ChoiceButton.generated.h"

UCLASS()
class VNMODULE_API UChoiceButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UChoiceButton(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct();

private:
	TObjectPtr<UCanvas> mChoiceCanvas;
	TObjectPtr<UButton> mChoiceButton;
	TObjectPtr<UTextBlock> mChoiceText;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Choice")
	int32 mButtonIndex;

protected:
	UFUNCTION()
	void ChoiceClicked();

public:
	UTextBlock* GetChoiceText()
	{
		return mChoiceText;
	}
};
