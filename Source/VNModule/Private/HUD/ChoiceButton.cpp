#include "HUD/ChoiceButton.h"
#include "HUD/DialogHUD.h"
#include "HUD/VisualNovelHUD.h"
#include "Interface/HUDInterface.h"


UChoiceButton::UChoiceButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mButtonIndex = 0;
}

void UChoiceButton::NativeConstruct()
{
	Super::NativeConstruct();
	mChoiceCanvas = Cast<UCanvas>(GetWidgetFromName(TEXT("Choice_Canvas")));
	mChoiceButton = Cast<UButton>(GetWidgetFromName(TEXT("Choice_Button")));
	mChoiceButton->OnPressed.AddDynamic(this, &UChoiceButton::ChoiceClicked);
	mChoiceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Choice_Text")));
}

void UChoiceButton::ChoiceClicked()
{
	OnCallChoiceButton.Broadcast(mButtonIndex);
	AHUD* hud = UGameplayStatics::GetPlayerController(GetWorld(),0)->GetHUD();
	if (hud->GetClass()->ImplementsInterface(UHUDInterface::StaticClass()))
	{
		auto interfaceVariable = Cast<IHUDInterface>(hud);
		//AVisualNovelHUD* vnHUD=interfaceVariable->GetVNHUD();
		AVisualNovelHUD* vnHUD=interfaceVariable->Execute_GetVNHUD(hud);
		UDialogHUD* dialogHUD = vnHUD->GetDialogWidget();
		//dialogHUD->SetMouseCursor(false);
		auto choiceButtons= dialogHUD->GetChoiceButtons();
		for (auto& choiceButton : choiceButtons) 
		{
			choiceButton->RemoveFromParent();
		}
	}
}