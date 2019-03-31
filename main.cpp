#include "DES.h"
#include "permutation.h"

#include <iostream>
#include <iomanip>

using namespace std;

void test(bitset<64> data, bitset<64> key) {
    cout << hex;
    cout << "key:       " << key.to_ullong() << endl;
    cout << "raw:       " << data.to_ullong() << endl;
    auto encrypted = encryptBlock(data, key);
    cout << "encrypted: " << encrypted.to_ullong() << endl;
    auto decrypted = decryptBlock(encrypted, key);
    cout << "decrypted: " << decrypted.to_ullong() << endl << endl;
}

int main()
{
    test(bitset<64>("0000"), bitset<64>(0x12345678));
    test(bitset<64>("0001"), bitset<64>(0x12345678));

    return 0;
}
