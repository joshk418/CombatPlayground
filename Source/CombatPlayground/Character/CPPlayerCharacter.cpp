// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/WintPlayerState.h"
#include "UI/HUDs/WintHUD.h"
#include "AbilitySystem/WintAttributeSet.h"

ACPPlayerCharacter::ACPPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1800.f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetUsingAbsoluteRotation(true);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void ACPPlayerCharacter::InitAbilityActorInfo()
{
	AWintPlayerState* WintPlayerState = GetPlayerState<AWintPlayerState>();
	check(WintPlayerState);

	// needs to be called so that effects, abilities, and attributes are set up before setting up HUD
	WintPlayerState->InitPawnData();

	UAbilitySystemComponent* StateAbilitySystemComponent = WintPlayerState->GetAbilitySystemComponent();
	if (!StateAbilitySystemComponent)
	{
		return;
	}

	StateAbilitySystemComponent->InitAbilityActorInfo(WintPlayerState, this);

	AbilitySystemComponent = StateAbilitySystemComponent;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AWintHUD* HUD = Cast<AWintHUD>(PlayerController->GetHUD()))
		{
			const UAttributeSet* Set = AbilitySystemComponent->GetAttributeSet(UWintAttributeSet::StaticClass());
			HUD->InitOverlay(PlayerController, WintPlayerState, AbilitySystemComponent, Set);
		}
	}
}

void ACPPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACPPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController)
	{
		InitAbilityActorInfo();
	}
}

void ACPPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (Controller)
	{
		InitAbilityActorInfo();
	}
}

void ACPPlayerCharacter::StartCleanupTimer(float Time)
{
	GetWorldTimerManager().SetTimer(CleanupCharacterTimerHandle, this, &ACPPlayerCharacter::CleanupCharacter, Time, false);
}

void ACPPlayerCharacter::CleanupCharacter()
{
	Destroy();
}

