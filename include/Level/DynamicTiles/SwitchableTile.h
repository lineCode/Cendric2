#pragma once

#include "global.h"
#include "Level/DynamicTiles/LeverDependentTile.h"

class SwitchableTile final : public LeverDependentTile {
public:
	SwitchableTile(LevelScreen* levelScreen) : 
		LevelDynamicTile(levelScreen), 
		LeverDependentTile(levelScreen) {}
	bool init(const LevelTileProperties& properties) override;
	void setInitialState(bool on) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override { /*nop*/ };

	void switchTile() override;
	bool isSwitchable() const override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::SwitchableOn; }

private:
	std::string getSpritePath() const override;
};