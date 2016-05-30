#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "WorldScreen.h"
#include "ResourceManager.h"
#include "Level/LevelMainCharacterLoader.h"
#include "Level/LevelInterface.h"

#include "GUI/Button.h"
#include "GUI/YesOrNoForm.h"
#include "GUI/ProgressLog.h"

class LevelScreen : public WorldScreen {
public:
	LevelScreen(const std::string& levelID, CharacterCore* core);
	// called by the loading screen
	void load();

	void execUpdate(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& renderTarget) override;
	void execOnEnter(const Screen* previousScreen) override;
	void execOnExit(const Screen* nextScreen) override;

	void addSpellBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, Spell* spell, const AttributeData& attr);
	void addFoodBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const std::string& itemID, const AttributeData& attr);
	void addDotBuffToInterface(const sf::IntRect& textureLocation, const sf::Time& duration, const DamageOverTimeData& data);
	void removeTypedBuffs(SpellID id);

	void notifyQuickSlotAssignment(const std::string& itemId, int quickslotNr);

	// called by the loading screen. the dynamic tiles & light in level
	void loadForRenderTexture();

	bool exitWorld() override;
	void notifyBackFromMenu() override;
	void notifyBossKilled();

	LevelMainCharacter* getMainCharacter() const override;
	const Level* getWorld() const override;

private:
	void quicksave() override;

private:
	Level m_currentLevel;
	LevelMainCharacter* m_mainChar = nullptr;
	std::string m_levelID;

	bool m_isGameOver = false;
	bool m_isBossDefeated = false;
	bool m_isPaused = false;
	sf::Time m_bossDefeatedWaitTime = sf::seconds(5.0f);
	sf::Time m_respawnWaitTime = sf::seconds(2.5f);
	YesOrNoForm* m_yesOrNoForm = nullptr;
	Button* m_retryButton = nullptr;
	Button* m_backToMenuButton = nullptr;
	Button* m_resumeButton = nullptr;

	ScreenOverlay* m_gamePausedOverlay = nullptr;

	void handleGameOver(const sf::Time& frameTime);
	void handleBossDefeated(const sf::Time& frameTime);

	// pretty little agents to give to our yes or no form and buttons
	void onNo();
	void onYesToCheckpoint();
	void onBackToCheckpoint();
	void onBackToMenu();
	void onRetry();
	void onResume();

	void cleanUp();
};