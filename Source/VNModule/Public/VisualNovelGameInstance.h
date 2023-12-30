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
	const FDialogInfo* FindDialogInfoData(const FName& name);
};
