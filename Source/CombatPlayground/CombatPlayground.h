// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreenMessage(UObject* WorldContext, int Key, float TimeToDisplay, FColor Color, FString Message, bool bShowContextName = false, bool bShowOuterName = false)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	FString FormattedName     = bShowContextName ? FString::Printf(TEXT("[Name: %s] "), *WorldContext->GetName()) : "";
	FString FormattedOuter    = bShowOuterName && WorldContext->GetOuter() != nullptr ? FString::Printf(TEXT("[Outer: %s] "), *WorldContext->GetOuter()->GetFName().ToString()) : "";
	FString NetModeDescriptor = (World->IsNetMode(NM_Client) ? "[Client] " : "[Server] ") + FormattedName + FormattedOuter;
	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, NetModeDescriptor + Message);
}