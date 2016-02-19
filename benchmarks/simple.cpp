#include "http_request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define COUNT 10000000

int main ( ) {
  uint8_t buf[512] = "GET /cookies HTTP/1.1\n\rHost: 127.0.0.1:8090\n\rConnection: keep-alive\n\rCache-Control: max-age=0\n\rAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n\rUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17\n\rAccept-Encoding: gzip,deflate,sdch\n\rAccept-Language: en-US,en;q=0.8\n\rAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\n\rCookie: name=wookie\n\r\n\r";
  int i;
  float start, end;
  Request *parsed;

  start = (float)clock()/CLOCKS_PER_SEC;
  for (i = 0; i < COUNT; i++) {
    parsed = parse_request(buf);
  }
  end = (float)clock()/CLOCKS_PER_SEC;

  printf("Elapsed %f seconds. (%d requests/second)\n", (end - start), (int) (COUNT / (end - start)));

  return 0;
}
