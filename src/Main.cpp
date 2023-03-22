#include <stddef.h>
#include <Xinput.h>
#include "Globals.h"
#include "Hooks.h"
#include "JSONParser.h"

using namespace RE;
using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

const MessagingInterface* g_message = nullptr;

namespace {
	void InitializeLogging() {
		auto path = log_directory();
		if (!path) {
			report_and_fail("Unable to lookup SKSE logs directory.");
		}
		*path /= PluginDeclaration::GetSingleton()->GetName();
		*path += L".log";

		std::shared_ptr<spdlog::logger> log;
		if (IsDebuggerPresent()) {
			log = std::make_shared<spdlog::logger>(
				"Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
		}
		else {
			log = std::make_shared<spdlog::logger>(
				"Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
		}
		log->set_level(spdlog::level::trace);
		log->flush_on(spdlog::level::trace);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
	}
}

SKSEPluginLoad(const LoadInterface* skse) {
	InitializeLogging();

	auto* plugin = PluginDeclaration::GetSingleton();
	auto version = plugin->GetVersion();
	logger::info("{} {} is loading...", plugin->GetName(), version);


	Init(skse);
	g_message = GetMessagingInterface();
	g_message->RegisterListener([](MessagingInterface::Message* msg) -> void {
		if (msg->type == MessagingInterface::kDataLoaded) {
			Globals::InitializeGlobalVaribles();
			Hooks::InitializeHooks();
			JSONParser::LoadJSONFiles();
		}
	});

	logger::info("{} has finished loading.", plugin->GetName());
	return true;
}
