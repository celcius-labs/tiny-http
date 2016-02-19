#include <stdlib.h>
#include <string.h>

#include "test.h"

#include "http_request.h"

uint8_t test_parse_string ( ) {
  Request *request;

  uint8_t body[256];


  strcpy((char *) body, "GET /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse GET does not return NULL");
  check((request->method == HTTP_GET), "method is set to GET");
  check((strcmp((char *) request->path, "/") == 0), "path is set correctly");

  strcpy((char *) body, "POST /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse POST does not return NULL");
  check((request->method == HTTP_POST), "method is set to POST");
  check((strcmp((char *) request->path, "/") == 0), "path is set correctly");

  strcpy((char *) body, "PUT /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse PUT does not return NULL");
  check((request->method == HTTP_PUT), "method is set to PUT");
  check((strcmp((char *) request->path, "/") == 0), "path is set correctly");

  strcpy((char *) body, "FOO /\n\r\n\r");
  request = parse_request(body);

  check((request != NULL), "parse FOO does not return NULL");
  check((request->method == HTTP_ERROR), "method is set to ERROR");

  strcpy((char *) body, "GET");
  request = parse_request(body);

  check((request != NULL), "parse just GET does not return NULL");
  check((request->method == HTTP_ERROR), "method is set to ERROR");

  strcpy((char *) body, "GET");
  request = parse_request(body);

  check((request != NULL), "parse just POST does not return NULL");
  check((request->method == HTTP_ERROR), "method is set to ERROR");

  strcpy((char *) body, "PUT");
  request = parse_request(body);

  check((request != NULL), "parse just PUT does not return NULL");
  check((request->method == HTTP_ERROR), "method is set to ERROR");

  strcpy((char *) body, "GET /?foo=bar HTTP/1.1\n\r");
  request = parse_request(body);

  check((request != NULL), "parse GET does not return NULL");
  check((request->method == HTTP_GET), "method is set to GET");
  check((strcmp((char *) request->path, "/") == 0), "path is set correctly");
  check((strcmp((char *) request->params[0], "foo=bar") == 0), "params are set correctly");

  strcpy((char *) body, "GET /?foo=bar HTTP/1.1\n\rHeader1: foo\n\rHeader2: bar");
  request = parse_request(body);

  check((request != NULL), "parse GET does not return NULL");
  check((request->method == HTTP_GET), "method is set to GET");
  check((strcmp((char *) request->path, "/") == 0), "path is set correctly");
  check((strcmp((char *) request->params[0], "foo=bar") == 0), "params are set correctly");
  check((strcmp((char *) request->headers[0], "Header1: foo") == 0), "header 1 is set correctly");
  check((strcmp((char *) request->headers[1], "Header2: bar") == 0), "header 2 is set correctly");
  check((request->headers[2] == NULL), "header 3 is null");

  strcpy((char *) body, "GET /?foo=bar HTTP/1.1\r\nHeader1: foo\r\nHeader2: bar");
  request = parse_request(body);

  check((request != NULL), "parse GET does not return NULL when CRLF reversed");
  check((request->method == HTTP_GET), "method is set to GET");
  check((strcmp((char *) request->path, "/") == 0), "path is set correctly");
  check((strcmp((char *) request->params[0], "foo=bar") == 0), "params are set correctly");
  check((strcmp((char *) request->headers[0], "Header1: foo") == 0), "header 1 is set correctly");
  check((strcmp((char *) request->headers[1], "Header2: bar") == 0), "header 2 is set correctly");
  check((request->headers[2] == NULL), "header 3 is null");

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

  strcpy((char *) uri, "/foo?test=hello&yay=tests&more=tests");
  params = parse_params(uri);

  check((params != NULL), "parse of uri with three params is not NULL");
  check((strcmp((char *) params[0], "test=hello") == 0), "first param is correct");
  check((strcmp((char *) params[1], "yay=tests") == 0), "second param is correct");
  check((strcmp((char *) params[2], "more=tests") == 0), "third param is correct");
  check((params[3] == NULL), "fourth param is NULL");

  done();
}

uint8_t test_parse_headers ( ) {
  uint8_t request[256];
  uint8_t **headers;

  request[0] = '\0';
  headers = parse_headers(request);

  check((headers == NULL), "parse of headers is NULL when there are no headers");

  strcpy((char *) request, "Header1: foo");
  headers = parse_headers(request);

  check((headers != NULL), "parse of headers is not NULL");
  check((strcmp((char *) headers[0], "Header1: foo") == 0), "first header is correct");
  check((headers[1] == NULL), "second header is NULL");

  strcpy((char *) request, "Header1: foo\n\rHeader2: bar\n\r");
  headers = parse_headers(request);

  check((headers != NULL), "parse of headers is not NULL");
  check((strcmp((char *) headers[0], "Header1: foo") == 0), "first header is correct");
  check((strcmp((char *) headers[1], "Header2: bar") == 0), "second header is correct");
  check((headers[2] == NULL), "third header is NULL");

  done();
}
