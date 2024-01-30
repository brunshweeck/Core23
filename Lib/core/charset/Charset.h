//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_CHARSET_H
#define CORE23_CHARSET_H

#include <core/String.h>
#include <core/util/HashSet.h>
#include <core/native/StringArray.h>
#include <core/native/CharArray2D.h>

namespace core {
    namespace charset {

        /**
         * A named mapping between sequences of sixteen-bit Unicode <a href="">code units</a> and sequences of
         * bytes.  This class defines methods for creating decoders and encoders and
         * for retrieving the various names associated with a charset.  Instances of
         * this class are immutable.
         *
         * <p> This class also defines static methods for testing whether a particular
         * charset is supported, for locating charset instances by name, and for
         * constructing a map that contains every charset for which support is
         * available in the current Java virtual machine.
         *
         * <p> All of the methods defined in this class are safe for use by multiple
         * concurrent threads.
         *
         *
         * <h2><a id="names">Charset names</a></h2>
         *
         * <p> Charsets are named by strings composed of the following characters:
         *
         * <ul>
         *
         *   <li> The uppercase letters <b> 'A'</b> through <b> 'Z'</b>
         *        (<code>'&#92;u0041'</code>&nbsp;through&nbsp;<code>'&#92;u005a'</code>),
         *
         *   <li> The lowercase letters <b> 'a'</b> through <b> 'z'</b>
         *        (<code>'&#92;u0061'</code>&nbsp;through&nbsp;<code>'&#92;u007a'</code>),
         *
         *   <li> The digits <b> '0'</b> through <b> '9'</b>
         *        (<code>'&#92;u0030'</code>&nbsp;through&nbsp;<code>'&#92;u0039'</code>),
         *
         *   <li> The dash character <b> '-'</b>
         *        (<code>'&#92;u002d'</code>,&nbsp;<small>HYPHEN-MINUS</small>),
         *
         *   <li> The plus character <b> '+'</b>
         *        (<code>'&#92;u002b'</code>,&nbsp;<small>PLUS SIGN</small>),
         *
         *   <li> The period character <b> '.'</b>
         *        (<code>'&#92;u002e'</code>,&nbsp;<small>FULL STOP</small>),
         *
         *   <li> The colon character <b> ':'</b>
         *        (<code>'&#92;u003a'</code>,&nbsp;<small>COLON</small>), and
         *
         *   <li> The underscore character <b> '_'</b>
         *        (<code>'&#92;u005f'</code>,&nbsp;<small>LOW&nbsp;LINE</small>).
         *
         * </ul>
         *
         * A charset name must begin with either a letter or a digit.  The empty string
         * is not a legal charset name.  Charset names are not case-sensitive; that is,
         * case is always ignored when comparing charset names.  Charset names
         * generally follow the conventions documented in <a
         * href="http://www.ietf.org/rfc/rfc2278.txt"><i>RFC&nbsp;2278:&nbsp;IANA Charset
         * Registration Procedures</i></a>.
         *
         * <p> Every charset has a <i>canonical name</i> and may also have one or more
         * <i>aliases</i>.  The canonical name is returned by the <b style="color:orange;"> name</b> method
         * of this class.  Canonical names are, by convention, usually in upper case.
         * The aliases of a charset are returned by the <b style="color:orange;"> aliases</b>
         * method.
         *
         * <p><a id="hn">Some charsets have an <i>historical name</i> that is defined for
         * compatibility with previous versions of the Java platform.</a>  A charset's
         * historical name is either its canonical name or one of its aliases.  The
         * historical name is returned by the <b> getEncoding()</b> methods of the
         * <b style="color:orange;"> InputStreamReader</b> and <b style="color:orange;"> OutputStreamWriter</b> classes.
         *
         * <p><a id="iana"> </a>If a charset listed in the <a
         * href="http://www.iana.org/assignments/character-sets"><i>IANA Charset
         * Registry</i></a> is supported by an implementation of the Java platform then
         * its canonical name must be the name listed in the registry. Many charsets
         * are given more than one name in the registry, in which case the registry
         * identifies one of the names as <i>MIME-preferred</i>.  If a charset has more
         * than one registry name then its canonical name must be the MIME-preferred
         * name and the other names in the registry must be valid aliases.  If a
         * supported charset is not listed in the IANA registry then its canonical name
         * must begin with one of the strings <b> "X-"</b> or <b> "x-"</b>.
         *
         * <p> The IANA charset registry does change over time, and so the canonical
         * name and the aliases of a particular charset may also change over time.  To
         * ensure compatibility it is recommended that no alias ever be removed from a
         * charset, and that if the canonical name of a charset is changed then its
         * previous canonical name be made into an alias.
         *
         *
         * <h2><a id="standard">Standard charsets</a></h2>
         *
         *
         * <p> Every implementation of the Java platform is required to support the
         * following standard charsets.  Consult the release documentation for your
         * implementation to see if any other charsets are supported.  The behavior
         * of such optional charsets may differ between implementations.
         *
         * <blockquote><table class="striped" style="width:80%">
         * <caption style="display:none">Description of standard charsets</caption>
         * <thead>
         * <tr><th scope="col" style="text-align:left">Charset</th><th scope="col" style="text-align:left">Description</th></tr>
         * </thead>
         * <tbody>
         * <tr><th scope="row" style="vertical-align:top"><b> US-ASCII</b></th>
         *     <td>Seven-bit ASCII, a.k.a. <b> ISO646-US</b>,
         *         a.k.a. the Basic Latin block of the Unicode character setValue</td></tr>
         * <tr><th scope="row" style="vertical-align:top"><code>ISO-8859-1&nbsp;&nbsp;</code></th>
         *     <td>ISO Latin Alphabet No. 1, a.k.a. <b> ISO-LATIN-1</b></td></tr>
         * <tr><th scope="row" style="vertical-align:top"><b> UTF-8</b></th>
         *     <td>Eight-bit UCS Transformation Format</td></tr>
         * <tr><th scope="row" style="vertical-align:top"><b> UTF-16BE</b></th>
         *     <td>Sixteen-bit UCS Transformation Format,
         *         big-endian byte&nbsp;order</td></tr>
         * <tr><th scope="row" style="vertical-align:top"><b> UTF-16LE</b></th>
         *     <td>Sixteen-bit UCS Transformation Format,
         *         little-endian byte&nbsp;order</td></tr>
         * <tr><th scope="row" style="vertical-align:top"><b> UTF-16</b></th>
         *     <td>Sixteen-bit UCS Transformation Format,
         *         byte&nbsp;order identified by an optional byte-order mark</td></tr>
         * </tbody>
         * </table></blockquote>
         *
         * <p> The <b> UTF-8</b> charset is specified by <a
         * href="http://www.ietf.org/rfc/rfc2279.txt"><i>RFC&nbsp;2279</i></a>; the
         * transformation format upon which it is based is specified in
         * Amendment&nbsp;2 of ISO&nbsp;10646-1 and is also described in the <a
         * href="http://www.unicode.org/standard/standard.html"><i>Unicode
         * Standard</i></a>.
         *
         * <p> The <b> UTF-16</b> charsets are specified by <a
         * href="http://www.ietf.org/rfc/rfc2781.txt"><i>RFC&nbsp;2781</i></a>; the
         * transformation formats upon which they are based are specified in
         * Amendment&nbsp;1 of ISO&nbsp;10646-1 and are also described in the <a
         * href="http://www.unicode.org/standard/standard.html"><i>Unicode
         * Standard</i></a>.
         *
         * <p> The <b> UTF-16</b> charsets use sixteen-bit quantities and are
         * therefore sensitive to byte order.  In these encodings the byte order of a
         * stream may be indicated by an initial <i>byte-order mark</i> represented by
         * the Unicode character <code>'&#92;uFEFF'</code>.  Byte-order marks are handled
         * as follows:
         *
         * <ul>
         *
         *   <li><p> When decoding, the <b> UTF-16BE</b> and <b> UTF-16LE</b>
         *   charsets interpret the initial byte-order marks as a <small>ZERO-WIDTH
         *   NON-BREAKING SPACE</small>; when encoding, they do not write
         *   byte-order marks. </p></li>
         *
         *   <li><p> When decoding, the <b> UTF-16</b> charset interprets the
         *   byte-order mark at the beginning of the input stream to indicate the
         *   byte-order of the stream but defaults to big-endian if there is no
         *   byte-order mark; when encoding, it uses big-endian byte order and writes
         *   a big-endian byte-order mark. </p></li>
         *
         * </ul>
         *
         * In any case, byte order marks occurring after the first element of an
         * input sequence are not omitted since the same code is used to represent
         * <small>ZERO-WIDTH NON-BREAKING SPACE</small>.
         *
         * <p> Every instance of the Java virtual machine has a default charset, which
         * is <b> UTF-8</b> unless changed in an implementation specific manner. Refer to
         * <b style="color:orange;"> defaultCharset()</b> for more detail.
         *
         * <p> The <b style="color:orange;"> StandardCharsets</b> class defines constants for each of the
         * standard charsets.
         *
         * <h2>Terminology</h2>
         *
         * <p> The name of this class is taken from the terms used in
         * <a href="http://www.ietf.org/rfc/rfc2278.txt"><i>RFC&nbsp;2278</i></a>.
         * In that document a <i>charset</i> is defined as the combination of
         * one or more coded character sets and a character-encoding scheme.
         * (This definition is confusing; some other software systems define
         * <i>charset</i> as a synonym for <i>coded character setValue</i>.)
         *
         * <p> A <i>coded character set</i> is a mapping between a setValue of abstract
         * characters and a setValue of integers.  US-ASCII, ISO&nbsp;8859-1,
         * JIS&nbsp;X&nbsp;0201, and Unicode are examples of coded character sets.
         *
         * <p> Some standards have defined a <i>character setValue</i> to be simply a
         * setValue of abstract characters without an associated assigned numbering.
         * An alphabet is an example of such a character setValue.  However, the subtle
         * distinction between <i>character setValue</i> and <i>coded character set</i>
         * is rarely used in practice; the former has become a short form for the
         * latter, including in the API specification.
         *
         * <p> A <i>character-encoding scheme</i> is a mapping between one or more
         * coded character sets and a setValue of octet (eight-bit byte) sequences.
         * UTF-8, UTF-16, ISO&nbsp;2022, and EUC are examples of
         * character-encoding schemes.  Encoding schemes are often associated with
         * a particular coded character setValue; UTF-8, for example, is used only to
         * encode Unicode.  Some schemes, however, are associated with multiple
         * coded character sets; EUC, for example, can be used to encode
         * characters in a variety of Asian coded character sets.
         *
         * <p> When a coded character setValue is used exclusively with a single
         * character-encoding scheme then the corresponding charset is usually
         * named for the coded character setValue; otherwise a charset is usually named
         * for the encoding scheme and, possibly, the locale of the coded
         * character sets that it supports.  Hence <b> US-ASCII</b> is both the
         * name of a coded character setValue and of the charset that encodes it, while
         * <b> EUC-JP</b> is the name of the charset that encodes the
         * JIS&nbsp;X&nbsp;0201, JIS&nbsp;X&nbsp;0208, and JIS&nbsp;X&nbsp;0212
         * coded character sets for the Japanese language.
         *
         * <p> The native character encoding of the Java programming language is
         * UTF-16.  A charset in the Java platform therefore defines a mapping
         * between sequences of sixteen-bit UTF-16 code units (that is, sequences
         * of chars) and sequences of bytes. </p>
         *
         *
         * @author Brunshweeck Tazeussong
         *
         * @see CharsetDecoder
         * @see CharsetEncoder
         * @see Character
         */
        class Charset: public Object, public Comparable<Charset> {
        private:

