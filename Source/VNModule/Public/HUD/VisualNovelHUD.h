#pragma once
#include "../../VNModule.h"
#include "GameFramework/HUD.h"
#include "VisualNovelHUD.generated.h"

UCLASS()
class VNMODULE_API AVisualNovelHUD : public AHUD
{
	GENERATED_BODY()
public:
	AVisualNovelHUD();
protected:
	virtual void BeginPlay()	override;

private:
	TSubclassOf<UUserWidget>	m_UIClass;
	TObjectPtr<class UDialogHUD>	m_DialogWidget;
};
