#pragma once
#include "../../VNModule.h"
#include "GameFramework/PlayerController.h"
#include "VisualNovelPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class AVisualNovelHUD;

UCLASS()
class VNMODULE_API AVisualNovelPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AVisualNovelPlayerController();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> mIMC;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> mNextDialogKey;

	TObjectPtr<AVisualNovelHUD> mVNHUD;

protected:
	void NextDialog();

public:
	UFUNCTION(BlueprintCallable)
	void SetNewController();

	UInputAction* GetNextDialogKey()
	{
		return mNextDialogKey;
	}
};