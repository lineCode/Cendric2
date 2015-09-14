#pragma once

#include "global.h"
#include "CharacterCore.h"
#include "InputController.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GUI/InventorySlot.h"
#include "GUI/InventorySlotClone.h"
#include "GUI/InventoryDescriptionWindow.h"
#include "GUI/TexturedButton.h"
#include "GUI/InventoryEquipment.h"

class LevelInterface;
class MapInterface;

// the inventory, as displayed in a level or a map
// it takes its information directly from the character core
// the inventory also shows item description and equipped items.
class Inventory
{
public:
	Inventory(LevelInterface* _interface);
	Inventory(MapInterface* _interface);
	~Inventory();

	void show();
	void hide();

	bool isVisible() const;

	void render(sf::RenderTarget& target);
	void update(const sf::Time& frameTime);

	// reloads the inventory text & items, depending on the core
	void reload();

private:
	CharacterCore* m_core;
	LevelInterface* m_levelInterface = nullptr;
	MapInterface* m_mapInterface = nullptr;
	bool m_isVisible = false;

	void init();

	void clearAllSlots();
	// reorganizes the positions of the 'slots' vector
	void calculateSlotPositions(std::vector<InventorySlot>& slots);

	Window* m_window;

	BitmapText m_goldText;
	BitmapText m_selectedTabText;

	std::vector<std::pair<TexturedButton, ItemType>> m_tabs;

	std::vector<InventorySlot> m_consumableItems;
	std::vector<InventorySlot> m_equipmentItems;
	std::vector<InventorySlot> m_miscItems;
	std::vector<InventorySlot> m_questItems;
	std::vector<InventorySlot> m_documentItems;

	ItemType m_currentTab;
	InventorySlot* m_selectedSlot = nullptr;
	void selectTab(ItemType type);
	void selectSlot(InventorySlot* selectedSlot, bool isEquipmentSlot);

	// used for drag & drop handling
	InventorySlotClone* m_currentClone = nullptr;
	bool m_hasDraggingStarted = false;
	bool m_isEquipmentSlotDragged = false;
	bool m_isDragging = false;
	// the mouse has to move this distance while pressed to spawn a clone.
	const float DRAG_DISTANCE = 10.f;
	sf::Vector2f m_startMousePosition;
	void handleDragAndDrop();
	void removeEquipmentItem();

	InventoryDescriptionWindow* m_descriptionWindow = nullptr;
	void showDescription(const InventorySlot& slot);
	void hideDescription();

	InventoryEquipment* m_equipment = nullptr;

	const int SLOT_COUNT_X = 5;
	const int SLOT_COUNT_Y = 5;
	const sf::Vector2f BUTTON_SIZE = sf::Vector2f(InventorySlot::SIDE_LENGTH + 10.f, 35.f);

	const float MARGIN = 7.f;
	const float INVENTORY_TOP = InventoryEquipment::TOP;
	const float INVENTORY_LEFT = InventoryEquipment::LEFT + MARGIN + InventoryEquipment::WIDTH;
	const float INVENTORY_HEIGHT = 
		3 * GUIConstants::TEXT_OFFSET +
		2 * GUIConstants::CHARACTER_SIZE_M +
		BUTTON_SIZE.y +
		(SLOT_COUNT_Y + 1) * MARGIN +
		SLOT_COUNT_Y * (MARGIN + InventorySlot::SIDE_LENGTH + 2 * InventorySlot::MARGIN);
	const float INVENTORY_WIDTH = 
		2 * GUIConstants::TEXT_OFFSET +
		(SLOT_COUNT_X - 1) * MARGIN +
		SLOT_COUNT_X * (InventorySlot::SIDE_LENGTH + 2 * InventorySlot::MARGIN);
	
	std::map<ItemType, std::vector<InventorySlot>*> m_typeMap;
};