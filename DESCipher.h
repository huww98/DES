#include <bitset>
#include <array>

class DESCipher {
  public:
    static constexpr size_t subKeyCount = 16;
    using SubKeysType = std::array<std::bitset<48>, subKeyCount>;
    using KeyType = std::bitset<64>;
    using BlockType = std::bitset<64>;
    DESCipher(const KeyType &key);
    BlockType encryptBlock(const BlockType &block);
    BlockType decryptBlock(const BlockType &block);
  private:
    SubKeysType subKeys;
};
