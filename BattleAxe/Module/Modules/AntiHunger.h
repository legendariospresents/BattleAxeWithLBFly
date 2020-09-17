#pragma once
#include "../ModuleManager.h"

class AntiHunger : public IModule {
private:
	bool sprint = true;

public:
	AntiHunger();
	~AntiHunger();

	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onEnable() override;
};