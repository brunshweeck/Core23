//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_URI_H
#define CORE23_URI_H

#include <core/String.h>
#include <core/private/Null.h>
#include <core/StringBuffer.h>
#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>
#include <core/charset/CharsetEncoder.h>

namespace core {
    namespace net {

        /**
         * Represents a Uniform Resource Identifier (URI) reference.
         *
         * <p> Aside from some minor deviations noted below, an instance of this
         * class represents a URI reference as defined by
         * <a href="http://www.ietf.org/rfc/rfc2396.txt"><i>RFC&nbsp;2396: Uniform
         * Resource Identifiers (URI): Generic Syntax</i></a>, amended by <a
         * href="http://www.ietf.org/rfc/rfc2732.txt"><i>RFC&nbsp;2732: Format for
         * Literal IPv6 Addresses in URLs</i></a>. The Literal IPv6 address format
         * also supports scope_ids. The syntax and usage of scope_ids is described
         * <a href="">here</a>.
         * This class provides constructors for creating URI instances from
         * their components or by parsing their string forms, methods for accessing the
         * various components of an instance, and methods for normalizing, resolving,
         * and relativizing URI instances.  Instances of this class are immutable.
         *
         *
         * <h2> URI syntax and components </h2>
         *
         * At the highest level a URI reference (hereinafter simply "URI") in string
         * form has the syntax
         *
         * <blockquote>
         * [<i>scheme</i><b><b> :</b> </b>]<i>scheme-specific-part</i>[<b><b> #</b> </b><i>fragment</i>]
         * </blockquote>
         *
         * where square brackets [...] delineate optional components and the characters
         * <b><b> :</b> </b> and <b><b> #</b> </b> stand for themselves.
         *
         * <p> An <i>absolute</i> URI specifies a scheme; a URI that is not absolute is
         * said to be <i>relative</i>.  URIs are also classified according to whether
         * they are <i>opaque</i> or <i>hierarchical</i>.
         *
         * <p> An <i>opaque</i> URI is an absolute URI whose scheme-specific part does
         * not begin with a slash character (<b> '/'</b> ).  Opaque URIs are not
         * subject to further parsing.  Some examples of opaque URIs are:
         *
         * <blockquote><ul>
         * <li><b> mailto:cxx-net@ www.example.com</b> </li>
         * <li><b> news:comp.lang.cxx</b> </li>
         * <li><b> urn:isbn:096139210x</b> </li>
         * </ul></blockquote>
         *
         * <p> A <i>hierarchical</i> URI is either an absolute URI whose
         * scheme-specific part begins with a slash character, or a relative URI, that
         * is, a URI that does not specify a scheme.  Some examples of hierarchical
         * URIs are:
         *
         * <blockquote>
         * <b> http://example.com/languages/cxx/</b> <br>
         * <b> sample/a/index.html#28</b> <br>
         * <b> ../../demo/b/index.html</b> <br>
         * <b> file:///~/calendar</b> 
         * </blockquote>
         *
         * <p> A hierarchical URI is subject to further parsing according to the syntax
         *
         * <blockquote>
         * [<i>scheme</i><b><b> :</b> </b>][<b><b> //</b> </b><i>authority</i>][<i>path</i>][<b><b> ?</b> </b><i>query</i>][<b><b> #</b> </b><i>fragment</i>]
         * </blockquote>
         *
         * where the characters <b><b> :</b> </b>, <b><b> /</b> </b>,
         * <b><b> ?</b> </b>, and <b><b> #</b> </b> stand for themselves.  The
         * scheme-specific part of a hierarchical URI consists of the characters
         * between the scheme and fragment components.
         *
         * <p> The authority component of a hierarchical URI is, if specified, either
         * <i>server-based</i> or <i>registry-based</i>.  A server-based authority
         * parses according to the familiar syntax
         *
         * <blockquote>
         * [<i>user-info</i><b><b> @ </b> </b>]<i>host</i>[<b><b> :</b> </b><i>port</i>]
         * </blockquote>
         *
         * where the characters <b><b> @ </b> </b> and <b><b> :</b> </b> stand for
         * themselves.  Nearly all URI schemes currently in use are server-based.  An
         * authority component that does not parse in this way is considered to be
         * registry-based.
         *
         * <p> The path component of a hierarchical URI is itself said to be absolute
         * if it begins with a slash character (<b> '/'</b> ); otherwise it is
         * relative.  The path of a hierarchical URI that is either absolute or
         * specifies an authority is always absolute.
         *
         * <p> All told, then, a URI instance has the following nine components:
         *
         * <table>
         * <caption style="display:none">Describes the components of a URI:scheme,scheme-specific-part,authority,user-info,host,port,path,query,fragment</caption>
         * <thead>
         * <tr><th scope="col">Component</th><th scope="col">Type</th></tr>
         * </thead>
         * <tbody >
         * <tr><th scope="row">scheme</th><td><b> String</b> </td></tr>
         * <tr><th scope="row">scheme-specific-part</th><td><b> String</b> </td></tr>
         * <tr><th scope="row">authority</th><td><b> String</b> </td></tr>
         * <tr><th scope="row">user-info</th><td><b> String</b> </td></tr>
         * <tr><th scope="row">host</th><td><b> String</b> </td></tr>
         * <tr><th scope="row">port</th><td><b> gint</b> </td></tr>
         * <tr><th scope="row">path</th><td><b> String</b> </td></tr>
         * <tr><th scope="row">query</th><td><b> String</b> </td></tr>
         * <tr><th scope="row">fragment</th><td><b> String</b> </td></tr>
         * </tbody>
         * </table>
         *
         * In a given instance any particular component is either <i>undefined</i> or
         * <i>defined</i> with a distinct value.  Undefined string components are
         * represented by <b> null</b> , while undefined integer components are
         * represented by <b> -1</b> .  A string component may be defined to have the
         * empty string as its value; this is not equivalent to that component being
         * undefined.
         *
         * <p> Whether a particular component is or is not defined in an instance
         * depends upon the type of the URI being represented.  An absolute URI has a
         * scheme component.  An opaque URI has a scheme, a scheme-specific part, and
         * possibly a fragment, but has no other components.  A hierarchical URI always
         * has a path (though it may be empty) and a scheme-specific-part (which at
         * least contains the path), and may have any of the other components.  If the
         * authority component is present and is server-based then the host component
         * will be defined and the user-information and port components may be defined.
         *
         *
         * <h3> Operations on URI instances </h3>
         *
         * The key operations supported by this class are those of
         * <i>normalization</i>, <i>resolution</i>, and <i>relativization</i>.
         *
         * <p> <i>Normalization</i> is the process of removing unnecessary <b> "."</b> 
         * and <b> ".."</b>  segments from the path component of a hierarchical URI.
         * Each <b> "."</b>  segment is simply removed.  A <b> ".."</b>  segment is
         * removed only if it is preceded by a non-<b> ".."</b>  segment.
         * Normalization has no effect upon opaque URIs.
         *
         * <p> <i>Resolution</i> is the process of resolving one URI against another,
         * <i>base</i> URI.  The resulting URI is constructed from components of both
         * URIs in the manner specified by RFC&nbsp;2396, taking components from the
         * base URI for those not specified in the original.  For hierarchical URIs,
         * the path of the original is resolved against the path of the base and then
         * normalized.  The result, for example, of resolving
         *
         * <blockquote>
         * <b> sample/a/index.html#28</b> 
         * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
         * &nbsp;&nbsp;&nbsp;&nbsp;(1)
         * </blockquote>
         *
         * against the base URI <b> http://example.com/languages/cxx/</b>  is the result
         * URI
         *
         * <blockquote>
         * <b> http://example.com/languages/cxx/sample/a/index.html#28</b>
         * </blockquote>
         *
         * Resolving the relative URI
         *
         * <blockquote>
         * <b> ../../demo/b/index.html</b> &nbsp;&nbsp;&nbsp;&nbsp;(2)
         * </blockquote>
         *
         * against this result yields, in turn,
         *
         * <blockquote>
         * <b> http://example.com/languages/cxx/demo/b/index.html</b>
         * </blockquote>
         *
         * Resolution of both absolute and relative URIs, and of both absolute and
         * relative paths in the case of hierarchical URIs, is supported.  Resolving
         * the URI <b> file:///~calendar</b>  against any other URI simply yields the
         * original URI, since it is absolute.  Resolving the relative URI (2) above
         * against the relative base URI (1) yields the normalized, but still relative,
         * URI
         *
         * <blockquote>
         * <b> demo/b/index.html</b> 
         * </blockquote>
         *
         * <p> <i>Relativization</i>, finally, can be regarded as the inverse of resolution.
         * Let <i>u</i> be any normalized absolute URI ending with a slash character (<b> '/'</b> )
         * and <i>v</i> be any normalized relative URI not beginning with a period character (<b> '.'</b> )
         * or slash character (<b> '/'</b> ). Then, the following statement is true:
         *
         * <blockquote>
         *   <i>u</i><b> .relativize(</b> <i>u</i><b> .resolve(</b> <i>v</i><b> )).equals(</b> <i>v</i><b> )</b> 
         * </blockquote>
         *
         * Let <i>u</i> be any normalized absolute URI ending with a slash character (<b> '/'</b> )
         * and <i>v</i> be any normalized absolute URI. Then, the following statement is true:
         *
         * <blockquote>
         *   <i>u</i><b> .resolve(</b> <i>u</i><b> .relativize(</b> <i>v</i><b> )).equals(</b> <i>v</i><b> )</b> 
         * </blockquote>
         *
         * This operation is often useful when constructing a document containing URIs
         * that must be made relative to the base URI of the document wherever
         * possible.  For example, relativizing the URI
         *
         * <blockquote>
         * <b> http://example.com/languages/cxx/sample/a/index.html#28</b>
         * </blockquote>
         *
         * against the base URI
         *
         * <blockquote>
         * <b> http://example.com/languages/cxx/</b>
         * </blockquote>
         *
         * yields the relative URI <b> sample/a/index.html#28</b> .
         *
         *
         * <h3> Character categories </h3>
         *
         * RFC&nbsp;2396 specifies precisely which characters are permitted in the
         * various components of a URI reference.  The following categories, most of
         * which are taken from that specification, are used below to describe these
         * constraints:
         *
         * <table>
         * <caption style="display:none">Describes categories alpha,digit,alphanum,unreserved,punct,reserved,escaped,and other</caption>
         *   <thead>
         *   <tr><th scope="col">Category</th><th scope="col">Description</th></tr>
         *   </thead>
         *   <tbody>
         *   <tr><th >alpha</th>
         *       <td>The US-ASCII alphabetic characters,
         *        <b> 'A'</b> &nbsp;through&nbsp;<b> 'Z'</b> 
         *        and <b> 'a'</b> &nbsp;through&nbsp;<b> 'z'</b> </td></tr>
         *   <tr><th>digit</th>
         *       <td>The US-ASCII decimal digit characters,
         *       <b> '0'</b> &nbsp;through&nbsp;<b> '9'</b> </td></tr>
         *   <tr><th >alphanum</th>
         *       <td>All <i>alpha</i> and <i>digit</i> characters</td></tr>
         *   <tr><th >unreserved</th>
         *       <td>All <i>alphanum</i> characters together with those in the string
         *        <b> "_-!.~'()*"</b> </td></tr>
         *   <tr><th >punct</th>
         *       <td>The characters in the string <b> ",;:$&+="</b> </td></tr>
         *   <tr><th >reserved</th>
         *       <td>All <i>punct</i> characters together with those in the string
         *        <b> "?/[]\@"</b> </td></tr>
         *   <tr><th >escaped</th>
         *       <td>Escaped octets, that is, triplets consisting of the percent
         *           character (<b> '%'</b> ) followed by two hexadecimal digits
         *           (<b> '0'</b> -<b> '9'</b> , <b> 'A'</b> -<b> 'F'</b> , and
         *           <b> 'a'</b> -<b> 'f'</b> )</td></tr>
         *   <tr><th >other</th>
         *       <td>The Unicode characters that are not in the US-ASCII character set,
         *           are not control characters (according to the <b style="color: orange;"> 
         *           Character.isISOControl</b> method), and are not space characters (according to the
         *           <b style="color: orange;"> Character.isSpaceChar</b> method)
         *           &nbsp;&nbsp;<i>(<b>Deviation from RFC 2396</b>, which is limited to US-ASCII)</i></td></tr>
         * </tbody>
         * </table>
         *
         * <p><a id="legal-chars"></a> The set of all legal URI characters consists of
         * the <i>unreserved</i>, <i>reserved</i>, <i>escaped</i>, and <i>other</i>
         * characters.
         *
         *
         * <h3> Escaped octets, quotation, encoding, and decoding </h3>
         *
         * RFC 2396 allows escaped octets to appear in the user-info, path, query, and
         * fragment components.  Escaping serves two purposes in URIs:
         *
         * <ul>
         *
         *   <li><p> To <i>encode</i> non-US-ASCII characters when a URI is required to
         *   conform strictly to RFC&nbsp;2396 by not containing any <i>other</i>
         *   characters.  </p></li>
         *
         *   <li><p> To <i>quote</i> characters that are otherwise illegal in a
         *   component.  The user-info, path, query, and fragment components differ
         *   slightly in terms of which characters are considered legal and illegal.
         *   </p></li>
         *
         * </ul>
         *
         * These purposes are served in this class by three related operations:
         *
         * <ul>
         *
         *   <li><p><a id="encode"></a> A character is <i>encoded</i> by replacing it
         *   with the sequence of escaped octets that represent that character in the
         *   UTF-8 character set.  The Euro currency symbol (<b> '\\u20AC'</b> ),
         *   for example, is encoded as <b> "%E2%82%AC"</b> .  <i>(<b>Deviation from
         *   RFC&nbsp;2396</b>, which does not specify any particular character
         *   set.)</i> </p></li>
         *
         *   <li><p><a id="quote"></a> An illegal character is <i>quoted</i> simply by
         *   encoding it.  The space character, for example, is quoted by replacing it
         *   with <b> "%20"</b> .  UTF-8 contains US-ASCII, hence for US-ASCII
         *   characters this transformation has exactly the effect required by
         *   RFC&nbsp;2396. </p></li>
         *
         *   <li><p><a id="decode"></a>
         *   A sequence of escaped octets is <i>decoded</i> by
         *   replacing it with the sequence of characters that it represents in the
         *   UTF-8 character set.  UTF-8 contains US-ASCII, hence decoding has the
         *   effect of de-quoting any quoted US-ASCII characters as well as that of
         *   decoding any encoded non-US-ASCII characters.  If a <a
         *   href="../nio/charset/CharsetDecoder.html#ce">decoding error</a> occurs
         *   when decoding the escaped octets then the erroneous octets are replaced by
         *   <b> '\\uFFFD'</b> , the Unicode replacement character.  </p></li>
         *
         * </ul>
         *
         * These operations are exposed in the constructors and methods of this class
         * as follows:
         *
         * <ul>
         *
         *   <li><p> The <b style="color: green;"> URI(String) single-argument
         *   constructor</b>  requires any illegal characters in its argument to be
         *   quoted and preserves any escaped octets and <i>other</i> characters that
         *   are present.  </p></li>
         *
         *   <li><p> The <b style="color: green;"> URI(String,String,String,gint,String,String,String)
         *   multi-argument constructors</b>  quote illegal characters as
         *   required by the components in which they appear.  The percent character
         *   (<b> '%'</b> ) is always quoted by these constructors.  Any <i>other</i>
         *   characters are preserved.  </p></li>
         *
         *   <li><p> The <b style="color: orange;"> getRawUserInfo</b> , <b style="color: orange;">
         *   getRawPath</b> , <b style="color: orange;"> getRawQuery</b> , <b style="color: orange;">
         *   getRawFragment</b> , <b style="color: orange;"> getRawAuthority</b> , and <b style="color: orange;">
         *   getRawSchemeSpecificPart</b>  methods return the
         *   values of their corresponding components in raw form, without interpreting
         *   any escaped octets.  The strings returned by these methods may contain
         *   both escaped octets and <i>other</i> characters, and will not contain any
         *   illegal characters.  </p></li>
         *
         *   <li><p> The <b style="color: orange;"> getUserInfo</b> , <b style="color: orange;">
         *   getPath</b> , <b style="color: orange;"> getQuery</b> , <b style="color: orange;">
         *   getFragment</b> , <b style="color: orange;"> getAuthority</b> , and <b style="color: orange;">
         *   getSchemeSpecificPart</b>  methods decode any escaped
         *   octets in their corresponding components.  The strings returned by these
         *   methods may contain both <i>other</i> characters and illegal characters,
         *   and will not contain any escaped octets.  </p></li>
         *
         *   <li><p> The <b style="color: orange;"> toString</b>  method returns a URI string with
         *   all necessary quotation but which may contain <i>other</i> characters.
         *   </p></li>
         *
         *   <li><p> The <b style="color: orange;"> toASCIIString</b>  method returns a fully
         *   quoted and encoded URI string that does not contain any <i>other</i>
         *   characters.  </p></li>
         *
         * </ul>
         *
         *
         * <h3> Identities </h3>
         *
         * For any URI <i>u</i>, it is always the case that
         *
         * <blockquote>
         * <b> new URI(</b> <i>u</i><b> .toString()).equals(</b> <i>u</i><b> )</b> &nbsp;.
         * </blockquote>
         *
         * For any URI <i>u</i> that does not contain redundant syntax such as two
         * slashes before an empty authority (as in <b> file:///tmp/</b> &nbsp;) or a
         * colon following a host name but no port (as in
         * <b> http://www.example.com:</b> &nbsp;), and that does not encode characters
         * except those that must be quoted, the following identities also hold:
         * <pre>
         *     new URI(<i>u</i>.getScheme(),
         *             <i>u</i>.getSchemeSpecificPart(),
         *             <i>u</i>.getFragment())
         *     .equals(<i>u</i>)</pre>
         * in all cases,
         * <pre>
         *     new URI(<i>u</i>.getScheme(),
         *             <i>u</i>.getAuthority(),
         *             <i>u</i>.getPath(), <i>u</i>.getQuery(),
         *             <i>u</i>.getFragment())
         *     .equals(<i>u</i>)</pre>
         * if <i>u</i> is hierarchical, and
         * <pre>
         *     new URI(<i>u</i>.getScheme(),
         *             <i>u</i>.getUserInfo(), <i>u</i>.getHost(), <i>u</i>.getPort(),
         *             <i>u</i>.getPath(), <i>u</i>.getQuery(),
         *             <i>u</i>.getFragment())
         *     .equals(<i>u</i>)</pre>
         * if <i>u</i> is hierarchical and has either no authority or a server-based
         * authority.
         *
         *
         * <h3> URIs, URLs, and URNs </h3>
         *
         * A URI is a uniform resource <i>identifier</i> while a URL is a uniform
         * resource <i>locator</i>.  Hence every URL is a URI, abstractly speaking, but
         * not every URI is a URL.  This is because there is another subcategory of
         * URIs, uniform resource <i>names</i> (URNs), which name resources but do not
         * specify how to locate them.  The <b> mailto</b> , <b> news</b> , and
         * <b> isbn</b>  URIs shown above are examples of URNs.
         *
         * <p> The conceptual distinction between URIs and URLs is reflected in the
         * differences between this class and the <b style="color: orange;"> URL</b>  class.
         *
         * <p> An instance of this class represents a URI reference in the syntactic
         * sense defined by RFC&nbsp;2396.  A URI may be either absolute or relative.
         * A URI string is parsed according to the generic syntax without regard to the
         * scheme, if any, that it specifies.  No lookup of the host, if any, is
         * performed, and no scheme-dependent stream handler is constructed.  Equality,
         * hashing, and comparison are defined strictly in terms of the character
         * content of the instance.  In other words, a URI instance is little more than
         * a structured string that supports the syntactic, scheme-independent
         * operations of comparison, normalization, resolution, and relativization.
         *
         * <p> An instance of the <b style="color: orange;"> URL</b>  class, by contrast, represents the
         * syntactic components of a URL together with some of the information required
         * to access the resource that it describes.  A URL must be absolute, that is,
         * it must always specify a scheme.  A URL string is parsed according to its
         * scheme.  A stream handler is always established for a URL, and in fact it is
         * impossible to create a URL instance for a scheme for which no handler is
         * available.  Equality and hashing depend upon both the scheme and the
         * Internet address of the host, if any; comparison is not defined.  In other
         * words, a URL is a structured string that supports the syntactic operation of
         * resolution as well as the network I/O operations of looking up the host and
         * opening a connection to the specified resource.
         *
         * @apiNote
         *
         * Applications working with file paths and file URIs should take great
         * care to use the appropriate methods to convert between the two.
         * The <b style="color: orange;"> File.File(URI)</b>  constructor can be used to create
         * <b style="color: orange;"> File</b>  objects from a file URI. and <b style="color: orange;"> File.toURI()</b>
         * can be used to create a <b style="color: orange;"> URI</b>  from a file path.
         * Applications should never try to <b style="color: green;">  URI(String, String, String, gint, String, String, String)
         * construct</b> , <b style="color: green;"> parse</b> , or <b style="color: green;"> resolve</b>  a <b> URI</b>
         * from the direct string representation of a <b> File</b>  or <b> Path</b>  instance.
         *
         * <p>
         * Some components of a URL or URI, such as <i>userinfo</i>, may
         * be abused to construct misleading URLs or URIs. Applications
         * that deal with URLs or URIs should take into account
         * the recommendations advised in <a
         * href="https://tools.ietf.org/html/rfc3986#section-7">RFC3986,
         * Section 7, Security Considerations</a>.
         *
         * @author Brunshweeck Tazeussong
         *
         * @see <a href="http://www.ietf.org/rfc/rfc2279.txt"><i>RFC&nbsp;2279: UTF-8, a
         * transformation format of ISO 10646</i></a>
         * @see <a href="http://www.ietf.org/rfc/rfc2373.txt"><i>RFC&nbsp;2373: IPv6 Addressing
         * Architecture</i></a>
         * @see <a href="http://www.ietf.org/rfc/rfc2396.txt"><i>RFC&nbsp;2396: Uniform
         * Resource Identifiers (URI): Generic Syntax</i></a>
         * @see <a href="http://www.ietf.org/rfc/rfc2732.txt"><i>RFC&nbsp;2732: Format for
         * Literal IPv6 Addresses in URLs</i></a>
         * @see <a href="URISyntaxException.html">URISyntaxException</a>
         */
        class URI CORE_FINAL : public Object, public Comparable<URI> {
        private:

