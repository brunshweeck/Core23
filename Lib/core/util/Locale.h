//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_LOCALE_H
#define CORE23_LOCALE_H

#include <core/Object.h>

namespace core {

    namespace util {
        template<class E>
        class List;



        /**
         * A <b> Locale</b> object represents a specific geographical, political,
         * or cultural region. An operation that requires a <b> Locale</b> to perform
         * its task is called <em>locale-sensitive</em> and uses the <b> Locale</b>
         * to tailor information for the user. For example, displaying a number
         * is a locale-sensitive operation&mdash; the number should be formatted
         * according to the customs and conventions of the user's native country,
         * region, or culture.
         *
         * <p> The <b> Locale</b> class implements IETF BCP 47 which is composed of
         * <a href="https://tools.ietf.org/html/rfc4647">RFC 4647 "Matching of Language
         * Tags"</a> and <a href="https://tools.ietf.org/html/rfc5646">RFC 5646 "Tags
         * for Identifying Languages"</a> with support for the LDML (UTS#35, "Unicode
         * Locale Data Markup Language") BCP 47-compatible extensions for locale array
         * exchange.
         *
         * <p> A <b> Locale</b> object logically consists of the fields
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
         *   <b> Locale</b> always canonicalizes to lower case.</dd>
         *
         *   <dd>Well-formed language values have the form
         *   <code>[a-zA-Z]{2,8</b></code>.  Note that this is not the full
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
         *   <b> Locale</b> always canonicalizes to title case (the first
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
         *   <b> Locale</b> always canonicalizes to upper case.</dd>
         *
         *   <dd>Well-formed country/region values have
         *   the form <code>[a-zA-Z]{2} | [0-9]{3}</code></dd>
         *
         *   <dd>Example: "US" (United States), "FR" (France), "029"
         *   (Caribbean)</dd>
         * </dl>
         *
         * <b>Note:</b> Although BCP 47 requires field values to be registered
         * in the IANA Language Subtag Registry, the <b> Locale</b> class
         * does not provide any validation features.  The <b> Builder</b>
         * only checks if an individual field satisfies the syntactic
         * requirement (is well-formed), but does not validate the value
         * itself.  See <b style="color: orange;"> Builder</b> for details.
         *
         * <p>
         * A well-formed locale key has the form
         * <code>[0-9a-zA-Z]{2}</code>.  A well-formed locale type has the
         * form <code>"" | [0-9a-zA-Z]{3,8} ('-' [0-9a-zA-Z]{3,8})*</code> (it
         * can be empty, or a series of subtags 3-8 alphanums in length).  A
         * well-formed locale attribute has the form
         * <code>[0-9a-zA-Z]{3,8}</code> (it is a single subtag with the same
         * form as a locale type subtag).
         *
         * <p>
         * The Unicode locale extension specifies optional behavior in
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
         * <h4>Builder</h4>
         *
         * <p>Using <b style="color: orange;"> Builder</b> you can construct a <b> Locale</b> object
         * that conforms to BCP 47 syntax.
         *
         * <h4>Factory Methods</h4>
         *
         * <p>
         * The method <b style="color: orange;"> forLanguageTag()</b> obtains a <b> Locale</b>
         * object for a well-formed BCP 47 language tag. The method
         * <b style="color: orange;"> of(String, String, String)</b> and its overloads obtain a
         * <b> Locale</b> object from given <b> language</b>, <b> country</b>,
         * and/or <b> script</b> defined above.
         *
         * <h4>Locale Constants</h4>
         *
         * <p>
         * The <b> Locale</b> class provides a number of convenient constants
         * that you can use to obtain <b> Locale</b> objects for commonly used
         * locales. For example, <b> Locale::US</b> is the <b> Locale</b> object
         * for the United States.
         *
         * <h3><a id="LocaleMatching">Locale Matching</a></h3>
         *
         * <p>
         * If an application or a system is internationalized and provides localized
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
         * <p>
         * A user's preference is called a <em>Language Priority List</em> and is
         * expressed as a list of language ranges. There are syntactically two types of
         * language ranges: basic and extended.
         *
         * <h4>Filtering</h4>
         *
         * <p>
         * The filtering operation returns all matching language tags. It is defined
         * in RFC 4647 as follows:
         * "In filtering, each language range represents the least specific language
         * tag (that is, the language tag with fewest number of subtags) that is an
         * acceptable match. All of the language tags in the matching set of tags will
         * have an equal or greater number of subtags than the language range. Every
         * non-wildcard subtag in the language range will appear in every one of the
         * matching language tags."
         *
         * <p>
         * There are two types of filtering: filtering for basic language ranges
         * (called "basic filtering") and filtering for extended language ranges
         * (called "extended filtering"). They may return different results by what
         * kind of language ranges are included in the given Language Priority List.
         *
         * <h4>Lookup</h4>
         *
         * <p>
         * The lookup operation returns the best matching language tags. It is
         * defined in RFC 4647 as follows:
         * "By contrast with filtering, each language range represents the most
         * specific tag that is an acceptable match.  The first matching tag found,
         * according to the user's priority, is considered the closest match and is the
         * item returned."
         *
         * <p>
         * For example, if a Language Priority List consists of two language ranges,
         * <b> "zh-Hant-TW"</b> and <b> "en-US"</b>, in prioritized order, lookup
         * method progressively searches the language tags below in order to find the
         * best matching language tag.
         * <blockquote>
         * <pre>
         *    1. zh-Hant-TW <br>
         *    2. zh-Hant <br>
         *    3. zh <br>
         *    4. en-US <br>
         *    5. en <br>
         * </pre>
         * </blockquote>
         * If there is a language tag which matches completely to a language range
         * above, the language tag is returned.
         *
         * <p><b> "*"</b> is the special language range, and it is ignored in lookup.
         *
         * <p>If multiple language tags match as a result of the subtag <b> '*'</b>
         * included in a language range, the first matching language tag returned by
         * an <b style="color: orange;"> Iterator</b> over a <b style="color: orange;"> Collection</b> of language tags is treated as
         * the best matching one.
         *
         * <h3>Use of Locale</h3>
         *
         * <p>
         * Once you've obtained a <b> Locale</b> you can query it for information
         * about itself. Use <b> country() </b> to get the country (or region)
         * code and <b> language() </b> to get the language code.
         * You can use <b> displayCountry() </b> to get the
         * name of the country suitable for displaying to the user. Similarly,
         * you can use <b> displayLanguage() </b> to get the name of
         * the language suitable for displaying to the user. Interestingly,
         * the <b> displayXXX() </b> methods are themselves locale-sensitive
         * and have two versions: one that uses the default
         * <b style="color: orange;"> DISPLAY</b> locale and one that uses the locale specified
         * as an argument.
         *
         * <p>
         * The Platform provides a number of classes that perform locale-sensitive
         * operations. For example, the <b> NumberFormat</b> class formats
         * numbers, currency, and percentages in a locale-sensitive manner. Classes
         * such as <b> NumberFormat</b> have several convenience methods
         * for creating a default object of that type. For example, the
         * <b> NumberFormat</b> class provides these three convenience methods
         * for creating a default <b> NumberFormat</b> object:
         * <blockquote>
         * <pre>
         *     NumberFormat.createInstance()
         *     NumberFormat.createCurrencyInstance()
         *     NumberFormat.createPercentInstance()
         * </pre>
         * </blockquote>
         * Each of these methods has two variants; one with an explicit locale
         * and one without; the latter uses the default
         * <b style="color: orange;"> FORMAT</b> locale:
         * <blockquote>
         * <pre>
         *     NumberFormat.createInstance(myLocale)
         *     NumberFormat.createCurrencyInstance(myLocale)
         *     NumberFormat.createPercentInstance(myLocale)
         * </pre>
         * </blockquote>
         * A <b> Locale</b> is the mechanism for identifying the kind of object
         * (<b> NumberFormat</b>) that you would like to get. The locale is
         * <STRONG>just</STRONG> a mechanism for identifying objects,
         * <STRONG>not</STRONG> a container for the objects themselves.
         *
         * <h3>Compatibility</h3>
         *
         * <p>In addition, BCP 47 imposes syntax restrictions that are not
         * imposed by Locale's constructors. This means that conversions
         * between some Locales and BCP 47 language tags cannot be made without
         * losing information. Thus <b> toLanguageTag</b> cannot
         * represent the state of locales whose language, country
         * do not conform to BCP 47.
         *
         * <p>Because of these issues, it is recommended that clients migrate
         * away from constructing non-conforming locales and use the
         * <b> forLanguageTag</b> and <b> Locale.Builder</b> APIs instead.
         * Clients desiring a string representation of the complete locale can
         * then always rely on <b> toLanguageTag</b> for this purpose.
         *
         * <h4><a id="legacy_language_codes">Legacy language codes</a></h4>
         *
         * <p>
         * Locale's constructor has always converted three language codes to
         * their earlier, obsoleted forms: <b> he</b> maps to <b> iw</b>,
         * <b> yi</b> maps to <b> ji</b>, and <b> id</b> maps to
         * <b> in</b>.
         *
         * <p>
         * The APIs added map between the old and new language codes,
         * maintaining the mapped codes internal to Locale,
         * but using the new codes in the BCP 47 language tag APIs. This
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
         * @see Format
         * @see NumberFormat
         * @see Collator
         *
         * @author Brunshweeck Tazeussong
         */
        class Locale : public Object {
        public:
            /**
             * Enumeration of all available languages
             */
            enum class Language : gshort {
                UNDEFINED = 0,
                // -------------- [Standard Languages] -------------
                C,
                ABKHAZIAN,
                AFAR,
                AFRIKAANS,
                AGHEM,
                AKAN,
                AKKADIAN,
                AKOOSE,
                ALBANIAN,
                AMERICAN_SIGN_LANGUAGE,
                AMHARIC,
                ANCIENT_EGYPTIAN,
                ANCIENT_GREEK,
                ARABIC,
                ARAGONESE,
                ARAMAIC,
                ARMENIAN,
                ASSAMESE,
                ASTURIAN,
                ASU,
                ATSAM,
                AVARIC,
                AVESTAN,
                AYMARA,
                AZERBAIJANI,
                BAFIA,
                BALINESE,
                BAMBARA,
                BAMUN,
                BANGLA,
                BASAA,
                BASHKIR,
                BASQUE,
                BATAK_TOBA,
                BELARUSIAN,
                BEMBA,
                BENA,
                BHOJPURI,
                BISLAMA,
                BLIN,
                BODO,
                BOSNIAN,
                BRETON,
                BUGINESE,
                BULGARIAN,
                BURMESE,
                CANTONESE,
                CATALAN,
                CEBUANO,
                CENTRAL_ATLAS_TAMAZIGHT,
                CENTRAL_KURDISH,
                CHAKMA,
                CHAMORRO,
                CHECHEN,
                CHEROKEE,
                CHICKASAW,
                CHIGA,
                CHINESE,
                CHURCH,
                CHUVASH,
                COLOGNIAN,
                COPTIC,
                CORNISH,
                CORSICAN,
                CREE,
                CROATIAN,
                CZECH,
                DANISH,
                DIVEHI,
                DOGRI,
                DUALA,
                DUTCH,
                DZONGKHA,
                EMBU,
                ENGLISH,
                ERZYA,
                ESPERANTO,
                ESTONIAN,
                EWE,
                EWONDO,
                FAROESE,
                FIJIAN,
                FILIPINO,
                FINNISH,
                FRENCH,
                FRIULIAN,
                FULAH,
                GAELIC,
                GA,
                GALICIAN,
                GANDA,
                GEEZ,
                GEORGIAN,
                GERMAN,
                GOTHIC,
                GREEK,
                GUARANI,
                GUJARATI,
                GUSII,
                HAITIAN,
                HAUSA,
                HAWAIIAN,
                HEBREW,
                HERERO,
                HINDI,
                HIRI_MOTU,
                HUNGARIAN,
                ICELANDIC,
                IDO,
                IGBO,
                INARI_SAMI,
                INDONESIAN,
                INGUSH,
                INTERLINGUA,
                INTERLINGUE,
                INUKTITUT,
                INUPIAQ,
                IRISH,
                ITALIAN,
                JAPANESE,
                JAVANESE,
                JJU,
                JOLA_FONYI,
                KABUVERDIANU,
                KABYLE,
                KAKO,
                KALAALLISUT,
                KALENJIN,
                KAMBA,
                KANNADA,
                KANURI,
                KASHMIRI,
                KAZAKH,
                KENYANG,
                KHMER,
                KICHE,
                KIKUYU,
                KINYARWANDA,
                KOMI,
                KONGO,
                KONKANI,
                KOREAN,
                KORO,
                KOYRABORO_SENNI,
                KOYRA_CHIINI,
                KPELLE,
                KUANYAMA,
                KURDISH,
                KWASIO,
                KYRGYZ,
                LAKOTA,
                LANGI,
                LAO,
                LATIN,
                LATVIAN,
                LEZGHIAN,
                LIMBURGISH,
                LINGALA,
                LITERARY_CHINESE,
                LITHUANIAN,
                LOJBAN,
                LOWER_SORBIAN,
                LOW_GERMAN,
                LUBA_KATANGA,
                LULE_SAMI,
                LUO,
                LUXEMBOURGISH,
                LUYIA,
                MACEDONIAN,
                MACHAME,
                MAITHILI,
                MAKHUWA_MEETTO,
                MAKONDE,
                MALAGASY,
                MALAYALAM,
                MALAY,
                MALTESE,
                MANDINGO,
                MANIPURI,
                MANX,
                MAORI,
                MAPUCHE,
                MARATHI,
                MARSHALLESE,
                MASAI,
                MAZANDERANI,
                MENDE,
                MERU,
                META,
                MOHAWK,
                MONGOLIAN,
                MORISYEN,
                MUNDANG,
                MUSCOGEE,
                NAMA,
                NAURU,
                NAVAJO,
                NDONGA,
                NEPALI,
                NEWARI,
                NGIEMBOON,
                NGOMBA,
                NIGERIAN_PIDGIN,
                NKO,
                NORTHERN_LURI,
                NORTHERN_SAMI,
                NORTHERN_SOTHO,
                NORTH_NDEBELE,
                NORWEGIAN_BOKMAL,
                NORWEGIAN_NYNORSK,
                NUER,
                NYANJA,
                NYANKOLE,
                OCCITAN,
                ODIA,
                OJIBWA,
                OLD_IRISH,
                OLD_NORSE,
                OLD_PERSIAN,
                OROMO,
                OSAGE,
                OSSETIC,
                PAHLAVI,
                PALAUAN,
                PALI,
                PAPIAMENTO,
                PASHTO,
                PERSIAN,
                PHOENICIAN,
                POLISH,
                PORTUGUESE,
                PRUSSIAN,
                PUNJABI,
                QUECHUA,
                ROMANIAN,
                ROMANSH,
                ROMBO,
                RUNDI,
                RUSSIAN,
                RWA,
                SAHO,
                SAKHA,
                SAMBURU,
                SAMOAN,
                SANGO,
                SANGU,
                SANSKRIT,
                SANTALI,
                SARDINIAN,
                SAURASHTRA,
                SENA,
                SERBIAN,
                SHAMBALA,
                SHONA,
                SICHUAN_YI,
                SICILIAN,
                SIDAMO,
                SILESIAN,
                SINDHI,
                SINHALA,
                SKOLT_SAMI,
                SLOVAK,
                SLOVENIAN,
                SOGA,
                SOMALI,
                SOUTHERN_KURDISH,
                SOUTHERN_SAMI,
                SOUTHERN_SOTHO,
                SOUTH_NDEBELE,
                SPANISH,
                STANDARD_MOROCCAN_TAMAZIGHT,
                SUNDANESE,
                SWAHILI,
                SWATI,
                SWEDISH,
                SWISS_GERMAN,
                SYRIAC,
                TACHELHIT,
                TAHITIAN,
                TAI_DAM,
                TAITA,
                TAJIK,
                TAMIL,
                TAROKO,
                TASAWAQ,
                TATAR,
                TELUGU,
                TESO,
                THAI,
                TIBETAN,
                TIGRE,
                TIGRINYA,
                TOKELAU,
                TOK_PISIN,
                TONGAN,
                TSONGA,
                TSWANA,
                TURKISH,
                TURKMEN,
                TUVALU,
                TYAP,
                UGARITIC,
                UKRAINIAN,
                UPPER_SORBIAN,
                URDU,
                UYGHUR,
                UZBEK,
                VAI,
                VENDA,
                VIETNAMESE,
                VOLAPUK,
                VUNJO,
                WALLOON,
                WALSER,
                WARLPIRI,
                WELSH,
                WESTERN_BALOCHI,
                WESTERN_FRISIAN,
                WOLAYTTA,
                WOLOF,
                XHOSA,
                YANGBEN,
                YIDDISH,
                YORUBA,
                ZARMA,
                ZHUANG,
                ZULU,
                KAINGANG,
                NHEENGATU,
                HARYANVI,
                NORTHERN_FRISIAN,
                RAJASTHANI,
                MOKSHA,
                TOKI_PONA,
                PIJIN,
                OBOLO,
// ------------- [Other Languages] ----------------
                AFAN, // similar to OROMO,
                BENGALI, // similar to BANGLA,
                BHUTANI, // similar to DZONGKHA,
                BYELORUSSIAN, // similar to BELARUSIAN,
                CAMBODIAN, // similar to KHMER,
                CENTRAL_MOROCCO_TAMAZIGHT, // similar to CENTRAL_ATLAS_TAMAZIGHT,
                CHEWA, // similar to NYANJA,
                FRISIAN, // similar to WESTERN_FRISIAN,
                GREENLANDIC, // similar to KALAALLISUT,
                INUPIAK, // similar to INUPIAQ,
                KIRGHIZ, // similar to KYRGYZ,
                KURUNDI, // similar to RUNDI,
                KWANYAMA, // similar to KUANYAMA,
                NAVAHO, // similar to NAVAJO,
                ORIYA, // similar to ODIA,
                RHAETO_ROMANCE, // similar to ROMANSH,
                UIGHUR, // similar to UYGHUR,
                UIGUR, // similar to UYGHUR,
                WALAMO, // similar to WOLAYTTA,
            };

