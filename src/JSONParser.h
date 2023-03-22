#pragma once

#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace JSONParser {
	void LoadJSONFiles();
	void ParseKeyActionJSON(const std::filesystem::path path);
}