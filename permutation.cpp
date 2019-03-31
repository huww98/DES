#include "permutation.h"

#include "permutationData.h"

#include <array>
#include <bitset>

using namespace std;

constexpr auto buildFpTable() {
	array<uint8_t, 64> table{};
	for (size_t i = 0; i < table.size(); i++)
	{
		table[ipTable[i]] = uint8_t(i);
	}
	return table;
}

constexpr auto fpTable = buildFpTable();

template<size_t outSize>
/** Do permutation acroding to permutationTable.
 * It runs slow, and should be used to build mapping table only
 */
constexpr uint64_t slowPermutation(const array<uint8_t, outSize> &permutationTable, uint64_t in) {
	uint64_t result = 0;
	for (size_t i = 0; i < outSize; i++)
	{
		result >>= 1;
		result |= (in << permutationTable[i]) & (uint64_t(1) << outSize - 1);
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

constexpr uint32_t slowP(uint32_t in) { return slowPermutation(pTable, in); }

constexpr uint8_t slowS(uint8_t sBoxIndex, uint8_t in) {
    int rowIdx = in & 0b000001;
    rowIdx ^= (in >> 4) & 0b000010;
    int columnIdx = (in >> 1) & 0b001111;
    return sBoxes[sBoxIndex][rowIdx][columnIdx];
}

constexpr auto buildSPMapping() {
    array<array<uint32_t, 64>, 8> mapping{};
    for (size_t b = 0; b < mapping.size(); b++)
	{
		auto& m = mapping[b];
		for (uint64_t i = 0; i < m.size(); i++)
		{
			m[size_t(i)] = slowP(slowS(b, i) << (4 * b));
		}
	}
    return mapping;
}

constexpr auto spMapping = buildSPMapping();

bitset<32> sp(bitset<48> in) {
    bitset<32> result;
	for (size_t i = 0; i < spMapping.size(); i++)
	{
		result |= spMapping[i][in.to_ulong() & 0b111111];
        in >> 6;
    }
    return result;
}
