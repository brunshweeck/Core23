//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "URI.h"
#include <core/IllegalArgumentException.h>
#include <core/StringBuffer.h>
#include <core/Character.h>
#include <core/private/Unsafe.h>
#include <core/net/URISyntaxException.h>
#include <core/net/URL.h>
#include <core/Math.h>
#include <core/native/CharArray.h>
#include <core/InternalError.h>

#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CharacterCodingException.h>
#include <core/NumberFormatException.h>
#include <core/util/Optional.h>
#include <core/charset/private/UTF8.h>

namespace core {
    namespace net {
        using namespace native;
        using namespace io;
        using namespace charset;
        using namespace util;

        namespace {
        }

        URI::Parser::Parser(URI &root, String s) : root(root), input(Unsafe::moveInstance(s)) {}

        gbool URI::Parser::at(gint start, gint end, gchar c) const {
            return (start < end) && (input.charAt(start) == c);
        }

        gbool URI::Parser::at(gint start, gint end, const String &s) const {
            gint p = start;
            gint const sn = s.length();
            if (sn > end - p)
                return false;
            gint i = 0;
            while (i < sn) {
                if (input.charAt(p++) != s.charAt(i)) {
                    break;
                }
                i++;
            }
            return (i == sn);
        }

        gint URI::Parser::scan(gint start, gint end, gchar c) const {
            if ((start < end) && (input.charAt(start) == c))
                return start + 1;
            return start;
        }

        gint URI::Parser::scan(gint start, gint end, const String &err, const String &stop) const {
            gint p = start;
            while (p < end) {
                gchar const c = input.charAt(p);
                if (err.indexOf(c) >= 0)
                    return -1;
                if (stop.indexOf(c) >= 0)
                    break;
                p++;
            }
            return p;
        }

        gint URI::Parser::scan(gint start, gint end, const String &stop) const {
            gint p = start;
            while (p < end) {
                gchar const c = input.charAt(p);
                if (stop.indexOf(c) >= 0)
                    break;
                p++;
            }
            return p;
        }

        gint URI::Parser::scanEscape(gint start, gint n, gchar first) const {
            gint const p = start;
            gchar const c = first;
            if (c == '%') {
                // Process escape pair
                if ((p + 3 <= n)
                    && match(input.charAt(p + 1), L_HEX, H_HEX)
                    && match(input.charAt(p + 2), L_HEX, H_HEX)) {
                    return p + 3;
                }
                URISyntaxException(input, "Malformed escape pair", p).throws(__trace("core.next.URI"));
            } else if ((c > 128) && !Character::isSpace(c) && !Character::isControl(c)) {
                // Allow unescaped but visible non-US-ASCII chars
                return p + 1;
            }
            return p;
        }

        gint URI::Parser::scan(gint start, gint n, glong lowMask, glong highMask) const {
            gint p = start;
            while (p < n) {
                gchar const c = input.charAt(p);
                if (match(c, lowMask, highMask)) {
                    p++;
                    continue;
                }
                if ((lowMask & L_ESCAPED) != 0) {
                    gint const q = scanEscape(p, n, c);
                    if (q > p) {
                        p = q;
                        continue;
                    }
                }
                break;
            }
            return p;
        }

        void URI::Parser::checkChars(gint start, gint end, glong lowMask, glong highMask, const String &what) {
            gint const p = scan(start, end, lowMask, highMask);
            if (p < end)
                URISyntaxException(input, "Illegal character in " + what, p).throws(__trace("core.net.URI"));
        }

        void URI::Parser::checkChar(gint p, glong lowMask, glong highMask, const String &what) {
            checkChars(p, p + 1, lowMask, highMask, what);
        }

        void URI::Parser::parse(gbool rsa) {
            requireServerAuthority = rsa;
            gint const n = input.length();
            gint p = scan(0, n, "/?#", ":");
            if ((p >= 0) && at(p, n, ':')) {
                if (p == 0) {
                    URISyntaxException(input, "Expected scheme name", 0).throws(__trace("core.net.URI"));
                }
                checkChar(0, L_ALPHA, H_ALPHA, "scheme name");
                checkChars(1, p, L_SCHEME, H_SCHEME, "scheme name");
                root.uriScheme = input.subString(0, p);
                p++;                    // Skip ':'
                if (at(p, n, '/')) {
                    p = parseHierarchical(p, n);
                } else {
                    // opaque; need to create the schemeSpecificPart
                    gint const q = scan(p, n, "#");
                    if (q <= p) {
                        URISyntaxException(input, "Expected scheme-specific part", p).throws(__trace("core.net.URI"));
                    }
                    checkChars(p, q, L_URIC, H_URIC, "opaque part");
                    root.uriSchemeSpecificPart = input.subString(p, q);
                    p = q;
                }
            } else {
                p = parseHierarchical(0, n);
            }
            if (at(p, n, '#')) {
                checkChars(p + 1, n, L_URIC, H_URIC, "fragment");
                root.uriFragment = input.subString(p + 1, n);
                p = n;
            }
            if (p < n) {
                URISyntaxException(input, "End of URI", p).throws(__trace("core.net.URI"));
            }
        }

        gint URI::Parser::parseHierarchical(gint start, gint n) {
            gint p = start;
            if (at(p, n, '/') && at(p + 1, n, '/')) {
                p += 2;
                gint const q = scan(p, n, "/?#");
                if (q > p) {
                    p = parseAuthority(p, q);
                } else if (q < n) {
                    // DEVIATION: Allow empty authority prior to non-empty
                    // path, query component or fragment identifier
                } else {
                    URISyntaxException(input, "Expected authority", p).throws(__trace("core.net.URI"));
                }
            }
            gint q = scan(p, n, "?#"); // DEVIATION: May be empty
            checkChars(p, q, L_PATH, H_PATH, "path");
            root.uriPath = input.subString(p, q);
            p = q;
            if (at(p, n, '?')) {
                p++;
                q = scan(p, n, "#");
                checkChars(p, q, L_URIC, H_URIC, "query");
                root.uriQuery = input.subString(p, q);
                p = q;
            }
            return p;
        }

