//
// Created by T.N.Brunshweeck on 15/01/2024.
//

#include "Windows1258.h"
#include <core/charset/private/SingleByte.h>

namespace core {
    namespace charset {
        Windows1258::Windows1258() : Charset("windows-1258", StringArray::of("cp1258", "cp5346")) {
            SingleByte::initC2B(b2c, c2bNR, c2b, c2bIndex);
        }

        gbool Windows1258::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII") || Class<Windows1258>::hasInstance(cs);
        }

        CharsetDecoder &Windows1258::decoder() const {
            return Unsafe::allocateInstance<SingleByte::Decoder>(INSTANCE, b2c, true, false);
        }

        CharsetEncoder &Windows1258::encoder() const {
            return Unsafe::allocateInstance<SingleByte::Encoder>(INSTANCE, c2b, c2bIndex, true);
        }

        Object &Windows1258::clone() const {
            return INSTANCE;
        }

        CharArray Windows1258::b2c = CharArray::of(
                0x20AC, 0xFFFD, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,      // 0x80 - 0x87
                0x02C6, 0x2030, 0xFFFD, 0x2039, 0x0152, 0xFFFD, 0xFFFD, 0xFFFD,      // 0x88 - 0x8f
                0xFFFD, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,      // 0x90 - 0x97
                0x02DC, 0x2122, 0xFFFD, 0x203A, 0x0153, 0xFFFD, 0xFFFD, 0x0178,      // 0x98 - 0x9f
                0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,      // 0xa0 - 0xa7
                0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,      // 0xa8 - 0xaf
                0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,      // 0xb0 - 0xb7
                0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,      // 0xb8 - 0xbf
                0x00C0, 0x00C1, 0x00C2, 0x0102, 0x00C4, 0x00C5, 0x00C6, 0x00C7,      // 0xc0 - 0xc7
                0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x0300, 0x00CD, 0x00CE, 0x00CF,      // 0xc8 - 0xcf
                0x0110, 0x00D1, 0x0309, 0x00D3, 0x00D4, 0x01A0, 0x00D6, 0x00D7,      // 0xd0 - 0xd7
                0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x01AF, 0x0303, 0x00DF,      // 0xd8 - 0xdf
                0x00E0, 0x00E1, 0x00E2, 0x0103, 0x00E4, 0x00E5, 0x00E6, 0x00E7,      // 0xe0 - 0xe7
                0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x0301, 0x00ED, 0x00EE, 0x00EF,      // 0xe8 - 0xef
                0x0111, 0x00F1, 0x0323, 0x00F3, 0x00F4, 0x01A1, 0x00F6, 0x00F7,      // 0xf0 - 0xf7
                0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x01B0, 0x20AB, 0x00FF,      // 0xf8 - 0xff
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
        CharArray Windows1258::c2b = CharArray(0x700);
        CharArray Windows1258::c2bIndex = CharArray(0x100);
        CharArray Windows1258::c2bNR = CharArray(0);

        // initialize after b2c, c2b, ... for calling SingleByte.initC2B
        Windows1258 Windows1258::INSTANCE{};
    } // charset
} // core