            // Components of all URIs: [<scheme>:]<scheme-specific-part>[#<fragment>]
            String uriScheme;      // empty ==> relative URI
            String uriFragment;

            // Hierarchical URI components: [//<authority>]<path>[?<query>]
            String uriAuthority;   // Registry or server

            // Server-based authority: [<userInfo>@]<host>[:<port>]
            String uriUserInfo;
            String uriHost;        // empty ==> registry-based
            gint uriPort = -1;     // -1 ==> undefined

            // Remaining components of hierarchical URIs
            String uriPath;
            String uriQuery;

            // The remaining fields may be computed on demand, which is safe even in
            // the face of multiple threads racing to initialize them
            String uriSchemeSpecificPart;
            gint uriHash = 0;      // Zero ==> undefined

            String decodedUserInfo;
            String decodedAuthority;
            String decodedPath;
            String decodedQuery;
            String decodedFragment;
            String decodedSchemeSpecificPart;

            /**
             * The string form of this URI.
             *
             */
            String string;

            /**
             * Private constructor used internally
             */
            CORE_FAST URI() = default;

            /**
             * Constructs a URI by parsing the given string.
             *
             * <p> This constructor parses the given string exactly as specified by the
             * grammar in <a
             * href="http://www.ietf.org/rfc/rfc2396.txt">RFC&nbsp;2396</a>,
             * Appendix&nbsp;A, <b><i>except for the following deviations:</i></b> </p>
             *
             * <ul>
             *
             *   <li><p> An empty authority component is permitted as long as it is
             *   followed by a non-empty path, a query component, or a fragment
             *   component.  This allows the parsing of URIs such as
             *   <b> "file:///foo/bar"</b>, which seems to be the intent of
             *   RFC&nbsp;2396 although the grammar does not permit it.  If the
             *   authority component is empty then the user-information, host, and port
             *   components are undefined. </p></li>
             *
             *   <li><p> Empty relative paths are permitted; this seems to be the
             *   intent of RFC&nbsp;2396 although the grammar does not permit it.  The
             *   primary consequence of this deviation is that a standalone fragment
             *   such as <b> "#foo"</b> parses as a relative URI with an empty path
             *   and the given fragment, and can be usefully <a href="">resolved</a> against a base URI.
             *
             *   <li><p> IPv4 addresses in host components are parsed rigorously, as
             *   specified by <a
             *   href="http://www.ietf.org/rfc/rfc2732.txt">RFC&nbsp;2732</a>: Each
             *   element of a dotted-quad address must contain no more than three
             *   decimal digits.  Each element is further constrained to have a value
             *   no greater than 255. </p></li>
             *
             *   <li> <p> Hostnames in host components that comprise only a single
             *   domain label are permitted to start with an <i>alphanum</i>
             *   character. This seems to be the intent of <a
             *   href="http://www.ietf.org/rfc/rfc2396.txt">RFC&nbsp;2396</a>
             *   section&nbsp;3.2.2 although the grammar does not permit it. The
             *   consequence of this deviation is that the authority component of a
             *   hierarchical URI such as <b> s://123</b>, will parse as a server-based
             *   authority. </p></li>
             *
             *   <li><p> IPv6 addresses are permitted for the host component.  An IPv6
             *   address must be enclosed in square brackets (<b> '['</b> and
             *   <b> ']'</b>) as specified by <a
             *   href="http://www.ietf.org/rfc/rfc2732.txt">RFC&nbsp;2732</a>.  The
             *   IPv6 address itself must parse according to <a
             *   href="http://www.ietf.org/rfc/rfc2373.txt">RFC&nbsp;2373</a>.  IPv6
             *   addresses are further constrained to describe no more than sixteen
             *   bytes of address information, a constraint implicit in RFC&nbsp;2373
             *   but not expressible in the grammar. </p></li>
             *
             *   <li><p> Characters in the <i>other</i> category are permitted wherever
             *   RFC&nbsp;2396 permits <i>escaped</i> octets, that is, in the
             *   user-information, path, query, and fragment components, as well as in
             *   the authority component if the authority is registry-based.  This
             *   allows URIs to contain Unicode characters beyond those in the US-ASCII
             *   character set. </p></li>
             *
             * </ul>
             *
             * @param  str   The string to be parsed into a URI
             *
             * @throws  NullPointerException
             *          If <b> str</b> is <b> null</b>
             *
             * @throws  URISyntaxException
             *          If the given string violates RFC&nbsp;2396, as augmented
             *          by the above deviations
             */
            CORE_EXPLICIT URI(String str);

