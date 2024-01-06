// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPGameModeBase.h"
#include "CombatPlayground/Character/CPCharacterBase.h"
#include "Player/WintPlayerState.h"
#include "UObject/ConstructorHelpers.h"

ACPGameModeBase::ACPGameModeBase()
{
	
}

void ACPGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (const ACPCharacterBase* NewPlayerCharacter = NewPlayer->GetPawn<ACPCharacterBase>())
	{
		if (AWintPlayerState* NewPlayerState = NewPlayerCharacter->GetPlayerState<AWintPlayerState>())
		{
			NewPlayerState->SetPawnData(DefaultPawnData);
		}
	}
}

void ACPGameModeBase::RespawnPlayer(AController* Controller, TSubclassOf<ACharacter> CharacterClass)
{
	if (!CharacterClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ACPGameModeBase::RespawnPlayer: CharacterClass is nullptr"));
		return;
	}

	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("ACPGameModeBase::RespawnPlayer: Controller is nullptr"));
		return;
	}

	if (APawn* PlayerPawn = Controller->GetPawn())
	{
		PlayerPawn->Destroy();
	}

	const AActor* ControllerPlayerStart = ChoosePlayerStart(Controller);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;	
	
	AActor* NewPlayer = GetWorld()->SpawnActor(CharacterClass, &ControllerPlayerStart->GetActorTransform(), SpawnParameters);
	if (NewPlayer)
	{
		// NewPlayer->SetActorTransform(ControllerPlayerStart->GetActorTransform());
	
		if (APawn* PlayerPawn = Cast<APawn>(NewPlayer))
		{
			Controller->Possess(PlayerPawn);
		}
	}
	
	// if (ACPPlayerCharacter* PlayerCharacter = Controller->GetPawn<ACPPlayerCharacter>())
	// {
	// 	if (PlayerCharacter->GetLocomotionAction() == AlsLocomotionActionTags::Ragdolling)
	// 	{
	// 		PlayerCharacter->StopRagdolling();
	// 	}
	//
	// 	if (UWintAbilitySystemComponent* Asc = Cast<UWintAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent()))
	// 	{
	// 		Asc->CancelAllActiveAbilities();
	// 	}
	// 		
	// 	if (AActor* PlayerStart = FindPlayerStart(Controller))
	// 	{
	// 		RestartPlayerAtPlayerStart(Controller, PlayerStart);			
	// 	}
	// }
}
