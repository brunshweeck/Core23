//
// Created by T.N.Brunshweeck on 15/01/2024.
//

#include "Windows1256.h"
#include <core/charset/private/SingleByte.h>

namespace core {
    namespace charset {
        Windows1256::Windows1256() : Charset("windows-1256", StringArray::of("cp1256", "cp5346")) {
            SingleByte::initC2B(b2c, c2bNR, c2b, c2bIndex);
        }

        gbool Windows1256::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII") || Class<Windows1256>::hasInstance(cs);
        }

        CharsetDecoder &Windows1256::decoder() const {
            return Unsafe::allocateInstance<SingleByte::Decoder>(INSTANCE, b2c, true, false);
        }

        CharsetEncoder &Windows1256::encoder() const {
            return Unsafe::allocateInstance<SingleByte::Encoder>(INSTANCE, c2b, c2bIndex, true);
        }

        Object &Windows1256::clone() const {
            return INSTANCE;
        }

        CharArray Windows1256::b2c = CharArray::of(
                0x20AC, 0x067E, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,      // 0x80 - 0x87
                0x02C6, 0x2030, 0x0679, 0x2039, 0x0152, 0x0686, 0x0698, 0x0688,      // 0x88 - 0x8f
                0x06AF, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,      // 0x90 - 0x97
                0x06A9, 0x2122, 0x0691, 0x203A, 0x0153, 0x200C, 0x200D, 0x06BA,      // 0x98 - 0x9f
                0x00A0, 0x060C, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,      // 0xa0 - 0xa7
                0x00A8, 0x00A9, 0x06BE, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,      // 0xa8 - 0xaf
                0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,      // 0xb0 - 0xb7
                0x00B8, 0x00B9, 0x061B, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x061F,      // 0xb8 - 0xbf
                0x06C1, 0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627,      // 0xc0 - 0xc7
                0x0628, 0x0629, 0x062A, 0x062B, 0x062C, 0x062D, 0x062E, 0x062F,      // 0xc8 - 0xcf
                0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x00D7,      // 0xd0 - 0xd7
                0x0637, 0x0638, 0x0639, 0x063A, 0x0640, 0x0641, 0x0642, 0x0643,      // 0xd8 - 0xdf
                0x00E0, 0x0644, 0x00E2, 0x0645, 0x0646, 0x0647, 0x0648, 0x00E7,      // 0xe0 - 0xe7
                0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x0649, 0x064A, 0x00EE, 0x00EF,      // 0xe8 - 0xef
                0x064B, 0x064C, 0x064D, 0x064E, 0x00F4, 0x064F, 0x0650, 0x00F7,      // 0xf0 - 0xf7
                0x0651, 0x00F9, 0x0652, 0x00FB, 0x00FC, 0x200E, 0x200F, 0x06D2,      // 0xf8 - 0xff
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
        CharArray Windows1256::c2b = CharArray(0x600);
        CharArray Windows1256::c2bIndex = CharArray(0x100);
        CharArray Windows1256::c2bNR = CharArray(0);

        // initialize after b2c, c2b, ... for calling SingleByte.initC2B
        Windows1256 Windows1256::INSTANCE{};
    } // charset
} // core