#include "EnchantCommand.h"

#include "../../../Utils/Utils.h"

EnchantCommand::EnchantCommand() : IMCCommand("enchant", "Enchants items", "<enchantment> [level] <mode: auto / manual : 1/0>") {
	enchantMap["protection"] = 0;
	enchantMap["prot"] = 0;
	enchantMap["p"] = 0;

	enchantMap["fire_protection"] = 1;
	enchantMap["fire_prot"] = 1;
	enchantMap["fireprot"] = 1;
	enchantMap["fprot"] = 1;
	enchantMap["fp"] = 1;

	enchantMap["feather_falling"] = 2;

	enchantMap["blast_protection"] = 3;
	enchantMap["blast_prot"] = 3;
	enchantMap["blastprot"] = 3;
	enchantMap["bprot"] = 3;
	enchantMap["bp"] = 3;

	enchantMap["projectile_protection"] = 4;
	enchantMap["projectile_prot"] = 4;
	enchantMap["proj_prot"] = 4;
	enchantMap["projprot"] = 4;
	enchantMap["pprot"] = 4;
	enchantMap["pp"] = 4;

	enchantMap["thorns"] = 5;

	enchantMap["respiration"] = 6;
	enchantMap["resp"] = 6;

	enchantMap["depth_strider"] = 7;
	enchantMap["depthstrider"] = 7;

	enchantMap["aqua_affinity"] = 8;
	enchantMap["aquaaffinity"] = 8;
	enchantMap["aquaaff"] = 8;

	enchantMap["frost_walker"] = 25;
	enchantMap["frostwalker"] = 25;

	enchantMap["sharpness"] = 9;
	enchantMap["sharp"] = 9;
	enchantMap["s"] = 9;

	enchantMap["smite"] = 10;

	enchantMap["bane_of_arthropods"] = 11;
	enchantMap["baneofartropods"] = 11;
	enchantMap["boa"] = 11;

	enchantMap["knockback"] = 12;
	enchantMap["kb"] = 12;

	enchantMap["fire_aspect"] = 13;
	enchantMap["fireaspect"] = 13;
	enchantMap["fireasp"] = 13;

	enchantMap["looting"] = 14;
	enchantMap["loot"] = 14;

	enchantMap["channeling"] = 32;

	enchantMap["impaling"] = 29;

	enchantMap["loyalty"] = 31;

	enchantMap["riptide"] = 30;

	enchantMap["silktouch"] = 16;
	enchantMap["silk_touch"] = 16;

	enchantMap["fortune"] = 18;

	enchantMap["unbreaking"] = 17;
	enchantMap["unbr"] = 17;
	enchantMap["u"] = 17;

	enchantMap["efficiency"] = 15;
	enchantMap["eff"] = 15;
	enchantMap["e"] = 15;

	enchantMap["mending"] = 26;
	enchantMap["m"] = 26;

	enchantMap["power"] = 19;

	enchantMap["punch"] = 20;

	enchantMap["flame"] = 21;

	enchantMap["infinity"] = 22;
	enchantMap["inf"] = 22;
	enchantMap["i"] = 22;

	enchantMap["multishot"] = 33;

	enchantMap["quick_charge"] = 35;
	enchantMap["quickcharge"] = 35;

	enchantMap["piercing"] = 34;

	enchantMap["luck_of_sea"] = 23;
	enchantMap["luck_of_the_sea"] = 23;
	enchantMap["luckofthesea"] = 23;

	enchantMap["lure"] = 24;
}

EnchantCommand::~EnchantCommand() {
}

