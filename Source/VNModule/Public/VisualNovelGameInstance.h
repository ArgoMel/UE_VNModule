#pragma once
#include "../VNModule.h"
#include "Engine/GameInstance.h"
#include "VisualNovelGameInstance.generated.h"

class UVNSaveSetting;

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
	TObjectPtr<UDataTable>	mDisplayName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UDataTable>	mTextStyleTable;

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	TObjectPtr<UVNSaveSetting>	mSaveSetting;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Setting|Dialog")
	float DialogSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Setting|Language")
	ELanguage Language;

	UPROPERTY(BlueprintReadWrite, Category = "Setting|Auto")
	float AutoModeDuration;

	UPROPERTY(BlueprintReadWrite, Category = "Setting|Font")
	FName FontName;
	UPROPERTY(BlueprintReadWrite, Category = "Setting|Font")
	int32 FontSize;

	UPROPERTY(BlueprintReadWrite, Category = "Setting|Sound")
	TArray<float> Volumes;

public:
	const FDialogInfo* FindDialogInfoData(const FName& name);
	const FDisplayName* FindDisplayNameData(const FName& name);
	const FRichTextStyleRow* FindTextStyleData(const FName& name);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveSetting();
	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadSetting();
	
	TArray<FName> GetFontNames()
	{
		return mFontNames;
	}
};