        gint URI::Parser::parseAuthority(gint start, gint n) {
            gint const p = start;
            gint q = p;
            gint qreg = p;
            Optional<URISyntaxException> ex = Optional<URISyntaxException>::empty();

            gbool serverChars;
            gbool regChars;

            if (scan(p, n, "]") > p) {
                // contains a literal IPv6 address, therefore % is allowed
                serverChars = (scan(p, n, L_SERVER_PERCENT, H_SERVER_PERCENT) == n);
            } else {
                serverChars = (scan(p, n, L_SERVER, H_SERVER) == n);
            }
            regChars = ((qreg = scan(p, n, L_REG_NAME, H_REG_NAME)) == n);

            if (regChars && !serverChars) {
                // Must be a registry-based authority
                root.uriAuthority = input.subString(p, n);
                return n;
            }

            if (serverChars) {
                // Might be (probably is) a server-based authority, so attempt
                // to parse it as such.  If the attempt fails, try to treat it
                // as a registry-based authority.
                try {
                    q = parseServer(p, n);
                    if (q < n) {
                        URISyntaxException(input, "Expected end of authority", q).throws(__trace("core.net.URI"));
                    }
                    root.uriAuthority = input.subString(p, n);
                } catch (URISyntaxException const &x) {
                    // Undo results of failed parse
                    root.uriUserInfo = {};
                    root.uriHost = {};
                    root.uriPort = -1;
                    if (requireServerAuthority) {
                        // If we're insisting upon a server-based authority,
                        // then just re-throw the exception
                        x.throws(__trace("core.net.URI"));
                    } else {
                        // Save the exception in case it doesn't parse as a
                        // registry either
                        ex = Optional<URISyntaxException>::of(x);
                        q = p;
                    }
                }
            }

            if (q < n) {
                if (regChars) {
                    // Registry-based authority
                    root.uriAuthority = input.subString(p, n);
                } else if (!ex.isEmpty()) {
                    // Re-throw exception; it was probably due to
                    // a malformed IPv6 address
                    ex.get().throws(__trace("core.net.URI"));
                } else {
                    URISyntaxException(input, "Illegal character in authority", serverChars ? q : qreg)
                            .throws(__trace("core.net.URI"));
                }
            }

            return n;
        }

        gint URI::Parser::parseServer(gint start, gint n) {
            gint p = start;
            gint q;

            // userinfo
            q = scan(p, n, "/?#", "@");
            if ((q >= p) && at(q, n, '@')) {
                checkChars(p, q, L_USERINFO, H_USERINFO, "user info");
                root.uriUserInfo = input.subString(p, q);
                p = q + 1;              // Skip '@'
            }

            // hostname, IPv4 address, or IPv6 address
            if (at(p, n, '[')) {
                // DEVIATION from RFC2396: Support IPv6 addresses, per RFC2732
                p++;
                q = scan(p, n, "/?#", "]");
                if ((q > p) && at(q, n, ']')) {
                    // look for a "%" scope id
                    gint const r = scan(p, q, "%");
                    if (r > p) {
                        parseIPv6Reference(p, r);
                        if (r + 1 == q) {
                            URISyntaxException(input, "scope id expected").throws(__trace("core.net.URI"));
                        }
                        checkChars(r + 1, q, L_SCOPE_ID, H_SCOPE_ID,
                                   "scope id");
                    } else {
                        parseIPv6Reference(p, q);
                    }
                    root.uriHost = input.subString(p - 1, q + 1);
                    p = q + 1;
                } else {
                    URISyntaxException(input, "Expected closing bracket for IPv6 address", q)
                            .throws(__trace("core.net.URI"));
                }
            } else {
                q = parseIPv4Address(p, n);
                if (q <= p)
                    q = parseHostname(p, n);
                p = q;
            }

            // port
            if (at(p, n, ':')) {
                p++;
                q = scan(p, n, "/");
                if (q > p) {
                    checkChars(p, q, L_DIGIT, H_DIGIT, "port number");
                    try {
                        root.uriPort = Integer::parseInt(input, p, q, 10);
                    } catch (NumberFormatException const &x) {
                        URISyntaxException(input, "Malformed port number", p)
                                .throws(__trace("core.net.URI"));
                    }
                    p = q;
                }
            }
            if (p < n) {
                URISyntaxException(input, "Expected port number", q)
                        .throws(__trace("core.net.URI"));
            }

            return p;
        }

        gint URI::Parser::scanByte(gint start, gint n) {
            gint const p = start;
            gint const q = scan(p, n, L_DIGIT, H_DIGIT);
            if (q <= p) return q;
            if (Integer::parseInt(input, p, q, 10) > 255)
                return p;
            return q;
        }

        gint URI::Parser::scanIPv4Address(gint start, gint n, gbool strict) {
            gint p = start;
            gint q;
            gint const m = scan(p, n, L_DIGIT | L_DOT, H_DIGIT | H_DOT);
            if ((m <= p) || (strict && (m != n)))
                return -1;
            for (;;) {
                // Per RFC2732: At most three digits per gbyte
                // Further constraint: Each element fits in a gbyte
                if ((q = scanByte(p, m)) <= p) break;
                p = q;
                if ((q = scan(p, m, '.')) <= p) break;
                p = q;
                if ((q = scanByte(p, m)) <= p) break;
                p = q;
                if ((q = scan(p, m, '.')) <= p) break;
                p = q;
                if ((q = scanByte(p, m)) <= p) break;
                p = q;
                if ((q = scan(p, m, '.')) <= p) break;
                p = q;
                if ((q = scanByte(p, m)) <= p) break;
                p = q;
                if (q < m) break;
                return q;
            }
            URISyntaxException(input, "Malformed IPv4 address", q).throws(__trace("core.net.URI"));
            return -1;
        }

        gint URI::Parser::takeIPv4Address(gint start, gint n, const String &expected) {
            gint const p = scanIPv4Address(start, n, true);
            if (p <= start) {
                URISyntaxException(input, "Expected " + expected, start).throws(__trace("core.net.URI"));
            }
            return p;
        }

        gint URI::Parser::parseIPv4Address(gint start, gint n) {
            gint p;

            try {
                p = scanIPv4Address(start, n, false);
            } catch (URISyntaxException const &) {
                return -1;
            } catch (NumberFormatException const &) {
                return -1;
            }

            if (p > start && p < n) {
                // IPv4 address is followed by something - check that
                // it's a ":" as this is the only valid character to
                // follow an address.
                if (input.charAt(p) != ':') {
                    p = -1;
                }
            }

            if (p > start)
                root.uriHost = input.subString(start, p);

            return p;
        }

