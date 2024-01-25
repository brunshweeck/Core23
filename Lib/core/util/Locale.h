//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_LOCALE_H
#define CORE23_LOCALE_H

#include <core/native/StringArray.h>
#include <core/native/StringArray2D.h>
#include <core/util/Optional.h>
#include <core/util/ArrayList.h>
#include <core/util/HashMap.h>
#include <core/util/StringTokenizer.h>
#include <core/AssertionError.h>
#include <core/StringBuffer.h>

namespace core {

    namespace util {

        /**
         * A <b> Locale</b>  object represents a specific geographical, political,
         * or cultural region. An operation that requires a <b> Locale</b>  to perform
         * its task is called <em>locale-sensitive</em> and uses the <b> Locale</b> 
         * to tailor information for the user. For example, displaying a number
         * is a locale-sensitive operation&mdash; the number should be formatted
         * according to the customs and conventions of the user's native country,
         * region, or culture.
         *
         * <p> The <b> Locale</b>  class implements IETF BCP 47 which is composed of
         * <a href="https://tools.ietf.org/html/rfc4647">RFC 4647 "Matching of Language
         * Tags"</a> and <a href="https://tools.ietf.org/html/rfc5646">RFC 5646 "Tags
         * for Identifying Languages"</a> with support for the LDML (UTS#35, "Unicode
         * Locale Data Markup Language") BCP 47-compatible extensions for locale data
         * exchange.
         *
         * <p> A <b> Locale</b>  object logically consists of the fields
         * described below.
         *
         * <dl>
         *   <dt><a id="def_language"><b>language</b></a></dt>
         *
         *   <dd>ISO 639 alpha-2 or alpha-3 language code, or registered
         *   language subtags up to 8 alpha letters (for future enhancements).
         *   When a language has both an alpha-2 code and an alpha-3 code, the
         *   alpha-2 code must be used.  You can find a full list of valid
         *   language codes in the IANA Language Subtag Registry (search for
         *   "Type: language").  The language field is case insensitive, but
         *   <b> Locale</b>  always canonicalizes to lower case.</dd>
         *
         *   <dd>Well-formed language values have the form
         *   <code>[a-zA-Z]{2,8}</code>.  Note that this is not the full
         *   BCP47 language production, since it excludes extlang.  They are
         *   not needed since modern three-letter language codes replace
         *   them.</dd>
         *
         *   <dd>Example: "en" (English), "ja" (Japanese), "kok" (Konkani)</dd>
         *
         *   <dt><a id="def_script"><b>script</b></a></dt>
         *
         *   <dd>ISO 15924 alpha-4 script code.  You can find a full list of
         *   valid script codes in the IANA Language Subtag Registry (search
         *   for "Type: script").  The script field is case insensitive, but
         *   <b> Locale</b>  always canonicalizes to title case (the first
         *   letter is upper case and the rest of the letters are lower
         *   case).</dd>
         *
         *   <dd>Well-formed script values have the form
         *   <code>[a-zA-Z]{4}</code></dd>
         *
         *   <dd>Example: "Latn" (Latin), "Cyrl" (Cyrillic)</dd>
         *
         *   <dt><a id="def_region"><b>country (region)</b></a></dt>
         *
         *   <dd>ISO 3166 alpha-2 country code or UN M.49 numeric-3 area code.
         *   You can find a full list of valid country and region codes in the
         *   IANA Language Subtag Registry (search for "Type: region").  The
         *   country (region) field is case insensitive, but
         *   <b> Locale</b>  always canonicalizes to upper case.</dd>
         *
         *   <dd>Well-formed country/region values have
         *   the form <code>[a-zA-Z]{2} | [0-9]{3}</code></dd>
         *
         *   <dd>Example: "US" (United States), "FR" (France), "029"
         *   (Caribbean)</dd>
         *
         *   <dt><a id="def_variant"><b>variant</b></a></dt>
         *
         *   <dd>Any arbitrary value used to indicate a variation of a
         *   <b> Locale</b> .  Where there are two or more variant values
         *   each indicating its own semantics, these values should be ordered
         *   by importance, with most important first, separated by
         *   underscore('_').  The variant field is case sensitive.</dd>
         *
         *   <dd>Note: IETF BCP 47 places syntactic restrictions on variant
         *   subtags.  Also BCP 47 subtags are strictly used to indicate
         *   additional variations that define a language or its dialects that
         *   are not covered by any combinations of language, script and
         *   region subtags.  You can find a full list of valid variant codes
         *   in the IANA Language Subtag Registry (search for "Type: variant").
         *
         *   <p>However, the variant field in <b> Locale</b>  has
         *   historically been used for any kind of variation, not just
         *   language variations.  For example, some supported variants
         *   available in Runtime Environments indicate alternative
         *   cultural behaviors such as calendar type or number script.  In
         *   BCP 47 this kind of information, which does not identify the
         *   language, is supported by extension subtags or private use
         *   subtags.</dd>
         *
         *   <dd>Well-formed variant values have the form <code>SUBTAG
         *   (('_'|'-') SUBTAG)*</code> where <code>SUBTAG =
         *   [0-9][0-9a-zA-Z]{3} | [0-9a-zA-Z]{5,8}</code>. (Note: BCP 47 only
         *   uses hyphen ('-') as a delimiter, this is more lenient).</dd>
         *
         *   <dd>Example: "polyton" (Polytonic Greek), "POSIX"</dd>
         *
         *   <dt><a id="def_extensions"><b>extensions</b></a></dt>
         *
         *   <dd>A map from single character keys to string values, indicating
         *   extensions apart from language identification.  The extensions in
         *   <b> Locale</b>  implement the semantics and syntax of BCP 47
         *   extension subtags and private use subtags. The extensions are
         *   case insensitive, but <b> Locale</b>  canonicalizes all
         *   extension keys and values to lower case. Note that extensions
         *   cannot have empty values.</dd>
         *
         *   <dd>Well-formed keys are single characters from the set
         *   <b> [0-9a-zA-Z]</b> .  Well-formed values have the form
         *   <b> SUBTAG ('-' SUBTAG)*</b>  where for the key 'x'
         *   <code>SUBTAG = [0-9a-zA-Z]{1,8}</code> and for other keys
         *   <code>SUBTAG = [0-9a-zA-Z]{2,8}</code> (that is, 'x' allows
         *   single-character subtag).</dd>
         *
         *   <dd>Example: key="u"/value="ca-japanese" (Japanese Calendar)</dd>
         * </dl>
         *
         * <b>Note:</b> Although BCP 47 requires field values to be registered
         * in the IANA Language Subtag Registry, the <b> Locale</b>  class
         * does not provide any validation features.
         *
         * <h2><a id="">Unicode locale/language extension</a></h2>
         *
         * <p>UTS#35, "Unicode Locale Data Markup Language" defines optional
         * attributes and keywords to override or refine the default behavior
         * associated with a locale.  A keyword is represented by a pair of
         * key and type.  For example, "nu-thai" indicates that Thai local
         * digits (value:"thai") should be used for formatting numbers
         * (key:"nu").
         *
         * <p>The keywords are mapped to a BCP 47 extension value using the
         * extension key 'u' (<b style="color: orange;"> UNICODE_EXTENSION_KEY</b> ).  The above
         * example, "nu-thai", becomes the extension "u-nu-thai".
         *
         * <p>Thus, when a <b> Locale</b>  object contains Unicode locale
         * attributes and keywords,
         * <b> Locale::extension(UNICODE_EXTENSION_KEY)</b>  will return a
         * String representing this information, for example, "nu-thai".  The
         * <b> Locale</b>  class also provides <b style="color: orange;"> 
         * Locale::unicodeLocaleAttributes</b> , <b style="color: orange;"> Locale::unicodeLocaleKeys</b> , and
         * <b style="color: orange;"> Locale::unicodeLocaleType</b>  which allow you to access Unicode
         * locale attributes and key/type pairs directly.  When represented as
         * a string, the Unicode Locale Extension lists attributes
         * alphabetically, followed by key/type sequences with keys listed
         * alphabetically (the order of subtags comprising a key's type is
         * fixed when the type is defined)
         *
         * <p>A well-formed locale key has the form
         * <code>[0-9a-zA-Z]{2}</code>.  A well-formed locale type has the
         * form <code>"" | [0-9a-zA-Z]{3,8} ('-' [0-9a-zA-Z]{3,8})*</code> (it
         * can be empty, or a series of subtags 3-8 alphanums in length).  A
         * well-formed locale attribute has the form
         * <code>[0-9a-zA-Z]{3,8}</code> (it is a single subtag with the same
         * form as a locale type subtag).
         *
         * <p>The Unicode locale extension specifies optional behavior in
         * locale-sensitive services.  Although the LDML specification defines
         * various keys and values, actual locale-sensitive service
         * implementations in a Runtime Environment might not support any
         * particular Unicode locale attributes or key/type pairs.
         *
         * <h3><a id="ObtainingLocale">Obtaining a Locale</a></h3>
         *
         * <p>There are several ways to obtain a <b> Locale</b> 
         * object.
         *
         * <h4>Factory Methods</h4>
         *
         * <p>The method <b style="color: orange;"> Locale::forLanguageTag</b>  obtains a <b> Locale</b>
         * object for a well-formed BCP 47 language tag. The method
         * <b style="color: orange;"> Locale::of(String, String, String)</b>  and its overloads obtain a
         * <b> Locale</b>  object from given <b> language</b> , <b> country</b> ,
         * and/or <b> variant</b>  defined above.
         *
         * <h4>Locale Constants</h4>
         *
         * <p>The <b> Locale</b>  class provides a number of convenient constants
         * that you can use to obtain <b> Locale</b>  objects for commonly used
         * locales. For example, <b> Locale::US</b>  is the <b> Locale</b>  object
         * for the United States.
         *
         * <h3><a id="LocaleMatching">Locale Matching</a></h3>
         *
         * <p>If an application or a system is internationalized and provides localized
         * resources for multiple locales, it sometimes needs to find one or more
         * locales (or language tags) which meet each user's specific preferences. Note
         * that a term "language tag" is used interchangeably with "locale" in this
         * locale matching documentation.
         *
         * <p>In order to do matching a user's preferred locales to a set of language
         * tags, <a href="https://tools.ietf.org/html/rfc4647">RFC 4647 Matching of
         * Language Tags</a> defines two mechanisms: filtering and lookup.
         * <em>Filtering</em> is used to get all matching locales, whereas
         * <em>lookup</em> is to choose the best matching locale.
         * Matching is done case-insensitively. These matching mechanisms are described
         * in the following sections.
         *
         * <p>A user's preference is called a <em>Language Priority List</em> and is
         * expressed as a list of language ranges. There are syntactically two types of
         * language ranges: basic and extended. See
         * <b style="color: orange;"> Locale::LanguageRange</b>  for details.
         *
         * <h4>Filtering</h4>
         *
         * <p>The filtering operation returns all matching language tags. It is defined
         * in RFC 4647 as follows:
         * "In filtering, each language range represents the least specific language
         * tag (that is, the language tag with fewest number of subtags) that is an
         * acceptable match. All of the language tags in the matching set of tags will
         * have an equal or greater number of subtags than the language range. Every
         * non-wildcard subtag in the language range will appear in every one of the
         * matching language tags."
         *
         * <p>There are two types of filtering: filtering for basic language ranges
         * (called "basic filtering") and filtering for extended language ranges
         * (called "extended filtering"). They may return different results by what
         * kind of language ranges are included in the given Language Priority List.
         * <b style="color: orange;"> Locale::FilteringMode</b>  is a parameter to specify how filtering should
         * be done.
         *
         * <h4>Lookup</h4>
         *
         * <p>The lookup operation returns the best matching language tags. It is
         * defined in RFC 4647 as follows:
         * "By contrast with filtering, each language range represents the most
         * specific tag that is an acceptable match.  The first matching tag found,
         * according to the user's priority, is considered the closest match and is the
         * item returned."
         *
         * <p>For example, if a Language Priority List consists of two language ranges,
         * <b> "zh-Hant-TW"</b>  and <b> "en-US"</b> , in prioritized order, lookup
         * method progressively searches the language tags below in order to find the
         * best matching language tag.
         * <blockquote>
         * <pre>
         *    1. zh-Hant-TW
         *    2. zh-Hant
         *    3. zh
         *    4. en-US
         *    5. en
         * </pre>
         * </blockquote>
         * If there is a language tag which matches completely to a language range
         * above, the language tag is returned.
         *
         * <p><b> "*"</b>  is the special language range, and it is ignored in lookup.
         *
         * <p>If multiple language tags match as a result of the subtag <b> '*'</b> 
         * included in a language range, the first matching language tag returned by
         * an <b style="color: orange;"> Iterator</b>  over a <b style="color: orange;"> Collection</b>  of language tags is treated as
         * the best matching one.
         *
         * <h3>Use of Locale</h3>
         *
         * <p>Once you've obtained a <b> Locale</b>  you can query it for information
         * about itself. Use <b> getCountry</b>  to get the country (or region)
         * code and <b> Locale::language</b>  to get the language code.
         * You can use <b> Locale::displayCountry</b>  to get the
         * name of the country suitable for displaying to the user. Similarly,
         * you can use <b> Locale::displayLanguage</b>  to get the name of
         * the language suitable for displaying to the user. Interestingly,
         * the <b> Locale::displayXXX</b>  methods are themselves locale-sensitive
         * and have two versions: one that uses the default
         * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale and one
         * that uses the locale specified as an argument.
         *
         * <p>The Platform provides a number of classes that perform locale-sensitive
         * operations. For example, the <b> NumberFormat</b>  class formats
         * numbers, currency, and percentages in a locale-sensitive manner. Classes
         * such as <b> NumberFormat</b>  have several convenience methods
         * for creating a default object of that type. For example, the
         * <b> NumberFormat</b>  class provides these three convenience methods
         * for creating a default <b> NumberFormat</b>  object:
         * <blockquote>
         * <pre>
         *     NumberFormat.newInstance()
         *     NumberFormat.newCurrencyInstance()
         *     NumberFormat.newPercentInstance()
         * </pre>
         * </blockquote>
         * Each of these methods has two variants; one with an explicit locale
         * and one without; the latter uses the default
         * <b style="color: orange;"> Locale::Category::FORMAT</b>  locale:
         * <blockquote>
         * <pre>
         *     NumberFormat.newInstance(myLocale)
         *     NumberFormat.newCurrencyInstance(myLocale)
         *     NumberFormat.newPercentInstance(myLocale)
         * </pre>
         * </blockquote>
         * A <b> Locale</b>  is the mechanism for identifying the kind of object
         * (<b> NumberFormat</b> ) that you would like to get. The locale is
         * <STRONG>just</STRONG> a mechanism for identifying objects,
         * <STRONG>not</STRONG> a container for the objects themselves.
         *
         * <h3>Compatibility</h3>
         *
         * <p>In order to maintain compatibility, Locale's
         * constructors retain their behavior prior to the Runtime
         * Environment.  The same is largely true for the
         * <b> toString</b>  method. Thus Locale objects can continue to
         * be used as they were. In particular, clients who parse the output
         * of toString into language, country, and variant fields can continue
         * to do so (although this is strongly discouraged), although the
         * variant field will have additional information in it if script or
         * extensions are present.
         *
         * <p>In addition, BCP 47 imposes syntax restrictions that are not
         * imposed by Locale's constructors. This means that conversions
         * between some Locales and BCP 47 language tags cannot be made without
         * losing information. Thus <b> toLanguageTag</b>  cannot
         * represent the state of locales whose language, country, or variant
         * do not conform to BCP 47.
         *
         * <p>Because of these issues, it is recommended that clients migrate
         * away from constructing non-conforming locales and use the
         * <b> LocaleforLanguageTag</b>.
         * Clients desiring a string representation of the complete locale can
         * then always rely on <b> toLanguageTag</b>  for this purpose.
         *
         * <h4><a id="">Special cases</a></h4>
         *
         * <p>For compatibility reasons, two
         * non-conforming locales are treated as special cases.  These are
         * <b><b> ja__JP_JP</b> </b> and <b><b> th__TH_TH</b> </b>. These are ill-formed
         * in BCP 47 since the variants are too short. To ease migration to BCP 47,
         * these are treated specially during construction.
         *
         * <p>Locale has used <b> ja__JP_JP</b>  to represent Japanese as used in
         * Japan together with the Japanese Imperial calendar. This is now
         * representable using a Unicode locale extension, by specifying the
         * Unicode locale key <b> ca</b>  (for "calendar") and type
         * <b> japanese</b> . When the Locale constructor is called with the
         * arguments "ja", "JP", "JP", the extension "u-ca-japanese" is
         * automatically added.
         *
         * <p>Locale has used <b> th__TH_TH</b>  to represent Thai as used in
         * Thailand together with Thai digits. This is also now representable using
         * a Unicode locale extension, by specifying the Unicode locale key
         * <b> nu</b>  (for "number") and value <b> thai</b> . When the Locale
         * constructor is called with the arguments "th", "TH", "TH", the
         * extension "u-nu-thai" is automatically added.
         *
         * <h4>Serialization</h4>
         *
         * <p>During serialization, writeObject writes all fields to the output
         * stream, including extensions.
         *
         * <p>During deserialization, readResolve adds extensions as described
         * in <a href="">Special Cases</a>, only
         * for the two cases th__TH_TH and ja__JP_JP.
         *
         * <h4><a id="">Legacy language codes</a></h4>
         *
         * <p>Locale's constructor has always converted three language codes to
         * their earlier, obsoleted forms: <b> he</b>  maps to <b> iw</b> ,
         * <b> yi</b>  maps to <b> ji</b> , and <b> id</b>  maps to
         * <b> in</b> . Each language maps to its new form; <b> iw</b>  maps to <b> he</b> ,
         * <b> ji</b> maps to <b> yi</b> , and <b> in</b>  maps to <b> id</b> .
         *
         * <p>The APIs added map between the old and new language codes,
         * maintaining the mapped codes internal to Locale,
         * but using the new codes in the BCP 47 language tag APIs (so
         * that <b> toLanguageTag</b>  reflects the new one). This
         * preserves the equivalence between Locales no matter which code or
         * API is used to construct them.
         *
         * <h4>Three-letter language/country(region) codes</h4>
         *
         * <p>The Locale constructors have always specified that the language
         * and the country param be two characters in length, although in
         * practice they have accepted any length.  The specification has now
         * been relaxed to allow language codes of two to eight characters and
         * country (region) codes of two to three characters, and in
         * particular, three-letter language codes and three-digit region
         * codes as specified in the IANA Language Subtag Registry.  For
         * compatibility, the implementation still does not impose a length
         * constraint.
         *
         * @see core.text.Format
         * @see core.text.NumberFormat
         * @see core.text.Collator
         * @author Brunshweeck Tazeussong
         */
        class Locale CORE_FINAL : public Object {
        private:
            /**
             * The internal locale
             */
            class BaseLocale CORE_FINAL : public Object {
            public:
                static const String SEP; // "_"

