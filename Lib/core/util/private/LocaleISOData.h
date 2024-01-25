//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#ifndef CORE23_LOCALEISODATA_H
#define CORE23_LOCALEISODATA_H

#include <core/util/Locale.h>

namespace core {
    namespace util {

        class LocaleISOData CORE_FINAL : public Object {
        private:
            LocaleISOData() = default;

        public:

            /**
             * The 2- and 3-letter ISO 639 language codes.
             */
            static String const ISO_LANGUAGES_TABLE;

            /**
             * The 2- and 3-letter ISO 3166 country codes.
             */
            static String const ISO_COUNTRIES_TABLE;

            /**
             * Array to hold country codes for ISO3166-3.
             */
            static StringArray const ISO3166_3;
        };

    } // util
} // core

#endif //CORE23_LOCALEISODATA_H
