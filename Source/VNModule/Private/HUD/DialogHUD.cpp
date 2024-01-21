#include "HUD/DialogHUD.h"
#include "HUD/ChoiceButton.h"
#include "HUD/ChoicesGridPanel.h"
#include "HUD/VisualNovelHUD.h"
#include "Widget/Log/LogMain.h"
#include "Widget/Log/LogData.h"
#include "Widget/Log/LogGridPanel.h"
#include "Widget/Setting/SettingUI.h"
#include "VisualNovelGameInstance.h"

UDialogHUD::UDialogHUD(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    mDialogState = EDialogState::None;
    mRowNumber = 1;
    mLetterIndex = 0;
    mDialogFinished = false;
    mCanSkipDialog = false;
    mDisableLMB = false;

    mIsChoiceTriggered = false;

    mLogIndex = 0;

    mIsAutoModeOn = false;

    static ConstructorHelpers::FClassFinder<UUserWidget>	WBP_LogMain(TEXT(
        "/Game/VisualNovel/Widgets/Log/WBP_LogMain.WBP_LogMain_C"));
    if (WBP_LogMain.Succeeded())
    {
        mLogMainClass = WBP_LogMain.Class;
    }
    static ConstructorHelpers::FClassFinder<UUserWidget>	WBP_LogData(TEXT(
        "/Game/VisualNovel/Widgets/Log/WBP_LogData.WBP_LogData_C"));
    if (WBP_LogData.Succeeded())
    {
        mLogDataClass = WBP_LogData.Class;
    }
    static ConstructorHelpers::FClassFinder<UUserWidget>	WBP_Setting(TEXT(
        "/Game/VisualNovel/Widgets/Setting/WBP_Setting.WBP_Setting_C"));
    if (WBP_Setting.Succeeded())
    {
        mSettingUIClass = WBP_Setting.Class;
    }
    static ConstructorHelpers::FClassFinder<UUserWidget>	WBP_ChoiceButton_C(TEXT(
        "/Game/VisualNovel/BluePrints/HUD/WBP_ChoiceButton.WBP_ChoiceButton_C"));
    if (WBP_ChoiceButton_C.Succeeded())
    {
        mChoiceButtonClass = WBP_ChoiceButton_C.Class;
    }
    static ConstructorHelpers::FObjectFinder<USoundBase>	SW_HighPitch(TEXT(
        "/Script/Engine.SoundWave'/Game/VisualNovel/Sounds/SFX/SW_HighPitch.SW_HighPitch'"));
    if (SW_HighPitch.Succeeded())
    {
        mShakeSound.Add(SW_HighPitch.Object);
    }
    static ConstructorHelpers::FObjectFinder<USoundBase>	SW_LowPitch(TEXT(
        "/Script/Engine.SoundWave'/Game/VisualNovel/Sounds/SFX/SW_LowPitch.SW_LowPitch'"));
    if (SW_LowPitch.Succeeded())
    {
        mShakeSound.Add(SW_LowPitch.Object);
    }
    static ConstructorHelpers::FObjectFinder<USoundBase>	SW_Choices(TEXT(
        "/Script/Engine.SoundWave'/Game/VisualNovel/Sounds/SFX/SW_Choices.SW_Choices'"));
    if (SW_Choices.Succeeded())
    {
        mChoiceSound = SW_Choices.Object;
    }
    static ConstructorHelpers::FObjectFinder<USoundBase>	SW_Skip(TEXT(
        "/Script/Engine.SoundWave'/Game/VisualNovel/Sounds/SFX/SW_Skip.SW_Skip'"));
    if (SW_Skip.Succeeded())
    {
        mSkipSound = SW_Skip.Object;
    }
    static ConstructorHelpers::FObjectFinder<USoundBase> SW_BG(TEXT(
        "/Script/Engine.SoundWave'/Game/VisualNovel/Sounds/SFX/SW_BG.SW_BG'"));
    if (SW_BG.Succeeded())
    {
        mBGSound = SW_BG.Object;
    }
    static ConstructorHelpers::FObjectFinder<USoundBase>	SC_Keys(TEXT(
        "/Script/Engine.SoundCue'/Game/VisualNovel/Sounds/SFX/SC_Keys.SC_Keys'"));
    if (SC_Keys.Succeeded())
    {
        mLetterSound = SC_Keys.Object;
    }
    static ConstructorHelpers::FObjectFinder<USoundBase>	SW_Music(TEXT(
        "/Script/Engine.SoundWave'/Game/VisualNovel/Sounds/Music/SW_Music.SW_Music'"));
    if (SW_Music.Succeeded())
    {
        mMusicSound = SW_Music.Object;
    }
}