            /**
             * Constructs a hierarchical URI from the given components.
             *
             * <p> If a scheme is given then the path, if also given, must either be
             * empty or begin with a slash character (<b> '/'</b>).  Otherwise a
             * component of the new URI may be left undefined by passing <b> null</b>
             * for the corresponding parameter or, in the case of the <b> port</b>
             * parameter, by passing <b> -1</b>.
             *
             * <p> This constructor first builds a URI string from the given components
             * according to the rules specified in <a
             * href="http://www.ietf.org/rfc/rfc2396.txt">RFC&nbsp;2396</a>,
             * section&nbsp;5.2, step&nbsp;7: </p>
             *
             * <ol>
             *
             *   <li><p> Initially, the result string is empty. </p></li>
             *
             *   <li><p> If a scheme is given then it is appended to the result,
             *   followed by a colon character (<b> ':'</b>).  </p></li>
             *
             *   <li><p> If user information, a host, or a port are given then the
             *   string <b> "//"</b> is appended.  </p></li>
             *
             *   <li><p> If user information is given then it is appended, followed by
             *   a commercial-at character (<b> '@'</b>).  Any character not in the
             *   <i>unreserved</i>, <i>punct</i>, <i>escaped</i>, or <i>other</i>
             *   categories is <a href="">quoted</a>.  </p></li>
             *
             *   <li><p> If a host is given then it is appended.  If the host is a
             *   literal IPv6 address but is not enclosed in square brackets
             *   (<b> '['</b> and <b> ']'</b>) then the square brackets are added.
             *   </p></li>
             *
             *   <li><p> If a port number is given then a colon character
             *   (<b> ':'</b>) is appended, followed by the port number in decimal.
             *   </p></li>
             *
             *   <li><p> If a path is given then it is appended.  Any character not in
             *   the <i>unreserved</i>, <i>punct</i>, <i>escaped</i>, or <i>other</i>
             *   categories, and not equal to the slash character (<b> '/'</b>) or the
             *   commercial-at character (<b> '@'</b>), is quoted.  </p></li>
             *
             *   <li><p> If a query is given then a question-mark character
             *   (<b> '?'</b>) is appended, followed by the query.  Any character that
             *   is not a <a href="">legal URI character</a> is quoted.
             *   </p></li>
             *
             *   <li><p> Finally, if a fragment is given then a hash character
             *   (<b> '#'</b>) is appended, followed by the fragment.  Any character
             *   that is not a legal URI character is quoted.  </p></li>
             *
             * </ol>
             *
             * <p> The resulting URI string is then parsed as if by invoking the <b style="color:orange;"> 
             * URI(String)</b> constructor and then invoking the <b style="color:orange;">
             * parseServerAuthority()</b> method upon the result; this may cause a <b style="color:orange;">
             * URISyntaxException</b> to be thrown.  </p>
             *
             * @param   scheme    Scheme name
             * @param   userInfo  User name and authorization information
             * @param   host      Host name
             * @param   port      Port number
             * @param   path      Path
             * @param   query     Query
             * @param   fragment  Fragment
             *
             * @throws URISyntaxException
             *         If both a scheme and a path are given but the path is relative,
             *         if the URI string constructed from the given components violates
             *         RFC&nbsp;2396, or if the authority component of the string is
             *         present but cannot be parsed as a server-based authority
             */
            CORE_EXPLICIT URI(const String &scheme,
                              const String &userInfo, const String &host, gint port,
                              const String &path, const String &query, const String &fragment);

