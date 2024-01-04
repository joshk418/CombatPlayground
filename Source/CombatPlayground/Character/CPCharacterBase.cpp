// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "Actor/WintPawnData.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ACPCharacterBase::ACPCharacterBase()
{
	
}

void ACPCharacterBase::BeginPlay()
{  
	Super::BeginPlay();
}

void ACPCharacterBase::OnRep_PawnData()
{
}

void ACPCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPCharacterBase, PawnData);
}

void ACPCharacterBase::SetPawnData(const UWintPawnData* InPawnData)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to set PawnData [%s] on character [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	PawnData = InPawnData;

	ForceNetUpdate();
}

