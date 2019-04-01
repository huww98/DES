#pragma once

#include <vector>
#include <cstring>
#include <cstdint>

template<size_t blockByteCount>
class Padding {
  public:
    std::vector<std::uint8_t> operator()(const std::uint8_t * lastBlock, std::size_t byteCount) {
        auto paddedSize = (byteCount / blockByteCount + 1) * blockByteCount;
        std::vector<std::uint8_t> padded(paddedSize);
        std::memcpy(padded.data(), lastBlock, byteCount);
        auto paddingSize = paddedSize - byteCount;
        std::memset(padded.data() + byteCount, paddingSize, paddingSize);
        return padded;
    }
};