bool EnchantCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() > 1);

	int enchantId = 0;
	int enchantLevel = 1;
	bool isAuto = true;

	if (args->at(1) != "all") {
		try {
			// convert string to back to lower case
			std::string data = args->at(1);
			std::transform(data.begin(), data.end(), data.begin(), ::tolower);

			auto convertedString = enchantMap.find(data);
			if (convertedString != enchantMap.end())
				enchantId = convertedString->second;
			else
				enchantId = assertInt(args->at(1));
		} catch (int) {
			clientMessageF("%sException while trying to get enchant string", RED);
			enchantId = assertInt(args->at(1));
		}
	}

	if (args->size() > 2) {
		if (args->at(2) == "max")
			enchantLevel = 32767;
		else if (assertInt(args->at(2)) > 32767) {
			clientMessageF("%sEnchant failed, try using a lower enchant-level", RED);
			return true;
		} else
			enchantLevel = assertInt(args->at(2));
	}
	if (args->size() > 3)
		isAuto = static_cast<bool>(assertInt(args->at(3)));

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();

	int selectedSlot = supplies->selectedHotbarSlot;
	C_ItemStack* item = inv->getItemStack(selectedSlot);

	C_InventoryAction* firstAction = nullptr;
	C_InventoryAction* secondAction = nullptr;

	if (isAuto) {
		{
			firstAction = new C_InventoryAction(supplies->selectedHotbarSlot, item, nullptr);
			if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.mineplex.com") == 0)
				secondAction = new C_InventoryAction(0, nullptr, item, 32766, 100);
			else
				secondAction = new C_InventoryAction(0, nullptr, item, 507, 99999);
			manager->addInventoryAction(*firstAction);
			manager->addInventoryAction(*secondAction);
			delete firstAction;
			delete secondAction;
		}
	}

	using getEnchantsFromUserData_t = void(__fastcall*)(C_ItemStack*, void*);
	using addEnchant_t = bool(__fastcall*)(void*, __int64);
	using saveEnchantsToUserData_t = void(__fastcall*)(C_ItemStack*, void*);

	static getEnchantsFromUserData_t getEnchantsFromUserData = reinterpret_cast<getEnchantsFromUserData_t>(FindSignature("48 8B C4 55 57 41 54 41 56 41 57 48 8D 68 ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 70 ?? 0F 29 70 C8 4C 8B E2 4C 8B F9 48 89 54 24 ?? 33 F6"));
	static addEnchant_t addEnchant = reinterpret_cast<addEnchant_t>(FindSignature("48 89 5C 24 ?? 48 89 54 24 ?? 57 48 83 EC ?? 45 0F"));

	static saveEnchantsToUserData_t saveEnchantsToUserData = 0x0;
	if (!saveEnchantsToUserData) {
		saveEnchantsToUserData = reinterpret_cast<saveEnchantsToUserData_t>(FindSignature("40 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? ?? ?? ?? ?? 48 89 9C 24 ?? ?? ?? ?? 48 8B FA 4C 8B C1 48 8B 41 ?? 48 85 C0 74 29 48 83 38 ?? 74 23 48 8D 42 ?? 48 83 C2 50"));
	}

	if (strcmp(args->at(1).c_str(), "all") == 0) {
		for (int i = 0; i < 38; i++) {
			void* EnchantData = malloc(0x60);
			if (EnchantData != nullptr)
				memset(EnchantData, 0x0, 0x60);

			getEnchantsFromUserData(item, EnchantData);

			__int64 enchantPair = ((__int64)enchantLevel << 32) | i;

			if (addEnchant(EnchantData, enchantPair)) {  // Upper 4 bytes = level, lower 4 bytes = enchant type
				saveEnchantsToUserData(item, EnchantData);
				__int64 proxy = reinterpret_cast<__int64>(g_Data.getLocalPlayer()->getSupplies());
				if (!*(uint8_t*)(proxy + 168))
					(*(void(__fastcall**)(unsigned long long, unsigned long long, C_ItemStack*))(**(unsigned long long**)(proxy + 176) + 72i64))(
						*(unsigned long long*)(proxy + 176),
						*(unsigned int*)(proxy + 16),
						item);  // Player::selectItem

				//g_Data.getLocalPlayer()->sendInventory();
			}
			free(EnchantData);
		}
		clientMessageF("%sEnchant successful!", GREEN);
	} else {
		void* EnchantData = malloc(0x60);
		if (EnchantData != nullptr)
			memset(EnchantData, 0x0, 0x60);

		getEnchantsFromUserData(item, EnchantData);

		__int64 enchantPair = ((__int64)enchantLevel << 32) | enchantId;

		if (addEnchant(EnchantData, enchantPair)) {  // Upper 4 bytes = level, lower 4 bytes = enchant type
			saveEnchantsToUserData(item, EnchantData);
			__int64 proxy = reinterpret_cast<__int64>(g_Data.getLocalPlayer()->getSupplies());
			if (!*(uint8_t*)(proxy + 168))
				(*(void(__fastcall**)(unsigned long long, unsigned long long, C_ItemStack*))(**(unsigned long long**)(proxy + 176) + 72i64))(
					*(unsigned long long*)(proxy + 176),
					*(unsigned int*)(proxy + 16),
					item);  // Player::selectItem

			//g_Data.getLocalPlayer()->sendInventory();
			clientMessageF("%sEnchant successful!", GREEN);
		} else
			clientMessageF("%sEnchant failed, does your item already have that enchantment?", RED);

		free(EnchantData);
	}

	if (isAuto) {
		if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.mineplex.com") == 0)
			firstAction = new C_InventoryAction(0, item, nullptr, 32766, 100);
		else
			firstAction = new C_InventoryAction(0, item, nullptr, 507, 99999);
		secondAction = new C_InventoryAction(supplies->selectedHotbarSlot, nullptr, item);
		manager->addInventoryAction(*firstAction);
		manager->addInventoryAction(*secondAction);
		delete firstAction;
		delete secondAction;
	}

	return true;
}