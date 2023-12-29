//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_NULL_H
#define CORE23_NULL_H

#include <core/Object.h>

namespace core {

    /**
     * The class Null represent null address pointer (nullptr in c++) and
     * null Object (for json object, ...)
     */
    class Null CORE_FINAL : public Object {
        /**
         * Construct new null object.
         * Note: This constructor is called one time only
         */
        CORE_FAST Null() = default;

    public:
        gbool equals(const Object &o) const override;

        Object &clone() const override;

        String toString() const override;

        template<class T>
        CORE_FAST CORE_ENABLE_IMPLICIT_CAST(T *, 0, const);

        static Null INSTANCE;

    };

    extern Null &null;

} // core

#endif //CORE23_NULL_H
