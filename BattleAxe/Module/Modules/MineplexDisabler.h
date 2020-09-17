#pragma once
#include "../ModuleManager.h"

class MineplexDisabler : public IModule {
private:
	vec3_t lastPacketPosition;
	bool artificial = false;

public:
	MineplexDisabler();
	~MineplexDisabler();

	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onTick(C_GameMode* gm) override;
};