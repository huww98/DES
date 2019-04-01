#include "Padding.h"

#include <istream>
#include <ostream>
#include <bitset>
#include <array>
#include <vector>

template <typename Chipher, size_t blockSize, size_t keySize, typename PaddingType = Padding<blockSize / 8>>
class CBC
{
  public:
    static constexpr auto blockByteCount = blockSize / 8;
    using BlockDataType = std::array<uint8_t, blockByteCount>;
    using BlockType = std::bitset<blockSize>;

  private:
    Chipher chipher;
    PaddingType padding;

    void processBlock(BlockDataType blockData, BlockType &iv, std::ostream &out) {
        BlockType block;
        for (auto b : blockData) {
            block <<= 8;
            block |= BlockType(b);
        }
        block ^= iv;
        BlockType outBlock = chipher.encryptBlock(block);
        iv = outBlock;
        BlockDataType outData;
        for (auto it = outData.rbegin(); it != outData.rend(); ++it)
        {
            *it = (uint8_t)(outBlock & BlockType(0xFF)).to_ulong();
            outBlock >>= 8;
        }
        out.write(reinterpret_cast<char*>(outData.data()), blockByteCount);
    }

  public:
    CBC(std::bitset<keySize> key) : chipher(key) {}

    void encrypt(std::istream &in, std::ostream &out, BlockType iv) {
        BlockDataType data;
        while (true)
        {
            in.read(reinterpret_cast<char*>(data.data()), blockByteCount);
            if (!in)
                break;
            processBlock(data, iv, out);
        }
        auto readBytes = in.gcount();
        std::vector<uint8_t> padded = padding(data.data(), readBytes);
        for(size_t i = 0; i < padded.size(); i+=blockByteCount)
        {
            BlockDataType data;
            std::memcpy(data.data(), padded.data() + i * blockByteCount, blockByteCount);
            processBlock(data, iv, out);
        }
    }
};
