REM Format: <editor> <project> <map> -server -log -port=<virtualport>
REM    <editor> The path to UE4Editor.exe (inside /Engine/Binaries/Win64)
REM    <project> The path to the .uproject to launch
REM    <map> The map to load
REM    <virtualport> The virtual port of the server to join (see https://partner.steamgames.com/doc/api/ISteamNetworkingSockets#CreateListenSocketP2P for guidance on virtual ports)

"E:\UE_4.27\Engine\Binaries\Win64\UE4Editor.exe" "%cd%\SteamNetDemo.uproject" /Game/FirstPersonBP/Maps/FirstPersonExampleMap.umap -server -log -port=17777