#include "permutation.h"

#include <array>
#include <bitset>

using namespace std;

constexpr auto buildIpTable() {
	array<uint8_t, 64> ipTable{
		58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9,  1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7,
	};
	for (auto& i : ipTable) {
		i--;
	}
	return ipTable;
}

constexpr auto ipTable = buildIpTable();

constexpr auto buildFpTable() {
	array<uint8_t, 64> table{};
	for (size_t i = 0; i < table.size(); i++)
	{
		table[ipTable[i]] = uint8_t(i);
	}
	return table;
}

constexpr auto fpTable = buildFpTable();

constexpr uint64_t slowPermutation(const array<uint8_t, 64> &permutationTable, uint64_t in) {
	uint64_t result = 0;
	for (size_t i = 0; i < 64; i++)
	{
		result |= in << (permutationTable[i]) & (uint64_t(1) << 63);
		result >>= 1;
	}
	return result;
}

using PermutationMappingType = array<array<uint64_t, 256>, 8>;

constexpr auto buildPermutationMapping(const array<uint8_t, 64> &permutationTable) {
	PermutationMappingType mapping{};
	for (size_t b = 0; b < mapping.size(); b++)
	{
		auto& m = mapping[b];
		for (uint64_t i = 0; i < m.size(); i++)
		{
			m[size_t(i)] = slowPermutation(permutationTable, i << (8 * b));
		}
	}
	return mapping;
}

constexpr auto ipMapping = buildPermutationMapping(ipTable);
constexpr auto fpMapping = buildPermutationMapping(fpTable);

bitset<64> permutation(const PermutationMappingType &mapping, bitset<64> in)
{
	bitset<64> result;
	for (size_t i = 0; i < 8; i++)
	{
		result |= mapping[i][in.to_ulong() & 0xFF];
	}
	return result;
}

bitset<64> ip(bitset<64> in) { return permutation(ipMapping, in); }
bitset<64> fp(bitset<64> in) { return permutation(fpMapping, in); }
