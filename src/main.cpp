#include <cstdint>
#include <chrono>
#include <cstdio>

#include "CPU.h"
#include "file.h"

// all measurements are standard e.g. hz, s
constexpr double CLK_FREQ = 4'194'304.0;
constexpr auto CLK_PERIOD = std::chrono::duration<double>(1.0 / CLK_FREQ);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: ./gameboy path/to/rom";
        return 1;
    }

    std::filesystem::path path = argv[1];
    auto data = readFile(path);

    Cartridge cartridge{std::move(data)};
    MemoryMap memory{cartridge};
    CPU cpu{memory};

    auto prevTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> accumulator{};

    while (true) {
        auto curTime = std::chrono::steady_clock::now();
        accumulator += curTime - prevTime;
        prevTime = curTime;

        while (accumulator >= CLK_PERIOD) {
            cpu.tick();

            accumulator -= CLK_PERIOD;
        }
    }
}