        gint URI::Parser::parseHostname(gint start, gint n) {
            gint p = start;
            gint q;
            gint l = -1;                 // Start of last parsed label

            do {
                // domainlabel = alphanum [ *( alphanum | "-" ) alphanum ]
                q = scan(p, n, L_ALPHANUM, H_ALPHANUM);
                if (q <= p)
                    break;
                l = p;
                p = q;
                q = scan(p, n, L_ALPHANUM | L_DASH, H_ALPHANUM | H_DASH);
                if (q > p) {
                    if (input.charAt(q - 1) == '-')
                        URISyntaxException(input, "Illegal character in hostname", q - 1)
                                .throws(__trace("core.net.URI"));
                    p = q;
                }
                q = scan(p, n, '.');
                if (q <= p)
                    break;
                p = q;
            } while (p < n);

            if ((p < n) && !at(p, n, ':'))
                URISyntaxException(input, "Illegal character in hostname", p)
                        .throws(__trace("core.net.URI"));

            if (l < 0) {
                URISyntaxException(input, "Expected hostname", start).throws(__trace("core.net.URI"));
            }

            // for a fully qualified hostname check that the rightmost
            // label starts with an alpha character.
            if (l > start && !match(input.charAt(l), L_ALPHA, H_ALPHA)) {
                URISyntaxException(input, "Illegal character in hostname", l).throws(__trace("core.net.URI"));
            }

            root.uriHost = input.subString(start, p);
            return p;
        }

        gint URI::Parser::parseIPv6Reference(gint start, gint n) {
            gint p = start;
            gint q;
            gbool compressedZeros = false;

            q = scanHexSeq(p, n);

            if (q > p) {
                p = q;
                if (at(p, n, "::")) {
                    compressedZeros = true;
                    p = scanHexPost(p + 2, n);
                } else if (at(p, n, ':')) {
                    p = takeIPv4Address(p + 1, n, "IPv4 address");
                    ipv6byteCount += 4;
                }
            } else if (at(p, n, "::")) {
                compressedZeros = true;
                p = scanHexPost(p + 2, n);
            }
            if (p < n)
                URISyntaxException(input, "Malformed IPv6 address", start).throws(__trace("core.net.URI"));
            if (ipv6byteCount > 16)
                URISyntaxException(input, "IPv6 address too long", start).throws(__trace("core.net.URI"));
            if (!compressedZeros && ipv6byteCount < 16)
                URISyntaxException(input, "IPv6 address too short", start).throws(__trace("core.net.URI"));
            if (compressedZeros && ipv6byteCount == 16)
                URISyntaxException(input, "Malformed IPv6 address", start).throws(__trace("core.net.URI"));

            return p;
        }

        gint URI::Parser::scanHexPost(gint start, gint n) {
            gint p = start;
            gint q;

            if (p == n)
                return p;

            q = scanHexSeq(p, n);
            if (q > p) {
                p = q;
                if (at(p, n, ':')) {
                    p++;
                    p = takeIPv4Address(p, n, "hex digits or IPv4 address");
                    ipv6byteCount += 4;
                }
            } else {
                p = takeIPv4Address(p, n, "hex digits or IPv4 address");
                ipv6byteCount += 4;
            }
            return p;
        }

        gint URI::Parser::scanHexSeq(gint start, gint n) {
            gint p = start;
            gint q;

            q = scan(p, n, L_HEX, H_HEX);
            if (q <= p)
                return -1;
            if (at(q, n, '.'))          // Beginning of IPv4 address
                return -1;
            if (q > p + 4)
                URISyntaxException(input, "IPv6 hexadecimal digit sequence too long", p)
                .throws(__trace("core.net.URI"));
            ipv6byteCount += 2;
            p = q;
            while (p < n) {
                if (!at(p, n, ':'))
                    break;
                if (at(p + 1, n, ':'))
                    break;              // "::"
                p++;
                q = scan(p, n, L_HEX, H_HEX);
                if (q <= p)
                    URISyntaxException(input, "digits for an IPv6 address", p).throws(__trace("core.net.URI"));
                if (at(q, n, '.')) {    // Beginning of IPv4 address
                    p--;
                    break;
                }
                if (q > p + 4)
                    URISyntaxException(input, "IPv6 hexadecimal digit sequence too long", p)
                            .throws(__trace("core.net.URI"));
                ipv6byteCount += 2;
                p = q;
            }

            return p;
        }

        URI::URI(String str) {
            Parser(*this, Unsafe::moveInstance(str)).parse(false);
        }

        URI::URI(const String &scheme, const String &userInfo, const String &host, gint port, const String &path,
                 const String &query, const String &fragment) {
            try {
                String const s = toString(scheme, {}, {}, userInfo, host, port, path, query, fragment);
                checkPath(s, scheme, path);
                Parser(*this, Unsafe::moveInstance(s)).parse(true);
            } catch (URISyntaxException const &urisex) {
                urisex.throws(__trace("core.net.URI"));
            }
        }

        URI::URI(const String &scheme, const String &authority, const String &path, const String &query,
                 const String &fragment) {
            try {
                String const s = toString(scheme, {}, authority, {}, {}, -1, path, query, fragment);
                checkPath(s, scheme, path);
                Parser(*this, Unsafe::moveInstance(s)).parse(false);
            } catch (URISyntaxException const &urisex) {
                urisex.throws(__trace("core.net.URI"));
            }
        }

        URI::URI(const String &scheme, const String &host, const String &path, const String &fragment)
                : URI(scheme, {}, host, -1, path, {}, fragment) {}

        URI::URI(const String &scheme, const String &ssp, const String &fragment) {
            try {
                String const s = toString(scheme, ssp, {}, {}, {}, -1, {}, {}, fragment);
                Parser(*this, Unsafe::moveInstance(s)).parse(false);
            } catch (URISyntaxException const &urisex) {
                urisex.throws(__trace("core.net.URI"));
            }
        }

        URI::URI(String scheme, String path) :
                uriScheme(Unsafe::moveInstance(scheme)), uriPath(Unsafe::moveInstance(path)) {}

        gbool URI::validateSchemeAndPath(const String &scheme, const String &path) {
            try {
                URI const u = URI(scheme + ":" + path);
                return scheme.equals(u.uriScheme) && path.equals(u.uriPath);
            } catch (URISyntaxException const &urisex) {
                return false;
            }
        }

        URI URI::create(const String &str) {
            try {
                return URI(str);
            } catch (URISyntaxException const &urisex) {
                IllegalArgumentException(urisex.message(), urisex).throws(__trace("core.net.URI"));
            }
        }

        URI &URI::parseServerAuthority() {
            // We could be clever and cache the error message and index from the
            // exception thrown during the original parse, but that would require
            // either more fields or a more-obscure representation.
            if ((!uriHost.isEmpty()) || (uriAuthority.isEmpty()))
                return *this;
            Parser(*this, toString()).parse(true);
            return *this;
        }

