/*
 *  test.c
 *
 *  copyright (c) 2019 Xiongfei Shi
 *
 *  author: Xiongfei Shi <jenson.shixf(a)gmail.com>
 *  license: Apache2.0
 *
 *  https://github.com/shixiongfei/url-parser
 */

#include <stdio.h>
#include <string.h>

#include "url-parser.h"

int main(int argc, char *argv[]) {
  const char urlstr[] = { "ftp://root:123456@shixf.com:12345/url-parser/source?hello=123&world=456#README.md" };
  const char text[] = { "Hello World!" };
  url_t url;
  char scheme[32] = { 0 };
  char host[32] = { 0 };
  char port[32] = { 0 };
  char path[32] = { 0 };
  char query[32] = { 0 };
  char fragment[32] = { 0 };
  char username[32] = { 0 };
  char password[32] = { 0 };
  char encode[64] = { 0 };
  char decode[64] = { 0 };

  printf("Parse URL: %d\n", url_parse(&url, urlstr));
  printf("scheme: %s\n",    strncpy(scheme,   urlstr + url.scheme[0],   url.scheme[1]));
  printf("host: %s\n",      strncpy(host,     urlstr + url.host[0],     url.host[1]));
  printf("port: %s\n",      strncpy(port,     urlstr + url.port[0],     url.port[1]));
  printf("path: %s\n",      strncpy(path,     urlstr + url.path[0],     url.path[1]));
  printf("query: %s\n",     strncpy(query,    urlstr + url.query[0],    url.query[1]));
  printf("fragment: %s\n",  strncpy(fragment, urlstr + url.fragment[0], url.fragment[1]));
  printf("username: %s\n",  strncpy(username, urlstr + url.username[0], url.username[1]));
  printf("password: %s\n",  strncpy(password, urlstr + url.password[0], url.password[1]));

  url_encode(encode, sizeof(encode), text);
  url_decode(decode, sizeof(decode), encode);

  printf("url encode: %s\n", encode);
  printf("url decode: %s\n", decode);

  return 0;
}
