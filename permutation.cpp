#include "permutation.h"

#include <array>
#include <bitset>

using namespace std;

template<size_t size>
constexpr auto minusOne(array<uint8_t, size> table) {
    for (auto& i : table) {
		i--;
	}
    return table;
}

constexpr auto ipTable = minusOne<64>({
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9,  1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7,
});

constexpr auto buildFpTable() {
	array<uint8_t, 64> table{};
	for (size_t i = 0; i < table.size(); i++)
	{
		table[ipTable[i]] = uint8_t(i);
	}
	return table;
}

constexpr auto fpTable = buildFpTable();

constexpr auto eTable = minusOne<48>({
    32, 1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9,  10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1,
});

constexpr auto pTable = minusOne<32>({
    16, 7,  20, 21, 29, 12, 28, 17,
    1,  15, 23, 26, 5,  18, 31, 10,
    2,  8,  24, 14, 32, 27, 3,  9,
    19, 13, 30, 6,  22, 11, 4,  25,
});

/** Used to build mapping table only
 */
template<size_t outSize>
constexpr uint64_t slowPermutation(const array<uint8_t, outSize> &permutationTable, uint64_t in) {
	uint64_t result = 0;
	for (size_t i = 0; i < outSize; i++)
	{
		result |= (in << permutationTable[i]) & (uint64_t(1) << outSize - 1);
		result >>= 1;
	}
	return result;
}

template<size_t inSize>
using PermutationMappingType = array<array<uint64_t, 256>, inSize/8>;

template<size_t outSize, size_t inSize>
constexpr auto buildPermutationMapping(const array<uint8_t, outSize> &permutationTable) {
	PermutationMappingType<inSize> mapping{};
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

constexpr auto ipMapping = buildPermutationMapping<64,64>(ipTable);
constexpr auto fpMapping = buildPermutationMapping<64,64>(fpTable);
constexpr auto eMapping = buildPermutationMapping<48,32>(eTable);

template<size_t outSize, size_t inSize>
auto permutation(const PermutationMappingType<inSize> &mapping, bitset<inSize> in)
{
	bitset<outSize> result;
	for (size_t i = 0; i < mapping.size(); i++)
	{
		result |= mapping[i][in.to_ulong() & 0xFF];
        in >> 8;
    }
    return result;
}

bitset<64> ip(bitset<64> in) { return permutation<64>(ipMapping, in); }
bitset<64> fp(bitset<64> in) { return permutation<64>(fpMapping, in); }

bitset<48> e(bitset<32> in) { return permutation<48>(eMapping, in); }
