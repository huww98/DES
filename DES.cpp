#include "DES.h"

#include "permutation.h"

#include <iostream>
#include <bitset>
#include <array>

using namespace std;

bitset<32> feistelFunction(bitset<32> halfBlock, bitset<48> subkey)
{
    return sp(e(halfBlock) ^ subkey);
}

bitset<64> feistel(const bitset<64> &plainText, const array<bitset<48>, 16> &subkeys)
{
	auto result = ip(plainText);
    bitset<32> right = result.to_ullong();
    bitset<32> left = result.to_ullong() >> 32;
    for (size_t i = 0; i < subkeys.size(); i++)
    {
        auto nextLeft = right;
        right = left ^ feistelFunction(right, subkeys[i]);
        left = nextLeft;
        // cout << dec << i << " " << hex << left.to_ullong() << " " << right.to_ullong() << endl;
    }
    return fp(right.to_ullong() << 32 | left.to_ullong());
}

template<size_t size>
bitset<size> rotateLeft(const bitset<size> &data, int count) {
    return data << count | data >> (data.size() - count);
}

constexpr array<uint8_t, 16> numOfRotation{1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

template<typename TOutIt>
void scheduleKey(const bitset<64> &key, TOutIt outIt)
{
    auto left = pc1Left(key);
    auto right = pc1Right(key);
    for(auto num : numOfRotation)
    {
        left = rotateLeft(left, num);
        right = rotateLeft(right, num);
        *outIt = pc2(left.to_ullong() << right.size() | right.to_ullong());
        ++outIt;
    }
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
