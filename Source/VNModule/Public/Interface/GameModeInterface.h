#pragma once
#include "../../VNModule.h"
#include "UObject/Interface.h"
#include "GameModeInterface.generated.h"

class AVisualNovelGameModeBase;

UINTERFACE(MinimalAPI)
class UGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class VNMODULE_API IGameModeInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AVisualNovelGameModeBase* GetVNGameMode();
};
