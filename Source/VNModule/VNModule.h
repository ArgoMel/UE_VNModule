#pragma once
#include "EngineMinimal.h"
#include "VNModule.generated.h"

UENUM(BlueprintType)
enum class ECharacterName : uint8
{
	Amanda,
	Jessica,
};

UENUM(BlueprintType)
enum class ECharacterSetting : uint8
{
	LeftSpriteSpeaking,
	RightSpriteSpeaking,
	LeftSpriteNotSpeaking,
	RightSpriteNotSpeaking,
	AllSpriteNotSpeaking,
};

USTRUCT(BlueprintType)
struct FDialogInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ECharacterName CharacterName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString DialogText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ECharacterSetting CharacterSetting;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UTexture2D BGImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UTexture2D LeftSpriteImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UTexture2D RightSpriteImage;
};