// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPGameModeBase.generated.h"

class UWintPawnData;

UCLASS(minimalapi)
class ACPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWintPawnData> DefaultPawnData;

public:
	ACPGameModeBase();

	virtual void OnPostLogin(AController* NewPlayer) override;

	void RespawnPlayer(AController* Controller, TSubclassOf<ACharacter> CharacterClass);
};



