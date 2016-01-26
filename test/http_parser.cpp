#include <stdlib.h>
#include <string.h>

#include "test.h"

#include "http_parser.h"

uint8_t test_parse_string ( ) {
  Request *request;

  uint8_t body[256];


  strcpy((char *) body, "GET /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse GET does not return NULL");
  check((request->method == HTTP_GET), "method is set to GET");
  check((strcmp((char *) request->uri, "/") == 0), "URI is set correctly");

  free(request);

  strcpy((char *) body, "POST /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse POST does not return NULL");
  check((request->method == HTTP_POST), "method is set to POST");
  check((strcmp((char *) request->uri, "/") == 0), "URI is set correctly");

  free(request);

  strcpy((char *) body, "PUT /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse PUT does not return NULL");
  check((request->method == HTTP_PUT), "method is set to PUT");
  check((strcmp((char *) request->uri, "/") == 0), "URI is set correctly");

  free(request);

  strcpy((char *) body, "FOO /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse FOO does not return NULL");
  check((request->method == HTTP_ERROR), "method is set to ERROR");

  free(request);

  strcpy((char *) body, "GET");
  request = parse_request(body);

  check((request != NULL), "parse just GET does not return NULL");
  check((request->method == HTTP_ERROR), "method is set to ERROR");

  free(request);

  strcpy((char *) body, "GET");
  request = parse_request(body);

  check((request != NULL), "parse just POST does not return NULL");
  check((request->method == HTTP_ERROR), "method is set to ERROR");

  free(request);

  strcpy((char *) body, "PUT");
  request = parse_request(body);

  check((request != NULL), "parse just PUT does not return NULL");
  check((request->method == HTTP_ERROR), "method is set to ERROR");

  free(request);

  done();
}
