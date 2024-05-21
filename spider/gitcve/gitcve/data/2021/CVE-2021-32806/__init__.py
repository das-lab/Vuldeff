# -*- coding: utf-8 -*-
from ._compat import get_external_sites
from ._compat import unescape
from ._compat import urljoin
from ._compat import urlparse
from posixpath import normpath

# This is the class we will patch:
from Products.CMFPlone.URLTool import URLTool

import re
import string
import unicodedata


# These schemas are allowed in full urls to consider them in the portal:
# A mailto schema is an obvious sign of a url that is not in the portal.
# This is a whitelist.
ALLOWED_SCHEMAS = [
    "https",
    "http",
]
# These bad parts are not allowed in urls that are in the portal:
# This is a blacklist.
BAD_URL_PARTS = [
    "\\\\",
    "<script",
    "%3cscript",
    "javascript:",
    "javascript%3a",
]

# Determine allowed ascii characters.
# We want to allow most printable characters,
# but no whitespace, and no punctuation, except for a few exceptions.
# This boils down to ascii letters plus digits plus exceptions.
# Exceptions:
# - dot and slash for relative or absolute paths.
# - @ because we have views starting with @@
# - + because we have ++resource++ urls
allowed_ascii = string.ascii_letters + string.digits + "./@+"


def safe_url_first_char(url):
    # For character code points higher than 127, the bytes representation of a character
    # is longer than the unicode representation, so url[0] may give different results
    # for bytes and unicode.  On Python 2:
    # >>> unichr(128)
    # u'\x80'
    # >>> len(unichr(128))
    # 1
    # >>> unichr(128).encode("latin-1")
    # '\x80'
    # >>> len(unichr(128).encode("latin-1"))
    # 1
    # >>> unichr(128).encode("utf-8")
    # '\xc2\x80'
    # >>> len(unichr(128).encode("utf-8"))
    # 2
    # >>> unichr(128).encode("utf-8")[0]
    # '\xc2'
    # So make sure we have unicode here for comparing the first character.
    if isinstance(url, bytes):
        # Remember, on Python 2, bytes == str.
        try:
            first = url.decode("utf-8")[0]
        except UnicodeDecodeError:
            # We don't trust this
            return False
    else:
        first = url[0]
    if ord(first) < 128:
        if first not in allowed_ascii:
            # The first character of the url is ascii but not in the allowed range.
            return False
    else:
        # This is non-ascii, which has lots of control characters, which may be dangerous.
        # Check taken from django.utils.http._is_safe_url.  See
        # https://github.com/django/django/blob/2.1.5/django/utils/http.py#L356-L382
        # Forbid URLs that start with control characters. Some browsers (like
        # Chrome) ignore quite a few control characters at the start of a
        # URL and might consider the URL as scheme relative.
        # For categories, see 5.7.1 General Category Values here:
        # http://www.unicode.org/reports/tr44/tr44-6.html#Property_Values
        # We look for Control categories here.
        if unicodedata.category(first)[0] == "C":
            return False
    return True


def isURLInPortal(self, url, context=None):
    # Note: no docstring, because the method is publicly available
    # but does not need to be callable on site-url/portal_url/isURLInPortal.

    # Check if a given url is on the same host and contains the portal
    # path.  Used to ensure that login forms can determine relevant
    # referrers (i.e. in portal).  Also return true for some relative
    # urls if context is passed in to allow for url parsing. When context
    # is not provided, assume that relative urls are in the portal. It is
    # assumed that http://portal is the same portal as https://portal.

    # External sites listed in 'allow_external_login_sites' of
    # site_properties are also considered within the portal to allow for
    # single sign on.

    if len(url.splitlines()) > 1:
        # very fishy
        return False
    if url != url.strip():
        # somewhat fishy
        return False
    if url != " ".join(url.split()):
        # Some non-normal whitespace is used, like a tab.
        # Could be a ploy to circumvent our checks.  We don't trust this.
        return False
    if url and not safe_url_first_char(url):
        return False

    # sanitize url
    url = re.sub("^[\x00-\x20]+", "", url).strip()
    cmp_url = url.lower()
    for bad in BAD_URL_PARTS:
        if bad in cmp_url:
            return False

    p_url = self()

    schema, u_host, u_path, _, _, _ = urlparse(url)
    if schema and schema not in ALLOWED_SCHEMAS:
        # Redirecting to 'data:' may be harmful,
        # and redirecting to 'mailto:' or 'ftp:' is silly.
        return False

    # Someone may be doing tricks with escaped html code.
    unescaped_url = unescape(url)
    if unescaped_url != url:
        if not self.isURLInPortal(unescaped_url):
            return False

    if not u_host and not u_path.startswith("/"):
        if context is None:
            return True  # old behavior
        if not context.isPrincipiaFolderish:
            useurl = context.aq_parent.absolute_url()
        else:
            useurl = context.absolute_url()
    else:
        useurl = p_url  # when u_path.startswith('/')
    if not useurl.endswith("/"):
        useurl += "/"

    # urljoin to current url to get an absolute path
    _, u_host, u_path, _, _, _ = urlparse(urljoin(useurl, url))

    # normalise to end with a '/' so /foobar is not considered within /foo
    if not u_path:
        u_path = "/"
    else:
        u_path = normpath(u_path)
        if not u_path.endswith("/"):
            u_path += "/"
    _, host, path, _, _, _ = urlparse(p_url)
    if not path.endswith("/"):
        path += "/"
    if host == u_host and u_path.startswith(path):
        return True

    for external_site in get_external_sites(self):
        _, host, path, _, _, _ = urlparse(external_site)
        if not path.endswith("/"):
            path += "/"
        if host == u_host and u_path.startswith(path):
            return True
    return False


# Add our method to the URLTool.
URLTool.isURLInPortal = isURLInPortal
