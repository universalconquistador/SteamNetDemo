// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "SteamGameSession.generated.h"

/**
 * A GameSession that supports using SteamSockets as a dedicated server.
 */
UCLASS()
class STEAMNETDEMO_API ASteamGameSession : public AGameSession
{
	GENERATED_BODY()

	virtual void RegisterServer() override;

private:
	void ReportListenAddress();

	void OnServerLoginComplete(bool bWasSuccessful);
};
