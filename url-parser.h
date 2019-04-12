/*
 *  url-parser.h
 *
 *  copyright (c) 2019 Xiongfei Shi
 *
 *  author: Xiongfei Shi <jenson.shixf(a)gmail.com>
 *  license: Apache2.0
 *
 *  https://github.com/shixiongfei/url-parser
 */

#ifndef __URL_PARSER_H__
#define __URL_PARSER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct url_s {
  int scheme[2];    /* mandatory */
  int host[2];      /* mandatory */
  int port[2];      /* optional */
  int path[2];      /* optional */
  int query[2];     /* optional */
  int fragment[2];  /* optional */
  int username[2];  /* optional */
  int password[2];  /* optional */
} url_t;

int url_parse(url_t *url, const char *urlstr);
int url_encode(char *outbuf, int outlen, const char *url);
int url_decode(char *outbuf, int outlen, const char *url);

#ifdef __cplusplus
};
#endif

#endif /* __URL_PARSER_H__ */