        URI URI::normalize() const {
            return normalize(*this);
        }

        URI URI::resolve(const URI &uri) const {
            return resolve(*this, uri);
        }

        URI URI::resolve(const String &str) const {
            return resolve(URI::create(str));
        }

        URI URI::relativize(const URI &uri) const {
            return relativize(*this, uri);
        }

        URL URI::toURL() const {
//            return URL::of(*this, null);
            CORE_UNREACHABLE();
        }

        String URI::scheme() const {
            return uriScheme;
        }

        gbool URI::isAbsolute() const {
            return !uriScheme.isEmpty();
        }

        gbool URI::isOpaque() const {
            return uriPath.isEmpty();
        }

        String URI::rawSchemeSpecificPart() const {
            String part = uriSchemeSpecificPart;
            if (!part.isEmpty()) {
                return part;
            }

            String const s = string;
            if (!s.isEmpty()) {
                // if a string is defined, components will have been parsed
                gint start = 0;
                gint end = s.length();
                if (!uriScheme.isEmpty()) {
                    start = uriScheme.length() + 1;
                }
                if (!uriFragment.isEmpty()) {
                    end -= uriFragment.length() + 1;
                }
                if (!uriPath.isEmpty() && uriPath.length() == end - start) {
                    part = uriPath;
                } else {
                    part = s.subString(start, end);
                }
            } else {
                StringBuffer sb = {};
                appendSchemeSpecificPart(sb, {}, authority(), userInfo(), uriHost, uriPort, path(), query());
                part = sb.toString();
            }
            return (String &) uriSchemeSpecificPart = part;
        }

        String URI::schemeSpecificPart() const {
            String part = decodedSchemeSpecificPart;
            if (part.isEmpty()) {
                (String &) decodedSchemeSpecificPart = part = decode(rawSchemeSpecificPart());
            }
            return part;
        }

        String URI::rawAuthority() const {
            return uriAuthority;
        }

        String URI::authority() const {
            String auth = decodedAuthority;
            if ((auth.isEmpty()) && (!uriAuthority.isEmpty())) {
                (String &) decodedAuthority = auth = decode(uriAuthority);
            }
            return auth;
        }

        String URI::rawUserInfo() const {
            return uriUserInfo;
        }

        String URI::userInfo() const {
            String user = decodedUserInfo;
            if ((user.isEmpty()) && (!uriUserInfo.isEmpty())) {
                (String &) decodedUserInfo = user = decode(uriUserInfo);
            }
            return user;
        }

        String URI::host() const {
            return uriHost;
        }

        gint URI::port() const {
            return uriPort;
        }

        String URI::rawPath() const {
            return uriPath;
        }

        String URI::path() const {
            String decoded = decodedPath;
            if ((decoded.isEmpty()) && (!uriPath.isEmpty())) {
                (String &) decodedPath = decoded = decode(uriPath);
            }
            return decoded;
        }

        String URI::rawQuery() const {
            return uriQuery;
        }

        String URI::query() const {
            String decoded = decodedQuery;
            if ((decoded.isEmpty()) && (!uriQuery.isEmpty())) {
                (String &) decodedQuery = decoded = decode(uriQuery, false);
            }
            return decoded;
        }

        String URI::rawFragment() const {
            return uriFragment;
        }

        String URI::fragment() const {
            String decoded = decodedFragment;
            if ((decoded.isEmpty()) && (!uriFragment.isEmpty())) {
                (String &) decodedFragment = decoded = decode(uriFragment, false);
            }
            return decoded;
        }

        gbool URI::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            if (!Class<URI>::hasInstance(obj))
                return false;
            URI const &that = (const URI &) obj;
            if (isOpaque() != that.isOpaque())
                return false;
            if (!equalIgnoringCase(uriScheme, that.uriScheme))
                return false;
            if (!equal(uriFragment, that.uriFragment))
                return false;

            // Opaque
            if (isOpaque())
                return equal(uriSchemeSpecificPart, that.uriSchemeSpecificPart);

            // Hierarchical
            if (!equal(uriPath, that.uriPath))
                return false;
            if (!equal(uriQuery, that.uriQuery))
                return false;

            // Authorities
            if (uriAuthority == that.uriAuthority)
                return true;
            if (!uriHost.isEmpty()) {
                // Server-based
                if (!equal(uriUserInfo, that.uriUserInfo))
                    return false;
                if (!equalIgnoringCase(uriHost, that.uriHost))
                    return false;
                if (uriPort != that.uriPort)
                    return false;
            } else if (!uriAuthority.isEmpty()) {
                // Registry-based
                if (!equal(uriAuthority, that.uriAuthority))
                    return false;
            } else if (uriAuthority != that.uriAuthority) {
                return false;
            }

            return true;
        }

        gint URI::hash() const {
            gint h = uriHash;
            if (h == 0) {
                h = hashIgnoringCase(0, uriScheme);
                h = hash(h, uriFragment);
                if (isOpaque()) {
                    h = hash(h, uriSchemeSpecificPart);
                } else {
                    h = hash(h, uriPath);
                    h = hash(h, uriQuery);
                    if (!uriHost.isEmpty()) {
                        h = hash(h, uriUserInfo);
                        h = hashIgnoringCase(h, uriHost);
                        h += 1949 * uriPort;
                    } else {
                        h = hash(h, uriAuthority);
                    }
                }
                if (h != 0) {
                    (gint &) uriHash = h;
                }
            }
            return h;
        }

        gint URI::compareTo(const URI &other) const {
            gint c = 0;
            if ((c = compareIgnoringCase(uriScheme, other.uriScheme)) != 0)
                return c;

            if (isOpaque()) {
                if (other.isOpaque()) {
                    // Both opaque
                    if ((c = compare(uriSchemeSpecificPart, other.uriSchemeSpecificPart)) != 0)
                        return c;
                    return compare(uriFragment, other.uriFragment);
                }
                return +1;                  // Opaque > hierarchical
            } else if (other.isOpaque()) {
                return -1;                  // Hierarchical < opaque
            }

            // Hierarchical
            if ((!uriHost.isEmpty()) && (!other.uriHost.isEmpty())) {
                // Both server-based
                if ((c = compare(uriUserInfo, other.uriUserInfo)) != 0)
                    return c;
                if ((c = compareIgnoringCase(uriHost, other.uriHost)) != 0)
                    return c;
                if ((c = uriPort - other.uriPort) != 0)
                    return c;
            } else {
                // If one or both authorities are registry-based, then we simply
                // compare them in the usual, case-sensitive way.If one is
                // registry-based and one is server-based, then the strings are
                // guaranteed to be unequal, hence the comparison will never return
                // zero and the compareTo and equals methods will remain
                // consistent.
                if ((c = compare(uriAuthority, other.uriAuthority)) != 0) return c;
            }

            if ((c = compare(uriPath, other.uriPath)) != 0) return c;
            if ((c = compare(uriQuery, other.uriQuery)) != 0) return c;
            return compare(uriFragment, other.uriFragment);
        }

