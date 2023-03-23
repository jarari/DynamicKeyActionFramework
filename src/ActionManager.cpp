#include "ActionManager.h"
#include "Globals.h"
#include "Utils.h"

using namespace RE;

ActionManager* ActionManager::instance = nullptr;

void ActionManager::EvaluateAndPerformActions() {
	std::unordered_set<Action*> queueList;
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
			queueList.insert(actionIt._Unwrapped());
		}
	}
	if (queueList.size() > 0 && !actionQueued) {
		SetActionQueued(true);
		SKSE::GetTaskInterface()->AddTask([queueList]() {
			for (auto queueIt = queueList.begin(); queueIt != queueList.end(); ++queueIt) {
				Action* action = (*queueIt);
				bool actionSuccess = false;
				//logger::debug("Try action {}", action->actionOrIdle->GetFormEditorID());
				if (action->actionOrIdle->formType.get() == FormType::Idle) {
					if (action->pairedSearchMode > Action::SEARCH_MODE::kNone) {
						Actor* target = nullptr;
						bhkPickData pick;
						NiPoint3 eyeOrig, eyeDir, origin, dir;
						NiPoint3 playerPos = Globals::p->GetPosition();
						auto process = ProcessLists::GetSingleton();
						Globals::p->GetEyeVector(eyeOrig, eyeDir, false);
						switch (action->pairedSearchMode) {
							case Action::SEARCH_MODE::kRayFromActor:
								//logger::debug("Ray from Actor");
								target = Utils::PickActor(pick, Globals::p->parentCell, eyeOrig, eyeDir, action->pairedSearchDist, Globals::p);
								if (target) {
									if (!Utils::ActorHasKeywords(target, action->pairedTargetKeywords)) {
										target = nullptr;
									}
								}
								break;
							case Action::SEARCH_MODE::kRayFromCamera:
								//logger::debug("Ray from Camera");
								origin = Main::WorldRootCamera()->world.translate;
								dir = NiPoint3(Main::WorldRootCamera()->world.rotate.entry[0][0],
											   Main::WorldRootCamera()->world.rotate.entry[1][0],
											   Main::WorldRootCamera()->world.rotate.entry[2][0]);
								Utils::Normalize(dir);
								target = Utils::PickActor(pick, Globals::p->parentCell, origin, dir, 3000.f, Globals::p);
								if (target) {
									//logger::debug("Target found");
									if (!Utils::ActorHasKeywords(target, action->pairedTargetKeywords)) {
										target = nullptr;
									}
									else {
										NiPoint3 hitPos = origin + (dir * 3000.f) * pick.rayOutput.hitFraction;
										//logger::debug("dist from eye {} from cam {}", hitPos.GetDistance(eyeOrig), hitPos.GetDistance(origin));
										if (hitPos.GetDistance(eyeOrig) > action->pairedSearchDist && hitPos.GetDistance(origin) > action->pairedSearchDist)
											target = nullptr;
									}
								}
								break;
							case Action::SEARCH_MODE::kRadius:
								if (process) {
									float minAng = 5.f;
									//logger::debug("Paired target keyword count {}", action->pairedTargetKeywords.size());
									for (const auto& handle : process->aliveActorList) {
										if (const auto actor = handle.get(); actor && actor.get() != Globals::p) {
											NiPoint3 actorPos = actor->GetPosition();
											if (actorPos.GetDistance(playerPos) <= action->pairedSearchDist) {
												if (action->pairedTargetKeywords.size() > 0
													&& !Utils::ActorHasKeywords(actor.get(), action->pairedTargetKeywords))
													continue;
												NiPoint3 actorDir = actorPos - playerPos;
												Utils::Normalize(actorDir);
												float angDiff = acos(eyeDir.Dot(actorDir));
												if (angDiff <= minAng) {
													target = actor.get();
													minAng = angDiff;
												}
											}
										}
									}
								}
								break;
							default:
								break;
						}
						if (target) {
							//logger::debug("Processing paired idle on target {}", target->GetActorBase()->GetFullName());
							if (Utils::PlayIdle(reinterpret_cast<TESIdleForm*>(action->actionOrIdle), Globals::p, action->actionAssocIdle, target)) {
								actionSuccess = true;
								//logger::debug("Idle Success");
							}
							else {
								//logger::debug("Idle Fail");
							}
						}
						else {
							//logger::debug("Idle No Target Found");
						}
					}
					else {
						if (Utils::PlayIdle(reinterpret_cast<TESIdleForm*>(action->actionOrIdle), Globals::p, action->actionAssocIdle, nullptr)) {
							actionSuccess = true;
							//logger::debug("Idle Success");
						}
						else {
							//logger::debug("Idle Fail");
						}
					}
				}
				else {
					if (Utils::PerformAction(reinterpret_cast<BGSAction*>(action->actionOrIdle), Globals::p)) {
						actionSuccess = true;
						//logger::debug("Action Success");
					}
					else {
						//logger::debug("Action Fail");
					}
				}
				if (actionSuccess) {
					for (auto keyIt = action->keys.begin(); keyIt != action->keys.end(); ++keyIt) {
						ActionManager::GetSingleton()->OnKeyReleased(*keyIt);
						ActionManager::GetSingleton()->AddFilterKey(*keyIt);
					}
					for (auto keyIt = action->gamepadKeys.begin(); keyIt != action->gamepadKeys.end(); ++keyIt) {
						ActionManager::GetSingleton()->OnKeyReleased(*keyIt);
						ActionManager::GetSingleton()->AddFilterKey(*keyIt);
					}
					break;
				}
			}
			ActionManager::GetSingleton()->SetActionQueued(false);
		});
	}
}