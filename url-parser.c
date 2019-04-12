/*
 *  url-parser.c
 *
 *  copyright (c) 2019 Xiongfei Shi
 *
 *  author: Xiongfei Shi <jenson.shixf(a)gmail.com>
 *  license: Apache2.0
 *
 *  https://github.com/shixiongfei/url-parser
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "url-parser.h"

static int url_isschemechar(int c) {
  return !(!isalpha(c) && ('+' != c) && ('-' != c) && ('.' != c));
}

int url_parse(url_t *url, const char *urlstr) {
  const char *tmpstr;
  const char *curstr;
  int i, len;
  int userpass_flag;
  int bracket_flag;

  memset(url, 0, sizeof(url_t));

  curstr = urlstr;

  /*
   * <scheme>:<scheme-specific-part>
   * <scheme> := [a-z\+\-\.]+
   *             upper case = lower case for resiliency
   */
  /* Read scheme */
  tmpstr = strchr(curstr, ':');

  /* Not found the character */
  if (NULL == tmpstr) return -1;

  /* Get the scheme length */
  len = (int)(tmpstr - curstr);

  /* Check restrictions */
  for (i = 0; i < len; i++) {
    /* Invalid format */
    if (!url_isschemechar(curstr[i])) return -1;
  }

  /* Scheme position and length */
  url->scheme[0] = (int)(curstr - urlstr);
  url->scheme[1] = len;

  /* Skip ':' */
  tmpstr++;
  curstr = tmpstr;

  /*
   * //<user>:<password>@<host>:<port>/<url-path>
   * Any ":", "@" and "/" must be encoded.
   */
  /* Eat "//" */
  for (i = 0; i < 2; i++) {
    if ('/' != *curstr) return -1;
    curstr++;
  }

  /* Check if the user (and password) are specified. */
  userpass_flag = 0;
  tmpstr = curstr;
  while ('\0' != *tmpstr) {
    if ('@' == *tmpstr) {
      /* Username and password are specified */
      userpass_flag = 1;
      break;
    } else if ('/' == *tmpstr) {
      /* End of <host>:<port> specification */
      userpass_flag = 0;
      break;
    }
    tmpstr++;
  }

  /* User and password specification */
  tmpstr = curstr;
  if (userpass_flag) {
    /* Read username */
    while ('\0' != *tmpstr && ':' != *tmpstr && '@' != *tmpstr)
      tmpstr++;

    url->username[0] = (int)(curstr - urlstr);
    url->username[1] = (int)(tmpstr - curstr);

    /* Proceed current pointer */
    curstr = tmpstr;
    if (':' == *curstr) {
      /* Skip ':' */
      curstr++;
      /* Read password */
      tmpstr = curstr;
      while ('\0' != *tmpstr && '@' != *tmpstr)
        tmpstr++;

      url->password[0] = (int)(curstr - urlstr);
      url->password[1] = (int)(tmpstr - curstr);

      curstr = tmpstr;
    }
    /* Skip '@' */
    if ('@' != *curstr) return -1;
    curstr++;
  }

  if ('[' == *curstr)
    bracket_flag = 1;
  else
    bracket_flag = 0;

  /* Proceed on by delimiters with reading host */
  tmpstr = curstr;
  while ('\0' != *tmpstr) {
    if (bracket_flag && ']' == *tmpstr) {
      /* End of IPv6 address. */
      tmpstr++;
      break;
    } else if (!bracket_flag && (':' == *tmpstr || '/' == *tmpstr))
      break; /* Port number is specified. */
    tmpstr++;
  }

  url->host[0] = (int)(curstr - urlstr);
  url->host[1] = (int)(tmpstr - curstr);

  curstr = tmpstr;

  /* Is port number specified? */
  if (':' == *curstr) {
    curstr++;
    /* Read port number */
    tmpstr = curstr;
    while ('\0' != *tmpstr && '/' != *tmpstr)
      tmpstr++;

    url->port[0] = (int)(curstr - urlstr);
    url->port[1] = (int)(tmpstr - curstr);

    curstr = tmpstr;
  }

  /* End of the string */
  if ('\0' == *curstr) return 0;

  /* Skip '/' */
  if ('/' != *curstr) return -1;
  curstr++;

  /* Parse path */
  tmpstr = curstr;
  while ('\0' != *tmpstr && '#' != *tmpstr && '?' != *tmpstr)
    tmpstr++;

  url->path[0] = (int)(curstr - urlstr);
  url->path[1] = (int)(tmpstr - curstr);

  curstr = tmpstr;

  /* Is query specified? */
  if ('?' == *curstr) {
    /* Skip '?' */
    curstr++;
    /* Read query */
    tmpstr = curstr;
    while ('\0' != *tmpstr && '#' != *tmpstr)
      tmpstr++;

    url->query[0] = (int)(curstr - urlstr);
    url->query[1] = (int)(tmpstr - curstr);

    curstr = tmpstr;
  }

  /* Is fragment specified? */
  if ('#' == *curstr) {
    /* Skip '#' */
    curstr++;
    /* Read fragment */
    tmpstr = curstr;
    while ('\0' != *tmpstr)
      tmpstr++;

    url->fragment[0] = (int)(curstr - urlstr);
    url->fragment[1] = (int)(tmpstr - curstr);

    curstr = tmpstr;
  }

  return 0;
}

static const char unreserved_char[] = {
    /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, // 2
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // 3
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, // 5
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, // 7
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // F
};

static const char hex_to_num[] = {
    /* 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 1
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 2
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  -1, -1, -1, -1, -1, -1, // 3
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 4
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 5
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 6
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 7
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 8
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 9
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // A
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // B
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // C
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // D
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // E
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  // F
};

int url_encode(char *outbuf, int outlen, const char *url) {
  const unsigned char *p = (const unsigned char *)url;
  int retval = 0;

  outlen -= 1; /* last character is '\0' */
  while (('\0' != (*p)) && (retval < outlen)) {
    if (unreserved_char[*p])
      outbuf[retval++] = (char)(*p);
    else {
      outbuf[retval++] = '%';

      if ((outlen - retval) < 2)
        break;

      retval += sprintf(outbuf + retval, "%02x", (*p));
    }
    p += 1;
  }
  outbuf[retval] = '\0';

  return retval;
}

int url_decode(char *outbuf, int outlen, const char *url) {
  const unsigned char *p = (const unsigned char *)url;
  int retval = 0;

  outlen -= 1; /* last character is '\0' */
  while (('\0' != (*p)) && (retval < outlen)) {
    if ('%' == (*p)) {
      p += 1;
      if ('\0' == (*p))
        break;
      outbuf[retval] = hex_to_num[*p] << 4;

      p += 1;
      if ('\0' == (*p))
        break;
      outbuf[retval] |= hex_to_num[*p];

      retval += 1;
    } else
      outbuf[retval++] = (char)(*p);
    p += 1;
  }
  outbuf[retval] = '\0';

  return retval;
}