            /**
             * Checks that the given string is a legal charset name. </p>
             *
             * @param  s
             *         A purported charset name
             *
             * @throws  IllegalCharsetNameException
             *          If the given name is not a legal charset name
             */
             static void checkName(const String& s);

             /**
              * The setValue of available charsets
              */
             static util::Set<Charset> &available;

        public:

            /**
             * Tells whether the named charset is supported.
             *
             * @param  charsetName
             *         The name of the requested charset; may be either
             *         a canonical name or an alias
             *
             * @return  <b> true</b> if, and only if, support for the named charset
             *          is available in the current Java virtual machine
             *
             * @throws IllegalCharsetNameException
             *         If the given charset name is illegal
             *
             * @throws  IllegalArgumentException
             *          If the given <b> charsetName</b> is null
             */
             static gbool isSupported(const String& charsetName);

            /**
             * Returns a charset object for the named charset.
             *
             * @param  charsetName
             *         The name of the requested charset; may be either
             *         a canonical name or an alias
             *
             * @return  A charset object for the named charset
             *
             * @throws  IllegalCharsetNameException
             *          If the given charset name is illegal
             *
             * @throws  IllegalArgumentException
             *          If the given <b> charsetName</b> is null
             *
             * @throws  UnsupportedCharsetException
             *          If no support for the named charset is available
             *          in this instance of the Java virtual machine
             */
             static const Charset& forName(const String& charsetName);

