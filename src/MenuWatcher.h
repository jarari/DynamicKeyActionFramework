#pragma once

using namespace SKSE;

class MenuWatcher : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
public:
	virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* evn, RE::BSTEventSource<RE::MenuOpenCloseEvent>* dispatcher) override;
	TES_HEAP_REDEFINE_NEW();
};
