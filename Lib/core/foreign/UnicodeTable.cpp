//
// Created by T.N.Brunshweeck on 20/11/2023.
//

#include <core/ArgumentException.h>
#include <core/Integer.h>
#include "UnicodeTable.h"

namespace core {
    namespace foreign {

        namespace {
            CORE_ALIAS(UnicodeScript, Character::UnicodeScript);
            CORE_ALIAS(UnicodeBlock, Character::UnicodeBlock);

            CORE_FAST gint X[] = {
                    0x0000,   // 0000..0040; COMMON
                    0x0041,   // 0041..005A; LATIN
                    0x005B,   // 005B..0060; COMMON
                    0x0061,   // 0061..007A; LATIN
                    0x007B,   // 007B..00A9; COMMON
                    0x00AA,   // 00AA      ; LATIN
                    0x00AB,   // 00AB..00B9; COMMON
                    0x00BA,   // 00BA      ; LATIN
                    0x00BB,   // 00BB..00BF; COMMON
                    0x00C0,   // 00C0..00D6; LATIN
                    0x00D7,   // 00D7      ; COMMON
                    0x00D8,   // 00D8..00F6; LATIN
                    0x00F7,   // 00F7      ; COMMON
                    0x00F8,   // 00F8..02B8; LATIN
                    0x02B9,   // 02B9..02DF; COMMON
                    0x02E0,   // 02E0..02E4; LATIN
                    0x02E5,   // 02E5..02E9; COMMON
                    0x02EA,   // 02EA..02EB; BOPOMOFO
                    0x02EC,   // 02EC..02FF; COMMON
                    0x0300,   // 0300..036F; INHERITED
                    0x0370,   // 0370..0373; GREEK
                    0x0374,   // 0374      ; COMMON
                    0x0375,   // 0375..0377; GREEK
                    0x0378,   // 0378..0379; UNKNOWN
                    0x037A,   // 037A..037D; GREEK
                    0x037E,   // 037E      ; COMMON
                    0x037F,   // 037F      ; GREEK
                    0x0380,   // 0380..0383; UNKNOWN
                    0x0384,   // 0384      ; GREEK
                    0x0385,   // 0385      ; COMMON
                    0x0386,   // 0386      ; GREEK
                    0x0387,   // 0387      ; COMMON
                    0x0388,   // 0388..038A; GREEK
                    0x038B,   // 038B      ; UNKNOWN
                    0x038C,   // 038C      ; GREEK
                    0x038D,   // 038D      ; UNKNOWN
                    0x038E,   // 038E..03A1; GREEK
                    0x03A2,   // 03A2      ; UNKNOWN
                    0x03A3,   // 03A3..03E1; GREEK
                    0x03E2,   // 03E2..03EF; COPTIC
                    0x03F0,   // 03F0..03FF; GREEK
                    0x0400,   // 0400..0484; CYRILLIC
                    0x0485,   // 0485..0486; INHERITED
                    0x0487,   // 0487..052F; CYRILLIC
                    0x0530,   // 0530      ; UNKNOWN
                    0x0531,   // 0531..0556; ARMENIAN
                    0x0557,   // 0557..0558; UNKNOWN
                    0x0559,   // 0559..058A; ARMENIAN
                    0x058B,   // 058B..058C; UNKNOWN
                    0x058D,   // 058D..058F; ARMENIAN
                    0x0590,   // 0590      ; UNKNOWN
                    0x0591,   // 0591..05C7; HEBREW
                    0x05C8,   // 05C8..05CF; UNKNOWN
                    0x05D0,   // 05D0..05EA; HEBREW
                    0x05EB,   // 05EB..05EE; UNKNOWN
                    0x05EF,   // 05EF..05F4; HEBREW
                    0x05F5,   // 05F5..05FF; UNKNOWN
                    0x0600,   // 0600..0604; ARABIC
                    0x0605,   // 0605      ; COMMON
                    0x0606,   // 0606..060B; ARABIC
                    0x060C,   // 060C      ; COMMON
                    0x060D,   // 060D..061A; ARABIC
                    0x061B,   // 061B      ; COMMON
                    0x061C,   // 061C..061E; ARABIC
                    0x061F,   // 061F      ; COMMON
                    0x0620,   // 0620..063F; ARABIC
                    0x0640,   // 0640      ; COMMON
                    0x0641,   // 0641..064A; ARABIC
                    0x064B,   // 064B..0655; INHERITED
                    0x0656,   // 0656..066F; ARABIC
                    0x0670,   // 0670      ; INHERITED
                    0x0671,   // 0671..06DC; ARABIC
                    0x06DD,   // 06DD      ; COMMON
                    0x06DE,   // 06DE..06FF; ARABIC
                    0x0700,   // 0700..070D; SYRIAC
                    0x070E,   // 070E      ; UNKNOWN
                    0x070F,   // 070F..074A; SYRIAC
                    0x074B,   // 074B..074C; UNKNOWN
                    0x074D,   // 074D..074F; SYRIAC
                    0x0750,   // 0750..077F; ARABIC
                    0x0780,   // 0780..07B1; THAANA
                    0x07B2,   // 07B2..07BF; UNKNOWN
                    0x07C0,   // 07C0..07FA; NKO
                    0x07FB,   // 07FB..07FC; UNKNOWN
                    0x07FD,   // 07FD..07FF; NKO
                    0x0800,   // 0800..082D; SAMARITAN
                    0x082E,   // 082E..082F; UNKNOWN
                    0x0830,   // 0830..083E; SAMARITAN
                    0x083F,   // 083F      ; UNKNOWN
                    0x0840,   // 0840..085B; MANDAIC
                    0x085C,   // 085C..085D; UNKNOWN
                    0x085E,   // 085E      ; MANDAIC
                    0x085F,   // 085F      ; UNKNOWN
                    0x0860,   // 0860..086A; SYRIAC
                    0x086B,   // 086B..086F; UNKNOWN
                    0x0870,   // 0870..088E; ARABIC
                    0x088F,   // 088F      ; UNKNOWN
                    0x0890,   // 0890..0891; ARABIC
                    0x0892,   // 0892..0897; UNKNOWN
                    0x0898,   // 0898..08E1; ARABIC
                    0x08E2,   // 08E2      ; COMMON
                    0x08E3,   // 08E3..08FF; ARABIC
                    0x0900,   // 0900..0950; DEVANAGARI
                    0x0951,   // 0951..0954; INHERITED
                    0x0955,   // 0955..0963; DEVANAGARI
                    0x0964,   // 0964..0965; COMMON
                    0x0966,   // 0966..097F; DEVANAGARI
                    0x0980,   // 0980..0983; BENGALI
                    0x0984,   // 0984      ; UNKNOWN
                    0x0985,   // 0985..098C; BENGALI
                    0x098D,   // 098D..098E; UNKNOWN
                    0x098F,   // 098F..0990; BENGALI
                    0x0991,   // 0991..0992; UNKNOWN
                    0x0993,   // 0993..09A8; BENGALI
                    0x09A9,   // 09A9      ; UNKNOWN
                    0x09AA,   // 09AA..09B0; BENGALI
                    0x09B1,   // 09B1      ; UNKNOWN
                    0x09B2,   // 09B2      ; BENGALI
                    0x09B3,   // 09B3..09B5; UNKNOWN
                    0x09B6,   // 09B6..09B9; BENGALI
                    0x09BA,   // 09BA..09BB; UNKNOWN
                    0x09BC,   // 09BC..09C4; BENGALI
                    0x09C5,   // 09C5..09C6; UNKNOWN
                    0x09C7,   // 09C7..09C8; BENGALI
                    0x09C9,   // 09C9..09CA; UNKNOWN
                    0x09CB,   // 09CB..09CE; BENGALI
                    0x09CF,   // 09CF..09D6; UNKNOWN
                    0x09D7,   // 09D7      ; BENGALI
                    0x09D8,   // 09D8..09DB; UNKNOWN
                    0x09DC,   // 09DC..09DD; BENGALI
                    0x09DE,   // 09DE      ; UNKNOWN
                    0x09DF,   // 09DF..09E3; BENGALI
                    0x09E4,   // 09E4..09E5; UNKNOWN
                    0x09E6,   // 09E6..09FE; BENGALI
                    0x09FF,   // 09FF..0A00; UNKNOWN
                    0x0A01,   // 0A01..0A03; GURMUKHI
                    0x0A04,   // 0A04      ; UNKNOWN
                    0x0A05,   // 0A05..0A0A; GURMUKHI
                    0x0A0B,   // 0A0B..0A0E; UNKNOWN
                    0x0A0F,   // 0A0F..0A10; GURMUKHI
                    0x0A11,   // 0A11..0A12; UNKNOWN
                    0x0A13,   // 0A13..0A28; GURMUKHI
                    0x0A29,   // 0A29      ; UNKNOWN
                    0x0A2A,   // 0A2A..0A30; GURMUKHI
                    0x0A31,   // 0A31      ; UNKNOWN
                    0x0A32,   // 0A32..0A33; GURMUKHI
                    0x0A34,   // 0A34      ; UNKNOWN
                    0x0A35,   // 0A35..0A36; GURMUKHI
                    0x0A37,   // 0A37      ; UNKNOWN
                    0x0A38,   // 0A38..0A39; GURMUKHI
                    0x0A3A,   // 0A3A..0A3B; UNKNOWN
                    0x0A3C,   // 0A3C      ; GURMUKHI
                    0x0A3D,   // 0A3D      ; UNKNOWN
                    0x0A3E,   // 0A3E..0A42; GURMUKHI
                    0x0A43,   // 0A43..0A46; UNKNOWN
                    0x0A47,   // 0A47..0A48; GURMUKHI
                    0x0A49,   // 0A49..0A4A; UNKNOWN
                    0x0A4B,   // 0A4B..0A4D; GURMUKHI
                    0x0A4E,   // 0A4E..0A50; UNKNOWN
                    0x0A51,   // 0A51      ; GURMUKHI
                    0x0A52,   // 0A52..0A58; UNKNOWN
                    0x0A59,   // 0A59..0A5C; GURMUKHI
                    0x0A5D,   // 0A5D      ; UNKNOWN
                    0x0A5E,   // 0A5E      ; GURMUKHI
                    0x0A5F,   // 0A5F..0A65; UNKNOWN
                    0x0A66,   // 0A66..0A76; GURMUKHI
                    0x0A77,   // 0A77..0A80; UNKNOWN
                    0x0A81,   // 0A81..0A83; GUJARATI
                    0x0A84,   // 0A84      ; UNKNOWN
                    0x0A85,   // 0A85..0A8D; GUJARATI
                    0x0A8E,   // 0A8E      ; UNKNOWN
                    0x0A8F,   // 0A8F..0A91; GUJARATI
                    0x0A92,   // 0A92      ; UNKNOWN
                    0x0A93,   // 0A93..0AA8; GUJARATI
                    0x0AA9,   // 0AA9      ; UNKNOWN
                    0x0AAA,   // 0AAA..0AB0; GUJARATI
                    0x0AB1,   // 0AB1      ; UNKNOWN
                    0x0AB2,   // 0AB2..0AB3; GUJARATI
                    0x0AB4,   // 0AB4      ; UNKNOWN
                    0x0AB5,   // 0AB5..0AB9; GUJARATI
                    0x0ABA,   // 0ABA..0ABB; UNKNOWN
                    0x0ABC,   // 0ABC..0AC5; GUJARATI
                    0x0AC6,   // 0AC6      ; UNKNOWN
                    0x0AC7,   // 0AC7..0AC9; GUJARATI
                    0x0ACA,   // 0ACA      ; UNKNOWN
                    0x0ACB,   // 0ACB..0ACD; GUJARATI
                    0x0ACE,   // 0ACE..0ACF; UNKNOWN
                    0x0AD0,   // 0AD0      ; GUJARATI
                    0x0AD1,   // 0AD1..0ADF; UNKNOWN
                    0x0AE0,   // 0AE0..0AE3; GUJARATI
                    0x0AE4,   // 0AE4..0AE5; UNKNOWN
                    0x0AE6,   // 0AE6..0AF1; GUJARATI
                    0x0AF2,   // 0AF2..0AF8; UNKNOWN
                    0x0AF9,   // 0AF9..0AFF; GUJARATI
                    0x0B00,   // 0B00      ; UNKNOWN
                    0x0B01,   // 0B01..0B03; ORIYA
                    0x0B04,   // 0B04      ; UNKNOWN
                    0x0B05,   // 0B05..0B0C; ORIYA
                    0x0B0D,   // 0B0D..0B0E; UNKNOWN
                    0x0B0F,   // 0B0F..0B10; ORIYA
                    0x0B11,   // 0B11..0B12; UNKNOWN
                    0x0B13,   // 0B13..0B28; ORIYA
                    0x0B29,   // 0B29      ; UNKNOWN
                    0x0B2A,   // 0B2A..0B30; ORIYA
                    0x0B31,   // 0B31      ; UNKNOWN
                    0x0B32,   // 0B32..0B33; ORIYA
                    0x0B34,   // 0B34      ; UNKNOWN
                    0x0B35,   // 0B35..0B39; ORIYA
                    0x0B3A,   // 0B3A..0B3B; UNKNOWN
                    0x0B3C,   // 0B3C..0B44; ORIYA
                    0x0B45,   // 0B45..0B46; UNKNOWN
                    0x0B47,   // 0B47..0B48; ORIYA
                    0x0B49,   // 0B49..0B4A; UNKNOWN
                    0x0B4B,   // 0B4B..0B4D; ORIYA
                    0x0B4E,   // 0B4E..0B54; UNKNOWN
                    0x0B55,   // 0B55..0B57; ORIYA
                    0x0B58,   // 0B58..0B5B; UNKNOWN
                    0x0B5C,   // 0B5C..0B5D; ORIYA
                    0x0B5E,   // 0B5E      ; UNKNOWN
                    0x0B5F,   // 0B5F..0B63; ORIYA
                    0x0B64,   // 0B64..0B65; UNKNOWN
                    0x0B66,   // 0B66..0B77; ORIYA
                    0x0B78,   // 0B78..0B81; UNKNOWN
                    0x0B82,   // 0B82..0B83; TAMIL
                    0x0B84,   // 0B84      ; UNKNOWN
                    0x0B85,   // 0B85..0B8A; TAMIL
                    0x0B8B,   // 0B8B..0B8D; UNKNOWN
                    0x0B8E,   // 0B8E..0B90; TAMIL
                    0x0B91,   // 0B91      ; UNKNOWN
                    0x0B92,   // 0B92..0B95; TAMIL
                    0x0B96,   // 0B96..0B98; UNKNOWN
                    0x0B99,   // 0B99..0B9A; TAMIL
                    0x0B9B,   // 0B9B      ; UNKNOWN
                    0x0B9C,   // 0B9C      ; TAMIL
                    0x0B9D,   // 0B9D      ; UNKNOWN
                    0x0B9E,   // 0B9E..0B9F; TAMIL
                    0x0BA0,   // 0BA0..0BA2; UNKNOWN
                    0x0BA3,   // 0BA3..0BA4; TAMIL
                    0x0BA5,   // 0BA5..0BA7; UNKNOWN
                    0x0BA8,   // 0BA8..0BAA; TAMIL
                    0x0BAB,   // 0BAB..0BAD; UNKNOWN
                    0x0BAE,   // 0BAE..0BB9; TAMIL
                    0x0BBA,   // 0BBA..0BBD; UNKNOWN
                    0x0BBE,   // 0BBE..0BC2; TAMIL
                    0x0BC3,   // 0BC3..0BC5; UNKNOWN
                    0x0BC6,   // 0BC6..0BC8; TAMIL
                    0x0BC9,   // 0BC9      ; UNKNOWN
                    0x0BCA,   // 0BCA..0BCD; TAMIL
                    0x0BCE,   // 0BCE..0BCF; UNKNOWN
                    0x0BD0,   // 0BD0      ; TAMIL
                    0x0BD1,   // 0BD1..0BD6; UNKNOWN
                    0x0BD7,   // 0BD7      ; TAMIL
                    0x0BD8,   // 0BD8..0BE5; UNKNOWN
                    0x0BE6,   // 0BE6..0BFA; TAMIL
                    0x0BFB,   // 0BFB..0BFF; UNKNOWN
                    0x0C00,   // 0C00..0C0C; TELUGU
                    0x0C0D,   // 0C0D      ; UNKNOWN
                    0x0C0E,   // 0C0E..0C10; TELUGU
                    0x0C11,   // 0C11      ; UNKNOWN
                    0x0C12,   // 0C12..0C28; TELUGU
                    0x0C29,   // 0C29      ; UNKNOWN
                    0x0C2A,   // 0C2A..0C39; TELUGU
                    0x0C3A,   // 0C3A..0C3B; UNKNOWN
                    0x0C3C,   // 0C3C..0C44; TELUGU
                    0x0C45,   // 0C45      ; UNKNOWN
                    0x0C46,   // 0C46..0C48; TELUGU
                    0x0C49,   // 0C49      ; UNKNOWN
                    0x0C4A,   // 0C4A..0C4D; TELUGU
                    0x0C4E,   // 0C4E..0C54; UNKNOWN
                    0x0C55,   // 0C55..0C56; TELUGU
                    0x0C57,   // 0C57      ; UNKNOWN
                    0x0C58,   // 0C58..0C5A; TELUGU
                    0x0C5B,   // 0C5B..0C5C; UNKNOWN
                    0x0C5D,   // 0C5D      ; TELUGU
                    0x0C5E,   // 0C5E..0C5F; UNKNOWN
                    0x0C60,   // 0C60..0C63; TELUGU
                    0x0C64,   // 0C64..0C65; UNKNOWN
                    0x0C66,   // 0C66..0C6F; TELUGU
                    0x0C70,   // 0C70..0C76; UNKNOWN
                    0x0C77,   // 0C77..0C7F; TELUGU
                    0x0C80,   // 0C80..0C8C; KANNADA
                    0x0C8D,   // 0C8D      ; UNKNOWN
                    0x0C8E,   // 0C8E..0C90; KANNADA
                    0x0C91,   // 0C91      ; UNKNOWN
                    0x0C92,   // 0C92..0CA8; KANNADA
                    0x0CA9,   // 0CA9      ; UNKNOWN
                    0x0CAA,   // 0CAA..0CB3; KANNADA
                    0x0CB4,   // 0CB4      ; UNKNOWN
                    0x0CB5,   // 0CB5..0CB9; KANNADA
                    0x0CBA,   // 0CBA..0CBB; UNKNOWN
                    0x0CBC,   // 0CBC..0CC4; KANNADA
                    0x0CC5,   // 0CC5      ; UNKNOWN
                    0x0CC6,   // 0CC6..0CC8; KANNADA
                    0x0CC9,   // 0CC9      ; UNKNOWN
                    0x0CCA,   // 0CCA..0CCD; KANNADA
                    0x0CCE,   // 0CCE..0CD4; UNKNOWN
                    0x0CD5,   // 0CD5..0CD6; KANNADA
                    0x0CD7,   // 0CD7..0CDC; UNKNOWN
                    0x0CDD,   // 0CDD..0CDE; KANNADA
                    0x0CDF,   // 0CDF      ; UNKNOWN
                    0x0CE0,   // 0CE0..0CE3; KANNADA
                    0x0CE4,   // 0CE4..0CE5; UNKNOWN
                    0x0CE6,   // 0CE6..0CEF; KANNADA
                    0x0CF0,   // 0CF0      ; UNKNOWN
                    0x0CF1,   // 0CF1..0CF3; KANNADA
                    0x0CF4,   // 0CF4..0CFF; UNKNOWN
                    0x0D00,   // 0D00..0D0C; MALAYALAM
                    0x0D0D,   // 0D0D      ; UNKNOWN
                    0x0D0E,   // 0D0E..0D10; MALAYALAM
                    0x0D11,   // 0D11      ; UNKNOWN
                    0x0D12,   // 0D12..0D44; MALAYALAM
                    0x0D45,   // 0D45      ; UNKNOWN
                    0x0D46,   // 0D46..0D48; MALAYALAM
                    0x0D49,   // 0D49      ; UNKNOWN
                    0x0D4A,   // 0D4A..0D4F; MALAYALAM
                    0x0D50,   // 0D50..0D53; UNKNOWN
                    0x0D54,   // 0D54..0D63; MALAYALAM
                    0x0D64,   // 0D64..0D65; UNKNOWN
                    0x0D66,   // 0D66..0D7F; MALAYALAM
                    0x0D80,   // 0D80      ; UNKNOWN
                    0x0D81,   // 0D81..0D83; SINHALA
                    0x0D84,   // 0D84      ; UNKNOWN
                    0x0D85,   // 0D85..0D96; SINHALA
                    0x0D97,   // 0D97..0D99; UNKNOWN
                    0x0D9A,   // 0D9A..0DB1; SINHALA
                    0x0DB2,   // 0DB2      ; UNKNOWN
                    0x0DB3,   // 0DB3..0DBB; SINHALA
                    0x0DBC,   // 0DBC      ; UNKNOWN
                    0x0DBD,   // 0DBD      ; SINHALA
                    0x0DBE,   // 0DBE..0DBF; UNKNOWN
                    0x0DC0,   // 0DC0..0DC6; SINHALA
                    0x0DC7,   // 0DC7..0DC9; UNKNOWN
                    0x0DCA,   // 0DCA      ; SINHALA
                    0x0DCB,   // 0DCB..0DCE; UNKNOWN
                    0x0DCF,   // 0DCF..0DD4; SINHALA
                    0x0DD5,   // 0DD5      ; UNKNOWN
                    0x0DD6,   // 0DD6      ; SINHALA
                    0x0DD7,   // 0DD7      ; UNKNOWN
                    0x0DD8,   // 0DD8..0DDF; SINHALA
                    0x0DE0,   // 0DE0..0DE5; UNKNOWN
                    0x0DE6,   // 0DE6..0DEF; SINHALA
                    0x0DF0,   // 0DF0..0DF1; UNKNOWN
                    0x0DF2,   // 0DF2..0DF4; SINHALA
                    0x0DF5,   // 0DF5..0E00; UNKNOWN
                    0x0E01,   // 0E01..0E3A; THAI
                    0x0E3B,   // 0E3B..0E3E; UNKNOWN
                    0x0E3F,   // 0E3F      ; COMMON
                    0x0E40,   // 0E40..0E5B; THAI
                    0x0E5C,   // 0E5C..0E80; UNKNOWN
                    0x0E81,   // 0E81..0E82; LAO
                    0x0E83,   // 0E83      ; UNKNOWN
                    0x0E84,   // 0E84      ; LAO
                    0x0E85,   // 0E85      ; UNKNOWN
                    0x0E86,   // 0E86..0E8A; LAO
                    0x0E8B,   // 0E8B      ; UNKNOWN
                    0x0E8C,   // 0E8C..0EA3; LAO
                    0x0EA4,   // 0EA4      ; UNKNOWN
                    0x0EA5,   // 0EA5      ; LAO
                    0x0EA6,   // 0EA6      ; UNKNOWN
                    0x0EA7,   // 0EA7..0EBD; LAO
                    0x0EBE,   // 0EBE..0EBF; UNKNOWN
                    0x0EC0,   // 0EC0..0EC4; LAO
                    0x0EC5,   // 0EC5      ; UNKNOWN
                    0x0EC6,   // 0EC6      ; LAO
                    0x0EC7,   // 0EC7      ; UNKNOWN
                    0x0EC8,   // 0EC8..0ECE; LAO
                    0x0ECF,   // 0ECF      ; UNKNOWN
                    0x0ED0,   // 0ED0..0ED9; LAO
                    0x0EDA,   // 0EDA..0EDB; UNKNOWN
                    0x0EDC,   // 0EDC..0EDF; LAO
                    0x0EE0,   // 0EE0..0EFF; UNKNOWN
                    0x0F00,   // 0F00..0F47; TIBETAN
                    0x0F48,   // 0F48      ; UNKNOWN
                    0x0F49,   // 0F49..0F6C; TIBETAN
                    0x0F6D,   // 0F6D..0F70; UNKNOWN
                    0x0F71,   // 0F71..0F97; TIBETAN
                    0x0F98,   // 0F98      ; UNKNOWN
                    0x0F99,   // 0F99..0FBC; TIBETAN
                    0x0FBD,   // 0FBD      ; UNKNOWN
                    0x0FBE,   // 0FBE..0FCC; TIBETAN
                    0x0FCD,   // 0FCD      ; UNKNOWN
                    0x0FCE,   // 0FCE..0FD4; TIBETAN
                    0x0FD5,   // 0FD5..0FD8; COMMON
                    0x0FD9,   // 0FD9..0FDA; TIBETAN
                    0x0FDB,   // 0FDB..0FFF; UNKNOWN
                    0x1000,   // 1000..109F; MYANMAR
                    0x10A0,   // 10A0..10C5; GEORGIAN
                    0x10C6,   // 10C6      ; UNKNOWN
                    0x10C7,   // 10C7      ; GEORGIAN
                    0x10C8,   // 10C8..10CC; UNKNOWN
                    0x10CD,   // 10CD      ; GEORGIAN
                    0x10CE,   // 10CE..10CF; UNKNOWN
                    0x10D0,   // 10D0..10FA; GEORGIAN
                    0x10FB,   // 10FB      ; COMMON
                    0x10FC,   // 10FC..10FF; GEORGIAN
                    0x1100,   // 1100..11FF; HANGUL
                    0x1200,   // 1200..1248; ETHIOPIC
                    0x1249,   // 1249      ; UNKNOWN
                    0x124A,   // 124A..124D; ETHIOPIC
                    0x124E,   // 124E..124F; UNKNOWN
                    0x1250,   // 1250..1256; ETHIOPIC
                    0x1257,   // 1257      ; UNKNOWN
                    0x1258,   // 1258      ; ETHIOPIC
                    0x1259,   // 1259      ; UNKNOWN
                    0x125A,   // 125A..125D; ETHIOPIC
                    0x125E,   // 125E..125F; UNKNOWN
                    0x1260,   // 1260..1288; ETHIOPIC
                    0x1289,   // 1289      ; UNKNOWN
                    0x128A,   // 128A..128D; ETHIOPIC
                    0x128E,   // 128E..128F; UNKNOWN
                    0x1290,   // 1290..12B0; ETHIOPIC
                    0x12B1,   // 12B1      ; UNKNOWN
                    0x12B2,   // 12B2..12B5; ETHIOPIC
                    0x12B6,   // 12B6..12B7; UNKNOWN
                    0x12B8,   // 12B8..12BE; ETHIOPIC
                    0x12BF,   // 12BF      ; UNKNOWN
                    0x12C0,   // 12C0      ; ETHIOPIC
                    0x12C1,   // 12C1      ; UNKNOWN
                    0x12C2,   // 12C2..12C5; ETHIOPIC
                    0x12C6,   // 12C6..12C7; UNKNOWN
                    0x12C8,   // 12C8..12D6; ETHIOPIC
                    0x12D7,   // 12D7      ; UNKNOWN
                    0x12D8,   // 12D8..1310; ETHIOPIC
                    0x1311,   // 1311      ; UNKNOWN
                    0x1312,   // 1312..1315; ETHIOPIC
                    0x1316,   // 1316..1317; UNKNOWN
                    0x1318,   // 1318..135A; ETHIOPIC
                    0x135B,   // 135B..135C; UNKNOWN
                    0x135D,   // 135D..137C; ETHIOPIC
                    0x137D,   // 137D..137F; UNKNOWN
                    0x1380,   // 1380..1399; ETHIOPIC
                    0x139A,   // 139A..139F; UNKNOWN
                    0x13A0,   // 13A0..13F5; CHEROKEE
                    0x13F6,   // 13F6..13F7; UNKNOWN
                    0x13F8,   // 13F8..13FD; CHEROKEE
                    0x13FE,   // 13FE..13FF; UNKNOWN
                    0x1400,   // 1400..167F; CANADIAN_ABORIGINAL
                    0x1680,   // 1680..169C; OGHAM
                    0x169D,   // 169D..169F; UNKNOWN
                    0x16A0,   // 16A0..16EA; RUNIC
                    0x16EB,   // 16EB..16ED; COMMON
                    0x16EE,   // 16EE..16F8; RUNIC
                    0x16F9,   // 16F9..16FF; UNKNOWN
                    0x1700,   // 1700..1715; TAGALOG
                    0x1716,   // 1716..171E; UNKNOWN
                    0x171F,   // 171F      ; TAGALOG
                    0x1720,   // 1720..1734; HANUNOO
                    0x1735,   // 1735..1736; COMMON
                    0x1737,   // 1737..173F; UNKNOWN
                    0x1740,   // 1740..1753; BUHID
                    0x1754,   // 1754..175F; UNKNOWN
                    0x1760,   // 1760..176C; TAGBANWA
                    0x176D,   // 176D      ; UNKNOWN
                    0x176E,   // 176E..1770; TAGBANWA
                    0x1771,   // 1771      ; UNKNOWN
                    0x1772,   // 1772..1773; TAGBANWA
                    0x1774,   // 1774..177F; UNKNOWN
                    0x1780,   // 1780..17DD; KHMER
                    0x17DE,   // 17DE..17DF; UNKNOWN
                    0x17E0,   // 17E0..17E9; KHMER
                    0x17EA,   // 17EA..17EF; UNKNOWN
                    0x17F0,   // 17F0..17F9; KHMER
                    0x17FA,   // 17FA..17FF; UNKNOWN
                    0x1800,   // 1800..1801; MONGOLIAN
                    0x1802,   // 1802..1803; COMMON
                    0x1804,   // 1804      ; MONGOLIAN
                    0x1805,   // 1805      ; COMMON
                    0x1806,   // 1806..1819; MONGOLIAN
                    0x181A,   // 181A..181F; UNKNOWN
                    0x1820,   // 1820..1878; MONGOLIAN
                    0x1879,   // 1879..187F; UNKNOWN
                    0x1880,   // 1880..18AA; MONGOLIAN
                    0x18AB,   // 18AB..18AF; UNKNOWN
                    0x18B0,   // 18B0..18F5; CANADIAN_ABORIGINAL
                    0x18F6,   // 18F6..18FF; UNKNOWN
                    0x1900,   // 1900..191E; LIMBU
                    0x191F,   // 191F      ; UNKNOWN
                    0x1920,   // 1920..192B; LIMBU
                    0x192C,   // 192C..192F; UNKNOWN
                    0x1930,   // 1930..193B; LIMBU
                    0x193C,   // 193C..193F; UNKNOWN
                    0x1940,   // 1940      ; LIMBU
                    0x1941,   // 1941..1943; UNKNOWN
                    0x1944,   // 1944..194F; LIMBU
                    0x1950,   // 1950..196D; TAI_LE
                    0x196E,   // 196E..196F; UNKNOWN
                    0x1970,   // 1970..1974; TAI_LE
                    0x1975,   // 1975..197F; UNKNOWN
                    0x1980,   // 1980..19AB; NEW_TAI_LUE
                    0x19AC,   // 19AC..19AF; UNKNOWN
                    0x19B0,   // 19B0..19C9; NEW_TAI_LUE
                    0x19CA,   // 19CA..19CF; UNKNOWN
                    0x19D0,   // 19D0..19DA; NEW_TAI_LUE
                    0x19DB,   // 19DB..19DD; UNKNOWN
                    0x19DE,   // 19DE..19DF; NEW_TAI_LUE
                    0x19E0,   // 19E0..19FF; KHMER
                    0x1A00,   // 1A00..1A1B; BUGINESE
                    0x1A1C,   // 1A1C..1A1D; UNKNOWN
                    0x1A1E,   // 1A1E..1A1F; BUGINESE
                    0x1A20,   // 1A20..1A5E; TAI_THAM
                    0x1A5F,   // 1A5F      ; UNKNOWN
                    0x1A60,   // 1A60..1A7C; TAI_THAM
                    0x1A7D,   // 1A7D..1A7E; UNKNOWN
                    0x1A7F,   // 1A7F..1A89; TAI_THAM
                    0x1A8A,   // 1A8A..1A8F; UNKNOWN
                    0x1A90,   // 1A90..1A99; TAI_THAM
                    0x1A9A,   // 1A9A..1A9F; UNKNOWN
                    0x1AA0,   // 1AA0..1AAD; TAI_THAM
                    0x1AAE,   // 1AAE..1AAF; UNKNOWN
                    0x1AB0,   // 1AB0..1ACE; INHERITED
                    0x1ACF,   // 1ACF..1AFF; UNKNOWN
                    0x1B00,   // 1B00..1B4C; BALINESE
                    0x1B4D,   // 1B4D..1B4F; UNKNOWN
                    0x1B50,   // 1B50..1B7E; BALINESE
                    0x1B7F,   // 1B7F      ; UNKNOWN
                    0x1B80,   // 1B80..1BBF; SUNDANESE
                    0x1BC0,   // 1BC0..1BF3; BATAK
                    0x1BF4,   // 1BF4..1BFB; UNKNOWN
                    0x1BFC,   // 1BFC..1BFF; BATAK
                    0x1C00,   // 1C00..1C37; LEPCHA
                    0x1C38,   // 1C38..1C3A; UNKNOWN
                    0x1C3B,   // 1C3B..1C49; LEPCHA
                    0x1C4A,   // 1C4A..1C4C; UNKNOWN
                    0x1C4D,   // 1C4D..1C4F; LEPCHA
                    0x1C50,   // 1C50..1C7F; OL_CHIKI
                    0x1C80,   // 1C80..1C88; CYRILLIC
                    0x1C89,   // 1C89..1C8F; UNKNOWN
                    0x1C90,   // 1C90..1CBA; GEORGIAN
                    0x1CBB,   // 1CBB..1CBC; UNKNOWN
                    0x1CBD,   // 1CBD..1CBF; GEORGIAN
                    0x1CC0,   // 1CC0..1CC7; SUNDANESE
                    0x1CC8,   // 1CC8..1CCF; UNKNOWN
                    0x1CD0,   // 1CD0..1CD2; INHERITED
                    0x1CD3,   // 1CD3      ; COMMON
                    0x1CD4,   // 1CD4..1CE0; INHERITED
                    0x1CE1,   // 1CE1      ; COMMON
                    0x1CE2,   // 1CE2..1CE8; INHERITED
                    0x1CE9,   // 1CE9..1CEC; COMMON
                    0x1CED,   // 1CED      ; INHERITED
                    0x1CEE,   // 1CEE..1CF3; COMMON
                    0x1CF4,   // 1CF4      ; INHERITED
                    0x1CF5,   // 1CF5..1CF7; COMMON
                    0x1CF8,   // 1CF8..1CF9; INHERITED
                    0x1CFA,   // 1CFA      ; COMMON
                    0x1CFB,   // 1CFB..1CFF; UNKNOWN
                    0x1D00,   // 1D00..1D25; LATIN
                    0x1D26,   // 1D26..1D2A; GREEK
                    0x1D2B,   // 1D2B      ; CYRILLIC
                    0x1D2C,   // 1D2C..1D5C; LATIN
                    0x1D5D,   // 1D5D..1D61; GREEK
                    0x1D62,   // 1D62..1D65; LATIN
                    0x1D66,   // 1D66..1D6A; GREEK
                    0x1D6B,   // 1D6B..1D77; LATIN
                    0x1D78,   // 1D78      ; CYRILLIC
                    0x1D79,   // 1D79..1DBE; LATIN
                    0x1DBF,   // 1DBF      ; GREEK
                    0x1DC0,   // 1DC0..1DFF; INHERITED
                    0x1E00,   // 1E00..1EFF; LATIN
                    0x1F00,   // 1F00..1F15; GREEK
                    0x1F16,   // 1F16..1F17; UNKNOWN
                    0x1F18,   // 1F18..1F1D; GREEK
                    0x1F1E,   // 1F1E..1F1F; UNKNOWN
                    0x1F20,   // 1F20..1F45; GREEK
                    0x1F46,   // 1F46..1F47; UNKNOWN
                    0x1F48,   // 1F48..1F4D; GREEK
                    0x1F4E,   // 1F4E..1F4F; UNKNOWN
                    0x1F50,   // 1F50..1F57; GREEK
                    0x1F58,   // 1F58      ; UNKNOWN
                    0x1F59,   // 1F59      ; GREEK
                    0x1F5A,   // 1F5A      ; UNKNOWN
                    0x1F5B,   // 1F5B      ; GREEK
                    0x1F5C,   // 1F5C      ; UNKNOWN
                    0x1F5D,   // 1F5D      ; GREEK
                    0x1F5E,   // 1F5E      ; UNKNOWN
                    0x1F5F,   // 1F5F..1F7D; GREEK
                    0x1F7E,   // 1F7E..1F7F; UNKNOWN
                    0x1F80,   // 1F80..1FB4; GREEK
                    0x1FB5,   // 1FB5      ; UNKNOWN
                    0x1FB6,   // 1FB6..1FC4; GREEK
                    0x1FC5,   // 1FC5      ; UNKNOWN
                    0x1FC6,   // 1FC6..1FD3; GREEK
                    0x1FD4,   // 1FD4..1FD5; UNKNOWN
                    0x1FD6,   // 1FD6..1FDB; GREEK
                    0x1FDC,   // 1FDC      ; UNKNOWN
                    0x1FDD,   // 1FDD..1FEF; GREEK
                    0x1FF0,   // 1FF0..1FF1; UNKNOWN
                    0x1FF2,   // 1FF2..1FF4; GREEK
                    0x1FF5,   // 1FF5      ; UNKNOWN
                    0x1FF6,   // 1FF6..1FFE; GREEK
                    0x1FFF,   // 1FFF      ; UNKNOWN
                    0x2000,   // 2000..200B; COMMON
                    0x200C,   // 200C..200D; INHERITED
                    0x200E,   // 200E..2064; COMMON
                    0x2065,   // 2065      ; UNKNOWN
                    0x2066,   // 2066..2070; COMMON
                    0x2071,   // 2071      ; LATIN
                    0x2072,   // 2072..2073; UNKNOWN
                    0x2074,   // 2074..207E; COMMON
                    0x207F,   // 207F      ; LATIN
                    0x2080,   // 2080..208E; COMMON
                    0x208F,   // 208F      ; UNKNOWN
                    0x2090,   // 2090..209C; LATIN
                    0x209D,   // 209D..209F; UNKNOWN
                    0x20A0,   // 20A0..20C0; COMMON
                    0x20C1,   // 20C1..20CF; UNKNOWN
                    0x20D0,   // 20D0..20F0; INHERITED
                    0x20F1,   // 20F1..20FF; UNKNOWN
                    0x2100,   // 2100..2125; COMMON
                    0x2126,   // 2126      ; GREEK
                    0x2127,   // 2127..2129; COMMON
                    0x212A,   // 212A..212B; LATIN
                    0x212C,   // 212C..2131; COMMON
                    0x2132,   // 2132      ; LATIN
                    0x2133,   // 2133..214D; COMMON
                    0x214E,   // 214E      ; LATIN
                    0x214F,   // 214F..215F; COMMON
                    0x2160,   // 2160..2188; LATIN
                    0x2189,   // 2189..218B; COMMON
                    0x218C,   // 218C..218F; UNKNOWN
                    0x2190,   // 2190..2426; COMMON
                    0x2427,   // 2427..243F; UNKNOWN
                    0x2440,   // 2440..244A; COMMON
                    0x244B,   // 244B..245F; UNKNOWN
                    0x2460,   // 2460..27FF; COMMON
                    0x2800,   // 2800..28FF; BRAILLE
                    0x2900,   // 2900..2B73; COMMON
                    0x2B74,   // 2B74..2B75; UNKNOWN
                    0x2B76,   // 2B76..2B95; COMMON
                    0x2B96,   // 2B96      ; UNKNOWN
                    0x2B97,   // 2B97..2BFF; COMMON
                    0x2C00,   // 2C00..2C5F; GLAGOLITIC
                    0x2C60,   // 2C60..2C7F; LATIN
                    0x2C80,   // 2C80..2CF3; COPTIC
                    0x2CF4,   // 2CF4..2CF8; UNKNOWN
                    0x2CF9,   // 2CF9..2CFF; COPTIC
                    0x2D00,   // 2D00..2D25; GEORGIAN
                    0x2D26,   // 2D26      ; UNKNOWN
                    0x2D27,   // 2D27      ; GEORGIAN
                    0x2D28,   // 2D28..2D2C; UNKNOWN
                    0x2D2D,   // 2D2D      ; GEORGIAN
                    0x2D2E,   // 2D2E..2D2F; UNKNOWN
                    0x2D30,   // 2D30..2D67; TIFINAGH
                    0x2D68,   // 2D68..2D6E; UNKNOWN
                    0x2D6F,   // 2D6F..2D70; TIFINAGH
                    0x2D71,   // 2D71..2D7E; UNKNOWN
                    0x2D7F,   // 2D7F      ; TIFINAGH
                    0x2D80,   // 2D80..2D96; ETHIOPIC
                    0x2D97,   // 2D97..2D9F; UNKNOWN
                    0x2DA0,   // 2DA0..2DA6; ETHIOPIC
                    0x2DA7,   // 2DA7      ; UNKNOWN
                    0x2DA8,   // 2DA8..2DAE; ETHIOPIC
                    0x2DAF,   // 2DAF      ; UNKNOWN
                    0x2DB0,   // 2DB0..2DB6; ETHIOPIC
                    0x2DB7,   // 2DB7      ; UNKNOWN
                    0x2DB8,   // 2DB8..2DBE; ETHIOPIC
                    0x2DBF,   // 2DBF      ; UNKNOWN
                    0x2DC0,   // 2DC0..2DC6; ETHIOPIC
                    0x2DC7,   // 2DC7      ; UNKNOWN
                    0x2DC8,   // 2DC8..2DCE; ETHIOPIC
                    0x2DCF,   // 2DCF      ; UNKNOWN
                    0x2DD0,   // 2DD0..2DD6; ETHIOPIC
                    0x2DD7,   // 2DD7      ; UNKNOWN
                    0x2DD8,   // 2DD8..2DDE; ETHIOPIC
                    0x2DDF,   // 2DDF      ; UNKNOWN
                    0x2DE0,   // 2DE0..2DFF; CYRILLIC
                    0x2E00,   // 2E00..2E5D; COMMON
                    0x2E5E,   // 2E5E..2E7F; UNKNOWN
                    0x2E80,   // 2E80..2E99; HAN
                    0x2E9A,   // 2E9A      ; UNKNOWN
                    0x2E9B,   // 2E9B..2EF3; HAN
                    0x2EF4,   // 2EF4..2EFF; UNKNOWN
                    0x2F00,   // 2F00..2FD5; HAN
                    0x2FD6,   // 2FD6..2FEF; UNKNOWN
                    0x2FF0,   // 2FF0..2FFB; COMMON
                    0x2FFC,   // 2FFC..2FFF; UNKNOWN
                    0x3000,   // 3000..3004; COMMON
                    0x3005,   // 3005      ; HAN
                    0x3006,   // 3006      ; COMMON
                    0x3007,   // 3007      ; HAN
                    0x3008,   // 3008..3020; COMMON
                    0x3021,   // 3021..3029; HAN
                    0x302A,   // 302A..302D; INHERITED
                    0x302E,   // 302E..302F; HANGUL
                    0x3030,   // 3030..3037; COMMON
                    0x3038,   // 3038..303B; HAN
                    0x303C,   // 303C..303F; COMMON
                    0x3040,   // 3040      ; UNKNOWN
                    0x3041,   // 3041..3096; HIRAGANA
                    0x3097,   // 3097..3098; UNKNOWN
                    0x3099,   // 3099..309A; INHERITED
                    0x309B,   // 309B..309C; COMMON
                    0x309D,   // 309D..309F; HIRAGANA
                    0x30A0,   // 30A0      ; COMMON
                    0x30A1,   // 30A1..30FA; KATAKANA
                    0x30FB,   // 30FB..30FC; COMMON
                    0x30FD,   // 30FD..30FF; KATAKANA
                    0x3100,   // 3100..3104; UNKNOWN
                    0x3105,   // 3105..312F; BOPOMOFO
                    0x3130,   // 3130      ; UNKNOWN
                    0x3131,   // 3131..318E; HANGUL
                    0x318F,   // 318F      ; UNKNOWN
                    0x3190,   // 3190..319F; COMMON
                    0x31A0,   // 31A0..31BF; BOPOMOFO
                    0x31C0,   // 31C0..31E3; COMMON
                    0x31E4,   // 31E4..31EF; UNKNOWN
                    0x31F0,   // 31F0..31FF; KATAKANA
                    0x3200,   // 3200..321E; HANGUL
                    0x321F,   // 321F      ; UNKNOWN
                    0x3220,   // 3220..325F; COMMON
                    0x3260,   // 3260..327E; HANGUL
                    0x327F,   // 327F..32CF; COMMON
                    0x32D0,   // 32D0..32FE; KATAKANA
                    0x32FF,   // 32FF      ; COMMON
                    0x3300,   // 3300..3357; KATAKANA
                    0x3358,   // 3358..33FF; COMMON
                    0x3400,   // 3400..4DBF; HAN
                    0x4DC0,   // 4DC0..4DFF; COMMON
                    0x4E00,   // 4E00..9FFF; HAN
                    0xA000,   // A000..A48C; YI
                    0xA48D,   // A48D..A48F; UNKNOWN
                    0xA490,   // A490..A4C6; YI
                    0xA4C7,   // A4C7..A4CF; UNKNOWN
                    0xA4D0,   // A4D0..A4FF; LISU
                    0xA500,   // A500..A62B; VAI
                    0xA62C,   // A62C..A63F; UNKNOWN
                    0xA640,   // A640..A69F; CYRILLIC
                    0xA6A0,   // A6A0..A6F7; BAMUM
                    0xA6F8,   // A6F8..A6FF; UNKNOWN
                    0xA700,   // A700..A721; COMMON
                    0xA722,   // A722..A787; LATIN
                    0xA788,   // A788..A78A; COMMON
                    0xA78B,   // A78B..A7CA; LATIN
                    0xA7CB,   // A7CB..A7CF; UNKNOWN
                    0xA7D0,   // A7D0..A7D1; LATIN
                    0xA7D2,   // A7D2      ; UNKNOWN
                    0xA7D3,   // A7D3      ; LATIN
                    0xA7D4,   // A7D4      ; UNKNOWN
                    0xA7D5,   // A7D5..A7D9; LATIN
                    0xA7DA,   // A7DA..A7F1; UNKNOWN
                    0xA7F2,   // A7F2..A7FF; LATIN
                    0xA800,   // A800..A82C; SYLOTI_NAGRI
                    0xA82D,   // A82D..A82F; UNKNOWN
                    0xA830,   // A830..A839; COMMON
                    0xA83A,   // A83A..A83F; UNKNOWN
                    0xA840,   // A840..A877; PHAGS_PA
                    0xA878,   // A878..A87F; UNKNOWN
                    0xA880,   // A880..A8C5; SAURASHTRA
                    0xA8C6,   // A8C6..A8CD; UNKNOWN
                    0xA8CE,   // A8CE..A8D9; SAURASHTRA
                    0xA8DA,   // A8DA..A8DF; UNKNOWN
                    0xA8E0,   // A8E0..A8FF; DEVANAGARI
                    0xA900,   // A900..A92D; KAYAH_LI
                    0xA92E,   // A92E      ; COMMON
                    0xA92F,   // A92F      ; KAYAH_LI
                    0xA930,   // A930..A953; REJANG
                    0xA954,   // A954..A95E; UNKNOWN
                    0xA95F,   // A95F      ; REJANG
                    0xA960,   // A960..A97C; HANGUL
                    0xA97D,   // A97D..A97F; UNKNOWN
                    0xA980,   // A980..A9CD; JAVANESE
                    0xA9CE,   // A9CE      ; UNKNOWN
                    0xA9CF,   // A9CF      ; COMMON
                    0xA9D0,   // A9D0..A9D9; JAVANESE
                    0xA9DA,   // A9DA..A9DD; UNKNOWN
                    0xA9DE,   // A9DE..A9DF; JAVANESE
                    0xA9E0,   // A9E0..A9FE; MYANMAR
                    0xA9FF,   // A9FF      ; UNKNOWN
                    0xAA00,   // AA00..AA36; CHAM
                    0xAA37,   // AA37..AA3F; UNKNOWN
                    0xAA40,   // AA40..AA4D; CHAM
                    0xAA4E,   // AA4E..AA4F; UNKNOWN
                    0xAA50,   // AA50..AA59; CHAM
                    0xAA5A,   // AA5A..AA5B; UNKNOWN
                    0xAA5C,   // AA5C..AA5F; CHAM
                    0xAA60,   // AA60..AA7F; MYANMAR
                    0xAA80,   // AA80..AAC2; TAI_VIET
                    0xAAC3,   // AAC3..AADA; UNKNOWN
                    0xAADB,   // AADB..AADF; TAI_VIET
                    0xAAE0,   // AAE0..AAF6; MEETEI_MAYEK
                    0xAAF7,   // AAF7..AB00; UNKNOWN
                    0xAB01,   // AB01..AB06; ETHIOPIC
                    0xAB07,   // AB07..AB08; UNKNOWN
                    0xAB09,   // AB09..AB0E; ETHIOPIC
                    0xAB0F,   // AB0F..AB10; UNKNOWN
                    0xAB11,   // AB11..AB16; ETHIOPIC
                    0xAB17,   // AB17..AB1F; UNKNOWN
                    0xAB20,   // AB20..AB26; ETHIOPIC
                    0xAB27,   // AB27      ; UNKNOWN
                    0xAB28,   // AB28..AB2E; ETHIOPIC
                    0xAB2F,   // AB2F      ; UNKNOWN
                    0xAB30,   // AB30..AB5A; LATIN
                    0xAB5B,   // AB5B      ; COMMON
                    0xAB5C,   // AB5C..AB64; LATIN
                    0xAB65,   // AB65      ; GREEK
                    0xAB66,   // AB66..AB69; LATIN
                    0xAB6A,   // AB6A..AB6B; COMMON
                    0xAB6C,   // AB6C..AB6F; UNKNOWN
                    0xAB70,   // AB70..ABBF; CHEROKEE
                    0xABC0,   // ABC0..ABED; MEETEI_MAYEK
                    0xABEE,   // ABEE..ABEF; UNKNOWN
                    0xABF0,   // ABF0..ABF9; MEETEI_MAYEK
                    0xABFA,   // ABFA..ABFF; UNKNOWN
                    0xAC00,   // AC00..D7A3; HANGUL
                    0xD7A4,   // D7A4..D7AF; UNKNOWN
                    0xD7B0,   // D7B0..D7C6; HANGUL
                    0xD7C7,   // D7C7..D7CA; UNKNOWN
                    0xD7CB,   // D7CB..D7FB; HANGUL
                    0xD7FC,   // D7FC..F8FF; UNKNOWN
                    0xF900,   // F900..FA6D; HAN
                    0xFA6E,   // FA6E..FA6F; UNKNOWN
                    0xFA70,   // FA70..FAD9; HAN
                    0xFADA,   // FADA..FAFF; UNKNOWN
                    0xFB00,   // FB00..FB06; LATIN
                    0xFB07,   // FB07..FB12; UNKNOWN
                    0xFB13,   // FB13..FB17; ARMENIAN
                    0xFB18,   // FB18..FB1C; UNKNOWN
                    0xFB1D,   // FB1D..FB36; HEBREW
                    0xFB37,   // FB37      ; UNKNOWN
                    0xFB38,   // FB38..FB3C; HEBREW
                    0xFB3D,   // FB3D      ; UNKNOWN
                    0xFB3E,   // FB3E      ; HEBREW
                    0xFB3F,   // FB3F      ; UNKNOWN
                    0xFB40,   // FB40..FB41; HEBREW
                    0xFB42,   // FB42      ; UNKNOWN
                    0xFB43,   // FB43..FB44; HEBREW
                    0xFB45,   // FB45      ; UNKNOWN
                    0xFB46,   // FB46..FB4F; HEBREW
                    0xFB50,   // FB50..FBC2; ARABIC
                    0xFBC3,   // FBC3..FBD2; UNKNOWN
                    0xFBD3,   // FBD3..FD3D; ARABIC
                    0xFD3E,   // FD3E..FD3F; COMMON
                    0xFD40,   // FD40..FD8F; ARABIC
                    0xFD90,   // FD90..FD91; UNKNOWN
                    0xFD92,   // FD92..FDC7; ARABIC
                    0xFDC8,   // FDC8..FDCE; UNKNOWN
                    0xFDCF,   // FDCF      ; ARABIC
                    0xFDD0,   // FDD0..FDEF; UNKNOWN
                    0xFDF0,   // FDF0..FDFF; ARABIC
                    0xFE00,   // FE00..FE0F; INHERITED
                    0xFE10,   // FE10..FE19; COMMON
                    0xFE1A,   // FE1A..FE1F; UNKNOWN
                    0xFE20,   // FE20..FE2D; INHERITED
                    0xFE2E,   // FE2E..FE2F; CYRILLIC
                    0xFE30,   // FE30..FE52; COMMON
                    0xFE53,   // FE53      ; UNKNOWN
                    0xFE54,   // FE54..FE66; COMMON
                    0xFE67,   // FE67      ; UNKNOWN
                    0xFE68,   // FE68..FE6B; COMMON
                    0xFE6C,   // FE6C..FE6F; UNKNOWN
                    0xFE70,   // FE70..FE74; ARABIC
                    0xFE75,   // FE75      ; UNKNOWN
                    0xFE76,   // FE76..FEFC; ARABIC
                    0xFEFD,   // FEFD..FEFE; UNKNOWN
                    0xFEFF,   // FEFF      ; COMMON
                    0xFF00,   // FF00      ; UNKNOWN
                    0xFF01,   // FF01..FF20; COMMON
                    0xFF21,   // FF21..FF3A; LATIN
                    0xFF3B,   // FF3B..FF40; COMMON
                    0xFF41,   // FF41..FF5A; LATIN
                    0xFF5B,   // FF5B..FF65; COMMON
                    0xFF66,   // FF66..FF6F; KATAKANA
                    0xFF70,   // FF70      ; COMMON
                    0xFF71,   // FF71..FF9D; KATAKANA
                    0xFF9E,   // FF9E..FF9F; COMMON
                    0xFFA0,   // FFA0..FFBE; HANGUL
                    0xFFBF,   // FFBF..FFC1; UNKNOWN
                    0xFFC2,   // FFC2..FFC7; HANGUL
                    0xFFC8,   // FFC8..FFC9; UNKNOWN
                    0xFFCA,   // FFCA..FFCF; HANGUL
                    0xFFD0,   // FFD0..FFD1; UNKNOWN
                    0xFFD2,   // FFD2..FFD7; HANGUL
                    0xFFD8,   // FFD8..FFD9; UNKNOWN
                    0xFFDA,   // FFDA..FFDC; HANGUL
                    0xFFDD,   // FFDD..FFDF; UNKNOWN
                    0xFFE0,   // FFE0..FFE6; COMMON
                    0xFFE7,   // FFE7      ; UNKNOWN
                    0xFFE8,   // FFE8..FFEE; COMMON
                    0xFFEF,   // FFEF..FFF8; UNKNOWN
                    0xFFF9,   // FFF9..FFFD; COMMON
                    0xFFFE,   // FFFE..FFFF; UNKNOWN
                    0x10000,  // 10000..1000B; LINEAR_B
                    0x1000C,  // 1000C       ; UNKNOWN
                    0x1000D,  // 1000D..10026; LINEAR_B
                    0x10027,  // 10027       ; UNKNOWN
                    0x10028,  // 10028..1003A; LINEAR_B
                    0x1003B,  // 1003B       ; UNKNOWN
                    0x1003C,  // 1003C..1003D; LINEAR_B
                    0x1003E,  // 1003E       ; UNKNOWN
                    0x1003F,  // 1003F..1004D; LINEAR_B
                    0x1004E,  // 1004E..1004F; UNKNOWN
                    0x10050,  // 10050..1005D; LINEAR_B
                    0x1005E,  // 1005E..1007F; UNKNOWN
                    0x10080,  // 10080..100FA; LINEAR_B
                    0x100FB,  // 100FB..100FF; UNKNOWN
                    0x10100,  // 10100..10102; COMMON
                    0x10103,  // 10103..10106; UNKNOWN
                    0x10107,  // 10107..10133; COMMON
                    0x10134,  // 10134..10136; UNKNOWN
                    0x10137,  // 10137..1013F; COMMON
                    0x10140,  // 10140..1018E; GREEK
                    0x1018F,  // 1018F       ; UNKNOWN
                    0x10190,  // 10190..1019C; COMMON
                    0x1019D,  // 1019D..1019F; UNKNOWN
                    0x101A0,  // 101A0       ; GREEK
                    0x101A1,  // 101A1..101CF; UNKNOWN
                    0x101D0,  // 101D0..101FC; COMMON
                    0x101FD,  // 101FD       ; INHERITED
                    0x101FE,  // 101FE..1027F; UNKNOWN
                    0x10280,  // 10280..1029C; LYCIAN
                    0x1029D,  // 1029D..1029F; UNKNOWN
                    0x102A0,  // 102A0..102D0; CARIAN
                    0x102D1,  // 102D1..102DF; UNKNOWN
                    0x102E0,  // 102E0       ; INHERITED
                    0x102E1,  // 102E1..102FB; COMMON
                    0x102FC,  // 102FC..102FF; UNKNOWN
                    0x10300,  // 10300..10323; OLD_ITALIC
                    0x10324,  // 10324..1032C; UNKNOWN
                    0x1032D,  // 1032D..1032F; OLD_ITALIC
                    0x10330,  // 10330..1034A; GOTHIC
                    0x1034B,  // 1034B..1034F; UNKNOWN
                    0x10350,  // 10350..1037A; OLD_PERMIC
                    0x1037B,  // 1037B..1037F; UNKNOWN
                    0x10380,  // 10380..1039D; UGARITIC
                    0x1039E,  // 1039E       ; UNKNOWN
                    0x1039F,  // 1039F       ; UGARITIC
                    0x103A0,  // 103A0..103C3; OLD_PERSIAN
                    0x103C4,  // 103C4..103C7; UNKNOWN
                    0x103C8,  // 103C8..103D5; OLD_PERSIAN
                    0x103D6,  // 103D6..103FF; UNKNOWN
                    0x10400,  // 10400..1044F; DESERET
                    0x10450,  // 10450..1047F; SHAVIAN
                    0x10480,  // 10480..1049D; OSMANYA
                    0x1049E,  // 1049E..1049F; UNKNOWN
                    0x104A0,  // 104A0..104A9; OSMANYA
                    0x104AA,  // 104AA..104AF; UNKNOWN
                    0x104B0,  // 104B0..104D3; OSAGE
                    0x104D4,  // 104D4..104D7; UNKNOWN
                    0x104D8,  // 104D8..104FB; OSAGE
                    0x104FC,  // 104FC..104FF; UNKNOWN
                    0x10500,  // 10500..10527; ELBASAN
                    0x10528,  // 10528..1052F; UNKNOWN
                    0x10530,  // 10530..10563; CAUCASIAN_ALBANIAN
                    0x10564,  // 10564..1056E; UNKNOWN
                    0x1056F,  // 1056F       ; CAUCASIAN_ALBANIAN
                    0x10570,  // 10570..1057A; VITHKUQI
                    0x1057B,  // 1057B       ; UNKNOWN
                    0x1057C,  // 1057C..1058A; VITHKUQI
                    0x1058B,  // 1058B       ; UNKNOWN
                    0x1058C,  // 1058C..10592; VITHKUQI
                    0x10593,  // 10593       ; UNKNOWN
                    0x10594,  // 10594..10595; VITHKUQI
                    0x10596,  // 10596       ; UNKNOWN
                    0x10597,  // 10597..105A1; VITHKUQI
                    0x105A2,  // 105A2       ; UNKNOWN
                    0x105A3,  // 105A3..105B1; VITHKUQI
                    0x105B2,  // 105B2       ; UNKNOWN
                    0x105B3,  // 105B3..105B9; VITHKUQI
                    0x105BA,  // 105BA       ; UNKNOWN
                    0x105BB,  // 105BB..105BC; VITHKUQI
                    0x105BD,  // 105BD..105FF; UNKNOWN
                    0x10600,  // 10600..10736; LINEAR_A
                    0x10737,  // 10737..1073F; UNKNOWN
                    0x10740,  // 10740..10755; LINEAR_A
                    0x10756,  // 10756..1075F; UNKNOWN
                    0x10760,  // 10760..10767; LINEAR_A
                    0x10768,  // 10768..1077F; UNKNOWN
                    0x10780,  // 10780..10785; LATIN
                    0x10786,  // 10786       ; UNKNOWN
                    0x10787,  // 10787..107B0; LATIN
                    0x107B1,  // 107B1       ; UNKNOWN
                    0x107B2,  // 107B2..107BA; LATIN
                    0x107BB,  // 107BB..107FF; UNKNOWN
                    0x10800,  // 10800..10805; CYPRIOT
                    0x10806,  // 10806..10807; UNKNOWN
                    0x10808,  // 10808       ; CYPRIOT
                    0x10809,  // 10809       ; UNKNOWN
                    0x1080A,  // 1080A..10835; CYPRIOT
                    0x10836,  // 10836       ; UNKNOWN
                    0x10837,  // 10837..10838; CYPRIOT
                    0x10839,  // 10839..1083B; UNKNOWN
                    0x1083C,  // 1083C       ; CYPRIOT
                    0x1083D,  // 1083D..1083E; UNKNOWN
                    0x1083F,  // 1083F       ; CYPRIOT
                    0x10840,  // 10840..10855; IMPERIAL_ARAMAIC
                    0x10856,  // 10856       ; UNKNOWN
                    0x10857,  // 10857..1085F; IMPERIAL_ARAMAIC
                    0x10860,  // 10860..1087F; PALMYRENE
                    0x10880,  // 10880..1089E; NABATAEAN
                    0x1089F,  // 1089F..108A6; UNKNOWN
                    0x108A7,  // 108A7..108AF; NABATAEAN
                    0x108B0,  // 108B0..108DF; UNKNOWN
                    0x108E0,  // 108E0..108F2; HATRAN
                    0x108F3,  // 108F3       ; UNKNOWN
                    0x108F4,  // 108F4..108F5; HATRAN
                    0x108F6,  // 108F6..108FA; UNKNOWN
                    0x108FB,  // 108FB..108FF; HATRAN
                    0x10900,  // 10900..1091B; PHOENICIAN
                    0x1091C,  // 1091C..1091E; UNKNOWN
                    0x1091F,  // 1091F       ; PHOENICIAN
                    0x10920,  // 10920..10939; LYDIAN
                    0x1093A,  // 1093A..1093E; UNKNOWN
                    0x1093F,  // 1093F       ; LYDIAN
                    0x10940,  // 10940..1097F; UNKNOWN
                    0x10980,  // 10980..1099F; MEROITIC_HIEROGLYPHS
                    0x109A0,  // 109A0..109B7; MEROITIC_CURSIVE
                    0x109B8,  // 109B8..109BB; UNKNOWN
                    0x109BC,  // 109BC..109CF; MEROITIC_CURSIVE
                    0x109D0,  // 109D0..109D1; UNKNOWN
                    0x109D2,  // 109D2..109FF; MEROITIC_CURSIVE
                    0x10A00,  // 10A00..10A03; KHAROSHTHI
                    0x10A04,  // 10A04       ; UNKNOWN
                    0x10A05,  // 10A05..10A06; KHAROSHTHI
                    0x10A07,  // 10A07..10A0B; UNKNOWN
                    0x10A0C,  // 10A0C..10A13; KHAROSHTHI
                    0x10A14,  // 10A14       ; UNKNOWN
                    0x10A15,  // 10A15..10A17; KHAROSHTHI
                    0x10A18,  // 10A18       ; UNKNOWN
                    0x10A19,  // 10A19..10A35; KHAROSHTHI
                    0x10A36,  // 10A36..10A37; UNKNOWN
                    0x10A38,  // 10A38..10A3A; KHAROSHTHI
                    0x10A3B,  // 10A3B..10A3E; UNKNOWN
                    0x10A3F,  // 10A3F..10A48; KHAROSHTHI
                    0x10A49,  // 10A49..10A4F; UNKNOWN
                    0x10A50,  // 10A50..10A58; KHAROSHTHI
                    0x10A59,  // 10A59..10A5F; UNKNOWN
                    0x10A60,  // 10A60..10A7F; OLD_SOUTH_ARABIAN
                    0x10A80,  // 10A80..10A9F; OLD_NORTH_ARABIAN
                    0x10AA0,  // 10AA0..10ABF; UNKNOWN
                    0x10AC0,  // 10AC0..10AE6; MANICHAEAN
                    0x10AE7,  // 10AE7..10AEA; UNKNOWN
                    0x10AEB,  // 10AEB..10AF6; MANICHAEAN
                    0x10AF7,  // 10AF7..10AFF; UNKNOWN
                    0x10B00,  // 10B00..10B35; AVESTAN
                    0x10B36,  // 10B36..10B38; UNKNOWN
                    0x10B39,  // 10B39..10B3F; AVESTAN
                    0x10B40,  // 10B40..10B55; INSCRIPTIONAL_PARTHIAN
                    0x10B56,  // 10B56..10B57; UNKNOWN
                    0x10B58,  // 10B58..10B5F; INSCRIPTIONAL_PARTHIAN
                    0x10B60,  // 10B60..10B72; INSCRIPTIONAL_PAHLAVI
                    0x10B73,  // 10B73..10B77; UNKNOWN
                    0x10B78,  // 10B78..10B7F; INSCRIPTIONAL_PAHLAVI
                    0x10B80,  // 10B80..10B91; PSALTER_PAHLAVI
                    0x10B92,  // 10B92..10B98; UNKNOWN
                    0x10B99,  // 10B99..10B9C; PSALTER_PAHLAVI
                    0x10B9D,  // 10B9D..10BA8; UNKNOWN
                    0x10BA9,  // 10BA9..10BAF; PSALTER_PAHLAVI
                    0x10BB0,  // 10BB0..10BFF; UNKNOWN
                    0x10C00,  // 10C00..10C48; OLD_TURKIC
                    0x10C49,  // 10C49..10C7F; UNKNOWN
                    0x10C80,  // 10C80..10CB2; OLD_HUNGARIAN
                    0x10CB3,  // 10CB3..10CBF; UNKNOWN
                    0x10CC0,  // 10CC0..10CF2; OLD_HUNGARIAN
                    0x10CF3,  // 10CF3..10CF9; UNKNOWN
                    0x10CFA,  // 10CFA..10CFF; OLD_HUNGARIAN
                    0x10D00,  // 10D00..10D27; HANIFI_ROHINGYA
                    0x10D28,  // 10D28..10D2F; UNKNOWN
                    0x10D30,  // 10D30..10D39; HANIFI_ROHINGYA
                    0x10D3A,  // 10D3A..10E5F; UNKNOWN
                    0x10E60,  // 10E60..10E7E; ARABIC
                    0x10E7F,  // 10E7F       ; UNKNOWN
                    0x10E80,  // 10E80..10EA9; YEZIDI
                    0x10EAA,  // 10EAA       ; UNKNOWN
                    0x10EAB,  // 10EAB..10EAD; YEZIDI
                    0x10EAE,  // 10EAE..10EAF; UNKNOWN
                    0x10EB0,  // 10EB0..10EB1; YEZIDI
                    0x10EB2,  // 10EB2..10EFC; UNKNOWN
                    0x10EFD,  // 10EFD..10EFF; ARABIC
                    0x10F00,  // 10F00..10F27; OLD_SOGDIAN
                    0x10F28,  // 10F28..10F2F; UNKNOWN
                    0x10F30,  // 10F30..10F59; SOGDIAN
                    0x10F5A,  // 10F5A..10F6F; UNKNOWN
                    0x10F70,  // 10F70..10F89; OLD_UYGHUR
                    0x10F8A,  // 10F8A..10FAF; UNKNOWN
                    0x10FB0,  // 10FB0..10FCB; CHORASMIAN
                    0x10FCC,  // 10FCC..10FDF; UNKNOWN
                    0x10FE0,  // 10FE0..10FF6; ELYMAIC
                    0x10FF7,  // 10FF7..10FFF; UNKNOWN
                    0x11000,  // 11000..1104D; BRAHMI
                    0x1104E,  // 1104E..11051; UNKNOWN
                    0x11052,  // 11052..11075; BRAHMI
                    0x11076,  // 11076..1107E; UNKNOWN
                    0x1107F,  // 1107F       ; BRAHMI
                    0x11080,  // 11080..110C2; KAITHI
                    0x110C3,  // 110C3..110CC; UNKNOWN
                    0x110CD,  // 110CD       ; KAITHI
                    0x110CE,  // 110CE..110CF; UNKNOWN
                    0x110D0,  // 110D0..110E8; SORA_SOMPENG
                    0x110E9,  // 110E9..110EF; UNKNOWN
                    0x110F0,  // 110F0..110F9; SORA_SOMPENG
                    0x110FA,  // 110FA..110FF; UNKNOWN
                    0x11100,  // 11100..11134; CHAKMA
                    0x11135,  // 11135       ; UNKNOWN
                    0x11136,  // 11136..11147; CHAKMA
                    0x11148,  // 11148..1114F; UNKNOWN
                    0x11150,  // 11150..11176; MAHAJANI
                    0x11177,  // 11177..1117F; UNKNOWN
                    0x11180,  // 11180..111DF; SHARADA
                    0x111E0,  // 111E0       ; UNKNOWN
                    0x111E1,  // 111E1..111F4; SINHALA
                    0x111F5,  // 111F5..111FF; UNKNOWN
                    0x11200,  // 11200..11211; KHOJKI
                    0x11212,  // 11212       ; UNKNOWN
                    0x11213,  // 11213..11241; KHOJKI
                    0x11242,  // 11242..1127F; UNKNOWN
                    0x11280,  // 11280..11286; MULTANI
                    0x11287,  // 11287       ; UNKNOWN
                    0x11288,  // 11288       ; MULTANI
                    0x11289,  // 11289       ; UNKNOWN
                    0x1128A,  // 1128A..1128D; MULTANI
                    0x1128E,  // 1128E       ; UNKNOWN
                    0x1128F,  // 1128F..1129D; MULTANI
                    0x1129E,  // 1129E       ; UNKNOWN
                    0x1129F,  // 1129F..112A9; MULTANI
                    0x112AA,  // 112AA..112AF; UNKNOWN
                    0x112B0,  // 112B0..112EA; KHUDAWADI
                    0x112EB,  // 112EB..112EF; UNKNOWN
                    0x112F0,  // 112F0..112F9; KHUDAWADI
                    0x112FA,  // 112FA..112FF; UNKNOWN
                    0x11300,  // 11300..11303; GRANTHA
                    0x11304,  // 11304       ; UNKNOWN
                    0x11305,  // 11305..1130C; GRANTHA
                    0x1130D,  // 1130D..1130E; UNKNOWN
                    0x1130F,  // 1130F..11310; GRANTHA
                    0x11311,  // 11311..11312; UNKNOWN
                    0x11313,  // 11313..11328; GRANTHA
                    0x11329,  // 11329       ; UNKNOWN
                    0x1132A,  // 1132A..11330; GRANTHA
                    0x11331,  // 11331       ; UNKNOWN
                    0x11332,  // 11332..11333; GRANTHA
                    0x11334,  // 11334       ; UNKNOWN
                    0x11335,  // 11335..11339; GRANTHA
                    0x1133A,  // 1133A       ; UNKNOWN
                    0x1133B,  // 1133B       ; INHERITED
                    0x1133C,  // 1133C..11344; GRANTHA
                    0x11345,  // 11345..11346; UNKNOWN
                    0x11347,  // 11347..11348; GRANTHA
                    0x11349,  // 11349..1134A; UNKNOWN
                    0x1134B,  // 1134B..1134D; GRANTHA
                    0x1134E,  // 1134E..1134F; UNKNOWN
                    0x11350,  // 11350       ; GRANTHA
                    0x11351,  // 11351..11356; UNKNOWN
                    0x11357,  // 11357       ; GRANTHA
                    0x11358,  // 11358..1135C; UNKNOWN
                    0x1135D,  // 1135D..11363; GRANTHA
                    0x11364,  // 11364..11365; UNKNOWN
                    0x11366,  // 11366..1136C; GRANTHA
                    0x1136D,  // 1136D..1136F; UNKNOWN
                    0x11370,  // 11370..11374; GRANTHA
                    0x11375,  // 11375..113FF; UNKNOWN
                    0x11400,  // 11400..1145B; NEWA
                    0x1145C,  // 1145C       ; UNKNOWN
                    0x1145D,  // 1145D..11461; NEWA
                    0x11462,  // 11462..1147F; UNKNOWN
                    0x11480,  // 11480..114C7; TIRHUTA
                    0x114C8,  // 114C8..114CF; UNKNOWN
                    0x114D0,  // 114D0..114D9; TIRHUTA
                    0x114DA,  // 114DA..1157F; UNKNOWN
                    0x11580,  // 11580..115B5; SIDDHAM
                    0x115B6,  // 115B6..115B7; UNKNOWN
                    0x115B8,  // 115B8..115DD; SIDDHAM
                    0x115DE,  // 115DE..115FF; UNKNOWN
                    0x11600,  // 11600..11644; MODI
                    0x11645,  // 11645..1164F; UNKNOWN
                    0x11650,  // 11650..11659; MODI
                    0x1165A,  // 1165A..1165F; UNKNOWN
                    0x11660,  // 11660..1166C; MONGOLIAN
                    0x1166D,  // 1166D..1167F; UNKNOWN
                    0x11680,  // 11680..116B9; TAKRI
                    0x116BA,  // 116BA..116BF; UNKNOWN
                    0x116C0,  // 116C0..116C9; TAKRI
                    0x116CA,  // 116CA..116FF; UNKNOWN
                    0x11700,  // 11700..1171A; AHOM
                    0x1171B,  // 1171B..1171C; UNKNOWN
                    0x1171D,  // 1171D..1172B; AHOM
                    0x1172C,  // 1172C..1172F; UNKNOWN
                    0x11730,  // 11730..11746; AHOM
                    0x11747,  // 11747..117FF; UNKNOWN
                    0x11800,  // 11800..1183B; DOGRA
                    0x1183C,  // 1183C..1189F; UNKNOWN
                    0x118A0,  // 118A0..118F2; WARANG_CITI
                    0x118F3,  // 118F3..118FE; UNKNOWN
                    0x118FF,  // 118FF       ; WARANG_CITI
                    0x11900,  // 11900..11906; DIVES_AKURU
                    0x11907,  // 11907..11908; UNKNOWN
                    0x11909,  // 11909       ; DIVES_AKURU
                    0x1190A,  // 1190A..1190B; UNKNOWN
                    0x1190C,  // 1190C..11913; DIVES_AKURU
                    0x11914,  // 11914       ; UNKNOWN
                    0x11915,  // 11915..11916; DIVES_AKURU
                    0x11917,  // 11917       ; UNKNOWN
                    0x11918,  // 11918..11935; DIVES_AKURU
                    0x11936,  // 11936       ; UNKNOWN
                    0x11937,  // 11937..11938; DIVES_AKURU
                    0x11939,  // 11939..1193A; UNKNOWN
                    0x1193B,  // 1193B..11946; DIVES_AKURU
                    0x11947,  // 11947..1194F; UNKNOWN
                    0x11950,  // 11950..11959; DIVES_AKURU
                    0x1195A,  // 1195A..1199F; UNKNOWN
                    0x119A0,  // 119A0..119A7; NANDINAGARI
                    0x119A8,  // 119A8..119A9; UNKNOWN
                    0x119AA,  // 119AA..119D7; NANDINAGARI
                    0x119D8,  // 119D8..119D9; UNKNOWN
                    0x119DA,  // 119DA..119E4; NANDINAGARI
                    0x119E5,  // 119E5..119FF; UNKNOWN
                    0x11A00,  // 11A00..11A47; ZANABAZAR_SQUARE
                    0x11A48,  // 11A48..11A4F; UNKNOWN
                    0x11A50,  // 11A50..11AA2; SOYOMBO
                    0x11AA3,  // 11AA3..11AAF; UNKNOWN
                    0x11AB0,  // 11AB0..11ABF; CANADIAN_ABORIGINAL
                    0x11AC0,  // 11AC0..11AF8; PAU_CIN_HAU
                    0x11AF9,  // 11AF9..11AFF; UNKNOWN
                    0x11B00,  // 11B00..11B09; DEVANAGARI
                    0x11B0A,  // 11B0A..11BFF; UNKNOWN
                    0x11C00,  // 11C00..11C08; BHAIKSUKI
                    0x11C09,  // 11C09       ; UNKNOWN
                    0x11C0A,  // 11C0A..11C36; BHAIKSUKI
                    0x11C37,  // 11C37       ; UNKNOWN
                    0x11C38,  // 11C38..11C45; BHAIKSUKI
                    0x11C46,  // 11C46..11C4F; UNKNOWN
                    0x11C50,  // 11C50..11C6C; BHAIKSUKI
                    0x11C6D,  // 11C6D..11C6F; UNKNOWN
                    0x11C70,  // 11C70..11C8F; MARCHEN
                    0x11C90,  // 11C90..11C91; UNKNOWN
                    0x11C92,  // 11C92..11CA7; MARCHEN
                    0x11CA8,  // 11CA8       ; UNKNOWN
                    0x11CA9,  // 11CA9..11CB6; MARCHEN
                    0x11CB7,  // 11CB7..11CFF; UNKNOWN
                    0x11D00,  // 11D00..11D06; MASARAM_GONDI
                    0x11D07,  // 11D07       ; UNKNOWN
                    0x11D08,  // 11D08..11D09; MASARAM_GONDI
                    0x11D0A,  // 11D0A       ; UNKNOWN
                    0x11D0B,  // 11D0B..11D36; MASARAM_GONDI
                    0x11D37,  // 11D37..11D39; UNKNOWN
                    0x11D3A,  // 11D3A       ; MASARAM_GONDI
                    0x11D3B,  // 11D3B       ; UNKNOWN
                    0x11D3C,  // 11D3C..11D3D; MASARAM_GONDI
                    0x11D3E,  // 11D3E       ; UNKNOWN
                    0x11D3F,  // 11D3F..11D47; MASARAM_GONDI
                    0x11D48,  // 11D48..11D4F; UNKNOWN
                    0x11D50,  // 11D50..11D59; MASARAM_GONDI
                    0x11D5A,  // 11D5A..11D5F; UNKNOWN
                    0x11D60,  // 11D60..11D65; GUNJALA_GONDI
                    0x11D66,  // 11D66       ; UNKNOWN
                    0x11D67,  // 11D67..11D68; GUNJALA_GONDI
                    0x11D69,  // 11D69       ; UNKNOWN
                    0x11D6A,  // 11D6A..11D8E; GUNJALA_GONDI
                    0x11D8F,  // 11D8F       ; UNKNOWN
                    0x11D90,  // 11D90..11D91; GUNJALA_GONDI
                    0x11D92,  // 11D92       ; UNKNOWN
                    0x11D93,  // 11D93..11D98; GUNJALA_GONDI
                    0x11D99,  // 11D99..11D9F; UNKNOWN
                    0x11DA0,  // 11DA0..11DA9; GUNJALA_GONDI
                    0x11DAA,  // 11DAA..11EDF; UNKNOWN
                    0x11EE0,  // 11EE0..11EF8; MAKASAR
                    0x11EF9,  // 11EF9..11EFF; UNKNOWN
                    0x11F00,  // 11F00..11F10; KAWI
                    0x11F11,  // 11F11       ; UNKNOWN
                    0x11F12,  // 11F12..11F3A; KAWI
                    0x11F3B,  // 11F3B..11F3D; UNKNOWN
                    0x11F3E,  // 11F3E..11F59; KAWI
                    0x11F5A,  // 11F5A..11FAF; UNKNOWN
                    0x11FB0,  // 11FB0       ; LISU
                    0x11FB1,  // 11FB1..11FBF; UNKNOWN
                    0x11FC0,  // 11FC0..11FF1; TAMIL
                    0x11FF2,  // 11FF2..11FFE; UNKNOWN
                    0x11FFF,  // 11FFF       ; TAMIL
                    0x12000,  // 12000..12399; CUNEIFORM
                    0x1239A,  // 1239A..123FF; UNKNOWN
                    0x12400,  // 12400..1246E; CUNEIFORM
                    0x1246F,  // 1246F       ; UNKNOWN
                    0x12470,  // 12470..12474; CUNEIFORM
                    0x12475,  // 12475..1247F; UNKNOWN
                    0x12480,  // 12480..12543; CUNEIFORM
                    0x12544,  // 12544..12F8F; UNKNOWN
                    0x12F90,  // 12F90..12FF2; CYPRO_MINOAN
                    0x12FF3,  // 12FF3..12FFF; UNKNOWN
                    0x13000,  // 13000..13455; EGYPTIAN_HIEROGLYPHS
                    0x13456,  // 13456..143FF; UNKNOWN
                    0x14400,  // 14400..14646; ANATOLIAN_HIEROGLYPHS
                    0x14647,  // 14647..167FF; UNKNOWN
                    0x16800,  // 16800..16A38; BAMUM
                    0x16A39,  // 16A39..16A3F; UNKNOWN
                    0x16A40,  // 16A40..16A5E; MRO
                    0x16A5F,  // 16A5F       ; UNKNOWN
                    0x16A60,  // 16A60..16A69; MRO
                    0x16A6A,  // 16A6A..16A6D; UNKNOWN
                    0x16A6E,  // 16A6E..16A6F; MRO
                    0x16A70,  // 16A70..16ABE; TANGSA
                    0x16ABF,  // 16ABF       ; UNKNOWN
                    0x16AC0,  // 16AC0..16AC9; TANGSA
                    0x16ACA,  // 16ACA..16ACF; UNKNOWN
                    0x16AD0,  // 16AD0..16AED; BASSA_VAH
                    0x16AEE,  // 16AEE..16AEF; UNKNOWN
                    0x16AF0,  // 16AF0..16AF5; BASSA_VAH
                    0x16AF6,  // 16AF6..16AFF; UNKNOWN
                    0x16B00,  // 16B00..16B45; PAHAWH_HMONG
                    0x16B46,  // 16B46..16B4F; UNKNOWN
                    0x16B50,  // 16B50..16B59; PAHAWH_HMONG
                    0x16B5A,  // 16B5A       ; UNKNOWN
                    0x16B5B,  // 16B5B..16B61; PAHAWH_HMONG
                    0x16B62,  // 16B62       ; UNKNOWN
                    0x16B63,  // 16B63..16B77; PAHAWH_HMONG
                    0x16B78,  // 16B78..16B7C; UNKNOWN
                    0x16B7D,  // 16B7D..16B8F; PAHAWH_HMONG
                    0x16B90,  // 16B90..16E3F; UNKNOWN
                    0x16E40,  // 16E40..16E9A; MEDEFAIDRIN
                    0x16E9B,  // 16E9B..16EFF; UNKNOWN
                    0x16F00,  // 16F00..16F4A; MIAO
                    0x16F4B,  // 16F4B..16F4E; UNKNOWN
                    0x16F4F,  // 16F4F..16F87; MIAO
                    0x16F88,  // 16F88..16F8E; UNKNOWN
                    0x16F8F,  // 16F8F..16F9F; MIAO
                    0x16FA0,  // 16FA0..16FDF; UNKNOWN
                    0x16FE0,  // 16FE0       ; TANGUT
                    0x16FE1,  // 16FE1       ; NUSHU
                    0x16FE2,  // 16FE2..16FE3; HAN
                    0x16FE4,  // 16FE4       ; KHITAN_SMALL_SCRIPT
                    0x16FE5,  // 16FE5..16FEF; UNKNOWN
                    0x16FF0,  // 16FF0..16FF1; HAN
                    0x16FF2,  // 16FF2..16FFF; UNKNOWN
                    0x17000,  // 17000..187F7; TANGUT
                    0x187F8,  // 187F8..187FF; UNKNOWN
                    0x18800,  // 18800..18AFF; TANGUT
                    0x18B00,  // 18B00..18CD5; KHITAN_SMALL_SCRIPT
                    0x18CD6,  // 18CD6..18CFF; UNKNOWN
                    0x18D00,  // 18D00..18D08; TANGUT
                    0x18D09,  // 18D09..1AFEF; UNKNOWN
                    0x1AFF0,  // 1AFF0..1AFF3; KATAKANA
                    0x1AFF4,  // 1AFF4       ; UNKNOWN
                    0x1AFF5,  // 1AFF5..1AFFB; KATAKANA
                    0x1AFFC,  // 1AFFC       ; UNKNOWN
                    0x1AFFD,  // 1AFFD..1AFFE; KATAKANA
                    0x1AFFF,  // 1AFFF       ; UNKNOWN
                    0x1B000,  // 1B000       ; KATAKANA
                    0x1B001,  // 1B001..1B11F; HIRAGANA
                    0x1B120,  // 1B120..1B122; KATAKANA
                    0x1B123,  // 1B123..1B131; UNKNOWN
                    0x1B132,  // 1B132       ; HIRAGANA
                    0x1B133,  // 1B133..1B14F; UNKNOWN
                    0x1B150,  // 1B150..1B152; HIRAGANA
                    0x1B153,  // 1B153..1B154; UNKNOWN
                    0x1B155,  // 1B155       ; KATAKANA
                    0x1B156,  // 1B156..1B163; UNKNOWN
                    0x1B164,  // 1B164..1B167; KATAKANA
                    0x1B168,  // 1B168..1B16F; UNKNOWN
                    0x1B170,  // 1B170..1B2FB; NUSHU
                    0x1B2FC,  // 1B2FC..1BBFF; UNKNOWN
                    0x1BC00,  // 1BC00..1BC6A; DUPLOYAN
                    0x1BC6B,  // 1BC6B..1BC6F; UNKNOWN
                    0x1BC70,  // 1BC70..1BC7C; DUPLOYAN
                    0x1BC7D,  // 1BC7D..1BC7F; UNKNOWN
                    0x1BC80,  // 1BC80..1BC88; DUPLOYAN
                    0x1BC89,  // 1BC89..1BC8F; UNKNOWN
                    0x1BC90,  // 1BC90..1BC99; DUPLOYAN
                    0x1BC9A,  // 1BC9A..1BC9B; UNKNOWN
                    0x1BC9C,  // 1BC9C..1BC9F; DUPLOYAN
                    0x1BCA0,  // 1BCA0..1BCA3; COMMON
                    0x1BCA4,  // 1BCA4..1CEFF; UNKNOWN
                    0x1CF00,  // 1CF00..1CF2D; INHERITED
                    0x1CF2E,  // 1CF2E..1CF2F; UNKNOWN
                    0x1CF30,  // 1CF30..1CF46; INHERITED
                    0x1CF47,  // 1CF47..1CF4F; UNKNOWN
                    0x1CF50,  // 1CF50..1CFC3; COMMON
                    0x1CFC4,  // 1CFC4..1CFFF; UNKNOWN
                    0x1D000,  // 1D000..1D0F5; COMMON
                    0x1D0F6,  // 1D0F6..1D0FF; UNKNOWN
                    0x1D100,  // 1D100..1D126; COMMON
                    0x1D127,  // 1D127..1D128; UNKNOWN
                    0x1D129,  // 1D129..1D166; COMMON
                    0x1D167,  // 1D167..1D169; INHERITED
                    0x1D16A,  // 1D16A..1D17A; COMMON
                    0x1D17B,  // 1D17B..1D182; INHERITED
                    0x1D183,  // 1D183..1D184; COMMON
                    0x1D185,  // 1D185..1D18B; INHERITED
                    0x1D18C,  // 1D18C..1D1A9; COMMON
                    0x1D1AA,  // 1D1AA..1D1AD; INHERITED
                    0x1D1AE,  // 1D1AE..1D1EA; COMMON
                    0x1D1EB,  // 1D1EB..1D1FF; UNKNOWN
                    0x1D200,  // 1D200..1D245; GREEK
                    0x1D246,  // 1D246..1D2BF; UNKNOWN
                    0x1D2C0,  // 1D2C0..1D2D3; COMMON
                    0x1D2D4,  // 1D2D4..1D2DF; UNKNOWN
                    0x1D2E0,  // 1D2E0..1D2F3; COMMON
                    0x1D2F4,  // 1D2F4..1D2FF; UNKNOWN
                    0x1D300,  // 1D300..1D356; COMMON
                    0x1D357,  // 1D357..1D35F; UNKNOWN
                    0x1D360,  // 1D360..1D378; COMMON
                    0x1D379,  // 1D379..1D3FF; UNKNOWN
                    0x1D400,  // 1D400..1D454; COMMON
                    0x1D455,  // 1D455       ; UNKNOWN
                    0x1D456,  // 1D456..1D49C; COMMON
                    0x1D49D,  // 1D49D       ; UNKNOWN
                    0x1D49E,  // 1D49E..1D49F; COMMON
                    0x1D4A0,  // 1D4A0..1D4A1; UNKNOWN
                    0x1D4A2,  // 1D4A2       ; COMMON
                    0x1D4A3,  // 1D4A3..1D4A4; UNKNOWN
                    0x1D4A5,  // 1D4A5..1D4A6; COMMON
                    0x1D4A7,  // 1D4A7..1D4A8; UNKNOWN
                    0x1D4A9,  // 1D4A9..1D4AC; COMMON
                    0x1D4AD,  // 1D4AD       ; UNKNOWN
                    0x1D4AE,  // 1D4AE..1D4B9; COMMON
                    0x1D4BA,  // 1D4BA       ; UNKNOWN
                    0x1D4BB,  // 1D4BB       ; COMMON
                    0x1D4BC,  // 1D4BC       ; UNKNOWN
                    0x1D4BD,  // 1D4BD..1D4C3; COMMON
                    0x1D4C4,  // 1D4C4       ; UNKNOWN
                    0x1D4C5,  // 1D4C5..1D505; COMMON
                    0x1D506,  // 1D506       ; UNKNOWN
                    0x1D507,  // 1D507..1D50A; COMMON
                    0x1D50B,  // 1D50B..1D50C; UNKNOWN
                    0x1D50D,  // 1D50D..1D514; COMMON
                    0x1D515,  // 1D515       ; UNKNOWN
                    0x1D516,  // 1D516..1D51C; COMMON
                    0x1D51D,  // 1D51D       ; UNKNOWN
                    0x1D51E,  // 1D51E..1D539; COMMON
                    0x1D53A,  // 1D53A       ; UNKNOWN
                    0x1D53B,  // 1D53B..1D53E; COMMON
                    0x1D53F,  // 1D53F       ; UNKNOWN
                    0x1D540,  // 1D540..1D544; COMMON
                    0x1D545,  // 1D545       ; UNKNOWN
                    0x1D546,  // 1D546       ; COMMON
                    0x1D547,  // 1D547..1D549; UNKNOWN
                    0x1D54A,  // 1D54A..1D550; COMMON
                    0x1D551,  // 1D551       ; UNKNOWN
                    0x1D552,  // 1D552..1D6A5; COMMON
                    0x1D6A6,  // 1D6A6..1D6A7; UNKNOWN
                    0x1D6A8,  // 1D6A8..1D7CB; COMMON
                    0x1D7CC,  // 1D7CC..1D7CD; UNKNOWN
                    0x1D7CE,  // 1D7CE..1D7FF; COMMON
                    0x1D800,  // 1D800..1DA8B; SIGNWRITING
                    0x1DA8C,  // 1DA8C..1DA9A; UNKNOWN
                    0x1DA9B,  // 1DA9B..1DA9F; SIGNWRITING
                    0x1DAA0,  // 1DAA0       ; UNKNOWN
                    0x1DAA1,  // 1DAA1..1DAAF; SIGNWRITING
                    0x1DAB0,  // 1DAB0..1DEFF; UNKNOWN
                    0x1DF00,  // 1DF00..1DF1E; LATIN
                    0x1DF1F,  // 1DF1F..1DF24; UNKNOWN
                    0x1DF25,  // 1DF25..1DF2A; LATIN
                    0x1DF2B,  // 1DF2B..1DFFF; UNKNOWN
                    0x1E000,  // 1E000..1E006; GLAGOLITIC
                    0x1E007,  // 1E007       ; UNKNOWN
                    0x1E008,  // 1E008..1E018; GLAGOLITIC
                    0x1E019,  // 1E019..1E01A; UNKNOWN
                    0x1E01B,  // 1E01B..1E021; GLAGOLITIC
                    0x1E022,  // 1E022       ; UNKNOWN
                    0x1E023,  // 1E023..1E024; GLAGOLITIC
                    0x1E025,  // 1E025       ; UNKNOWN
                    0x1E026,  // 1E026..1E02A; GLAGOLITIC
                    0x1E02B,  // 1E02B..1E02F; UNKNOWN
                    0x1E030,  // 1E030..1E06D; CYRILLIC
                    0x1E06E,  // 1E06E..1E08E; UNKNOWN
                    0x1E08F,  // 1E08F       ; CYRILLIC
                    0x1E090,  // 1E090..1E0FF; UNKNOWN
                    0x1E100,  // 1E100..1E12C; NYIAKENG_PUACHUE_HMONG
                    0x1E12D,  // 1E12D..1E12F; UNKNOWN
                    0x1E130,  // 1E130..1E13D; NYIAKENG_PUACHUE_HMONG
                    0x1E13E,  // 1E13E..1E13F; UNKNOWN
                    0x1E140,  // 1E140..1E149; NYIAKENG_PUACHUE_HMONG
                    0x1E14A,  // 1E14A..1E14D; UNKNOWN
                    0x1E14E,  // 1E14E..1E14F; NYIAKENG_PUACHUE_HMONG
                    0x1E150,  // 1E150..1E28F; UNKNOWN
                    0x1E290,  // 1E290..1E2AE; TOTO
                    0x1E2AF,  // 1E2AF..1E2BF; UNKNOWN
                    0x1E2C0,  // 1E2C0..1E2F9; WANCHO
                    0x1E2FA,  // 1E2FA..1E2FE; UNKNOWN
                    0x1E2FF,  // 1E2FF       ; WANCHO
                    0x1E300,  // 1E300..1E4CF; UNKNOWN
                    0x1E4D0,  // 1E4D0..1E4F9; NAG_MUNDARI
                    0x1E4FA,  // 1E4FA..1E7DF; UNKNOWN
                    0x1E7E0,  // 1E7E0..1E7E6; ETHIOPIC
                    0x1E7E7,  // 1E7E7       ; UNKNOWN
                    0x1E7E8,  // 1E7E8..1E7EB; ETHIOPIC
                    0x1E7EC,  // 1E7EC       ; UNKNOWN
                    0x1E7ED,  // 1E7ED..1E7EE; ETHIOPIC
                    0x1E7EF,  // 1E7EF       ; UNKNOWN
                    0x1E7F0,  // 1E7F0..1E7FE; ETHIOPIC
                    0x1E7FF,  // 1E7FF       ; UNKNOWN
                    0x1E800,  // 1E800..1E8C4; MENDE_KIKAKUI
                    0x1E8C5,  // 1E8C5..1E8C6; UNKNOWN
                    0x1E8C7,  // 1E8C7..1E8D6; MENDE_KIKAKUI
                    0x1E8D7,  // 1E8D7..1E8FF; UNKNOWN
                    0x1E900,  // 1E900..1E94B; ADLAM
                    0x1E94C,  // 1E94C..1E94F; UNKNOWN
                    0x1E950,  // 1E950..1E959; ADLAM
                    0x1E95A,  // 1E95A..1E95D; UNKNOWN
                    0x1E95E,  // 1E95E..1E95F; ADLAM
                    0x1E960,  // 1E960..1EC70; UNKNOWN
                    0x1EC71,  // 1EC71..1ECB4; COMMON
                    0x1ECB5,  // 1ECB5..1ED00; UNKNOWN
                    0x1ED01,  // 1ED01..1ED3D; COMMON
                    0x1ED3E,  // 1ED3E..1EDFF; UNKNOWN
                    0x1EE00,  // 1EE00..1EE03; ARABIC
                    0x1EE04,  // 1EE04       ; UNKNOWN
                    0x1EE05,  // 1EE05..1EE1F; ARABIC
                    0x1EE20,  // 1EE20       ; UNKNOWN
                    0x1EE21,  // 1EE21..1EE22; ARABIC
                    0x1EE23,  // 1EE23       ; UNKNOWN
                    0x1EE24,  // 1EE24       ; ARABIC
                    0x1EE25,  // 1EE25..1EE26; UNKNOWN
                    0x1EE27,  // 1EE27       ; ARABIC
                    0x1EE28,  // 1EE28       ; UNKNOWN
                    0x1EE29,  // 1EE29..1EE32; ARABIC
                    0x1EE33,  // 1EE33       ; UNKNOWN
                    0x1EE34,  // 1EE34..1EE37; ARABIC
                    0x1EE38,  // 1EE38       ; UNKNOWN
                    0x1EE39,  // 1EE39       ; ARABIC
                    0x1EE3A,  // 1EE3A       ; UNKNOWN
                    0x1EE3B,  // 1EE3B       ; ARABIC
                    0x1EE3C,  // 1EE3C..1EE41; UNKNOWN
                    0x1EE42,  // 1EE42       ; ARABIC
                    0x1EE43,  // 1EE43..1EE46; UNKNOWN
                    0x1EE47,  // 1EE47       ; ARABIC
                    0x1EE48,  // 1EE48       ; UNKNOWN
                    0x1EE49,  // 1EE49       ; ARABIC
                    0x1EE4A,  // 1EE4A       ; UNKNOWN
                    0x1EE4B,  // 1EE4B       ; ARABIC
                    0x1EE4C,  // 1EE4C       ; UNKNOWN
                    0x1EE4D,  // 1EE4D..1EE4F; ARABIC
                    0x1EE50,  // 1EE50       ; UNKNOWN
                    0x1EE51,  // 1EE51..1EE52; ARABIC
                    0x1EE53,  // 1EE53       ; UNKNOWN
                    0x1EE54,  // 1EE54       ; ARABIC
                    0x1EE55,  // 1EE55..1EE56; UNKNOWN
                    0x1EE57,  // 1EE57       ; ARABIC
                    0x1EE58,  // 1EE58       ; UNKNOWN
                    0x1EE59,  // 1EE59       ; ARABIC
                    0x1EE5A,  // 1EE5A       ; UNKNOWN
                    0x1EE5B,  // 1EE5B       ; ARABIC
                    0x1EE5C,  // 1EE5C       ; UNKNOWN
                    0x1EE5D,  // 1EE5D       ; ARABIC
                    0x1EE5E,  // 1EE5E       ; UNKNOWN
                    0x1EE5F,  // 1EE5F       ; ARABIC
                    0x1EE60,  // 1EE60       ; UNKNOWN
                    0x1EE61,  // 1EE61..1EE62; ARABIC
                    0x1EE63,  // 1EE63       ; UNKNOWN
                    0x1EE64,  // 1EE64       ; ARABIC
                    0x1EE65,  // 1EE65..1EE66; UNKNOWN
                    0x1EE67,  // 1EE67..1EE6A; ARABIC
                    0x1EE6B,  // 1EE6B       ; UNKNOWN
                    0x1EE6C,  // 1EE6C..1EE72; ARABIC
                    0x1EE73,  // 1EE73       ; UNKNOWN
                    0x1EE74,  // 1EE74..1EE77; ARABIC
                    0x1EE78,  // 1EE78       ; UNKNOWN
                    0x1EE79,  // 1EE79..1EE7C; ARABIC
                    0x1EE7D,  // 1EE7D       ; UNKNOWN
                    0x1EE7E,  // 1EE7E       ; ARABIC
                    0x1EE7F,  // 1EE7F       ; UNKNOWN
                    0x1EE80,  // 1EE80..1EE89; ARABIC
                    0x1EE8A,  // 1EE8A       ; UNKNOWN
                    0x1EE8B,  // 1EE8B..1EE9B; ARABIC
                    0x1EE9C,  // 1EE9C..1EEA0; UNKNOWN
                    0x1EEA1,  // 1EEA1..1EEA3; ARABIC
                    0x1EEA4,  // 1EEA4       ; UNKNOWN
                    0x1EEA5,  // 1EEA5..1EEA9; ARABIC
                    0x1EEAA,  // 1EEAA       ; UNKNOWN
                    0x1EEAB,  // 1EEAB..1EEBB; ARABIC
                    0x1EEBC,  // 1EEBC..1EEEF; UNKNOWN
                    0x1EEF0,  // 1EEF0..1EEF1; ARABIC
                    0x1EEF2,  // 1EEF2..1EFFF; UNKNOWN
                    0x1F000,  // 1F000..1F02B; COMMON
                    0x1F02C,  // 1F02C..1F02F; UNKNOWN
                    0x1F030,  // 1F030..1F093; COMMON
                    0x1F094,  // 1F094..1F09F; UNKNOWN
                    0x1F0A0,  // 1F0A0..1F0AE; COMMON
                    0x1F0AF,  // 1F0AF..1F0B0; UNKNOWN
                    0x1F0B1,  // 1F0B1..1F0BF; COMMON
                    0x1F0C0,  // 1F0C0       ; UNKNOWN
                    0x1F0C1,  // 1F0C1..1F0CF; COMMON
                    0x1F0D0,  // 1F0D0       ; UNKNOWN
                    0x1F0D1,  // 1F0D1..1F0F5; COMMON
                    0x1F0F6,  // 1F0F6..1F0FF; UNKNOWN
                    0x1F100,  // 1F100..1F1AD; COMMON
                    0x1F1AE,  // 1F1AE..1F1E5; UNKNOWN
                    0x1F1E6,  // 1F1E6..1F1FF; COMMON
                    0x1F200,  // 1F200       ; HIRAGANA
                    0x1F201,  // 1F201..1F202; COMMON
                    0x1F203,  // 1F203..1F20F; UNKNOWN
                    0x1F210,  // 1F210..1F23B; COMMON
                    0x1F23C,  // 1F23C..1F23F; UNKNOWN
                    0x1F240,  // 1F240..1F248; COMMON
                    0x1F249,  // 1F249..1F24F; UNKNOWN
                    0x1F250,  // 1F250..1F251; COMMON
                    0x1F252,  // 1F252..1F25F; UNKNOWN
                    0x1F260,  // 1F260..1F265; COMMON
                    0x1F266,  // 1F266..1F2FF; UNKNOWN
                    0x1F300,  // 1F300..1F6D7; COMMON
                    0x1F6D8,  // 1F6D8..1F6DB; UNKNOWN
                    0x1F6DC,  // 1F6DC..1F6EC; COMMON
                    0x1F6ED,  // 1F6ED..1F6EF; UNKNOWN
                    0x1F6F0,  // 1F6F0..1F6FC; COMMON
                    0x1F6FD,  // 1F6FD..1F6FF; UNKNOWN
                    0x1F700,  // 1F700..1F776; COMMON
                    0x1F777,  // 1F777..1F77A; UNKNOWN
                    0x1F77B,  // 1F77B..1F7D9; COMMON
                    0x1F7DA,  // 1F7DA..1F7DF; UNKNOWN
                    0x1F7E0,  // 1F7E0..1F7EB; COMMON
                    0x1F7EC,  // 1F7EC..1F7EF; UNKNOWN
                    0x1F7F0,  // 1F7F0       ; COMMON
                    0x1F7F1,  // 1F7F1..1F7FF; UNKNOWN
                    0x1F800,  // 1F800..1F80B; COMMON
                    0x1F80C,  // 1F80C..1F80F; UNKNOWN
                    0x1F810,  // 1F810..1F847; COMMON
                    0x1F848,  // 1F848..1F84F; UNKNOWN
                    0x1F850,  // 1F850..1F859; COMMON
                    0x1F85A,  // 1F85A..1F85F; UNKNOWN
                    0x1F860,  // 1F860..1F887; COMMON
                    0x1F888,  // 1F888..1F88F; UNKNOWN
                    0x1F890,  // 1F890..1F8AD; COMMON
                    0x1F8AE,  // 1F8AE..1F8AF; UNKNOWN
                    0x1F8B0,  // 1F8B0..1F8B1; COMMON
                    0x1F8B2,  // 1F8B2..1F8FF; UNKNOWN
                    0x1F900,  // 1F900..1FA53; COMMON
                    0x1FA54,  // 1FA54..1FA5F; UNKNOWN
                    0x1FA60,  // 1FA60..1FA6D; COMMON
                    0x1FA6E,  // 1FA6E..1FA6F; UNKNOWN
                    0x1FA70,  // 1FA70..1FA7C; COMMON
                    0x1FA7D,  // 1FA7D..1FA7F; UNKNOWN
                    0x1FA80,  // 1FA80..1FA88; COMMON
                    0x1FA89,  // 1FA89..1FA8F; UNKNOWN
                    0x1FA90,  // 1FA90..1FABD; COMMON
                    0x1FABE,  // 1FABE       ; UNKNOWN
                    0x1FABF,  // 1FABF..1FAC5; COMMON
                    0x1FAC6,  // 1FAC6..1FACD; UNKNOWN
                    0x1FACE,  // 1FACE..1FADB; COMMON
                    0x1FADC,  // 1FADC..1FADF; UNKNOWN
                    0x1FAE0,  // 1FAE0..1FAE8; COMMON
                    0x1FAE9,  // 1FAE9..1FAEF; UNKNOWN
                    0x1FAF0,  // 1FAF0..1FAF8; COMMON
                    0x1FAF9,  // 1FAF9..1FAFF; UNKNOWN
                    0x1FB00,  // 1FB00..1FB92; COMMON
                    0x1FB93,  // 1FB93       ; UNKNOWN
                    0x1FB94,  // 1FB94..1FBCA; COMMON
                    0x1FBCB,  // 1FBCB..1FBEF; UNKNOWN
                    0x1FBF0,  // 1FBF0..1FBF9; COMMON
                    0x1FBFA,  // 1FBFA..1FFFF; UNKNOWN
                    0x20000,  // 20000..2A6DF; HAN
                    0x2A6E0,  // 2A6E0..2A6FF; UNKNOWN
                    0x2A700,  // 2A700..2B739; HAN
                    0x2B73A,  // 2B73A..2B73F; UNKNOWN
                    0x2B740,  // 2B740..2B81D; HAN
                    0x2B81E,  // 2B81E..2B81F; UNKNOWN
                    0x2B820,  // 2B820..2CEA1; HAN
                    0x2CEA2,  // 2CEA2..2CEAF; UNKNOWN
                    0x2CEB0,  // 2CEB0..2EBE0; HAN
                    0x2EBE1,  // 2EBE1..2F7FF; UNKNOWN
                    0x2F800,  // 2F800..2FA1D; HAN
                    0x2FA1E,  // 2FA1E..2FFFF; UNKNOWN
                    0x30000,  // 30000..3134A; HAN
                    0x3134B,  // 3134B..3134F; UNKNOWN
                    0x31350,  // 31350..323AF; HAN
                    0x323B0,  // 323B0..E0000; UNKNOWN
                    0xE0001,  // E0001       ; COMMON
                    0xE0002,  // E0002..E001F; UNKNOWN
                    0xE0020,  // E0020..E007F; COMMON
                    0xE0080,  // E0080..E00FF; UNKNOWN
                    0xE0100,  // E0100..E01EF; INHERITED
                    0xE01F0  // E01F0..10FFFF; UNKNOWN
            };