            /**
             * Constructs a hierarchical URI from the given components.
             *
             * <p> If a scheme is given then the path, if also given, must either be
             * empty or begin with a slash character (<b> '/'</b>).  Otherwise a
             * component of the new URI may be left undefined by passing <b> null</b>
             * for the corresponding parameter.
             *
             * <p> This constructor first builds a URI string from the given components
             * according to the rules specified in <a
             * href="http://www.ietf.org/rfc/rfc2396.txt">RFC&nbsp;2396</a>,
             * section&nbsp;5.2, step&nbsp;7: </p>
             *
             * <ol>
             *
             *   <li><p> Initially, the result string is empty.  </p></li>
             *
             *   <li><p> If a scheme is given then it is appended to the result,
             *   followed by a colon character (<b> ':'</b>).  </p></li>
             *
             *   <li><p> If an authority is given then the string <b> "//"</b> is
             *   appended, followed by the authority.  If the authority contains a
             *   literal IPv6 address then the address must be enclosed in square
             *   brackets (<b> '['</b> and <b> ']'</b>).  Any character not in the
             *   <i>unreserved</i>, <i>punct</i>, <i>escaped</i>, or <i>other</i>
             *   categories, and not equal to the commercial-at character
             *   (<b> '@'</b>), is <a href="">quoted</a>.  </p></li>
             *
             *   <li><p> If a path is given then it is appended.  Any character not in
             *   the <i>unreserved</i>, <i>punct</i>, <i>escaped</i>, or <i>other</i>
             *   categories, and not equal to the slash character (<b> '/'</b>) or the
             *   commercial-at character (<b> '@'</b>), is quoted.  </p></li>
             *
             *   <li><p> If a query is given then a question-mark character
             *   (<b> '?'</b>) is appended, followed by the query.  Any character that
             *   is not a <a href="">legal URI character</a> is quoted.
             *   </p></li>
             *
             *   <li><p> Finally, if a fragment is given then a hash character
             *   (<b> '#'</b>) is appended, followed by the fragment.  Any character
             *   that is not a legal URI character is quoted.  </p></li>
             *
             * </ol>
             *
             * <p> The resulting URI string is then parsed as if by invoking the <b style="color:orange;"> 
             * URI(String)</b> constructor and then invoking the <b style="color:orange;">
             * parseServerAuthority()</b> method upon the result; this may cause a <b style="color:orange;">
             * URISyntaxException</b> to be thrown.  </p>
             *
             * @param   scheme     Scheme name
             * @param   authority  Authority
             * @param   path       Path
             * @param   query      Query
             * @param   fragment   Fragment
             *
             * @throws URISyntaxException
             *         If both a scheme and a path are given but the path is relative,
             *         if the URI string constructed from the given components violates
             *         RFC&nbsp;2396, or if the authority component of the string is
             *         present but cannot be parsed as a server-based authority
             */
            CORE_EXPLICIT URI(const String &scheme,
                              const String &authority,
                              const String &path, const String &query, const String &fragment);

            /**
             * Constructs a hierarchical URI from the given components.
             *
             * <p> A component may be left undefined by passing <b> null</b>.
             *
             * <p> This convenience constructor works as if by invoking the
             * seven-argument constructor as follows:
             *
             * <blockquote>
             * <b> new</b> <b style="color:orange;"> URI</b><b> (scheme, "", host, -1, path, "", fragment);</b>
             * </blockquote>
             *
             * @param   scheme    Scheme name
             * @param   host      Host name
             * @param   path      Path
             * @param   fragment  Fragment
             *
             * @throws  URISyntaxException
             *          If the URI string constructed from the given components
             *          violates RFC&nbsp;2396
             */
            CORE_EXPLICIT URI(const String &scheme, const String &host, const String &path, const String &fragment);

            /**
             * Constructs a URI from the given components.
             *
             * <p> A component may be left undefined by passing <b> null</b>.
             *
             * <p> This constructor first builds a URI in string form using the given
             * components as follows:  </p>
             *
             * <ol>
             *
             *   <li><p> Initially, the result string is empty.  </p></li>
             *
             *   <li><p> If a scheme is given then it is appended to the result,
             *   followed by a colon character (<b> ':'</b>).  </p></li>
             *
             *   <li><p> If a scheme-specific part is given then it is appended.  Any
             *   character that is not a <a href="">legal URI character</a>
             *   is <a href="">quoted</a>.  </p></li>
             *
             *   <li><p> Finally, if a fragment is given then a hash character
             *   (<b> '#'</b>) is appended to the string, followed by the fragment.
             *   Any character that is not a legal URI character is quoted.  </p></li>
             *
             * </ol>
             *
             * <p> The resulting URI string is then parsed in order to create the new
             * URI instance as if by invoking the <b style="color:orange;"> URI(String)</b> constructor;
             * this may cause a <b style="color:orange;"> URISyntaxException</b> to be thrown.  </p>
             *
             * @param   scheme    Scheme name
             * @param   ssp       Scheme-specific part
             * @param   fragment  Fragment
             *
             * @throws  URISyntaxException
             *          If the URI string constructed from the given components
             *          violates RFC&nbsp;2396
             */
            CORE_EXPLICIT URI(const String &scheme, const String &ssp, const String &fragment);

        protected:
            /**
             * Constructs a simple URI consisting of only a scheme and a pre-validated
             * path. Provides a fast-path for some internal cases.
             */
            CORE_EXPLICIT URI(String scheme, String path);

            static gbool validateSchemeAndPath(const String &scheme, const String &path);

        public:
            /**
             * Creates a URI by parsing the given string.
             *
             * <p> This convenience factory method works as if by invoking the <b style="color:orange;"> 
             * URI(String)</b> constructor; any <b style="color:orange;"> URISyntaxException</b> thrown by the
             * constructor is caught and wrapped in a new <b style="color:orange;"> 
             * IllegalArgumentException</b> object, which is then thrown.
             *
             * <p> This method is provided for use in situations where it is known that
             * the given string is a legal URI, for example for URI constants declared
             * within a program, and so it would be considered a programming error
             * for the string not to parse as such.  The constructors, which throw
             * <b style="color:orange;"> URISyntaxException</b> directly, should be used in situations where a
             * URI is being constructed from user input or from some other source that
             * may be prone to errors.  </p>
             *
             * @param  str   The string to be parsed into a URI
             * @return The new URI
             *
             * @throws  NullPointerException
             *          If <b> str</b> is <b> null</b>
             *
             * @throws  IllegalArgumentException
             *          If the given string violates RFC&nbsp;2396
             */
            static URI create(const String &str);

            /**
             * Attempts to parse this URI's authority component, if defined, into
             * user-information, host, and port components.
             *
             * <p> If this URI's authority component has already been recognized as
             * being server-based then it will already have been parsed into
             * user-information, host, and port components.  In this case, or if this
             * URI has no authority component, this method simply returns this URI.
             *
             * <p> Otherwise this method attempts once more to parse the authority
             * component into user-information, host, and port components, and throws
             * an exception describing why the authority component could not be parsed
             * in that way.
             *
             * <p> This method is provided because the generic URI syntax specified in
             * <a href="http://www.ietf.org/rfc/rfc2396.txt">RFC&nbsp;2396</a>
             * cannot always distinguish a malformed server-based authority from a
             * legitimate registry-based authority.  It must therefore treat some
             * instances of the former as instances of the latter.  The authority
             * component in the URI string <b> "//foo:bar"</b>, for example, is not a
             * legal server-based authority but it is legal as a registry-based
             * authority.
             *
             * <p> In many common situations, for example when working URIs that are
             * known to be either URNs or URLs, the hierarchical URIs being used will
             * always be server-based.  They therefore must either be parsed as such or
             * treated as an error.  In these cases a statement such as
             *
             * <blockquote>
             * <b> URI </b><i>u</i><b>  = new URI(str).parseServerAuthority();</b>
             * </blockquote>
             *
             * <p> can be used to ensure that <i>u</i> always refers to a URI that, if
             * it has an authority component, has a server-based authority with proper
             * user-information, host, and port components.  Invoking this method also
             * ensures that if the authority could not be parsed in that way then an
             * appropriate diagnostic message can be issued based upon the exception
             * that is thrown. </p>
             *
             * @return  A URI whose authority field has been parsed
             *          as a server-based authority
             *
             * @throws  URISyntaxException
             *          If the authority component of this URI is defined
             *          but cannot be parsed as a server-based authority
             *          according to RFC&nbsp;2396
             */
            URI &parseServerAuthority();

            /**
             * Normalizes this URI's path.
             *
             * <p> If this URI is opaque, or if its path is already in normal form,
             * then this URI is returned.  Otherwise a new URI is constructed that is
             * identical to this URI except that its path is computed by normalizing
             * this URI's path in a manner consistent with <a
             * href="http://www.ietf.org/rfc/rfc2396.txt">RFC&nbsp;2396</a>,
             * section&nbsp;5.2, step&nbsp;6, sub-steps&nbsp;c through&nbsp;f; that is:
             * </p>
             *
             * <ol>
             *
             *   <li><p> All <b> "."</b> segments are removed. </p></li>
             *
             *   <li><p> If a <b> ".."</b> segment is preceded by a non-<b> ".."</b>
             *   segment then both of these segments are removed.  This step is
             *   repeated until it is no longer applicable. </p></li>
             *
             *   <li><p> If the path is relative, and if its first segment contains a
             *   colon character (<b> ':'</b>), then a <b> "."</b> segment is
             *   prepended.  This prevents a relative URI with a path such as
             *   <b> "a:b/c/d"</b> from later being re-parsed as an opaque URI with a
             *   scheme of <b> "a"</b> and a scheme-specific part of <b> "b/c/d"</b>.
             *   <b><i>(Deviation from RFC&nbsp;2396)</i></b> </p></li>
             *
             * </ol>
             *
             * <p> A normalized path will begin with one or more <b> ".."</b> segments
             * if there were insufficient non-<b> ".."</b> segments preceding them to
             * allow their removal.  A normalized path will begin with a <b> "."</b>
             * segment if one was inserted by step 3 above.  Otherwise, a normalized
             * path will not contain any <b> "."</b> or <b> ".."</b> segments. </p>
             *
             * @return  A URI equivalent to this URI,
             *          but whose path is in normal form
             */
            URI normalize() const;

