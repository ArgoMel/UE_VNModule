#include "HUD/ChoicesGridPanel.h"

void UChoicesGridPanel::NativeConstruct()
{
	Super::NativeConstruct();

	mChoices = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("Choice_UniformGridPanel")));
}