            CORE_FAST gint Y[] = {
                    0x0000,   // 0000..007F; Basic Latin
                    0x0080,   // 0080..00FF; Latin-1 Supplement
                    0x0100,   // 0100..017F; Latin Extended-A
                    0x0180,   // 0180..024F; Latin Extended-B
                    0x0250,   // 0250..02AF; IPA Extensions
                    0x02B0,   // 02B0..02FF; Spacing Modifier Letters
                    0x0300,   // 0300..036F; Combining Diacritical Marks
                    0x0370,   // 0370..03FF; Greek and Coptic
                    0x0400,   // 0400..04FF; Cyrillic
                    0x0500,   // 0500..052F; Cyrillic Supplement
                    0x0530,   // 0530..058F; Armenian
                    0x0590,   // 0590..05FF; Hebrew
                    0x0600,   // 0600..06FF; Arabic
                    0x0700,   // 0700..074F; Syriac
                    0x0750,   // 0750..077F; Arabic Supplement
                    0x0780,   // 0780..07BF; Thaana
                    0x07C0,   // 07C0..07FF; NKo
                    0x0800,   // 0800..083F; Samaritan
                    0x0840,   // 0840..085F; Mandaic
                    0x0860,   // 0860..086F; Syriac Supplement
                    0x0870,   // 0870..089F; Arabic Extended-B
                    0x08A0,   // 08A0..08FF; Arabic Extended-A
                    0x0900,   // 0900..097F; Devanagari
                    0x0980,   // 0980..09FF; Bengali
                    0x0A00,   // 0A00..0A7F; Gurmukhi
                    0x0A80,   // 0A80..0AFF; Gujarati
                    0x0B00,   // 0B00..0B7F; Oriya
                    0x0B80,   // 0B80..0BFF; Tamil
                    0x0C00,   // 0C00..0C7F; Telugu
                    0x0C80,   // 0C80..0CFF; Kannada
                    0x0D00,   // 0D00..0D7F; Malayalam
                    0x0D80,   // 0D80..0DFF; Sinhala
                    0x0E00,   // 0E00..0E7F; Thai
                    0x0E80,   // 0E80..0EFF; Lao
                    0x0F00,   // 0F00..0FFF; Tibetan
                    0x1000,   // 1000..109F; Myanmar
                    0x10A0,   // 10A0..10FF; Georgian
                    0x1100,   // 1100..11FF; Hangul Jamo
                    0x1200,   // 1200..137F; Ethiopic
                    0x1380,   // 1380..139F; Ethiopic Supplement
                    0x13A0,   // 13A0..13FF; Cherokee
                    0x1400,   // 1400..167F; Unified Canadian Aboriginal Syllabics
                    0x1680,   // 1680..169F; Ogham
                    0x16A0,   // 16A0..16FF; Runic
                    0x1700,   // 1700..171F; Tagalog
                    0x1720,   // 1720..173F; Hanunoo
                    0x1740,   // 1740..175F; Buhid
                    0x1760,   // 1760..177F; Tagbanwa
                    0x1780,   // 1780..17FF; Khmer
                    0x1800,   // 1800..18AF; Mongolian
                    0x18B0,   // 18B0..18FF; Unified Canadian Aboriginal Syllabics Extended
                    0x1900,   // 1900..194F; Limbu
                    0x1950,   // 1950..197F; Tai Le
                    0x1980,   // 1980..19DF; New Tai Lue
                    0x19E0,   // 19E0..19FF; Khmer Symbols
                    0x1A00,   // 1A00..1A1F; Buginese
                    0x1A20,   // 1A20..1AAF; Tai Tham
                    0x1AB0,   // 1AB0..1AFF; Combining Diacritical Marks Extended
                    0x1B00,   // 1B00..1B7F; Balinese
                    0x1B80,   // 1B80..1BBF; Sundanese
                    0x1BC0,   // 1BC0..1BFF; Batak
                    0x1C00,   // 1C00..1C4F; Lepcha
                    0x1C50,   // 1C50..1C7F; Ol Chiki
                    0x1C80,   // 1C80..1C8F; Cyrillic Extended-C
                    0x1C90,   // 1C90..1CBF; Georgian Extended
                    0x1CC0,   // 1CC0..1CCF; Sundanese Supplement
                    0x1CD0,   // 1CD0..1CFF; Vedic Extensions
                    0x1D00,   // 1D00..1D7F; Phonetic Extensions
                    0x1D80,   // 1D80..1DBF; Phonetic Extensions Supplement
                    0x1DC0,   // 1DC0..1DFF; Combining Diacritical Marks Supplement
                    0x1E00,   // 1E00..1EFF; Latin Extended Additional
                    0x1F00,   // 1F00..1FFF; Greek Extended
                    0x2000,   // 2000..206F; General Punctuation
                    0x2070,   // 2070..209F; Superscripts and Subscripts
                    0x20A0,   // 20A0..20CF; Currency Symbols
                    0x20D0,   // 20D0..20FF; Combining Diacritical Marks for Symbols
                    0x2100,   // 2100..214F; Letterlike Symbols
                    0x2150,   // 2150..218F; Number Forms
                    0x2190,   // 2190..21FF; Arrows
                    0x2200,   // 2200..22FF; Mathematical Operators
                    0x2300,   // 2300..23FF; Miscellaneous Technical
                    0x2400,   // 2400..243F; Control Pictures
                    0x2440,   // 2440..245F; Optical Character Recognition
                    0x2460,   // 2460..24FF; Enclosed Alphanumerics
                    0x2500,   // 2500..257F; Box Drawing
                    0x2580,   // 2580..259F; Block Elements
                    0x25A0,   // 25A0..25FF; Geometric Shapes
                    0x2600,   // 2600..26FF; Miscellaneous Symbols
                    0x2700,   // 2700..27BF; Dingbats
                    0x27C0,   // 27C0..27EF; Miscellaneous Mathematical Symbols-A
                    0x27F0,   // 27F0..27FF; Supplemental Arrows-A
                    0x2800,   // 2800..28FF; Braille Patterns
                    0x2900,   // 2900..297F; Supplemental Arrows-B
                    0x2980,   // 2980..29FF; Miscellaneous Mathematical Symbols-B
                    0x2A00,   // 2A00..2AFF; Supplemental Mathematical Operators
                    0x2B00,   // 2B00..2BFF; Miscellaneous Symbols and Arrows
                    0x2C00,   // 2C00..2C5F; Glagolitic
                    0x2C60,   // 2C60..2C7F; Latin Extended-C
                    0x2C80,   // 2C80..2CFF; Coptic
                    0x2D00,   // 2D00..2D2F; Georgian Supplement
                    0x2D30,   // 2D30..2D7F; Tifinagh
                    0x2D80,   // 2D80..2DDF; Ethiopic Extended
                    0x2DE0,   // 2DE0..2DFF; Cyrillic Extended-A
                    0x2E00,   // 2E00..2E7F; Supplemental Punctuation
                    0x2E80,   // 2E80..2EFF; CJK Radicals Supplement
                    0x2F00,   // 2F00..2FDF; Kangxi Radicals
                    0x2FE0,   //             unassigned
                    0x2FF0,   // 2FF0..2FFF; Ideographic Description Characters
                    0x3000,   // 3000..303F; CJK Symbols and Punctuation
                    0x3040,   // 3040..309F; Hiragana
                    0x30A0,   // 30A0..30FF; Katakana
                    0x3100,   // 3100..312F; Bopomofo
                    0x3130,   // 3130..318F; Hangul Compatibility Jamo
                    0x3190,   // 3190..319F; Kanbun
                    0x31A0,   // 31A0..31BF; Bopomofo Extended
                    0x31C0,   // 31C0..31EF; CJK Strokes
                    0x31F0,   // 31F0..31FF; Katakana Phonetic Extensions
                    0x3200,   // 3200..32FF; Enclosed CJK Letters and Months
                    0x3300,   // 3300..33FF; CJK Compatibility
                    0x3400,   // 3400..4DBF; CJK Unified Ideographs Extension A
                    0x4DC0,   // 4DC0..4DFF; Yijing Hexagram Symbols
                    0x4E00,   // 4E00..9FFF; CJK Unified Ideographs
                    0xA000,   // A000..A48F; Yi Syllables
                    0xA490,   // A490..A4CF; Yi Radicals
                    0xA4D0,   // A4D0..A4FF; Lisu
                    0xA500,   // A500..A63F; Vai
                    0xA640,   // A640..A69F; Cyrillic Extended-B
                    0xA6A0,   // A6A0..A6FF; Bamum
                    0xA700,   // A700..A71F; Modifier Tone Letters
                    0xA720,   // A720..A7FF; Latin Extended-D
                    0xA800,   // A800..A82F; Syloti Nagri
                    0xA830,   // A830..A83F; Common Indic Number Forms
                    0xA840,   // A840..A87F; Phags-pa
                    0xA880,   // A880..A8DF; Saurashtra
                    0xA8E0,   // A8E0..A8FF; Devanagari Extended
                    0xA900,   // A900..A92F; Kayah Li
                    0xA930,   // A930..A95F; Rejang
                    0xA960,   // A960..A97F; Hangul Jamo Extended-A
                    0xA980,   // A980..A9DF; Javanese
                    0xA9E0,   // A9E0..A9FF; Myanmar Extended-B
                    0xAA00,   // AA00..AA5F; Cham
                    0xAA60,   // AA60..AA7F; Myanmar Extended-A
                    0xAA80,   // AA80..AADF; Tai Viet
                    0xAAE0,   // AAE0..AAFF; Meetei Mayek Extensions
                    0xAB00,   // AB00..AB2F; Ethiopic Extended-A
                    0xAB30,   // AB30..AB6F; Latin Extended-E
                    0xAB70,   // AB70..ABBF; Cherokee Supplement
                    0xABC0,   // ABC0..ABFF; Meetei Mayek
                    0xAC00,   // AC00..D7AF; Hangul Syllables
                    0xD7B0,   // D7B0..D7FF; Hangul Jamo Extended-B
                    0xD800,   // D800..DB7F; High Surrogates
                    0xDB80,   // DB80..DBFF; High Private Use Surrogates
                    0xDC00,   // DC00..DFFF; Low Surrogates
                    0xE000,   // E000..F8FF; Private Use Area
                    0xF900,   // F900..FAFF; CJK Compatibility Ideographs
                    0xFB00,   // FB00..FB4F; Alphabetic Presentation Forms
                    0xFB50,   // FB50..FDFF; Arabic Presentation Forms-A
                    0xFE00,   // FE00..FE0F; Variation Selectors
                    0xFE10,   // FE10..FE1F; Vertical Forms
                    0xFE20,   // FE20..FE2F; Combining Half Marks
                    0xFE30,   // FE30..FE4F; CJK Compatibility Forms
                    0xFE50,   // FE50..FE6F; Small Form Variants
                    0xFE70,   // FE70..FEFF; Arabic Presentation Forms-B
                    0xFF00,   // FF00..FFEF; Halfwidth and Fullwidth Forms
                    0xFFF0,   // FFF0..FFFF; Specials
                    0x10000,  // 10000..1007F; Linear B Syllabary
                    0x10080,  // 10080..100FF; Linear B Ideograms
                    0x10100,  // 10100..1013F; Aegean Numbers
                    0x10140,  // 10140..1018F; Ancient Greek Numbers
                    0x10190,  // 10190..101CF; Ancient Symbols
                    0x101D0,  // 101D0..101FF; Phaistos Disc
                    0x10200,  //               unassigned
                    0x10280,  // 10280..1029F; Lycian
                    0x102A0,  // 102A0..102DF; Carian
                    0x102E0,  // 102E0..102FF; Coptic Epact Numbers
                    0x10300,  // 10300..1032F; Old Italic
                    0x10330,  // 10330..1034F; Gothic
                    0x10350,  // 10350..1037F; Old Permic
                    0x10380,  // 10380..1039F; Ugaritic
                    0x103A0,  // 103A0..103DF; Old Persian
                    0x103E0,  //               unassigned
                    0x10400,  // 10400..1044F; Deseret
                    0x10450,  // 10450..1047F; Shavian
                    0x10480,  // 10480..104AF; Osmanya
                    0x104B0,  // 104B0..104FF; Osage
                    0x10500,  // 10500..1052F; Elbasan
                    0x10530,  // 10530..1056F; Caucasian Albanian
                    0x10570,  // 10570..105BF; Vithkuqi
                    0x105C0,  //               unassigned
                    0x10600,  // 10600..1077F; Linear A
                    0x10780,  // 10780..107BF; Latin Extended-F
                    0x107C0,  //               unassigned
                    0x10800,  // 10800..1083F; Cypriot Syllabary
                    0x10840,  // 10840..1085F; Imperial Aramaic
                    0x10860,  // 10860..1087F; Palmyrene
                    0x10880,  // 10880..108AF; Nabataean
                    0x108B0,  //               unassigned
                    0x108E0,  // 108E0..108FF; Hatran
                    0x10900,  // 10900..1091F; Phoenician
                    0x10920,  // 10920..1093F; Lydian
                    0x10940,  //               unassigned
                    0x10980,  // 10980..1099F; Meroitic Hieroglyphs
                    0x109A0,  // 109A0..109FF; Meroitic Cursive
                    0x10A00,  // 10A00..10A5F; Kharoshthi
                    0x10A60,  // 10A60..10A7F; Old South Arabian
                    0x10A80,  // 10A80..10A9F; Old North Arabian
                    0x10AA0,  //               unassigned
                    0x10AC0,  // 10AC0..10AFF; Manichaean
                    0x10B00,  // 10B00..10B3F; Avestan
                    0x10B40,  // 10B40..10B5F; Inscriptional Parthian
                    0x10B60,  // 10B60..10B7F; Inscriptional Pahlavi
                    0x10B80,  // 10B80..10BAF; Psalter Pahlavi
                    0x10BB0,  //               unassigned
                    0x10C00,  // 10C00..10C4F; Old Turkic
                    0x10C50,  //               unassigned
                    0x10C80,  // 10C80..10CFF; Old Hungarian
                    0x10D00,  // 10D00..10D3F; Hanifi Rohingya
                    0x10D40,  //               unassigned
                    0x10E60,  // 10E60..10E7F; Rumi Numeral Symbols
                    0x10E80,  // 10E80..10EBF; Yezidi
                    0x10EC0,  // 10EC0..10EFF; Arabic Extended-C
                    0x10F00,  // 10F00..10F2F; Old Sogdian
                    0x10F30,  // 10F30..10F6F; Sogdian
                    0x10F70,  // 10F70..10FAF; Old Uyghur
                    0x10FB0,  // 10FB0..10FDF; Chorasmian
                    0x10FE0,  // 10FE0..10FFF; Elymaic
                    0x11000,  // 11000..1107F; Brahmi
                    0x11080,  // 11080..110CF; Kaithi
                    0x110D0,  // 110D0..110FF; Sora Sompeng
                    0x11100,  // 11100..1114F; Chakma
                    0x11150,  // 11150..1117F; Mahajani
                    0x11180,  // 11180..111DF; Sharada
                    0x111E0,  // 111E0..111FF; Sinhala Archaic Numbers
                    0x11200,  // 11200..1124F; Khojki
                    0x11250,  //               unassigned
                    0x11280,  // 11280..112AF; Multani
                    0x112B0,  // 112B0..112FF; Khudawadi
                    0x11300,  // 11300..1137F; Grantha
                    0x11380,  //               unassigned
                    0x11400,  // 11400..1147F; Newa
                    0x11480,  // 11480..114DF; Tirhuta
                    0x114E0,  //               unassigned
                    0x11580,  // 11580..115FF; Siddham
                    0x11600,  // 11600..1165F; Modi
                    0x11660,  // 11660..1167F; Mongolian Supplement
                    0x11680,  // 11680..116CF; Takri
                    0x116D0,  //               unassigned
                    0x11700,  // 11700..1174F; Ahom
                    0x11750,  //               unassigned
                    0x11800,  // 11800..1184F; Dogra
                    0x11850,  //               unassigned
                    0x118A0,  // 118A0..118FF; Warang Citi
                    0x11900,  // 11900..1195F; Dives Akuru
                    0x11960,  //               unassigned
                    0x119A0,  // 119A0..119FF; Nandinagari
                    0x11A00,  // 11A00..11A4F; Zanabazar Square
                    0x11A50,  // 11A50..11AAF; Soyombo
                    0x11AB0,  // 11AB0..11ABF; Unified Canadian Aboriginal Syllabics Extended-A
                    0x11AC0,  // 11AC0..11AFF; Pau Cin Hau
                    0x11B00,  // 11B00..11B5F; Devanagari Extended-A
                    0x11B60,  //               unassigned
                    0x11C00,  // 11C00..11C6F; Bhaiksuki
                    0x11C70,  // 11C70..11CBF; Marchen
                    0x11CC0,  //               unassigned
                    0x11D00,  // 11D00..11D5F; Masaram Gondi
                    0x11D60,  // 11D60..11DAF; Gunjala Gondi
                    0x11DB0,  //               unassigned
                    0x11EE0,  // 11EE0..11EFF; Makasar
                    0x11F00,  // 11F00..11F5F; Kawi
                    0x11F60,  //               unassigned
                    0x11FB0,  // 11FB0..11FBF; Lisu Supplement
                    0x11FC0,  // 11FC0..11FFF; Tamil Supplement
                    0x12000,  // 12000..123FF; Cuneiform
                    0x12400,  // 12400..1247F; Cuneiform Numbers and Punctuation
                    0x12480,  // 12480..1254F; Early Dynastic Cuneiform
                    0x12550,  //               unassigned
                    0x12F90,  // 12F90..12FFF; Cypro-Minoan
                    0x13000,  // 13000..1342F; Egyptian Hieroglyphs
                    0x13430,  // 13430..1345F; Egyptian Hieroglyph Format Controls
                    0x13460,  //               unassigned
                    0x14400,  // 14400..1467F; Anatolian Hieroglyphs
                    0x14680,  //               unassigned
                    0x16800,  // 16800..16A3F; Bamum Supplement
                    0x16A40,  // 16A40..16A6F; Mro
                    0x16A70,  // 16A70..16ACF; Tangsa
                    0x16AD0,  // 16AD0..16AFF; Bassa Vah
                    0x16B00,  // 16B00..16B8F; Pahawh Hmong
                    0x16B90,  //               unassigned
                    0x16E40,  // 16E40..16E9F; Medefaidrin
                    0x16EA0,  //               unassigned
                    0x16F00,  // 16F00..16F9F; Miao
                    0x16FA0,  //               unassigned
                    0x16FE0,  // 16FE0..16FFF; Ideographic Symbols and Punctuation
                    0x17000,  // 17000..187FF; Tangut
                    0x18800,  // 18800..18AFF; Tangut Components
                    0x18B00,  // 18B00..18CFF; Khitan Small Script
                    0x18D00,  // 18D00..18D7F; Tangut Supplement
                    0x18D80,  //               unassigned
                    0x1AFF0,  // 1AFF0..1AFFF; Kana Extended-B
                    0x1B000,  // 1B000..1B0FF; Kana Supplement
                    0x1B100,  // 1B100..1B12F; Kana Extended-A
                    0x1B130,  // 1B130..1B16F; Small Kana Extension
                    0x1B170,  // 1B170..1B2FF; Nushu
                    0x1B300,  //               unassigned
                    0x1BC00,  // 1BC00..1BC9F; Duployan
                    0x1BCA0,  // 1BCA0..1BCAF; Shorthand Format Controls
                    0x1BCB0,  //               unassigned
                    0x1CF00,  // 1CF00..1CFCF; Znamenny Musical Notation
                    0x1CFD0,  //               unassigned
                    0x1D000,  // 1D000..1D0FF; Byzantine Musical Symbols
                    0x1D100,  // 1D100..1D1FF; Musical Symbols
                    0x1D200,  // 1D200..1D24F; Ancient Greek Musical Notation
                    0x1D250,  //               unassigned
                    0x1D2C0,  // 1D2C0..1D2DF; Kaktovik Numerals
                    0x1D2E0,  // 1D2E0..1D2FF; Mayan Numerals
                    0x1D300,  // 1D300..1D35F; Tai Xuan Jing Symbols
                    0x1D360,  // 1D360..1D37F; Counting Rod Numerals
                    0x1D380,  //               unassigned
                    0x1D400,  // 1D400..1D7FF; Mathematical Alphanumeric Symbols
                    0x1D800,  // 1D800..1DAAF; Sutton SignWriting
                    0x1DAB0,  //               unassigned
                    0x1DF00,  // 1DF00..1DFFF; Latin Extended-G
                    0x1E000,  // 1E000..1E02F; Glagolitic Supplement
                    0x1E030,  // 1E030..1E08F; Cyrillic Extended-D
                    0x1E090,  //               unassigned
                    0x1E100,  // 1E100..1E14F; Nyiakeng Puachue Hmong
                    0x1E150,  //               unassigned
                    0x1E290,  // 1E290..1E2BF; Toto
                    0x1E2C0,  // 1E2C0..1E2FF; Wancho
                    0x1E300,  //               unassigned
                    0x1E4D0,  // 1E4D0..1E4FF; Nag Mundari
                    0x1E500,  //               unassigned
                    0x1E7E0,  // 1E7E0..1E7FF; Ethiopic Extended-B
                    0x1E800,  // 1E800..1E8DF; Mende Kikakui
                    0x1E8E0,  //               unassigned
                    0x1E900,  // 1E900..1E95F; Adlam
                    0x1E960,  //               unassigned
                    0x1EC70,  // 1EC70..1ECBF; Indic Siyaq Numbers
                    0x1ECC0,  //               unassigned
                    0x1ED00,  // 1ED00..1ED4F; Ottoman Siyaq Numbers
                    0x1ED50,  //               unassigned
                    0x1EE00,  // 1EE00..1EEFF; Arabic Mathematical Alphabetic Symbols
                    0x1EF00,  //               unassigned
                    0x1F000,  // 1F000..1F02F; Mahjong Tiles
                    0x1F030,  // 1F030..1F09F; Domino Tiles
                    0x1F0A0,  // 1F0A0..1F0FF; Playing Cards
                    0x1F100,  // 1F100..1F1FF; Enclosed Alphanumeric Supplement
                    0x1F200,  // 1F200..1F2FF; Enclosed Ideographic Supplement
                    0x1F300,  // 1F300..1F5FF; Miscellaneous Symbols and Pictographs
                    0x1F600,  // 1F600..1F64F; Emoticons
                    0x1F650,  // 1F650..1F67F; Ornamental Dingbats
                    0x1F680,  // 1F680..1F6FF; Transport and Map Symbols
                    0x1F700,  // 1F700..1F77F; Alchemical Symbols
                    0x1F780,  // 1F780..1F7FF; Geometric Shapes Extended
                    0x1F800,  // 1F800..1F8FF; Supplemental Arrows-C
                    0x1F900,  // 1F900..1F9FF; Supplemental Symbols and Pictographs
                    0x1FA00,  // 1FA00..1FA6F; Chess Symbols
                    0x1FA70,  // 1FA70..1FAFF; Symbols and Pictographs Extended-A
                    0x1FB00,  // 1FB00..1FBFF; Symbols for Legacy Computing
                    0x1FC00,  //               unassigned
                    0x20000,  // 20000..2A6DF; CJK Unified Ideographs Extension B
                    0x2A6E0,  //               unassigned
                    0x2A700,  // 2A700..2B73F; CJK Unified Ideographs Extension C
                    0x2B740,  // 2B740..2B81F; CJK Unified Ideographs Extension D
                    0x2B820,  // 2B820..2CEAF; CJK Unified Ideographs Extension E
                    0x2CEB0,  // 2CEB0..2EBEF; CJK Unified Ideographs Extension F
                    0x2EBF0,  //               unassigned
                    0x2F800,  // 2F800..2FA1F; CJK Compatibility Ideographs Supplement
                    0x2FA20,  //               unassigned
                    0x30000,  // 30000..3134F; CJK Unified Ideographs Extension G
                    0x31350,  // 31350..323AF; CJK Unified Ideographs Extension H
                    0x323B0,  //               unassigned
                    0xE0000,  // E0000..E007F; Tags
                    0xE0080,  //               unassigned
                    0xE0100,  // E0100..E01EF; Variation Selectors Supplement
                    0xE01F0,  //               unassigned
                    0xF0000,  // F0000..FFFFF; Supplementary Private Use Area-A
                    0x100000, // 100000..10FFFF; Supplementary Private Use Area-B
            };

