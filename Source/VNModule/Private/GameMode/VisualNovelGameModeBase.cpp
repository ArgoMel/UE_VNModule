#include "GameMode/VisualNovelGameModeBase.h"
#include "PlayerController/VisualNovelPlayerController.h"
#include "HUD/VisualNovelHUD.h"

AVisualNovelGameModeBase::AVisualNovelGameModeBase()
{
	PlayerControllerClass = AVisualNovelPlayerController::StaticClass();
	HUDClass = AVisualNovelHUD::StaticClass();
	//DefaultPawnClass = nullptr;
}

AVisualNovelGameModeBase* AVisualNovelGameModeBase::GetVNGameMode_Implementation()
{
	return this;
}