            /**
             * Enumeration of all available countries
             */
            enum class Country : gshort {
                UNDEFINED = 0,
// ---------------- [Standard Country names] -----------------
                AFGHANISTAN,
                ALAND_ISLANDS,
                ALBANIA,
                ALGERIA,
                AMERICAN_SAMOA,
                ANDORRA,
                ANGOLA,
                ANGUILLA,
                ANTARCTICA,
                ANTIGUA_AND_BARBUDA,
                ARGENTINA,
                ARMENIA,
                ARUBA,
                ASCENSION_ISLAND,
                AUSTRALIA,
                AUSTRIA,
                AZERBAIJAN,
                BAHAMAS,
                BAHRAIN,
                BANGLADESH,
                BARBADOS,
                BELARUS,
                BELGIUM,
                BELIZE,
                BENIN,
                BERMUDA,
                BHUTAN,
                BOLIVIA,
                BOSNIA_AND_HERZEGOVINA,
                BOTSWANA,
                BOUVET_ISLAND,
                BRAZIL,
                BRITISH_INDIAN_OCEAN,
                BRITISH_VIRGIN_ISLANDS,
                BRUNEI,
                BULGARIA,
                BURKINA_FASO,
                BURUNDI,
                CAMBODIA,
                CAMEROON,
                CANADA,
                CANARY_ISLANDS,
                CAPE_VERDE,
                CARIBBEAN_NETHERLANDS,
                CAYMAN_ISLANDS,
                CENTRAL_AFRICAN_REPUBLIC,
                CEUTA_AND_MELILLA,
                CHAD,
                CHILE,
                CHINA,
                CHRISTMAS_ISLAND,
                CLIPPERTON_ISLAND,
                COCOS_ISLANDS,
                COLOMBIA,
                COMOROS,
                CONGO_BRAZZAVILLE,
                CONGO_KINSHASA,
                COOK_ISLANDS,
                COSTA_RICA,
                CROATIA,
                CUBA,
                CURACAO,
                CYPRUS,
                CZECHIA,
                DENMARK,
                DIEGO_GARCIA,
                DJIBOUTI,
                DOMINICA,
                DOMINICAN_REPUBLIC,
                ECUADOR,
                EGYPT,
                EL_SALVADOR,
                EQUATORIAL_GUINEA,
                ERITREA,
                ESTONIA,
                ESWATINI,
                ETHIOPIA,
                EUROPE,
                EUROPEAN_UNION,
                FALKLAND_ISLANDS,
                FAROE_ISLANDS,
                FIJI,
                FINLAND,
                FRANCE,
                FRENCH_GUIANA,
                FRENCH_POLYNESIA,
                FRENCH_SOUTHERN_TERRITORIES,
                GABON,
                GAMBIA,
                GEORGIA,
                GERMANY,
                GHANA,
                GIBRALTAR,
                GREECE,
                GREENLAND,
                GRENADA,
                GUADELOUPE,
                GUAM,
                GUATEMALA,
                GUERNSEY,
                GUINEA_BISSAU,
                GUINEA,
                GUYANA,
                HAITI,
                HEARD_AND_MC_DONALD_ISLANDS,
                HONDURAS,
                HONG_KONG,
                HUNGARY,
                ICELAND,
                INDIA,
                INDONESIA,
                IRAN,
                IRAQ,
                IRELAND,
                ISLE_OF_MAN,
                ISRAEL,
                ITALY,
                IVORY_COAST,
                JAMAICA,
                JAPAN,
                JERSEY,
                JORDAN,
                KAZAKHSTAN,
                KENYA,
                KIRIBATI,
                KOSOVO,
                KUWAIT,
                KYRGYZSTAN,
                LAOS,
                LATIN_AMERICA,
                LATVIA,
                LEBANON,
                LESOTHO,
                LIBERIA,
                LIBYA,
                LIECHTENSTEIN,
                LITHUANIA,
                LUXEMBOURG,
                MACAO,
                MACEDONIA,
                MADAGASCAR,
                MALAWI,
                MALAYSIA,
                MALDIVES,
                MALI,
                MALTA,
                MARSHALL_ISLANDS,
                MARTINIQUE,
                MAURITANIA,
                MAURITIUS,
                MAYOTTE,
                MEXICO,
                MICRONESIA,
                MOLDOVA,
                MONACO,
                MONGOLIA,
                MONTENEGRO,
                MONTSERRAT,
                MOROCCO,
                MOZAMBIQUE,
                MYANMAR,
                NAMIBIA,
                NAURU,
                NEPAL,
                NETHERLANDS,
                NEW_CALEDONIA,
                NEW_ZEALAND,
                NICARAGUA,
                NIGERIA,
                NIGER,
                NIUE,
                NORFOLK_ISLAND,
                NORTHERN_MARIANA_ISLANDS,
                NORTH_KOREA,
                NORWAY,
                OMAN,
                OUTLYING_OCEANIA,
                PAKISTAN,
                PALAU,
                PALESTINIAN_TERRITORIES,
                PANAMA,
                PAPUA_NEW_GUINEA,
                PARAGUAY,
                PERU,
                PHILIPPINES,
                PITCAIRN,
                POLAND,
                PORTUGAL,
                PUERTO_RICO,
                QATAR,
                REUNION,
                ROMANIA,
                RUSSIA,
                RWANDA,
                SAINT_BARTHELEMY,
                SAINT_HELENA,
                SAINT_KITTS_AND_NEVIS,
                SAINT_LUCIA,
                SAINT_MARTIN,
                SAINT_PIERRE_AND_MIQUELON,
                SAINT_VINCENT_AND_GRENADINES,
                SAMOA,
                SAN_MARINO,
                SAO_TOME_AND_PRINCIPE,
                SAUDI_ARABIA,
                SENEGAL,
                SERBIA,
                SEYCHELLES,
                SIERRA_LEONE,
                SINGAPORE,
                SINT_MAARTEN,
                SLOVAKIA,
                SLOVENIA,
                SOLOMON_ISLANDS,
                SOMALIA,
                SOUTH_AFRICA,
                SOUTH_GEORGIA_AND_SOUTH_SANDWICH_ISLANDS,
                SOUTH_KOREA,
                SOUTH_SUDAN,
                SPAIN,
                SRI_LANKA,
                SUDAN,
                SURINAME,
                SVALBARD_AND_JAN_MAYEN,
                SWEDEN,
                SWITZERLAND,
                SYRIA,
                TAIWAN,
                TAJIKISTAN,
                TANZANIA,
                THAILAND,
                TIMOR_LESTE,
                TOGO,
                TOKELAU,
                TONGA,
                TRINIDAD_AND_TOBAGO,
                TRISTAN_DA_CUNHA,
                TUNISIA,
                TURKEY,
                TURKMENISTAN,
                TURKS_AND_CAICOS_ISLANDS,
                TUVALU,
                UGANDA,
                UKRAINE,
                UNITED_ARAB_EMIRATES,
                UNITED_KINGDOM,
                UNITED_STATES_OUTLYING_ISLANDS,
                UNITED_STATES,
                UNITED_STATES_VIRGIN_ISLANDS,
                URUGUAY,
                UZBEKISTAN,
                VANUATU,
                VATICAN_CITY,
                VENEZUELA,
                VIETNAM,
                WALLIS_AND_FUTUNA,
                WESTERN_SAHARA,
                WORLD,
                YEMEN,
                ZAMBIA,
                ZIMBABWE,
// ---------------- [Other Country names] -----------------
                BONAIRE, // similar to CARIBBEAN_NETHERLANDS,
                BOSNIA_AND_HERZEGOWINA, // similar to BOSNIA_AND_HERZEGOVINA,
                CURA_SAO, // similar to CURACAO,
                CZECH_REPUBLIC, // similar to CZECHIA,
                DEMOCRATIC_REPUBLIC_OF_CONGO, // similar to CONGO_KINSHASA,
                DEMOCRATIC_REPUBLIC_OF_KOREA, // similar to NORTH_KOREA,
                EAST_TIMOR, // similar to TIMOR_LESTE,
                LATIN_AMERICA_AND_THE_CARIBBEAN, // similar to LATIN_AMERICA,
                MACAU, // similar to MACAO,
                PEOPLES_REPUBLIC_OF_CONGO, // similar to CONGO_BRAZZAVILLE,
                REPUBLIC_OF_KOREA, // similar to SOUTH_KOREA,
                RUSSIAN_FEDERATION, // similar to RUSSIA,
                SAINT_VINCENT_AND_THE_GRENADINES, // similar to SAINT_VINCENT_AND_GRENADINES,
                SOUTH_GEORGIA_AND_THE_SOUTH_SANDWICH_ISLANDS, // similar to SOUTH_GEORGIA_AND_SOUTH_SANDWICH_ISLANDS,
                SVALBARD_AND_JAN_MAYEN_ISLANDS, // similar to SVALBARD_AND_JAN_MAYEN,
                SWAZILAND, // similar to ESWATINI,
                SYRIAN_ARAB_REPUBLIC, // similar to SYRIA,
                UNITED_STATES_MINOR_OUTLYING_ISLANDS, // similar to UNITED_STATES_OUTLYING_ISLANDS,
                VATICAN_CITY_STATE, // similar to VATICAN_CITY,
                WALLIS_AND_FUTUNA_ISLANDS, // similar to WALLIS_AND_FUTUNA,
            };

