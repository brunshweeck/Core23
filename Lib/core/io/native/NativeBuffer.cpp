//
// Created by T.N.Brunshweeck on 29/12/2023.
//

#include "NativeBuffer.h"
#include <core/private/Unsafe.h>
#include <core/Long.h>
#include <core/StringBuffer.h>

namespace core {
    using namespace native;

    namespace io {
        NativeBuffer::NativeBuffer(gint size) : sizeInBytes(0), memoryAddress(0) {
            try {
                memoryAddress = Unsafe::allocateMemory(memoryAddress = size);
            } catch (const Throwable &ex) { ex.throws(__trace("core.io.NativeBuffer")); }
        }

        NativeBuffer::NativeBuffer(const NativeBuffer &buf) : sizeInBytes(0), memoryAddress(0) {
            try {
                memoryAddress = Unsafe::allocateMemory(sizeInBytes = buf.size());
                Unsafe::copyMemory(buf.address(), address(), buf.size());
            } catch (const Throwable &ex) { ex.throws(__trace("core.io.NativeBuffer")); }
        }

        NativeBuffer::NativeBuffer(NativeBuffer &&buf) CORE_NOTHROW:
                sizeInBytes(buf.sizeInBytes + (buf.sizeInBytes -= buf.sizeInBytes)),
                memoryAddress(buf.memoryAddress + (buf.memoryAddress -= buf.memoryAddress)) {}

        NativeBuffer &NativeBuffer::operator=(const NativeBuffer &buf) {
            if (this != &buf) {
                if (sizeInBytes - buf.sizeInBytes <= CORE_PROCESSOR_WORDSIZE && buf.sizeInBytes > 0) {
                    sizeInBytes = buf.sizeInBytes;
                } else {
                    free();
                    try {
                        memoryAddress = Unsafe::allocateMemory(sizeInBytes = buf.sizeInBytes);
                    } catch (const Throwable &ex) { ex.throws(__trace("core.io.NativeBuffer")); }
                }
                Unsafe::copyMemory(buf.memoryAddress, memoryAddress, buf.sizeInBytes);
            }
            return *this;
        }

        NativeBuffer &NativeBuffer::operator=(NativeBuffer &&buf) CORE_NOTHROW {
            if (this != &buf) {
                sizeInBytes = buf.sizeInBytes + ((buf.sizeInBytes = sizeInBytes) - sizeInBytes);
                memoryAddress = buf.memoryAddress + ((buf.memoryAddress = memoryAddress) - memoryAddress);
            }
            return *this;
        }

        glong NativeBuffer::address() const {
            return memoryAddress;
        }

        gint NativeBuffer::size() const {
            return sizeInBytes;
        }

        void NativeBuffer::free() {
            if (sizeInBytes > 0 && memoryAddress != 0) {
                Unsafe::freeMemory(memoryAddress);
            }
            sizeInBytes = 0;
            memoryAddress = 0;
        }

        void NativeBuffer::close() {
            if (sizeInBytes > 0 && memoryAddress != 0) {
                free();
            }
        }

        gbool NativeBuffer::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<Object>::hasInstance(o))
                return false;
            const NativeBuffer &other = (const NativeBuffer &) o;
            return memoryAddress == other.memoryAddress && sizeInBytes == other.sizeInBytes;
        }

        Object &NativeBuffer::clone() const {
            return Object::clone();
        }

        String NativeBuffer::toString() const {
            return StringBuffer(32)
                    .append("NativeBuffer[0x")
                    .append(Long::toHexString(memoryAddress))
                    .append(", ")
                    .append(sizeInBytes)
                    .append(sizeInBytes == 1 ? "byte" : "bytes")
                    .toString();

        }

        gint NativeBuffer::hash() const {
            return Long::hash(address());
        }

        NativeBuffer::~NativeBuffer() {
            if (sizeInBytes > 0 && memoryAddress != 0) {
                free();
            }
        }

    } // io
} // core