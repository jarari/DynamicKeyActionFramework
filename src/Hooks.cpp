#include "Hooks.h"
#include "Globals.h"
#include "InputWatcher.h"
#include "MenuWatcher.h"

void Hooks::InitializeHooks() {
	InputWatcher* iw = new InputWatcher();
	Globals::inputDeviceManager->AddEventSink(iw);
	MenuWatcher* mw = new MenuWatcher();
	Globals::ui->GetEventSource<RE::MenuOpenCloseEvent>()->AddEventSink(mw);
}
