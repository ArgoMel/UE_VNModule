#pragma once
#include "../../VNModule.h"
#include "GameFramework/SaveGame.h"
#include "VNSaveGame.generated.h"

UCLASS()
class VNMODULE_API UVNSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UVNSaveGame();

public:
	UPROPERTY(BlueprintReadWrite, Category = "Slot")
	FString SlotName;
	UPROPERTY(BlueprintReadWrite, Category = "Slot")
	int32 UserIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	TObjectPtr<UDataTable>	DialogInfoTable;
	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	int32 SavedRowNumber;
};
