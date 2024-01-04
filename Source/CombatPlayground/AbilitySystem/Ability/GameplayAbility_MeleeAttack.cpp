// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_MeleeAttack.h"

#include "AbilityHelper.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UGameplayAbility_MeleeAttack::ApplyDamageToTarget(TSubclassOf<UGameplayEffect> DamageEffect, bool bDebug) const
{
	TArray<FHitResult> HitResults;
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	const FVector StartLocation = Avatar->GetActorLocation();
	const FVector EndLocation = StartLocation + Avatar->GetActorForwardVector() * AttackDistanceFromOwner;
	const FCollisionShape Shape = FCollisionShape::MakeSphere(CollisionSphereRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwningActorFromActorInfo());
	Params.AddIgnoredActor(Avatar);
	
	if (GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::Identity, ECC_Pawn,  Shape, Params))
	{
		for (const FHitResult& HitResult : HitResults)
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				if (bDebug)
				{
					LogOnScreenMessage(GetWorld(), -1, 2, FColor::Red, FString::Printf(TEXT("Hit %s"), *GetNameSafe(HitActor)));
					DrawDebugSphere(GetWorld(), EndLocation, CollisionSphereRadius, 12, FColor::Red, false, DebugSphereLifeTime);
				}
				
				UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
				if (!TargetAsc)
				{
					return;
				}

				UAbilitySystemComponent* OwnerAsc = GetAbilitySystemComponentFromActorInfo();
				FGameplayEffectContextHandle ContextHandle = OwnerAsc->MakeEffectContext();
				ContextHandle.AddSourceObject(GetOwningActorFromActorInfo());

				const FGameplayEffectSpecHandle SpecHandle = OwnerAsc->MakeOutgoingSpec(DamageEffect, 1, ContextHandle);
				OwnerAsc->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetAsc);
			}
		}
	}
}
