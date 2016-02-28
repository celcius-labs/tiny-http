#include <stdlib.h>
#include <string.h>

#include "test.h"

#include "http_response.h"

static uint8_t write_buffer[1024] = { 0 };

static void reset_write ( ) {
  write_buffer[0] = '\0';
}

static void test_write (uint8_t *buf) {
  strcat((char *) write_buffer, (char *) buf);
}

uint8_t test_response_write ( ) {
  Response *response = response_create(0);
  response->write = test_write;

  response_write(response, (uint8_t *) "Hello");

  check((response->headers_sent == 1), "headers are sent");
  check((response->code == 200), "response code is set to 200");

  const char *expected = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\nHello";
  check((strcmp((char *) write_buffer, expected) == 0), "output is correct");


  reset_write();

  response = response_create(0);
  response->write = test_write;

  check((response->headers_sent == 0), "response_create returns a clean response");

  response_set_content_type(response, (uint8_t *) "test/foo");

  check((strcmp((char *) response->content_type, "test/foo") == 0), "the content_type is set correctly");
  response_write(response, (uint8_t *) "Hello");

  check((response->headers_sent == 1), "headers are sent");
  check((response->code == 200), "response code is set to 200");

  const char *expected2 = "HTTP/1.1 200 OK\r\nContent-type: test/foo\r\n\r\nHello";
  check((strcmp((char *) write_buffer, expected2) == 0), "output is correct");


  reset_write();

  response = response_create(0);
  response->write = test_write;

  response->code = 404;

  check((response->code == 404), "the response code is set correctly");
  response_write(response, (uint8_t *) "Hello");

  check((response->headers_sent == 1), "headers are sent");
  check((response->code == 404), "response code is still set to 404");

  const char *expected3 = "HTTP/1.1 404 NOT FOUND\r\nContent-type: text/html\r\n\r\nHello";
  check((strcmp((char *) write_buffer, expected3) == 0), "output is correct");

  done();
}
