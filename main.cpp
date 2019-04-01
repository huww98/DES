#include "DES.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// void test(bitset<64> data, bitset<64> key) {
//     cout << hex;
//     cout << "key:       " << key.to_ullong() << endl;
//     cout << "raw:       " << data.to_ullong() << endl;
//     auto encrypted = encryptBlock(data, key);
//     cout << "encrypted: " << encrypted.to_ullong() << endl;
//     auto decrypted = decryptBlock(encrypted, key);
//     cout << "decrypted: " << decrypted.to_ullong() << endl << endl;
// }

int main()
{
    DES des(bitset<64>(0x00));
    ifstream in("/dev/stdin", ios::binary);
    ofstream out("/dev/stdout", ios::binary);
    des.encrypt(in, out, bitset<64>(0x00));
    return 0;
}
