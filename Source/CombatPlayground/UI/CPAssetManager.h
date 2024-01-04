// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CPAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class COMBATPLAYGROUND_API UCPAssetManager : public UAssetManager
{
	GENERATED_BODY()

protected:
	virtual void StartInitialLoading() override;

public:
	static UCPAssetManager& Get();
};
