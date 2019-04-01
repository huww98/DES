#pragma once

#include <vector>
#include <cstring>

template<size_t blockByteCount>
class Padding {
  public:
    std::vector<char> operator()(const char * lastBlock, std::size_t byteCount) {
        auto paddedSize = (byteCount / blockByteCount + 1) * blockByteCount;
        std::vector<char> padded(paddedSize);
        std::memcpy(padded.data(), lastBlock, byteCount);
        auto paddingSize = paddedSize - byteCount;
        std::memset(padded.data() + byteCount, paddingSize, paddingSize);
        return padded;
    }
};
