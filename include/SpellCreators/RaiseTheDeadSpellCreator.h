#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/RaiseTheDeadSpell.h"

// a class that creates raise the dead spells
class RaiseTheDeadSpellCreator final : public SpellCreator {
public:
	RaiseTheDeadSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);

	void execExecuteSpell(const sf::Vector2f& target) override;
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

private:
	void addStrengthModifier(int level) override;
	void addDurationModifier(int level) override;
	void addRangeModifier(int level) override;
};