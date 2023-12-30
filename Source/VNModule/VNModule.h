#pragma once
#include "EngineMinimal.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include "VNModule.generated.h"

UENUM(BlueprintType)
enum class ECharacterName : uint8
{
	Amanda ,
	Jessica ,
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
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECharacterName CharacterName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DialogText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECharacterSetting CharacterSetting;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> BGImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> LeftSpriteImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> RightSpriteImage;
};

template<typename T>
FString EnumToFString(T enumValue)
{
#if !WITH_EDITOR
	return "";
#else
	static FString enumName=TEXT("/Script/VNModule.");
	static bool bEnumNameInit = false;
	// Init it once
	if (bEnumNameInit == false)
	{
		bEnumNameInit = true;
		FString functionSignature(__FUNCSIG__);
		UE_LOG(LogTemp, Display, TEXT("Enum funcsig: [%s]"), *functionSignature);
		int32 firstIDX = functionSignature.Find(TEXT("(enum "), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

		if (firstIDX != INDEX_NONE)
		{
			firstIDX += 6;
			int32 lastIDX = (functionSignature.Len() - 1);

			if (functionSignature.IsValidIndex(firstIDX) && 
				functionSignature.IsValidIndex(lastIDX) && firstIDX < lastIDX)
			{
				enumName.Append(functionSignature.Mid(firstIDX, lastIDX - firstIDX));
				UE_LOG(LogTemp, Display, TEXT("Enum name: [%s]"), *enumName);
			}
		}
	}

	if (enumName.IsEmpty())
	{
		return FString("ENUM_Invalid");
	}
	const UEnum* enumPtr = FindObject<UEnum>(nullptr, *enumName,true);
	if (!enumPtr)
	{
		return FString("ENUM_Invalid");
	}
	return enumPtr->GetDisplayNameTextByIndex(static_cast<int32>(enumValue)).ToString();
#endif
}