//
// Created by T.N.Brunshweeck on 15/01/2024.
//

#include "ISO8859_5.h"
#include <core/charset/private/SingleByte.h>

namespace core {
    namespace charset {
        ISO8859_5::ISO8859_5() :
            Charset("ISO-8859-5", StringArray::of("iso8859_5", "8859_5", "iso-ir-144", "ISO_8859-5", "ISO_8859-5:1988", "ISO8859-5", "cyrillic", "ibm915", "ibm-915", "cp915", "915", "csISOLatinCyrillic")) {
            SingleByte::initC2B(b2c, c2bNR, c2b, c2bIndex);
        }

        gbool ISO8859_5::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII") || Class<ISO8859_5>::hasInstance(cs);
        }

        CharsetDecoder &ISO8859_5::decoder() const {
            return Unsafe::allocateInstance<SingleByte::Decoder>(INSTANCE, b2c, true, false);
        }

        CharsetEncoder &ISO8859_5::encoder() const {
            return Unsafe::allocateInstance<SingleByte::Encoder>(INSTANCE, c2b, c2bIndex, true);
        }

        Object &ISO8859_5::clone() const {
            return INSTANCE;
        }

        CharArray ISO8859_5::b2c = CharArray::of(
                0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,      // 0x80 - 0x87
                0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,      // 0x88 - 0x8f
                0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,      // 0x90 - 0x97
                0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,      // 0x98 - 0x9f
                0x00A0, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407,      // 0xa0 - 0xa7
                0x0408, 0x0409, 0x040A, 0x040B, 0x040C, 0x00AD, 0x040E, 0x040F,      // 0xa8 - 0xaf
                0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,      // 0xb0 - 0xb7
                0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,      // 0xb8 - 0xbf
                0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,      // 0xc0 - 0xc7
                0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,      // 0xc8 - 0xcf
                0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,      // 0xd0 - 0xd7
                0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,      // 0xd8 - 0xdf
                0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,      // 0xe0 - 0xe7
                0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F,      // 0xe8 - 0xef
                0x2116, 0x0451, 0x0452, 0x0453, 0x0454, 0x0455, 0x0456, 0x0457,      // 0xf0 - 0xf7
                0x0458, 0x0459, 0x045A, 0x045B, 0x045C, 0x00A7, 0x045E, 0x045F,      // 0xf8 - 0xff
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

        CharArray ISO8859_5::c2b = CharArray(0x300);
        CharArray ISO8859_5::c2bIndex = CharArray(0x100);
        CharArray ISO8859_5::c2bNR = CharArray(0);

        // initialize after b2c, c2b, ... for calling SingleByte.initC2B
        ISO8859_5 ISO8859_5::INSTANCE{};
    } // charset
} // core