            /**
             * Returns a charset object for the named charset. If the charset object
             * for the named charset is not available or <b> charsetName</b> is not a
             * legal charset name, then <b> fallback</b> is returned.
             *
             * @param  charsetName
             *         The name of the requested charset; may be either
             *         a canonical name or an alias
             *
             * @param  fallback
             *         fallback charset in case the charset object for the named
             *         charset is not available or <b> charsetName</b> is not a legal
             *         charset name. May be <b> null</b>
             *
             * @return  A charset object for the named charset, or <b> fallback</b>
             *          in case the charset object for the named charset is not
             *          available or <b> charsetName</b> is not a legal charset name
             *
             * @throws  IllegalArgumentException
             *          If the given <b> charsetName</b> is <b> invalid</b>
             */
             static const Charset &forName(const String& charsetName, const Charset& fallback);

            /**
             * Returns the default charset of this Java virtual machine.
             *
             * <p> The default charset is <b> UTF-8</b>, unless changed in an
             * implementation specific manner.
             *
             * @implNote An implementation may override the default charset with
             * the system property <b> file.encoding</b> on the command line. If the
             * value is <b> COMPAT</b>, the default charset is derived from
             * the <b> native.encoding</b> system property, which typically depends
             * upon the locale and charset of the underlying operating system.
             *
             * @return  A charset object for the default charset
             * @see <a href="../../lang/System.html#file.encoding">file.encoding</a>
             * @see <a href="../../lang/System.html#native.encoding">native.encoding</a>
             *
             */
             static Charset& defaultCharset();

