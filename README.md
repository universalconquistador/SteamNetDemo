SteamNetDemo
============

This is an example of how to integrate SteamSockets into your replicated UE4 game, using dedicated servers.

The first commit in this repo is a super basic replicated 'game'. Players can join, move, shoot, and knock cubes around.

The second commit in this repo adds SteamSockets support, using the steps outlined below.


Integration Steps
-----------------

 - Enable the `SteamShared`, `OnlineSubsystemSteam`, and `SteamSockets` plugins for your project.

 - In your `DefaultEngine.ini` (or a platform-specific `Engine.ini` as desired), enable `OnlineSubsystemSteam` with your Steam AppID (you can use `480` as a testing AppID in development), and integrate the SteamSockets NetDriver, like so:
   ```
   [OnlineSubsystem]
   DefaultPlatformService=Steam
   
   [OnlineSubsystemSteam]
   bEnabled=True
   SteamDevAppId=480
   
   [/Script/Engine.GameEngine]
   !NetDriverDefinitions=ClearArray
   +NetDriverDefinitions=(DefName="GameNetDriver",DriverClassName="/Script/SteamSockets.SteamSocketsNetDriver",DriverClassNameFallback="/Script/OnlineSubsystemUtils.IpNetDriver")
   ```

   Note that, unlike some examples you might find online, we use `IpNetDriver` as the fallback NetDriver - this means that when SteamSockets are entirely disabled (see the `-NOSTEAM` option below), the normal IpNetDriver networking stack will be used instead, as opposed to disabling networking entirely.

 - Use a Game Session class that adds a call to `ISteamGameServer::LogOn()` or `ISteamGameServer::LogOnAnonymous()` in its `RegisterServer()`.   
   This involves creating a new C++ class extending `AGameSession` with the appropriate Steam calls, adding the necessary module dependencies to the `.Build.cs` file, and then adjusting the Game Mode blueprint to use the newly-created Game Session class.


Using the Code
--------------

To launch a dedicated server, run the `Launch Dedicated Server.bat` script. Inside is also an explanation of the command it uses.  
Once the dedicated server has finished loading, you should see a log message somewhere near the end that looks like
```
SteamGameSession: Listening on '<address>'
```
where `<address>`, when using the Steam Datagram Relay, is the SteamID that was assigned to it and the virtual port that the server is listening on.  
Once you have this SteamID, paste it into the `Launch Client.bat` file in place of `<steamid>`, and run the file. Assuming you are logged into Steam on your development machine, the client should connect to the server, as shown in the server's output, using the Steam Datagram Relay.

It is possible to use SteamSockets wihout the Steam Datagram Relay (although we are encouraged to just switch back to normal IP networking at that point). To disable the Steam Datagram Relay, add `-SteamSocketsRelays=false` to the command line when launching the client & server. With the relay disabled, you will need to connect to the server using the regular IP & port, rather than using `steam.<steamid>`. (Note that if the server shows it as listening on `[::]:17777`, that means any incoming address on port 17777 - use `localhost` or `127.0.0.1` as the IP.)

To disable Steam entirely, including SteamSockets, add `-NOSTEAM` to the command line when launching the client & server. This results in using ordinary IP networking, just like a fresh Unreal project. As above, use the IP address of the server (`localhost` or `127.0.0.1` when running it locally), along with the port, to connect.


Playing In-Editor
-----------------

To connect to a server from a client that you are launching from the editor, both the client and server must be using SteamSockets with the Steam Datagram Relay, both be using SteamSockets with normal IP addresses, or both be using the normal Unreal IpNetDriver.

In general, guidelines for launching the game are as follows:
 - To iterate on networked functionality: **Selected Viewport** or **New Window**, **Play As Client**, **Run Under One Process: YES**  
   This will use normal IpNetDriver networking, and by all being in one process you skip most of the load time.
 - To test Steam networking: **Standalone Game**, **Play Standalone**, **Run Under One Process: NO**, **Launch Separate Server: YES**  
   This will use SteamSockets networking, although you will need to initiate the connection to the server either through a Blueprint or by using the console command `open steam.<steamid>:<port>`. If you are iterating on frontend code for matchmaking / connecting to the server, you might want to launch the server from outside the Editor (e.g. using `Launch Dedicated Server.bat`) so that it does not stop when the game stops and launch each time the game launches, and so that it keeps a constant SteamID.

Here are the details on what network stack the client and server will use based on how they are launched:

**Client**: If you play the game from the editor using the 'Standalone Game' mode, the client will use SteamSockets with the Steam Datagram Relay - but in the other modes ('Selected Viewport', 'New Editor Window', etc) the game will use the standard IpNetDriver.

If you play the game using the `Launch Client.bat` file, the client will use SteamSockets with the Steam Datagram Relay, unless the `-SteamSocketsRelay=false` or `-NOSTEAM` command-line arguments are specified in the `.bat` file, as outlined above.

**Server**: If the 'Run Under One Process' option is enabled, or you are using the net mode 'Play as Listen Server', the server will use the standard IpNetDriver. Otherwise, the server will be launched in a new process and will use SteamSockets with the Steam Datagram Relay.

If you launch the server using the `Launch Dedicated Server.bat` file, the server will use SteamSockets with the Steam Datagram Relay, unless the `-SteamSocketsRelay=false` or `-NOSTEAM` command-line arguments are specified in the `.bat` file, as outlined above.
