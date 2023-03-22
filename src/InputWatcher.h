#pragma once

using namespace SKSE;

using InputEvents = RE::InputEvent*;
class InputWatcher : public RE::BSTEventSink<InputEvents> {
public:
	virtual RE::BSEventNotifyControl ProcessEvent(const InputEvents* evns, RE::BSTEventSource<InputEvents>* dispatcher) override;
	TES_HEAP_REDEFINE_NEW();
};
