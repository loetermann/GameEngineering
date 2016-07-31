# GameEngineering

# Shader

**IMPORTANT** Shaders will not work before they get installed correctly. For more information see `Unreal\SpaceGladiator\Shader\README.txt`.

# Multiplayer

Some detailed information for starting multiplayer can be found [here](https://docs.unrealengine.com/latest/INT/Gameplay/Networking/Server/index.html).

But theres still some  information missing. If the *UE4Editor.exe* is not present in your `%PATH%` Variable you should add it, or fully cd to the correct directory. 

Type | Command
-----|---------
Listen Server | UE4Editor.exe ProjectName Level1?Listen -game
Dedicated Server | UE4Editor.exe ProjectName Level1 -server -game -log
Client | UE4Editor.exe ProjectName ServerIP -game

However *ProjectName* needs to be replaced with the absolute path to the `.uproject` file.
For example if the project is located in 

```
D:\Unreal Projects\GameEngineering\

```

then execute

```
UE4Editor.exe "D:\Unreal Projects\GameEngineering\Unreal\SpaceGladiator\SpaceGladiator.uproject" Level1?Listen -game

```

to start the server.
*HINT*: Sometimes it is required to reopen the server, after the game has launched (don't actually now why, but it works :wink:) therefore press `Tab` to open the console and execute `open Level1?Listen` to open Level1 and listen for clients. (on a client execute `open <ip-adress>` to connect e.g. `open 10.0.0.3`). therefore press `Tab` to open the console and execute `open Level1?Listen` to open Level1 and listen for clients. (on a client execute `open <ip-adress>` to connect e.g. `open 10.0.0.3`).

# Controls

Holding `Tab` shows the score.

## Movement

Use Mouse to tilt and look around. Use `A` and `D` to turn left and right.

## Targeting

You'll shoot in your viewing direction. Hold down `Shift` to shoot in your movement direction.

## Combat
Use `Left Mouse` to shoot and `Right Mouse` to recall your projectiles.
`W` toggles placing the wall. `Spacebar` uses a collected item.



# Items

* Inverse Controlls - inverts Left- and Right-Turning for all other players.
* Invert Camera - rolls camera for all other players 180 degrees
* Magnet - recalls all projectiles on the map
* Absorb Projectiles - damage from projectiles will add to a shield that expands your HP.
* Unstoppable - allows the player to break through walls and crash into other players without taking damage
