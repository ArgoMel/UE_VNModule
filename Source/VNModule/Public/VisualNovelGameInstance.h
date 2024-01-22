#pragma once
#include "../VNModule.h"
#include "Engine/GameInstance.h"
#include "VisualNovelGameInstance.generated.h"

UCLASS()
class VNMODULE_API UVisualNovelGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UVisualNovelGameInstance();
protected:
	virtual void Init();

private:
	TArray<FName> mFontNames;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UDataTable>	mDialogInfoTable;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UDataTable>	mTextStyleTable;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Setting|Dialog")
	float DialogSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Setting|Language")
	ELanguage Language;

	UPROPERTY(BlueprintReadWrite, Category = "Setting|Auto")
	float AutoModeDuration;
	UPROPERTY(BlueprintReadWrite, Category = "Setting|Auto")
	float ResetAutoModeDuration;

	UPROPERTY(BlueprintReadWrite, Category = "Setting|Font")
	FName FontName;
	UPROPERTY(BlueprintReadWrite, Category = "Setting|Font")
	int32 FontSize;

public:
	const FDialogInfo* FindDialogInfoData(const FName& name);
	const FRichTextStyleRow* FindTextStyleData(const FName& name);
	
	TArray<FName> GetFontNames()
	{
		return mFontNames;
	}
};
