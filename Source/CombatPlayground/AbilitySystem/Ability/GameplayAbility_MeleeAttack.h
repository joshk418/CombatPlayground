// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class COMBATPLAYGROUND_API UGameplayAbility_MeleeAttack : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Attack")
	float AttackDistanceFromOwner = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debug")
	float DebugSphereLifeTime = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debug")
	float CollisionSphereRadius = 80.0f;

public:
	// Performs a sphere sweep in front of the owning actor and applies damage to all hit actors.
	UFUNCTION(BlueprintCallable, Category = "Ability|Attack")
	void ApplyDamageToTarget(TSubclassOf<UGameplayEffect> DamageEffect, bool bDebug = false) const;
};
