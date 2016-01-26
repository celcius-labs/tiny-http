#pragma once

#include <stdint.h>

#define HTTP_ERROR 0
#define HTTP_GET 1
#define HTTP_POST 2
#define HTTP_PUT 3

typedef struct Request {
  uint8_t method;
  uint8_t **params;
  uint8_t *uri;
  uint8_t *body;
} Request;

Request *parse_request (uint8_t *);
