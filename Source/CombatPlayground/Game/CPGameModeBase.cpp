// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPGameModeBase.h"

#include "AbilitySystem/WintAbilitySystemComponent.h"
#include "CombatPlayground/Character/CPCharacterBase.h"
#include "CombatPlayground/Character/CPPlayerCharacter.h"
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

void ACPGameModeBase::RespawnPlayer(AController* Controller)
{
	if (ACPPlayerCharacter* PlayerCharacter = Controller->GetPawn<ACPPlayerCharacter>())
	{
		if (PlayerCharacter->GetLocomotionAction() == AlsLocomotionActionTags::Ragdolling)
		{
			PlayerCharacter->StopRagdolling();
		}

		if (UWintAbilitySystemComponent* Asc = Cast<UWintAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent()))
		{
			Asc->CancelAllActiveAbilities();
		}
			
		if (AActor* PlayerStart = FindPlayerStart(Controller))
		{
			RestartPlayerAtPlayerStart(Controller, PlayerStart);			
		}
	}
}
