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
  check((strcmp((char *) request->path, "/") == 0), "path is set correctly");

  free(request);

  strcpy((char *) body, "POST /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse POST does not return NULL");
  check((request->method == HTTP_POST), "method is set to POST");
  check((strcmp((char *) request->path, "/") == 0), "path is set correctly");

  free(request);

  strcpy((char *) body, "PUT /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse PUT does not return NULL");
  check((request->method == HTTP_PUT), "method is set to PUT");
  check((strcmp((char *) request->path, "/") == 0), "path is set correctly");

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

uint8_t test_parse_params ( ) {
  uint8_t uri[256];
  uint8_t **params;

  strcpy((char *) uri, "/foo");
  params = parse_params(uri);

  check((params == NULL), "parse of uri without params should have no params and set to NULL");

  strcpy((char *) uri, "/foo?");
  params = parse_params(uri);

  check((params == NULL), "parse of uri without params but a ? should have no params and set to NULL");

  strcpy((char *) uri, "/foo?test=hello");
  params = parse_params(uri);

  check((params != NULL), "parse of uri with one param is not NULL");
  check((strcmp((char *) params[0], "test=hello") == 0), "first param is correct");
  check((params[1] == NULL), "second param is NULL");

  strcpy((char *) uri, "/foo?test=hello&yay=tests");
  params = parse_params(uri);

  check((params != NULL), "parse of uri with two params is not NULL");
  check((strcmp((char *) params[0], "test=hello") == 0), "first param is correct");
  check((strcmp((char *) params[1], "yay=tests") == 0), "second param is correct");
  check((params[2] == NULL), "third param is NULL");

  strcpy((char *) uri, "/foo?test=hello&yay=tests&more=tests");
  params = parse_params(uri);

  check((params != NULL), "parse of uri with three params is not NULL");
  check((strcmp((char *) params[0], "test=hello") == 0), "first param is correct");
  check((strcmp((char *) params[1], "yay=tests") == 0), "second param is correct");
  check((strcmp((char *) params[2], "more=tests") == 0), "third param is correct");
  check((params[3] == NULL), "fourth param is NULL");

  done();
}