                String language;
                String script;
                String region;
                String variant;
                gint hashcode = 0;

                CORE_EXPLICIT BaseLocale(String language, String script, String region, String variant);

                static String convertOldISOCode(const String &language);

                gbool equals(const Object &o) const override;

                String toString() const override;

                gint hash() const override;

                Object &clone() const override;

                static BaseLocale of(const String &language);

                static BaseLocale of(const String &language, const String &region);
            };

            // The private locale class dependencies
            class LanguageTag;

            class LocaleBuilder;

            /**
             * The locale property
             */
            BaseLocale baseLocale;
            HashMap<Character, String> localeExtensions;

        public:

            /**
             * Useful constant for language.
             */
            static const Locale ENGLISH;

            /**
             * Useful constant for language.
             */
            static const Locale FRENCH;

            /**
             * Useful constant for language.
             */
            static const Locale GERMAN;

            /**
             * Useful constant for language.
             */
            static const Locale ITALIAN;

            /**
             * Useful constant for language.
             */
            static const Locale JAPANESE;

            /**
             * Useful constant for language.
             */
            static const Locale KOREAN;

            /**
             * Useful constant for language.
             */
            static const Locale CHINESE;

            /**
             * Useful constant for language.
             */
            static const Locale SIMPLIFIED_CHINESE;

