#pragma once
#include "../../VNModule.h"
#include "Blueprint/UserWidget.h"
#include "ChoicesGridPanel.generated.h"

UCLASS()
class VNMODULE_API UChoicesGridPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct();
	
private:
	TObjectPtr<UUniformGridPanel> mChoices;

public:
	UUniformGridPanel* GetChoices()
	{
		return mChoices;
	}
};
