#pragma once

// Axiom core
#include "Logger/Logger.h"
#include "Utils/Time/Time.h"

// Essential
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// Json
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using csv = std::vector<std::vector<std::string>>;
// ImGui
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
