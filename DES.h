#pragma once

#include "CBC.h"
#include "DESCipher.h"

using DES_CBC = CBC<DESCipher, 64, 64>;
using DES = DES_CBC;
