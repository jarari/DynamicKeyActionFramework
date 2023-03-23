#include "MenuWatcher.h"
#include "ActionManager.h"

RE::BSEventNotifyControl MenuWatcher::ProcessEvent(const RE::MenuOpenCloseEvent* evn, RE::BSTEventSource<RE::MenuOpenCloseEvent>* dispatcher) {
	if (evn->menuName == RE::InterfaceStrings::GetSingleton()->loadingMenu && evn->opening) {
		ActionManager::GetSingleton()->ClearPressedKeys();
		ActionManager::GetSingleton()->ClearFilterKeys();
	}
	return RE::BSEventNotifyControl::kContinue;
}