            /**
             * Enumeration of all available scripts
             */
            enum class Script : gshort {
                UNDEFINED = 0,
// ---------------- [Standard Script names] -----------------
                ADLAM,
                AHOM,
                ANATOLIAN_HIEROGLYPHS,
                ARABIC,
                ARMENIAN,
                AVESTAN,
                BALINESE,
                BAMUM,
                BANGLA,
                BASSA_VAH,
                BATAK,
                BHAIKSUKI,
                BOPOMOFO,
                BRAHMI,
                BRAILLE,
                BUGINESE,
                BUHID,
                CANADIAN_ABORIGINAL,
                CARIAN,
                CAUCASIAN_ALBANIAN,
                CHAKMA,
                CHAM,
                CHEROKEE,
                COPTIC,
                CUNEIFORM,
                CYPRIOT,
                CYRILLIC,
                DESERET,
                DEVANAGARI,
                DUPLOYAN,
                EGYPTIAN_HIEROGLYPHS,
                ELBASAN,
                ETHIOPIC,
                FRASER,
                GEORGIAN,
                GLAGOLITIC,
                GOTHIC,
                GRANTHA,
                GREEK,
                GUJARATI,
                GURMUKHI,
                HANGUL,
                HAN,
                HANUNOO,
                HAN_WITH_BOPOMOFO,
                HATRAN,
                HEBREW,
                HIRAGANA,
                IMPERIAL_ARAMAIC,
                INSCRIPTIONAL_PAHLAVI,
                INSCRIPTIONAL_PARTHIAN,
                JAMO,
                JAPANESE,
                JAVANESE,
                KAITHI,
                KANNADA,
                KATAKANA,
                KAYAH_LI,
                KHAROSHTHI,
                KHMER,
                KHOJKI,
                KHUDAWADI,
                KOREAN,
                LANNA,
                LAO,
                LATIN,
                LEPCHA,
                LIMBU,
                LINEAR_ASCRIPT,
                LINEAR_BSCRIPT,
                LYCIAN,
                LYDIAN,
                MAHAJANI,
                MALAYALAM,
                MANDAEAN,
                MANICHAEAN,
                MARCHEN,
                MEITEI_MAYEK,
                MENDE,
                MEROITIC_CURSIVE,
                MEROITIC,
                MODI,
                MONGOLIAN,
                MRO,
                MULTANI,
                MYANMAR,
                NABATAEAN,
                NEWA,
                NEW_TAI_LUE,
                NKO,
                ODIA,
                OGHAM,
                OL_CHIKI,
                OLD_HUNGARIAN,
                OLD_ITALIC,
                OLD_NORTH_ARABIAN,
                OLD_PERMIC,
                OLD_PERSIAN,
                OLD_SOUTH_ARABIAN,
                ORKHON,
                OSAGE,
                OSMANYA,
                PAHAWH_HMONG,
                PALMYRENE,
                PAU_CIN_HAU,
                PHAGS_PA,
                PHOENICIAN,
                POLLARD_PHONETIC,
                PSALTER_PAHLAVI,
                REJANG,
                RUNIC,
                SAMARITAN,
                SAURASHTRA,
                SHARADA,
                SHAVIAN,
                SIDDHAM,
                SIGN_WRITING,
                SIMPLIFIED_HAN,
                SINHALA,
                SORA_SOMPENG,
                SUNDANESE,
                SYLOTI_NAGRI,
                SYRIAC,
                TAGALOG,
                TAGBANWA,
                TAI_LE,
                TAI_VIET,
                TAKRI,
                TAMIL,
                TANGUT,
                TELUGU,
                THAANA,
                THAI,
                TIBETAN,
                TIFINAGH,
                TIRHUTA,
                TRADITIONAL_HAN,
                UGARITIC,
                VAI,
                VARANG_KSHITI,
                YI,
// ---------------- [Other Country names] -----------------
                BENGALI, // similar to BANGLA,
                MENDE_KIKAKUI, // similar to MENDE,
                ORIYA, // similar to ODIA,
                SIMPLIFIED_CHINESE, // similar to SIMPLIFIED_HAN,
                TRADITIONAL_CHINESE, // similar to TRADITIONAL_HAN,
            };

