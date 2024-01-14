#include "HUD/DialogHUD.h"
#include "HUD/ChoiceButton.h"
#include "HUD/ChoicesGridPanel.h"
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

    mButtonIndex = 0;
    mIsChoiceTriggered = false;

    static ConstructorHelpers::FClassFinder<UUserWidget>	WBP_ChoiceButton_C(TEXT(
        "/Game/VisualNovel/BluePrints/HUD/WBP_ChoiceButton.WBP_ChoiceButton_C"));
    if (WBP_ChoiceButton_C.Succeeded())
    {
        mChoiceButtonClass = WBP_ChoiceButton_C.Class;
    }
    ///Script/Engine.SoundCue'/Game/VisualNovel/Sounds/SFX/SC_Keys.SC_Keys'
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
    mBGImage = Cast<UImage>(GetWidgetFromName(TEXT("BG_Image")));
    mLeftSpriteImage = Cast<UImage>(GetWidgetFromName(TEXT("LeftSprite_Image")));
    mRightSpriteImage = Cast<UImage>(GetWidgetFromName(TEXT("RightSprite_Image")));
    mDialogBorder = Cast<UBorder>(GetWidgetFromName(TEXT("Dialog_Border")));
    mChoiceGridPanel = Cast<UChoicesGridPanel>(GetWidgetFromName(TEXT("WBP_ChoicesGridPanel")));
    mTypingThrobber = Cast<UThrobber>(GetWidgetFromName(TEXT("Typing_Throbber")));

    SetMouseCursor(false);
    RefreshData();
    SetLetterByLetter();
    ToggleDialogState(EDialogState::Typing);
    PlayAnimation(mFadeContinueTextAnim,0.f,0,EUMGSequencePlayMode::Forward,0.5f);
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
    mDialogText->SetText(FText::FromString(dialogInfo.DialogText));
    mBGImage->SetBrushFromTexture(dialogInfo.BGImage);
    mLeftSpriteImage->SetBrushFromTexture(dialogInfo.LeftSpriteImage);
    mRightSpriteImage->SetBrushFromTexture(dialogInfo.RightSpriteImage);
    switch (dialogInfo.CharacterSetting)
    {
    case ECharacterSetting::LeftSpriteSpeaking:
        mLeftSpriteImage->SetRenderOpacity(1.f);
        mRightSpriteImage->SetRenderOpacity(0.8f);
        break;
    case ECharacterSetting::RightSpriteSpeaking:
        mLeftSpriteImage->SetRenderOpacity(0.8f);
        mRightSpriteImage->SetRenderOpacity(1.f);
        break;
    case ECharacterSetting::LeftSpriteNotSpeaking:
        mLeftSpriteImage->SetRenderOpacity(0.f);
        mRightSpriteImage->SetRenderOpacity(1.f);
        break;
    case ECharacterSetting::RightSpriteNotSpeaking:
        mLeftSpriteImage->SetRenderOpacity(1.f);
        mRightSpriteImage->SetRenderOpacity(0.f);
        break;
    case ECharacterSetting::AllSpriteNotSpeaking:
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
    if(dialogInfo.DialogText.Len()>=mLetterIndex)
    {
        mCurDialogText.AppendChar(dialogInfo.DialogText.GetCharArray()[mLetterIndex]);
        mDialogText->SetText(FText::FromString(mCurDialogText));
        ++mLetterIndex;
        mDialogFinished = false;
        mCanSkipDialog = true;
        ToggleDialogState(EDialogState::Typing);
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
    mCurDialogText = GetDTInfo().DialogText;
    mDialogText->SetText(FText::FromString(mCurDialogText));
    ToggleDialogState(EDialogState::FinishedTyping);
    //if(GetDTInfo().ChoiceInfo.IsEmpty())
    //{
    //    ToggleDialogState(EDialogState::FinishedTyping);
    //}
    //else if(!mIsChoiceTriggered)
    //{
    //    ToggleDialogState(EDialogState::FinishedTyping);
    //}
}

void UDialogHUD::ContinueDialog(bool hasChoice, int32 selectedIndex)
{
    ClearDialog();
    if(!hasChoice)
    {
        SetNextDialogRowIndex(1);
        RefreshData();
        SetLetterByLetter();
        PlayVisualFX(GetDTInfo().VisualFX);
    }
    else
    {
        SelectChoiceRowIndex(selectedIndex);
        RefreshData();
        SetLetterByLetter();
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
        PlayAnimation(mShakeAnim);
        FTimerHandle clearTimer;
        GetWorld()->GetTimerManager().SetTimer(clearTimer, this, &UDialogHUD::BordersOn, 0.4f, false);
        break;
    }
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
    FString text=TEXT("클릭!");
    FName name= FName(TEXT("클릭!"));
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
        break;
    case EDialogState::Choice:
        mTypingThrobber->SetAnimateHorizontally(false);
        mTypingThrobber->SetAnimateOpacity(false);
        mTypingThrobber->SetAnimateVertically(false);
        mTypingThrobber->SetNumberOfPieces(1);
        brush.TintColor = FLinearColor::Red;
        mContinueText->SetVisibility(ESlateVisibility::Visible);
        break;
    }
    mContinueText->SetText(FText::FromName(name));
    mTypingThrobber->SetImage(brush);
    mDialogState = state;
}

