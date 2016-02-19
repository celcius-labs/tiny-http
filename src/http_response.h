#pragma once

#include <stdint.h>

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#endif

#include "http_request.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Response {
  uint16_t code;
  uint8_t *headers[MAX_HEADERS];
  uint8_t num_headers;
  uint8_t content_type[64];
  uint8_t headers_sent;
#ifdef ARDUINO_ARCH_ESP8266
  WiFiClient connection;
#else
  uint8_t fd;
#endif
} Response;

#ifdef ARDUINO_ARCH_ESP8266
Response *response_create (WiFiClient);
#else
Response *response_create (uint8_t);
#endif

void response_write (Response *, uint8_t *);
void response_set_header (Response *, uint8_t *);
void response_set_content_type (Response *, uint8_t *);

#ifdef __cplusplus
}
#endif
