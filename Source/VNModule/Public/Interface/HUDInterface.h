#pragma once
#include "../../VNModule.h"
#include "UObject/Interface.h"
#include "HUDInterface.generated.h"

class AVisualNovelHUD;

UINTERFACE(MinimalAPI)
class UHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class VNMODULE_API IHUDInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AVisualNovelHUD* GetVNHUD();
};
