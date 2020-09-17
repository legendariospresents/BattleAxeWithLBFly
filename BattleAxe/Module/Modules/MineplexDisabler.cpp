#include "MineplexDisabler.h"

MineplexDisabler::MineplexDisabler() : IModule(0x0, Category::MISC, "Disables Mineplex vertical checks") {
}

MineplexDisabler::~MineplexDisabler() {
}

const char* MineplexDisabler::getModuleName() {
	return "MineplexDisabler";
}

void MineplexDisabler::onTick(C_GameMode* gm) {
	if (lastPacketPosition.y >= 0.f && lastPacketPosition != nullptr) {
		artificial = true;  // make sure the packet hook knows we generated it
		C_MovePlayerPacket pp(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pp);
	}
	// For some reason, Nukkit's velocity calculation doesn't care about how fast you send da packets so we use that to our advantage to disable mineplex lul
}

void MineplexDisabler::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		auto player = g_Data.getLocalPlayer();
		if (!artificial)
			lastPacketPosition = movePacket->Position;
		if (artificial) {
			movePacket->Position = lastPacketPosition;
			movePacket->pitch = player->pitch;
			movePacket->yaw = player->bodyYaw;
			movePacket->headYaw = player->yaw;
			artificial = false;
		}
	}
}