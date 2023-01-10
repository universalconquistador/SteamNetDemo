REM Format: <editor> <project> steam.<steamid>:<virtualport><map> -game -log
REM    <editor> The path to UE4Editor.exe (inside /Engine/Binaries/Win64)
REM    <project> The path to the .uproject to launch
REM    <steamid> The SteamID of the server to join - dedicated servers are given a SteamID when they call ISteamGameServer::LogOn() or LogOnAnonymous() and the call finishes (happens asynchronously)
REM              Once the LogOn call finishes, you can get the SteamID of the server using FSteamSocketsSubsystem::GetIdentityAddress()
REM    <virtualport> The virtual port of the server to join (see https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CreateListenSocketP2P for guidance on virtual ports)
REM    <map> The map to load

REM MAKE SURE TO PASTE IN THE CURRENT DEDICATED SERVER STEAMID BEFORE RUNNING!!!

"E:\UE_4.27\Engine\Binaries\Win64\UE4Editor.exe" "%cd%\SteamNetDemo.uproject" steam.<steamid>:17777/Game/FirstPersonBP/Maps/FirstPersonExampleMap.umap -game -log