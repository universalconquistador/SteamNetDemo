// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamGameSession.h"
#include "Sockets.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemSteam.h"
#include "SteamSocketsSubsystem.h"
#include "steam/isteamgameserver.h"

void ASteamGameSession::RegisterServer()
{
	// Determine whether Steam is in use
	FOnlineSubsystemSteam* steam = (FOnlineSubsystemSteam*)IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	if (steam != nullptr)
	{
		// Register to handle the server logon event so we can report the SteamID assigned to us
		steam->AddOnSteamServerLoginCompletedDelegate_Handle(
			FOnSteamServerLoginCompletedDelegate::CreateUObject(this, &ASteamGameSession::OnServerLoginComplete));

		// Log on, allowing passing in a server ticket as a command-line argument
		FString serverToken;
		if (FParse::Value(FCommandLine::Get(), TEXT("SteamServerTicket"), serverToken))
		{
			UE_LOG(LogSockets, Log, TEXT("SteamGameSession: Registering server with ticket '%s'"), *serverToken);
			SteamGameServer()->LogOn(StringCast<char>(*serverToken).Get());
		}
		else
		{
			UE_LOG(LogSockets, Log, TEXT("SteamGameSession: Registering server anonymously"));
			SteamGameServer()->LogOnAnonymous();
		}
	}
	else
	{
		// Report the listen address right away
		this->ReportListenAddress();
	}

	// Base class
	AGameSession::RegisterServer();
}

void ASteamGameSession::ReportListenAddress()
{
	UNetDriver* netDriver = this->GetWorld()->GetNetDriver();
	if (netDriver != nullptr)
	{
		UE_LOG(LogSockets, Display, TEXT("SteamGameSession: Listening on %s"), *netDriver->LocalAddr->ToString(true));
	}
}

void ASteamGameSession::OnServerLoginComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		// The listen address isn't actually set until the SteamSocketsSubsystem handles this same delegate, so wait for next tick to read the address
		GetWorldTimerManager().SetTimerForNextTick(this, &ASteamGameSession::ReportListenAddress);
	}
	else
	{
		// Hmm, this is pretty bad.
		UE_LOG(LogSockets, Error, TEXT("SteamGameSession: Failed to log on dedicated server!"));
	}
}
