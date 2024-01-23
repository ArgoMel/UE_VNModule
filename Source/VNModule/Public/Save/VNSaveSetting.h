#pragma once
#include "../../VNModule.h"
#include "GameFramework/SaveGame.h"
#include "VNSaveSetting.generated.h"

UCLASS()
class VNMODULE_API UVNSaveSetting : public USaveGame
{
	GENERATED_BODY()
public:
	UVNSaveSetting();

public:
	UPROPERTY(BlueprintReadWrite, Category = "Slot")
	FString SlotName;
	UPROPERTY(BlueprintReadWrite, Category = "Slot")
	int32 UserIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	TArray<float> Volumes;
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	ELanguage Language;
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	FName FontName;
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	int32 FontSize;
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	float DialogSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	float AutoModeDuration;
};
