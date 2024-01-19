#include "Widget/Log/LogMain.h"
#include "Widget/Log/LogGridPanel.h"

void ULogMain::NativeConstruct()
{
	Super::NativeConstruct();

	mLogGridPanel = Cast<ULogGridPanel>(GetWidgetFromName(TEXT("WBP_LogGridPanel")));
	mLogButton = Cast<UButton>(GetWidgetFromName(TEXT("WBP_LogGridPanel")));

	mLogButton->OnClicked.AddDynamic(this, &ULogMain::LogButtonClicked);
}

void ULogMain::LogButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
}
