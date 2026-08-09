#pragma once

#include <vector>
#include <cstdint>
#include <filesystem>

std::vector<uint8_t> readFile(std::filesystem::path path);