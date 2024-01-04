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
