#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FCPGameplayTags
{
private:
	static FCPGameplayTags GameplayTags;

public:
	static const FCPGameplayTags& Get()
	{
		return GameplayTags;
	}
	
	static void InitializeGameplayTags();

	FGameplayTag Attributes_Secondary_MaxHealth;

	FGameplayTag Input_Actions_Primary;
	FGameplayTag Input_Actions_Secondary;

	FGameplayTag Input_Movement_Jump;
};
