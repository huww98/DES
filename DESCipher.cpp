#include "DESCipher.h"

#include "permutation.h"

#include <iostream>
#include <bitset>
#include <array>

using namespace std;

bitset<32> feistelFunction(bitset<32> halfBlock, bitset<48> subkey)
{
    return sp(e(halfBlock) ^ subkey);
}

template<typename TSubKeyIt>
bitset<64> feistel(const bitset<64> &plainText, TSubKeyIt subKeyIt)
{
	auto result = ip(plainText);
    bitset<32> right = result.to_ullong();
    bitset<32> left = result.to_ullong() >> 32;
    for (size_t i = 0; i < DESCipher::subKeyCount; i++)
    {
        auto nextLeft = right;
        right = left ^ feistelFunction(right, *subKeyIt);
        left = nextLeft;
        ++subKeyIt;
    }
    return fp(right.to_ullong() << 32 | left.to_ullong());
}

template<size_t size>
bitset<size> rotateLeft(const bitset<size> &data, int count) {
    return data << count | data >> (data.size() - count);
}

constexpr array<uint8_t, 16> numOfRotation{1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

auto scheduleKey(const bitset<64> &key)
{
    auto left = pc1Left(key);
    auto right = pc1Right(key);
    DESCipher::SubKeysType subKeys;
    auto outIt = subKeys.begin();
    for (auto num : numOfRotation)
    {
        left = rotateLeft(left, num);
        right = rotateLeft(right, num);
        *outIt = pc2(left.to_ullong() << right.size() | right.to_ullong());
        ++outIt;
    }
    return subKeys;
}

DESCipher::DESCipher(const KeyType &key) : subKeys(scheduleKey(key)) {}

auto DESCipher::encryptBlock(const BlockType &block) -> BlockType
{
    return feistel(block, this->subKeys.begin());
}

auto DESCipher::decryptBlock(const bitset<64> &block) -> BlockType
{
    return feistel(block, this->subKeys.rbegin());
}