        String URI::toString() const {
            String s = string;
            if (s.isEmpty()) {
                s = defineString();
            }
            return s;
        }

        String URI::defineString() const {
            String s = string;
            if (!s.isEmpty()) {
                return s;
            }

            StringBuffer sb = {};
            if (!uriScheme.isEmpty()) {
                sb.append(uriScheme);
                sb.append(':');
            }
            if (isOpaque()) {
                sb.append(uriSchemeSpecificPart);
            } else {
                if (!uriHost.isEmpty()) {
                    sb.append("//");
                    if (!uriUserInfo.isEmpty()) {
                        sb.append(uriUserInfo);
                        sb.append('@');
                    }
                    gbool const needBrackets = ((uriHost.indexOf(':') >= 0)
                                                && !uriHost.startsWith("[")
                                                && !uriHost.endsWith("]"));
                    if (needBrackets) sb.append('[');
                    sb.append(uriHost);
                    if (needBrackets) sb.append(']');
                    if (uriPort != -1) {
                        sb.append(':');
                        sb.append(uriPort);
                    }
                } else if (!uriAuthority.isEmpty()) {
                    sb.append("//");
                    sb.append(uriAuthority);
                }
                if (!uriPath.isEmpty())
                    sb.append(uriPath);
                if (!uriQuery.isEmpty()) {
                    sb.append('?');
                    sb.append(uriQuery);
                }
            }
            if (!uriFragment.isEmpty()) {
                sb.append('#');
                sb.append(uriFragment);
            }
            return (String &) string = sb.toString();
        }

        String URI::toASCIIString() const {
            return encode(toString());
        }

        gint URI::toLower(gchar c) {
            if ((c >= 'A') && (c <= 'Z'))
                return c + ('a' - 'A');
            return c;
        }

        gint URI::toUpper(gchar c) {
            if ((c >= 'a') && (c <= 'z'))
                return c - ('a' - 'A');
            return c;
        }

        gbool URI::equal(const String &s, const String &t) {
            gbool const testForEquality = true;
            gint const result = percentNormalizedComparison(s, t, testForEquality);
            return result == 0;
        }

        gbool URI::equalIgnoringCase(const String &s, const String &t) {
            if (s == t) return true;
            if ((!s.isEmpty()) && (!t.isEmpty())) {
                gint const n = s.length();
                if (t.length() != n)
                    return false;
                for (gint i = 0; i < n; i++) {
                    if (toLower(s.charAt(i)) != toLower(t.charAt(i)))
                        return false;
                }
                return true;
            }
            return false;
        }

        gint URI::hash(gint hash, const String &s) {
            if (s.isEmpty())
                return hash;
            return s.indexOf('%') < 0 ? hash * 127 + s.hash()
                                      : normalizedHash(hash, s);
        }

        gint URI::normalizedHash(gint hash, const String &s) {
            gint h = 0;
            for (gint index = 0; index < s.length(); index++) {
                gchar const ch = s.charAt(index);
                h = 31 * h + ch;
                if (ch == '%') {
                    /*
                     * Process the next two encoded characters
                     */
                    for (gint i = index + 1; i < index + 3; i++)
                        h = 31 * h + toUpper(s.charAt(i));
                    index += 2;
                }
            }
            return hash * 127 + h;
        }

        gint URI::hashIgnoringCase(gint hash, const String &s) {
            if (s.isEmpty())
                return hash;
            gint h = hash;
            gint const n = s.length();
            for (gint i = 0; i < n; i++)
                h = 31 * h + toLower(s.charAt(i));
            return h;
        }

        gint URI::compare(const String &s, const String &t) {
            gbool const testForEquality = false;
            gint const result = percentNormalizedComparison(s, t, testForEquality);
            return result;
        }

        gint URI::percentNormalizedComparison(const String &s, const String &t, gbool testForEquality) {

            if (s == t) return 0;
            if (!s.isEmpty()) {
                if (!t.isEmpty()) {
                    if (s.indexOf('%') < 0) {
                        return s.compareTo(t);
                    }
                    gint const sn = s.length();
                    gint const tn = t.length();
                    if ((sn != tn) && testForEquality)
                        return sn - tn;
                    gint val = 0;
                    gint const n = Math::min(sn, tn);
                    for (gint i = 0; i < n;) {
                        gchar const c = s.charAt(i);
                        gchar const d = t.charAt(i);
                        val = c - d;
                        if (c != '%') {
                            if (val != 0)
                                return val;
                            i++;
                            continue;
                        }
                        if (d != '%') {
                            if (val != 0)
                                return val;
                        }
                        i++;
                        val = toLower(s.charAt(i)) - toLower(t.charAt(i));
                        if (val != 0)
                            return val;
                        i++;
                        val = toLower(s.charAt(i)) - toLower(t.charAt(i));
                        if (val != 0)
                            return val;
                        i++;
                    }
                    return sn - tn;
                } else
                    return +1;
            } else {
                return -1;
            }
        }

        gint URI::compareIgnoringCase(const String &s, const String &t) {
            if (s == t)
                return 0;
            if (!s.isEmpty()) {
                if (!t.isEmpty()) {
                    gint const sn = s.length();
                    gint const tn = t.length();
                    gint const n = sn < tn ? sn : tn;
                    for (gint i = 0; i < n; i++) {
                        gint const c = toLower(s.charAt(i)) - toLower(t.charAt(i));
                        if (c != 0)
                            return c;
                    }
                    return sn - tn;
                }
                return +1;
            } else {
                return -1;
            }
        }

        void URI::checkPath(const String &s, const String &scheme, const String &path) {
            if (!scheme.isEmpty()) {
                if (!path.isEmpty() && !path.isEmpty() && path.charAt(0) != '/')
                    URISyntaxException(s, "Relative path in absolute URI").throws(__trace("core.net.URI"));
            }
        }

