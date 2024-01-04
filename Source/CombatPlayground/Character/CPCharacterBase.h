// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AlsCharacter.h"
#include "Logging/LogMacros.h"
#include "CPCharacterBase.generated.h"

class UWintPawnData;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACPCharacterBase : public AAlsCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_PawnData)
	TObjectPtr<const UWintPawnData> PawnData;

public:
	ACPCharacterBase();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_PawnData();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	};

	template<class T>
	const T* GetPawnData() const
	{
		return Cast<T>(PawnData);
	}

	void SetPawnData(const UWintPawnData* InPawnData);
};

