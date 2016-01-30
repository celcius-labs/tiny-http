#include "http_response.h"
#ifndef ARDUINO_ARCH_ESP8266
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif

static void send_headers (Response *);

#ifdef ARDUINO_ARCH_ESP8266
Response *response_create (WiFiClient connection) {
  static Response response = { 0 };

  response.connection = connection;

  return &response;
}
#else
Response *response_create (uint8_t fd) {
  static Response response = { 0 };

  response.fd = fd;

  return &response;
}
#endif

void response_write (Response *response, uint8_t *data) {
  if (response->headers_sent == 0) {
    send_headers(response);
  }

#ifdef ARDUINO_ARCH_ESP8266
  response->connection.print((char *) data);
#else
  size_t size = strlen((char *) data);
  write(response->fd, (void *) data, size * sizeof(uint8_t));
#endif
}

static const char *response_status (uint16_t code) {
  switch (code) {
    case 200:
    return "HTTP/1.1 200 OK\r\n";
    case 400:
    return "HTTP/1.1 404 NOT FOUND\r\n";
    case 500:
    return "HTTP/1.1 500 ERROR\r\n";
    default:
    return "HTTP/1.1 200 OK\r\n";
  }
}

static void send_headers (Response *response) {
  response->headers_sent = 1;

#ifdef ARDUINO_ARCH_ESP8266
  response->connection.print(response_status(response->code));

  response->connection.print("Content-type: ")

  if (response->content_type[0] != '\0') {
    response->connection.print(response->content_type);
    response->connection.print("\r\n");
  } else {
    response->connection.print("text/plain\r\n");
  }

  for (uint8_t i = 0; i < response->num_headers; i++) {
    response->connection.print(response->headers[i]);
    response->connection.print("\r\n");
  }

  Serial.print("\r\n");
#else
  const char *status = response_status(response->code);
  write(response->fd, (void *) status, strlen(status));
  write(response->fd, (void *) "Content-type: ", 14);

  if (response->content_type[0] != '\0') {
    write(response->fd, (void *) response->content_type, strlen((char *) response->content_type));
  } else {
    write(response->fd, (void *) "text/plain", 10);
  }

  write(response->fd, "\r\n", 2);

  for (uint8_t i = 0; i < response->num_headers; i++) {
    write(response->fd, (void *) response->headers[i], strlen((char *) response->headers[i]));
    write(response->fd, "\r\n", 2);
  }

  write(response->fd, "\r\n", 2);
#endif
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