            /**
             * Resolves the given URI against this URI.
             *
             * <p> If the given URI is already absolute, or if this URI is opaque, then
             * the given URI is returned.
             *
             * <p><a id="resolve-frag"></a> If the given URI's fragment component is
             * defined, its path component is empty, and its scheme, authority, and
             * query components are undefined, then a URI with the given fragment but
             * with all other components equal to those of this URI is returned.  This
             * allows a URI representing a standalone fragment reference, such as
             * <b> "#foo"</b>, to be usefully resolved against a base URI.
             *
             * <p> Otherwise this method constructs a new hierarchical URI in a manner
             * consistent with <a
             * href="http://www.ietf.org/rfc/rfc2396.txt">RFC&nbsp;2396</a>,
             * section&nbsp;5.2; that is: </p>
             *
             * <ol>
             *
             *   <li><p> A new URI is constructed with this URI's scheme and the given
             *   URI's query and fragment components. </p></li>
             *
             *   <li><p> If the given URI has an authority component then the new URI's
             *   authority and path are taken from the given URI. </p></li>
             *
             *   <li><p> Otherwise the new URI's authority component is copied from
             *   this URI, and its path is computed as follows: </p>
             *
             *   <ol>
             *
             *     <li><p> If the given URI's path is absolute then the new URI's path
             *     is taken from the given URI. </p></li>
             *
             *     <li><p> Otherwise the given URI's path is relative, and so the new
             *     URI's path is computed by resolving the path of the given URI
             *     against the path of this URI.  This is done by concatenating all but
             *     the last segment of this URI's path, if any, with the given URI's
             *     path and then normalizing the result as if by invoking the <b style="color:orange;"> 
             *     normalize</b> method. </p></li>
             *
             *   </ol></li>
             *
             * </ol>
             *
             * <p> The result of this method is absolute if, and only if, either this
             * URI is absolute or the given URI is absolute.  </p>
             *
             * @param  uri  The URI to be resolved against this URI
             * @return The resulting URI
             *
             * @throws  NullPointerException
             *          If <b> uri</b> is <b> null</b>
             */
            URI resolve(const URI &uri) const;

            /**
             * Constructs a new URI by parsing the given string and then resolving it
             * against this URI.
             *
             * <p> This convenience method works as if invoking it were equivalent to
             * evaluating the expression <b style="color:orange;">
             * resolve</b><b> (URI.</b><b style="color:orange;"> create</b><b> (str))</b>. </p>
             *
             * @param  str   The string to be parsed into a URI
             * @return The resulting URI
             *
             * @throws  IllegalArgumentException
             *          If the given string violates RFC&nbsp;2396
             */
            URI resolve(const String &str) const;

            /**
             * Relativizes the given URI against this URI.
             *
             * <p> The relativization of the given URI against this URI is computed as
             * follows: </p>
             *
             * <ol>
             *
             *   <li><p> If either this URI or the given URI are opaque, or if the
             *   scheme and authority components of the two URIs are not identical, or
             *   if the path of this URI is not a prefix of the path of the given URI,
             *   then the given URI is returned. </p></li>
             *
             *   <li><p> Otherwise a new relative hierarchical URI is constructed with
             *   query and fragment components taken from the given URI and with a path
             *   component computed by removing this URI's path from the beginning of
             *   the given URI's path. </p></li>
             *
             * </ol>
             *
             * @param  uri  The URI to be relativized against this URI
             * @return The resulting URI
             *
             * @throws  NullPointerException
             *          If <b> uri</b> is <b> null</b>
             */
            URI relativize(const URI &uri) const;

            /**
             * Constructs a URL from this URI.
             *
             * <p> This convenience method works as if invoking it were equivalent to
             * evaluating the expression <b> new URL(this.toString())</b> after
             * first checking that this URI is absolute. </p>
             *
             * @return  A URL constructed from this URI
             *
             * @throws  IllegalArgumentException
             *          If this URL is not absolute
             *
             * @throws  MalformedURLException
             *          If a protocol handler for the URL could not be found,
             *          or if some other error occurred while constructing the URL
             */
            URL toURL() const;

            /**
             * Returns the scheme component of this URI.
             *
             * <p> The scheme component of a URI, if defined, only contains characters
             * in the <i>alphanum</i> category and in the string <b> "-.+"</b>.  A
             * scheme always starts with an <i>alpha</i> character. <p>
             *
             * The scheme component of a URI cannot contain escaped octets, hence this
             * method does not perform any decoding.
             *
             * @return  The scheme component of this URI,
             *          or <b> null</b> if the scheme is undefined
             */
            String scheme() const;

            /**
             * Tells whether or not this URI is absolute.
             *
             * <p> A URI is absolute if, and only if, it has a scheme component. </p>
             *
             * @return  <b> true</b> if, and only if, this URI is absolute
             */
            gbool isAbsolute() const;

            /**
             * Tells whether or not this URI is opaque.
             *
             * <p> A URI is opaque if, and only if, it is absolute and its
             * scheme-specific part does not begin with a slash character ('/').
             * An opaque URI has a scheme, a scheme-specific part, and possibly
             * a fragment; all other components are undefined. </p>
             *
             * @return  <b> true</b> if, and only if, this URI is opaque
             */
            gbool isOpaque() const;

            /**
             * Returns the raw scheme-specific part of this URI.  The scheme-specific
             * part is never undefined, though it may be empty.
             *
             * <p> The scheme-specific part of a URI only contains legal URI
             * characters. </p>
             *
             * @return  The raw scheme-specific part of this URI
             *          (never <b> null</b>)
             */
            String rawSchemeSpecificPart() const;

            /**
             * Returns the decoded scheme-specific part of this URI.
             *
             * <p> The string returned by this method is equal to that returned by the
             * <b style="color:orange;"> rawSchemeSpecificPart</b> method
             * except that all sequences of escaped octets are <a
             * href="">decoded</a>.  </p>
             *
             * @return  The decoded scheme-specific part of this URI
             */
            String schemeSpecificPart() const;

            /**
             * Returns the raw authority component of this URI.
             *
             * <p> The authority component of a URI, if defined, only contains the
             * commercial-at character (<b> '@'</b>) and characters in the
             * <i>unreserved</i>, <i>punct</i>, <i>escaped</i>, and <i>other</i>
             * categories.  If the authority is server-based then it is further
             * constrained to have valid user-information, host, and port
             * components. </p>
             *
             * @return  The raw authority component of this URI,
             *          or <b> null</b> if the authority is undefined
             */
            String rawAuthority() const;

            /**
             * Returns the decoded authority component of this URI.
             *
             * <p> The string returned by this method is equal to that returned by the
             * <b style="color:orange;"> rawAuthority</b> method except that all
             * sequences of escaped octets are <a href="">decoded</a>.  </p>
             *
             * @return  The decoded authority component of this URI,
             *          or <b> "" </b> if the authority is undefined
             */
            String authority() const;

            /**
             * Returns the raw user-information component of this URI.
             *
             * <p> The user-information component of a URI, if defined, only contains
             * characters in the <i>unreserved</i>, <i>punct</i>, <i>escaped</i>, and
             * <i>other</i> categories. </p>
             *
             * @return  The raw user-information component of this URI,
             *          or <b> null</b> if the user information is undefined
             */
            String rawUserInfo() const;

            /**
             * Returns the decoded user-information component of this URI.
             *
             * <p> The string returned by this method is equal to that returned by the
             * <b style="color:orange;"> rawUserInfo</b> method except that all
             * sequences of escaped octets are <a href="">decoded</a>.  </p>
             *
             * @return  The decoded user-information component of this URI,
             *          or <b> "" </b> if the user information is undefined
             */
            String userInfo() const;

            /**
             * Returns the host component of this URI.
             *
             * <p> The host component of a URI, if defined, will have one of the
             * following forms: </p>
             *
             * <ul>
             *
             *   <li><p> A domain name consisting of one or more <i>labels</i>
             *   separated by period characters (<b> '.'</b>), optionally followed by
             *   a period character.  Each label consists of <i>alphanum</i> characters
             *   as well as hyphen characters (<b> '-'</b>), though hyphens never
             *   occur as the first or last characters in a label. The rightmost
             *   label of a domain name consisting of two or more labels, begins
             *   with an <i>alpha</i> character. </li>
             *
             *   <li><p> A dotted-quad IPv4 address of the form
             *   <i>digit</i><b> +.</b><i>digit</i><b> +.</b><i>digit</i><b> +.</b><i>digit</i><b> +</b>,
             *   where no <i>digit</i> sequence is longer than three characters and no
             *   sequence has a value larger than 255. </p></li>
             *
             *   <li><p> An IPv6 address enclosed in square brackets (<b> '['</b> and
             *   <b> ']'</b>) and consisting of hexadecimal digits, colon characters
             *   (<b> ':'</b>), and possibly an embedded IPv4 address.  The full
             *   syntax of IPv6 addresses is specified in <a
             *   href="http://www.ietf.org/rfc/rfc2373.txt"><i>RFC&nbsp;2373: IPv6
             *   Addressing Architecture</i></a>.  </p></li>
             *
             * </ul>
             *
             * The host component of a URI cannot contain escaped octets, hence this
             * method does not perform any decoding.
             *
             * @return  The host component of this URI,
             *          or <b> null</b> if the host is undefined
             */
            String host() const;

            /**
             * Returns the port number of this URI.
             *
             * <p> The port component of a URI, if defined, is a non-negative
             * integer. </p>
             *
             * @return  The port component of this URI,
             *          or <b> -1</b> if the port is undefined
             */
            gint port() const;