            /**
             * Useful constant for language.
             */
            static const Locale TRADITIONAL_CHINESE;

            /**
             * Useful constant for country.
             */
            static const Locale FRANCE;

            /**
             * Useful constant for country.
             */
            static const Locale GERMANY;

            /**
             * Useful constant for country.
             */
            static const Locale ITALY;

            /**
             * Useful constant for country.
             */
            static const Locale JAPAN;

            /**
             * Useful constant for country.
             */
            static const Locale KOREA;

            /**
             * Useful constant for country.
             */
            static const Locale UK;

            /**
             * Useful constant for country.
             */
            static const Locale US;

            /**
             * Useful constant for country.
             */
            static const Locale CANADA;

            /**
             * Useful constant for country.
             */
            static const Locale CANADA_FRENCH;

            /**
             * Useful constant for the root locale.  The root locale is the locale whose
             * language, country, and script are empty ("") strings.  This is regarded
             * as the base locale of all locales, and is used as the language/country
             * neutral locale for the locale sensitive operations.
             */
            static const Locale ROOT;

            /**
             * Useful constant for country.
             */
            static const Locale CHINA;

            /**
             * Useful constant for country.
             */
            static const Locale PRC;

            /**
             * Useful constant for country.
             */
            static const Locale TAIWAN;

