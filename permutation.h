#pragma once

#include <bitset>

std::bitset<64> ip(std::bitset<64> in);
std::bitset<64> fp(std::bitset<64> in);
std::bitset<48> e(std::bitset<32> in);
std::bitset<32> sp(std::bitset<48> in);

std::bitset<28> pc1Left(std::bitset<64> in);
std::bitset<28> pc1Right(std::bitset<64> in);
std::bitset<48> pc2(std::bitset<56> in);