            /**
             * Returns the raw path component of this URI.
             *
             * <p> The path component of a URI, if defined, only contains the slash
             * character (<b> '/'</b>), the commercial-at character (<b> '@'</b>),
             * and characters in the <i>unreserved</i>, <i>punct</i>, <i>escaped</i>,
             * and <i>other</i> categories. </p>
             *
             * @return  The path component of this URI,
             *          or <b> null</b> if the path is undefined
             */
            String rawPath() const;

            /**
             * Returns the decoded path component of this URI.
             *
             * <p> The string returned by this method is equal to that returned by the
             * <b style="color:orange;"> rawPath</b> method except that all sequences of
             * escaped octets are <a href="">decoded</a>.  </p>
             *
             * @return  The decoded path component of this URI,
             *          or <b> null</b> if the path is undefined
             */
            String path() const;

            /**
             * Returns the raw query component of this URI.
             *
             * <p> The query component of a URI, if defined, only contains legal URI
             * characters. </p>
             *
             * @return  The raw query component of this URI,
             *          or <b> null</b> if the query is undefined
             */
            String rawQuery() const;

            /**
             * Returns the decoded query component of this URI.
             *
             * <p> The string returned by this method is equal to that returned by the
             * <b style="color:orange;"> rawQuery</b> method except that all sequences of
             * escaped octets are <a href="">decoded</a>.  </p>
             *
             * @return  The decoded query component of this URI,
             *          or <b> "" </b> if the query is undefined
             */
            String query() const;

            /**
             * Returns the raw fragment component of this URI.
             *
             * <p> The fragment component of a URI, if defined, only contains legal URI
             * characters. </p>
             *
             * @return  The raw fragment component of this URI,
             *          or <b> null</b> if the fragment is undefined
             */
            String rawFragment() const;

            /**
             * Returns the decoded fragment component of this URI.
             *
             * <p> The string returned by this method is equal to that returned by the
             * <b style="color:orange;"> rawFragment</b> method except that all
             * sequences of escaped octets are <a href="">decoded</a>.  </p>
             *
             * @return  The decoded fragment component of this URI,
             *          or <b> ""</b> if the fragment is undefined
             */
            String fragment() const;

            /**
             * Tests this URI for equality with another object.
             *
             * <p> If the given object is not a URI then this method immediately
             * returns <b> false</b>.
             *
             * <p> For two URIs to be considered equal requires that either both are
             * opaque or both are hierarchical.  Their schemes must either both be
             * undefined or else be equal without regard to case. Their fragments
             * must either both be undefined or else be equal.
             *
             * <p> For two opaque URIs to be considered equal, their scheme-specific
             * parts must be equal.
             *
             * <p> For two hierarchical URIs to be considered equal, their paths must
             * be equal and their queries must either both be undefined or else be
             * equal.  Their authorities must either both be undefined, or both be
             * registry-based, or both be server-based.  If their authorities are
             * defined and are registry-based, then they must be equal.  If their
             * authorities are defined and are server-based, then their hosts must be
             * equal without regard to case, their port numbers must be equal, and
             * their user-information components must be equal.
             *
             * <p> When testing the user-information, path, query, fragment, authority,
             * or scheme-specific parts of two URIs for equality, the raw forms rather
             * than the encoded forms of these components are compared and the
             * hexadecimal digits of escaped octets are compared without regard to
             * case.
             *
             * <p> This method satisfies the general contract of the <b style="color:orange;"> 
             * Object.equals</b> method. </p>
             *
             * @param   obj   The object to which this object is to be compared
             *
             * @return  <b> true</b> if, and only if, the given object is a URI that
             *          is identical to this URI
             */
            gbool equals(const Object &obj) const override;

            /**
             * Returns a hash-code value for this URI.  The hash code is based upon all
             * of the URI's components, and satisfies the general contract of the
             * <b style="color:orange;"> Object.hash </b> method.
             *
             * @return  A hash-code value for this URI
             */
            gint hash() const override;

            /**
             * Compares this URI to another object, which must be a URI.
             *
             * <p> When comparing corresponding components of two URIs, if one
             * component is undefined but the other is defined then the first is
             * considered to be less than the second.  Unless otherwise noted, string
             * components are ordered according to their natural, case-sensitive
             * ordering as defined by the <b style="color:orange;">
             * String.compareTo</b> method.  String components that are subject to
             * encoding are compared by comparing their raw forms rather than their
             * encoded forms and the hexadecimal digits of escaped octets are compared
             * without regard to case.
             *
             * <p> The ordering of URIs is defined as follows: </p>
             *
             * <ul>
             *
             *   <li><p> Two URIs with different schemes are ordered according the
             *   ordering of their schemes, without regard to case. </p></li>
             *
             *   <li><p> A hierarchical URI is considered to be less than an opaque URI
             *   with an identical scheme. </p></li>
             *
             *   <li><p> Two opaque URIs with identical schemes are ordered according
             *   to the ordering of their scheme-specific parts. </p></li>
             *
             *   <li><p> Two opaque URIs with identical schemes and scheme-specific
             *   parts are ordered according to the ordering of their
             *   fragments. </p></li>
             *
             *   <li><p> Two hierarchical URIs with identical schemes are ordered
             *   according to the ordering of their authority components: </p>
             *
             *   <ul>
             *
             *     <li><p> If both authority components are server-based then the URIs
             *     are ordered according to their user-information components; if these
             *     components are identical then the URIs are ordered according to the
             *     ordering of their hosts, without regard to case; if the hosts are
             *     identical then the URIs are ordered according to the ordering of
             *     their ports. </p></li>
             *
             *     <li><p> If one or both authority components are registry-based then
             *     the URIs are ordered according to the ordering of their authority
             *     components. </p></li>
             *
             *   </ul></li>
             *
             *   <li><p> Finally, two hierarchical URIs with identical schemes and
             *   authority components are ordered according to the ordering of their
             *   paths; if their paths are identical then they are ordered according to
             *   the ordering of their queries; if the queries are identical then they
             *   are ordered according to the order of their fragments. </p></li>
             *
             * </ul>
             *
             * <p> This method satisfies the general contract of the <b style="color:orange;"> 
             * Comparable.compareTo</b>
             * method. </p>
             *
             * @param   other
             *          The object to which this URI is to be compared
             *
             * @return  A negative integer, zero, or a positive integer as this URI is
             *          less than, equal to, or greater than the given URI
             *
             * @throws  ClassCastException
             *          If the given object is not a URI
             */
            gint compareTo(const URI &other) const override;

            /**
             * Returns the content of this URI as a string.
             *
             * <p> If this URI was created by invoking one of the constructors in this
             * class then a string equivalent to the original input string, or to the
             * string computed from the originally-given components, as appropriate, is
             * returned.  Otherwise this URI was created by normalization, resolution,
             * or relativization, and so a string is constructed from this URI's
             * components according to the rules specified in <a
             * href="http://www.ietf.org/rfc/rfc2396.txt">RFC&nbsp;2396</a>,
             * section&nbsp;5.2, step&nbsp;7. </p>
             *
             * @return  The string form of this URI
             */
            String toString() const override;

        private:
            String defineString() const;

        public:
            /**
             * Returns the content of this URI as a US-ASCII string.
             *
             * <p> If this URI does not contain any characters in the <i>other</i>
             * category then an invocation of this method will return the same value as
             * an invocation of the <b style="color:orange;"> toString</b> method.  Otherwise
             * this method works as if by invoking that method and then <a
             * href="#encode">encoding</a> the result.  </p>
             *
             * @return  The string form of this URI, encoded as needed
             *          so that it only contains characters in the US-ASCII
             *          charset
             */
            String toASCIIString() const;


        private:

            // These methods return appropriate values for null string arguments,
            // thereby simplifying the equals, hashCode, and compareTo methods.
            //
            // The case-ignoring methods should only be applied to strings whose
            // characters are all known to be US-ASCII.  Because of this restriction,
            // these methods are faster than the similar methods in the String class.

            // US-ASCII only
            static gint toLower(gchar c);

            // US-ASCII only
            static gint toUpper(gchar c);

            static gbool equal(const String &s, const String &t);

            // US-ASCII only
            static gbool equalIgnoringCase(const String &s, const String &t);

            static gint hash(gint hash, const String &s);

            static gint normalizedHash(gint hash, const String &s);

            // US-ASCII only
            static gint hashIgnoringCase(gint hash, const String &s);

            static gint compare(const String &s, const String &t);

            // The percentNormalizedComparison method does not verify two
            // characters that follow the % sign are hexadecimal digits.
            // Reason being:
            // 1) percentNormalizedComparison method is not called with
            // 'decoded' strings
            // 2) The only place where a percent can be followed by anything
            // other than hexadecimal digits is in the authority component
            // (for a IPv6 scope) and the whole authority component is case-
            // insensitive.
            static gint percentNormalizedComparison(const String &s, const String &t, gbool testForEquality);

            // US-ASCII only
            static gint compareIgnoringCase(const String &s, const String &t);


            // -- String construction --

            // If a scheme is given then the path, if given, must be absolute
            //
            static void checkPath(const String &s, const String &scheme, const String &path);

            static void
            appendAuthority(StringBuffer &sb, const String &authority, const String &userInfo, const String &host,
                            gint port);

            static void appendSchemeSpecificPart(StringBuffer &sb, const String &opaquePart, const String &authority,
                                                 const String &userInfo, const String &host, gint port,
                                                 const String &path,
                                                 const String &query);

            static void appendFragment(StringBuffer &sb, const String &fragment);

            static String toString(const String &scheme, const String &opaquePart,
                                   const String &authority, const String &userInfo,
                                   const String &host, gint port, const String &path,
                                   const String &query, const String &fragment);

            // -- Normalization, resolution, and relativization --

            // RFC2396 5.2 (6)
            static String resolvePath(const String &base, const String &child, gbool absolute);

            // RFC2396 5.2
            static URI resolve(const URI &base, const URI &child);

            // If the given URI's path is normal, then return the URI;
            // o.w., return a new URI containing the normalized path.
            //
            static URI normalize(const URI &u);