void UDialogHUD::CreateChoices_Implementation()
{
    mIsChoiceTriggered = true;
    ToggleDialogState(EDialogState::Choice);
    SetMouseCursor(true);
    TArray<FChoiceInfo> choiceInfo= GetDTInfo().ChoiceInfo;
    int32 size = choiceInfo.Num();
    for (int32 i = 0; i < size;++i)
    {
        mButtonIndex = i;
        if (IsValid(mChoiceButtonClass))
        {
            mChoiceButtonWidget = CreateWidget<UChoiceButton>(GetWorld(), mChoiceButtonClass);
            if (IsValid(mChoiceButtonWidget))
            {
                mChoiceButtonWidget->SetButtonIndex(mButtonIndex);
                mChoiceGridPanel->GetChoices()->AddChildToUniformGrid(mChoiceButtonWidget, mButtonIndex);
                mChoiceButtonWidgets.Add(mChoiceButtonWidget);
                mChoiceButtonWidget->GetChoiceText()->SetText((
                    FText::FromString(choiceInfo[mButtonIndex].ChoicesText)));
                mChoiceButtonWidget->OnCallChoiceButton.AddDynamic(this, &UDialogHUD::ClickChoice);
            }
        }
    }
}

void UDialogHUD::ClickChoice_Implementation(int32 index)
{
    mIsChoiceTriggered = false;
    if(IsValid(mChoiceSound))
    {
        PlaySound(mChoiceSound);
    }
    ToggleDialogState(EDialogState::Typing);
    int32 choiceindex=GetDTInfo().ChoiceInfo[index].SelectedChoiceRowIndex;
    ContinueDialog(true, choiceindex);
}

void UDialogHUD::SelectChoiceRowIndex(int32 selectedIndex)
{
    mRowNumber = selectedIndex;
}

void UDialogHUD::SetMouseCursor(bool showMouse)
{
    APlayerController* controller= UGameplayStatics::GetPlayerController(GetWorld(), 0);
    controller->bShowMouseCursor = showMouse;
    if(showMouse)
    {
        FInputModeUIOnly inputMode;
        controller->SetInputMode(inputMode);
    }
    else
    {
        ;
        FInputModeGameOnly inputMode;
        controller->SetInputMode(inputMode);
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
        ToggleDialogState(EDialogState::Typing);
        if(GetDTInfo().ChoiceInfo.IsEmpty())
        {
            ContinueDialog(false);
        }
        else
        {
            CreateChoices();
        }
    }
    else
    {
        SkipDialog();
    }
}