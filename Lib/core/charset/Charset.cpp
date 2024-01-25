//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "Charset.h"
#include <core/charset/IllegalCharsetNameException.h>
#include <core/io/CharBuffer.h>
#include <core/io/ByteBuffer.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/UnsupportedCharsetException.h>
#include <core/charset/CharacterCodingException.h>
#include <core/util/TreeSet.h>
#include <core/String.h>
#include <core/charset/private/CESU8.h>
#include <core/charset/private/GB18030.h>
#include <core/charset/private/GBK.h>
#include <core/charset/private/IBM437.h>
#include <core/charset/private/IBM737.h>
#include <core/charset/private/IBM775.h>
#include <core/charset/private/IBM850.h>
#include <core/charset/private/IBM852.h>
#include <core/charset/private/IBM855.h>
#include <core/charset/private/IBM857.h>
#include <core/charset/private/IBM858.h>
#include <core/charset/private/IBM862.h>
#include <core/charset/private/IBM866.h>
#include <core/charset/private/IBM874.h>
#include <core/charset/private/ISO8859_1.h>
#include <core/charset/private/ISO8859_13.h>
#include <core/charset/private/ISO8859_15.h>
#include <core/charset/private/ISO8859_16.h>
#include <core/charset/private/ISO8859_2.h>
#include <core/charset/private/ISO8859_4.h>
#include <core/charset/private/ISO8859_5.h>
#include <core/charset/private/ISO8859_7.h>
#include <core/charset/private/ISO8859_9.h>
#include <core/charset/private/JIS_X0201.h>
#include <core/charset/private/Johab.h>
#include <core/charset/private/KOI8_R.h>
#include <core/charset/private/KOI8_U.h>
#include <core/charset/private/SJIS.h>
#include <core/charset/private/US_ASCII.h>
#include <core/charset/private/UTF16.h>
#include <core/charset/private/UTF16BE.h>
#include <core/charset/private/UTF16LE.h>
#include <core/charset/private/UTF16LE_BOM.h>
#include <core/charset/private/UTF32.h>
#include <core/charset/private/UTF32BE.h>
#include <core/charset/private/UTF32BE_BOM.h>
#include <core/charset/private/UTF32LE.h>
#include <core/charset/private/UTF32LE_BOM.h>
#include <core/charset/private/UTF8.h>

namespace core {
    namespace charset {

        using namespace io;
        using namespace util;
        using namespace native;

        Set<Charset> &Charset::available = Unsafe::allocateInstance<TreeSet<Charset>>();

        void Charset::checkName(const String &s) {
            gint const n = s.length();
            if (n == 0) {
                IllegalCharsetNameException(s).throws(__trace("core.charset.Charset"));
            }
            for (int i = 0; i < n; i++) {
                gchar const c = s.charAt(i);
                if (c >= 'A' && c <= 'Z') continue;
                if (c >= 'a' && c <= 'z') continue;
                if (c >= '0' && c <= '9') continue;
                if (c == '-' && i != 0) continue;
                if (c == '+' && i != 0) continue;
                if (c == ':' && i != 0) continue;
                if (c == '_' && i != 0) continue;
                if (c == '.' && i != 0) continue;
                IllegalCharsetNameException(s).throws(__trace("core.charset.Charset"));
            }
        }

        String Charset::toString() const {
            return name();
        }