            // If both URIs are hierarchical, their scheme and authority components are
            // identical, and the base path is a prefix of the child's path, then
            // return a relative URI that, when resolved against the base, yields the
            // child; otherwise, return the child.
            //
            static URI relativize(const URI &base, URI child);

            // -- Path normalization --

            // The following algorithm for path normalization avoids the creation of a
            // string object for each segment, as well as the use of a string buffer to
            // compute the final result, by using a single gchar array and editing it in
            // place.  The array is first split into segments, replacing each slash
            // with '\0' and creating a segment-index array, each element of which is
            // the index of the first gchar in the corresponding segment.  We then walk
            // through both arrays, removing ".", "..", and other segments as necessary
            // by setting their entries in the index array to -1.  Finally, the two
            // arrays are used to rejoin the segments and compute the final result.
            //
            // This code is based upon src/solaris/private/java/io/canonicalize_md.c

            // Check the given path to see if it might need normalization.
            // A path might need normalization if it contains duplicate slashes, a "."
            // Segment, or a ".."
            // Segment.
            // Return -1 if no further normalization is
            // possible, otherwise return the number of segments found.
            //
            // This method takes a string argument rather than a gchar array so that
            // this test can be performed without invoking path.toCharArray().
            //
            static gint needsNormalization(const String &path);


            // Split the given path into segments, replacing slashes with nulls and
            // filling in the given segment-index array.
            //
            // Preconditions:
            //   segs.length == Number of segments in path
            //
            // Postconditions:
            //   All slashes in path replaced by '\0'
            //   segs[i] == Index of first gchar in segment i (0 <= i < segs.length)
            //
            static void split(CharArray &path, IntArray &segs);


            // Join the segments in the given path according to the given segment-index
            // array, ignoring those segments whose index entries have been set to -1,
            // and inserting slashes as needed.  Return the length of the resulting
            // path.
            //
            // Preconditions:
            //   segs[i] == -1 implies segment i is to be ignored
            //   path computed by split, as above, with '\0' having replaced '/'
            //
            // Postconditions:
            //   path[0] .. path[return value] == Resulting path
            //
            static gint join(CharArray &path, IntArray &segs);

            // Remove "." segments from the given path, and remove segment pairs
            // consisting of a non-".." segment followed by a ".." segment.
            //
            static void removeDots(CharArray &path, IntArray &segs);


            // DEVIATION: If the normalized path is relative, and if the first
            // segment could be parsed as a scheme name, then prepend a "." segment
            //
            static void maybeAddLeadingDot(CharArray &path, IntArray &segs);


            // Normalize the given path string.  A normal path string has no empty
            // segments (i.e., occurrences of "//"), no segments equal to ".", and no
            // segments equal to ".." that are preceded by a segment not equal to "..".
            // In contrast to Unix-style pathname normalization, for URI paths we
            // always retain trailing slashes.
            //
            static String normalize(const String &ps);

            // -- Character classes for parsing --

            // RFC2396 precisely specifies which characters in the US-ASCII charset are
            // permissible in the various components of a URI reference.  We here
            // define a set of mask pairs to aid in enforcing these restrictions.  Each
            // mask pair consists of two longs, a low mask and a high mask.  Taken
            // together they represent a 128-bit mask, where bit i is set iff the
            // character with value i is permitted.
            //
            // This approach is more efficient than sequentially searching arrays of
            // permitted characters.  It could be made still more efficient by
            // precompiling the mask information so that a character's presence in a
            // given mask could be determined by a single table lookup.

            // To save startup time, we manually calculate the low-/highMask constants.
            // For reference, the following methods were used to calculate the values:

            // Compute the low-order mask for the characters in the given string
            //     static long lowMask(const String &chars) {
            //        gint n = chars.length();
            //        long m = 0;
            //        for (gint i = 0; i < n; i++) {
            //            gchar c = chars.charAt(i);
            //            if (c < 64)
            //                m |= (1L << c);
            //        }
            //        return m;
            //    }

            // Compute the high-order mask for the characters in the given string
            //    static long highMask(const String &chars) {
            //        gint n = chars.length();
            //        long m = 0;
            //        for (gint i = 0; i < n; i++) {
            //            gchar c = chars.charAt(i);
            //            if ((c >= 64) && (c < 128))
            //                m |= (1L << (c - 64));
            //        }
            //        return m;
            //    }

            // Compute a low-order mask for the characters
            // between first and last, inclusive
            //    static long lowMask(gchar first, gchar last) {
            //        long m = 0;
            //        gint f = Math.max(Math.min(first, 63), 0);
            //        gint l = Math.max(Math.min(last, 63), 0);
            //        for (gint i = f; i <= l; i++)
            //            m |= 1L << i;
            //        return m;
            //    }

            // Compute a high-order mask for the characters
            // between first and last, inclusive
            //    static long highMask(gchar first, gchar last) {
            //        long m = 0;
            //        gint f = Math.max(Math.min(first, 127), 64) - 64;
            //        gint l = Math.max(Math.min(last, 127), 64) - 64;
            //        for (gint i = f; i <= l; i++)
            //            m |= 1L << i;
            //        return m;
            //    }

            // Tell whether the given character is permitted by the given mask pair
            static gbool match(gchar c, glong lowMask, glong highMask);

            // Character-class masks, in reverse order from RFC2396 because
            // initializers for static fields cannot make forward references.

            // digit    = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" |
            //            "8" | "9"
            static CORE_FAST glong L_DIGIT = 0x3FF000000000000L; // lowMask('0', '9');
            static CORE_FAST glong H_DIGIT = 0L;

            // up-alpha  = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" |
            //            "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" |
            //            "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
            static CORE_FAST glong L_UPALPHA = 0L;
            static CORE_FAST glong H_UPALPHA = 0x7FFFFFEL; // highMask('A', 'Z');

            // low-alpha = "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" |
            //            "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" |
            //            "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
            static CORE_FAST glong L_LOWALPHA = 0L;
            static CORE_FAST glong H_LOWALPHA = 0x7FFFFFE00000000L; // highMask('a', 'z');

            // alpha         = low-alpha | up-alpha
            static CORE_FAST glong L_ALPHA = L_LOWALPHA | L_UPALPHA;
            static CORE_FAST glong H_ALPHA = H_LOWALPHA | H_UPALPHA;

            // alphanum      = alpha | digit
            static CORE_FAST glong L_ALPHANUM = L_DIGIT | L_ALPHA;
            static CORE_FAST glong H_ALPHANUM = H_DIGIT | H_ALPHA;

            // hex           = digit | "A" | "B" | "C" | "D" | "E" | "F" |
            //                         "a" | "b" | "c" | "d" | "e" | "f"
            static CORE_FAST glong L_HEX = L_DIGIT;
            static CORE_FAST glong H_HEX = 0x7E0000007EL; // highMask('A', 'F') | highMask('a', 'f');

            // mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" |
            //                 "(" | ")"
            static CORE_FAST glong L_MARK = 0x678200000000L; // lowMask("-_.!~*'()");
            static CORE_FAST glong H_MARK = 0x4000000080000000L; // highMask("-_.!~*'()");

            // unreserved    = alphanum | mark
            static CORE_FAST glong L_UNRESERVED = L_ALPHANUM | L_MARK;
            static CORE_FAST glong H_UNRESERVED = H_ALPHANUM | H_MARK;

            // reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
            //                 "$" | "," | "[" | "]"
            // Added per RFC2732: "[", "]"
            static CORE_FAST glong L_RESERVED = (glong) 0xAC00985000000000L; // lowMask(";/?:@&=+$,[]");
            static CORE_FAST glong H_RESERVED = 0x28000001L; // highMask(";/?:@&=+$,[]");

            // The zero'th bit is used to indicate that escape pairs and non-US-ASCII
            // characters are allowed; this is handled by the scanEscape method below.
            static CORE_FAST glong L_ESCAPED = 1L;
            static CORE_FAST glong H_ESCAPED = 0L;

            // uric = reserved | unreserved | escaped
            static CORE_FAST glong L_URIC = L_RESERVED | L_UNRESERVED | L_ESCAPED;
            static CORE_FAST glong H_URIC = H_RESERVED | H_UNRESERVED | H_ESCAPED;

            // pchar = unreserved | escaped |
            //                 ":" | "@" | "&" | "=" | "+" | "$" | ","
            static CORE_FAST glong L_PCHAR
                    = L_UNRESERVED | L_ESCAPED | 0x2400185000000000L; // lowMask(":@&=+$,");
            static CORE_FAST glong H_PCHAR
                    = H_UNRESERVED | H_ESCAPED | 0x1L; // highMask(":@&=+$,");

            // All valid path characters
            static CORE_FAST glong L_PATH = L_PCHAR | 0x800800000000000L; // lowMask(";/");
            static CORE_FAST glong H_PATH = H_PCHAR; // highMask(";/") == 0x0L;

            // Dash, for use in domainlabel and toplabel
            static CORE_FAST glong L_DASH = 0x200000000000L; // lowMask("-");
            static CORE_FAST glong H_DASH = 0x0L; // highMask("-");

            // Dot, for use in hostnames
            static CORE_FAST glong L_DOT = 0x400000000000L; // lowMask(".");
            static CORE_FAST glong H_DOT = 0x0L; // highMask(".");

            // userinfo      = *( unreserved | escaped |
            //                    ";" | ":" | "&" | "=" | "+" | "$" | "," )
            static CORE_FAST glong L_USERINFO
                    = L_UNRESERVED | L_ESCAPED | 0x2C00185000000000L; // lowMask(";:&=+$,");
            static CORE_FAST glong H_USERINFO
                    = H_UNRESERVED | H_ESCAPED; // | highMask(";:&=+$,") == 0L;

            // reg_name      = 1*( unreserved | escaped | "$" | "," |
            //                     ";" | ":" | "@" | "&" | "=" | "+" )
            static CORE_FAST glong L_REG_NAME
                    = L_UNRESERVED | L_ESCAPED | 0x2C00185000000000L; // lowMask("$,;:@&=+");
            static CORE_FAST glong H_REG_NAME
                    = H_UNRESERVED | H_ESCAPED | 0x1L; // highMask("$,;:@&=+");

