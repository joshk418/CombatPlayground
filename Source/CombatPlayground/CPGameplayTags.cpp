#include "CPGameplayTags.h"

#include "GameplayTagsManager.h"

FCPGameplayTags FCPGameplayTags::GameplayTags;

void FCPGameplayTags::InitializeGameplayTags()
{
	// Secondary Attributes
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of health obtainable")
	);

	// Actions Input
	GameplayTags.Input_Actions_Primary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Actions.Primary"),
		FString("Input for primary action")
	);

	GameplayTags.Input_Actions_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Actions.Secondary"),
		FString("Input for secondary action")
	);

	//  Movement Input
	GameplayTags.Input_Movement_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Movement.Jump"),
		FString("Input for jumping")
	);
}
