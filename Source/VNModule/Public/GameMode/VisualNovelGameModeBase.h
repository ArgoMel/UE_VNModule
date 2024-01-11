#pragma once
#include "../../VNModule.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/GameModeInterface.h"
#include "VisualNovelGameModeBase.generated.h"

UCLASS()
class VNMODULE_API AVisualNovelGameModeBase : public AGameModeBase,public IGameModeInterface
{
	GENERATED_BODY()
public:
	AVisualNovelGameModeBase();

protected:
	virtual AVisualNovelGameModeBase* GetVNGameMode_Implementation() override;
};
