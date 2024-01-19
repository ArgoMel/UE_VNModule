#pragma once
#include "../../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "LogMain.generated.h"

class ULogGridPanel;

UCLASS()
class VNMODULE_API ULogMain : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct();

private:
	TObjectPtr<ULogGridPanel> mLogGridPanel;
	TObjectPtr<UButton> mLogButton;

protected:
	UFUNCTION()
	void LogButtonClicked();

public:
	ULogGridPanel* GetLogGridPanel()
	{
		return mLogGridPanel;
	}
};