            CORE_FAST UnicodeScript A[] = {
                    UnicodeScript::COMMON,                   // 0000..0040
                    UnicodeScript::LATIN,                    // 0041..005A
                    UnicodeScript::COMMON,                   // 005B..0060
                    UnicodeScript::LATIN,                    // 0061..007A
                    UnicodeScript::COMMON,                   // 007B..00A9
                    UnicodeScript::LATIN,                    // 00AA
                    UnicodeScript::COMMON,                   // 00AB..00B9
                    UnicodeScript::LATIN,                    // 00BA
                    UnicodeScript::COMMON,                   // 00BB..00BF
                    UnicodeScript::LATIN,                    // 00C0..00D6
                    UnicodeScript::COMMON,                   // 00D7
                    UnicodeScript::LATIN,                    // 00D8..00F6
                    UnicodeScript::COMMON,                   // 00F7
                    UnicodeScript::LATIN,                    // 00F8..02B8
                    UnicodeScript::COMMON,                   // 02B9..02DF
                    UnicodeScript::LATIN,                    // 02E0..02E4
                    UnicodeScript::COMMON,                   // 02E5..02E9
                    UnicodeScript::BOPOMOFO,                 // 02EA..02EB
                    UnicodeScript::COMMON,                   // 02EC..02FF
                    UnicodeScript::INHERITED,                // 0300..036F
                    UnicodeScript::GREEK,                    // 0370..0373
                    UnicodeScript::COMMON,                   // 0374
                    UnicodeScript::GREEK,                    // 0375..0377
                    UnicodeScript::UNKNOWN,                  // 0378..0379
                    UnicodeScript::GREEK,                    // 037A..037D
                    UnicodeScript::COMMON,                   // 037E
                    UnicodeScript::GREEK,                    // 037F
                    UnicodeScript::UNKNOWN,                  // 0380..0383
                    UnicodeScript::GREEK,                    // 0384
                    UnicodeScript::COMMON,                   // 0385
                    UnicodeScript::GREEK,                    // 0386
                    UnicodeScript::COMMON,                   // 0387
                    UnicodeScript::GREEK,                    // 0388..038A
                    UnicodeScript::UNKNOWN,                  // 038B
                    UnicodeScript::GREEK,                    // 038C
                    UnicodeScript::UNKNOWN,                  // 038D
                    UnicodeScript::GREEK,                    // 038E..03A1
                    UnicodeScript::UNKNOWN,                  // 03A2
                    UnicodeScript::GREEK,                    // 03A3..03E1
                    UnicodeScript::COPTIC,                   // 03E2..03EF
                    UnicodeScript::GREEK,                    // 03F0..03FF
                    UnicodeScript::CYRILLIC,                 // 0400..0484
                    UnicodeScript::INHERITED,                // 0485..0486
                    UnicodeScript::CYRILLIC,                 // 0487..052F
                    UnicodeScript::UNKNOWN,                  // 0530
                    UnicodeScript::ARMENIAN,                 // 0531..0556
                    UnicodeScript::UNKNOWN,                  // 0557..0558
                    UnicodeScript::ARMENIAN,                 // 0559..058A
                    UnicodeScript::UNKNOWN,                  // 058B..058C
                    UnicodeScript::ARMENIAN,                 // 058D..058F
                    UnicodeScript::UNKNOWN,                  // 0590
                    UnicodeScript::HEBREW,                   // 0591..05C7
                    UnicodeScript::UNKNOWN,                  // 05C8..05CF
                    UnicodeScript::HEBREW,                   // 05D0..05EA
                    UnicodeScript::UNKNOWN,                  // 05EB..05EE
                    UnicodeScript::HEBREW,                   // 05EF..05F4
                    UnicodeScript::UNKNOWN,                  // 05F5..05FF
                    UnicodeScript::ARABIC,                   // 0600..0604
                    UnicodeScript::COMMON,                   // 0605
                    UnicodeScript::ARABIC,                   // 0606..060B
                    UnicodeScript::COMMON,                   // 060C
                    UnicodeScript::ARABIC,                   // 060D..061A
                    UnicodeScript::COMMON,                   // 061B
                    UnicodeScript::ARABIC,                   // 061C..061E
                    UnicodeScript::COMMON,                   // 061F
                    UnicodeScript::ARABIC,                   // 0620..063F
                    UnicodeScript::COMMON,                   // 0640
                    UnicodeScript::ARABIC,                   // 0641..064A
                    UnicodeScript::INHERITED,                // 064B..0655
                    UnicodeScript::ARABIC,                   // 0656..066F
                    UnicodeScript::INHERITED,                // 0670
                    UnicodeScript::ARABIC,                   // 0671..06DC
                    UnicodeScript::COMMON,                   // 06DD
                    UnicodeScript::ARABIC,                   // 06DE..06FF
                    UnicodeScript::SYRIAC,                   // 0700..070D
                    UnicodeScript::UNKNOWN,                  // 070E
                    UnicodeScript::SYRIAC,                   // 070F..074A
                    UnicodeScript::UNKNOWN,                  // 074B..074C
                    UnicodeScript::SYRIAC,                   // 074D..074F
                    UnicodeScript::ARABIC,                   // 0750..077F
                    UnicodeScript::THAANA,                   // 0780..07B1
                    UnicodeScript::UNKNOWN,                  // 07B2..07BF
                    UnicodeScript::NKO,                      // 07C0..07FA
                    UnicodeScript::UNKNOWN,                  // 07FB..07FC
                    UnicodeScript::NKO,                      // 07FD..07FF
                    UnicodeScript::SAMARITAN,                // 0800..082D
                    UnicodeScript::UNKNOWN,                  // 082E..082F
                    UnicodeScript::SAMARITAN,                // 0830..083E
                    UnicodeScript::UNKNOWN,                  // 083F
                    UnicodeScript::MANDAIC,                  // 0840..085B
                    UnicodeScript::UNKNOWN,                  // 085C..085D
                    UnicodeScript::MANDAIC,                  // 085E
                    UnicodeScript::UNKNOWN,                  // 085F
                    UnicodeScript::SYRIAC,                   // 0860..086A
                    UnicodeScript::UNKNOWN,                  // 086B..086F
                    UnicodeScript::ARABIC,                   // 0870..088E
                    UnicodeScript::UNKNOWN,                  // 088F
                    UnicodeScript::ARABIC,                   // 0890..0891
                    UnicodeScript::UNKNOWN,                  // 0892..0897
                    UnicodeScript::ARABIC,                   // 0898..08E1
                    UnicodeScript::COMMON,                   // 08E2
                    UnicodeScript::ARABIC,                   // 08E3..08FF
                    UnicodeScript::DEVANAGARI,               // 0900..0950
                    UnicodeScript::INHERITED,                // 0951..0954
                    UnicodeScript::DEVANAGARI,               // 0955..0963
                    UnicodeScript::COMMON,                   // 0964..0965
                    UnicodeScript::DEVANAGARI,               // 0966..097F
                    UnicodeScript::BENGALI,                  // 0980..0983
                    UnicodeScript::UNKNOWN,                  // 0984
                    UnicodeScript::BENGALI,                  // 0985..098C
                    UnicodeScript::UNKNOWN,                  // 098D..098E
                    UnicodeScript::BENGALI,                  // 098F..0990
                    UnicodeScript::UNKNOWN,                  // 0991..0992
                    UnicodeScript::BENGALI,                  // 0993..09A8
                    UnicodeScript::UNKNOWN,                  // 09A9
                    UnicodeScript::BENGALI,                  // 09AA..09B0
                    UnicodeScript::UNKNOWN,                  // 09B1
                    UnicodeScript::BENGALI,                  // 09B2
                    UnicodeScript::UNKNOWN,                  // 09B3..09B5
                    UnicodeScript::BENGALI,                  // 09B6..09B9
                    UnicodeScript::UNKNOWN,                  // 09BA..09BB
                    UnicodeScript::BENGALI,                  // 09BC..09C4
                    UnicodeScript::UNKNOWN,                  // 09C5..09C6
                    UnicodeScript::BENGALI,                  // 09C7..09C8
                    UnicodeScript::UNKNOWN,                  // 09C9..09CA
                    UnicodeScript::BENGALI,                  // 09CB..09CE
                    UnicodeScript::UNKNOWN,                  // 09CF..09D6
                    UnicodeScript::BENGALI,                  // 09D7
                    UnicodeScript::UNKNOWN,                  // 09D8..09DB
                    UnicodeScript::BENGALI,                  // 09DC..09DD
                    UnicodeScript::UNKNOWN,                  // 09DE
                    UnicodeScript::BENGALI,                  // 09DF..09E3
                    UnicodeScript::UNKNOWN,                  // 09E4..09E5
                    UnicodeScript::BENGALI,                  // 09E6..09FE
                    UnicodeScript::UNKNOWN,                  // 09FF..0A00
                    UnicodeScript::GURMUKHI,                 // 0A01..0A03
                    UnicodeScript::UNKNOWN,                  // 0A04
                    UnicodeScript::GURMUKHI,                 // 0A05..0A0A
                    UnicodeScript::UNKNOWN,                  // 0A0B..0A0E
                    UnicodeScript::GURMUKHI,                 // 0A0F..0A10
                    UnicodeScript::UNKNOWN,                  // 0A11..0A12
                    UnicodeScript::GURMUKHI,                 // 0A13..0A28
                    UnicodeScript::UNKNOWN,                  // 0A29
                    UnicodeScript::GURMUKHI,                 // 0A2A..0A30
                    UnicodeScript::UNKNOWN,                  // 0A31
                    UnicodeScript::GURMUKHI,                 // 0A32..0A33
                    UnicodeScript::UNKNOWN,                  // 0A34
                    UnicodeScript::GURMUKHI,                 // 0A35..0A36
                    UnicodeScript::UNKNOWN,                  // 0A37
                    UnicodeScript::GURMUKHI,                 // 0A38..0A39
                    UnicodeScript::UNKNOWN,                  // 0A3A..0A3B
                    UnicodeScript::GURMUKHI,                 // 0A3C
                    UnicodeScript::UNKNOWN,                  // 0A3D
                    UnicodeScript::GURMUKHI,                 // 0A3E..0A42
                    UnicodeScript::UNKNOWN,                  // 0A43..0A46
                    UnicodeScript::GURMUKHI,                 // 0A47..0A48
                    UnicodeScript::UNKNOWN,                  // 0A49..0A4A
                    UnicodeScript::GURMUKHI,                 // 0A4B..0A4D
                    UnicodeScript::UNKNOWN,                  // 0A4E..0A50
                    UnicodeScript::GURMUKHI,                 // 0A51
                    UnicodeScript::UNKNOWN,                  // 0A52..0A58
                    UnicodeScript::GURMUKHI,                 // 0A59..0A5C
                    UnicodeScript::UNKNOWN,                  // 0A5D
                    UnicodeScript::GURMUKHI,                 // 0A5E
                    UnicodeScript::UNKNOWN,                  // 0A5F..0A65
                    UnicodeScript::GURMUKHI,                 // 0A66..0A76
                    UnicodeScript::UNKNOWN,                  // 0A77..0A80
                    UnicodeScript::GUJARATI,                 // 0A81..0A83
                    UnicodeScript::UNKNOWN,                  // 0A84
                    UnicodeScript::GUJARATI,                 // 0A85..0A8D
                    UnicodeScript::UNKNOWN,                  // 0A8E
                    UnicodeScript::GUJARATI,                 // 0A8F..0A91
                    UnicodeScript::UNKNOWN,                  // 0A92
                    UnicodeScript::GUJARATI,                 // 0A93..0AA8
                    UnicodeScript::UNKNOWN,                  // 0AA9
                    UnicodeScript::GUJARATI,                 // 0AAA..0AB0
                    UnicodeScript::UNKNOWN,                  // 0AB1
                    UnicodeScript::GUJARATI,                 // 0AB2..0AB3
                    UnicodeScript::UNKNOWN,                  // 0AB4
                    UnicodeScript::GUJARATI,                 // 0AB5..0AB9
                    UnicodeScript::UNKNOWN,                  // 0ABA..0ABB
                    UnicodeScript::GUJARATI,                 // 0ABC..0AC5
                    UnicodeScript::UNKNOWN,                  // 0AC6
                    UnicodeScript::GUJARATI,                 // 0AC7..0AC9
                    UnicodeScript::UNKNOWN,                  // 0ACA
                    UnicodeScript::GUJARATI,                 // 0ACB..0ACD
                    UnicodeScript::UNKNOWN,                  // 0ACE..0ACF
                    UnicodeScript::GUJARATI,                 // 0AD0
                    UnicodeScript::UNKNOWN,                  // 0AD1..0ADF
                    UnicodeScript::GUJARATI,                 // 0AE0..0AE3
                    UnicodeScript::UNKNOWN,                  // 0AE4..0AE5
                    UnicodeScript::GUJARATI,                 // 0AE6..0AF1
                    UnicodeScript::UNKNOWN,                  // 0AF2..0AF8
                    UnicodeScript::GUJARATI,                 // 0AF9..0AFF
                    UnicodeScript::UNKNOWN,                  // 0B00
                    UnicodeScript::ORIYA,                    // 0B01..0B03
                    UnicodeScript::UNKNOWN,                  // 0B04
                    UnicodeScript::ORIYA,                    // 0B05..0B0C
                    UnicodeScript::UNKNOWN,                  // 0B0D..0B0E
                    UnicodeScript::ORIYA,                    // 0B0F..0B10
                    UnicodeScript::UNKNOWN,                  // 0B11..0B12
                    UnicodeScript::ORIYA,                    // 0B13..0B28
                    UnicodeScript::UNKNOWN,                  // 0B29
                    UnicodeScript::ORIYA,                    // 0B2A..0B30
                    UnicodeScript::UNKNOWN,                  // 0B31
                    UnicodeScript::ORIYA,                    // 0B32..0B33
                    UnicodeScript::UNKNOWN,                  // 0B34
                    UnicodeScript::ORIYA,                    // 0B35..0B39
                    UnicodeScript::UNKNOWN,                  // 0B3A..0B3B
                    UnicodeScript::ORIYA,                    // 0B3C..0B44
                    UnicodeScript::UNKNOWN,                  // 0B45..0B46
                    UnicodeScript::ORIYA,                    // 0B47..0B48
                    UnicodeScript::UNKNOWN,                  // 0B49..0B4A
                    UnicodeScript::ORIYA,                    // 0B4B..0B4D
                    UnicodeScript::UNKNOWN,                  // 0B4E..0B54
                    UnicodeScript::ORIYA,                    // 0B55..0B57
                    UnicodeScript::UNKNOWN,                  // 0B58..0B5B
                    UnicodeScript::ORIYA,                    // 0B5C..0B5D
                    UnicodeScript::UNKNOWN,                  // 0B5E
                    UnicodeScript::ORIYA,                    // 0B5F..0B63
                    UnicodeScript::UNKNOWN,                  // 0B64..0B65
                    UnicodeScript::ORIYA,                    // 0B66..0B77
                    UnicodeScript::UNKNOWN,                  // 0B78..0B81
                    UnicodeScript::TAMIL,                    // 0B82..0B83
                    UnicodeScript::UNKNOWN,                  // 0B84
                    UnicodeScript::TAMIL,                    // 0B85..0B8A
                    UnicodeScript::UNKNOWN,                  // 0B8B..0B8D
                    UnicodeScript::TAMIL,                    // 0B8E..0B90
                    UnicodeScript::UNKNOWN,                  // 0B91
                    UnicodeScript::TAMIL,                    // 0B92..0B95
                    UnicodeScript::UNKNOWN,                  // 0B96..0B98
                    UnicodeScript::TAMIL,                    // 0B99..0B9A
                    UnicodeScript::UNKNOWN,                  // 0B9B
                    UnicodeScript::TAMIL,                    // 0B9C
                    UnicodeScript::UNKNOWN,                  // 0B9D
                    UnicodeScript::TAMIL,                    // 0B9E..0B9F
                    UnicodeScript::UNKNOWN,                  // 0BA0..0BA2
                    UnicodeScript::TAMIL,                    // 0BA3..0BA4
                    UnicodeScript::UNKNOWN,                  // 0BA5..0BA7
                    UnicodeScript::TAMIL,                    // 0BA8..0BAA
                    UnicodeScript::UNKNOWN,                  // 0BAB..0BAD
                    UnicodeScript::TAMIL,                    // 0BAE..0BB9
                    UnicodeScript::UNKNOWN,                  // 0BBA..0BBD
                    UnicodeScript::TAMIL,                    // 0BBE..0BC2
                    UnicodeScript::UNKNOWN,                  // 0BC3..0BC5
                    UnicodeScript::TAMIL,                    // 0BC6..0BC8
                    UnicodeScript::UNKNOWN,                  // 0BC9
                    UnicodeScript::TAMIL,                    // 0BCA..0BCD
                    UnicodeScript::UNKNOWN,                  // 0BCE..0BCF
                    UnicodeScript::TAMIL,                    // 0BD0
                    UnicodeScript::UNKNOWN,                  // 0BD1..0BD6
                    UnicodeScript::TAMIL,                    // 0BD7
                    UnicodeScript::UNKNOWN,                  // 0BD8..0BE5
                    UnicodeScript::TAMIL,                    // 0BE6..0BFA
                    UnicodeScript::UNKNOWN,                  // 0BFB..0BFF
                    UnicodeScript::TELUGU,                   // 0C00..0C0C
                    UnicodeScript::UNKNOWN,                  // 0C0D
                    UnicodeScript::TELUGU,                   // 0C0E..0C10
                    UnicodeScript::UNKNOWN,                  // 0C11
                    UnicodeScript::TELUGU,                   // 0C12..0C28
                    UnicodeScript::UNKNOWN,                  // 0C29
                    UnicodeScript::TELUGU,                   // 0C2A..0C39
                    UnicodeScript::UNKNOWN,                  // 0C3A..0C3B
                    UnicodeScript::TELUGU,                   // 0C3C..0C44
                    UnicodeScript::UNKNOWN,                  // 0C45
                    UnicodeScript::TELUGU,                   // 0C46..0C48
                    UnicodeScript::UNKNOWN,                  // 0C49
                    UnicodeScript::TELUGU,                   // 0C4A..0C4D
                    UnicodeScript::UNKNOWN,                  // 0C4E..0C54
                    UnicodeScript::TELUGU,                   // 0C55..0C56
                    UnicodeScript::UNKNOWN,                  // 0C57
                    UnicodeScript::TELUGU,                   // 0C58..0C5A
                    UnicodeScript::UNKNOWN,                  // 0C5B..0C5C
                    UnicodeScript::TELUGU,                   // 0C5D
                    UnicodeScript::UNKNOWN,                  // 0C5E..0C5F
                    UnicodeScript::TELUGU,                   // 0C60..0C63
                    UnicodeScript::UNKNOWN,                  // 0C64..0C65
                    UnicodeScript::TELUGU,                   // 0C66..0C6F
                    UnicodeScript::UNKNOWN,                  // 0C70..0C76
                    UnicodeScript::TELUGU,                   // 0C77..0C7F
                    UnicodeScript::KANNADA,                  // 0C80..0C8C
                    UnicodeScript::UNKNOWN,                  // 0C8D
                    UnicodeScript::KANNADA,                  // 0C8E..0C90
                    UnicodeScript::UNKNOWN,                  // 0C91
                    UnicodeScript::KANNADA,                  // 0C92..0CA8
                    UnicodeScript::UNKNOWN,                  // 0CA9
                    UnicodeScript::KANNADA,                  // 0CAA..0CB3
                    UnicodeScript::UNKNOWN,                  // 0CB4
                    UnicodeScript::KANNADA,                  // 0CB5..0CB9
                    UnicodeScript::UNKNOWN,                  // 0CBA..0CBB
                    UnicodeScript::KANNADA,                  // 0CBC..0CC4
                    UnicodeScript::UNKNOWN,                  // 0CC5
                    UnicodeScript::KANNADA,                  // 0CC6..0CC8
                    UnicodeScript::UNKNOWN,                  // 0CC9
                    UnicodeScript::KANNADA,                  // 0CCA..0CCD
                    UnicodeScript::UNKNOWN,                  // 0CCE..0CD4
                    UnicodeScript::KANNADA,                  // 0CD5..0CD6
                    UnicodeScript::UNKNOWN,                  // 0CD7..0CDC
                    UnicodeScript::KANNADA,                  // 0CDD..0CDE
                    UnicodeScript::UNKNOWN,                  // 0CDF
                    UnicodeScript::KANNADA,                  // 0CE0..0CE3
                    UnicodeScript::UNKNOWN,                  // 0CE4..0CE5
                    UnicodeScript::KANNADA,                  // 0CE6..0CEF
                    UnicodeScript::UNKNOWN,                  // 0CF0
                    UnicodeScript::KANNADA,                  // 0CF1..0CF3
                    UnicodeScript::UNKNOWN,                  // 0CF4..0CFF
                    UnicodeScript::MALAYALAM,                // 0D00..0D0C
                    UnicodeScript::UNKNOWN,                  // 0D0D
                    UnicodeScript::MALAYALAM,                // 0D0E..0D10
                    UnicodeScript::UNKNOWN,                  // 0D11
                    UnicodeScript::MALAYALAM,                // 0D12..0D44
                    UnicodeScript::UNKNOWN,                  // 0D45
                    UnicodeScript::MALAYALAM,                // 0D46..0D48
                    UnicodeScript::UNKNOWN,                  // 0D49
                    UnicodeScript::MALAYALAM,                // 0D4A..0D4F
                    UnicodeScript::UNKNOWN,                  // 0D50..0D53
                    UnicodeScript::MALAYALAM,                // 0D54..0D63
                    UnicodeScript::UNKNOWN,                  // 0D64..0D65
                    UnicodeScript::MALAYALAM,                // 0D66..0D7F
                    UnicodeScript::UNKNOWN,                  // 0D80
                    UnicodeScript::SINHALA,                  // 0D81..0D83
                    UnicodeScript::UNKNOWN,                  // 0D84
                    UnicodeScript::SINHALA,                  // 0D85..0D96
                    UnicodeScript::UNKNOWN,                  // 0D97..0D99
                    UnicodeScript::SINHALA,                  // 0D9A..0DB1
                    UnicodeScript::UNKNOWN,                  // 0DB2
                    UnicodeScript::SINHALA,                  // 0DB3..0DBB
                    UnicodeScript::UNKNOWN,                  // 0DBC
                    UnicodeScript::SINHALA,                  // 0DBD
                    UnicodeScript::UNKNOWN,                  // 0DBE..0DBF
                    UnicodeScript::SINHALA,                  // 0DC0..0DC6
                    UnicodeScript::UNKNOWN,                  // 0DC7..0DC9
                    UnicodeScript::SINHALA,                  // 0DCA
                    UnicodeScript::UNKNOWN,                  // 0DCB..0DCE
                    UnicodeScript::SINHALA,                  // 0DCF..0DD4
                    UnicodeScript::UNKNOWN,                  // 0DD5
                    UnicodeScript::SINHALA,                  // 0DD6
                    UnicodeScript::UNKNOWN,                  // 0DD7
                    UnicodeScript::SINHALA,                  // 0DD8..0DDF
                    UnicodeScript::UNKNOWN,                  // 0DE0..0DE5
                    UnicodeScript::SINHALA,                  // 0DE6..0DEF
                    UnicodeScript::UNKNOWN,                  // 0DF0..0DF1
                    UnicodeScript::SINHALA,                  // 0DF2..0DF4
                    UnicodeScript::UNKNOWN,                  // 0DF5..0E00
                    UnicodeScript::THAI,                     // 0E01..0E3A
                    UnicodeScript::UNKNOWN,                  // 0E3B..0E3E
                    UnicodeScript::COMMON,                   // 0E3F
                    UnicodeScript::THAI,                     // 0E40..0E5B
                    UnicodeScript::UNKNOWN,                  // 0E5C..0E80
                    UnicodeScript::LAO,                      // 0E81..0E82
                    UnicodeScript::UNKNOWN,                  // 0E83
                    UnicodeScript::LAO,                      // 0E84
                    UnicodeScript::UNKNOWN,                  // 0E85
                    UnicodeScript::LAO,                      // 0E86..0E8A
                    UnicodeScript::UNKNOWN,                  // 0E8B
                    UnicodeScript::LAO,                      // 0E8C..0EA3
                    UnicodeScript::UNKNOWN,                  // 0EA4
                    UnicodeScript::LAO,                      // 0EA5
                    UnicodeScript::UNKNOWN,                  // 0EA6
                    UnicodeScript::LAO,                      // 0EA7..0EBD
                    UnicodeScript::UNKNOWN,                  // 0EBE..0EBF
                    UnicodeScript::LAO,                      // 0EC0..0EC4
                    UnicodeScript::UNKNOWN,                  // 0EC5
                    UnicodeScript::LAO,                      // 0EC6
                    UnicodeScript::UNKNOWN,                  // 0EC7
                    UnicodeScript::LAO,                      // 0EC8..0ECE
                    UnicodeScript::UNKNOWN,                  // 0ECF
                    UnicodeScript::LAO,                      // 0ED0..0ED9
                    UnicodeScript::UNKNOWN,                  // 0EDA..0EDB
                    UnicodeScript::LAO,                      // 0EDC..0EDF
                    UnicodeScript::UNKNOWN,                  // 0EE0..0EFF
                    UnicodeScript::TIBETAN,                  // 0F00..0F47
                    UnicodeScript::UNKNOWN,                  // 0F48
                    UnicodeScript::TIBETAN,                  // 0F49..0F6C
                    UnicodeScript::UNKNOWN,                  // 0F6D..0F70
                    UnicodeScript::TIBETAN,                  // 0F71..0F97
                    UnicodeScript::UNKNOWN,                  // 0F98
                    UnicodeScript::TIBETAN,                  // 0F99..0FBC
                    UnicodeScript::UNKNOWN,                  // 0FBD
                    UnicodeScript::TIBETAN,                  // 0FBE..0FCC
                    UnicodeScript::UNKNOWN,                  // 0FCD
                    UnicodeScript::TIBETAN,                  // 0FCE..0FD4
                    UnicodeScript::COMMON,                   // 0FD5..0FD8
                    UnicodeScript::TIBETAN,                  // 0FD9..0FDA
                    UnicodeScript::UNKNOWN,                  // 0FDB..0FFF
                    UnicodeScript::MYANMAR,                  // 1000..109F
                    UnicodeScript::GEORGIAN,                 // 10A0..10C5
                    UnicodeScript::UNKNOWN,                  // 10C6
                    UnicodeScript::GEORGIAN,                 // 10C7
                    UnicodeScript::UNKNOWN,                  // 10C8..10CC
                    UnicodeScript::GEORGIAN,                 // 10CD
                    UnicodeScript::UNKNOWN,                  // 10CE..10CF
                    UnicodeScript::GEORGIAN,                 // 10D0..10FA
                    UnicodeScript::COMMON,                   // 10FB
                    UnicodeScript::GEORGIAN,                 // 10FC..10FF
                    UnicodeScript::HANGUL,                   // 1100..11FF
                    UnicodeScript::ETHIOPIC,                 // 1200..1248
                    UnicodeScript::UNKNOWN,                  // 1249
                    UnicodeScript::ETHIOPIC,                 // 124A..124D
                    UnicodeScript::UNKNOWN,                  // 124E..124F
                    UnicodeScript::ETHIOPIC,                 // 1250..1256
                    UnicodeScript::UNKNOWN,                  // 1257
                    UnicodeScript::ETHIOPIC,                 // 1258
                    UnicodeScript::UNKNOWN,                  // 1259
                    UnicodeScript::ETHIOPIC,                 // 125A..125D
                    UnicodeScript::UNKNOWN,                  // 125E..125F
                    UnicodeScript::ETHIOPIC,                 // 1260..1288
                    UnicodeScript::UNKNOWN,                  // 1289
                    UnicodeScript::ETHIOPIC,                 // 128A..128D
                    UnicodeScript::UNKNOWN,                  // 128E..128F
                    UnicodeScript::ETHIOPIC,                 // 1290..12B0
                    UnicodeScript::UNKNOWN,                  // 12B1
                    UnicodeScript::ETHIOPIC,                 // 12B2..12B5
                    UnicodeScript::UNKNOWN,                  // 12B6..12B7
                    UnicodeScript::ETHIOPIC,                 // 12B8..12BE
                    UnicodeScript::UNKNOWN,                  // 12BF
                    UnicodeScript::ETHIOPIC,                 // 12C0
                    UnicodeScript::UNKNOWN,                  // 12C1
                    UnicodeScript::ETHIOPIC,                 // 12C2..12C5
                    UnicodeScript::UNKNOWN,                  // 12C6..12C7
                    UnicodeScript::ETHIOPIC,                 // 12C8..12D6
                    UnicodeScript::UNKNOWN,                  // 12D7
                    UnicodeScript::ETHIOPIC,                 // 12D8..1310
                    UnicodeScript::UNKNOWN,                  // 1311
                    UnicodeScript::ETHIOPIC,                 // 1312..1315
                    UnicodeScript::UNKNOWN,                  // 1316..1317
                    UnicodeScript::ETHIOPIC,                 // 1318..135A
                    UnicodeScript::UNKNOWN,                  // 135B..135C
                    UnicodeScript::ETHIOPIC,                 // 135D..137C
                    UnicodeScript::UNKNOWN,                  // 137D..137F
                    UnicodeScript::ETHIOPIC,                 // 1380..1399
                    UnicodeScript::UNKNOWN,                  // 139A..139F
                    UnicodeScript::CHEROKEE,                 // 13A0..13F5
                    UnicodeScript::UNKNOWN,                  // 13F6..13F7
                    UnicodeScript::CHEROKEE,                 // 13F8..13FD
                    UnicodeScript::UNKNOWN,                  // 13FE..13FF
                    UnicodeScript::CANADIAN_ABORIGINAL,      // 1400..167F
                    UnicodeScript::OGHAM,                    // 1680..169C
                    UnicodeScript::UNKNOWN,                  // 169D..169F
                    UnicodeScript::RUNIC,                    // 16A0..16EA
                    UnicodeScript::COMMON,                   // 16EB..16ED
                    UnicodeScript::RUNIC,                    // 16EE..16F8
                    UnicodeScript::UNKNOWN,                  // 16F9..16FF
                    UnicodeScript::TAGALOG,                  // 1700..1715
                    UnicodeScript::UNKNOWN,                  // 1716..171E
                    UnicodeScript::TAGALOG,                  // 171F
                    UnicodeScript::HANUNOO,                  // 1720..1734
                    UnicodeScript::COMMON,                   // 1735..1736
                    UnicodeScript::UNKNOWN,                  // 1737..173F
                    UnicodeScript::BUHID,                    // 1740..1753
                    UnicodeScript::UNKNOWN,                  // 1754..175F
                    UnicodeScript::TAGBANWA,                 // 1760..176C
                    UnicodeScript::UNKNOWN,                  // 176D
                    UnicodeScript::TAGBANWA,                 // 176E..1770
                    UnicodeScript::UNKNOWN,                  // 1771
                    UnicodeScript::TAGBANWA,                 // 1772..1773
                    UnicodeScript::UNKNOWN,                  // 1774..177F
                    UnicodeScript::KHMER,                    // 1780..17DD
                    UnicodeScript::UNKNOWN,                  // 17DE..17DF
                    UnicodeScript::KHMER,                    // 17E0..17E9
                    UnicodeScript::UNKNOWN,                  // 17EA..17EF
                    UnicodeScript::KHMER,                    // 17F0..17F9
                    UnicodeScript::UNKNOWN,                  // 17FA..17FF
                    UnicodeScript::MONGOLIAN,                // 1800..1801
                    UnicodeScript::COMMON,                   // 1802..1803
                    UnicodeScript::MONGOLIAN,                // 1804
                    UnicodeScript::COMMON,                   // 1805
                    UnicodeScript::MONGOLIAN,                // 1806..1819
                    UnicodeScript::UNKNOWN,                  // 181A..181F
                    UnicodeScript::MONGOLIAN,                // 1820..1878
                    UnicodeScript::UNKNOWN,                  // 1879..187F
                    UnicodeScript::MONGOLIAN,                // 1880..18AA
                    UnicodeScript::UNKNOWN,                  // 18AB..18AF
                    UnicodeScript::CANADIAN_ABORIGINAL,      // 18B0..18F5
                    UnicodeScript::UNKNOWN,                  // 18F6..18FF
                    UnicodeScript::LIMBU,                    // 1900..191E
                    UnicodeScript::UNKNOWN,                  // 191F
                    UnicodeScript::LIMBU,                    // 1920..192B
                    UnicodeScript::UNKNOWN,                  // 192C..192F
                    UnicodeScript::LIMBU,                    // 1930..193B
                    UnicodeScript::UNKNOWN,                  // 193C..193F
                    UnicodeScript::LIMBU,                    // 1940
                    UnicodeScript::UNKNOWN,                  // 1941..1943
                    UnicodeScript::LIMBU,                    // 1944..194F
                    UnicodeScript::TAI_LE,                   // 1950..196D
                    UnicodeScript::UNKNOWN,                  // 196E..196F
                    UnicodeScript::TAI_LE,                   // 1970..1974
                    UnicodeScript::UNKNOWN,                  // 1975..197F
                    UnicodeScript::NEW_TAI_LUE,              // 1980..19AB
                    UnicodeScript::UNKNOWN,                  // 19AC..19AF
                    UnicodeScript::NEW_TAI_LUE,              // 19B0..19C9
                    UnicodeScript::UNKNOWN,                  // 19CA..19CF
                    UnicodeScript::NEW_TAI_LUE,              // 19D0..19DA
                    UnicodeScript::UNKNOWN,                  // 19DB..19DD
                    UnicodeScript::NEW_TAI_LUE,              // 19DE..19DF
                    UnicodeScript::KHMER,                    // 19E0..19FF
                    UnicodeScript::BUGINESE,                 // 1A00..1A1B
                    UnicodeScript::UNKNOWN,                  // 1A1C..1A1D
                    UnicodeScript::BUGINESE,                 // 1A1E..1A1F
                    UnicodeScript::TAI_THAM,                 // 1A20..1A5E
                    UnicodeScript::UNKNOWN,                  // 1A5F
                    UnicodeScript::TAI_THAM,                 // 1A60..1A7C
                    UnicodeScript::UNKNOWN,                  // 1A7D..1A7E
                    UnicodeScript::TAI_THAM,                 // 1A7F..1A89
                    UnicodeScript::UNKNOWN,                  // 1A8A..1A8F
                    UnicodeScript::TAI_THAM,                 // 1A90..1A99
                    UnicodeScript::UNKNOWN,                  // 1A9A..1A9F
                    UnicodeScript::TAI_THAM,                 // 1AA0..1AAD
                    UnicodeScript::UNKNOWN,                  // 1AAE..1AAF
                    UnicodeScript::INHERITED,                // 1AB0..1ACE
                    UnicodeScript::UNKNOWN,                  // 1ACF..1AFF
                    UnicodeScript::BALINESE,                 // 1B00..1B4C
                    UnicodeScript::UNKNOWN,                  // 1B4D..1B4F
                    UnicodeScript::BALINESE,                 // 1B50..1B7E
                    UnicodeScript::UNKNOWN,                  // 1B7F
                    UnicodeScript::SUNDANESE,                // 1B80..1BBF
                    UnicodeScript::BATAK,                    // 1BC0..1BF3
                    UnicodeScript::UNKNOWN,                  // 1BF4..1BFB
                    UnicodeScript::BATAK,                    // 1BFC..1BFF
                    UnicodeScript::LEPCHA,                   // 1C00..1C37
                    UnicodeScript::UNKNOWN,                  // 1C38..1C3A
                    UnicodeScript::LEPCHA,                   // 1C3B..1C49
                    UnicodeScript::UNKNOWN,                  // 1C4A..1C4C
                    UnicodeScript::LEPCHA,                   // 1C4D..1C4F
                    UnicodeScript::OL_CHIKI,                 // 1C50..1C7F
                    UnicodeScript::CYRILLIC,                 // 1C80..1C88
                    UnicodeScript::UNKNOWN,                  // 1C89..1C8F
                    UnicodeScript::GEORGIAN,                 // 1C90..1CBA
                    UnicodeScript::UNKNOWN,                  // 1CBB..1CBC
                    UnicodeScript::GEORGIAN,                 // 1CBD..1CBF
                    UnicodeScript::SUNDANESE,                // 1CC0..1CC7
                    UnicodeScript::UNKNOWN,                  // 1CC8..1CCF
                    UnicodeScript::INHERITED,                // 1CD0..1CD2
                    UnicodeScript::COMMON,                   // 1CD3
                    UnicodeScript::INHERITED,                // 1CD4..1CE0
                    UnicodeScript::COMMON,                   // 1CE1
                    UnicodeScript::INHERITED,                // 1CE2..1CE8
                    UnicodeScript::COMMON,                   // 1CE9..1CEC
                    UnicodeScript::INHERITED,                // 1CED
                    UnicodeScript::COMMON,                   // 1CEE..1CF3
                    UnicodeScript::INHERITED,                // 1CF4
                    UnicodeScript::COMMON,                   // 1CF5..1CF7
                    UnicodeScript::INHERITED,                // 1CF8..1CF9
                    UnicodeScript::COMMON,                   // 1CFA
                    UnicodeScript::UNKNOWN,                  // 1CFB..1CFF
                    UnicodeScript::LATIN,                    // 1D00..1D25
                    UnicodeScript::GREEK,                    // 1D26..1D2A
                    UnicodeScript::CYRILLIC,                 // 1D2B
                    UnicodeScript::LATIN,                    // 1D2C..1D5C
                    UnicodeScript::GREEK,                    // 1D5D..1D61
                    UnicodeScript::LATIN,                    // 1D62..1D65
                    UnicodeScript::GREEK,                    // 1D66..1D6A
                    UnicodeScript::LATIN,                    // 1D6B..1D77
                    UnicodeScript::CYRILLIC,                 // 1D78
                    UnicodeScript::LATIN,                    // 1D79..1DBE
                    UnicodeScript::GREEK,                    // 1DBF
                    UnicodeScript::INHERITED,                // 1DC0..1DFF
                    UnicodeScript::LATIN,                    // 1E00..1EFF
                    UnicodeScript::GREEK,                    // 1F00..1F15
                    UnicodeScript::UNKNOWN,                  // 1F16..1F17
                    UnicodeScript::GREEK,                    // 1F18..1F1D
                    UnicodeScript::UNKNOWN,                  // 1F1E..1F1F
                    UnicodeScript::GREEK,                    // 1F20..1F45
                    UnicodeScript::UNKNOWN,                  // 1F46..1F47
                    UnicodeScript::GREEK,                    // 1F48..1F4D
                    UnicodeScript::UNKNOWN,                  // 1F4E..1F4F
                    UnicodeScript::GREEK,                    // 1F50..1F57
                    UnicodeScript::UNKNOWN,                  // 1F58
                    UnicodeScript::GREEK,                    // 1F59
                    UnicodeScript::UNKNOWN,                  // 1F5A
                    UnicodeScript::GREEK,                    // 1F5B
                    UnicodeScript::UNKNOWN,                  // 1F5C
                    UnicodeScript::GREEK,                    // 1F5D
                    UnicodeScript::UNKNOWN,                  // 1F5E
                    UnicodeScript::GREEK,                    // 1F5F..1F7D
                    UnicodeScript::UNKNOWN,                  // 1F7E..1F7F
                    UnicodeScript::GREEK,                    // 1F80..1FB4
                    UnicodeScript::UNKNOWN,                  // 1FB5
                    UnicodeScript::GREEK,                    // 1FB6..1FC4
                    UnicodeScript::UNKNOWN,                  // 1FC5
                    UnicodeScript::GREEK,                    // 1FC6..1FD3
                    UnicodeScript::UNKNOWN,                  // 1FD4..1FD5
                    UnicodeScript::GREEK,                    // 1FD6..1FDB
                    UnicodeScript::UNKNOWN,                  // 1FDC
                    UnicodeScript::GREEK,                    // 1FDD..1FEF
                    UnicodeScript::UNKNOWN,                  // 1FF0..1FF1
                    UnicodeScript::GREEK,                    // 1FF2..1FF4
                    UnicodeScript::UNKNOWN,                  // 1FF5
                    UnicodeScript::GREEK,                    // 1FF6..1FFE
                    UnicodeScript::UNKNOWN,                  // 1FFF
                    UnicodeScript::COMMON,                   // 2000..200B
                    UnicodeScript::INHERITED,                // 200C..200D
                    UnicodeScript::COMMON,                   // 200E..2064
                    UnicodeScript::UNKNOWN,                  // 2065
                    UnicodeScript::COMMON,                   // 2066..2070
                    UnicodeScript::LATIN,                    // 2071
                    UnicodeScript::UNKNOWN,                  // 2072..2073
                    UnicodeScript::COMMON,                   // 2074..207E
                    UnicodeScript::LATIN,                    // 207F
                    UnicodeScript::COMMON,                   // 2080..208E
                    UnicodeScript::UNKNOWN,                  // 208F
                    UnicodeScript::LATIN,                    // 2090..209C
                    UnicodeScript::UNKNOWN,                  // 209D..209F
                    UnicodeScript::COMMON,                   // 20A0..20C0
                    UnicodeScript::UNKNOWN,                  // 20C1..20CF
                    UnicodeScript::INHERITED,                // 20D0..20F0
                    UnicodeScript::UNKNOWN,                  // 20F1..20FF
                    UnicodeScript::COMMON,                   // 2100..2125
                    UnicodeScript::GREEK,                    // 2126
                    UnicodeScript::COMMON,                   // 2127..2129
                    UnicodeScript::LATIN,                    // 212A..212B
                    UnicodeScript::COMMON,                   // 212C..2131
                    UnicodeScript::LATIN,                    // 2132
                    UnicodeScript::COMMON,                   // 2133..214D
                    UnicodeScript::LATIN,                    // 214E
                    UnicodeScript::COMMON,                   // 214F..215F
                    UnicodeScript::LATIN,                    // 2160..2188
                    UnicodeScript::COMMON,                   // 2189..218B
                    UnicodeScript::UNKNOWN,                  // 218C..218F
                    UnicodeScript::COMMON,                   // 2190..2426
                    UnicodeScript::UNKNOWN,                  // 2427..243F
                    UnicodeScript::COMMON,                   // 2440..244A
                    UnicodeScript::UNKNOWN,                  // 244B..245F
                    UnicodeScript::COMMON,                   // 2460..27FF
                    UnicodeScript::BRAILLE,                  // 2800..28FF
                    UnicodeScript::COMMON,                   // 2900..2B73
                    UnicodeScript::UNKNOWN,                  // 2B74..2B75
                    UnicodeScript::COMMON,                   // 2B76..2B95
                    UnicodeScript::UNKNOWN,                  // 2B96
                    UnicodeScript::COMMON,                   // 2B97..2BFF
                    UnicodeScript::GLAGOLITIC,               // 2C00..2C5F
                    UnicodeScript::LATIN,                    // 2C60..2C7F
                    UnicodeScript::COPTIC,                   // 2C80..2CF3
                    UnicodeScript::UNKNOWN,                  // 2CF4..2CF8
                    UnicodeScript::COPTIC,                   // 2CF9..2CFF
                    UnicodeScript::GEORGIAN,                 // 2D00..2D25
                    UnicodeScript::UNKNOWN,                  // 2D26
                    UnicodeScript::GEORGIAN,                 // 2D27
                    UnicodeScript::UNKNOWN,                  // 2D28..2D2C
                    UnicodeScript::GEORGIAN,                 // 2D2D
                    UnicodeScript::UNKNOWN,                  // 2D2E..2D2F
                    UnicodeScript::TIFINAGH,                 // 2D30..2D67
                    UnicodeScript::UNKNOWN,                  // 2D68..2D6E
                    UnicodeScript::TIFINAGH,                 // 2D6F..2D70
                    UnicodeScript::UNKNOWN,                  // 2D71..2D7E
                    UnicodeScript::TIFINAGH,                 // 2D7F
                    UnicodeScript::ETHIOPIC,                 // 2D80..2D96
                    UnicodeScript::UNKNOWN,                  // 2D97..2D9F
                    UnicodeScript::ETHIOPIC,                 // 2DA0..2DA6
                    UnicodeScript::UNKNOWN,                  // 2DA7
                    UnicodeScript::ETHIOPIC,                 // 2DA8..2DAE
                    UnicodeScript::UNKNOWN,                  // 2DAF
                    UnicodeScript::ETHIOPIC,                 // 2DB0..2DB6
                    UnicodeScript::UNKNOWN,                  // 2DB7
                    UnicodeScript::ETHIOPIC,                 // 2DB8..2DBE
                    UnicodeScript::UNKNOWN,                  // 2DBF
                    UnicodeScript::ETHIOPIC,                 // 2DC0..2DC6
                    UnicodeScript::UNKNOWN,                  // 2DC7
                    UnicodeScript::ETHIOPIC,                 // 2DC8..2DCE
                    UnicodeScript::UNKNOWN,                  // 2DCF
                    UnicodeScript::ETHIOPIC,                 // 2DD0..2DD6
                    UnicodeScript::UNKNOWN,                  // 2DD7
                    UnicodeScript::ETHIOPIC,                 // 2DD8..2DDE
                    UnicodeScript::UNKNOWN,                  // 2DDF
                    UnicodeScript::CYRILLIC,                 // 2DE0..2DFF
                    UnicodeScript::COMMON,                   // 2E00..2E5D
                    UnicodeScript::UNKNOWN,                  // 2E5E..2E7F
                    UnicodeScript::HAN,                      // 2E80..2E99
                    UnicodeScript::UNKNOWN,                  // 2E9A
                    UnicodeScript::HAN,                      // 2E9B..2EF3
                    UnicodeScript::UNKNOWN,                  // 2EF4..2EFF
                    UnicodeScript::HAN,                      // 2F00..2FD5
                    UnicodeScript::UNKNOWN,                  // 2FD6..2FEF
                    UnicodeScript::COMMON,                   // 2FF0..2FFB
                    UnicodeScript::UNKNOWN,                  // 2FFC..2FFF
                    UnicodeScript::COMMON,                   // 3000..3004
                    UnicodeScript::HAN,                      // 3005
                    UnicodeScript::COMMON,                   // 3006
                    UnicodeScript::HAN,                      // 3007
                    UnicodeScript::COMMON,                   // 3008..3020
                    UnicodeScript::HAN,                      // 3021..3029
                    UnicodeScript::INHERITED,                // 302A..302D
                    UnicodeScript::HANGUL,                   // 302E..302F
                    UnicodeScript::COMMON,                   // 3030..3037
                    UnicodeScript::HAN,                      // 3038..303B
                    UnicodeScript::COMMON,                   // 303C..303F
                    UnicodeScript::UNKNOWN,                  // 3040
                    UnicodeScript::HIRAGANA,                 // 3041..3096
                    UnicodeScript::UNKNOWN,                  // 3097..3098
                    UnicodeScript::INHERITED,                // 3099..309A
                    UnicodeScript::COMMON,                   // 309B..309C
                    UnicodeScript::HIRAGANA,                 // 309D..309F
                    UnicodeScript::COMMON,                   // 30A0
                    UnicodeScript::KATAKANA,                 // 30A1..30FA
                    UnicodeScript::COMMON,                   // 30FB..30FC
                    UnicodeScript::KATAKANA,                 // 30FD..30FF
                    UnicodeScript::UNKNOWN,                  // 3100..3104
                    UnicodeScript::BOPOMOFO,                 // 3105..312F
                    UnicodeScript::UNKNOWN,                  // 3130
                    UnicodeScript::HANGUL,                   // 3131..318E
                    UnicodeScript::UNKNOWN,                  // 318F
                    UnicodeScript::COMMON,                   // 3190..319F
                    UnicodeScript::BOPOMOFO,                 // 31A0..31BF
                    UnicodeScript::COMMON,                   // 31C0..31E3
                    UnicodeScript::UNKNOWN,                  // 31E4..31EF
                    UnicodeScript::KATAKANA,                 // 31F0..31FF
                    UnicodeScript::HANGUL,                   // 3200..321E
                    UnicodeScript::UNKNOWN,                  // 321F
                    UnicodeScript::COMMON,                   // 3220..325F
                    UnicodeScript::HANGUL,                   // 3260..327E
                    UnicodeScript::COMMON,                   // 327F..32CF
                    UnicodeScript::KATAKANA,                 // 32D0..32FE
                    UnicodeScript::COMMON,                   // 32FF
                    UnicodeScript::KATAKANA,                 // 3300..3357
                    UnicodeScript::COMMON,                   // 3358..33FF
                    UnicodeScript::HAN,                      // 3400..4DBF
                    UnicodeScript::COMMON,                   // 4DC0..4DFF
                    UnicodeScript::HAN,                      // 4E00..9FFF
                    UnicodeScript::YI,                       // A000..A48C
                    UnicodeScript::UNKNOWN,                  // A48D..A48F
                    UnicodeScript::YI,                       // A490..A4C6
                    UnicodeScript::UNKNOWN,                  // A4C7..A4CF
                    UnicodeScript::LISU,                     // A4D0..A4FF
                    UnicodeScript::VAI,                      // A500..A62B
                    UnicodeScript::UNKNOWN,                  // A62C..A63F
                    UnicodeScript::CYRILLIC,                 // A640..A69F
                    UnicodeScript::BAMUM,                    // A6A0..A6F7
                    UnicodeScript::UNKNOWN,                  // A6F8..A6FF
                    UnicodeScript::COMMON,                   // A700..A721
                    UnicodeScript::LATIN,                    // A722..A787
                    UnicodeScript::COMMON,                   // A788..A78A
                    UnicodeScript::LATIN,                    // A78B..A7CA
                    UnicodeScript::UNKNOWN,                  // A7CB..A7CF
                    UnicodeScript::LATIN,                    // A7D0..A7D1
                    UnicodeScript::UNKNOWN,                  // A7D2
                    UnicodeScript::LATIN,                    // A7D3
                    UnicodeScript::UNKNOWN,                  // A7D4
                    UnicodeScript::LATIN,                    // A7D5..A7D9
                    UnicodeScript::UNKNOWN,                  // A7DA..A7F1
                    UnicodeScript::LATIN,                    // A7F2..A7FF
                    UnicodeScript::SYLOTI_NAGRI,             // A800..A82C
                    UnicodeScript::UNKNOWN,                  // A82D..A82F
                    UnicodeScript::COMMON,                   // A830..A839
                    UnicodeScript::UNKNOWN,                  // A83A..A83F
                    UnicodeScript::PHAGS_PA,                 // A840..A877
                    UnicodeScript::UNKNOWN,                  // A878..A87F
                    UnicodeScript::SAURASHTRA,               // A880..A8C5
                    UnicodeScript::UNKNOWN,                  // A8C6..A8CD
                    UnicodeScript::SAURASHTRA,               // A8CE..A8D9
                    UnicodeScript::UNKNOWN,                  // A8DA..A8DF
                    UnicodeScript::DEVANAGARI,               // A8E0..A8FF
                    UnicodeScript::KAYAH_LI,                 // A900..A92D
                    UnicodeScript::COMMON,                   // A92E
                    UnicodeScript::KAYAH_LI,                 // A92F
                    UnicodeScript::REJANG,                   // A930..A953
                    UnicodeScript::UNKNOWN,                  // A954..A95E
                    UnicodeScript::REJANG,                   // A95F
                    UnicodeScript::HANGUL,                   // A960..A97C
                    UnicodeScript::UNKNOWN,                  // A97D..A97F
                    UnicodeScript::JAVANESE,                 // A980..A9CD
                    UnicodeScript::UNKNOWN,                  // A9CE
                    UnicodeScript::COMMON,                   // A9CF
                    UnicodeScript::JAVANESE,                 // A9D0..A9D9
                    UnicodeScript::UNKNOWN,                  // A9DA..A9DD
                    UnicodeScript::JAVANESE,                 // A9DE..A9DF
                    UnicodeScript::MYANMAR,                  // A9E0..A9FE
                    UnicodeScript::UNKNOWN,                  // A9FF
                    UnicodeScript::CHAM,                     // AA00..AA36
                    UnicodeScript::UNKNOWN,                  // AA37..AA3F
                    UnicodeScript::CHAM,                     // AA40..AA4D
                    UnicodeScript::UNKNOWN,                  // AA4E..AA4F
                    UnicodeScript::CHAM,                     // AA50..AA59
                    UnicodeScript::UNKNOWN,                  // AA5A..AA5B
                    UnicodeScript::CHAM,                     // AA5C..AA5F
                    UnicodeScript::MYANMAR,                  // AA60..AA7F
                    UnicodeScript::TAI_VIET,                 // AA80..AAC2
                    UnicodeScript::UNKNOWN,                  // AAC3..AADA
                    UnicodeScript::TAI_VIET,                 // AADB..AADF
                    UnicodeScript::MEETEI_MAYEK,             // AAE0..AAF6
                    UnicodeScript::UNKNOWN,                  // AAF7..AB00
                    UnicodeScript::ETHIOPIC,                 // AB01..AB06
                    UnicodeScript::UNKNOWN,                  // AB07..AB08
                    UnicodeScript::ETHIOPIC,                 // AB09..AB0E
                    UnicodeScript::UNKNOWN,                  // AB0F..AB10
                    UnicodeScript::ETHIOPIC,                 // AB11..AB16
                    UnicodeScript::UNKNOWN,                  // AB17..AB1F
                    UnicodeScript::ETHIOPIC,                 // AB20..AB26
                    UnicodeScript::UNKNOWN,                  // AB27
                    UnicodeScript::ETHIOPIC,                 // AB28..AB2E
                    UnicodeScript::UNKNOWN,                  // AB2F
                    UnicodeScript::LATIN,                    // AB30..AB5A
                    UnicodeScript::COMMON,                   // AB5B
                    UnicodeScript::LATIN,                    // AB5C..AB64
                    UnicodeScript::GREEK,                    // AB65
                    UnicodeScript::LATIN,                    // AB66..AB69
                    UnicodeScript::COMMON,                   // AB6A..AB6B
                    UnicodeScript::UNKNOWN,                  // AB6C..AB6F
                    UnicodeScript::CHEROKEE,                 // AB70..ABBF
                    UnicodeScript::MEETEI_MAYEK,             // ABC0..ABED
                    UnicodeScript::UNKNOWN,                  // ABEE..ABEF
                    UnicodeScript::MEETEI_MAYEK,             // ABF0..ABF9
                    UnicodeScript::UNKNOWN,                  // ABFA..ABFF
                    UnicodeScript::HANGUL,                   // AC00..D7A3
                    UnicodeScript::UNKNOWN,                  // D7A4..D7AF
                    UnicodeScript::HANGUL,                   // D7B0..D7C6
                    UnicodeScript::UNKNOWN,                  // D7C7..D7CA
                    UnicodeScript::HANGUL,                   // D7CB..D7FB
                    UnicodeScript::UNKNOWN,                  // D7FC..F8FF
                    UnicodeScript::HAN,                      // F900..FA6D
                    UnicodeScript::UNKNOWN,                  // FA6E..FA6F
                    UnicodeScript::HAN,                      // FA70..FAD9
                    UnicodeScript::UNKNOWN,                  // FADA..FAFF
                    UnicodeScript::LATIN,                    // FB00..FB06
                    UnicodeScript::UNKNOWN,                  // FB07..FB12
                    UnicodeScript::ARMENIAN,                 // FB13..FB17
                    UnicodeScript::UNKNOWN,                  // FB18..FB1C
                    UnicodeScript::HEBREW,                   // FB1D..FB36
                    UnicodeScript::UNKNOWN,                  // FB37
                    UnicodeScript::HEBREW,                   // FB38..FB3C
                    UnicodeScript::UNKNOWN,                  // FB3D
                    UnicodeScript::HEBREW,                   // FB3E
                    UnicodeScript::UNKNOWN,                  // FB3F
                    UnicodeScript::HEBREW,                   // FB40..FB41
                    UnicodeScript::UNKNOWN,                  // FB42
                    UnicodeScript::HEBREW,                   // FB43..FB44
                    UnicodeScript::UNKNOWN,                  // FB45
                    UnicodeScript::HEBREW,                   // FB46..FB4F
                    UnicodeScript::ARABIC,                   // FB50..FBC2
                    UnicodeScript::UNKNOWN,                  // FBC3..FBD2
                    UnicodeScript::ARABIC,                   // FBD3..FD3D
                    UnicodeScript::COMMON,                   // FD3E..FD3F
                    UnicodeScript::ARABIC,                   // FD40..FD8F
                    UnicodeScript::UNKNOWN,                  // FD90..FD91
                    UnicodeScript::ARABIC,                   // FD92..FDC7
                    UnicodeScript::UNKNOWN,                  // FDC8..FDCE
                    UnicodeScript::ARABIC,                   // FDCF
                    UnicodeScript::UNKNOWN,                  // FDD0..FDEF
                    UnicodeScript::ARABIC,                   // FDF0..FDFF
                    UnicodeScript::INHERITED,                // FE00..FE0F
                    UnicodeScript::COMMON,                   // FE10..FE19
                    UnicodeScript::UNKNOWN,                  // FE1A..FE1F
                    UnicodeScript::INHERITED,                // FE20..FE2D
                    UnicodeScript::CYRILLIC,                 // FE2E..FE2F
                    UnicodeScript::COMMON,                   // FE30..FE52
                    UnicodeScript::UNKNOWN,                  // FE53
                    UnicodeScript::COMMON,                   // FE54..FE66
                    UnicodeScript::UNKNOWN,                  // FE67
                    UnicodeScript::COMMON,                   // FE68..FE6B
                    UnicodeScript::UNKNOWN,                  // FE6C..FE6F
                    UnicodeScript::ARABIC,                   // FE70..FE74
                    UnicodeScript::UNKNOWN,                  // FE75
                    UnicodeScript::ARABIC,                   // FE76..FEFC
                    UnicodeScript::UNKNOWN,                  // FEFD..FEFE
                    UnicodeScript::COMMON,                   // FEFF
                    UnicodeScript::UNKNOWN,                  // FF00
                    UnicodeScript::COMMON,                   // FF01..FF20
                    UnicodeScript::LATIN,                    // FF21..FF3A
                    UnicodeScript::COMMON,                   // FF3B..FF40
                    UnicodeScript::LATIN,                    // FF41..FF5A
                    UnicodeScript::COMMON,                   // FF5B..FF65
                    UnicodeScript::KATAKANA,                 // FF66..FF6F
                    UnicodeScript::COMMON,                   // FF70
                    UnicodeScript::KATAKANA,                 // FF71..FF9D
                    UnicodeScript::COMMON,                   // FF9E..FF9F
                    UnicodeScript::HANGUL,                   // FFA0..FFBE
                    UnicodeScript::UNKNOWN,                  // FFBF..FFC1
                    UnicodeScript::HANGUL,                   // FFC2..FFC7
                    UnicodeScript::UNKNOWN,                  // FFC8..FFC9
                    UnicodeScript::HANGUL,                   // FFCA..FFCF
                    UnicodeScript::UNKNOWN,                  // FFD0..FFD1
                    UnicodeScript::HANGUL,                   // FFD2..FFD7
                    UnicodeScript::UNKNOWN,                  // FFD8..FFD9
                    UnicodeScript::HANGUL,                   // FFDA..FFDC
                    UnicodeScript::UNKNOWN,                  // FFDD..FFDF
                    UnicodeScript::COMMON,                   // FFE0..FFE6
                    UnicodeScript::UNKNOWN,                  // FFE7
                    UnicodeScript::COMMON,                   // FFE8..FFEE
                    UnicodeScript::UNKNOWN,                  // FFEF..FFF8
                    UnicodeScript::COMMON,                   // FFF9..FFFD
                    UnicodeScript::UNKNOWN,                  // FFFE..FFFF
                    UnicodeScript::LINEAR_B,                 // 10000..1000B
                    UnicodeScript::UNKNOWN,                  // 1000C
                    UnicodeScript::LINEAR_B,                 // 1000D..10026
                    UnicodeScript::UNKNOWN,                  // 10027
                    UnicodeScript::LINEAR_B,                 // 10028..1003A
                    UnicodeScript::UNKNOWN,                  // 1003B
                    UnicodeScript::LINEAR_B,                 // 1003C..1003D
                    UnicodeScript::UNKNOWN,                  // 1003E
                    UnicodeScript::LINEAR_B,                 // 1003F..1004D
                    UnicodeScript::UNKNOWN,                  // 1004E..1004F
                    UnicodeScript::LINEAR_B,                 // 10050..1005D
                    UnicodeScript::UNKNOWN,                  // 1005E..1007F
                    UnicodeScript::LINEAR_B,                 // 10080..100FA
                    UnicodeScript::UNKNOWN,                  // 100FB..100FF
                    UnicodeScript::COMMON,                   // 10100..10102
                    UnicodeScript::UNKNOWN,                  // 10103..10106
                    UnicodeScript::COMMON,                   // 10107..10133
                    UnicodeScript::UNKNOWN,                  // 10134..10136
                    UnicodeScript::COMMON,                   // 10137..1013F
                    UnicodeScript::GREEK,                    // 10140..1018E
                    UnicodeScript::UNKNOWN,                  // 1018F
                    UnicodeScript::COMMON,                   // 10190..1019C
                    UnicodeScript::UNKNOWN,                  // 1019D..1019F
                    UnicodeScript::GREEK,                    // 101A0
                    UnicodeScript::UNKNOWN,                  // 101A1..101CF
                    UnicodeScript::COMMON,                   // 101D0..101FC
                    UnicodeScript::INHERITED,                // 101FD
                    UnicodeScript::UNKNOWN,                  // 101FE..1027F
                    UnicodeScript::LYCIAN,                   // 10280..1029C
                    UnicodeScript::UNKNOWN,                  // 1029D..1029F
                    UnicodeScript::CARIAN,                   // 102A0..102D0
                    UnicodeScript::UNKNOWN,                  // 102D1..102DF
                    UnicodeScript::INHERITED,                // 102E0
                    UnicodeScript::COMMON,                   // 102E1..102FB
                    UnicodeScript::UNKNOWN,                  // 102FC..102FF
                    UnicodeScript::OLD_ITALIC,               // 10300..10323
                    UnicodeScript::UNKNOWN,                  // 10324..1032C
                    UnicodeScript::OLD_ITALIC,               // 1032D..1032F
                    UnicodeScript::GOTHIC,                   // 10330..1034A
                    UnicodeScript::UNKNOWN,                  // 1034B..1034F
                    UnicodeScript::OLD_PERMIC,               // 10350..1037A
                    UnicodeScript::UNKNOWN,                  // 1037B..1037F
                    UnicodeScript::UGARITIC,                 // 10380..1039D
                    UnicodeScript::UNKNOWN,                  // 1039E
                    UnicodeScript::UGARITIC,                 // 1039F
                    UnicodeScript::OLD_PERSIAN,              // 103A0..103C3
                    UnicodeScript::UNKNOWN,                  // 103C4..103C7
                    UnicodeScript::OLD_PERSIAN,              // 103C8..103D5
                    UnicodeScript::UNKNOWN,                  // 103D6..103FF
                    UnicodeScript::DESERET,                  // 10400..1044F
                    UnicodeScript::SHAVIAN,                  // 10450..1047F
                    UnicodeScript::OSMANYA,                  // 10480..1049D
                    UnicodeScript::UNKNOWN,                  // 1049E..1049F
                    UnicodeScript::OSMANYA,                  // 104A0..104A9
                    UnicodeScript::UNKNOWN,                  // 104AA..104AF
                    UnicodeScript::OSAGE,                    // 104B0..104D3
                    UnicodeScript::UNKNOWN,                  // 104D4..104D7
                    UnicodeScript::OSAGE,                    // 104D8..104FB
                    UnicodeScript::UNKNOWN,                  // 104FC..104FF
                    UnicodeScript::ELBASAN,                  // 10500..10527
                    UnicodeScript::UNKNOWN,                  // 10528..1052F
                    UnicodeScript::CAUCASIAN_ALBANIAN,       // 10530..10563
                    UnicodeScript::UNKNOWN,                  // 10564..1056E
                    UnicodeScript::CAUCASIAN_ALBANIAN,       // 1056F
                    UnicodeScript::VITHKUQI,                 // 10570..1057A
                    UnicodeScript::UNKNOWN,                  // 1057B
                    UnicodeScript::VITHKUQI,                 // 1057C..1058A
                    UnicodeScript::UNKNOWN,                  // 1058B
                    UnicodeScript::VITHKUQI,                 // 1058C..10592
                    UnicodeScript::UNKNOWN,                  // 10593
                    UnicodeScript::VITHKUQI,                 // 10594..10595
                    UnicodeScript::UNKNOWN,                  // 10596
                    UnicodeScript::VITHKUQI,                 // 10597..105A1
                    UnicodeScript::UNKNOWN,                  // 105A2
                    UnicodeScript::VITHKUQI,                 // 105A3..105B1
                    UnicodeScript::UNKNOWN,                  // 105B2
                    UnicodeScript::VITHKUQI,                 // 105B3..105B9
                    UnicodeScript::UNKNOWN,                  // 105BA
                    UnicodeScript::VITHKUQI,                 // 105BB..105BC
                    UnicodeScript::UNKNOWN,                  // 105BD..105FF
                    UnicodeScript::LINEAR_A,                 // 10600..10736
                    UnicodeScript::UNKNOWN,                  // 10737..1073F
                    UnicodeScript::LINEAR_A,                 // 10740..10755
                    UnicodeScript::UNKNOWN,                  // 10756..1075F
                    UnicodeScript::LINEAR_A,                 // 10760..10767
                    UnicodeScript::UNKNOWN,                  // 10768..1077F
                    UnicodeScript::LATIN,                    // 10780..10785
                    UnicodeScript::UNKNOWN,                  // 10786
                    UnicodeScript::LATIN,                    // 10787..107B0
                    UnicodeScript::UNKNOWN,                  // 107B1
                    UnicodeScript::LATIN,                    // 107B2..107BA
                    UnicodeScript::UNKNOWN,                  // 107BB..107FF
                    UnicodeScript::CYPRIOT,                  // 10800..10805
                    UnicodeScript::UNKNOWN,                  // 10806..10807
                    UnicodeScript::CYPRIOT,                  // 10808
                    UnicodeScript::UNKNOWN,                  // 10809
                    UnicodeScript::CYPRIOT,                  // 1080A..10835
                    UnicodeScript::UNKNOWN,                  // 10836
                    UnicodeScript::CYPRIOT,                  // 10837..10838
                    UnicodeScript::UNKNOWN,                  // 10839..1083B
                    UnicodeScript::CYPRIOT,                  // 1083C
                    UnicodeScript::UNKNOWN,                  // 1083D..1083E
                    UnicodeScript::CYPRIOT,                  // 1083F
                    UnicodeScript::IMPERIAL_ARAMAIC,         // 10840..10855
                    UnicodeScript::UNKNOWN,                  // 10856
                    UnicodeScript::IMPERIAL_ARAMAIC,         // 10857..1085F
                    UnicodeScript::PALMYRENE,                // 10860..1087F
                    UnicodeScript::NABATAEAN,                // 10880..1089E
                    UnicodeScript::UNKNOWN,                  // 1089F..108A6
                    UnicodeScript::NABATAEAN,                // 108A7..108AF
                    UnicodeScript::UNKNOWN,                  // 108B0..108DF
                    UnicodeScript::HATRAN,                   // 108E0..108F2
                    UnicodeScript::UNKNOWN,                  // 108F3
                    UnicodeScript::HATRAN,                   // 108F4..108F5
                    UnicodeScript::UNKNOWN,                  // 108F6..108FA
                    UnicodeScript::HATRAN,                   // 108FB..108FF
                    UnicodeScript::PHOENICIAN,               // 10900..1091B
                    UnicodeScript::UNKNOWN,                  // 1091C..1091E
                    UnicodeScript::PHOENICIAN,               // 1091F
                    UnicodeScript::LYDIAN,                   // 10920..10939
                    UnicodeScript::UNKNOWN,                  // 1093A..1093E
                    UnicodeScript::LYDIAN,                   // 1093F
                    UnicodeScript::UNKNOWN,                  // 10940..1097F
                    UnicodeScript::MEROITIC_HIEROGLYPHS,     // 10980..1099F
                    UnicodeScript::MEROITIC_CURSIVE,         // 109A0..109B7
                    UnicodeScript::UNKNOWN,                  // 109B8..109BB
                    UnicodeScript::MEROITIC_CURSIVE,         // 109BC..109CF
                    UnicodeScript::UNKNOWN,                  // 109D0..109D1
                    UnicodeScript::MEROITIC_CURSIVE,         // 109D2..109FF
                    UnicodeScript::KHAROSHTHI,               // 10A00..10A03
                    UnicodeScript::UNKNOWN,                  // 10A04
                    UnicodeScript::KHAROSHTHI,               // 10A05..10A06
                    UnicodeScript::UNKNOWN,                  // 10A07..10A0B
                    UnicodeScript::KHAROSHTHI,               // 10A0C..10A13
                    UnicodeScript::UNKNOWN,                  // 10A14
                    UnicodeScript::KHAROSHTHI,               // 10A15..10A17
                    UnicodeScript::UNKNOWN,                  // 10A18
                    UnicodeScript::KHAROSHTHI,               // 10A19..10A35
                    UnicodeScript::UNKNOWN,                  // 10A36..10A37
                    UnicodeScript::KHAROSHTHI,               // 10A38..10A3A
                    UnicodeScript::UNKNOWN,                  // 10A3B..10A3E
                    UnicodeScript::KHAROSHTHI,               // 10A3F..10A48
                    UnicodeScript::UNKNOWN,                  // 10A49..10A4F
                    UnicodeScript::KHAROSHTHI,               // 10A50..10A58
                    UnicodeScript::UNKNOWN,                  // 10A59..10A5F
                    UnicodeScript::OLD_SOUTH_ARABIAN,        // 10A60..10A7F
                    UnicodeScript::OLD_NORTH_ARABIAN,        // 10A80..10A9F
                    UnicodeScript::UNKNOWN,                  // 10AA0..10ABF
                    UnicodeScript::MANICHAEAN,               // 10AC0..10AE6
                    UnicodeScript::UNKNOWN,                  // 10AE7..10AEA
                    UnicodeScript::MANICHAEAN,               // 10AEB..10AF6
                    UnicodeScript::UNKNOWN,                  // 10AF7..10AFF
                    UnicodeScript::AVESTAN,                  // 10B00..10B35
                    UnicodeScript::UNKNOWN,                  // 10B36..10B38
                    UnicodeScript::AVESTAN,                  // 10B39..10B3F
                    UnicodeScript::INSCRIPTIONAL_PARTHIAN,   // 10B40..10B55
                    UnicodeScript::UNKNOWN,                  // 10B56..10B57
                    UnicodeScript::INSCRIPTIONAL_PARTHIAN,   // 10B58..10B5F
                    UnicodeScript::INSCRIPTIONAL_PAHLAVI,    // 10B60..10B72
                    UnicodeScript::UNKNOWN,                  // 10B73..10B77
                    UnicodeScript::INSCRIPTIONAL_PAHLAVI,    // 10B78..10B7F
                    UnicodeScript::PSALTER_PAHLAVI,          // 10B80..10B91
                    UnicodeScript::UNKNOWN,                  // 10B92..10B98
                    UnicodeScript::PSALTER_PAHLAVI,          // 10B99..10B9C
                    UnicodeScript::UNKNOWN,                  // 10B9D..10BA8
                    UnicodeScript::PSALTER_PAHLAVI,          // 10BA9..10BAF
                    UnicodeScript::UNKNOWN,                  // 10BB0..10BFF
                    UnicodeScript::OLD_TURKIC,               // 10C00..10C48
                    UnicodeScript::UNKNOWN,                  // 10C49..10C7F
                    UnicodeScript::OLD_HUNGARIAN,            // 10C80..10CB2
                    UnicodeScript::UNKNOWN,                  // 10CB3..10CBF
                    UnicodeScript::OLD_HUNGARIAN,            // 10CC0..10CF2
                    UnicodeScript::UNKNOWN,                  // 10CF3..10CF9
                    UnicodeScript::OLD_HUNGARIAN,            // 10CFA..10CFF
                    UnicodeScript::HANIFI_ROHINGYA,          // 10D00..10D27
                    UnicodeScript::UNKNOWN,                  // 10D28..10D2F
                    UnicodeScript::HANIFI_ROHINGYA,          // 10D30..10D39
                    UnicodeScript::UNKNOWN,                  // 10D3A..10E5F
                    UnicodeScript::ARABIC,                   // 10E60..10E7E
                    UnicodeScript::UNKNOWN,                  // 10E7F
                    UnicodeScript::YEZIDI,                   // 10E80..10EA9
                    UnicodeScript::UNKNOWN,                  // 10EAA
                    UnicodeScript::YEZIDI,                   // 10EAB..10EAD
                    UnicodeScript::UNKNOWN,                  // 10EAE..10EAF
                    UnicodeScript::YEZIDI,                   // 10EB0..10EB1
                    UnicodeScript::UNKNOWN,                  // 10EB2..10EFC
                    UnicodeScript::ARABIC,                   // 10EFD..10EFF
                    UnicodeScript::OLD_SOGDIAN,              // 10F00..10F27
                    UnicodeScript::UNKNOWN,                  // 10F28..10F2F
                    UnicodeScript::SOGDIAN,                  // 10F30..10F59
                    UnicodeScript::UNKNOWN,                  // 10F5A..10F6F
                    UnicodeScript::OLD_UYGHUR,               // 10F70..10F89
                    UnicodeScript::UNKNOWN,                  // 10F8A..10FAF
                    UnicodeScript::CHORASMIAN,               // 10FB0..10FCB
                    UnicodeScript::UNKNOWN,                  // 10FCC..10FDF
                    UnicodeScript::ELYMAIC,                  // 10FE0..10FF6
                    UnicodeScript::UNKNOWN,                  // 10FF7..10FFF
                    UnicodeScript::BRAHMI,                   // 11000..1104D
                    UnicodeScript::UNKNOWN,                  // 1104E..11051
                    UnicodeScript::BRAHMI,                   // 11052..11075
                    UnicodeScript::UNKNOWN,                  // 11076..1107E
                    UnicodeScript::BRAHMI,                   // 1107F
                    UnicodeScript::KAITHI,                   // 11080..110C2
                    UnicodeScript::UNKNOWN,                  // 110C3..110CC
                    UnicodeScript::KAITHI,                   // 110CD
                    UnicodeScript::UNKNOWN,                  // 110CE..110CF
                    UnicodeScript::SORA_SOMPENG,             // 110D0..110E8
                    UnicodeScript::UNKNOWN,                  // 110E9..110EF
                    UnicodeScript::SORA_SOMPENG,             // 110F0..110F9
                    UnicodeScript::UNKNOWN,                  // 110FA..110FF
                    UnicodeScript::CHAKMA,                   // 11100..11134
                    UnicodeScript::UNKNOWN,                  // 11135
                    UnicodeScript::CHAKMA,                   // 11136..11147
                    UnicodeScript::UNKNOWN,                  // 11148..1114F
                    UnicodeScript::MAHAJANI,                 // 11150..11176
                    UnicodeScript::UNKNOWN,                  // 11177..1117F
                    UnicodeScript::SHARADA,                  // 11180..111DF
                    UnicodeScript::UNKNOWN,                  // 111E0
                    UnicodeScript::SINHALA,                  // 111E1..111F4
                    UnicodeScript::UNKNOWN,                  // 111F5..111FF
                    UnicodeScript::KHOJKI,                   // 11200..11211
                    UnicodeScript::UNKNOWN,                  // 11212
                    UnicodeScript::KHOJKI,                   // 11213..11241
                    UnicodeScript::UNKNOWN,                  // 11242..1127F
                    UnicodeScript::MULTANI,                  // 11280..11286
                    UnicodeScript::UNKNOWN,                  // 11287
                    UnicodeScript::MULTANI,                  // 11288
                    UnicodeScript::UNKNOWN,                  // 11289
                    UnicodeScript::MULTANI,                  // 1128A..1128D
                    UnicodeScript::UNKNOWN,                  // 1128E
                    UnicodeScript::MULTANI,                  // 1128F..1129D
                    UnicodeScript::UNKNOWN,                  // 1129E
                    UnicodeScript::MULTANI,                  // 1129F..112A9
                    UnicodeScript::UNKNOWN,                  // 112AA..112AF
                    UnicodeScript::KHUDAWADI,                // 112B0..112EA
                    UnicodeScript::UNKNOWN,                  // 112EB..112EF
                    UnicodeScript::KHUDAWADI,                // 112F0..112F9
                    UnicodeScript::UNKNOWN,                  // 112FA..112FF
                    UnicodeScript::GRANTHA,                  // 11300..11303
                    UnicodeScript::UNKNOWN,                  // 11304
                    UnicodeScript::GRANTHA,                  // 11305..1130C
                    UnicodeScript::UNKNOWN,                  // 1130D..1130E
                    UnicodeScript::GRANTHA,                  // 1130F..11310
                    UnicodeScript::UNKNOWN,                  // 11311..11312
                    UnicodeScript::GRANTHA,                  // 11313..11328
                    UnicodeScript::UNKNOWN,                  // 11329
                    UnicodeScript::GRANTHA,                  // 1132A..11330
                    UnicodeScript::UNKNOWN,                  // 11331
                    UnicodeScript::GRANTHA,                  // 11332..11333
                    UnicodeScript::UNKNOWN,                  // 11334
                    UnicodeScript::GRANTHA,                  // 11335..11339
                    UnicodeScript::UNKNOWN,                  // 1133A
                    UnicodeScript::INHERITED,                // 1133B
                    UnicodeScript::GRANTHA,                  // 1133C..11344
                    UnicodeScript::UNKNOWN,                  // 11345..11346
                    UnicodeScript::GRANTHA,                  // 11347..11348
                    UnicodeScript::UNKNOWN,                  // 11349..1134A
                    UnicodeScript::GRANTHA,                  // 1134B..1134D
                    UnicodeScript::UNKNOWN,                  // 1134E..1134F
                    UnicodeScript::GRANTHA,                  // 11350
                    UnicodeScript::UNKNOWN,                  // 11351..11356
                    UnicodeScript::GRANTHA,                  // 11357
                    UnicodeScript::UNKNOWN,                  // 11358..1135C
                    UnicodeScript::GRANTHA,                  // 1135D..11363
                    UnicodeScript::UNKNOWN,                  // 11364..11365
                    UnicodeScript::GRANTHA,                  // 11366..1136C
                    UnicodeScript::UNKNOWN,                  // 1136D..1136F
                    UnicodeScript::GRANTHA,                  // 11370..11374
                    UnicodeScript::UNKNOWN,                  // 11375..113FF
                    UnicodeScript::NEWA,                     // 11400..1145B
                    UnicodeScript::UNKNOWN,                  // 1145C
                    UnicodeScript::NEWA,                     // 1145D..11461
                    UnicodeScript::UNKNOWN,                  // 11462..1147F
                    UnicodeScript::TIRHUTA,                  // 11480..114C7
                    UnicodeScript::UNKNOWN,                  // 114C8..114CF
                    UnicodeScript::TIRHUTA,                  // 114D0..114D9
                    UnicodeScript::UNKNOWN,                  // 114DA..1157F
                    UnicodeScript::SIDDHAM,                  // 11580..115B5
                    UnicodeScript::UNKNOWN,                  // 115B6..115B7
                    UnicodeScript::SIDDHAM,                  // 115B8..115DD
                    UnicodeScript::UNKNOWN,                  // 115DE..115FF
                    UnicodeScript::MODI,                     // 11600..11644
                    UnicodeScript::UNKNOWN,                  // 11645..1164F
                    UnicodeScript::MODI,                     // 11650..11659
                    UnicodeScript::UNKNOWN,                  // 1165A..1165F
                    UnicodeScript::MONGOLIAN,                // 11660..1166C
                    UnicodeScript::UNKNOWN,                  // 1166D..1167F
                    UnicodeScript::TAKRI,                    // 11680..116B9
                    UnicodeScript::UNKNOWN,                  // 116BA..116BF
                    UnicodeScript::TAKRI,                    // 116C0..116C9
                    UnicodeScript::UNKNOWN,                  // 116CA..116FF
                    UnicodeScript::AHOM,                     // 11700..1171A
                    UnicodeScript::UNKNOWN,                  // 1171B..1171C
                    UnicodeScript::AHOM,                     // 1171D..1172B
                    UnicodeScript::UNKNOWN,                  // 1172C..1172F
                    UnicodeScript::AHOM,                     // 11730..11746
                    UnicodeScript::UNKNOWN,                  // 11747..117FF
                    UnicodeScript::DOGRA,                    // 11800..1183B
                    UnicodeScript::UNKNOWN,                  // 1183C..1189F
                    UnicodeScript::WARANG_CITI,              // 118A0..118F2
                    UnicodeScript::UNKNOWN,                  // 118F3..118FE
                    UnicodeScript::WARANG_CITI,              // 118FF
                    UnicodeScript::DIVES_AKURU,              // 11900..11906
                    UnicodeScript::UNKNOWN,                  // 11907..11908
                    UnicodeScript::DIVES_AKURU,              // 11909
                    UnicodeScript::UNKNOWN,                  // 1190A..1190B
                    UnicodeScript::DIVES_AKURU,              // 1190C..11913
                    UnicodeScript::UNKNOWN,                  // 11914
                    UnicodeScript::DIVES_AKURU,              // 11915..11916
                    UnicodeScript::UNKNOWN,                  // 11917
                    UnicodeScript::DIVES_AKURU,              // 11918..11935
                    UnicodeScript::UNKNOWN,                  // 11936
                    UnicodeScript::DIVES_AKURU,              // 11937..11938
                    UnicodeScript::UNKNOWN,                  // 11939..1193A
                    UnicodeScript::DIVES_AKURU,              // 1193B..11946
                    UnicodeScript::UNKNOWN,                  // 11947..1194F
                    UnicodeScript::DIVES_AKURU,              // 11950..11959
                    UnicodeScript::UNKNOWN,                  // 1195A..1199F
                    UnicodeScript::NANDINAGARI,              // 119A0..119A7
                    UnicodeScript::UNKNOWN,                  // 119A8..119A9
                    UnicodeScript::NANDINAGARI,              // 119AA..119D7
                    UnicodeScript::UNKNOWN,                  // 119D8..119D9
                    UnicodeScript::NANDINAGARI,              // 119DA..119E4
                    UnicodeScript::UNKNOWN,                  // 119E5..119FF
                    UnicodeScript::ZANABAZAR_SQUARE,         // 11A00..11A47
                    UnicodeScript::UNKNOWN,                  // 11A48..11A4F
                    UnicodeScript::SOYOMBO,                  // 11A50..11AA2
                    UnicodeScript::UNKNOWN,                  // 11AA3..11AAF
                    UnicodeScript::CANADIAN_ABORIGINAL,      // 11AB0..11ABF
                    UnicodeScript::PAU_CIN_HAU,              // 11AC0..11AF8
                    UnicodeScript::UNKNOWN,                  // 11AF9..11AFF
                    UnicodeScript::DEVANAGARI,               // 11B00..11B09
                    UnicodeScript::UNKNOWN,                  // 11B0A..11BFF
                    UnicodeScript::BHAIKSUKI,                // 11C00..11C08
                    UnicodeScript::UNKNOWN,                  // 11C09
                    UnicodeScript::BHAIKSUKI,                // 11C0A..11C36
                    UnicodeScript::UNKNOWN,                  // 11C37
                    UnicodeScript::BHAIKSUKI,                // 11C38..11C45
                    UnicodeScript::UNKNOWN,                  // 11C46..11C4F
                    UnicodeScript::BHAIKSUKI,                // 11C50..11C6C
                    UnicodeScript::UNKNOWN,                  // 11C6D..11C6F
                    UnicodeScript::MARCHEN,                  // 11C70..11C8F
                    UnicodeScript::UNKNOWN,                  // 11C90..11C91
                    UnicodeScript::MARCHEN,                  // 11C92..11CA7
                    UnicodeScript::UNKNOWN,                  // 11CA8
                    UnicodeScript::MARCHEN,                  // 11CA9..11CB6
                    UnicodeScript::UNKNOWN,                  // 11CB7..11CFF
                    UnicodeScript::MASARAM_GONDI,            // 11D00..11D06
                    UnicodeScript::UNKNOWN,                  // 11D07
                    UnicodeScript::MASARAM_GONDI,            // 11D08..11D09
                    UnicodeScript::UNKNOWN,                  // 11D0A
                    UnicodeScript::MASARAM_GONDI,            // 11D0B..11D36
                    UnicodeScript::UNKNOWN,                  // 11D37..11D39
                    UnicodeScript::MASARAM_GONDI,            // 11D3A
                    UnicodeScript::UNKNOWN,                  // 11D3B
                    UnicodeScript::MASARAM_GONDI,            // 11D3C..11D3D
                    UnicodeScript::UNKNOWN,                  // 11D3E
                    UnicodeScript::MASARAM_GONDI,            // 11D3F..11D47
                    UnicodeScript::UNKNOWN,                  // 11D48..11D4F
                    UnicodeScript::MASARAM_GONDI,            // 11D50..11D59
                    UnicodeScript::UNKNOWN,                  // 11D5A..11D5F
                    UnicodeScript::GUNJALA_GONDI,            // 11D60..11D65
                    UnicodeScript::UNKNOWN,                  // 11D66
                    UnicodeScript::GUNJALA_GONDI,            // 11D67..11D68
                    UnicodeScript::UNKNOWN,                  // 11D69
                    UnicodeScript::GUNJALA_GONDI,            // 11D6A..11D8E
                    UnicodeScript::UNKNOWN,                  // 11D8F
                    UnicodeScript::GUNJALA_GONDI,            // 11D90..11D91
                    UnicodeScript::UNKNOWN,                  // 11D92
                    UnicodeScript::GUNJALA_GONDI,            // 11D93..11D98
                    UnicodeScript::UNKNOWN,                  // 11D99..11D9F
                    UnicodeScript::GUNJALA_GONDI,            // 11DA0..11DA9
                    UnicodeScript::UNKNOWN,                  // 11DAA..11EDF
                    UnicodeScript::MAKASAR,                  // 11EE0..11EF8
                    UnicodeScript::UNKNOWN,                  // 11EF9..11EFF
                    UnicodeScript::KAWI,                     // 11F00..11F10
                    UnicodeScript::UNKNOWN,                  // 11F11
                    UnicodeScript::KAWI,                     // 11F12..11F3A
                    UnicodeScript::UNKNOWN,                  // 11F3B..11F3D
                    UnicodeScript::KAWI,                     // 11F3E..11F59
                    UnicodeScript::UNKNOWN,                  // 11F5A..11FAF
                    UnicodeScript::LISU,                     // 11FB0
                    UnicodeScript::UNKNOWN,                  // 11FB1..11FBF
                    UnicodeScript::TAMIL,                    // 11FC0..11FF1
                    UnicodeScript::UNKNOWN,                  // 11FF2..11FFE
                    UnicodeScript::TAMIL,                    // 11FFF
                    UnicodeScript::CUNEIFORM,                // 12000..12399
                    UnicodeScript::UNKNOWN,                  // 1239A..123FF
                    UnicodeScript::CUNEIFORM,                // 12400..1246E
                    UnicodeScript::UNKNOWN,                  // 1246F
                    UnicodeScript::CUNEIFORM,                // 12470..12474
                    UnicodeScript::UNKNOWN,                  // 12475..1247F
                    UnicodeScript::CUNEIFORM,                // 12480..12543
                    UnicodeScript::UNKNOWN,                  // 12544..12F8F
                    UnicodeScript::CYPRO_MINOAN,             // 12F90..12FF2
                    UnicodeScript::UNKNOWN,                  // 12FF3..12FFF
                    UnicodeScript::EGYPTIAN_HIEROGLYPHS,     // 13000..13455
                    UnicodeScript::UNKNOWN,                  // 13456..143FF
                    UnicodeScript::ANATOLIAN_HIEROGLYPHS,    // 14400..14646
                    UnicodeScript::UNKNOWN,                  // 14647..167FF
                    UnicodeScript::BAMUM,                    // 16800..16A38
                    UnicodeScript::UNKNOWN,                  // 16A39..16A3F
                    UnicodeScript::MRO,                      // 16A40..16A5E
                    UnicodeScript::UNKNOWN,                  // 16A5F
                    UnicodeScript::MRO,                      // 16A60..16A69
                    UnicodeScript::UNKNOWN,                  // 16A6A..16A6D
                    UnicodeScript::MRO,                      // 16A6E..16A6F
                    UnicodeScript::TANGSA,                   // 16A70..16ABE
                    UnicodeScript::UNKNOWN,                  // 16ABF
                    UnicodeScript::TANGSA,                   // 16AC0..16AC9
                    UnicodeScript::UNKNOWN,                  // 16ACA..16ACF
                    UnicodeScript::BASSA_VAH,                // 16AD0..16AED
                    UnicodeScript::UNKNOWN,                  // 16AEE..16AEF
                    UnicodeScript::BASSA_VAH,                // 16AF0..16AF5
                    UnicodeScript::UNKNOWN,                  // 16AF6..16AFF
                    UnicodeScript::PAHAWH_HMONG,             // 16B00..16B45
                    UnicodeScript::UNKNOWN,                  // 16B46..16B4F
                    UnicodeScript::PAHAWH_HMONG,             // 16B50..16B59
                    UnicodeScript::UNKNOWN,                  // 16B5A
                    UnicodeScript::PAHAWH_HMONG,             // 16B5B..16B61
                    UnicodeScript::UNKNOWN,                  // 16B62
                    UnicodeScript::PAHAWH_HMONG,             // 16B63..16B77
                    UnicodeScript::UNKNOWN,                  // 16B78..16B7C
                    UnicodeScript::PAHAWH_HMONG,             // 16B7D..16B8F
                    UnicodeScript::UNKNOWN,                  // 16B90..16E3F
                    UnicodeScript::MEDEFAIDRIN,              // 16E40..16E9A
                    UnicodeScript::UNKNOWN,                  // 16E9B..16EFF
                    UnicodeScript::MIAO,                     // 16F00..16F4A
                    UnicodeScript::UNKNOWN,                  // 16F4B..16F4E
                    UnicodeScript::MIAO,                     // 16F4F..16F87
                    UnicodeScript::UNKNOWN,                  // 16F88..16F8E
                    UnicodeScript::MIAO,                     // 16F8F..16F9F
                    UnicodeScript::UNKNOWN,                  // 16FA0..16FDF
                    UnicodeScript::TANGUT,                   // 16FE0
                    UnicodeScript::NUSHU,                    // 16FE1
                    UnicodeScript::HAN,                      // 16FE2..16FE3
                    UnicodeScript::KHITAN_SMALL_SCRIPT,      // 16FE4
                    UnicodeScript::UNKNOWN,                  // 16FE5..16FEF
                    UnicodeScript::HAN,                      // 16FF0..16FF1
                    UnicodeScript::UNKNOWN,                  // 16FF2..16FFF
                    UnicodeScript::TANGUT,                   // 17000..187F7
                    UnicodeScript::UNKNOWN,                  // 187F8..187FF
                    UnicodeScript::TANGUT,                   // 18800..18AFF
                    UnicodeScript::KHITAN_SMALL_SCRIPT,      // 18B00..18CD5
                    UnicodeScript::UNKNOWN,                  // 18CD6..18CFF
                    UnicodeScript::TANGUT,                   // 18D00..18D08
                    UnicodeScript::UNKNOWN,                  // 18D09..1AFEF
                    UnicodeScript::KATAKANA,                 // 1AFF0..1AFF3
                    UnicodeScript::UNKNOWN,                  // 1AFF4
                    UnicodeScript::KATAKANA,                 // 1AFF5..1AFFB
                    UnicodeScript::UNKNOWN,                  // 1AFFC
                    UnicodeScript::KATAKANA,                 // 1AFFD..1AFFE
                    UnicodeScript::UNKNOWN,                  // 1AFFF
                    UnicodeScript::KATAKANA,                 // 1B000
                    UnicodeScript::HIRAGANA,                 // 1B001..1B11F
                    UnicodeScript::KATAKANA,                 // 1B120..1B122
                    UnicodeScript::UNKNOWN,                  // 1B123..1B131
                    UnicodeScript::HIRAGANA,                 // 1B132
                    UnicodeScript::UNKNOWN,                  // 1B133..1B14F
                    UnicodeScript::HIRAGANA,                 // 1B150..1B152
                    UnicodeScript::UNKNOWN,                  // 1B153..1B154
                    UnicodeScript::KATAKANA,                 // 1B155
                    UnicodeScript::UNKNOWN,                  // 1B156..1B163
                    UnicodeScript::KATAKANA,                 // 1B164..1B167
                    UnicodeScript::UNKNOWN,                  // 1B168..1B16F
                    UnicodeScript::NUSHU,                    // 1B170..1B2FB
                    UnicodeScript::UNKNOWN,                  // 1B2FC..1BBFF
                    UnicodeScript::DUPLOYAN,                 // 1BC00..1BC6A
                    UnicodeScript::UNKNOWN,                  // 1BC6B..1BC6F
                    UnicodeScript::DUPLOYAN,                 // 1BC70..1BC7C
                    UnicodeScript::UNKNOWN,                  // 1BC7D..1BC7F
                    UnicodeScript::DUPLOYAN,                 // 1BC80..1BC88
                    UnicodeScript::UNKNOWN,                  // 1BC89..1BC8F
                    UnicodeScript::DUPLOYAN,                 // 1BC90..1BC99
                    UnicodeScript::UNKNOWN,                  // 1BC9A..1BC9B
                    UnicodeScript::DUPLOYAN,                 // 1BC9C..1BC9F
                    UnicodeScript::COMMON,                   // 1BCA0..1BCA3
                    UnicodeScript::UNKNOWN,                  // 1BCA4..1CEFF
                    UnicodeScript::INHERITED,                // 1CF00..1CF2D
                    UnicodeScript::UNKNOWN,                  // 1CF2E..1CF2F
                    UnicodeScript::INHERITED,                // 1CF30..1CF46
                    UnicodeScript::UNKNOWN,                  // 1CF47..1CF4F
                    UnicodeScript::COMMON,                   // 1CF50..1CFC3
                    UnicodeScript::UNKNOWN,                  // 1CFC4..1CFFF
                    UnicodeScript::COMMON,                   // 1D000..1D0F5
                    UnicodeScript::UNKNOWN,                  // 1D0F6..1D0FF
                    UnicodeScript::COMMON,                   // 1D100..1D126
                    UnicodeScript::UNKNOWN,                  // 1D127..1D128
                    UnicodeScript::COMMON,                   // 1D129..1D166
                    UnicodeScript::INHERITED,                // 1D167..1D169
                    UnicodeScript::COMMON,                   // 1D16A..1D17A
                    UnicodeScript::INHERITED,                // 1D17B..1D182
                    UnicodeScript::COMMON,                   // 1D183..1D184
                    UnicodeScript::INHERITED,                // 1D185..1D18B
                    UnicodeScript::COMMON,                   // 1D18C..1D1A9
                    UnicodeScript::INHERITED,                // 1D1AA..1D1AD
                    UnicodeScript::COMMON,                   // 1D1AE..1D1EA
                    UnicodeScript::UNKNOWN,                  // 1D1EB..1D1FF
                    UnicodeScript::GREEK,                    // 1D200..1D245
                    UnicodeScript::UNKNOWN,                  // 1D246..1D2BF
                    UnicodeScript::COMMON,                   // 1D2C0..1D2D3
                    UnicodeScript::UNKNOWN,                  // 1D2D4..1D2DF
                    UnicodeScript::COMMON,                   // 1D2E0..1D2F3
                    UnicodeScript::UNKNOWN,                  // 1D2F4..1D2FF
                    UnicodeScript::COMMON,                   // 1D300..1D356
                    UnicodeScript::UNKNOWN,                  // 1D357..1D35F
                    UnicodeScript::COMMON,                   // 1D360..1D378
                    UnicodeScript::UNKNOWN,                  // 1D379..1D3FF
                    UnicodeScript::COMMON,                   // 1D400..1D454
                    UnicodeScript::UNKNOWN,                  // 1D455
                    UnicodeScript::COMMON,                   // 1D456..1D49C
                    UnicodeScript::UNKNOWN,                  // 1D49D
                    UnicodeScript::COMMON,                   // 1D49E..1D49F
                    UnicodeScript::UNKNOWN,                  // 1D4A0..1D4A1
                    UnicodeScript::COMMON,                   // 1D4A2
                    UnicodeScript::UNKNOWN,                  // 1D4A3..1D4A4
                    UnicodeScript::COMMON,                   // 1D4A5..1D4A6
                    UnicodeScript::UNKNOWN,                  // 1D4A7..1D4A8
                    UnicodeScript::COMMON,                   // 1D4A9..1D4AC
                    UnicodeScript::UNKNOWN,                  // 1D4AD
                    UnicodeScript::COMMON,                   // 1D4AE..1D4B9
                    UnicodeScript::UNKNOWN,                  // 1D4BA
                    UnicodeScript::COMMON,                   // 1D4BB
                    UnicodeScript::UNKNOWN,                  // 1D4BC
                    UnicodeScript::COMMON,                   // 1D4BD..1D4C3
                    UnicodeScript::UNKNOWN,                  // 1D4C4
                    UnicodeScript::COMMON,                   // 1D4C5..1D505
                    UnicodeScript::UNKNOWN,                  // 1D506
                    UnicodeScript::COMMON,                   // 1D507..1D50A
                    UnicodeScript::UNKNOWN,                  // 1D50B..1D50C
                    UnicodeScript::COMMON,                   // 1D50D..1D514
                    UnicodeScript::UNKNOWN,                  // 1D515
                    UnicodeScript::COMMON,                   // 1D516..1D51C
                    UnicodeScript::UNKNOWN,                  // 1D51D
                    UnicodeScript::COMMON,                   // 1D51E..1D539
                    UnicodeScript::UNKNOWN,                  // 1D53A
                    UnicodeScript::COMMON,                   // 1D53B..1D53E
                    UnicodeScript::UNKNOWN,                  // 1D53F
                    UnicodeScript::COMMON,                   // 1D540..1D544
                    UnicodeScript::UNKNOWN,                  // 1D545
                    UnicodeScript::COMMON,                   // 1D546
                    UnicodeScript::UNKNOWN,                  // 1D547..1D549
                    UnicodeScript::COMMON,                   // 1D54A..1D550
                    UnicodeScript::UNKNOWN,                  // 1D551
                    UnicodeScript::COMMON,                   // 1D552..1D6A5
                    UnicodeScript::UNKNOWN,                  // 1D6A6..1D6A7
                    UnicodeScript::COMMON,                   // 1D6A8..1D7CB
                    UnicodeScript::UNKNOWN,                  // 1D7CC..1D7CD
                    UnicodeScript::COMMON,                   // 1D7CE..1D7FF
                    UnicodeScript::SIGNWRITING,              // 1D800..1DA8B
                    UnicodeScript::UNKNOWN,                  // 1DA8C..1DA9A
                    UnicodeScript::SIGNWRITING,              // 1DA9B..1DA9F
                    UnicodeScript::UNKNOWN,                  // 1DAA0
                    UnicodeScript::SIGNWRITING,              // 1DAA1..1DAAF
                    UnicodeScript::UNKNOWN,                  // 1DAB0..1DEFF
                    UnicodeScript::LATIN,                    // 1DF00..1DF1E
                    UnicodeScript::UNKNOWN,                  // 1DF1F..1DF24
                    UnicodeScript::LATIN,                    // 1DF25..1DF2A
                    UnicodeScript::UNKNOWN,                  // 1DF2B..1DFFF
                    UnicodeScript::GLAGOLITIC,               // 1E000..1E006
                    UnicodeScript::UNKNOWN,                  // 1E007
                    UnicodeScript::GLAGOLITIC,               // 1E008..1E018
                    UnicodeScript::UNKNOWN,                  // 1E019..1E01A
                    UnicodeScript::GLAGOLITIC,               // 1E01B..1E021
                    UnicodeScript::UNKNOWN,                  // 1E022
                    UnicodeScript::GLAGOLITIC,               // 1E023..1E024
                    UnicodeScript::UNKNOWN,                  // 1E025
                    UnicodeScript::GLAGOLITIC,               // 1E026..1E02A
                    UnicodeScript::UNKNOWN,                  // 1E02B..1E02F
                    UnicodeScript::CYRILLIC,                 // 1E030..1E06D
                    UnicodeScript::UNKNOWN,                  // 1E06E..1E08E
                    UnicodeScript::CYRILLIC,                 // 1E08F
                    UnicodeScript::UNKNOWN,                  // 1E090..1E0FF
                    UnicodeScript::NYIAKENG_PUACHUE_HMONG,   // 1E100..1E12C
                    UnicodeScript::UNKNOWN,                  // 1E12D..1E12F
                    UnicodeScript::NYIAKENG_PUACHUE_HMONG,   // 1E130..1E13D
                    UnicodeScript::UNKNOWN,                  // 1E13E..1E13F
                    UnicodeScript::NYIAKENG_PUACHUE_HMONG,   // 1E140..1E149
                    UnicodeScript::UNKNOWN,                  // 1E14A..1E14D
                    UnicodeScript::NYIAKENG_PUACHUE_HMONG,   // 1E14E..1E14F
                    UnicodeScript::UNKNOWN,                  // 1E150..1E28F
                    UnicodeScript::TOTO,                     // 1E290..1E2AE
                    UnicodeScript::UNKNOWN,                  // 1E2AF..1E2BF
                    UnicodeScript::WANCHO,                   // 1E2C0..1E2F9
                    UnicodeScript::UNKNOWN,                  // 1E2FA..1E2FE
                    UnicodeScript::WANCHO,                   // 1E2FF
                    UnicodeScript::UNKNOWN,                  // 1E300..1E4CF
                    UnicodeScript::NAG_MUNDARI,              // 1E4D0..1E4F9
                    UnicodeScript::UNKNOWN,                  // 1E4FA..1E7DF
                    UnicodeScript::ETHIOPIC,                 // 1E7E0..1E7E6
                    UnicodeScript::UNKNOWN,                  // 1E7E7
                    UnicodeScript::ETHIOPIC,                 // 1E7E8..1E7EB
                    UnicodeScript::UNKNOWN,                  // 1E7EC
                    UnicodeScript::ETHIOPIC,                 // 1E7ED..1E7EE
                    UnicodeScript::UNKNOWN,                  // 1E7EF
                    UnicodeScript::ETHIOPIC,                 // 1E7F0..1E7FE
                    UnicodeScript::UNKNOWN,                  // 1E7FF
                    UnicodeScript::MENDE_KIKAKUI,            // 1E800..1E8C4
                    UnicodeScript::UNKNOWN,                  // 1E8C5..1E8C6
                    UnicodeScript::MENDE_KIKAKUI,            // 1E8C7..1E8D6
                    UnicodeScript::UNKNOWN,                  // 1E8D7..1E8FF
                    UnicodeScript::ADLAM,                    // 1E900..1E94B
                    UnicodeScript::UNKNOWN,                  // 1E94C..1E94F
                    UnicodeScript::ADLAM,                    // 1E950..1E959
                    UnicodeScript::UNKNOWN,                  // 1E95A..1E95D
                    UnicodeScript::ADLAM,                    // 1E95E..1E95F
                    UnicodeScript::UNKNOWN,                  // 1E960..1EC70
                    UnicodeScript::COMMON,                   // 1EC71..1ECB4
                    UnicodeScript::UNKNOWN,                  // 1ECB5..1ED00
                    UnicodeScript::COMMON,                   // 1ED01..1ED3D
                    UnicodeScript::UNKNOWN,                  // 1ED3E..1EDFF
                    UnicodeScript::ARABIC,                   // 1EE00..1EE03
                    UnicodeScript::UNKNOWN,                  // 1EE04
                    UnicodeScript::ARABIC,                   // 1EE05..1EE1F
                    UnicodeScript::UNKNOWN,                  // 1EE20
                    UnicodeScript::ARABIC,                   // 1EE21..1EE22
                    UnicodeScript::UNKNOWN,                  // 1EE23
                    UnicodeScript::ARABIC,                   // 1EE24
                    UnicodeScript::UNKNOWN,                  // 1EE25..1EE26
                    UnicodeScript::ARABIC,                   // 1EE27
                    UnicodeScript::UNKNOWN,                  // 1EE28
                    UnicodeScript::ARABIC,                   // 1EE29..1EE32
                    UnicodeScript::UNKNOWN,                  // 1EE33
                    UnicodeScript::ARABIC,                   // 1EE34..1EE37
                    UnicodeScript::UNKNOWN,                  // 1EE38
                    UnicodeScript::ARABIC,                   // 1EE39
                    UnicodeScript::UNKNOWN,                  // 1EE3A
                    UnicodeScript::ARABIC,                   // 1EE3B
                    UnicodeScript::UNKNOWN,                  // 1EE3C..1EE41
                    UnicodeScript::ARABIC,                   // 1EE42
                    UnicodeScript::UNKNOWN,                  // 1EE43..1EE46
                    UnicodeScript::ARABIC,                   // 1EE47
                    UnicodeScript::UNKNOWN,                  // 1EE48
                    UnicodeScript::ARABIC,                   // 1EE49
                    UnicodeScript::UNKNOWN,                  // 1EE4A
                    UnicodeScript::ARABIC,                   // 1EE4B
                    UnicodeScript::UNKNOWN,                  // 1EE4C
                    UnicodeScript::ARABIC,                   // 1EE4D..1EE4F
                    UnicodeScript::UNKNOWN,                  // 1EE50
                    UnicodeScript::ARABIC,                   // 1EE51..1EE52
                    UnicodeScript::UNKNOWN,                  // 1EE53
                    UnicodeScript::ARABIC,                   // 1EE54
                    UnicodeScript::UNKNOWN,                  // 1EE55..1EE56
                    UnicodeScript::ARABIC,                   // 1EE57
                    UnicodeScript::UNKNOWN,                  // 1EE58
                    UnicodeScript::ARABIC,                   // 1EE59
                    UnicodeScript::UNKNOWN,                  // 1EE5A
                    UnicodeScript::ARABIC,                   // 1EE5B
                    UnicodeScript::UNKNOWN,                  // 1EE5C
                    UnicodeScript::ARABIC,                   // 1EE5D
                    UnicodeScript::UNKNOWN,                  // 1EE5E
                    UnicodeScript::ARABIC,                   // 1EE5F
                    UnicodeScript::UNKNOWN,                  // 1EE60
                    UnicodeScript::ARABIC,                   // 1EE61..1EE62
                    UnicodeScript::UNKNOWN,                  // 1EE63
                    UnicodeScript::ARABIC,                   // 1EE64
                    UnicodeScript::UNKNOWN,                  // 1EE65..1EE66
                    UnicodeScript::ARABIC,                   // 1EE67..1EE6A
                    UnicodeScript::UNKNOWN,                  // 1EE6B
                    UnicodeScript::ARABIC,                   // 1EE6C..1EE72
                    UnicodeScript::UNKNOWN,                  // 1EE73
                    UnicodeScript::ARABIC,                   // 1EE74..1EE77
                    UnicodeScript::UNKNOWN,                  // 1EE78
                    UnicodeScript::ARABIC,                   // 1EE79..1EE7C
                    UnicodeScript::UNKNOWN,                  // 1EE7D
                    UnicodeScript::ARABIC,                   // 1EE7E
                    UnicodeScript::UNKNOWN,                  // 1EE7F
                    UnicodeScript::ARABIC,                   // 1EE80..1EE89
                    UnicodeScript::UNKNOWN,                  // 1EE8A
                    UnicodeScript::ARABIC,                   // 1EE8B..1EE9B
                    UnicodeScript::UNKNOWN,                  // 1EE9C..1EEA0
                    UnicodeScript::ARABIC,                   // 1EEA1..1EEA3
                    UnicodeScript::UNKNOWN,                  // 1EEA4
                    UnicodeScript::ARABIC,                   // 1EEA5..1EEA9
                    UnicodeScript::UNKNOWN,                  // 1EEAA
                    UnicodeScript::ARABIC,                   // 1EEAB..1EEBB
                    UnicodeScript::UNKNOWN,                  // 1EEBC..1EEEF
                    UnicodeScript::ARABIC,                   // 1EEF0..1EEF1
                    UnicodeScript::UNKNOWN,                  // 1EEF2..1EFFF
                    UnicodeScript::COMMON,                   // 1F000..1F02B
                    UnicodeScript::UNKNOWN,                  // 1F02C..1F02F
                    UnicodeScript::COMMON,                   // 1F030..1F093
                    UnicodeScript::UNKNOWN,                  // 1F094..1F09F
                    UnicodeScript::COMMON,                   // 1F0A0..1F0AE
                    UnicodeScript::UNKNOWN,                  // 1F0AF..1F0B0
                    UnicodeScript::COMMON,                   // 1F0B1..1F0BF
                    UnicodeScript::UNKNOWN,                  // 1F0C0
                    UnicodeScript::COMMON,                   // 1F0C1..1F0CF
                    UnicodeScript::UNKNOWN,                  // 1F0D0
                    UnicodeScript::COMMON,                   // 1F0D1..1F0F5
                    UnicodeScript::UNKNOWN,                  // 1F0F6..1F0FF
                    UnicodeScript::COMMON,                   // 1F100..1F1AD
                    UnicodeScript::UNKNOWN,                  // 1F1AE..1F1E5
                    UnicodeScript::COMMON,                   // 1F1E6..1F1FF
                    UnicodeScript::HIRAGANA,                 // 1F200
                    UnicodeScript::COMMON,                   // 1F201..1F202
                    UnicodeScript::UNKNOWN,                  // 1F203..1F20F
                    UnicodeScript::COMMON,                   // 1F210..1F23B
                    UnicodeScript::UNKNOWN,                  // 1F23C..1F23F
                    UnicodeScript::COMMON,                   // 1F240..1F248
                    UnicodeScript::UNKNOWN,                  // 1F249..1F24F
                    UnicodeScript::COMMON,                   // 1F250..1F251
                    UnicodeScript::UNKNOWN,                  // 1F252..1F25F
                    UnicodeScript::COMMON,                   // 1F260..1F265
                    UnicodeScript::UNKNOWN,                  // 1F266..1F2FF
                    UnicodeScript::COMMON,                   // 1F300..1F6D7
                    UnicodeScript::UNKNOWN,                  // 1F6D8..1F6DB
                    UnicodeScript::COMMON,                   // 1F6DC..1F6EC
                    UnicodeScript::UNKNOWN,                  // 1F6ED..1F6EF
                    UnicodeScript::COMMON,                   // 1F6F0..1F6FC
                    UnicodeScript::UNKNOWN,                  // 1F6FD..1F6FF
                    UnicodeScript::COMMON,                   // 1F700..1F776
                    UnicodeScript::UNKNOWN,                  // 1F777..1F77A
                    UnicodeScript::COMMON,                   // 1F77B..1F7D9
                    UnicodeScript::UNKNOWN,                  // 1F7DA..1F7DF
                    UnicodeScript::COMMON,                   // 1F7E0..1F7EB
                    UnicodeScript::UNKNOWN,                  // 1F7EC..1F7EF
                    UnicodeScript::COMMON,                   // 1F7F0
                    UnicodeScript::UNKNOWN,                  // 1F7F1..1F7FF
                    UnicodeScript::COMMON,                   // 1F800..1F80B
                    UnicodeScript::UNKNOWN,                  // 1F80C..1F80F
                    UnicodeScript::COMMON,                   // 1F810..1F847
                    UnicodeScript::UNKNOWN,                  // 1F848..1F84F
                    UnicodeScript::COMMON,                   // 1F850..1F859
                    UnicodeScript::UNKNOWN,                  // 1F85A..1F85F
                    UnicodeScript::COMMON,                   // 1F860..1F887
                    UnicodeScript::UNKNOWN,                  // 1F888..1F88F
                    UnicodeScript::COMMON,                   // 1F890..1F8AD
                    UnicodeScript::UNKNOWN,                  // 1F8AE..1F8AF
                    UnicodeScript::COMMON,                   // 1F8B0..1F8B1
                    UnicodeScript::UNKNOWN,                  // 1F8B2..1F8FF
                    UnicodeScript::COMMON,                   // 1F900..1FA53
                    UnicodeScript::UNKNOWN,                  // 1FA54..1FA5F
                    UnicodeScript::COMMON,                   // 1FA60..1FA6D
                    UnicodeScript::UNKNOWN,                  // 1FA6E..1FA6F
                    UnicodeScript::COMMON,                   // 1FA70..1FA7C
                    UnicodeScript::UNKNOWN,                  // 1FA7D..1FA7F
                    UnicodeScript::COMMON,                   // 1FA80..1FA88
                    UnicodeScript::UNKNOWN,                  // 1FA89..1FA8F
                    UnicodeScript::COMMON,                   // 1FA90..1FABD
                    UnicodeScript::UNKNOWN,                  // 1FABE
                    UnicodeScript::COMMON,                   // 1FABF..1FAC5
                    UnicodeScript::UNKNOWN,                  // 1FAC6..1FACD
                    UnicodeScript::COMMON,                   // 1FACE..1FADB
                    UnicodeScript::UNKNOWN,                  // 1FADC..1FADF
                    UnicodeScript::COMMON,                   // 1FAE0..1FAE8
                    UnicodeScript::UNKNOWN,                  // 1FAE9..1FAEF
                    UnicodeScript::COMMON,                   // 1FAF0..1FAF8
                    UnicodeScript::UNKNOWN,                  // 1FAF9..1FAFF
                    UnicodeScript::COMMON,                   // 1FB00..1FB92
                    UnicodeScript::UNKNOWN,                  // 1FB93
                    UnicodeScript::COMMON,                   // 1FB94..1FBCA
                    UnicodeScript::UNKNOWN,                  // 1FBCB..1FBEF
                    UnicodeScript::COMMON,                   // 1FBF0..1FBF9
                    UnicodeScript::UNKNOWN,                  // 1FBFA..1FFFF
                    UnicodeScript::HAN,                      // 20000..2A6DF
                    UnicodeScript::UNKNOWN,                  // 2A6E0..2A6FF
                    UnicodeScript::HAN,                      // 2A700..2B739
                    UnicodeScript::UNKNOWN,                  // 2B73A..2B73F
                    UnicodeScript::HAN,                      // 2B740..2B81D
                    UnicodeScript::UNKNOWN,                  // 2B81E..2B81F
                    UnicodeScript::HAN,                      // 2B820..2CEA1
                    UnicodeScript::UNKNOWN,                  // 2CEA2..2CEAF
                    UnicodeScript::HAN,                      // 2CEB0..2EBE0
                    UnicodeScript::UNKNOWN,                  // 2EBE1..2F7FF
                    UnicodeScript::HAN,                      // 2F800..2FA1D
                    UnicodeScript::UNKNOWN,                  // 2FA1E..2FFFF
                    UnicodeScript::HAN,                      // 30000..3134A
                    UnicodeScript::UNKNOWN,                  // 3134B..3134F
                    UnicodeScript::HAN,                      // 31350..323AF
                    UnicodeScript::UNKNOWN,                  // 323B0..E0000
                    UnicodeScript::COMMON,                   // E0001
                    UnicodeScript::UNKNOWN,                  // E0002..E001F
                    UnicodeScript::COMMON,                   // E0020..E007F
                    UnicodeScript::UNKNOWN,                  // E0080..E00FF
                    UnicodeScript::INHERITED,                // E0100..E01EF
                    UnicodeScript::UNKNOWN,                  // E01F0..10FFFF
            };

