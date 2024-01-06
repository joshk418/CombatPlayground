// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AlsCharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/WintAbilitySystemComponent.h"
#include "AbilitySystem/WintAttributeSet.h"
#include "Actor/WintPawnData.h"
#include "CombatPlayground/Character/CPPlayerCharacter.h"
#include "CombatPlayground/Game/CPGameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "Input/WintInputComponent.h"
#include "Player/WintPlayerState.h"

ACPPlayerController::ACPPlayerController()
{
	
}

void ACPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	ServerAssignHealthChangedDelegate(this);
}

void ACPPlayerController::ServerAssignHealthChangedDelegate_Implementation(APlayerController* PlayerController)
{
	if (UWintAbilitySystemComponent* Asc = GetWintAbilitySystemComponent())
	{
		if (const UWintAttributeSet* Set = Cast<UWintAttributeSet>(Asc->GetAttributeSet(UWintAttributeSet::StaticClass())))
		{
			Asc->GetGameplayAttributeValueChangeDelegate(Set->GetHealthAttribute()).AddUObject(this, &ACPPlayerController::HealthChanged);
		}
	}
}

void ACPPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	
	if (!MoveVector.IsZero())
	{
		if (APawn* ControlledPawn = GetPawn())
		{	
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			ControlledPawn->AddMovementInput(ForwardDirection, MoveVector.Y);
			ControlledPawn->AddMovementInput(RightDirection, MoveVector.X);
		}
	}
}

void ACPPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (!LookAxisVector.IsZero())
	{
		AddYawInput(LookAxisVector.X);
		AddPitchInput(LookAxisVector.Y);
	}
}

void ACPPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, *InputTag.ToString());
}

void ACPPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (UWintAbilitySystemComponent* Asc = GetWintAbilitySystemComponent())
	{
		Asc->AbilityInputReleased(InputTag);
	}
}

void ACPPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	if (UWintAbilitySystemComponent* Asc = GetWintAbilitySystemComponent())
	{
		Asc->AbilityInputHeld(InputTag);
	}
}

void ACPPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UWintInputComponent* WintInputComponent = CastChecked<UWintInputComponent>(InputComponent);
	WintInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPPlayerController::Move);
	WintInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPPlayerController::Look);
	
	// if (const AWintPlayerState* WintPlayerState = GetPlayerState<AWintPlayerState>())
	// {
	// 	if (const UWintPawnData* PawnData = WintPlayerState->GetPawnData<UWintPawnData>(); PawnData && PawnData->InputConfig)
	// 	{
	WintInputComponent->BindAbilityActions(
		// todo: replace with external input config
		InputConfig,
		this,
		&ThisClass::AbilityInputPressed,
		&ThisClass::AbilityInputReleased,
		&ThisClass::AbilityInputHeld
	);
	// 	}
	// }
}

UWintAbilitySystemComponent* ACPPlayerController::GetWintAbilitySystemComponent()
{
	if (!WintAbilitySystemComponent)
	{
		UAbilitySystemComponent* Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
		WintAbilitySystemComponent = Cast<UWintAbilitySystemComponent>(Asc);
	}

	return WintAbilitySystemComponent;
}

void ACPPlayerController::HealthChanged(const FOnAttributeChangeData& OnAttributeChangeData) 
{
	if (OnAttributeChangeData.NewValue <= 0 && !UnPossessTimerHandle.IsValid())
	{
		if (ACPPlayerCharacter* PawnCharacter = Cast<ACPPlayerCharacter>(GetPawn()))
		{
			PawnCharacter->StartRagdolling();
		}

		GetWorldTimerManager().SetTimer(UnPossessTimerHandle, this, &ACPPlayerController::OnUnPossessTimerExpired, 1.5f, false);
	}
}

void ACPPlayerController::OnUnPossessTimerExpired()
{
	UnPossessTimerHandle.Invalidate();

	if (ACPPlayerCharacter* PawnCharacter = Cast<ACPPlayerCharacter>(GetPawn()))
	{
		PawnCharacter->StartCleanupTimer(5);
	}
	
	UnPossess();
	
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ACPPlayerController::OnRespawnTimerExpired, 3.5f, false);
}


void ACPPlayerController::OnRespawnTimerExpired()
{
	RespawnTimerHandle.Invalidate();
	
	ServerRespawn(this);
}

void ACPPlayerController::ServerRespawn_Implementation(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null"));
		return;
	}

	if (ACPGameModeBase* GameMode = Cast<ACPGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->RespawnPlayer(PlayerController, PlayerCharacterClass);
	}
}