        private:

            /**
             * Construct a locale from a language code.
             * This constructor normalizes the language value to lowercase.
             * @implNote
             * <ul>
             * <li>Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms. See <a href="">Legacy language
             * codes</a> for more information.
             * <li>For backward compatibility reasons, this constructor does not make
             * any syntactic checks on the input.
             * </ul>
             *
             * @deprecated Locale constructors have been deprecated. See <a href="">
             * Obtaining a Locale</a> for other options.
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the <b> Locale</b>  class description about
             * valid language values.
             */
            CORE_EXPLICIT Locale(String language);

            /**
             * Construct a locale from language and country.
             * This constructor normalizes the language value to lowercase and
             * the country value to uppercase.
             * @implNote
             * <ul>
             * <li>Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms. See <a href="">Legacy language codes</a> for more information.
             * <li>For backward compatibility reasons, this constructor does not make
             * any syntactic checks on the input.
             * </ul>
             *
             * @deprecated Locale constructors have been deprecated. See <a href="">
             * Obtaining a Locale</a> for other options.
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the <b> Locale</b>  class description about
             * valid language values.
             * @param country An ISO 3166 alpha-2 country code or a UN M.49 numeric-3 area code.
             * See the <b> Locale</b>  class description about valid country values.
             */
            CORE_EXPLICIT Locale(String language, String country);

            /**
             * Construct a locale from language, country and script.
             * This constructor normalizes the language value to lowercase and
             * the country value to uppercase.
             * @note
             * <ul>
             * <li>Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms. See <a href="">Legacy language
             * codes</a> for more information.
             * <li>For backward compatibility reasons, this constructor does not make
             * any syntactic checks on the input.
             * <li>The two cases ("ja", "JP", "JP") and ("th", "TH", "TH") are handled specially,
             * see <a href="">Special Cases</a> for more information.
             * </ul>
             *
             * @deprecated Locale constructors have been deprecated. See <a href ="">
             * Obtaining a Locale</a> for other options.
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the <b> Locale</b> class description about
             * valid language values.
             * @param script An ISO 15924 alpha-4 script code.
             * See the <b> Locale</b> class description for the details.
             * @param country An ISO 3166 alpha-2 country code or a UN M.49 numeric-3 area code.
             * See the <b> Locale</b> class description about valid country values.
             */
            CORE_EXPLICIT Locale(String language, String script, String country);

            /**
             * Private constructor used by createInstance method
             */
            CORE_EXPLICIT Locale(BaseLocale baseLocale, const Map<Character, String> &extensions);

            /**
             * Returns a <b> Locale</b>  constructed from the given
             * <b> language</b> , <b> country</b>  and <b> script</b> .
             *
             * @param language lowercase 2 to 8 language code.
             * @param script The script code
             * @param country uppercase two-letter ISO-3166 code and numeric-3 UN M.49 area code.
             * @return the <b> Locale</b>  instance requested
             */
            static Locale createLocale(const String &language, const String &script, const String &country,
                                       const String &variant);

            static Locale createLocale(const String &language, const String &script, const String &country,
                                       const String &variant, const Map<Character, String> &extensions);

            static Locale createLocale(const String &language, const String &country);

            static Locale createLocale(const BaseLocale &baseLocale, const Map<Character, String> &localeExtensions);

        public:

            /**
             * Gets the current value of the default locale for this instance
             * of the Machine system.
             *
             * @return the default locale for this instance of the Machine system.
             */
            static Locale system();

            /**
             * Gets the current value of the default locale for this instance
             * of the process.
             *
             * @return the default locale for this instance of the process.
             */
            static Locale defaultLocale();

            /**
             * Enum for locale categories.  These locale categories are used to get/set
             * the default locale for the specific functionality represented by the
             * category.
             */
            enum class Category : gbyte {

                /**
                 * Category used to represent the default locale for
                 * displaying user interfaces.
                 */
                DISPLAY,

                /**
                 * Category used to represent the default locale for
                 * formatting dates, numbers, and/or currencies.
                 */
                FORMAT,
            };

            /**
             * Gets the current value of the default locale for the specified Category
             * for this instance of the system.
             *
             * @param category the specified category to get the default locale
             * @return the default locale for the specified Category for this instance
             *     of the system.
             */
            static Locale system(Category category);

            /**
             * Gets the current value of the default locale for the specified Category
             * for this instance of the current process.
             *
             * @param category the specified category to get the default locale
             * @return the default locale for the specified Category for this instance
             *     of the current process.
             */
            static Locale defaultLocale(Category category);

            /**
             * Sets the default locale for this instance of the process.
             * This does not affect the host locale.
             * <p>
             * Since changing the default locale may affect many different areas
             * of functionality, this method should only be used if the caller
             * is prepared to reinitialize locale-sensitive code running
             * within the same process.
             * <p>
             * By setting the default locale with this method, all of the default
             * locales for each Category are also set to the specified default locale.
             *
             * @throws SecurityException if a security manager exists and its
             *        <b> checkPermission</b>  method doesn't allow the operation.
             * @param newLocale the new default locale
             */
            static void setDefaultLocale(const Locale &newLocale);

            /**
             * Sets the default locale for the specified Category for this instance
             * of the Machine. This does not affect the host locale.
             * <p>
             * If there is a security manager, its checkPermission method is called
             * with a PropertyPermission("user.language", "write") permission before
             * the default locale is changed.
             * <p>
             * The Machine sets the default locale during startup based
             * on the host environment. It is used by many locale-sensitive methods
             * if no locale is explicitly specified.
             * <p>
             * Since changing the default locale may affect many different areas of
             * functionality, this method should only be used if the caller is
             * prepared to reinitialize locale-sensitive code running within the
             * same Machine.
             *
             * @param category the specified category to set the default locale
             * @param newLocale the new default locale
             * @throws SecurityException if a security manager exists and its
             *     checkPermission method doesn't allow the operation.
             *
             * @see defaultLocaleLocale::Category)
             */
            static void setDefaultLocale(Category category, const Locale &newLocale);

