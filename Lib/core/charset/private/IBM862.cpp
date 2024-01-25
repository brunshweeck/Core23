//
// Created by T.N.Brunshweeck on 15/01/2024.
//

#include "IBM862.h"
#include <core/charset/private/SingleByte.h>

namespace core {
    namespace charset {
        IBM862::IBM862() :
            Charset("IBM862", StringArray::of("cp862", "ibm862", "ibm-862", "862", "csIBM862", "cspc862latinhebrew")) {
            SingleByte::initC2B(b2c, c2bNR, c2b, c2bIndex);
        }

        gbool IBM862::contains(const Charset &cs) const {
            return Class<IBM862>::hasInstance(cs);
        }

        CharsetDecoder &IBM862::decoder() const {
            return Unsafe::allocateInstance<SingleByte::Decoder>(INSTANCE, b2c, false, false);
        }

        CharsetEncoder &IBM862::encoder() const {
            return Unsafe::allocateInstance<SingleByte::Encoder>(INSTANCE, c2b, c2bIndex, false);
        }

        Object &IBM862::clone() const {
            return INSTANCE;
        }

        CharArray IBM862::b2c = CharArray::of(
                0x05D0, 0x05D1, 0x05D2, 0x05D3, 0x05D4, 0x05D5, 0x05D6, 0x05D7,      // 0x80 - 0x87
                0x05D8, 0x05D9, 0x05DA, 0x05DB, 0x05DC, 0x05DD, 0x05DE, 0x05DF,      // 0x88 - 0x8f
                0x05E0, 0x05E1, 0x05E2, 0x05E3, 0x05E4, 0x05E5, 0x05E6, 0x05E7,      // 0x90 - 0x97
                0x05E8, 0x05E9, 0x05EA, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,      // 0x98 - 0x9f
                0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,      // 0xa0 - 0xa7
                0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,      // 0xa8 - 0xaf
                0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,      // 0xb0 - 0xb7
                0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,      // 0xb8 - 0xbf
                0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F,      // 0xc0 - 0xc7
                0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,      // 0xc8 - 0xcf
                0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B,      // 0xd0 - 0xd7
                0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,      // 0xd8 - 0xdf
                0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4,      // 0xe0 - 0xe7
                0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,      // 0xe8 - 0xef
                0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248,      // 0xf0 - 0xf7
                0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0,      // 0xf8 - 0xff
                0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,      // 0x00 - 0x07
                0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,      // 0x08 - 0x0f
                0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,      // 0x10 - 0x17
                0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,      // 0x18 - 0x1f
                0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,      // 0x20 - 0x27
                0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,      // 0x28 - 0x2f
                0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,      // 0x30 - 0x37
                0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,      // 0x38 - 0x3f
                0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,      // 0x40 - 0x47
                0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,      // 0x48 - 0x4f
                0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,      // 0x50 - 0x57
                0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,      // 0x58 - 0x5f
                0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,      // 0x60 - 0x67
                0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,      // 0x68 - 0x6f
                0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,      // 0x70 - 0x77
                0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F       // 0x78 - 0x7f
        );

        CharArray IBM862::c2b = CharArray(0x800);
        CharArray IBM862::c2bIndex = CharArray(0x100);
        CharArray IBM862::c2bNR = CharArray(0);

        // initialize after b2c, c2b, ... for calling SingleByte.initC2B
        IBM862 IBM862::INSTANCE{};
    } // charset
} // core