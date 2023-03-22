#pragma once

#include <unordered_map>
#include <vector>

struct Action {
public:
	std::vector<uint32_t> keys;
	std::vector<uint32_t> gamepadKeys;
	std::vector<RE::BGSKeyword*> pairedTargetKeywords;
	RE::TESForm* actionOrIdle;
	RE::DEFAULT_OBJECT actionAssocIdle;
	float pressDuration;
};

class ActionManager {
protected:
	static ActionManager* instance;
	std::vector<Action> actions;
	std::unordered_map<uint32_t, float> pressedKeys;
public:
	ActionManager() = default;
	ActionManager(ActionManager&) = delete;
	void operator=(const ActionManager&) = delete;
	static ActionManager* GetSingleton() {
		if (!instance)
			instance = new ActionManager();
		return instance;
	}
	bool AddAction(Action action) {
		if ((action.keys.size() == 0
			 && action.gamepadKeys.size() == 0)
			|| !action.actionOrIdle)
			return false;
		for (auto actionIt = actions.begin(); actionIt != actions.end(); ++actionIt) {
			if (actionIt->keys == action.keys
				&& actionIt->gamepadKeys == action.gamepadKeys
				&& actionIt->pairedTargetKeywords == action.pairedTargetKeywords
				&& actionIt->actionOrIdle == action.actionOrIdle
				&& actionIt->pressDuration == action.pressDuration) {
				return false;
			}
		}
		actions.push_back(action);
		return true;
	}
	void ClearActions() {
		actions.clear();
	}
	void EvaluateAndPerformActions();

	void OnKeyPressed(uint32_t keyCode, float duration) {
		auto found = pressedKeys.find(keyCode);
		if (found != pressedKeys.end()) {
			found->second = duration;
		}
		else {
			pressedKeys.insert(std::pair<uint32_t, float>(keyCode, duration));
		}
	}
	void OnKeyReleased(uint32_t keyCode) {
		auto found = pressedKeys.find(keyCode);
		if (found != pressedKeys.end()) {
			pressedKeys.erase(found);
		}
	}
	void ClearPressedKeys() {
		pressedKeys.clear();
	}
};