        private:
            /**
             * The internal locale
             */
            interface BaseLocale : Object {
                Language language;
                Country country;
                Script script;

                CORE_FAST BaseLocale() :
                        language(Language::UNDEFINED), country(Country::UNDEFINED), script(Script::UNDEFINED) {}

                CORE_FAST BaseLocale(Language language, Country country, Script script) :
                        language(language), country(country), script(script) {}

                CORE_FAST BaseLocale(const BaseLocale &locale) :
                        language(locale.language), country(locale.country), script(locale.script) {}

                static BaseLocale of(gint language, gint script, gint country) {
                    return BaseLocale((Language) language, (Country) country, (Script) script);
                }

                Object &clone() const override;

                /**
                 * Return the alpha-2 code of language of this locale
                 */
                String languageCode() const;

                /**
                 * Return the alpha-3 code of language of this locale
                 */
                String languageISO3Code() const;

                /**
                 * return display language name
                 */
                String languageName() const;

                /**
                 * Return the alpha-2 code of language if this locale
                 */
                String countryCode() const;

                /**
                 * Return the alpha-3 code of country if this locale
                 */
                String countryISO3Code() const;

                /**
                 * Return the display name of country if this locale
                 */
                String countryName() const;

                /**
                 * Return the alpha-4 code of script if this locale
                 */
                String scriptCode() const;