            /**
             * Returns a list of all 2-letter country codes defined in ISO 3166.
             * Can be used to obtain Locales.
             * This method is equivalent to <b style="color: orange;"> Locale::SOCountries(Locale::IsoCountryCode type)</b>
             * with <b> type</b>   <b style="color: orange;"> IsoCountryCode::PART1_ALPHA2</b> .
             * <p>
             * <b>Note:</b> The <b> Locale</b>  class also supports other codes for
             * country (region), such as 3-letter numeric UN M.49 area codes.
             * Therefore, the list returned by this method does not contain ALL valid
             * codes that can be used to obtain Locales.
             * <p>
             * Note that this method does not return obsolete 2-letter country codes.
             * ISO3166-3 codes which designate country codes for those obsolete codes,
             * can be retrieved from <b style="color: orange;"> Locale::SOCountries(Locale::IsoCountryCode type)</b>  with
             * <b> type</b>   <b style="color: orange;"> IsoCountryCode::PART3</b> .
             * @return An array of ISO 3166 two-letter country codes.
             */
            static StringArray isoCountries();

            enum class ISOCountryCode : gbyte {
                /**
                 * PART1_ALPHA2 is used to represent the ISO3166-1 alpha-2 two letter
                 * country codes.
                 */
                PART1_ALPHA2,

                /**
                 *
                 * PART1_ALPHA3 is used to represent the ISO3166-1 alpha-3 three letter
                 * country codes.
                 */
                PART1_ALPHA3,

                /**
                 * PART3 is used to represent the ISO3166-3 four letter country codes.
                 */
                PART3,
            };

            /**
             * Returns a <b> Set</b>  of ISO3166 country codes for the specified type.
             *
             * @param type <b style="color: orange;"> Locale::IsoCountryCode</b>  specified ISO code type.
             * @return a <b> Set</b>  of ISO country codes for the specified type.
             */
            static StringArray isoCountries(ISOCountryCode type);

            /**
             * Returns a list of all 2-letter language codes defined in ISO 639.
             * Can be used to obtain Locales.
             * <p>
             * <b>Note:</b>
             * <ul>
             * <li>ISO 639 is not a stable standard&mdash; some languages' codes have changed.
             * The list this function returns includes both the new and the old codes for the
             * languages whose codes have changed.
             * <li>The <b> Locale</b>  class also supports language codes up to
             * 8 characters in length.  Therefore, the list returned by this method does
             * not contain ALL valid codes that can be used to obtain Locales.
             * </ul>
             *
             * @return An array of ISO 639 two-letter language codes.
             */
            static StringArray isoLanguages();

            /**
             * Returns an array of all installed locales.
             * The returned array represents the union of locales supported
             * by the runtime environment and by installed
             * <b style="color: orange;"> system provider </b>
             * implementations. At a minimum, the returned array must contain a
             * <b> Locale</b>  instance equal to <b style="color: orange;"> Locale::ROOT</b>  and
             * a <b> Locale</b>  instance equal to <b style="color: orange;"> Locale::US</b> .
             *
             * @return An array of installed locales.
             */
            static Set<Locale> &availableLocales();

            /**
             * Returns the language code of this Locale.
             *
             * @implNote This method returns the new forms for the obsolete ISO 639
             * codes ("iw", "ji", and "in"). See <a href="">
             * Legacy language codes</a> for more information.
             *
             * @return The language code, or the empty string if none is defined.
             * @see Locale::displayLanguage
             */
            String language() const;

            /**
             * Returns the script for this locale, which should
             * either be the empty string or an ISO 15924 4-letter script
             * code. The first letter is uppercase and the rest are
             * lowercase, for example, 'Latn', 'Cyrl'.
             *
             * @return The script code, or the empty string if none is defined.
             * @see Locale::displayScript
             */
            String script() const;

            /**
             * Returns the country/region code for this locale, which should
             * either be the empty string, an uppercase ISO 3166 2-letter code,
             * or a UN M.49 3-digit code.
             *
             * @return The country/region code, or the empty string if none is defined.
             * @see Locale::displayCountry
             */
            String country() const;

            /**
             * Returns the variant code for this locale.
             *
             * @return The variant code, or the empty string if none is defined.
             * @see Locale::displayVariant
             */
            String variant() const;

            /**
             * Returns {@code true} if this {@code Locale} has any <a href="#def_extensions">
             * extensions</a>.
             *
             * @return {@code true} if this {@code Locale} has any extensions
             * @since 1.8
             */
            gbool hasExtensions() const;

            /**
             * Returns a copy of this {@code Locale} with no <a href="#def_extensions">
             * extensions</a>. If this {@code Locale} has no extensions, this {@code Locale}
             * is returned.
             *
             * @return a copy of this {@code Locale} with no extensions, or {@code this}
             *         if {@code this} has no extensions
             */
            Locale stripExtensions() const;

            /**
             * Returns the extension (or private use) value associated with
             * the specified key, or null if there is no extension
             * associated with the key. To be well-formed, the key must be one
             * of {@code [0-9A-Za-z]}. Keys are case-insensitive, so
             * for example 'z' and 'Z' represent the same extension.
             *
             * @param key the extension key
             * @return The extension, or null if this locale defines no
             * extension for the specified key.
             * @throws IllegalArgumentException if key is not well-formed
             * @see PRIVATE_USE_EXTENSION_KEY
             * @see UNICODE_EXTENSION_KEY
             */
            String extension(gchar key) const;

            static CORE_FAST gchar UNICODE_EXTENSION_KEY = u'u';
            static CORE_FAST gchar PRIVATE_USE_EXTENSION_KEY = u'x';

            /**
             * Returns a string representation of this <b> Locale</b> 
             * object, consisting of language, script, country and variant as below:
             * <blockquote> @code
             * language + _script + "_" + country + ("_#" + variant)
             * @endcode </blockquote>
             *
             * Language is always lower case, country is always upper case and script is always title
             * case.
             *
             * <p>If both the language and country fields are missing, this function will return
             * the empty string, even if the script, or variant field is present (you
             * can't have a locale with just a variant, the variant must accompany a well-formed
             * language or country code).
             *
             * <p>If script is present and variant is missing.
             *
             * <p>This behavior is designed to support debugging and to be compatible with
             * previous uses of <b> toString</b>  that expected language, country, and variant
             * fields only.  To represent a Locale as a String for interchange purposes, use
             * <b style="color: orange;"> Locale::toLanguageTag</b> .
             *
             * <p>Examples: <ul>
             * <li><b> en</b> </li>
             * <li><b> de_DE</b> </li>
             * <li><b> _GB</b> </li>
             * <li><b> en_US_#WIN</b> </li>
             * <li><b> de__#POSIX</b> </li>
             * <li><b> zh_Hans_CN</b> </li>
             * <li><b> zh_Hant_TW</b> </li>
             * <li><b> th_TH_TH</b> </li></ul>
             *
             * @return A string representation of the Locale, for debugging.
             * @see Locale::displayName
             * @see Locale::toLanguageTag
             */
            String toString() const override;