void UDialogHUD::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    mGameInstance = GetWorld()->GetGameInstance<UVisualNovelGameInstance>();
}

void UDialogHUD::NativeConstruct()
{
	Super::NativeConstruct();
    mCharacterNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CharacterName_Text")));
    mDialogText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Dialog_Text")));
    mContinueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Continue_Text")));
    mTimeRemainingText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TimeRemaining_Text")));
    mAutoSkipDurationText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AutoSkipDuration_Text")));
    mBGImage = Cast<UImage>(GetWidgetFromName(TEXT("BG_Image")));
    mLeftSpriteImage = Cast<UImage>(GetWidgetFromName(TEXT("LeftSprite_Image")));
    mRightSpriteImage = Cast<UImage>(GetWidgetFromName(TEXT("RightSprite_Image")));
    mDialogBorder = Cast<UBorder>(GetWidgetFromName(TEXT("Dialog_Border")));
    mChoiceGridPanel = Cast<UChoicesGridPanel>(GetWidgetFromName(TEXT("WBP_ChoicesGridPanel")));
    mTypingThrobber = Cast<UThrobber>(GetWidgetFromName(TEXT("Typing_Throbber")));
    mLogButton = Cast<UButton>(GetWidgetFromName(TEXT("Log_Button")));
    mAutoButton = Cast<UButton>(GetWidgetFromName(TEXT("Auto_Button")));
    mSettingButton = Cast<UButton>(GetWidgetFromName(TEXT("Setting_Button")));

    mLogButton->OnClicked.AddDynamic(this, &UDialogHUD::LogButtonClicked);
    mAutoButton->OnClicked.AddDynamic(this, &UDialogHUD::AutoButtonClicked);
    mAutoButton->OnHovered.AddDynamic(this, &UDialogHUD::AutoButtonHovered);
    mAutoButton->OnUnhovered.AddDynamic(this, &UDialogHUD::AutoButtonUnHovered);
    mSettingButton->OnClicked.AddDynamic(this, &UDialogHUD::SettingButtonClicked);

    RefreshData();
    SetLetterByLetter();
    ToggleDialogState(EDialogState::Typing);
    PlayAnimation(mFadeContinueTextAnim,0.f,0,EUMGSequencePlayMode::Forward,0.5f);
    UGameplayStatics::PlaySound2D(GetWorld(), mBGSound);
    UGameplayStatics::PlaySound2D(GetWorld(), mMusicSound);

    if (IsValid(mLogMainClass))
    {
        mLogMainWidget = CreateWidget<ULogMain>(GetWorld(), mLogMainClass);
        if (IsValid(mLogMainWidget))
        {
            mLogMainWidget->AddToViewport(1);
            mLogMainWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    if (IsValid(mSettingUIClass))
    {
        mSettingUIWidget = CreateWidget<USettingUI>(GetWorld(), mSettingUIClass);
        if (IsValid(mSettingUIWidget))
        {
            mSettingUIWidget->AddToViewport(1);
            mSettingUIWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    APlayerController* controller = GetOwningPlayer();
    if (!IsValid(controller))
    {
        UE_LOG(LogTemp, Warning, TEXT("No GetOwningPlayer"));
        return;
    }
    FInputModeGameAndUI inputMode;
    inputMode.SetWidgetToFocus(GetCachedWidget());
    controller->SetInputMode(inputMode);
}

void UDialogHUD::LogButtonClicked()
{
    mLogMainWidget->SetVisibility(ESlateVisibility::Visible);
    APlayerController* controller = GetOwningPlayer();
    if (!IsValid(controller))
    {
        return;
    }
    FInputModeUIOnly inputMode;
    inputMode.SetWidgetToFocus(mLogMainWidget->GetCachedWidget());
    controller->SetInputMode(inputMode);
    UGameplayStatics::SetGamePaused(GetWorld(), true);
    AVisualNovelHUD* hud = Cast<AVisualNovelHUD>(controller->GetHUD());
    if(!IsValid(hud))
    {
        return;
    }
    UGameplayStatics::PlaySound2D(GetWorld(), hud->GetLogButtonSound());
}

void UDialogHUD::AutoButtonClicked()
{
    APlayerController* controller = GetOwningPlayer();
    if (!IsValid(controller))
    {
        return;
    }
    AVisualNovelHUD* hud = Cast<AVisualNovelHUD>(controller->GetHUD());
    if (!IsValid(hud))
    {
        return;
    }
    UGameplayStatics::PlaySound2D(GetWorld(), hud->GetLogButtonSound());
    if(mIsAutoModeOn)
    {
        mAutoButton->SetRenderOpacity(0.7f);
        GetWorld()->GetTimerManager().PauseTimer(mAutoModeTimer);
        ClearAndResetAutoCountdown();
    }
    else
    {
        mAutoButton->SetRenderOpacity(1.f);
    }
    mIsAutoModeOn = !mIsAutoModeOn;
    if(mIsAutoModeOn&&mDialogFinished)
    {
        if(!mIsChoiceTriggered)
        {
            AutoModeStart();
        }
    }
}

void UDialogHUD::AutoButtonHovered()
{
    if (mIsAutoModeOn)
    {
        mAutoButton->SetRenderOpacity(0.9f);
    }
    else
    {
        mAutoButton->SetRenderOpacity(0.8f);
    }
}

void UDialogHUD::AutoButtonUnHovered()
{
    if (mIsAutoModeOn)
    {
        mAutoButton->SetRenderOpacity(0.7f);
    }
    else
    {
        mAutoButton->SetRenderOpacity(1.f);
    }
}

void UDialogHUD::SettingButtonClicked()
{
    mSettingUIWidget->SetVisibility(ESlateVisibility::Visible);
    APlayerController* controller = GetOwningPlayer();
    if (!IsValid(controller))
    {
        return;
    }
    FInputModeUIOnly inputMode;
    inputMode.SetWidgetToFocus(mSettingUIWidget->GetCachedWidget());
    controller->SetInputMode(inputMode);
    UGameplayStatics::SetGamePaused(GetWorld(), true);
    AVisualNovelHUD* hud = Cast<AVisualNovelHUD>(controller->GetHUD());
    if (!IsValid(hud))
    {
        return;
    }
    UGameplayStatics::PlaySound2D(GetWorld(), hud->GetLogButtonSound());
}

FDialogInfo UDialogHUD::GetDTInfo()
{
    FDialogInfo dialoginfo;  
    if (IsValid(mGameInstance))
    {
        dialoginfo=*mGameInstance->FindDialogInfoData(*FString::FromInt(mRowNumber));
    }
    return dialoginfo;
}

void UDialogHUD::SetHUDElements(FDialogInfo dialogInfo)
{
    mCharacterNameText->SetText(FText::FromString(EnumToFString<ECharacterName>(dialogInfo.CharacterName)));
    mBGImage->SetBrushFromTexture(dialogInfo.BGImage);
    mLeftSpriteImage->SetBrushFromTexture(dialogInfo.LeftSpriteImage);
    mRightSpriteImage->SetBrushFromTexture(dialogInfo.RightSpriteImage);
    switch (dialogInfo.CharacterSetting)
    {
    case ECharacterSetting::LeftSpriteSpeaking:
        mLeftSpriteImage->SetRenderOpacity(1.f);
        mRightSpriteImage->SetRenderOpacity(1.f);
        SetCharacterSettings(true);
        break;
    case ECharacterSetting::RightSpriteSpeaking:
        mLeftSpriteImage->SetRenderOpacity(1.f);
        mRightSpriteImage->SetRenderOpacity(1.f);
        SetCharacterSettings(false);
        break;
    case ECharacterSetting::LeftSpriteHidden:
        mLeftSpriteImage->SetRenderOpacity(0.f);
        mRightSpriteImage->SetRenderOpacity(1.f);
        SetCharacterSettings(false);
        break;
    case ECharacterSetting::RightSpriteHidden:
        mLeftSpriteImage->SetRenderOpacity(1.f);
        mRightSpriteImage->SetRenderOpacity(0.f);
        SetCharacterSettings(true);
        break;
    case ECharacterSetting::AllSpriteHidden:
        mLeftSpriteImage->SetRenderOpacity(0.f);
        mRightSpriteImage->SetRenderOpacity(0.f);
        break;
    }
}

void UDialogHUD::SetLetterByLetter()
{
    GetWorld()->GetTimerManager().SetTimer(mLetterTimer,this,&UDialogHUD::DialogLogic,0.05f,true,0.f);
}

void UDialogHUD::DialogLogic()
{
    FDialogInfo dialogInfo = GetDTInfo();
    if(dialogInfo.DialogText[(int32)mGameInstance->Language].Len() >= mLetterIndex)
    {
        mCurDialogText.AppendChar(
            dialogInfo.DialogText[(int32)mGameInstance->Language].GetCharArray()[mLetterIndex]);
        mDialogText->SetText(FText::FromString(mCurDialogText));
        ++mLetterIndex;
        mDialogFinished = false;
        mCanSkipDialog = true;
        ToggleDialogState(EDialogState::Typing);
        UGameplayStatics::PlaySound2D(GetWorld(), mLetterSound);
    }
    else
    {
        SkipDialog();
    }
}

void UDialogHUD::ClearDialog()
{
    mLetterIndex = 0;
    mCurDialogText.Empty();
    mDialogText->SetText(FText::FromString(mCurDialogText));
}

void UDialogHUD::SetNextDialogRowIndex(int32 index)
{
    mRowNumber+=index;
}

void UDialogHUD::RefreshData()
{
    SetHUDElements(GetDTInfo());
}

void UDialogHUD::SkipDialog()
{
    GetWorld()->GetTimerManager().ClearTimer(mLetterTimer);
    mDialogFinished = true;
    mCanSkipDialog = false;
    mCurDialogText = GetDTInfo().DialogText[(int32)mGameInstance->Language];
    mDialogText->SetText(FText::FromString(mCurDialogText));
    ToggleDialogState(EDialogState::FinishedTyping);
    UGameplayStatics::PlaySound2D(GetWorld(), mSkipSound);
    if (mIsAutoModeOn)
    {
        AutoModeStart();
    }
}

void UDialogHUD::ContinueDialog(bool hasChoice, int32 selectedIndex)
{
    ClearDialog();
    if(!hasChoice)
    {
        SetNextDialogRowIndex(1);
        RefreshData();
        SetLetterByLetter();
        ClearAndResetAutoCountdown();
    }
    else
    {
        SelectChoiceRowIndex(selectedIndex);
        RefreshData();
        SetLetterByLetter();
    }
    PlayVisualFX(GetDTInfo().VisualFX);
}

void UDialogHUD::ToggleBorders(bool bordersOn)
{
    if(bordersOn)
    {
        PlayAnimation(mFadeBordersAnim,0.f,1,EUMGSequencePlayMode::Forward,2.f);
    }
    else
    {
        mDialogBorder->SetRenderOpacity(0.f);
    }
}

void UDialogHUD::BordersOn()
{
    ClearDialog();
    ToggleBorders(true);
    mDisableLMB = false;
}

void UDialogHUD::ToggleDialogState(EDialogState state)
{
    if(mDialogState==state)
    {
        return;
    }
    FSlateBrush brush = mTypingThrobber->GetImage();
    FName name;
    switch (state)
    {
    case EDialogState::Typing:
        mTypingThrobber->SetAnimateHorizontally(true);
        mTypingThrobber->SetAnimateOpacity(true);
        mTypingThrobber->SetAnimateVertically(true);
        mTypingThrobber->SetNumberOfPieces(3);
        brush.TintColor = FLinearColor::Yellow;
        mContinueText->SetVisibility(ESlateVisibility::Collapsed);
        break;
    case EDialogState::FinishedTyping:
        mTypingThrobber->SetAnimateHorizontally(false);
        mTypingThrobber->SetAnimateOpacity(false);
        mTypingThrobber->SetAnimateVertically(false);
        mTypingThrobber->SetNumberOfPieces(1);
        brush.TintColor = FLinearColor::Blue;
        mContinueText->SetVisibility(ESlateVisibility::Visible);
        name = FName(TEXT("클릭!"));
        break;
    case EDialogState::Choice:
        mTypingThrobber->SetAnimateHorizontally(false);
        mTypingThrobber->SetAnimateOpacity(false);
        mTypingThrobber->SetAnimateVertically(false);
        mTypingThrobber->SetNumberOfPieces(1);
        brush.TintColor = FLinearColor::Red;
        mContinueText->SetVisibility(ESlateVisibility::Visible);
        name = FName(TEXT("선택중..."));
        break;
    }
    mContinueText->SetText(FText::FromName(name));
    mTypingThrobber->SetImage(brush);
    mDialogState = state;
}

void UDialogHUD::SetCharacterSettings(bool bIsLeftSpriteHighlighted)
{
    if(bIsLeftSpriteHighlighted)
    {
        mLeftSpriteImage->SetBrushTintColor(FLinearColor::White);
        mRightSpriteImage->SetBrushTintColor(FLinearColor(0.4f,0.4f,0.4f));
        mCharacterNameText->SetColorAndOpacity(LeftSpriteColor);
    }
    else
    {
        mLeftSpriteImage->SetBrushTintColor(FLinearColor(0.4f, 0.4f, 0.4f));
        mRightSpriteImage->SetBrushTintColor(FLinearColor::White);
        mCharacterNameText->SetColorAndOpacity(RightSpriteColor);
    }
}

void UDialogHUD::CreateChoices_Implementation()
{
    mIsChoiceTriggered = true;
    ToggleDialogState(EDialogState::Choice);
    ClearAndResetAutoCountdown();
    TArray<FChoiceInfo> choiceInfo= GetDTInfo().ChoiceInfo;
    int32 size = choiceInfo.Num();
    for (int32 i = 0; i < size;++i)
    {
        if (IsValid(mChoiceButtonClass))
        {
            mChoiceButtonWidget = CreateWidget<UChoiceButton>(GetWorld(), mChoiceButtonClass);
            if (IsValid(mChoiceButtonWidget))
            {
                mChoiceButtonWidget->SetButtonIndex(i);
                mChoiceGridPanel->GetChoices()->AddChildToUniformGrid(mChoiceButtonWidget, i);
                mChoiceButtonWidgets.Add(mChoiceButtonWidget);
                mChoiceButtonWidget->GetChoiceText()->SetText((
                    FText::FromString(choiceInfo[i].ChoicesText)));
                mChoiceButtonWidget->OnCallChoiceButton.AddDynamic(this, &UDialogHUD::ClickChoice);
            }
        }
    }
}

void UDialogHUD::ClickChoice_Implementation(int32 index)
{
    mIsChoiceTriggered = false;
    UGameplayStatics::PlaySound2D(GetWorld(), mChoiceSound);
    ToggleDialogState(EDialogState::Typing);
    int32 choiceindex=GetDTInfo().ChoiceInfo[index].SelectedChoiceRowIndex;
    ContinueDialog(true, choiceindex);
}

void UDialogHUD::SelectChoiceRowIndex(int32 selectedIndex)
{
    mRowNumber = selectedIndex;
}

void UDialogHUD::GenerateLogData()
{
    if (IsValid(mLogDataClass))
    {
        mLogDataWidget = CreateWidget<ULogData>(GetWorld(), mLogDataClass);
        if (IsValid(mLogDataWidget))
        {
            mLogMainWidget->GetLogGridPanel()->GetLogUniformGridPanel()->AddChildToUniformGrid(
                mLogDataWidget, mLogIndex);
            ++mLogIndex;
            SetLogData();
        }
    }
}

void UDialogHUD::SetLogData()
{
    FDialogInfo dialogInfo = GetDTInfo();
    switch (dialogInfo.CharacterSetting)
    {
    case ECharacterSetting::LeftSpriteSpeaking:
    case ECharacterSetting::RightSpriteHidden:
        mLogDataWidget->GetSpriteImage()->SetBrushFromTexture(dialogInfo.LeftSpriteImage);
        mLogDataWidget->GetLogCharacterNameText()->SetColorAndOpacity(LeftSpriteColor);
        break;
    case ECharacterSetting::RightSpriteSpeaking:
    case ECharacterSetting::LeftSpriteHidden:
        mLogDataWidget->GetSpriteImage()->SetBrushFromTexture(dialogInfo.RightSpriteImage);
        mLogDataWidget->GetLogCharacterNameText()->SetColorAndOpacity(RightSpriteColor);
        break;
    case ECharacterSetting::AllSpriteHidden:
        mLogDataWidget->GetSpriteImage()->SetVisibility(ESlateVisibility::Hidden);
        break;
    }
    mLogDataWidget->GetLogCharacterNameText()->SetText(FText::FromString(
        EnumToFString<ECharacterName>(dialogInfo.CharacterName)));
    mLogDataWidget->GetLogDialogText()->SetText(FText::FromString(
        dialogInfo.DialogText[(int32)mGameInstance->Language]));
    mLogDataWidget->GetHourText()->SetText(FText::FromString(GetHour()));
    mLogDataWidget->GetMinuteText()->SetText(FText::FromString(GetMinute()));
    mLogDataWidget->GetSecondText()->SetText(FText::FromString(GetSecond()));
}

void UDialogHUD::AutoModeStart_Implementation()
{
    AutoModeCountdown();
    GetWorld()->GetTimerManager().SetTimer(mAutoModeTimer, this, &UDialogHUD::AutoModeCountdown, 1.f,true);
    mTimeRemainingText->SetVisibility(ESlateVisibility::Visible);
    mAutoSkipDurationText->SetVisibility(ESlateVisibility::Visible);
    ToggleTimeRemainingText();
}

void UDialogHUD::AutoModeCountdown_Implementation()
{
    int32 autoModeDuration = (int32)mGameInstance->AutoModeDuration-1;
    FString string;
    if(autoModeDuration ==0)
    {
        string = FString(TEXT("다음 대사로 넘어가는 중..."));
    }
    else
    {
        string = FString::Printf(TEXT("%02i"), autoModeDuration);
    }
    mAutoSkipDurationText->SetText(FText::FromString(string));
    mGameInstance->AutoModeDuration = autoModeDuration;
    ToggleTimeRemainingText();
    if(autoModeDuration <0)
    {
        ClearAndResetAutoCountdown();
        NextDialog();
    }
}

void UDialogHUD::ClearAndResetAutoCountdown()
{
    GetWorld()->GetTimerManager().ClearTimer(mAutoModeTimer);
    mGameInstance->AutoModeDuration = mGameInstance->ResetAutoModeDuration;
    mTimeRemainingText->SetVisibility(ESlateVisibility::Collapsed);
    mAutoSkipDurationText->SetVisibility(ESlateVisibility::Collapsed);
}

void UDialogHUD::ToggleTimeRemainingText()
{
    if(mGameInstance->AutoModeDuration<=0)
    {
        mTimeRemainingText->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        mTimeRemainingText->SetVisibility(ESlateVisibility::Visible);
    }
}

void UDialogHUD::PlayVisualFX(EVisualFX visualFX)
{
    switch (visualFX)
    {
    case EVisualFX::NoFX:
        break;
    case EVisualFX::CamShake:
        mDisableLMB = true;
        ToggleBorders(false);
        for (auto& shakeSound : mShakeSound)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), shakeSound);
        }
        PlayAnimation(mShakeAnim);
        FTimerHandle clearTimer;
        GetWorld()->GetTimerManager().SetTimer(clearTimer, this, &UDialogHUD::BordersOn, 0.4f, false);
        break;
    }
}

void UDialogHUD::NextDialog()
{
    if(mDisableLMB)
    {
        return;
    }
    if (mDialogFinished)
    {
        if(GetDTInfo().ChoiceInfo.IsEmpty())
        {
            GenerateLogData();
            ContinueDialog(false);
        }
        else if(!mIsChoiceTriggered)
        {
            CreateChoices();
            GenerateLogData();
        }
    }
    else
    {
        SkipDialog();
    }
}