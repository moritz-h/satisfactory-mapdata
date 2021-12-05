# SatisfactoryMapDataMod

Mod for the game Satisfactory by Coffee Stain Studios to extract map data.

## Building

This mod can be built by adding it as a plugin to the [SatisfactoryModLoader](https://github.com/satisfactorymodding/SatisfactoryModLoader).
More details are available in the [modding documentation](https://docs.ficsit.app/satisfactory-modding/latest/index.html).

## Usage

The mod stores map information, into JSON files.
This could help when analyzing savegames, as the savegames do not store details that are static to the game world, such as resource nodes type and purity.
Currently, the following data is extracted:
- Resource node type and purity
- Items and power required by DropPods

To use the mod, just install it and start a new game. Always when a world is loaded, the map data is exported to `%LOCALAPPDATA%\FactoryGame\MapData-Export`.
