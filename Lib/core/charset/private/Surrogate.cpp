//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#include "Surrogate.h"

namespace core {
    namespace charset {
        gbool Surrogate::isHigh(gint c) {
            return (MIN_HIGH <= c) && (c <= MAX_HIGH);
        }

        gbool Surrogate::isLow(gint c) {
            return (MIN_LOW <= c) && (c <= MAX_LOW);
        }

        gbool Surrogate::is(gint c) {
            return (MIN <= c) && (c <= MAX);
        }

        gbool Surrogate::neededFor(gint uc) {
            return Character::isSupplementary(uc);
        }

        gchar Surrogate::high(gint uc) {
            // ? Character::isSupplementary(uc);
            return Character::highSurrogate(uc);
        }

        gchar Surrogate::low(gint uc) {
            // ?Character::isSupplementary(uc);
            return Character::lowSurrogate(uc);
        }

        gint Surrogate::toUCS4(gchar c, gchar d) {
            // ? Character::isHighSurrogate(c) && Character::isLowSurrogate(d);
            return Character::joinSurrogates(c, d);
        }

        gint Surrogate::Parser::character() const {
            return chr;
        }

        gbool Surrogate::Parser::isPair() const {
            return pair;
        }

        gint Surrogate::Parser::increment() const {
            return pair ? 2 : 1;
        }

        CoderResult Surrogate::Parser::error() const {
            return err;
        }

        CoderResult Surrogate::Parser::unmappableResult() const {
            return CoderResult::unmappableForLength(pair ? 2 : 1);
        }

        gint Surrogate::Parser::parse(gchar c, CharBuffer &in) {
            if (Character::isHighSurrogate(c)) {
                if (!in.hasRemaining()) {
                    err = CoderResult::UNDERFLOW;
                    return -1;
                }
                gchar const d = in.get();
                if (Character::isLowSurrogate(d)) {
                    chr = Character::joinSurrogates(c, d);
                    pair = true;
                    err = CoderResult::UNDERFLOW; // null
                    return chr;
                }
                err = CoderResult::malformedForLength(1);
                return -1;
            }
            if (Character::isLowSurrogate(c)) {
                err = CoderResult::malformedForLength(1);
                return -1;
            }
            chr = c;
            pair = false;
            err = CoderResult::UNDERFLOW; // null
            return chr;
        }

        gint Surrogate::Parser::parse(gchar c, const CharArray &ia, gint ip, gint il) {
            // ?(ia[ip] == c);
            if (Character::isHighSurrogate(c)) {
                if (il - ip < 2) {
                    err = CoderResult::UNDERFLOW;
                    return -1;
                }
                gchar const d = ia[ip + 1];
                if (Character::isLowSurrogate(d)) {
                    chr = Character::joinSurrogates(c, d);
                    pair = true;
                    err = CoderResult::UNDERFLOW; // null
                    return chr;
                }
                err = CoderResult::malformedForLength(1);
                return -1;
            }
            if (Character::isLowSurrogate(c)) {
                err = CoderResult::malformedForLength(1);
                return -1;
            }
            chr = c;
            pair = false;
            err = CoderResult::UNDERFLOW; // null
            return chr;
        }

        CoderResult Surrogate::Generator::error() const {
            // ?error != null;
            return err;
        }

        gint Surrogate::Generator::generate(gint uc, gint len, CharBuffer &dst) {
            if (Character::isBMP(uc)) {
                gchar const c = (gchar) uc;
                if (Character::isSurrogate(c)) {
                    err = CoderResult::malformedForLength(len);
                    return -1;
                }
                if (dst.remaining() < 1) {
                    err = CoderResult::OVERFLOW;
                    return -1;
                }
                dst.put(c);
                err = CoderResult::UNDERFLOW; // null
                return 1;
            } else if (Character::isValidCodePoint(uc)) {
                if (dst.remaining() < 2) {
                    err = CoderResult::OVERFLOW;
                    return -1;
                }
                dst.put(Character::highSurrogate(uc));
                dst.put(Character::lowSurrogate(uc));
                err = CoderResult::UNDERFLOW; // null
                return 2;
            } else {
                err = CoderResult::unmappableForLength(len);
                return -1;
            }
        }

        gint Surrogate::Generator::generate(gint uc, gint len, CharArray &da, gint dp, gint dl) {
            if (Character::isBMP(uc)) {
                gchar const c = (gchar) uc;
                if (Character::isSurrogate(c)) {
                    err = CoderResult::malformedForLength(len);
                    return -1;
                }
                if (dl - dp < 1) {
                    err = CoderResult::OVERFLOW;
                    return -1;
                }
                da[dp] = c;
                err = CoderResult::UNDERFLOW; // null
                return 1;
            } else if (Character::isValidCodePoint(uc)) {
                if (dl - dp < 2) {
                    err = CoderResult::OVERFLOW;
                    return -1;
                }
                da[dp] = Character::highSurrogate(uc);
                da[dp + 1] = Character::lowSurrogate(uc);
                err = CoderResult::UNDERFLOW; // null
                return 2;
            } else {
                err = CoderResult::unmappableForLength(len);
                return -1;
            }
        }
    } // charset
} // core