                /**
                 * Return the display name of script if this locale
                 */
                String scriptName() const;

                gint findLocaleIndex() const;

                gint findLocaleIndex0() const;

                gint findLocaleDataIndex() const;

                /**
                 * Return the BCP 47 locale name
                 */
                String bcp47Name() const;

                String name(gchar sep) const;

                /**
                 *  Returns a native name of the language for the locale.
                 */
                String nativeLanguageName() const;

                /**
                 *  Returns a native name of the country for the locale.
                 */
                String nativeCountryName() const;

                gbool equals(const Object &object) const override;

                gbool validateSC(const BaseLocale &target) const;

                gbool validateL(Language lang) const;

                gbool validate() const;

                BaseLocale addSubTag() const;

                BaseLocale deleteSubTag() const;

                /**
                 * The CLDR array
                 */
                interface CLDR {
                    gshort language;
                    gshort script;
                    gshort country;
                    gshort startListPatternOffset;
                    gshort midListPatternOffset;
                    gshort endListPatternOffset;
                    gshort pairListPatternOffset;
                    gshort listDelimitOffset;
                    gshort decimalSeparatorOffset;
                    gshort groupDelimOffset;
                    gshort percentSymbolOffset;
                    gshort zeroDigitOffset;
                    gshort minusSignOffset;
                    gshort plusSignOffset;
                    gshort exponentialOffset;
                    gshort quoteStartOffset;
                    gshort quoteEndOffset;
                    gshort quoteStartAltOffset;
                    gshort quoteEndAltOffset;
                    gshort longDateFormatOffset;
                    gshort shortDateFormatOffset;
                    gshort longTimeFormatOffset;
                    gshort shortTimeFormatOffset;
                    gshort longDayNamesStandaloneOffset;
                    gshort longDayNamesOffset;
                    gshort shortDayNamesStandaloneOffset;
                    gshort shortDayNamesOffset;
                    gshort narrowDayNamesStandaloneOffset;
                    gshort narrowDayNamesOffset;
                    gshort amOffset;
                    gshort pmOffset;
                    gshort byteCountOffset;
                    gshort byteAmountSIOffset;
                    gshort byteAmountIECOffset;
                    gshort currencySymbolOffset;
                    gshort currencyDisplayNameOffset;
                    gshort currencyFormatOffset;
                    gshort currencyFormatNegativeOffset;
                    gshort languageNameOffset;
                    gshort countryNameOffset;
                    gshort startListPatternSize;
                    gshort midListPatternSize;
                    gshort endListPatternSize;
                    gshort pairListPatternSize;
                    gshort listDelimitSize;
                    gshort decimalSeparatorSize;
                    gshort groupDelimSize;
                    gshort percentSymbolSize;
                    gshort zeroDigitSize;
                    gshort minusSignSize;
                    gshort plusSignSize;
                    gshort exponentialSize;
                    gshort quoteStartSize;
                    gshort quoteEndSize;
                    gshort quoteStartAltSize;
                    gshort quoteEndAltSize;
                    gshort longDateFormatSize;
                    gshort shortDateFormatSize;
                    gshort longTimeFormatSize;
                    gshort shortTimeFormatSize;
                    gshort longDayNamesStandaloneSize;
                    gshort longDayNamesSize;
                    gshort shortDayNamesStandaloneSize;
                    gshort shortDayNamesSize;
                    gshort narrowDayNamesStandaloneSize;
                    gshort narrowDayNamesSize;
                    gshort amSize;
                    gshort pmSize;
                    gshort byteCountSize;
                    gshort byteAmountSISize;
                    gshort byteAmountIECSize;
                    gshort currencySymbolSize;
                    gshort currencyDisplayNameSize;
                    gshort currencyFormatSize;
                    gshort currencyFormatNegativeSize;
                    gshort languageNameSize;
                    gshort countryNameSize;
                    gshort currencyISOCode[3];
                    gshort currencyDigits;
                    gshort currencyRound;
                    gshort firstDayOfWeek;
                    gshort weekendStart;
                    gshort weekendEnd;
                    gshort groupingTop;
                    gshort groupingHigh;
                    gshort groupingLeast;
                };