        private:
            String const canonicalName;
            StringArray const aliasSet;

            template<class T>
            CORE_ALIAS(CacheSet, typename Class<util::Set<T>>::Ptr);

            CacheSet<String> csAliasSet = null; // used to load alias one time

        protected:
            /**
             * Initializes a new charset with the given canonical name and alias
             * setValue.
             *
             * @param  canonicalName
             *         The canonical name of this charset
             *
             * @param  aliases
             *         An array of this charset's aliases, or null if it has no aliases
             *
             * @throws IllegalCharsetNameException
             *         If the canonical name or any of the aliases are illegal
             */
             CORE_EXPLICIT Charset(String canonicalName, StringArray aliases);

        public:
            /**
             * Returns this charset's canonical name.
             *
             * @return  The canonical name of this charset
             */
             virtual String name() const;

            /**
             * Returns a setValue containing this charset's aliases.
             *
             * @return  An immutable setValue of this charset's aliases
             */
             virtual util::Set<String> &aliases() const;

            /**
             * Returns this charset's human-readable name for the default locale.
             *
             * <p> The default implementation of this method simply returns this
             * charset's canonical name.  Concrete subclasses of this class may
             * override this method in order to provide a localized display name. </p>
             *
             * @return  The display name of this charset in the default locale
             */
             virtual String displayName() const;

