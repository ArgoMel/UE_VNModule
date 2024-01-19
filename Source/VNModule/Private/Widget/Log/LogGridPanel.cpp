#include "Widget/Log/LogGridPanel.h"

void ULogGridPanel::NativeConstruct()
{
	Super::NativeConstruct();

	mLogUniformGridPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("Log_GridPanel")));
}
