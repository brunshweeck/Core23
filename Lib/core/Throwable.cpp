//
// Created by Brunshweeck on 12/09/2023.
//

#include "Throwable.h"
#include "ArgumentException.h"
#include "Character.h"
#include "Math.h"
#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    namespace {
        gint putString(const String &str, gchar in[], gint offset, gint length) {
            gint limit = str.length();
            for (gint i = 0; i < limit && offset < length; ++i) {
                in[offset++] = str.charAt(i);
            }
            in[offset] = 0;
            return offset;
        }

        gint encodeUTF16_UTF8(const gchar in[], gint limit, char out[], gint offset, gint length) {
            gint i = {};
            for (i = 0; i < limit && offset < length && in[i] < 0x80; ++i) {
                if (in[i] == 0) {
                    out[offset++] = (char) '?';
                } else
                    out[offset++] = (char) in[i];
            }
            for (; i < limit && offset < length; ++i) {
                gchar c = in[i];
                if (c < 0x80) {
                    if (c == 0) {
                        out[offset++] = (char) '?';
                    } else
                        out[offset++] = (char) in[i];
                } else if (c < 0x800) {
                    out[offset++] = (char) (0xc0 | (c >> 6));
                    out[offset++] = (char) (0x80 | (c >> 6));
                } else if (0xd800 <= c && c <= 0xdfff) {
                    if (i + 1 >= limit || in[i + 1] < 0xdc00 || in[i + 1] > 0xdfff) {
                        out[offset++] = '?';
                    } else {
                        gint ch = Character::joinSurrogates(c, in[i]);
                        out[offset++] = (char) (0xf0 | (ch >> 18));
                        out[offset++] = (char) (0xf0 | ((ch >> 12) & 0x3f));
                        out[offset++] = (char) (0xf0 | ((ch >> 6) & 0x3f));
                        out[offset++] = (char) (0xf0 | (ch & 0x3f));
                        i += 1;
                    }
                } else {
                    out[offset++] = (char) (0xe0 | (c >> 12));
                    out[offset++] = (char) (0xe0 | ((c >> 6) & 0x3f));
                    out[offset++] = (char) (0xe0 | (c & 0x3f));
                    i += 1;
                }
            }
            return offset;
        }

    }

    namespace {
        char basic_alloc[4096] = {};

        const char *StrToCStr(const String &str) {
            static gchar in[4096] = {};
            static char out[8192] = {};
            gint const i = putString(str, in, 0, 4096);
            in[i] = 0;
            gint const j = encodeUTF16_UTF8(in, i, out, 0, 8192);
            if (j < 8192){
                for (int k = 0; k < Math::min(8192 - j, 20); ++k) {
                    out[j+k] = 0;
                }
            }
            return out;
        }

    }

    Throwable::Throwable() CORE_NOTHROW:
            cse(null), stack(null), stackSize(0), isTemporary(false) {
#ifdef CORE_COMPILER_MSVC
        native::GENERIC_THROWABLE const throwable = {StrToCStr(msg), 0};
        (native::GENERIC_THROWABLE &) *this = throwable;
#endif //
    }

    Throwable::Throwable(String message) CORE_NOTHROW:
            msg(U::moveInstance(message)), cse(null), stack(null), stackSize(0), isTemporary(false) {
#ifdef CORE_COMPILER_MSVC
        native::GENERIC_THROWABLE const throwable = {StrToCStr(msg), 0};
        (native::GENERIC_THROWABLE &) *this = throwable;
#endif //
    }

    Throwable::Throwable(String message, const Throwable &cause) CORE_NOTHROW:
            msg(U::moveInstance(message)), cse(null), stack(null), stackSize(0), isTemporary(false) {
#ifdef CORE_COMPILER_MSVC
        native::GENERIC_THROWABLE const throwable = {StrToCStr(msg), 0};
        (native::GENERIC_THROWABLE &) *this = throwable;
#endif //
        setCause(cause);
    }

    Throwable::Throwable(const Throwable &thr) CORE_NOTHROW: Throwable() {
        if (thr.isTemporary) {
            msg = U::moveInstance(thr.msg);
            cse = thr.cse;
            stack = thr.stack;
            stackSize = thr.stackSize;
            CORE_CAST(Throwable &, thr).cse = null;
            CORE_CAST(Throwable &, thr).stack = null;
            CORE_CAST(Throwable &, thr).stackSize = 0;
            CORE_CAST(Throwable &, thr).isTemporary = false;
            U::destroyInstance(thr);
        } else {
            msg = thr.msg;
            if (thr.cse != null)
                cse = &U::copyInstance(*thr.cse, true);
            copyStack(thr);
        }
#ifdef CORE_COMPILER_MSVC
        native::GENERIC_THROWABLE const throwable = {StrToCStr(msg), 0};
        (native::GENERIC_THROWABLE &) *this = throwable;
#endif //
    }

    Throwable::Throwable(Throwable &&thr) CORE_NOTHROW:
            msg(U::moveInstance(thr.msg)), cse(thr.cse), stack(thr.stack), stackSize(thr.stackSize) {
        thr.cse = null;
        thr.stack = null;
        thr.stackSize = 0;
        thr.isTemporary = false;
#ifdef CORE_COMPILER_MSVC
        native::GENERIC_THROWABLE const throwable = {StrToCStr(msg), 0};
        (native::GENERIC_THROWABLE &) *this = throwable;
#endif //
    }

    String Throwable::message() const {
        return msg;
    }

    const Throwable &Throwable::cause() const {
        return *(cse == null ? this : cse);
    }

    void Throwable::setCause(const Throwable &cause) {
        if (&cause == this)
            ArgumentException("Self-causation not authorized").throws(__trace("core.Throwable"));
        cse = &U::copyInstance(cause, true);
    }

    String Throwable::toString() const {
        String cln = classname();
        if (msg.isEmpty())
            return cln;
        return cln + ": " + msg;
    }

    gbool Throwable::equals(const Object &object) const {
        if (!Class<Throwable>::hasInstance(object))
            return false;
        if (this == &object)
            return true;
        const Throwable &th = (Throwable &) object;
        if (stackSize != th.stackSize)
            return false;
        if (msg != th.msg)
            return false;
        if (classname() != th.classname())
            return false;
        if (cse == null && th.cse != null || cse != null && th.cse == null)
            return false;
        if (cse != null && th.cse != null)
            if (*cse != *th.cse)
                return false;
        for (gint i = 0; i < stackSize; ++i) {
            if (*stack[i] != *th.stack[i])
                return false;
        }
        return true;
    }

    void Throwable::throws(const Trace &trace) const {
        Throwable &th = U::copyInstance(*this);
#ifdef CORE_COMPILER_MSVC
        native::GENERIC_THROWABLE const throwable = {StrToCStr(th.msg), 0};
        (native::GENERIC_THROWABLE &) th = throwable;
#endif //
        th.isTemporary = true;
        th.updateStack(trace);
        U::moveInstance(th).raise();
        CORE_UNREACHABLE();
    }

    Throwable::~Throwable() CORE_NOTHROW {
        isTemporary = false;
        if (stackSize > 0) {
            for (gint i = 0; i < stackSize; ++i) {
                U::destroyInstance(stack[i][0]);
                stack[i] = null;
            }
            stackSize = 0;
            stack = null;
        }
        cse = null;
    }

    Throwable::PRINTSTREAM Throwable::what() const CORE_NOTHROW {
        static gchar in[4096] = {};
        static char out[8192] = {};
        gint i = {};
        for (i = 0; i < 20; ++i) {
            out[i] = '\b';
        }
        CAUSE thr = (CAUSE) this;
        while (thr != null) {
            gint limit = putString(thr->toString(), in, 0, 4096);
            i = encodeUTF16_UTF8(in, limit, out, i, 8192);
            if (thr->stack != null && thr->stackSize > 0) {
                for (gint j = stackSize - 1; j >= 0; --j) {
                    out[i++] = '\n';
                    out[i++] = '\t';
                    out[i++] = 'a';
                    out[i++] = 't';
                    out[i++] = ' ';
                    limit = putString(thr->stack[j]->toString(), in, 0, 4096);
                    i = encodeUTF16_UTF8(in, limit, out, i, 8192);
                }
            }
            out[i++] = '\n';
            out[i++] = '\t';
            out[i++] = 'a';
            out[i++] = 't';
            out[i++] = ' ';
            out[i++] = '.';
            out[i++] = '.';
            out[i++] = '.';
            if (thr->cse == null || thr->cse == thr || i >= 4096)
                break;
            out[i++] = '\n';
            out[i++] = 'C';
            out[i++] = 'a';
            out[i++] = 'u';
            out[i++] = 's';
            out[i++] = 'e';
            out[i++] = 'd';
            out[i++] = ' ';
            out[i++] = 'b';
            out[i++] = 'y';
            out[i++] = ':';
            out[i++] = ' ';
            thr = thr->cse;
        }
        out[i++] = 0;
        return out;
    }

    void Throwable::copyStack(const Throwable &thr) {
        if (thr.stackSize > 0) {
            STACKTRACE st = new STACKPOINT[thr.stackSize + 1];
            st[thr.stackSize + 1] = (STACKPOINT) 0x1;
            stackSize = 0;
            for (gint i = 0; i < thr.stackSize; ++i) {
                st[i] = &U::copyInstance(thr.stack[i][0], true);
            }
            stack = st;
            stackSize = thr.stackSize;
        } else {
            stack = null;
            stackSize = 0;
        }
    }

    void Throwable::updateStack(const Trace &trace) {
        CORE_FAST gint MAX_STACK_SIZE = 1000;
        gint newSize = 0;
        if (stackSize >= 1000) {
            newSize = 1000;
            // removing of first trace
            for (gint i = 1; i < 1000; ++i)
                stack[i] = stack[i + 1];
            stack[999] = &U::copyInstance(trace, true);
        } elif (stackSize > 0 && stack[stackSize] != (STACKPOINT) 0x1) {
            newSize = stackSize + 1;
            stack[stackSize] = &U::copyInstance(trace, true);
        } else {
            newSize = stackSize + Math::max(stackSize >> 3, 1);
            STACKTRACE st = new STACKPOINT[newSize + 1];
            for (gint i = 0; i < stackSize; ++i) {
                st[i] = stack[i];
                stack[i] = null;
            }
            st[stackSize] = &U::copyInstance(trace, true);
            stack = st;
        }
        stackSize = newSize;
    }

} // core