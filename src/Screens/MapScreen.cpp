#include "Screens/MapScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

const char* QUICKSAVE_LOCATION = "saves/quicksave.sav";

MapScreen::MapScreen(MapID mapID, CharacterCore* core) : Screen(core)
{
	m_mapID = mapID;
}

Screen* MapScreen::update(const sf::Time& frameTime)
{
	if (g_inputController->isKeyJustPressed(Key::Escape))
	{
		// store pos & go back to menu screen
		m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
		return new MenuScreen(m_characterCore);
	}
	if (g_inputController->isKeyJustPressed(Key::Quickload))
	{
		// store pos & go back to menu screen
		CharacterCore* newCharacterCore = new CharacterCore();
		if (!newCharacterCore->load(QUICKSAVE_LOCATION))
		{
			// no quicksave exists
			setTooltipText(g_textProvider->getText("NoQuicksaveExists"), sf::Color::Cyan, true);
			delete newCharacterCore;
		}
		else
		{
			delete m_characterCore;
			m_characterCore = newCharacterCore;
			return new LoadingScreen(m_characterCore->getData().currentMap, m_characterCore);
		}
	}
	if (g_inputController->isKeyJustPressed(Key::Quicksave))
	{
		m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
		m_characterCore->save(QUICKSAVE_LOCATION);
		setTooltipText(g_textProvider->getText("GameSaved"), sf::Color::Cyan, true);
	}
	MapExitBean* bean = m_currentMap.checkLevelEntry((*m_mainChar->getBoundingBox()));
	if (bean == nullptr || m_isOnLevelEntry)
	{
		m_isOnLevelEntry = (bean != nullptr);
		updateObjects(GameObjectType::_MainCharacter, frameTime);
		updateObjects(GameObjectType::_NPC, frameTime);
		updateTooltipText(frameTime);
		return this;
	} 
	else
	{ 
		m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
		m_characterCore->setLevel(bean->levelSpawnPoint, bean->level);
		delete bean;
		return new LoadingScreen(m_characterCore->getData().currentLevel, getCharacterCore());
	}
}

void MapScreen::execOnEnter(const Screen *previousScreen)
{
	if (!(m_currentMap.load(m_mapID)))
	{
		string filename(g_resourceManager->getFilename(m_mapID));
		string errormsg = filename + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}
	m_mainChar = new MapMainCharacter(&m_currentMap);
	m_mainChar->setCharacterCore(getCharacterCore());
	addObject(GameObjectType::_MainCharacter, m_mainChar);
	m_currentMap.loadAfterMainChar(this);
}

void MapScreen::execOnExit(const Screen *nextScreen)
{
	m_currentMap.dispose();
}

void MapScreen::render(sf::RenderTarget &renderTarget)
{
	m_currentMap.drawBackground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	renderObjects(GameObjectType::_NPC, renderTarget);
	m_currentMap.drawForeground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	renderTooltipText(renderTarget);
}