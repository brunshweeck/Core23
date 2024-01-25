//
// Created by T.N.Brunshweeck on 29/12/2023.
//

#ifndef CORE23_NATIVEBUFFER_H
#define CORE23_NATIVEBUFFER_H

#include <core/AutoClosable.h>

namespace core {
    namespace io {

        /**
         * A light-weight buffer in native memory.
         */
        class NativeBuffer : public Object, public AutoClosable {
        private:
            glong memoryAddress = 0;
            gint sizeInBytes = 0;

        public:
            /**
             * Create new INSTANCE of Native buffer
             */
            CORE_EXPLICIT NativeBuffer(gint size);

            /**
             * Create new INSTANCE of Native buffer by
             * copy of other buffer
             */
            NativeBuffer(const NativeBuffer &buf);

            /**
             * Create new INSTANCE of Native buffer by
             * swapping with other buffer
             */
            NativeBuffer(NativeBuffer &&buf) CORE_NOTHROW;

            /**
             * Set this INSTANCE of Native buffer by the
             * copy of other buffer
             */
            NativeBuffer &operator=(const NativeBuffer &buf);

            /**
             * Swap this INSTANCE of Native buffer by
             * with with other buffer
             */
            NativeBuffer &operator=(NativeBuffer &&buf) CORE_NOTHROW;

            /**
             * Return this buffer address
             */
            glong address() const;

            /**
             * Return number of bytes allocated on this buffer
             */
            gint size() const;

            /**
             * Free memory allocated by this buffer
             */
             void free();

            void close() override;

            gbool equals(const Object &o) const override;

            Object &clone() const override;

            String toString() const override;

            gint hash() const override;

            ~NativeBuffer() override;
        };

    } // io
} // core

#endif //CORE23_NATIVEBUFFER_H
