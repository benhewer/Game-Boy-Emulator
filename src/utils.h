#pragma once

#include <cstdint>
#include <cstdlib>

uint16_t mergeBytes(uint8_t msb, uint8_t lsb);

uint8_t lsb(uint16_t twoBytes);
uint8_t msb(uint16_t twoBytes);

uint8_t readBit(uint8_t byte, size_t position);
void setBit(uint8_t& byte, uint8_t bit, size_t position);