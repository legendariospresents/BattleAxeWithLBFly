#pragma once
#include "Module.h"
class LifeboatFly : public IModule {
private:
	float speed = 0.2f;

public:
	LifeboatFly();
	~LifeboatFly();
	// Inherited by IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onSendPacket(C_Packet* packet) override;
};