        void URI::appendAuthority(StringBuffer &sb, const String &authority, const String &userInfo, const String &host,
                                  gint port) {
            if (!host.isEmpty()) {
                sb.append("//");
                if (!userInfo.isEmpty()) {
                    sb.append(quote(userInfo, L_USERINFO, H_USERINFO));
                    sb.append('@');
                }
                gbool const needBrackets = ((host.indexOf(':') >= 0)
                                      && !host.startsWith("[")
                                      && !host.endsWith("]"));
                if (needBrackets) sb.append('[');
                sb.append(host);
                if (needBrackets) sb.append(']');
                if (port != -1) {
                    sb.append(':');
                    sb.append(port);
                }
            } else if (!authority.isEmpty()) {
                sb.append("//");
                if (authority.startsWith("[")) {
                    // authority should (but may not) contain an embedded IPv6 address
                    gint const end = authority.indexOf(']');
                    String doquote = authority;
                    if (end != -1 && authority.indexOf(':') != -1) {
                        // the authority contains an IPv6 address
                        sb.append(authority, 0, end + 1);
                        doquote = authority.subString(end + 1);
                    }
                    sb.append(quote(doquote,
                                    L_REG_NAME | L_SERVER,
                                    H_REG_NAME | H_SERVER));
                } else {
                    sb.append(quote(authority,
                                    L_REG_NAME | L_SERVER,
                                    H_REG_NAME | H_SERVER));
                }
            }
        }

        void URI::appendSchemeSpecificPart(StringBuffer &sb, const String &opaquePart, const String &authority,
                                           const String &userInfo, const String &host, gint port, const String &path,
                                           const String &query) {
            if (!opaquePart.isEmpty()) {
                /* check if SSP begins with an IPv6 address
                 * because we must not quote a literal IPv6 address
                 */
                if (opaquePart.startsWith("//[")) {
                    gint const end = opaquePart.indexOf(']');
                    if (end != -1 && opaquePart.indexOf(':') != -1) {
                        String const doquote = opaquePart.subString(end + 1);
                        sb.append(opaquePart, 0, end + 1);
                        sb.append(quote(doquote, L_URIC, H_URIC));
                    }
                } else {
                    sb.append(quote(opaquePart, L_URIC, H_URIC));
                }
            } else {
                appendAuthority(sb, authority, userInfo, host, port);
                if (!path.isEmpty())
                    sb.append(quote(path, L_PATH, H_PATH));
                if (!query.isEmpty()) {
                    sb.append('?');
                    sb.append(quote(query, L_URIC, H_URIC));
                }
            }
        }

        void URI::appendFragment(StringBuffer &sb, const String &fragment) {
            if (!fragment.isEmpty()) {
                sb.append('#');
                sb.append(quote(fragment, L_URIC, H_URIC));
            }
        }

        String URI::toString(const String &scheme, const String &opaquePart, const String &authority,
                             const String &userInfo, const String &host, gint port, const String &path,
                             const String &query, const String &fragment) {
            StringBuffer sb = {};
            if (!scheme.isEmpty()) {
                sb.append(scheme);
                sb.append(':');
            }
            appendSchemeSpecificPart(sb, opaquePart, authority, userInfo, host, port, path, query);
            appendFragment(sb, fragment);
            return sb.toString();
        }

        String URI::resolvePath(const String &base, const String &child, gbool absolute) {
            gint const i = base.lastIndexOf('/');
            gint const cn = child.length();
            String path = "";

            if (cn == 0) {
                // 5.2 (6a)
                if (i >= 0)
                    path = base.subString(0, i + 1);
            } else {
                // 5.2 (6a-b)
                if (i >= 0 || !absolute) {
                    path = base.subString(0, i + 1).concat(child);
                } else {
                    path = "/"_S.concat(child);
                }

            }

            // 5.2 (6c-f)
            String np = normalize(path);

            // 5.2 (6g): If the result is absolute but the path begins with "../",
            // then we simply leave the path as-is

            return np;
        }

        URI URI::resolve(const URI &base, const URI &child) {
            // check if child is opaque first so that NPE is thrown
            // if child is null.
            if (child.isOpaque() || base.isOpaque())
                return child;

            // 5.2 (2): Reference to current document (lone fragment)
            if ((child.uriScheme == null) && (child.uriAuthority == null)
                && child.uriPath.isEmpty() && (!child.uriFragment.isEmpty())
                && (child.uriQuery == null)) {
                if ((!base.uriFragment.isEmpty())
                    && child.uriFragment.equals(base.uriFragment)) {
                    return base;
                }
                URI ru = {};
                ru.uriScheme = base.uriScheme;
                ru.uriAuthority = base.uriAuthority;
                ru.uriUserInfo = base.uriUserInfo;
                ru.uriHost = base.uriHost;
                ru.uriPort = base.uriPort;
                ru.uriPath = base.uriPath;
                ru.uriFragment = child.uriFragment;
                ru.uriQuery = base.uriQuery;
                return ru;
            }

            // 5.2 (3): Child is absolute
            if (!child.uriScheme.isEmpty())
                return child;

            URI ru = URI();             // Resolved URI
            ru.uriScheme = base.uriScheme;
            ru.uriQuery = child.uriQuery;
            ru.uriFragment = child.uriFragment;

            // 5.2 (4): Authority
            if (child.uriAuthority == null) {
                ru.uriAuthority = base.uriAuthority;
                ru.uriHost = base.uriHost;
                ru.uriUserInfo = base.uriUserInfo;
                ru.uriPort = base.uriPort;

                String const cp = child.uriPath;
                if (!cp.isEmpty() && cp.charAt(0) == '/') {
                    // 5.2 (5): Child path is absolute
                    ru.uriPath = child.uriPath;
                } else {
                    // 5.2 (6): Resolve relative path
                    ru.uriPath = resolvePath(base.uriPath, cp, base.isAbsolute());
                }
            } else {
                ru.uriAuthority = child.uriAuthority;
                ru.uriHost = child.uriHost;
                ru.uriUserInfo = child.uriUserInfo;
                ru.uriPort = child.uriPort;
                ru.uriPath = child.uriPath;
            }

            // 5.2 (7): Recombine (nothing to do here)
            return ru;
        }

        URI URI::normalize(const URI &u) {
            if (u.isOpaque() || u.uriPath == null || u.uriPath.isEmpty())
                return u;

            String const np = normalize(u.uriPath);
            if (np == u.uriPath)
                return u;

            URI v = {};
            v.uriScheme = u.uriScheme;
            v.uriFragment = u.uriFragment;
            v.uriAuthority = u.uriAuthority;
            v.uriUserInfo = u.uriUserInfo;
            v.uriHost = u.uriHost;
            v.uriPort = u.uriPort;
            v.uriPath = np;
            v.uriQuery = u.uriQuery;
            return v;
        }

