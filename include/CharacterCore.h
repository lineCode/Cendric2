#pragma once

#include <map>

#include "global.h"
#include "ResourceManager.h"
#include "FileIO/CharacterCoreReader.h"
#include "FileIO/CharacterCoreWriter.h"
#include "Item.h"
#include "Weapon.h"
#include "QuestLoader.h"

#include "Structs/AttributeBean.h"
#include "Structs/CharacterCoreData.h"

class CharacterCore
{
public:
	CharacterCore();
	CharacterCore(const CharacterCoreData& data);
	~CharacterCore();

	// returns the currently equipped item of type 'type'
	const Item* getEquippedItem(ItemType type);
	// getter for currently equipped weapon. convenience method for getItem of type weapon with a dynamic cast.
	const Weapon* getWeapon();
	// returns the item of id "id"
	const Item& getItem(const std::string& id);
	NPCState getNPCState(const std::string& id) const;
	QuestState getQuestState(const std::string& id) const;
	void setNPCState(const std::string& id, NPCState state);
	void setQuestState(const std::string& id, QuestState state);
	void setQuickslot(const std::string& item, int nr);
	// loads a new game with a new core and default attributes
	void loadNew();
	// tries to create a new file with this name. if successful, returns true. if it already exists, returns false
	bool createFile(const std::string& fileName) const;
	// uses the character core reader to load a .sav file
	bool load(const std::string& fileName);
	bool quickload();
	// uses the character core writer to save a .sav file. The name is the name chosen by the user.
	bool save(const std::string& fileName, const std::string& name);
	bool quicksave();
	// loads equipped itemvector (reload if not empty)
	void loadEquipmentItems();
	// loads itemvector (reload if not empty)
	void loadItems();
	// loads all quests that are started, completed or failed.
	void loadQuests();
	// reloads only the weapon slots.
	void reloadWeaponSlots();
	// base attributes plus the attributes gotten by equipment
	const AttributeBean& getTotalAttributes() const;
	// getter for items
	std::map<std::string, int>* getItems();
	// add gold to the data
	void addGold(int gold);
	// remove gold from the data
	void removeGold(int gold);
	// adds item(s) to the data
	void addItem(const std::string& item, int quantity);
	// removes item(s) to the data
	void removeItem(const std::string& item, int quantity);
	// save the position and map id. also sets "isInLevel" = false
	void setMap(const sf::Vector2f& position, const std::string& map);
	// save the position and level id  also sets "isInLevel" = true
	void setLevel(const sf::Vector2f& position, const std::string& level);
	// equips an item at the given position and adds the former equipment part into the inventory. To deequip an item, use this method with an empty string as item id.
	void equipItem(const std::string& item, ItemType type);
	// tries to remove the spell from slot nr "slotNr"
	void removeSpell(int slotNr);
	// tries to add the spell with id "id" to slot nr "slotNr"
	void addSpell(SpellID id, int slotNr);
	// tries to remove a modifier of type "type" from weapon slot nr "slotNr"
	void removeModifier(SpellModifierType type, int slotNr);
	// tries to add a modifier of type "type" to weapon slot nr "slotNr"
	// it uses "force" for that purpose
	void addModifier(const SpellModifier& modifier, int slotNr);
	// getter for core part
	const CharacterCoreData& getData() const;
	// initializes three empty boolean maps (enemies killed, enemies looted and items looted) for a given level
	void initializeMaps(const std::string& level);
	// signal enemy kill
	void setEnemyKilled(const std::string& level, int pos);
	// signal enemy looted
	void setEnemyLooted(const std::string& level, int pos);
	// signal item looted
	void setItemLooted(const std::string& level, int pos);
	// signal chest looted
	void setChestLooted(const std::string& level, int pos);
	// getter for quest data. If there is none for that id, returns nullptr
	const QuestData* getQuestData(const std::string& questID) const;
	// is the quest with id "id" complete?
	bool isQuestComplete(const std::string& questID) const;
	// a target for a quest has been killed
	void setQuestTargetKilled(const std::pair<std::string, std::string>& questtarget);
	// a condition for a quest has been fulfilled
	void setQuestConditionFulfilled(const std::string& questID, const std::string& condition);

	static const char* QUICKSAVE_LOCATION;
	static const char* DEBUGSAVE_LOCATION;

private:
	// clears equipped itemvector
	void clearEquippedItems();
	// clears itemvector
	void clearItems();
	// reload attributes based on equipped items (called by load). all attributes coming from food are removed.
	void reloadAttributes();

	// base attributes plus the attributes of all currently equipped items
	AttributeBean m_totalAttributes;

	std::map<std::string, Item> m_items;
	std::map<ItemType, Item*> m_equippedItems;
	std::map<std::string, QuestData> m_quests;

	CharacterCoreData m_data;

	sf::Clock m_stopwatch; 
};