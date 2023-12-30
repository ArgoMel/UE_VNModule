#include "HUD/DialogHUD.h"
#include "VisualNovelGameInstance.h"

void UDialogHUD::NativeConstruct()
{
	Super::NativeConstruct();
    mRowNumber = 1;
    mLetterIndex = 0;
    mDialogFinished = false;
    mCanSkipDialog = false;
    mDisableLMB = false;

    mCharacterNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CharacterName_Text")));
    mDialogText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Dialog_Text")));
    mBGImage = Cast<UImage>(GetWidgetFromName(TEXT("BG_Image")));
    mLeftSpriteImage = Cast<UImage>(GetWidgetFromName(TEXT("LeftSprite_Image")));
    mRightSpriteImage = Cast<UImage>(GetWidgetFromName(TEXT("RightSprite_Image")));
    mDialogBorder = Cast<UBorder>(GetWidgetFromName(TEXT("Dialog_Border")));
    //    m_List->OnItemClicked().AddUObject<UUW_List>(this, &UUW_List::ItemClick);

    RefreshData();
    SetLetterByLetter();
}

FDialogInfo UDialogHUD::GetDTInfo()
{
    FDialogInfo dialoginfo;
    UVisualNovelGameInstance* gameInst = GetWorld()->GetGameInstance<UVisualNovelGameInstance>();
    if (IsValid(gameInst))
    {
        dialoginfo=*gameInst->FindDialogInfoData(*FString::FromInt(mRowNumber));
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
}

void UDialogHUD::ContinueDialog()
{
    ClearDialog();
    SetNextDialogRowIndex(1);
    RefreshData();
    SetLetterByLetter();
    PlayVisualFX(GetDTInfo().VisualFX);
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

void UDialogHUD::NextDialog()
{
    if(mDisableLMB)
    {
        return;
    }
    if (mDialogFinished)
    {
        ContinueDialog();
    }
    else
    {
        SkipDialog();
    }
}