            /**
             * Tells whether or not this charset is registered in the <a
             * href="http://www.iana.org/assignments/character-sets">IANA Charset
             * Registry</a>.
             *
             * @return  <b> true</b> if, and only if, this charset is known by its
             *          implementor to be registered with the IANA
             */
             virtual gbool isRegistered() const;

            /**
             * Returns this charset's human-readable name for the given locale.
             *
             * <p> The default implementation of this method simply returns this
             * charset's canonical name.  Concrete subclasses of this class may
             * override this method in order to provide a localized display name. </p>
             *
             * @param  locale
             *         The locale for which the display name is to be retrieved
             *
             * @return  The display name of this charset in the given locale
             */
             virtual String displayName(const util::Locale& locale) const;

            /**
             * Tells whether or not this charset contains the given charset.
             *
             * <p> A charset <i>C</i> is said to <i>contain</i> a charset <i>D</i> if,
             * and only if, every character representable in <i>D</i> is also
             * representable in <i>C</i>.  If this relationship holds then it is
             * guaranteed that every string that can be encoded in <i>D</i> can also be
             * encoded in <i>C</i> without performing any replacements.
             *
             * <p> That <i>C</i> contains <i>D</i> does not imply that each character
             * representable in <i>C</i> by a particular byte sequence is represented
             * in <i>D</i> by the same byte sequence, although sometimes this is the
             * case.
             *
             * <p> Every charset contains itself.
             *
             * <p> This method computes an approximation of the containment relation:
             * If it returns <b> true</b> then the given charset is known to be
             * contained by this charset; if it returns <b> false</b>, however, then
             * it is not necessarily the case that the given charset is not contained
             * in this charset.
             *
             * @param   cs
             *          The given charset
             *
             * @return  <b> true</b> if the given charset is contained in this charset
             */
             virtual gbool contains(const Charset& cs) const = 0;

            /**
             * Constructs a new decoder for this charset.
             *
             * @return  A new decoder for this charset
             */
             virtual CharsetDecoder &decoder() const = 0;

            /**
             * Constructs a new encoder for this charset.
             *
             * @return  A new encoder for this charset
             *
             * @throws  UnsupportedOperationException
             *          If this charset does not support encoding
             */
             virtual CharsetEncoder &encoder() const = 0;

            /**
             * Tells whether or not this charset supports encoding.
             *
             * <p> Nearly all charsets support encoding.  The primary exceptions are
             * special-purpose <i>auto-detect</i> charsets whose decoders can determine
             * which of several possible encoding schemes is in use by examining the
             * input byte sequence.  Such charsets do not support encoding because
             * there is no way to determine which encoding should be used on output.
             * Implementations of such charsets should override this method to return
             * <b> false</b>. </p>
             *
             * @return  <b> true</b> if, and only if, this charset supports encoding
             */
             virtual gbool canEncode() const;

            /**
             * Convenience method that decodes bytes in this charset into Unicode
             * characters.
             *
             * <p> An invocation of this method upon a charset <b> cs</b> returns the
             * same result as the expression
             *
             * <pre>
             *     cs.newDecoder()
             *       .onMalformedInput(CodingErrorAction.REPLACE)
             *       .onUnmappableCharacter(CodingErrorAction.REPLACE)
             *       .decode(bb); </pre>
             *
             * except that it is potentially more efficient because it can cache
             * decoders between successive invocations.
             *
             * <p> This method always replaces malformed-input and unmappable-character
             * sequences with this charset's default replacement byte array.  In order
             * to detect such sequences, use the {@link
             * CharsetDecoder#decode(java.nio.ByteBuffer)</b> method directly.  </p>
             *
             * @param  bb  The byte buffer to be decoded
             *
             * @return  A char buffer containing the decoded characters
             */
             virtual io::CharBuffer &decode(io::ByteBuffer& bb) const;

