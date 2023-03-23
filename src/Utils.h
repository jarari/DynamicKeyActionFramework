#pragma once

namespace Utils {
	enum {
		// first 256 for keyboard, then 8 mouse buttons, then mouse wheel up, wheel down, then 16 gamepad buttons
		kMacro_KeyboardOffset = 0,		// not actually used, just for self-documentation
		kMacro_NumKeyboardKeys = 256,

		kMacro_MouseButtonOffset = kMacro_NumKeyboardKeys,	// 256
		kMacro_NumMouseButtons = 8,

		kMacro_MouseWheelOffset = kMacro_MouseButtonOffset + kMacro_NumMouseButtons,	// 264
		kMacro_MouseWheelDirections = 2,

		kMacro_GamepadOffset = kMacro_MouseWheelOffset + kMacro_MouseWheelDirections,	// 266
		kMacro_NumGamepadButtons = 16,

		kMaxMacros = kMacro_GamepadOffset + kMacro_NumGamepadButtons	// 282
	};

	enum {
		kGamepadButtonOffset_DPAD_UP = kMacro_GamepadOffset,	// 266
		kGamepadButtonOffset_DPAD_DOWN,
		kGamepadButtonOffset_DPAD_LEFT,
		kGamepadButtonOffset_DPAD_RIGHT,
		kGamepadButtonOffset_START,
		kGamepadButtonOffset_BACK,
		kGamepadButtonOffset_LEFT_THUMB,
		kGamepadButtonOffset_RIGHT_THUMB,
		kGamepadButtonOffset_LEFT_SHOULDER,
		kGamepadButtonOffset_RIGHT_SHOULDER,
		kGamepadButtonOffset_A,
		kGamepadButtonOffset_B,
		kGamepadButtonOffset_X,
		kGamepadButtonOffset_Y,
		kGamepadButtonOffset_LT,
		kGamepadButtonOffset_RT	// 281
	};

	uint32_t GamepadMaskToKeycode(uint32_t keyMask);
	std::string SplitString(const std::string str, const std::string delimiter, std::string& remainder);
	RE::TESForm* GetFormFromMod(std::string modname, uint32_t formid);
	RE::TESForm* GetFormFromConfigString(const std::string str);
	bool PerformAction(RE::BGSAction* action, RE::Actor* actor);
	bool PlayIdle(RE::TESIdleForm* idle, RE::Actor* actor, RE::DEFAULT_OBJECT action, RE::Actor* target);
	RE::Actor* PickActor(RE::bhkPickData& pick, RE::TESObjectCELL* cell, RE::NiPoint3 origin, RE::NiPoint3 dir, float dist, RE::Actor* excludedActor = nullptr);
	bool ActorHasKeywords(RE::Actor* actor, std::vector<RE::BGSKeyword*>& keywords);
	RE::NiPoint3 ToDirectionVector(RE::NiMatrix3 mat);
	RE::NiPoint3 ToUpVector(RE::NiMatrix3 mat);
	RE::NiPoint3 ToRightVector(RE::NiMatrix3 mat);
	void Normalize(RE::NiPoint3& p);
}