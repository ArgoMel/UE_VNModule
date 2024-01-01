#include "HUD/ChoiceButton.h"

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
}
