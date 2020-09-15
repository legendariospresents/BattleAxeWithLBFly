#pragma once

#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"

class Ignite : public IModule {
private:
	bool lava = false;
	bool extinguish = true;
	void findWeapon();
	int prevSlot = 10;

public:
	Ignite();
	~Ignite();

	void tryScaffold(vec3_t blockBelow);

	bool items = true;
	bool switched = true;

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};