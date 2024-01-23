#pragma once
#include "../../VNModule.h"
#include "GameFramework/HUD.h"
#include "Interface/HUDInterface.h"
#include "VisualNovelHUD.generated.h"

class UDialogHUD;
class UVisualNovelGameInstance;

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

	TArray<TObjectPtr<USoundClass>>	mSoundClasses;
	TObjectPtr<USoundMix>	mSoundMix;

	TObjectPtr<UVisualNovelGameInstance>	mGameInstance;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase>	mLogButtonSound;

public:
	AVisualNovelHUD* GetVNHUD_Implementation();

	void NextDialog();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SetVolume(float value, int32 index);

	UDialogHUD* GetDialogWidget()
	{
		return mDialogWidget;
	}
	USoundBase* GetLogButtonSound()
	{
		return mLogButtonSound;
	}
};
