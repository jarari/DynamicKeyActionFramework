#pragma once
#include "ActionManager.h"

namespace Globals {
	extern RE::PlayerCharacter* p;
	extern RE::PlayerControls* pc;
	extern RE::PlayerCamera* pcam;
	extern RE::UI* ui;
	extern RE::ControlMap* controlMap;
	extern RE::UserEvents* userEvents;
	extern RE::BSInputDeviceManager* inputDeviceManager;
	void InitializeGlobalVaribles();
}