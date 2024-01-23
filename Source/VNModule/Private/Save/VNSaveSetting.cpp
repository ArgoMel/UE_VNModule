#include "Save/VNSaveSetting.h"

UVNSaveSetting::UVNSaveSetting()
{
	SlotName = FString(TEXT("Setting"));
	UserIndex = 0;

	Volumes.Init(1.f, (int32)ESoundKind::Max);
	Language = ELanguage::Korean;
	FontName = FName(TEXT("Roboto"));
	FontSize = 24;
	DialogSpeed = 0.05f;
	AutoModeDuration = 4.f;
}
