#pragma once

#include "global.h"
#include "Level/Enemy.h"
#include "Level/Level.h"
#include "SpellManager.h"
#include "Screen.h"

// A spooky Nekomata in a level
class NekomataEnemy : public Enemy {
public:
	NekomataEnemy(Level* level, Screen* screen);
	virtual ~NekomataEnemy() {}

	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

	void loadAnimation() override;

	MovingBehavior* createMovingBehavior() override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	sf::Vector2f getConfiguredSpellOffset() const override;
	int getMentalStrength() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;

protected:
	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;

	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::DiskPositionGenerator* m_posGenerator;

	void loadParticleSystem();

	void updateParticleSystemPosition();
};