                static const CLDR cldr[635];

                static const BaseLocale tags[739 << 1];

                /**
                 * Return standard language
                 */
                static Language normalize(Language language);

                /**
                 * Return standard country
                 */
                static Country normalize(Country country);

                /**
                 * Return standard script
                 */
                static Script normalize(Script script);

                static Language parseLanguage(String language);

                static Language parseLanguageName(String language);

                static Language parseNativeLanguageName(String language);

                static Country parseCountry(String country);

                static Country parseCountryName(String country);

                static Country parseNativeCountryName(String country);

                static Script parseScript(String script);

                static Script parseScriptName(String script);

                static Locale parseLocale(String locale);
            };

            /**
             * The last available language id
             */
            static CORE_FAST Language LAST_STANDARD_LANGUAGE = Language::OBOLO;

            /**
             * The last available country id
             */
            static CORE_FAST Country LAST_STANDARD_COUNTRY = Country::ZIMBABWE;

            /**
             * The last available script id
             */
            static CORE_FAST Script LAST_STANDARD_SCRIPT = Script::YI;

            /**
             * The locale property
             */
            BaseLocale base{};

            /**
             * The internal locale constructor
             *
             * @param language The language id
             * @param country The country/region id
             * @param script The script id
             */
            CORE_EXPLICIT Locale(Language language, Country country, Script script) :
                    base({language, country, script}) {}

            /**
             * The internal locale constructor
             */
            CORE_EXPLICIT Locale(BaseLocale base) : base((BaseLocale &&) base) {}

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

            /**
             * Construct a locale from a language code.
             * This constructor normalizes the language value to lowercase.
             * @note
             * <ul>
             * <li>Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms. See <a href="#legacy_language_codes">Legacy language
             * codes</a> for more information.
             * <li>For backward compatibility reasons, this constructor does not make
             * any syntactic checks on the input.
             * </ul>
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the <b> Locale</b> class description about
             * valid language values.
             */
            CORE_EXPLICIT Locale(String language);

