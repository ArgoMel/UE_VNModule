#include "PlayerController/VisualNovelPlayerController.h"
#include "HUD/VisualNovelHUD.h"
#include "InputActionValue.h"
#include "InputModifiers.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AVisualNovelPlayerController::AVisualNovelPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext>	IMC_Default(TEXT(
		"/Game/VisualNovel/Input/IMC_Default.IMC_Default"));
	if (IMC_Default.Succeeded())
	{
		mIMC = IMC_Default.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>	IA_NextDialog(TEXT(
		"/Game/VisualNovel/Input/IA_NextDialog.IA_NextDialog"));
	if (IA_NextDialog.Succeeded())
	{
		mNextDialogKey = IA_NextDialog.Object;
	}
}

void AVisualNovelPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameAndUI inputMode;
	SetInputMode(inputMode);
	mVNHUD = Cast<AVisualNovelHUD>(GetHUD());
}

void AVisualNovelPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(InputComponent);
	if (IsValid(input))
	{
		input->BindAction(mNextDialogKey, ETriggerEvent::Started, this, &AVisualNovelPlayerController::NextDialog);
		SetNewController();
	}
}

void AVisualNovelPlayerController::NextDialog()
{
	if(IsValid(mVNHUD))
	{
		mVNHUD->NextDialog();
	}
}

void AVisualNovelPlayerController::SetNewController()
{
	ULocalPlayer* localPlayer = GetLocalPlayer();
	if (IsValid(localPlayer))
	{
		UEnhancedInputLocalPlayerSubsystem* subSystem =
			localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (IsValid(subSystem))
		{
			subSystem->ClearAllMappings();
			subSystem->AddMappingContext(mIMC, 0);
		}
	}
}