            CORE_FAST UnicodeBlock B[] = {
                    UnicodeBlock::BASIC_LATIN,
                    UnicodeBlock::LATIN_1_SUPPLEMENT,
                    UnicodeBlock::LATIN_EXTENDED_A,
                    UnicodeBlock::LATIN_EXTENDED_B,
                    UnicodeBlock::IPA_EXTENSIONS,
                    UnicodeBlock::SPACING_MODIFIER_LETTERS,
                    UnicodeBlock::COMBINING_DIACRITICAL_MARKS,
                    UnicodeBlock::GREEK,
                    UnicodeBlock::CYRILLIC,
                    UnicodeBlock::CYRILLIC_SUPPLEMENTARY,
                    UnicodeBlock::ARMENIAN,
                    UnicodeBlock::HEBREW,
                    UnicodeBlock::ARABIC,
                    UnicodeBlock::SYRIAC,
                    UnicodeBlock::ARABIC_SUPPLEMENT,
                    UnicodeBlock::THAANA,
                    UnicodeBlock::NKO,
                    UnicodeBlock::SAMARITAN,
                    UnicodeBlock::MANDAIC,
                    UnicodeBlock::SYRIAC_SUPPLEMENT,
                    UnicodeBlock::ARABIC_EXTENDED_B,
                    UnicodeBlock::ARABIC_EXTENDED_A,
                    UnicodeBlock::DEVANAGARI,
                    UnicodeBlock::BENGALI,
                    UnicodeBlock::GURMUKHI,
                    UnicodeBlock::GUJARATI,
                    UnicodeBlock::ORIYA,
                    UnicodeBlock::TAMIL,
                    UnicodeBlock::TELUGU,
                    UnicodeBlock::KANNADA,
                    UnicodeBlock::MALAYALAM,
                    UnicodeBlock::SINHALA,
                    UnicodeBlock::THAI,
                    UnicodeBlock::LAO,
                    UnicodeBlock::TIBETAN,
                    UnicodeBlock::MYANMAR,
                    UnicodeBlock::GEORGIAN,
                    UnicodeBlock::HANGUL_JAMO,
                    UnicodeBlock::ETHIOPIC,
                    UnicodeBlock::ETHIOPIC_SUPPLEMENT,
                    UnicodeBlock::CHEROKEE,
                    UnicodeBlock::UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS,
                    UnicodeBlock::OGHAM,
                    UnicodeBlock::RUNIC,
                    UnicodeBlock::TAGALOG,
                    UnicodeBlock::HANUNOO,
                    UnicodeBlock::BUHID,
                    UnicodeBlock::TAGBANWA,
                    UnicodeBlock::KHMER,
                    UnicodeBlock::MONGOLIAN,
                    UnicodeBlock::UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED,
                    UnicodeBlock::LIMBU,
                    UnicodeBlock::TAI_LE,
                    UnicodeBlock::NEW_TAI_LUE,
                    UnicodeBlock::KHMER_SYMBOLS,
                    UnicodeBlock::BUGINESE,
                    UnicodeBlock::TAI_THAM,
                    UnicodeBlock::COMBINING_DIACRITICAL_MARKS_EXTENDED,
                    UnicodeBlock::BALINESE,
                    UnicodeBlock::SUNDANESE,
                    UnicodeBlock::BATAK,
                    UnicodeBlock::LEPCHA,
                    UnicodeBlock::OL_CHIKI,
                    UnicodeBlock::CYRILLIC_EXTENDED_C,
                    UnicodeBlock::GEORGIAN_EXTENDED,
                    UnicodeBlock::SUNDANESE_SUPPLEMENT,
                    UnicodeBlock::VEDIC_EXTENSIONS,
                    UnicodeBlock::PHONETIC_EXTENSIONS,
                    UnicodeBlock::PHONETIC_EXTENSIONS_SUPPLEMENT,
                    UnicodeBlock::COMBINING_DIACRITICAL_MARKS_SUPPLEMENT,
                    UnicodeBlock::LATIN_EXTENDED_ADDITIONAL,
                    UnicodeBlock::GREEK_EXTENDED,
                    UnicodeBlock::GENERAL_PUNCTUATION,
                    UnicodeBlock::SUPERSCRIPTS_AND_SUBSCRIPTS,
                    UnicodeBlock::CURRENCY_SYMBOLS,
                    UnicodeBlock::COMBINING_MARKS_FOR_SYMBOLS,
                    UnicodeBlock::LETTERLIKE_SYMBOLS,
                    UnicodeBlock::NUMBER_FORMS,
                    UnicodeBlock::ARROWS,
                    UnicodeBlock::MATHEMATICAL_OPERATORS,
                    UnicodeBlock::MISCELLANEOUS_TECHNICAL,
                    UnicodeBlock::CONTROL_PICTURES,
                    UnicodeBlock::OPTICAL_CHARACTER_RECOGNITION,
                    UnicodeBlock::ENCLOSED_ALPHANUMERICS,
                    UnicodeBlock::BOX_DRAWING,
                    UnicodeBlock::BLOCK_ELEMENTS,
                    UnicodeBlock::GEOMETRIC_SHAPES,
                    UnicodeBlock::MISCELLANEOUS_SYMBOLS,
                    UnicodeBlock::DINGBATS,
                    UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A,
                    UnicodeBlock::SUPPLEMENTAL_ARROWS_A,
                    UnicodeBlock::BRAILLE_PATTERNS,
                    UnicodeBlock::SUPPLEMENTAL_ARROWS_B,
                    UnicodeBlock::MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B,
                    UnicodeBlock::SUPPLEMENTAL_MATHEMATICAL_OPERATORS,
                    UnicodeBlock::MISCELLANEOUS_SYMBOLS_AND_ARROWS,
                    UnicodeBlock::GLAGOLITIC,
                    UnicodeBlock::LATIN_EXTENDED_C,
                    UnicodeBlock::COPTIC,
                    UnicodeBlock::GEORGIAN_SUPPLEMENT,
                    UnicodeBlock::TIFINAGH,
                    UnicodeBlock::ETHIOPIC_EXTENDED,
                    UnicodeBlock::CYRILLIC_EXTENDED_A,
                    UnicodeBlock::SUPPLEMENTAL_PUNCTUATION,
                    UnicodeBlock::CJK_RADICALS_SUPPLEMENT,
                    UnicodeBlock::KANGXI_RADICALS,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::IDEOGRAPHIC_DESCRIPTION_CHARACTERS,
                    UnicodeBlock::CJK_SYMBOLS_AND_PUNCTUATION,
                    UnicodeBlock::HIRAGANA,
                    UnicodeBlock::KATAKANA,
                    UnicodeBlock::BOPOMOFO,
                    UnicodeBlock::HANGUL_COMPATIBILITY_JAMO,
                    UnicodeBlock::KANBUN,
                    UnicodeBlock::BOPOMOFO_EXTENDED,
                    UnicodeBlock::CJK_STROKES,
                    UnicodeBlock::KATAKANA_PHONETIC_EXTENSIONS,
                    UnicodeBlock::ENCLOSED_CJK_LETTERS_AND_MONTHS,
                    UnicodeBlock::CJK_COMPATIBILITY,
                    UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A,
                    UnicodeBlock::YIJING_HEXAGRAM_SYMBOLS,
                    UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS,
                    UnicodeBlock::YI_SYLLABLES,
                    UnicodeBlock::YI_RADICALS,
                    UnicodeBlock::LISU,
                    UnicodeBlock::VAI,
                    UnicodeBlock::CYRILLIC_EXTENDED_B,
                    UnicodeBlock::BAMUM,
                    UnicodeBlock::MODIFIER_TONE_LETTERS,
                    UnicodeBlock::LATIN_EXTENDED_D,
                    UnicodeBlock::SYLOTI_NAGRI,
                    UnicodeBlock::COMMON_INDIC_NUMBER_FORMS,
                    UnicodeBlock::PHAGS_PA,
                    UnicodeBlock::SAURASHTRA,
                    UnicodeBlock::DEVANAGARI_EXTENDED,
                    UnicodeBlock::KAYAH_LI,
                    UnicodeBlock::REJANG,
                    UnicodeBlock::HANGUL_JAMO_EXTENDED_A,
                    UnicodeBlock::JAVANESE,
                    UnicodeBlock::MYANMAR_EXTENDED_B,
                    UnicodeBlock::CHAM,
                    UnicodeBlock::MYANMAR_EXTENDED_A,
                    UnicodeBlock::TAI_VIET,
                    UnicodeBlock::MEETEI_MAYEK_EXTENSIONS,
                    UnicodeBlock::ETHIOPIC_EXTENDED_A,
                    UnicodeBlock::LATIN_EXTENDED_E,
                    UnicodeBlock::CHEROKEE_SUPPLEMENT,
                    UnicodeBlock::MEETEI_MAYEK,
                    UnicodeBlock::HANGUL_SYLLABLES,
                    UnicodeBlock::HANGUL_JAMO_EXTENDED_B,
                    UnicodeBlock::HIGH_SURROGATES,
                    UnicodeBlock::HIGH_PRIVATE_USE_SURROGATES,
                    UnicodeBlock::LOW_SURROGATES,
                    UnicodeBlock::PRIVATE_USE_AREA,
                    UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS,
                    UnicodeBlock::ALPHABETIC_PRESENTATION_FORMS,
                    UnicodeBlock::ARABIC_PRESENTATION_FORMS_A,
                    UnicodeBlock::VARIATION_SELECTORS,
                    UnicodeBlock::VERTICAL_FORMS,
                    UnicodeBlock::COMBINING_HALF_MARKS,
                    UnicodeBlock::CJK_COMPATIBILITY_FORMS,
                    UnicodeBlock::SMALL_FORM_VARIANTS,
                    UnicodeBlock::ARABIC_PRESENTATION_FORMS_B,
                    UnicodeBlock::HALFWIDTH_AND_FULLWIDTH_FORMS,
                    UnicodeBlock::SPECIALS,
                    UnicodeBlock::LINEAR_B_SYLLABARY,
                    UnicodeBlock::LINEAR_B_IDEOGRAMS,
                    UnicodeBlock::AEGEAN_NUMBERS,
                    UnicodeBlock::ANCIENT_GREEK_NUMBERS,
                    UnicodeBlock::ANCIENT_SYMBOLS,
                    UnicodeBlock::PHAISTOS_DISC,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::LYCIAN,
                    UnicodeBlock::CARIAN,
                    UnicodeBlock::COPTIC_EPACT_NUMBERS,
                    UnicodeBlock::OLD_ITALIC,
                    UnicodeBlock::GOTHIC,
                    UnicodeBlock::OLD_PERMIC,
                    UnicodeBlock::UGARITIC,
                    UnicodeBlock::OLD_PERSIAN,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::DESERET,
                    UnicodeBlock::SHAVIAN,
                    UnicodeBlock::OSMANYA,
                    UnicodeBlock::OSAGE,
                    UnicodeBlock::ELBASAN,
                    UnicodeBlock::CAUCASIAN_ALBANIAN,
                    UnicodeBlock::VITHKUQI,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::LINEAR_A,
                    UnicodeBlock::LATIN_EXTENDED_F,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::CYPRIOT_SYLLABARY,
                    UnicodeBlock::IMPERIAL_ARAMAIC,
                    UnicodeBlock::PALMYRENE,
                    UnicodeBlock::NABATAEAN,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::HATRAN,
                    UnicodeBlock::PHOENICIAN,
                    UnicodeBlock::LYDIAN,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::MEROITIC_HIEROGLYPHS,
                    UnicodeBlock::MEROITIC_CURSIVE,
                    UnicodeBlock::KHAROSHTHI,
                    UnicodeBlock::OLD_SOUTH_ARABIAN,
                    UnicodeBlock::OLD_NORTH_ARABIAN,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::MANICHAEAN,
                    UnicodeBlock::AVESTAN,
                    UnicodeBlock::INSCRIPTIONAL_PARTHIAN,
                    UnicodeBlock::INSCRIPTIONAL_PAHLAVI,
                    UnicodeBlock::PSALTER_PAHLAVI,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::OLD_TURKIC,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::OLD_HUNGARIAN,
                    UnicodeBlock::HANIFI_ROHINGYA,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::RUMI_NUMERAL_SYMBOLS,
                    UnicodeBlock::YEZIDI,
                    UnicodeBlock::ARABIC_EXTENDED_C,
                    UnicodeBlock::OLD_SOGDIAN,
                    UnicodeBlock::SOGDIAN,
                    UnicodeBlock::OLD_UYGHUR,
                    UnicodeBlock::CHORASMIAN,
                    UnicodeBlock::ELYMAIC,
                    UnicodeBlock::BRAHMI,
                    UnicodeBlock::KAITHI,
                    UnicodeBlock::SORA_SOMPENG,
                    UnicodeBlock::CHAKMA,
                    UnicodeBlock::MAHAJANI,
                    UnicodeBlock::SHARADA,
                    UnicodeBlock::SINHALA_ARCHAIC_NUMBERS,
                    UnicodeBlock::KHOJKI,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::MULTANI,
                    UnicodeBlock::KHUDAWADI,
                    UnicodeBlock::GRANTHA,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::NEWA,
                    UnicodeBlock::TIRHUTA,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::SIDDHAM,
                    UnicodeBlock::MODI,
                    UnicodeBlock::MONGOLIAN_SUPPLEMENT,
                    UnicodeBlock::TAKRI,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::AHOM,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::DOGRA,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::WARANG_CITI,
                    UnicodeBlock::DIVES_AKURU,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::NANDINAGARI,
                    UnicodeBlock::ZANABAZAR_SQUARE,
                    UnicodeBlock::SOYOMBO,
                    UnicodeBlock::UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED_A,
                    UnicodeBlock::PAU_CIN_HAU,
                    UnicodeBlock::DEVANAGARI_EXTENDED_A,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::BHAIKSUKI,
                    UnicodeBlock::MARCHEN,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::MASARAM_GONDI,
                    UnicodeBlock::GUNJALA_GONDI,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::MAKASAR,
                    UnicodeBlock::KAWI,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::LISU_SUPPLEMENT,
                    UnicodeBlock::TAMIL_SUPPLEMENT,
                    UnicodeBlock::CUNEIFORM,
                    UnicodeBlock::CUNEIFORM_NUMBERS_AND_PUNCTUATION,
                    UnicodeBlock::EARLY_DYNASTIC_CUNEIFORM,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::CYPRO_MINOAN,
                    UnicodeBlock::EGYPTIAN_HIEROGLYPHS,
                    UnicodeBlock::EGYPTIAN_HIEROGLYPH_FORMAT_CONTROLS,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::ANATOLIAN_HIEROGLYPHS,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::BAMUM_SUPPLEMENT,
                    UnicodeBlock::MRO,
                    UnicodeBlock::TANGSA,
                    UnicodeBlock::BASSA_VAH,
                    UnicodeBlock::PAHAWH_HMONG,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::MEDEFAIDRIN,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::MIAO,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION,
                    UnicodeBlock::TANGUT,
                    UnicodeBlock::TANGUT_COMPONENTS,
                    UnicodeBlock::KHITAN_SMALL_SCRIPT,
                    UnicodeBlock::TANGUT_SUPPLEMENT,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::KANA_EXTENDED_B,
                    UnicodeBlock::KANA_SUPPLEMENT,
                    UnicodeBlock::KANA_EXTENDED_A,
                    UnicodeBlock::SMALL_KANA_EXTENSION,
                    UnicodeBlock::NUSHU,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::DUPLOYAN,
                    UnicodeBlock::SHORTHAND_FORMAT_CONTROLS,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::ZNAMENNY_MUSICAL_NOTATION,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::BYZANTINE_MUSICAL_SYMBOLS,
                    UnicodeBlock::MUSICAL_SYMBOLS,
                    UnicodeBlock::ANCIENT_GREEK_MUSICAL_NOTATION,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::KAKTOVIK_NUMERALS,
                    UnicodeBlock::MAYAN_NUMERALS,
                    UnicodeBlock::TAI_XUAN_JING_SYMBOLS,
                    UnicodeBlock::COUNTING_ROD_NUMERALS,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::MATHEMATICAL_ALPHANUMERIC_SYMBOLS,
                    UnicodeBlock::SUTTON_SIGNWRITING,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::LATIN_EXTENDED_G,
                    UnicodeBlock::GLAGOLITIC_SUPPLEMENT,
                    UnicodeBlock::CYRILLIC_EXTENDED_D,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::NYIAKENG_PUACHUE_HMONG,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::TOTO,
                    UnicodeBlock::WANCHO,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::NAG_MUNDARI,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::ETHIOPIC_EXTENDED_B,
                    UnicodeBlock::MENDE_KIKAKUI,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::ADLAM,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::INDIC_SIYAQ_NUMBERS,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::OTTOMAN_SIYAQ_NUMBERS,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::MAHJONG_TILES,
                    UnicodeBlock::DOMINO_TILES,
                    UnicodeBlock::PLAYING_CARDS,
                    UnicodeBlock::ENCLOSED_ALPHANUMERIC_SUPPLEMENT,
                    UnicodeBlock::ENCLOSED_IDEOGRAPHIC_SUPPLEMENT,
                    UnicodeBlock::MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS,
                    UnicodeBlock::EMOTICONS,
                    UnicodeBlock::ORNAMENTAL_DINGBATS,
                    UnicodeBlock::TRANSPORT_AND_MAP_SYMBOLS,
                    UnicodeBlock::ALCHEMICAL_SYMBOLS,
                    UnicodeBlock::GEOMETRIC_SHAPES_EXTENDED,
                    UnicodeBlock::SUPPLEMENTAL_ARROWS_C,
                    UnicodeBlock::SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS,
                    UnicodeBlock::CHESS_SYMBOLS,
                    UnicodeBlock::SYMBOLS_AND_PICTOGRAPHS_EXTENDED_A,
                    UnicodeBlock::SYMBOLS_FOR_LEGACY_COMPUTING,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C,
                    UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D,
                    UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E,
                    UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G,
                    UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_H,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::TAGS,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::VARIATION_SELECTORS_SUPPLEMENT,
                    UnicodeBlock::UNASSIGNED,
                    UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_A,
                    UnicodeBlock::SUPPLEMENTARY_PRIVATE_USE_AREA_B,
            };
        }

