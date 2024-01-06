// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CPPlayerController.generated.h"

class ACPPlayerCharacter;
class UWintAbilitySystemComponent;
struct FGameplayTag;
class UWintInputConfig;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class COMBATPLAYGROUND_API ACPPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY()
	TObjectPtr<UWintAbilitySystemComponent> WintAbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWintInputConfig> InputConfig;

	UPROPERTY()
	FTimerHandle RespawnTimerHandle;

	UPROPERTY()
	FTimerHandle UnPossessTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player/Respawn")
	TSubclassOf<ACPPlayerCharacter> PlayerCharacterClass;

public:
	ACPPlayerController();

protected:
	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);

	UFUNCTION(Server, Reliable)
	void ServerAssignHealthChangedDelegate(APlayerController* PlayerController);

	void HealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);

	void OnUnPossessTimerExpired();
	
	void OnRespawnTimerExpired();

	UFUNCTION(Server, Reliable)
	void ServerRespawn(APlayerController* PlayerController);

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	UWintAbilitySystemComponent* GetWintAbilitySystemComponent();

	TSubclassOf<ACPPlayerCharacter> GetPlayerCharacterClass() const
	{
		return PlayerCharacterClass;
	}
};
