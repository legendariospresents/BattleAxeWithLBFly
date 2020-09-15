#include "ItemBurner.h"

int count = 0;

Ignite::Ignite() : IModule(0x0, Category::COMBAT, "Automatically burns players and items.") {
	registerBoolSetting("Item Destroyer", &this->items, this->items);
	registerBoolSetting("Auto Extinguish", &this->extinguish, this->extinguish);
	registerBoolSetting("MultiAura", &this->lava, this->lava);
}

Ignite::~Ignite() {
}

const char* Ignite::getModuleName() {
	return "IgniteAura";
}

void ignite(C_Entity* currentEntity, bool isRegularEntity) {
	static auto igniteMod = moduleMgr->getModule<Ignite>();

	if (count >= 2)
		return; // because anticheat

	if (currentEntity == nullptr)
		return;
	
	if (currentEntity->isInWaterOrRain())
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 64 && !igniteMod->items)  // item
		return;
	if (!Target::isValidTarget(currentEntity) && !(currentEntity->getEntityTypeId() == 64))
		return;

	if (currentEntity->isOnFire())
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 7.f) {
		vec3_t pos(*currentEntity->getPos());
		igniteMod->tryScaffold(pos);
		count++;
	}
}

void Ignite::tryScaffold(vec3_t blockBelow) {
	blockBelow = blockBelow.floor();  // the block below us

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = *(block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));  // 0 (Below)
			checklist.push_back(new vec3_ti(0, 1, 0));   // 1 (Above)

			checklist.push_back(new vec3_ti(0, 0, -1));  // 2 (-z)
			checklist.push_back(new vec3_ti(0, 0, 1));   // 3 (+z)

			checklist.push_back(new vec3_ti(-1, 0, 0));  // 4 (-x)
			checklist.push_back(new vec3_ti(1, 0, 0));   // 5 (+x) (Just for future reference)
		}

		// I should probably add a diagonal checklist too.

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			if (!(*(g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click

				// pretend this code makes a rotation

				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			auto localPlayer = g_Data.getLocalPlayer();
			auto selectedItemId = localPlayer->getSelectedItemId();
			if (selectedItemId != 259) {
				switched = false;
				prevSlot = supplies->selectedHotbarSlot;
				findWeapon();
			}
			g_Data.getCGameMode()->buildBlock(&blok, i);
		}
	}
}

void Ignite::onTick(C_GameMode* gm) {
	count = 0;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	auto localPlayer = g_Data.getLocalPlayer();
	auto selectedItemId = localPlayer->getSelectedItemId();
	g_Data.forEachEntity(ignite);
	if (count == 0 && prevSlot != 10 && selectedItemId == 259 && !switched) {
		supplies->selectedHotbarSlot = prevSlot;		
	}
	if (supplies->selectedHotbarSlot == prevSlot && count == 0)
		switched = true;
}

void Ignite::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 259) {
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}