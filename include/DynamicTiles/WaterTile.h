#pragma once

#include "global.h"
#include "DynamicTile.h"

class WaterTile : public DynamicTile
{
public:
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;

private:
};