        URI URI::relativize(const URI &base, URI child) {
            // check if child is opaque first so that NPE is thrown
            // if child is null.
            if (child.isOpaque() || base.isOpaque())
                return child;
            if (!equalIgnoringCase(base.uriScheme, child.uriScheme)
                || !equal(base.uriAuthority, child.uriAuthority))
                return child;

            String bp = normalize(base.uriPath);
            String const cp = normalize(child.uriPath);
            if (!bp.equals(cp)) {
                if (!bp.endsWith("/"))
                    bp = bp + "/";
                if (!cp.startsWith(bp))
                    return child;
            }

            URI v = {};
            v.uriPath = cp.subString(bp.length());
            v.uriQuery = child.uriQuery;
            v.uriFragment = child.uriFragment;
            return v;
        }

        gint URI::needsNormalization(const String &path) {
            gbool normal = true;
            gint ns = 0;                     // Number of segments
            gint const end = path.length() - 1;    // Index of last gchar in a path
            gint p = 0;                      // Index of next gchar in a path

            // Skip initial slashes
            while (p <= end) {
                if (path.charAt(p) != '/') break;
                p++;
            }
            if (p > 1) normal = false;

            // Scan segments
            while (p <= end) {

                // Looking at "." or ".." ?
                if ((path.charAt(p) == '.')
                    && ((p == end)
                        || ((path.charAt(p + 1) == '/')
                            || ((path.charAt(p + 1) == '.')
                                && ((p + 1 == end)
                                    || (path.charAt(p + 2) == '/')))))) {
                    normal = false;
                }
                ns++;

                // Find beginning of next segment
                while (p <= end) {
                    if (path.charAt(p++) != '/')
                        continue;

                    // Skip redundant slashes
                    while (p <= end) {
                        if (path.charAt(p) != '/') break;
                        normal = false;
                        p++;
                    }

                    break;
                }
            }

            return normal ? -1 : ns;
        }

        void URI::split(CharArray &path, IntArray &segs) {
            gint const end = path.length() - 1;      // Index of last gchar in the path
            gint p = 0;                      // Index of next gchar in the path
            gint i = 0;                      // Index of current segment

            // Skip initial slashes
            while (p <= end) {
                if (path[p] != '/') break;
                path[p] = '\0';
                p++;
            }

            while (p <= end) {

                // Note start of segment
                segs[i++] = p++;

                // Find beginning of next segment
                while (p <= end) {
                    if (path[p++] != '/')
                        continue;
                    path[p - 1] = '\0';

                    // Skip redundant slashes
                    while (p <= end) {
                        if (path[p] != '/') break;
                        path[p++] = '\0';
                    }
                    break;
                }
            }

            if (i != segs.length())
                InternalError().throws(__trace("core.net.URI"));  // ASSERT
        }

        gint URI::join(CharArray &path, IntArray &segs) {
            gint const ns = segs.length();           // Number of segments
            gint const end = path.length() - 1;      // Index of last gchar in path
            gint p = 0;                      // Index of next path gchar to write

            if (path[p] == '\0') {
                // Restore initial slash for absolute paths
                path[p++] = '/';
            }

            for (gint i = 0; i < ns; i++) {
                gint q = segs[i];            // Current segment
                if (q == -1)
                    // Ignore this segment
                    continue;

                if (p == q) {
                    // We're already at this segment, so just skip to its end
                    while ((p <= end) && (path[p] != '\0'))
                        p++;
                    if (p <= end) {
                        // Preserve trailing slash
                        path[p++] = '/';
                    }
                } else if (p < q) {
                    // Copy q down to p
                    while ((q <= end) && (path[q] != '\0'))
                        path[p++] = path[q++];
                    if (q <= end) {
                        // Preserve trailing slash
                        path[p++] = '/';
                    }
                } else
                    InternalError().throws(__trace("core.net.URI")); // ASSERT false
            }

            return p;
        }

        void URI::removeDots(CharArray &path, IntArray &segs) {
            gint const ns = segs.length();
            gint const end = path.length() - 1;

            for (gint i = 0; i < ns; i++) {
                gint dots = 0;               // Number of dots found (0, 1, or 2)

                // Find next occurrence of "." or ".."
                do {
                    gint const p = segs[i];
                    if (path[p] == '.') {
                        if (p == end) {
                            dots = 1;
                            break;
                        } else if (path[p + 1] == '\0') {
                            dots = 1;
                            break;
                        } else if ((path[p + 1] == '.')
                                   && ((p + 1 == end)
                                       || (path[p + 2] == '\0'))) {
                            dots = 2;
                            break;
                        }
                    }
                    i++;
                } while (i < ns);
                if ((i > ns) || (dots == 0))
                    break;

                if (dots == 1) {
                    // Remove this occurrence of "."
                    segs[i] = -1;
                } else {
                    // If there is a preceding non-".." segment, remove both that
                    // segment and this occurrence of ".."; otherwise, leave this
                    // ".." segment as-is.
                    gint j;
                    for (j = i - 1; j >= 0; j--) {
                        if (segs[j] != -1) break;
                    }
                    if (j >= 0) {
                        gint const q = segs[j];
                        if ((path[q] != '.')
                              || (path[q + 1] != '.')
                              || (path[q + 2] != '\0')) {
                            segs[i] = -1;
                            segs[j] = -1;
                        }
                    }
                }
            }
        }

        void URI::maybeAddLeadingDot(CharArray &path, IntArray &segs) {

            if (path[0] == '\0')
                // The path is absolute
                return;

            gint const ns = segs.length();
            gint f = 0;                      // Index of first segment
            while (f < ns) {
                if (segs[f] >= 0)
                    break;
                f++;
            }
            if ((f >= ns) || (f == 0))
                // The path is empty, or else the original first segment survived,
                // in which case we already know that no leading "." is needed
                return;

            gint p = segs[f];
            while ((p < path.length()) && (path[p] != ':') && (path[p] != '\0')) p++;
            if (p >= path.length() || path[p] == '\0')
                // No colon in first segment, so no "." needed
                return;

            // At this point we know that the first segment is unused,
            // hence we can insert a "." segment at that position
            path[0] = '.';
            path[1] = '\0';
            segs[0] = 0;
        }

