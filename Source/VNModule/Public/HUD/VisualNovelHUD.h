#pragma once
#include "../../VNModule.h"
#include "GameFramework/HUD.h"
#include "Interface/HUDInterface.h"
#include "VisualNovelHUD.generated.h"

class UDialogHUD;

UCLASS()
class VNMODULE_API AVisualNovelHUD : public AHUD, public IHUDInterface
{
	GENERATED_BODY()
public:
	AVisualNovelHUD();
protected:
	virtual void BeginPlay()	override;

private:
	TSubclassOf<UUserWidget>	mUIClass;
	TObjectPtr<UDialogHUD>	mDialogWidget;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase>	mLogButtonSound;

public:
	AVisualNovelHUD* GetVNHUD_Implementation();

	void NextDialog();

	UDialogHUD* GetDialogWidget()
	{
		return mDialogWidget;
	}
	USoundBase* GetLogButtonSound()
	{
		return mLogButtonSound;
	}
};
