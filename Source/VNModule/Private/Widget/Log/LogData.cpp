#include "Widget/Log/LogData.h"

void ULogData::NativeConstruct()
{
    Super::NativeConstruct();
    mLogCharacterNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LogCharacterName_Text")));
    mLogDialogText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LogDialog_Text")));
    mHourText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Hour_Text")));
    mMinuteText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Minute_Text")));
    mSecondText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Second_Text")));
    mSpriteImage = Cast<UImage>(GetWidgetFromName(TEXT("Log_Image")));
}
