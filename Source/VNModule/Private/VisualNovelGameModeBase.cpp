#include "VisualNovelGameModeBase.h"
#include "HUD/VisualNovelHUD.h"

AVisualNovelGameModeBase::AVisualNovelGameModeBase()
{
	HUDClass = AVisualNovelHUD::StaticClass();
}