        //
        Object &UnicodeTable::clone() const {
            return instance;
        }

        glong UnicodeTable::query(gint ch, UnicodeTable::Column c) {
            gint row = readProperties(ch);
            if (row < 0) {
                ArgumentException("Invalid unicode code point: " + Integer::toUnsignedString(ch, 16))
                        .throws(__trace("core.foreign.UnicodeTable"));
            }
            glong retVal = -1;
            switch (c) {
                case Column::CATEGORY:
                    retVal = query0(row, 0);
                    break;
                case Column::DIRECTION:
                    retVal = query0(row, 1);
                    break;
                case Column::COMBINING_CLASS:
                    retVal = query0(row, 2);
                    break;
                case Column::JOINING_TYPE:
                    retVal = query0(row, 3);
                    break;
                case Column::NUMERIC_VALUE:
                    retVal = query0(row, 4);
                    break;
                case Column::MIRROR_OFFSET:
                    retVal = query0(row, 5);
                    break;
                case Column::UNICODE_VERSION:
                    retVal = query0(row, 6);
                    break;
                case Column::LOWERCASE_SPECIAL:
                    retVal = query0(row, 8);
                    break;
                case Column::LOWERCASE_OFFSET:
                    retVal = query0(row, 9);
                    break;
                case Column::UPPERCASE_SPECIAL:
                    retVal = query0(row, 10);
                    break;
                case Column::UPPERCASE_OFFSET:
                    retVal = query0(row, 11);
                    break;
                case Column::TITLECASE_SPECIAL:
                    retVal = query0(row, 12);
                    break;
                case Column::TITLECASE_OFFSET:
                    retVal = query0(row, 13);
                    break;
                case Column::CASE_FOLD_SPECIAL:
                    retVal = query0(row, 14);
                    break;
                case Column::CASE_FOLD_OFFSET:
                    retVal = query0(row, 15);
                    break;
                case Column::GRAPHEME_BREAK_CLASS:
                    retVal = query0(row, 16);
                    break;
                case Column::WORD_BREAK_CLASS:
                    retVal = query0(row, 17);
                    break;
                case Column::LINE_BREAK_CLASS:
                    retVal = query0(row, 18);
                    break;
                case Column::SENTENCE_BREAK_CLASS:
                    retVal = query0(row, 19);
                    break;
                case Column::UNICODE_SCRIPT: {
                    retVal = query0(row, 20);
                    gint i = 1657;
                    gint j = 0;
                    gint k = i / 2;
                    while (j < i) {
                        if (X[k] > ch)
                            i = k;
                        else
                            j = k;
                        k = (i + j) / 2;
                    }
                    if (retVal != (gint) A[k])
                        retVal = (gint) A[k];
                }
                    break;
                case Column::UNICODE_BLOCK: {
                    gint i = 378;
                    gint j = 0;
                    gint k = i / 2;
                    while (j < i) {
                        if (Y[k] > ch)
                            i = k;
                        else
                            j = k;
                        k = (i + j) / 2;
                    }
                    retVal = (gint) B[k];
                }
                    break;
                case Column::DECOMPOSITION:
                    retVal = ch >= 0xac00 && ch <= 0xd7a4 ? 0 : readDecompositions(ch);
                    break;
            }

            return retVal;
        }

        UnicodeTable UnicodeTable::instance = {};

    } // core
} // foreign