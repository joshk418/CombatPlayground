// Fill out your copyright notice in the Description page of Project Settings.


#include "CPInteractionComponent.h"

#include "CombatPlayground/CombatPlayground.h"
#include "CombatPlayground/Interface/InteractableInterface.h"

UCPInteractionComponent::UCPInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InteractionReach = 250;
	TraceRadius = 30;
	TraceHalfHeight = 80;

	SetIsReplicatedByDefault(true);
}

void UCPInteractionComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UCPInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (const APawn* OwningPawn = Cast<APawn>(GetOwner()); OwningPawn && OwningPawn->IsLocallyControlled())
	{
		TraceForInteractable();
	}
}

void UCPInteractionComponent::TraceForInteractable()
{
	TObjectPtr<AActor> OwningActor = GetOwner();

	FVector EyesLocation;
	FRotator EyesRotation;
	OwningActor->GetActorEyesViewPoint(EyesLocation, EyesRotation);

	FVector EndLocation = EyesLocation + EyesRotation.Vector() * InteractionReach;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	
	FCollisionShape Shape;
	Shape.SetCapsule(TraceRadius, TraceHalfHeight);

	FocusedActor = nullptr;

	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyesLocation, EndLocation, FQuat::Identity, ObjectQueryParams, Shape);
	for (FHitResult Hit : Hits)
	{
		if (Hit.GetActor() == GetOwner())
		{
			continue;
		}
		
		if (bDrawDebug)
		{
			DrawDebugCapsule(GetWorld(), Hit.Location, TraceHalfHeight, TraceRadius, FQuat::Identity, FColor::Green, false, 0);
		}

		if (TObjectPtr<AActor> HitActor = Hit.GetActor(); HitActor && HitActor->Implements<UInteractableInterface>())
		{
			if (FocusedActor == nullptr || FocusedActor != HitActor)
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		LogOnScreenMessage(this, -1, 0, FColor::Purple, FString::Printf(TEXT("Press 'E' to interact with: %s"), *FocusedActor->GetName()), true, true);
	}

	if (bDrawDebug)
	{
		FColor HitColor = bBlockingHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), EyesLocation, EndLocation, HitColor, false, 2, 0);
	}
}

void UCPInteractionComponent::PrimaryInteract()
{
	ServerPrimaryInteract(FocusedActor);
}

void UCPInteractionComponent::ServerPrimaryInteract_Implementation(AActor* InFocusedActor)
{
	if (FocusedActor)
	{
		APawn* OwningPawn = Cast<APawn>(GetOwner());
		IInteractableInterface::Execute_PrimaryInteract(InFocusedActor, OwningPawn);
	}
}

void UCPInteractionComponent::SecondaryInteract()
{
	ServerSecondaryInteract(FocusedActor);
}

void UCPInteractionComponent::ServerSecondaryInteract_Implementation(AActor* InFocusedActor)
{
}