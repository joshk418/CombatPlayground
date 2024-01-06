// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATPLAYGROUND_API UCPInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionReach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> FocusedActor;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrawDebug;
	
public:
	UCPInteractionComponent();

protected:
	virtual void BeginPlay() override;

	void TraceForInteractable();
	
	UFUNCTION(Server, Reliable)
	void ServerPrimaryInteract(AActor* InFocusedActor);

	UFUNCTION(Server, Reliable)
	void ServerSecondaryInteract(AActor* InFocusedActor);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void PrimaryInteract();
	void SecondaryInteract();
};
