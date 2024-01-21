#pragma once
#include "Engine.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/Border.h>
#include <Components/UniformGridPanel.h>
#include <Components/Button.h>
#include <Components/Throbber.h>
#include <Components/ComboBoxString.h>
#include "EnhancedInputComponent.h"
#include "VNModule.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestDelegate);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCallChoiceButton, int32, Index);

static constexpr FLinearColor LeftSpriteColor(0.85f, 0.04f, 0.07f);
static constexpr FLinearColor RightSpriteColor(0.9f, 0.4f, 0.35f);

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
	LeftSpriteHidden,
	RightSpriteHidden,
	AllSpriteHidden,
};

UENUM(BlueprintType)
enum class EVisualFX : uint8
{
	NoFX,
	CamShake,
};

UENUM(BlueprintType)
enum class EDialogState : uint8
{
	None,
	Typing,
	FinishedTyping,
	Choice,
};

UENUM(BlueprintType)
enum class ELanguage : uint8
{
	Korean,
	English,
	//Japanese,
	Max,
};

USTRUCT(BlueprintType)
struct FChoiceInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ChoicesText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 SelectedChoiceRowIndex;
};

USTRUCT(BlueprintType)
struct FDialogInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> BGImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> LeftSpriteImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> RightSpriteImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FChoiceInfo> ChoiceInfo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> DialogText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECharacterName CharacterName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECharacterSetting CharacterSetting;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EVisualFX VisualFX;

	FDialogInfo() 
	{
		CharacterName = ECharacterName::Amanda;
		CharacterSetting = ECharacterSetting::LeftSpriteSpeaking;
		VisualFX = EVisualFX::NoFX;
	}
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