        gbool Charset::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<Charset>::hasInstance(o))
                return false;
            Charset const &cs = (const Charset &) o;
            return name().equals(cs.name());
        }

        gint Charset::hash() const {
            return name().hash();
        }

        gint Charset::compareTo(const Charset &that) const {
            if (this == &that)
                return 0;
            return name().compareToIgnoreCase(that.name());
        }

        io::ByteBuffer &Charset::encode(const String &str) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 return encode(CharBuffer::wrap(str));
             }, , __trace("core.charset.Charset"))
        }

        gbool Charset::isSupported(const String &charsetName) {
            for (const Charset &cs: available) {
                if (cs.name().equalsIgnoreCase(charsetName))
                    return true;
                for (String const& name: cs.aliases()){
                    if(name.equalsIgnoreCase(charsetName))
                        return true;
                }
            }
            return false;
        }

        const Charset &Charset::forName(const String &charsetName) {
            CORE_TRY_RETHROW_EXCEPTION(checkName(charsetName);, , __trace("core.charset.Charset"))
            if(available.size() < 35) {
                available.add(CESU8::INSTANCE);
                available.add(GB18030::INSTANCE);
                available.add(GBK::INSTANCE);
                available.add(IBM437::INSTANCE);
                available.add(IBM737::INSTANCE);
                available.add(IBM775::INSTANCE);
                available.add(IBM850::INSTANCE);
                available.add(IBM852::INSTANCE);
                available.add(IBM855::INSTANCE);
                available.add(IBM857::INSTANCE);
                available.add(IBM858::INSTANCE);
                available.add(IBM862::INSTANCE);
                available.add(IBM866::INSTANCE);
                available.add(IBM874::INSTANCE);
                available.add(ISO8859_1::INSTANCE);
                available.add(ISO8859_13::INSTANCE);
                available.add(ISO8859_15::INSTANCE);
                available.add(ISO8859_16::INSTANCE);
                available.add(ISO8859_2::INSTANCE);
                available.add(ISO8859_4::INSTANCE);
                available.add(ISO8859_5::INSTANCE);
                available.add(ISO8859_7::INSTANCE);
                available.add(ISO8859_9::INSTANCE);
                available.add(JIS_X0201::INSTANCE);
                available.add(Johab::INSTANCE);
                available.add(KOI8_R::INSTANCE);
                available.add(KOI8_U::INSTANCE);
                available.add(SJIS::INSTANCE);
                available.add(US_ASCII::INSTANCE);
                available.add(UTF16::INSTANCE);
                available.add(UTF16BE::INSTANCE);
                available.add(UTF16LE::INSTANCE);
                available.add(UTF16LE_BOM::INSTANCE);
                available.add(UTF32::INSTANCE);
                available.add(UTF32BE::INSTANCE);
                available.add(UTF32BE_BOM::INSTANCE);
                available.add(UTF32LE::INSTANCE);
                available.add(UTF32LE_BOM::INSTANCE);
            }
            for (const Charset &cs: available) {
                if (cs.name().equalsIgnoreCase(charsetName))
                    return cs;
                for (String const& name: cs.aliases()){
                    if(name.equalsIgnoreCase(charsetName))
                        return cs;
                }
            }
            UnsupportedCharsetException(charsetName).throws(__trace("core.charset.Charset"));
        }

        const Charset &Charset::forName(const String &charsetName, const Charset &fallback) {
            CORE_TRY_RETHROW_EXCEPTION(checkName(charsetName);, return fallback;, __trace("core.charset.Charset"))
            if(available.size() < 35) {
                available.add(CESU8::INSTANCE);
                available.add(GB18030::INSTANCE);
                available.add(GBK::INSTANCE);
                available.add(IBM437::INSTANCE);
                available.add(IBM737::INSTANCE);
                available.add(IBM775::INSTANCE);
                available.add(IBM850::INSTANCE);
                available.add(IBM852::INSTANCE);
                available.add(IBM855::INSTANCE);
                available.add(IBM857::INSTANCE);
                available.add(IBM858::INSTANCE);
                available.add(IBM862::INSTANCE);
                available.add(IBM866::INSTANCE);
                available.add(IBM874::INSTANCE);
                available.add(ISO8859_1::INSTANCE);
                available.add(ISO8859_13::INSTANCE);
                available.add(ISO8859_15::INSTANCE);
                available.add(ISO8859_16::INSTANCE);
                available.add(ISO8859_2::INSTANCE);
                available.add(ISO8859_4::INSTANCE);
                available.add(ISO8859_5::INSTANCE);
                available.add(ISO8859_7::INSTANCE);
                available.add(ISO8859_9::INSTANCE);
                available.add(JIS_X0201::INSTANCE);
                available.add(Johab::INSTANCE);
                available.add(KOI8_R::INSTANCE);
                available.add(KOI8_U::INSTANCE);
                available.add(SJIS::INSTANCE);
                available.add(US_ASCII::INSTANCE);
                available.add(UTF16::INSTANCE);
                available.add(UTF16BE::INSTANCE);
                available.add(UTF16LE::INSTANCE);
                available.add(UTF16LE_BOM::INSTANCE);
                available.add(UTF32::INSTANCE);
                available.add(UTF32BE::INSTANCE);
                available.add(UTF32BE_BOM::INSTANCE);
                available.add(UTF32LE::INSTANCE);
                available.add(UTF32LE_BOM::INSTANCE);
            }
            for (const Charset &cs: available) {
                if (cs.name().equalsIgnoreCase(charsetName))
                    return cs;
                for (String const& name: cs.aliases()){
                    if(name.equalsIgnoreCase(charsetName))
                        return cs;
                }
            }
            return fallback;
        }

        Charset::Charset(String canonicalName, StringArray aliases) :
                canonicalName(Unsafe::moveInstance(canonicalName)), aliasSet(Unsafe::moveInstance(aliases)) {
            if (Charset::canonicalName != "ISO-8859-1"_S && Charset::canonicalName != "US-ASCII"_S &&
                Charset::canonicalName != "UTF-8"_S) {
                CORE_TRY_RETHROW_EXCEPTION
                ({
                     checkName(Charset::canonicalName);
                     for (const String &alias: aliasSet) {
                         checkName(alias);
                     }
                 }, , __trace("core.charset.Charset"));
            }
        }

        String Charset::name() const {
            return canonicalName;
        }

        Set<String> &Charset::aliases() const {
            if (csAliasSet == null) {
                (CacheSet<String> &) aliasSet = &Set<String>::of(aliasSet);
            }
            return *csAliasSet;
        }

        String Charset::displayName() const {
            return canonicalName;
        }

        gbool Charset::isRegistered() const {
            return !canonicalName.startsWith("X-") && !canonicalName.startsWith("x-");
        }

        String Charset::displayName(const Locale &locale) const {
            CORE_IGNORE(locale);
            return canonicalName;
        }

        gbool Charset::canEncode() const {
            return true;
        }

        io::CharBuffer &Charset::decode(ByteBuffer &bb) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 return decoder()
                         .onMalformedInput(REPLACE)
                         .onUnmappableCharacter(REPLACE)
                         .decode(bb);
             },
             {
                 if (Class<CharacterCodingException>::hasInstance(ex)) {
                     Error(ex).throws(__trace("core.util.Charset"));
                 }
             }, __trace("core.charset.Charset"));
        }

        io::ByteBuffer &Charset::encode(CharBuffer &cb) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 return encoder()
                         .onMalformedInput(REPLACE)
                         .onUnmappableCharacter(REPLACE)
                         .encode(cb);
             },
             {
                 if (Class<CharacterCodingException>::hasInstance(ex)) {
                     Error(ex).throws(__trace("core.util.Charset"));
                 }
             }, __trace("core.charset.Charset"));
        }

        Charset &Charset::defaultCharset() {
            return UTF8::INSTANCE;
        }
    }
} // core