            /**
             * Returns a well-formed IETF BCP 47 language tag representing
             * this locale.
             *
             * <p>If this <b> Locale</b>  has a language, country, or
             * variant that does not satisfy the IETF BCP 47 language tag
             * syntax requirements, this method handles these fields as
             * described below:
             *
             * <p><b>Language:</b> If language is empty, or not <a
             * href="" >well-formed</a> (for example "a" or
             * "e2"), it will be emitted as "und" (Undetermined).
             *
             * <p><b>Country:</b> If country is not <a
             * href="">well-formed</a> (for example "12" or "USA"),
             * it will be omitted.
             *
             * <p><b>Variant:</b> If variant <b>is</b> <a
             * href="">well-formed</a>, each sub-segment
             * (delimited by '-' or '_') is emitted as a subtag.  Otherwise:
             * <ul>
             *
             * <li>if all sub-segments match <code>[0-9a-zA-Z]{1,8}</code>
             * (for example "WIN" or "Oracle_JDK_Standard_Edition"), the first
             * ill-formed sub-segment and all following will be appended to
             * the private use subtag.  The first appended subtag will be
             * "lvariant", followed by the sub-segments in order, separated by
             * hyphen. For example, "x-lvariant-WIN",
             * "Oracle-x-lvariant-JDK-Standard-Edition".
             *
             * <li>if any sub-segment does not match
             * <code>[0-9a-zA-Z]{1,8}</code>, the variant will be truncated
             * and the problematic sub-segment and all following sub-segments
             * will be omitted.  If the remainder is non-empty, it will be
             * emitted as a private use subtag as above (even if the remainder
             * turns out to be well-formed).  For example,
             * "Solaris_isjustthecoolestthing" is emitted as
             * "x-lvariant-Solaris", not as "solaris".</li></ul>
             *
             * <p><b>Special Conversions:</b> supports some old locale
             * representations, including deprecated ISO language codes,
             * for compatibility. This method performs the following
             * conversions:
             * <ul>
             *
             * <li>Deprecated ISO language codes "iw", "ji", and "in" are
             * converted to "he", "yi", and "id", respectively.
             *
             * <li>A locale with language "no", country "NO", and variant
             * "NY", representing Norwegian Nynorsk (Norway), is converted
             * to a language tag "nn-NO".</li></ul>
             *
             * <p><b>Note:</b> Although the language tag obtained by this
             * method is well-formed (satisfies the syntax requirements
             * defined by the IETF BCP 47 specification), it is not
             * necessarily a valid BCP 47 language tag.  For example,
             * <pre>
             *   Locale::forLanguageTag("xx-YY").toLanguageTag();</pre>
             *
             * will return "xx-YY", but the language subtag "xx" and the
             * region subtag "YY" are invalid because they are not registered
             * in the IANA Language Subtag Registry.
             *
             * @return a BCP47 language tag representing the locale
             * @see Locale::forLanguageTag(String)
             */
            String toLanguageTag() const;

            /**
             * Returns a locale for the specified IETF BCP 47 language tag string.
             *
             * <p>The following <b>conversions</b> are performed:<ul>
             *
             * <li>The language code "und" is mapped to language "".
             *
             * <li>The language codes "iw", "ji", and "in" are mapped to "he",
             * "yi", and "id" respectively. (This is the same canonicalization
             * that's done in Locale's constructors.) See
             * <a href="">Legacy language codes</a>
             * for more information.
             *
             * <li>The portion of a private use subtag prefixed by "lvariant",
             * if any, is removed and appended to the variant field in the
             * result locale (without case normalization).  If it is then
             * empty, the private use subtag is discarded:
             *
             * <pre>
             *     Locale loc;
             *     loc = Locale::forLanguageTag("en-US-x-lvariant-POSIX");
             *     loc.variant(); // returns "POSIX"
             *     loc.extension('x'); // returns null
             *
             *     loc = Locale::forLanguageTag("de-POSIX-x-URP-lvariant-Abc-Def");
             *     loc.variant(); // returns "POSIX_Abc_Def"
             *     loc.extension('x'); // returns "urp"
             * </pre>
             *
             * <li>When the languageTag argument contains an extlang subtag,
             * the first such subtag is used as the language, and the primary
             * language subtag and other extlang subtags are ignored:
             *
             * <pre>
             *     Locale::forLanguageTag("ar-aao").language(); // returns "aao"
             *     Locale::forLanguageTag("en-abc-def-us").toString(); // returns "abc_US"
             * </pre>
             *
             * <li>Case is normalized except for variant tags, which are left
             * unchanged.  Language is normalized to lower case, script to
             * title case, country to upper case, and extensions to lower
             * case.
             *
             * <li>If, after processing, the locale would exactly match either
             * ja_JP_JP or th_TH_TH with no extensions, the appropriate
             * extensions are added as though the constructor had been called:
             *
             * <pre>
             *    Locale::forLanguageTag("ja-JP").toLanguageTag();
             *    // returns "ja-JP"
             *    Locale::forLanguageTag("th-TH").toLanguageTag();
             *    // returns "th-TH"
             * </pre></ul>
             *
             * <p>This implements the 'Language-Tag' production of BCP47, and
             * so supports legacy (regular and irregular, referred to as
             * "Type: grandfathered" in BCP47) as well as
             * private use language tags.  Stand alone private use tags are
             * represented as empty language and extension 'x-whatever',
             * and legacy tags are converted to their canonical replacements
             * where they exist.
             *
             * <p>Legacy tags with canonical replacements are as follows:
             *
             * <table class="striped">
             * <caption style="display:none">Legacy tags with canonical replacements</caption>
             * <thead style="text-align:center">
             * <tr><th scope="col" style="padding: 0 2px">legacy tag</th><th scope="col" style="padding: 0 2px">modern replacement</th></tr>
             * </thead>
             * <tbody style="text-align:center">
             * <tr><th scope="row">art-lojban</th><td>jbo</td></tr>
             * <tr><th scope="row">i-ami</th><td>ami</td></tr>
             * <tr><th scope="row">i-bnn</th><td>bnn</td></tr>
             * <tr><th scope="row">i-hak</th><td>hak</td></tr>
             * <tr><th scope="row">i-klingon</th><td>tlh</td></tr>
             * <tr><th scope="row">i-lux</th><td>lb</td></tr>
             * <tr><th scope="row">i-navajo</th><td>nv</td></tr>
             * <tr><th scope="row">i-pwn</th><td>pwn</td></tr>
             * <tr><th scope="row">i-tao</th><td>tao</td></tr>
             * <tr><th scope="row">i-tay</th><td>tay</td></tr>
             * <tr><th scope="row">i-tsu</th><td>tsu</td></tr>
             * <tr><th scope="row">no-bok</th><td>nb</td></tr>
             * <tr><th scope="row">no-nyn</th><td>nn</td></tr>
             * <tr><th scope="row">sgn-BE-FR</th><td>sfb</td></tr>
             * <tr><th scope="row">sgn-BE-NL</th><td>vgt</td></tr>
             * <tr><th scope="row">sgn-CH-DE</th><td>sgg</td></tr>
             * <tr><th scope="row">zh-guoyu</th><td>cmn</td></tr>
             * <tr><th scope="row">zh-hakka</th><td>hak</td></tr>
             * <tr><th scope="row">zh-min-nan</th><td>nan</td></tr>
             * <tr><th scope="row">zh-xiang</th><td>hsn</td></tr>
             * </tbody>
             * </table>
             *
             * <p>Legacy tags with no modern replacement will be
             * converted as follows:
             *
             * <table class="striped">
             * <caption style="display:none">Legacy tags with no modern replacement</caption>
             * <thead style="text-align:center">
             * <tr><th scope="col" style="padding: 0 2px">legacy tag</th><th scope="col" style="padding: 0 2px">converts to</th></tr>
             * </thead>
             * <tbody style="text-align:center">
             * <tr><th scope="row">cel-gaulish</th><td>xtg-x-cel-gaulish</td></tr>
             * <tr><th scope="row">en-GB-oed</th><td>en-GB-x-oed</td></tr>
             * <tr><th scope="row">i-default</th><td>en-x-i-default</td></tr>
             * <tr><th scope="row">i-enochian</th><td>und-x-i-enochian</td></tr>
             * <tr><th scope="row">i-mingo</th><td>see-x-i-mingo</td></tr>
             * <tr><th scope="row">zh-min</th><td>nan-x-zh-min</td></tr>
             * </tbody>
             * </table>
             *
             * <p>For a list of all legacy tags, see the
             * IANA Language Subtag Registry (search for "Type: grandfathered").
             *
             * <p><b>Note</b>: there is no guarantee that <b> toLanguageTag</b> 
             * and <b> forLanguageTag</b>  will round-trip.
             *
             * @param languageTag the language tag
             * @return The locale that best represents the language tag.
             * @see Locale::toLanguageTag()
             */
            static Locale forLanguageTag(const String &languageTag);

