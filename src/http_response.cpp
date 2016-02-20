#include "http_response.h"
#ifndef ARDUINO_ARCH_ESP8266
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif

static void send_headers (Response *);
static void _write (Response *, uint8_t *);

#define WRITE(response, data) _write(response, (uint8_t *) data);

void _write (Response *response, uint8_t *data) {
  if (response->write != NULL) {
    response->write(data);
  } else {
#ifdef ARDUINO_ARCH_ESP8266
    response->connection.print((char *) data);
#else
    size_t size = strlen((char *) data);
    uint8_t res = write(response->fd, (void *) data, size * sizeof(uint8_t));

    if (res) {
      // gcc noop
    }
#endif
  }
}

#ifdef ARDUINO_ARCH_ESP8266
Response *response_create (WiFiClient connection) {
  static Response response = { 0 };

  memset(&response, 0, sizeof(Response));

  response.connection = connection;

  return &response;
}
#else
Response *response_create (uint8_t fd) {
  static Response response = { 0 };

  memset(&response, 0, sizeof(Response));

  response.fd = fd;

  return &response;
}
#endif

void response_write (Response *response, uint8_t *data) {
  if (response->headers_sent == 0) {
    send_headers(response);
  }

  WRITE(response, data);
}

static const char *response_status (uint16_t code) {
  switch (code) {
    case 200:
    return "HTTP/1.1 200 OK\r\n";
    case 404:
    return "HTTP/1.1 404 NOT FOUND\r\n";
    case 500:
    return "HTTP/1.1 500 ERROR\r\n";
    default:
    return "HTTP/1.1 200 OK\r\n";
  }
}

static void send_headers (Response *response) {
  // if there is no header sent, set it to 200
  if (response->code == 0) {
    response->code = 200;
  }

  response->headers_sent = 1;

  WRITE(response, response_status(response->code));
  WRITE(response, "Content-type: ")

  if (response->content_type[0] != '\0') {
    WRITE(response, response->content_type);
    WRITE(response, "\r\n");
  } else {
    WRITE(response, "text/html\r\n");
  }

  for (uint8_t i = 0; i < response->num_headers; i++) {
    WRITE(response, response->headers[i]);
    WRITE(response, "\r\n");
  }

  WRITE(response, "\r\n");
}

void response_set_header (Response *response, uint8_t *header) {
  if (response->num_headers < MAX_HEADERS - 1) {
    response->headers[response->num_headers] = header;
    response->num_headers++;
  }
}

void response_set_content_type (Response *response, uint8_t *type) {
  uint16_t len = strlen((char *) type);
  if (len < 64) {
    memcpy((void *) response->content_type, (void *) type, len);
  }
}
