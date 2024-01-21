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

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UDataTable>	mDialogInfoTable;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Setting|Language")
	ELanguage Language;
	UPROPERTY(BlueprintReadWrite, Category = "Setting|Auto")
	float AutoModeDuration;
	UPROPERTY(BlueprintReadWrite, Category = "Setting|Auto")
	float ResetAutoModeDuration;

public:
	const FDialogInfo* FindDialogInfoData(const FName& name);
};