            /**
             * Returns a three-letter abbreviation of this locale's language.
             * If the language matches an ISO 639-1 two-letter code, the
             * corresponding ISO 639-2/T three-letter lowercase code is
             * returned.  The ISO 639-2 language codes can be found on-line,
             * see "Codes for the Representation of Names of Languages Part 2:
             * Alpha-3 Code".  If the locale specifies a three-letter
             * language, the language is returned as is.  If the locale does
             * not specify a language the empty string is returned.
             *
             * @return A three-letter abbreviation of this locale's language.
             * @throws    MissingResourceException Throws MissingResourceException if
             * three-letter language abbreviation is not available for this locale.
             */
            String iso3language() const;

            /**
             * Returns a three-letter abbreviation for this locale's country.
             * If the country matches an ISO 3166-1 alpha-2 code, the
             * corresponding ISO 3166-1 alpha-3 uppercase code is returned.
             * If the locale doesn't specify a country, this will be the empty
             * string.
             *
             * <p>The ISO 3166-1 codes can be found on-line.
             *
             * @return A three-letter abbreviation of this locale's country.
             * @throws    MissingResourceException Throws MissingResourceException if the
             * three-letter country abbreviation is not available for this locale.
             */
            String iso3country() const;

            /**
             * Returns a name for the locale's language that is appropriate for display to the
             * user.
             * If possible, the name returned will be localized for the default
             * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale.
             * For example, if the locale is fr_FR and the default
             * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale
             * is en_US, Locale::displayLanguage() will return "French"; if the locale is en_US and
             * the default <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale is fr_FR,
             * Locale::displayLanguage() will return "anglais".
             * If the name returned cannot be localized for the default
             * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale,
             * (say, we don't have a Japanese name for Croatian),
             * this function falls back on the English name, and uses the ISO code as a last-resort
             * value.  If the locale doesn't specify a language, this function returns the empty string.
             *
             * @return The name of the display language.
             */
            String displayLanguage() const;

            /**
             * Returns a name for the locale's language that is appropriate for display to the
             * user.
             * If possible, the name returned will be localized according to inLocale.
             * For example, if the locale is fr_FR and inLocale
             * is en_US, Locale::displayLanguage() will return "French"; if the locale is en_US and
             * inLocale is fr_FR, Locale::displayLanguage() will return "anglais".
             * If the name returned cannot be localized according to inLocale,
             * (say, we don't have a Japanese name for Croatian),
             * this function falls back on the English name, and finally
             * on the ISO code as a last-resort value.  If the locale doesn't specify a language,
             * this function returns the empty string.
             *
             * @param inLocale The locale for which to retrieve the display language.
             * @return The name of the display language appropriate to the given locale.
             */
            String displayLanguage(const Locale &inLocale) const;

            /**
             * Returns a name for the locale's script that is appropriate for display to
             * the user. If possible, the name will be localized for the default
             * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale.  Returns
             * the empty string if this locale doesn't specify a script code.
             *
             * @return the display name of the script code for the current default
             *     <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale
             * @since 1.7
             */
            String displayScript() const;

            /**
             * Returns a name for the locale's script that is appropriate
             * for display to the user. If possible, the name will be
             * localized for the given locale. Returns the empty string if
             * this locale doesn't specify a script code.
             *
             * @param inLocale The locale for which to retrieve the display script.
             * @return the display name of the script code for the current default
             * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale
             */
            String displayScript(const Locale &inLocale) const;

            /**
             * Returns a name for the locale's country that is appropriate for display to the
             * user.
             * If possible, the name returned will be localized for the default
             * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale.
             * For example, if the locale is fr_FR and the default
             * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale
             * is en_US, Locale::displayCountry() will return "France"; if the locale is en_US and
             * the default <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale is fr_FR,
             * Locale::displayCountry() will return "Etats-Unis".
             * If the name returned cannot be localized for the default
             * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale,
             * (say, we don't have a Japanese name for Croatia),
             * this function falls back on the English name, and uses the ISO code as a last-resort
             * value.  If the locale doesn't specify a country, this function returns the empty string.
             *
             * @return The name of the country appropriate to the locale.
             */
            String displayCountry() const;

            /**
             * Returns a name for the locale's country that is appropriate for display to the
             * user.
             * If possible, the name returned will be localized according to inLocale.
             * For example, if the locale is fr_FR and inLocale
             * is en_US, Locale::displayCountry() will return "France"; if the locale is en_US and
             * inLocale is fr_FR, Locale::displayCountry() will return "Etats-Unis".
             * If the name returned cannot be localized according to inLocale.
             * (say, we don't have a Japanese name for Croatia),
             * this function falls back on the English name, and finally
             * on the ISO code as a last-resort value.  If the locale doesn't specify a country,
             * this function returns the empty string.
             *
             * @param inLocale The locale for which to retrieve the display country.
             * @return The name of the country appropriate to the given locale.
             */
            String displayCountry(const Locale &inLocale) const;

