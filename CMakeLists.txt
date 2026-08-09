cmake_minimum_required(VERSION 3.23)

project(gameboyEmulator)

add_executable(gameboy)

target_sources(gameboy
  PRIVATE 
    src/main.cpp
    src/Cartridge.cpp
    src/CPU.cpp
    src/file.cpp
    src/instructions.cpp
    src/MemoryMap.cpp
    src/utils.cpp
)

target_include_directories(gameboy
  PRIVATE 
    src
)