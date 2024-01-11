#pragma once
#include "../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "ChoiceButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCallChoiceButton, int32, Index);

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
	UPROPERTY(BlueprintReadWrite, Category = "Choice", Meta = (ExposeOnSpawn = true))
	int32 mButtonIndex;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FCallChoiceButton OnCallChoiceButton;

protected:
	UFUNCTION()
	void ChoiceClicked();

public:
	UTextBlock* GetChoiceText()
	{
		return mChoiceText;
	}
};
