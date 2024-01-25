//
// Created by T.N.Brunshweeck on 29/12/2023.
//

#include "NativeBuffer.h"
#include <core/private/Unsafe.h>
#include <core/Long.h>
#include <core/StringBuffer.h>

namespace core {

    namespace io {

        using namespace native;

        NativeBuffer::NativeBuffer(gint size) {
            if (size < 0)
                IllegalArgumentException().throws(__trace("core.io.NativeBuffer"));
            memoryAddress = Unsafe::allocateMemory(sizeInBytes = size);
        }

        NativeBuffer::NativeBuffer(const NativeBuffer &buf) {
            memoryAddress = Unsafe::allocateMemory(sizeInBytes = buf.size());
            Unsafe::copyMemory(buf.address(), address(), buf.size());
        }

        NativeBuffer::NativeBuffer(NativeBuffer &&buf) CORE_NOTHROW {
            Unsafe::swapValues(memoryAddress, buf.memoryAddress);
            Unsafe::swapValues(sizeInBytes, buf.sizeInBytes);
        }

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
                Unsafe::swapValues(memoryAddress, buf.memoryAddress);
                Unsafe::swapValues(sizeInBytes, buf.sizeInBytes);
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
            sizeInBytes = 0;
            memoryAddress = 0;
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