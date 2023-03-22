#include "Utils.h"
#include <Xinput.h>

using namespace RE;

uint32_t Utils::GamepadMaskToKeycode(uint32_t keyMask) {
	switch (keyMask) {
		case XINPUT_GAMEPAD_DPAD_UP:		return kGamepadButtonOffset_DPAD_UP;
		case XINPUT_GAMEPAD_DPAD_DOWN:		return kGamepadButtonOffset_DPAD_DOWN;
		case XINPUT_GAMEPAD_DPAD_LEFT:		return kGamepadButtonOffset_DPAD_LEFT;
		case XINPUT_GAMEPAD_DPAD_RIGHT:		return kGamepadButtonOffset_DPAD_RIGHT;
		case XINPUT_GAMEPAD_START:			return kGamepadButtonOffset_START;
		case XINPUT_GAMEPAD_BACK:			return kGamepadButtonOffset_BACK;
		case XINPUT_GAMEPAD_LEFT_THUMB:		return kGamepadButtonOffset_LEFT_THUMB;
		case XINPUT_GAMEPAD_RIGHT_THUMB:	return kGamepadButtonOffset_RIGHT_THUMB;
		case XINPUT_GAMEPAD_LEFT_SHOULDER:	return kGamepadButtonOffset_LEFT_SHOULDER;
		case XINPUT_GAMEPAD_RIGHT_SHOULDER: return kGamepadButtonOffset_RIGHT_SHOULDER;
		case XINPUT_GAMEPAD_A:				return kGamepadButtonOffset_A;
		case XINPUT_GAMEPAD_B:				return kGamepadButtonOffset_B;
		case XINPUT_GAMEPAD_X:				return kGamepadButtonOffset_X;
		case XINPUT_GAMEPAD_Y:				return kGamepadButtonOffset_Y;
		case 0x9:							return kGamepadButtonOffset_LT;
		case 0xA:							return kGamepadButtonOffset_RT;
		default:							return kMaxMacros; // Invalid
	}
}

std::string Utils::SplitString(const std::string str, const std::string delimiter, std::string& remainder) {
	std::string ret;
	size_t i = str.find(delimiter);
	if (i == std::string::npos) {
		ret = str;
		remainder = "";
		return ret;
	}

	ret = str.substr(0, i);
	remainder = str.substr(i + 1);
	return ret;
}

TESForm* Utils::GetFormFromMod(std::string modname, uint32_t formid) {
	if (!modname.length() || !formid)
		return nullptr;
	TESDataHandler* dh = TESDataHandler::GetSingleton();
	TESFile* modFile = nullptr;
	for (auto it = dh->files.begin(); it != dh->files.end(); ++it) {
		TESFile* f = *it;
		if (strcmp(f->fileName, modname.c_str()) == 0) {
			modFile = f;
			break;
		}
	}
	if (!modFile)
		return nullptr;
	uint8_t modIndex = modFile->compileIndex;
	uint32_t id = formid;
	if (modIndex < 0xFE) {
		id |= ((uint32_t)modIndex) << 24;
	}
	else {
		uint16_t lightModIndex = modFile->smallFileCompileIndex;
		if (lightModIndex != 0xFFFF) {
			id |= 0xFE000000 | (uint32_t(lightModIndex) << 12);
		}
	}
	return TESForm::LookupByID(id);
}

TESForm* Utils::GetFormFromConfigString(const std::string str) {
	std::string formIDstr;
	std::string plugin = SplitString(str, "|", formIDstr);
	if (formIDstr.length() != 0) {
		uint32_t formID = std::stoi(formIDstr, 0, 16);
		return GetFormFromMod(plugin, formID);
	}
	return nullptr;
}

bool Utils::PerformAction(BGSAction* action, Actor* actor) {
	if (action && actor) {
		std::unique_ptr<TESActionData> data(TESActionData::Create());
		data->source = NiPointer<TESObjectREFR>(actor);
		data->action = action;
		typedef bool func_t(TESActionData*);
		REL::Relocation<func_t> func{ RELOCATION_ID(40551, 41557) };
		return func(data.get());
	}
	return false;
}

bool Utils::PlayIdle(TESIdleForm* idle, Actor* actor, DEFAULT_OBJECT action, Actor* target) {
	if (actor && actor->GetActorRuntimeData().currentProcess) {
		typedef bool (*func_t)(AIProcess*, Actor*, DEFAULT_OBJECT, TESIdleForm*, bool, bool, Actor*);
		REL::Relocation<func_t> func{ RELOCATION_ID(38290, 39256) };
		return func(actor->GetActorRuntimeData().currentProcess, actor, action, idle, true, true, target);
	}
	return false;
}
