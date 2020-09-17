#include "AntiHunger.h"

AntiHunger::AntiHunger() : IModule(0, Category::MISC, "Never lose hunger again!") {
	registerBoolSetting("Anti Sprint", &this->sprint, this->sprint);
}

AntiHunger::~AntiHunger() {
}

const char* AntiHunger::getModuleName() {
	return "AntiHunger";
}

void AntiHunger::onEnable() {
	if (sprint)
		g_Data.getLocalPlayer()->setSprinting(false);
}

void AntiHunger::onSendPacket(C_Packet* packet) {
	if (!packet->isInstanceOf<C_PlayerActionPacket>())
		return;
	auto p = reinterpret_cast<C_PlayerActionPacket*>(packet);
	if (p->action == 8 && !sprint)
		p->action = 16;
	if (p->action == 8 && sprint)
		p->action = 10;
	if (p->action == 9 && sprint)
		p->action = 10;
}