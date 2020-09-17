#include <sstream>

#include "../../..//Utils/Logger.h"
#include "LifeboatFly.h"

/*
std::ostringstream ss;
ss << myFloat;
std::string s(ss.str());
*/

float glide = 1.001f;
float maxY = -1.f;
bool hasteleported = false;
vec3_t newPos;

// ignore that the filename is NoLiquids. Just ignore it.

LifeboatFly::LifeboatFly() : IModule(0, Category::MOVEMENT, "Allows you to fly on lifeboat without flagging the anticheat.") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.01f, 0.6f);
}

LifeboatFly::~LifeboatFly() {
}

const char* LifeboatFly::getModuleName() {
	return "LifeboatFly";
}

void LifeboatFly::onEnable() {
	maxY = -1.f;
	newPos = g_Data.getLocalPlayer()->currentPos;
	newPos.y += 0.2f;
	g_Data.getLocalPlayer()->jumpFromGround();
}

void LifeboatFly::onTick(C_GameMode* gm) {
	bool inGame = &GameData::isInGame;
	if (!inGame)
		return;
	auto* player = g_Data.getLocalPlayer();
	if (!player->onGround) {
		player->velocity.y = 0.f;
		if (player->isSneaking())
			player->velocity.y = -0.2f;
	}
	if (player->onGround) {
		glide = 1.001f;
	}
	auto yaw = gm->player->yaw;
	player->velocity.x = cos((yaw + 90.0f) * (3.14f / 180.0f)) * speed;  //0.6 bypasses
	player->velocity.z = sin((yaw + 90.0f) * (3.14f / 180.0f)) * speed;
	if (maxY > 0 && gm->player->currentPos.y > maxY)
		player->currentPos.y = maxY;
}

void LifeboatFly::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		auto player = g_Data.getLocalPlayer();
		if (!player->onGround) {
			if (player->velocity.y > -0.1f) {
				glide = glide - 0.001f;
			}
			movePacket->Position.y += glide;
			if (glide < -1.001f) {
				player->setPos(movePacket->Position);
				glide = 0.f;
				logF("reset glide value");
			}
		}
	}
}