            /**
             * Convenience method that encodes Unicode characters into bytes in this
             * charset.
             *
             * <p> An invocation of this method upon a charset <b> cs</b> returns the
             * same result as the expression
             *
             * <pre>
             *     cs.newEncoder()
             *       .onMalformedInput(CodingErrorAction.REPLACE)
             *       .onUnmappableCharacter(CodingErrorAction.REPLACE)
             *       .encode(bb); </pre>
             *
             * except that it is potentially more efficient because it can cache
             * encoders between successive invocations.
             *
             * <p> This method always replaces malformed-input and unmappable-character
             * sequences with this charset's default replacement string.  In order to
             * detect such sequences, use the {@link
             * CharsetEncoder#encode(java.nio.CharBuffer)</b> method directly.  </p>
             *
             * @param  cb  The char buffer to be encoded
             *
             * @return  A byte buffer containing the encoded characters
             */
             virtual io::ByteBuffer &encode(io::CharBuffer& cb) const;

            /**
             * Convenience method that encodes a string into bytes in this charset.
             *
             * <p> An invocation of this method upon a charset <b> cs</b> returns the
             * same result as the expression
             *
             * <pre>
             *     cs.encode(CharBuffer.wrap(s)); </pre>
             *
             * @param  str  The string to be encoded
             *
             * @return  A byte buffer containing the encoded characters
             */
             virtual io::ByteBuffer& encode(const String& str) const;

            /**
             * Compares this charset to another.
             *
             * <p> Charsets are ordered by their canonical names, without regard to
             * case. </p>
             *
             * @param  that
             *         The charset to which this charset is to be compared
             *
             * @return A negative integer, zero, or a positive integer as this charset
             *         is less than, equal to, or greater than the specified charset
             */
             gint compareTo(const Charset& that) const override;

            /**
             * Computes a hashcode for this charset.
             *
             * @return  An integer hashcode
             */
            gint hash() const override;

            /**
             * Tells whether or not this object is equal to another.
             *
             * <p> Two charsets are equal if, and only if, they have the same canonical
             * names.  A charset is never equal to any other type of object.  </p>
             *
             * @return  <b> true</b> if, and only if, this charset is equal to the
             *          given object
             */
            gbool equals(const Object &o) const override;

            /**
             * Returns a string describing this charset.
             *
             * @return  A string describing this charset
             */
            String toString() const override;

            /**
             * A typesafe enumeration for coding-error actions.
             *
             * <p> Instances of this class are used to specify how malformed-input and
             * unmappable-character errors are to be handled by charset <a
             * href="CharsetDecoder.html#cae">decoders</a> and <a
             * href="CharsetEncoder.html#cae">encoders</a>.  </p>
             *
             *
             * @author Brunshweeck Tazeussong
             */
             enum CodingErrorAction: gbyte {

                 /**
                  * Action indicating that a coding error is to be handled by dropping the
                  * erroneous input and resuming the coding operation.
                  */
                  IGNORE,

                 /**
                  * Action indicating that a coding error is to be handled by dropping the
                  * erroneous input, appending the coder's replacement value to the output
                  * buffer, and resuming the coding operation.
                  */
                  REPLACE,

                 /**
                  * Action indicating that a coding error is to be reported, either by
                  * returning a <b style="color:orange;"> CoderResult</b> object or by throwing a <b style="color:orange;"> 
                  * CharacterCodingException</b>, whichever is appropriate for the method
                  * implementing the coding process.
                  */
                  REPORT,
             };

            /**
             * A typesafe enumeration for byte orders.
             *
             * @author Brunshweeck Tazeussong
             */
            enum ByteOrder: gbool {

                /**
                 * Constant denoting big-endian byte order.  In this order, the bytes of a
                 * multibyte value are ordered from most significant to least significant.
                 */
                BIG_ENDIAN,

                /**
                 * Constant denoting little-endian byte order.  In this order, the bytes of
                 * a multibyte value are ordered from least significant to most
                 * significant.
                 */
                LITTLE_ENDIAN,

                // Retrieve the private byte order. It's used early during bootstrap, and
                // must be initialized after BIG_ENDIAN and LITTLE_ENDIAN.
                NATIVE_ENDIAN = native::Unsafe::BIG_ENDIAN ? BIG_ENDIAN : LITTLE_ENDIAN,
            };
        };

    }
} // core

#endif //CORE23_CHARSET_H