            // All valid characters for server-based authorities
            static CORE_FAST glong L_SERVER
                    = L_USERINFO | L_ALPHANUM | L_DASH | 0x400400000000000L; // lowMask(".:@[]");
            static CORE_FAST glong H_SERVER
                    = H_USERINFO | H_ALPHANUM | H_DASH | 0x28000001L; // highMask(".:@[]");

            // Special case of server authority that represents an IPv6 address
            // In this case, a % does not signify an escape sequence
            static CORE_FAST glong L_SERVER_PERCENT
                    = L_SERVER | 0x2000000000L; // lowMask("%");
            static CORE_FAST glong H_SERVER_PERCENT
                    = H_SERVER; // | highMask("%") == 0L;

            // scheme        = alpha *( alpha | digit | "+" | "-" | "." )
            static CORE_FAST glong L_SCHEME = L_ALPHA | L_DIGIT | 0x680000000000L; // lowMask("+-.");
            static CORE_FAST glong H_SCHEME = H_ALPHA | H_DIGIT; // | highMask("+-.") == 0L

            // scope_id = alpha | digit | "_" | "."
            static CORE_FAST glong L_SCOPE_ID
                    = L_ALPHANUM | 0x400000000000L; // lowMask("_.");
            static CORE_FAST glong H_SCOPE_ID
                    = H_ALPHANUM | 0x80000000L; // highMask("_.");

            // -- Escaping and encoding --

            static void appendEscape(StringBuffer &sb, gbyte b);

            static void appendEncoded(charset::CharsetEncoder &encoder, StringBuffer &sb, gchar c);

            // Quote any characters in s that are not permitted
            // by the given mask pair
            //
            static String quote(const String &s, glong lowMask, glong highMask);

            // Encodes all characters >= \u0080 into escaped, normalized UTF-8 octets,
            // assuming that s is otherwise legal
            //
            static String encode(const String &s);

            static gint decode(gchar c);

            static gbyte decode(gchar c1, gchar c2);

            // Evaluates all escapes in s, applying UTF-8 decoding if needed.  Assumes
            // that escapes are well-formed syntactically, i.e., of the form %XX.  If a
            // sequence of escaped octets is not valid UTF-8 then the erroneous octets
            // are replaced with '\uFFFD'.
            // Exception: any "%" found between "[]" is left alone. It is an IPv6 literal
            //            with a scope_id
            //
            static String decode(const String &s);

            // This method was introduced as a generalization of URI.decode method
            // to provide a fix for JDK-8037396
            static String decode(const String &s, gbool ignorePercentInBrackets);


            // -- Parsing --

            // For convenience, we wrap the input URI string in a new instance of the
            // following internal class.  This saves always having to pass the input
            // string as an argument to each internal scan/parse method.
            class Parser CORE_FINAL : public Object {
            public:
                URI &root;

                String input;

                gbool requireServerAuthority = false;

                CORE_EXPLICIT Parser(URI &root, String s);

            private:
                // Tells whether start < end and, if so, whether charAt(start) == c
                //
                gbool at(gint start, gint end, gchar c) const;

                // Tells whether start + s.length() < end and, if so,
                // whether the chars at the start position match s exactly
                //
                gbool at(gint start, gint end, const String &s) const;

                // The various scan and parse methods that follow use a uniform
                // convention of taking the current start position and end index as
                // their first two arguments.  The start is inclusive while the end is
                // exclusive, just as in the String class, i.e., a start/end pair
                // denotes the left-open interval [start, end) of the input string.
                //
                // These methods never proceed past the end position.  They may return
                // -1 to indicate outright failure, but more often they simply return
                // the position of the first gchar after the last gchar scanned.  Thus
                // a typical idiom is
                //
                //     gint p = start;
                //     gint q = scan(p, end, ...);
                //     if (q > p)
                //         // We scanned something
                //         ...;
                //     else if (q == p)
                //         // We scanned nothing
                //         ...;
                //     else if (q == -1)
                //         // Something went wrong
                //         ...;


                // Scan a specific gchar: If the gchar at the given start position is
                // equal to c, return the index of the next gchar; otherwise, return the
                // start position.
                //
                gint scan(gint start, gint end, gchar c) const;

                // Scan forward from the given start position.  Stop at the first gchar
                // in the err string (in which case -1 is returned), or the first gchar
                // in the stop string (in which case the index of the preceding gchar is
                // returned), or the end of the input string (in which case the length
                // of the input string is returned).  May return the start position if
                // nothing matches.
                //
                gint scan(gint start, gint end, const String &err, const String &stop) const;

                // Scan forward from the given start position.  Stop at the first gchar
                // in the stop string (in which case the index of the preceding gchar is
                // returned), or the end of the input string (in which case the length
                // of the input string is returned).  May return the start position if
                // nothing matches.
                //
                gint scan(gint start, gint end, const String &stop) const;

                // Scan a potential escape sequence, starting at the given position,
                // with the given first gchar (i.e., charAt(start) == c).
                //
                // This method assumes that if escapes are allowed then visible
                // non-US-ASCII chars are also allowed.
                //
                gint scanEscape(gint start, gint n, gchar first) const;

                // Scan chars that match the given mask pair
                //
                gint scan(gint start, gint n, glong lowMask, glong highMask) const;

                // Check that each of the chars in [start, end) matches the given mask
                //
                void checkChars(gint start, gint end,
                                glong lowMask, glong highMask,
                                const String &what);

                // Check that the gchar at position p matches the given mask
                //
                void checkChar(gint p,
                               glong lowMask, glong highMask,
                               const String &what);


                // -- Parsing --

                // [<scheme>:]<scheme-specific-part>[#<fragment>]
                //
                void parse(gbool rsa);

                // [//authority]<path>[?<query>]
                //
                // DEVIATION from RFC2396: We allow an empty authority component as
                // long as it's followed by a non-empty path, query component, or
                // fragment component.  This is so that URIs such as "file:///foo/bar"
                // will parse.  This seems to be the intent of RFC2396, though the
                // grammar does not permit it.  If the authority is empty then the
                // userInfo, host, and port components are undefined.
                //
                // DEVIATION from RFC2396: We allow empty relative paths.  This seems
                // to be the intent of RFC2396, but the grammar does not permit it.
                // The primary consequence of this deviation is that "#f" parses as a
                // relative URI with an empty path.
                //
                gint parseHierarchical(gint start, gint n);

                // authority = server | reg_name
                //
                // Ambiguity: An authority that is a registry name rather than a server
                // might have a prefix that parses as a server.  We use the fact that
                // the authority component is always followed by '/' or the end of the
                // input string to resolve this: If the complete authority did not
                // parse as a server then we try to parse it as a registry name.
                //
                gint parseAuthority(gint start, gint n);


                // [<userinfo>@]<host>[:<port>]
                //
                gint parseServer(gint start, gint n);

                // Scan a string of decimal digits whose value fits in a gbyte
                //
                gint scanByte(gint start, gint n);

                // Scan an IPv4 address.
                //
                // If the strict argument is true then we require that the given
                // interval contain nothing besides an IPv4 address; if it is false
                // then we only require that it start with an IPv4 address.
                //
                // If the interval does not contain or start with (depending upon the
                // strict argument) a legal IPv4 address characters then we return -1
                // immediately; otherwise we insist that these characters parse as a
                // legal IPv4 address and throw an exception on failure.
                //
                // We assume that any string of decimal digits and dots must be an IPv4
                // address.  It won't parse as a hostname anyway, so making that
                // assumption here allows more meaningful exceptions to be thrown.
                //
                gint scanIPv4Address(gint start, gint n, gbool strict);

                // Take an IPv4 address: Throw an exception if the given interval
                // contains anything except an IPv4 address
                //
                gint takeIPv4Address(gint start, gint n, const String &expected);

                // Attempt to parse an IPv4 address, returning -1 on failure but
                // allowing the given interval to contain [:<characters>] after
                // the IPv4 address.
                //
                gint parseIPv4Address(gint start, gint n);

                // hostname      = domainlabel [ "." ] | 1*( domainlabel "." ) toplabel [ "." ]
                // domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum
                // toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
                //
                gint parseHostname(gint start, gint n);


                // IPv6 address parsing, from RFC2373: IPv6 Addressing Architecture
                //
                // Bug: The grammar in RFC2373 Appendix B does not allow addresses of
                // the form ::12.34.56.78, which are clearly shown in the examples
                // earlier in the document.  Here is the original grammar:
                //
                //   IPv6address = hexpart [ ":" IPv4address ]
                //   hexpart     = hexseq | hexseq "::" [ hexseq ] | "::" [ hexseq ]
                //   hexseq      = hex4 *( ":" hex4)
                //   hex4        = 1*4HEXDIG
                //
                // We therefore use the following revised grammar:
                //
                //   IPv6address = hexseq [ ":" IPv4address ]
                //                 | hexseq [ "::" [ hexpost ] ]
                //                 | "::" [ hexpost ]
                //   hexpost     = hexseq | hexseq ":" IPv4address | IPv4address
                //   hexseq      = hex4 *( ":" hex4)
                //   hex4        = 1*4HEXDIG
                //
                // This covers all and only the following cases:
                //
                //   hexseq
                //   hexseq : IPv4address
                //   hexseq ::
                //   hexseq :: hexseq
                //   hexseq :: hexseq : IPv4address
                //   hexseq :: IPv4address
                //   :: hexseq
                //   :: hexseq : IPv4address
                //   :: IPv4address
                //   ::
                //
                // Additionally we constrain the IPv6 address as follows :-
                //
                //  i.  IPv6 addresses without compressed zeros should contain
                //      exactly 16 bytes.
                //
                //  ii. IPv6 addresses with compressed zeros should contain
                //      less than 16 bytes.

                gint ipv6byteCount = 0;

                gint parseIPv6Reference(gint start, gint n);

                gint scanHexPost(gint start, gint n);

                // Scan a hex sequence; return -1 if one could not be scanned
                //
                gint scanHexSeq(gint start, gint n);

                CORE_FRATERNITY(URI);
            }; // Parser


        public:
            Object &clone() const override;

        }; // URI

    } // core
} // net

#endif //CORE23_URI_H
