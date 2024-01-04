// Fill out your copyright notice in the Description page of Project Settings.


#include "CPAssetManager.h"
#include "CombatPlayground/CPGameplayTags.h"

UCPAssetManager& UCPAssetManager::Get()
{
	if (UCPAssetManager* This = Cast<UCPAssetManager>(GEngine->AssetManager))
	{
		return *This;
	}
	
	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be CPAssetManager!"));
	return *NewObject<UCPAssetManager>(); // never calls this
}

void UCPAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCPGameplayTags::InitializeGameplayTags();

	// UAbilitySystemGlobals::Get().InitGlobalData();
}


