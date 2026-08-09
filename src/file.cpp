#include <fstream>

#include "file.h"

std::vector<uint8_t> readFile(std::filesystem::path path) {
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);

    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);

    return data;
}