            /**
             * Construct a locale from language and country.
             * This constructor normalizes the language value to lowercase and
             * the country value to uppercase.
             * @note
             * <ul>
             * <li>Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms. See <a href="#legacy_language_codes">Legacy language
             * codes</a> for more information.
             * <li>For backward compatibility reasons, this constructor does not make
             * any syntactic checks on the input.
             * </ul>
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the <b> Locale</b> class description about
             * valid language values.
             * @param country An ISO 3166 alpha-2 country code or a UN M.49 numeric-3 area code.
             * See the <b> Locale</b> class description about valid country values.
             */
            CORE_EXPLICIT Locale(String language, String country);

            /**
             * Construct a locale from language, country and script.
             * This constructor normalizes the language value to lowercase and
             * the country value to uppercase.
             * @note
             * <ul>
             * <li>Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms. See <a href="#legacy_language_codes">Legacy language
             * codes</a> for more information.
             * <li>For backward compatibility reasons, this constructor does not make
             * any syntactic checks on the input.
             * <li>The two cases ("ja", "JP", "JP") and ("th", "TH", "TH") are handled specially,
             * see <a href="#special_cases_constructor">Special Cases</a> for more information.
             * </ul>
             *
             * @deprecated Locale constructors have been deprecated. See <a href ="#ObtainingLocale">
             * Obtaining a Locale</a> for other options.
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the <b> Locale</b> class description about
             * valid language values.
             * @param country An ISO 3166 alpha-2 country code or a UN M.49 numeric-3 area code.
             * See the <b> Locale</b> class description about valid country values.
             * @param script An ISO 15924 alpha-4 script code.
             * See the <b> Locale</b> class description for the details.
             */
            CORE_EXPLICIT Locale(String language, String script, String country);

            /**
             * Initialized new created locale with properties of the
             * given locale
             */
            CORE_FAST Locale(const Locale &locale);

            /**
             * Returns the language code of this Locale.
             *
             * @note This method returns the new forms for the obsolete ISO 639
             * codes ("iw", "ji", and "in"). See <a href="#legacy_language_codes">
             * Legacy language codes</a> for more information.
             *
             * @return The language code, or the empty string if none is defined.
             * @see displayLanguage
             */
            String language() const;

            /**
             * Returns the script for this locale, which should
             * either be the empty string or an ISO 15924 4-letter script
             * code. The first letter is uppercase and the rest are
             * lowercase, for example, 'Latn', 'Cyrl'.
             *
             * @return The script code, or the empty string if none is defined.
             * @see displayScript
             */
            String script() const;

            /**
             * Returns the country/region code for this locale, which should
             * either be the empty string, an uppercase ISO 3166 2-letter code,
             * or a UN M.49 3-digit code.
             *
             * @return The country/region code, or the empty string if none is defined.
             * @see displayCountry
             */
            String country() const;

            /**
             * Returns a string representation of this <b> Locale</b>
             * object, consisting of language, country and script as below:
             * <blockquote>
             * language + "_" + country + "_" + script
             * </blockquote>
             *
             * Language is always lower case, country is always upper case and script is always title
             * case.  Private use subtags will be in canonical order as explained in <b style="color: orange;"> languageTag</b>.
             *
             * <p>
             * If both the language and country fields are missing, this function will return
             * the empty string, even if the script field is present
             *
             * <p>
             * This behavior is designed to support debugging and to be compatible with
             * previous uses of <b> toString</b> that expected language and country
             * fields only.  To represent a Locale as a String for interchange purposes, use
             * <b style="color: orange;"> languageTag</b>.
             *
             * <p>Examples: <ul>
             * <li><b> en</b></li>
             * <li><b> de_DE</b></li>
             * <li><b> _GB</b></li>
             * <li><b> zh_CN_Hans</b></li></ul>
             *
             * @return A string representation of the Locale, for debugging.
             * @see displayName
             * @see languageTag
             */
            String toString() const override;

            /**
             * Return locale name
             * language((_script)_country)
             */
            String name() const;

            /**
             * Returns a well-formed IETF BCP 47 language tag representing
             * this locale.
             *
             * <p>If this <b> Locale</b> has a language and country
             * that does not satisfy the IETF BCP 47 language tag
             * syntax requirements, this method handles these fields as
             * described below:
             *
             * <p><b>Language:</b> If language is empty, or not <a
             * href="#def_language" >well-formed</a> (for example "a" or
             * "e2"), it will be emitted as "und" (Undetermined).
             *
             * <p><b>Country:</b> If country is not <a
             * href="#def_region">well-formed</a> (for example "12" or "USA"),
             * it will be omitted.
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
             * <li>A locale with language "no" and country "NO",
             * representing Norwegian Nynorsk (Norway), is converted
             * to a language tag "nn-NO".</li></ul>
             *
             * <p><b>Note:</b> Although the language tag obtained by this
             * method is well-formed (satisfies the syntax requirements
             * defined by the IETF BCP 47 specification), it is not
             * necessarily a valid BCP 47 language tag.  For example,
             * <pre>
             *   Locale.forLanguageTag("xx-YY").toLanguageTag();</pre>
             *
             * will return "xx-YY", but the language subtag "xx" and the
             * region subtag "YY" are invalid because they are not registered
             * in the IANA Language Subtag Registry.
             *
             * @return a BCP47 language tag representing the locale
             * @see forLanguageTag(String)
             */
            String toLanguageTag() const;

