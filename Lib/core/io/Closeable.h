//
// Created by T.N.Brunshweeck on 29/12/2023.
//

#ifndef CORE23_CLOSEABLE_H
#define CORE23_CLOSEABLE_H

#include <core/AutoClosable.h>

namespace core {
    namespace io {

        /**
         * A <b> Closeable</b>  is a source or destination of data that can be closed.
         * The close method is invoked to release resources that the object is
         * holding (such as open files).
         */
        interface Closeable : public AutoClosable {
        public:

            /**
             * Closes this stream and releases any system resources associated
             * with it. If the stream is already closed then invoking this
             * method has no effect.
             *
             * <p> As noted in <b style="color:orange;"> AutoCloseable#close()</b> , cases where the
             * close may fail require careful attention. It is strongly advised
             * to relinquish the underlying resources and to internally
             * <em>mark</em> the <b> Closeable</b>  as closed, prior to throwing
             * the <b> IOException</b> .
             *
             * @throws IOException if an I/O error occurs
             */
            void close() override = 0;
        };

    } // io
} // core

#endif //CORE23_CLOSEABLE_H
