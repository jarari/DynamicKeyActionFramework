#include "InputWatcher.h"
#include "ActionManager.h"
#include "Utils.h"

using namespace RE;

BSEventNotifyControl InputWatcher::ProcessEvent(const InputEvents* evns, BSTEventSource<InputEvents>* dispatcher) {
	if (!*evns)
		return RE::BSEventNotifyControl::kContinue;

	for (InputEvent* e = *evns; e; e = e->next) {
		switch (e->eventType.get()) {
			case INPUT_EVENT_TYPE::kButton:
				ButtonEvent* a_event = e->AsButtonEvent();

				uint32_t keyMask = a_event->idCode;
				uint32_t keyCode;

				// Mouse
				if (a_event->device.get() == INPUT_DEVICE::kMouse) {
					keyCode = Utils::kMacro_NumKeyboardKeys + keyMask;
				}
				// Gamepad
				else if (a_event->device.get() == INPUT_DEVICE::kGamepad) {
					keyCode = Utils::GamepadMaskToKeycode(keyMask);
				}
				// Keyboard
				else
					keyCode = keyMask;

				// Valid scancode?
				if (keyCode >= Utils::kMaxMacros)
					continue;

				float duration = a_event->heldDownSecs;
				bool isPressed = a_event->value != 0 && duration >= 0;
				bool isReleased = a_event->value == 0 && duration != 0;

				if (isPressed) {
					ActionManager::GetSingleton()->OnKeyPressed(keyCode, duration);
					ActionManager::GetSingleton()->EvaluateAndPerformActions();
				}
				else if (isReleased) {
					ActionManager::GetSingleton()->OnKeyReleased(keyCode);
				}
		}
	}
	return BSEventNotifyControl::kContinue;
}
