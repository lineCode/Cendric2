#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "BackgroundLayer.h"
#include "Structs/TMXData.h"
#include "Structs/LevelExitData.h"
#include "Structs/LevelDynamicTileData.h"
#include "Structs/LeverData.h"
#include "Structs/EnemyData.h"
#include "Structs/ChestData.h"

struct LevelData : TMXData {
	std::vector<BackgroundLayer> backgroundLayers;
	std::vector<std::pair<LevelDynamicTileID, std::vector<int>>> dynamicTileLayers;
	std::vector<LevelDynamicTileData> dynamicTiles;
	std::vector<AnimatedTileData> animatedTiles;
	std::vector<LeverData> levers;
	std::vector<std::string> levelItems;

	std::vector<EnemyData> enemies;
	std::vector<ChestData> chests;
	std::vector<LevelExitData> levelExits;
};