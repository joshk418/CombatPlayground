// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPCharacterBase.h"
#include "CPPlayerCharacter.generated.h"

class UCPInteractionComponent;
class UGameplayEffect;
struct FGameplayEffectSpec;

UCLASS()
class COMBATPLAYGROUND_API ACPPlayerCharacter : public ACPCharacterBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY()
	FTimerHandle CleanupCharacterTimerHandle;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Interaction")
	TObjectPtr<UCPInteractionComponent> InteractionComponent;

public:
	ACPPlayerCharacter();

private:
	void InitAbilityActorInfo();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void CleanupCharacter();

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void StartCleanupTimer(float Time);
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