            /**
             * Returns a locale for the specified IETF BCP 47 language tag string.
             *
             * <p>If the specified language tag contains any ill-formed subtags,
             * the first such subtag and all following subtags are ignored.
             *
             * <p>The following <b>conversions</b> are performed:<ul>
             *
             * <li>The language code "und" is mapped to language "".
             *
             * <li>The language codes "iw", "ji", and "in" are mapped to "he",
             * "yi", and "id" respectively. (This is the same canonicalization
             * that's done in Locale's constructors.) See
             * <a href="#legacy_language_codes">Legacy language codes</a>
             * for more information.
             *
             * <li>Case is normalized, which are left unchanged.
             * Language is normalized to lower case, script to
             * title case, country to upper case, and extensions to lower
             * case.
             *
             * <p>This implements the 'Language-Tag' production of BCP47, and
             * so supports legacy (regular and irregular, referred to as
             * "Type: grandfathered" in BCP47) as well as
             * private use language tags.  Stand alone private use tags are
             * represented as empty language and extension 'x-whatever',
             * and legacy tags are converted to their canonical replacements
             * where they exist.
             *
             * <p>For a list of all legacy tags, see the
             * IANA Language Subtag Registry (search for "Type: grandfathered").
             *
             * <p><b>Note</b>: there is no guarantee that <b> toLanguageTag</b>
             * and <b> forLanguageTag</b> will round-trip.
             *
             * @param languageTag the language tag
             * @return The locale that best represents the language tag.
             * @see toLanguageTag()
             */
            static Locale forLanguageTag(String languageTag);

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
             * <b style="color: orange;"> DISPLAY</b> locale.
             * For example, if the locale is fr_FR and the default
             * <b style="color: orange;"> DISPLAY</b> locale
             * is en_US, displayLanguage() will return "French"; if the locale is en_US and
             * the default <b style="color: orange;"> DISPLAY</b> locale is fr_FR,
             * displayLanguage() will return "anglais".
             * If the name returned cannot be localized for the default
             * <b style="color: orange;"> DISPLAY</b> locale,
             * (say, we don't have a Japanese name for Croatian),
             * this function falls back on the English name, and uses the ISO code as a last-resort
             * value.  If the locale doesn't specify a language, this function returns the empty string.
             *
             * @return The name of the display language.
             */
            String displayLanguage() const;

            String nativeLanguage() const;

            /**
             * Returns a name for the locale's script that is appropriate for display to
             * the user. If possible, the name will be localized for the default
             * <b style="color: orange;"> DISPLAY</b> locale.  Returns
             * the empty string if this locale doesn't specify a script code.
             *
             * @return the display name of the script code for the current default
             *     <b style="color: orange;"> DISPLAY</b> locale
             */
            String displayScript() const;

            /**
             * Returns a name for the locale's country that is appropriate for display to the
             * user.
             * If possible, the name returned will be localized for the default
             * <b style="color: orange;"> DISPLAY</b> locale.
             * For example, if the locale is fr_FR and the default
             * <b style="color: orange;"> DISPLAY</b> locale
             * is en_US, displayCountry() will return "France"; if the locale is en_US and
             * the default <b style="color: orange;"> DISPLAY</b> locale is fr_FR,
             * displayCountry() will return "Etats-Unis".
             * If the name returned cannot be localized for the default
             * <b style="color: orange;"> DISPLAY</b> locale,
             * (say, we don't have a Japanese name for Croatia),
             * this function falls back on the English name, and uses the ISO code as a last-resort
             * value.  If the locale doesn't specify a country, this function returns the empty string.
             *
             * @return The name of the country appropriate to the locale.
             */
            String displayCountry() const;

            String nativeCountry() const;

            /**
             * Returns a name for the locale that is appropriate for display to the
             * user. This will be the values returned by displayLanguage(),
             * displayScript() and displayCountry().  For example:
             * <blockquote>
             * language (script, country)<br>
             * language (country)<br>
             * script (country)<br>
             * </blockquote>
             * depending on which fields are specified in the locale. The field
             * separator in the above parentheses, denoted as a comma character, may
             * be localized depending on the locale. If the language, script and country
             * fields are all empty, this function returns the empty string.
             *
             * @return The name of the locale appropriate to display.
             */
            String displayName() const;

            /**
             * Return shadow copy of this locale
             */
            Object &clone() const override;

            /**
             * Return hashcode of this locale instance
             */
            gint hash() const override;

            /**
             * Return true if specified object is Locale instance and
             * has same properties with this locale
             *
             * @param o The object to be compared
             */
            gbool equals(const Object &o) const override;

            /**
             * Return the locale instance corresponding to specified locale representation
             *
             * @param locale The locale name
             */
            static Locale forName(String locale);

            /**
             * Obtains a locale from language, country and script
             *
             * @param language A language name. See the <b> Locale::Language</b> class description of
             * <a href="#def_language">language</a> values.
             * @param country A country name in english. See the <b> Locale::Country</b> class description of
             * <a href="#def_region">country</a> values.
             * @param script A script name in english. See the <b> Locale::Script</b> class description of
             * <a href="#def_script">script</a> values
             */
            static Locale of(String language, String script, String country);

            /**
             * Obtains a locale from language, country and script
             *
             * @param language A language name. See the <b> Locale::Language</b> class description of
             * <a href="#def_language">language</a> values.
             * @param country A country name. See the <b> Locale::Country</b> class description of
             * <a href="#def_region">country</a> values.
             * @param script A script name. See the <b> Locale::Script</b> class description of
             * <a href="#def_script">script</a> values
             */
            static Locale of(Language language, Script script, Country country);

            /**
             * Gets the current value of the default locale for host environment.
             */
            static Locale systemLocale();

            /**
             * Gets the current value of the default locale for this instance
             * of the process.
             * <p>
             * It is used by many locale-sensitive methods if no locale is explicitly specified.
             * It can be changed using the <b style="color: orange;"> setDefaultLocale</b> method.
             *
             * @return the default locale for this instance of the process.
             */
            static Locale defaultLocale();

            /**
             * Sets the default locale for this instance of the process.
             * This does not affect the host locale.
             * <p>
             * The process sets the default locale during startup
             * based on the host environment. It is used by many locale-sensitive
             * methods if no locale is explicitly specified.
             * <p>
             * Since changing the default locale may affect many different areas
             * of functionality, this method should only be used if the caller
             * is prepared to reinitialize locale-sensitive code running
             * within the same process.
             * <p>
             * By setting the default locale with this method, all of the default
             * locales for each Category are also set to the specified default locale.
             *
             * @param newLocale the new default locale
             */
            static void setDefaultLocale(Locale newLocale);

            /**
             * Return the list of locale names for use in selecting translations
             * Each entry in the returned list is the dash-joined name of a locale,
             * suitable to the user's preferences for what to translate the UI into. For
             * example, if the user has configured their system to use English as used in
             * the USA, the list would be "en-Latn-US", "en-US", "en". The order of entries
             * is the order in which to check for translations; earlier items in the list
             * are to be preferred over later ones.
             */
            static List<String> &availableUILanguages();

            /**
             * Returns a list of all installed locales.
             * At a minimum, the returned list must contain a
             * <b> Locale</b> instance equal to <b style="color: orange;"> Locale::ROOT</b> and
             * a <b> Locale</b> instance equal to <b style="color: orange;"> Locale::US</b>.
             */
            static List<Locale> &availableLocales();

            ~Locale() = default;
        };

    }
} // core

#endif //CORE23_LOCALE_H
