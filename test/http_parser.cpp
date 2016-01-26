#include <stdlib.h>

#include "test.h"

#include "http_parser.h"

uint8_t test_parse_string ( ) {
  Request *request;

  request = parse_request((uint8_t *) "GET /");

  check((request != NULL), "parse GET does not return NULL");
  check((request->method == HTTP_GET), "method is set to GET");

  free(request);

  request = parse_request((uint8_t *) "POST /");

  check((request != NULL), "parse POST does not return NULL");
  check((request->method == HTTP_POST), "method is set to POST");

  free(request);

  request = parse_request((uint8_t *) "PUT /");

  check((request != NULL), "parse PUT does not return NULL");
  check((request->method == HTTP_PUT), "method is set to PUT");

  free(request);

  done();
}
