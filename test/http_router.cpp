#include <stdlib.h>
#include <string.h>

#include "test.h"

#include "http_response.h"
#include "http_router.h"

static uint8_t write_buffer[1024] = { 0 };

static void reset_write ( ) {
  write_buffer[0] = '\0';
}

static void test_write (uint8_t *buf) {
  strcat((char *) write_buffer, (char *) buf);
}

static void foo_route (Request *request, Response *response) {
  response->write = test_write;

  response_write(response, (uint8_t *) "Hello");
}

uint8_t test_routing ( ) {
  Router *router;
  uint8_t req[255];

  reset_write();

  router = router_create();

  router_add_route(router, (uint8_t *) "/foo", foo_route);
  strcpy((char *) req, "GET /foo\n\r\n\r");
  route(0, router, req);

  const char *expected = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\nHello";
  check((strcmp((char *) write_buffer, expected) == 0), "routing is correct");

  done();
}