        String URI::normalize(const String &ps) {

            // Does this path need normalization?
            gint const ns = needsNormalization(ps);        // Number of segments
            if (ns < 0)
                // Nope -- just return it
                return ps;

            CharArray path = ps.chars();         // Path in gchar-array form

            // Split path into segments
            IntArray segs = IntArray(ns);               // Segment-index array
            split(path, segs);

            // Remove dots
            removeDots(path, segs);

            // Prevent scheme-name confusion
            maybeAddLeadingDot(path, segs);

            // Join the remaining segments and return the result
            String const s = String(path, 0, join(path, segs));
            if (s.equals(ps)) {
                // string was already normalized
                return ps;
            }
            return Unsafe::moveInstance(s);
        }

        gbool URI::match(gchar c, glong lowMask, glong highMask) {
            if (c == 0) // 0 doesn't have a slot in the mask. So, it never matches.
                return false;
            if (c < 64)
                return ((1L << c) & lowMask) != 0;
            if (c < 128)
                return ((1L << (c - 64)) & highMask) != 0;
            return false;
        }

        void URI::appendEncoded(charset::CharsetEncoder &encoder, StringBuffer &sb, gchar c) {
            try {
                io::ByteBuffer &bb = encoder.encode(io::CharBuffer::wrap(Unsafe::copyInstance(CharArray::of(c))));
                while (bb.hasRemaining()) {
                    gint const b = bb.get() & 0xff;
                    if (b >= 0x80)
                        appendEscape(sb, (gbyte) b);
                    else
                        sb.append((gchar) b);
                }
            } catch (CharacterCodingException const &) {
                CORE_ASSERT(false, "core.net.URI");
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.net.URI"));
            }
        }

        String URI::quote(const String &s, glong lowMask, glong highMask) {
            StringBuffer sb = {};
            gbool const allowNonASCII = ((lowMask & L_ESCAPED) != 0);
            for (gint i = 0; i < s.length(); i++) {
                gchar const c = s.charAt(i);
                if (c < u'\u0080') {
                    if (!match(c, lowMask, highMask)) {
                        if (sb.isEmpty()) {
                            sb.append(s, 0, i);
                        }
                        appendEscape(sb, (gbyte) c);
                    } else {
                        if (!sb.isEmpty())
                            sb.append(c);
                    }
                } else if (allowNonASCII && (Character::isSpace(c) || Character::isControl(c))) {
                    if (sb.isEmpty()) {
                        sb.append(s, 0, i);
                    }
                    appendEncoded(charset::UTF8::INSTANCE.encoder(), sb, c);
                } else {
                    if (!sb.isEmpty())
                        sb.append(c);
                }
            }
            return (sb.isEmpty()) ? s : sb.toString();
        }

        String URI::encode(const String &s) {
            gint const n = s.length();
            if (n == 0)
                return s;

            // First check whether we actually need to encode
            for (gint i = 0;;) {
                if (s.charAt(i) >= u'\u0080')
                    break;
                if (++i >= n)
                    return s;
            }

            String const& ns = s;//Normalizer.normalize(s, Normalizer.Form.NFC);
            ByteBuffer *bb = null;
            try {
                bb = &UTF8::INSTANCE.encoder().encode(CharBuffer::wrap(ns));
            } catch (CharacterCodingException const& x) {
                CORE_ASSERT(false, "core.net.URI");
            }

            StringBuffer sb = {};
            while (bb->hasRemaining()) {
                gint const b = bb->get() & 0xff;
                if (b >= 0x80)
                    appendEscape(sb, (gbyte)b);
                else
                    sb.append((gchar)b);
            }
            return sb.toString();
        }

        gint URI::decode(gchar c) {
            if ((c >= '0') && (c <= '9'))
                return c - '0';
            if ((c >= 'a') && (c <= 'f'))
                return c - 'a' + 10;
            if ((c >= 'A') && (c <= 'F'))
                return c - 'A' + 10;
//            assert false;
            return -1;
        }

        gbyte URI::decode(gchar c1, gchar c2) {
            return (gbyte) (((decode(c1) & 0xf) << 4)
                            | ((decode(c2) & 0xf) << 0));
        }

        String URI::decode(const String &s) {
            return decode(s, true);
        }

        String URI::decode(const String &s, gbool ignorePercentInBrackets) {
            gint const n = s.length();
            if (n == 0)
                return s;
            if (s.indexOf('%') < 0)
                return s;

            StringBuffer sb = StringBuffer(n);
            ByteBuffer &bb = ByteBuffer::allocate(n);
            CharBuffer &cb = CharBuffer::allocate(n);
            CharsetDecoder &dec = UTF8::INSTANCE.decoder()
                    .onMalformedInput(charset::Charset::REPLACE)
                    .onUnmappableCharacter(charset::Charset::REPLACE);

            // This is not horribly efficient, but it will do for now
            gchar c = s.charAt(0);
            gbool betweenBrackets = false;

            for (gint i = 0; i < n;) {
                CORE_ASSERT(c == s.charAt(i), "core.net.URI");    // Loop invariant
                if (c == '[') {
                    betweenBrackets = true;
                } else if (betweenBrackets && c == ']') {
                    betweenBrackets = false;
                }
                if (c != '%' || (betweenBrackets && ignorePercentInBrackets)) {
                    sb.append(c);
                    if (++i >= n)
                        break;
                    c = s.charAt(i);
                    continue;
                }
                bb.clear();
                for (;;) {
                    CORE_ASSERT(n - i >= 2, "core.net.URI");
                    bb.put(decode(s.charAt(i + 1), s.charAt(i+2)));
                    i += 2;
                    if (++i >= n)
                        break;
                    c = s.charAt(i);
                    if (c != '%')
                        break;
                }
                bb.flip();
                cb.clear();
                dec.reset();
                CoderResult cr = dec.decode(bb, cb, true);
                CORE_ASSERT(cr.isUnderflow(), "core.net.URI");
                cr = dec.flush(cb);
                CORE_ASSERT(cr.isUnderflow(), "core.net.URI");
                sb.append(cb.flip().toString());
            }
            Unsafe::destroyInstance(bb);
            Unsafe::destroyInstance(cb);
            Unsafe::destroyInstance(dec);
            return sb.toString();
        }

        void URI::appendEscape(StringBuffer &sb, gbyte b) {
            CORE_FAST gchar hexDigits[] = {
                    '0', '1', '2', '3', '4', '5', '6', '7',
                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
            };
            sb.append('%');
            sb.append(hexDigits[(b >> 4) & 0x0f]);
            sb.append(hexDigits[(b >> 0) & 0x0f]);
        }

        Object &URI::clone() const {
            return Unsafe::allocateInstance<URI>(*this);
        }


    } // core
} // net
