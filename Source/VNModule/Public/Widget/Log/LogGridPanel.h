#pragma once
#include "../../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "LogGridPanel.generated.h"

UCLASS()
class VNMODULE_API ULogGridPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct();

private:
	TObjectPtr<UUniformGridPanel> mLogUniformGridPanel;

public:
	UUniformGridPanel* GetLogUniformGridPanel()
	{
		return mLogUniformGridPanel;
	}
};