            /**
             * Returns a name for the locale's variant code that is appropriate for display to the
             * user.  If possible, the name will be localized for the default
             * <b style="color: orange;"> Locale::Category::DISPLAY</b>  locale.  If the locale
             * doesn't specify a variant code, this function returns the empty string.
             *
             * @return The name of the display variant code appropriate to the locale.
             */
            String displayVariant() const;

            /**
             * Returns a name for the locale's variant code that is appropriate for display to the
             * user.  If possible, the name will be localized for inLocale.  If the locale
             * doesn't specify a variant code, this function returns the empty string.
             *
             * @param inLocale The locale for which to retrieve the display variant code.
             * @return The name of the display variant code appropriate to the given locale.
             */
            String displayVariant(const Locale &inLocale) const;

            /**
             * Returns a name for the locale that is appropriate for display to the
             * user. This will be the values returned by Locale::displayLanguage(),
             * Locale::displayScript(), Locale::displayCountry(), Locale::displayVariant() and
             * optional <a href="">Unicode extensions</a>
             * assembled into a single string. The non-empty values are used in order, with
             * the second and subsequent names in parentheses.  For example:
             * <blockquote>
             * language (script, country, variant(, extension)*)<br>
             * language (country(, extension)*)<br>
             * language (variant(, extension)*)<br>
             * script (country(, extension)*)<br>
             * country (extension)*<br>
             * </blockquote>
             * depending on which fields are specified in the locale. The field
             * separator in the above parentheses, denoted as a comma character, may
             * be localized depending on the locale. If the language, script, country,
             * and variant fields are all empty, this function returns the empty string.
             *
             * @return The name of the locale appropriate to display.
             */
            String displayName() const;

            /**
             * Returns a name for the locale that is appropriate for display
             * to the user.  This will be the values returned by
             * Locale::displayLanguage(), Locale::displayScript(), Locale::displayCountry(),
             * Locale::displayVariant(), and optional <a href="">
             * Unicode extensions</a> assembled into a single string. The non-empty
             * values are used in order, with the second and subsequent names in
             * parentheses.  For example:
             * <blockquote>
             * language (script, country, variant)<br>
             * language (script, country)<br>
             * language (country)<br>
             * language (variant)<br>
             * script (country, variant)<br>
             * script (country)<br>
             * country <br>
             * </blockquote>
             * depending on which fields are specified in the locale. The field
             * separator in the above parentheses, denoted as a comma character, may
             * be localized depending on the locale. If the language, script, country,
             * and variant fields are all empty, this function returns the empty string.
             *
             * @param inLocale The locale for which to retrieve the display name.
             * @return The name of the locale appropriate to display.
             */
            String displayName(const Locale &inLocale) const;

            /**
             * Return shadow copy of this locale
             */
            Object &clone() const override;

            /**
             * Return hashcode of this locale INSTANCE
             */
            gint hash() const override;

            /**
             * Returns true if this Locale is equal to another object.  A Locale is
             * deemed equal to another Locale with identical language, script, country,
             * variant, and unequal to all other objects.
             *
             * @return true if this Locale is equal to the specified object.
             */
            gbool equals(const Object &obj) const override;

            /**
             * Obtains a locale from language, script, country and variant.
             * This method normalizes the language value to lowercase and
             * the country value to uppercase.
             * @implNote
             * <ul>
             * <li>This method does make full syntactic checks on the input with BCP47.
             * <li>The two cases ("ja", "", "JP", "JP") and ("th", "", "TH", "TH") are handled specially,
             * see <a href="">Special Cases</a> for more information.
             * <li>Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms. See <a href="">Legacy language codes</a> for more information.
             * </ul>
             *
             * @param language A language code. See the <b> Locale</b>  class description of
             * <a href="">language</a> values.
             * @param script A script code. See the <b> Locale</b>  class description of
             * <a href="">script</a> values.
             * @param country A country code. See the <b> Locale</b>  class description of
             * <a href="">country</a> values.
             * @param variant Any arbitrary value used to indicate a variation of a <b> Locale</b> .
             * See the <b> Locale</b>  class description of <a href="">variant</a> values.
             * @return A <b> Locale</b>  object
             */
            static Locale of(const String &language, const String &script, const String &country,
                             const String &variant);

            /**
             * Obtains a locale from language, script and country.
             * This method normalizes the language value to lowercase and
             * the country value to uppercase.
             * @implNote
             * <ul>
             * <li>This method does make full syntactic checks on the input with BCP47.
             * <li>The two cases ("ja", "", "JP", "JP") and ("th", "", "TH", "TH") are handled specially,
             * see <a href="">Special Cases</a> for more information.
             * <li>Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms. See <a href="">Legacy language codes</a> for more information.
             * </ul>
             *
             * @param language A language code. See the <b> Locale</b>  class description of
             * <a href="">language</a> values.
             * @param script A script code. See the <b> Locale</b>  class description of
             * <a href="">script</a> values.
             * @param country A country code. See the <b> Locale</b>  class description of
             * <a href="">country</a> values.
             * @return A <b> Locale</b>  object
             */
            static Locale of(const String &language, const String &script, const String &country);

            /**
             * Obtains a locale from language and country.
             * This method normalizes the language value to lowercase and
             * the country value to uppercase.
             * @implNote
             * <ul>
             * <li>This method does not make any syntactic checks on the input.
             * <li>Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms. See <a href="">Legacy language
             * codes</a> for more information.
             * </ul>
             *
             * @param language A language code. See the <b> Locale</b>  class description of
             * <a href="">language</a> values.
             * @param country A country code. See the <b> Locale</b>  class description of
             * <a href="">country</a> values.
             * @return A <b> Locale</b>  object
             */
            static Locale of(const String &language, const String &country);

            /**
             * Destroy this Locale instance
             */
            ~Locale() override = default;

        private:
            /**
             * Default locales
             */
            static Locale DEFAULT_LOCALE;
            static Locale DEFAULT_DISPLAY_LOCALE;
            static Locale DEFAULT_FORMAT_LOCALE;

            String BCP47_TAG;

            enum Query: gbyte {
                DISPLAY_LANGUAGE,
                DISPLAY_SCRIPT,
                DISPLAY_COUNTRY,
                DISPLAY_VARIANT
            };

            static String displayString(const String& code, const String& cat, const Locale &inLocale, Query query) {
                if (code.isEmpty())
                    return ""_S;
                CORE_IGNORE(cat);
                CORE_IGNORE(inLocale);
                CORE_IGNORE(query);
                return {};
            }

            static Locale initDefaultLocale();

            static Locale initDefaultLocale(Category category);

            static StringArray iso2Table(const String &table);

            static StringArray iso3Table(const String &table);

            static String iso3Code(const String &iso2Code, const String &table);
        };

    }
} // core


#endif //CORE23_LOCALE_H
