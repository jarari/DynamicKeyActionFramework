#include "ActionManager.h"
#include "Globals.h"
#include "Utils.h"

using namespace RE;

ActionManager* ActionManager::instance = nullptr;

void ActionManager::EvaluateAndPerformActions() {
	std::unordered_map<TESForm*, DEFAULT_OBJECT> queueList;
	for (auto actionIt = actions.begin(); actionIt != actions.end(); ++actionIt) {
		bool doAction = false;
		int keyMatchFound = 0;
		float minDuration = std::numeric_limits<float>::infinity();
		for (auto keyIt = actionIt->keys.begin(); keyIt != actionIt->keys.end(); ++keyIt) {
			auto found = pressedKeys.find(*keyIt);
			if (found != pressedKeys.end()) {
				if (found->second <= minDuration) {
					++keyMatchFound;
					minDuration = found->second;
				}
				else {
					break;
				}
			}
		}
		if (keyMatchFound == actionIt->keys.size() && minDuration >= actionIt->pressDuration) {
			doAction = true;
		}
		else {
			int gamepadKeyMatchFound = 0;
			float gamepadMinDuration = std::numeric_limits<float>::infinity();
			for (auto keyIt = actionIt->gamepadKeys.begin(); keyIt != actionIt->gamepadKeys.end(); ++keyIt) {
				auto found = pressedKeys.find(*keyIt);
				if (found != pressedKeys.end()) {
					if (found->second <= minDuration) {
						++gamepadKeyMatchFound;
						gamepadMinDuration = found->second;
					}
					else {
						break;
					}
				}
			}
			if (gamepadKeyMatchFound == actionIt->keys.size() && gamepadMinDuration >= actionIt->pressDuration) {
				doAction = true;
			}
		}

		if (doAction) {
			queueList.insert(std::pair<TESForm*, DEFAULT_OBJECT>(actionIt->actionOrIdle, actionIt->actionAssocIdle));
		}
	}
	if (queueList.size() > 0) {
		SKSE::GetTaskInterface()->AddTask([queueList]() {
			for (auto queueIt = queueList.begin(); queueIt != queueList.end(); ++queueIt) {
				if (queueIt->first->formType.get() == FormType::Idle) {
					//TODO: Implement Paired Idle Animation
					if (Utils::PlayIdle(reinterpret_cast<TESIdleForm*>(queueIt->first), Globals::p, queueIt->second, nullptr))
						break;
				}
				else {
					if (Utils::PerformAction(reinterpret_cast<BGSAction*>(queueIt->first), Globals::p))
						break;
				}
			}
		});
	}
}