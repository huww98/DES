#include "DES.h"

#include "permutation.h"

#include <iostream>
#include <bitset>
#include <array>

using namespace std;

bitset<32> feistelFunction(bitset<32> halfBlock, bitset<48> subkey)
{
	// TODO
	return {};
}

bitset<64> feistel(const bitset<64> &plainText, const array<bitset<48>, 16> &subkeys)
{
	auto result = ip(plainText);
	for (size_t i = 0; i < subkeys.size(); i++)
	{
		bitset<32> right = result.to_ullong();
		bitset<32> left = result.to_ullong() >> 32;
		result = (right.to_ullong() << 32) ^ feistelFunction(left, subkeys[i]).to_ullong();
	}
	return fp(result);
}

template<typename TOutIt>
void scheduleKey(const bitset<64> &key, TOutIt outIt)
{
    // TODO
}

bitset<64> encryptBlock(const bitset<64> &plainText, const bitset<64> &key)
{
    array<bitset<48>, 16> subkeys;
    scheduleKey(key, subkeys.begin());
    return feistel(plainText, subkeys);
}

bitset<64> decryptBlock(const bitset<64> &plainText, const bitset<64> &key)
{
    array<bitset<48>, 16> subkeys;
    scheduleKey(key, subkeys.rbegin());
    return feistel(plainText, subkeys);
}
