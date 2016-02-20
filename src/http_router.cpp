#include <string.h>

#include "http_router.h"

Router *router_create ( ) {
  static Router router;

  memset(&router, 0, sizeof(Router));

  return &router;
}

uint8_t router_add_route (Router *router, uint8_t *route, void (*method)(Request *, Response *)) {
  // only add as many routes as there are room
  if (router->num_routes >= MAX_ROUTES) {
    return 0;
  }

  if (strlen((char *) route) >= MAX_ROUTE_LENGTH) {
    return 0;
  }

  memcpy(router->routes[router->num_routes].route, route, strlen((char *) route));
  router->routes[router->num_routes].method = method;

  router->num_routes++;

  return 1;
}

static void four_oh_four (Request *request, Response *response) {
  response->code = 404;
  response_write(response, (uint8_t *) "404 Error");
}

static void five_hundred (Request *request, Response *response) {
  response->code = 500;
  response_write(response, (uint8_t *) "500 Error");
}

static int8_t find_route (Router *router, uint8_t *route) {
  uint8_t found = -1;

  for (uint8_t i = 0; i < router->num_routes; i++) {
    if (strcmp((char *) route, (char *) router->routes[i].route) == 0) {
      found = i;
      break;
    }
  }

  return found;
}


#ifdef ARDUINO_ARCH_ESP8266
void route (WiFiClient socket, Router *router, uint8_t *req) {
#else
void route (uint8_t socket, Router *router, uint8_t *req) {
#endif
  Request *request;
  Response *response;

  request = parse_request(req);
  response = response_create(socket);

  if (router->write) {
    response->write = router->write;
  }

  if (request->method == HTTP_ERROR) {
    five_hundred(request, response);
  } else {
    int8_t found = find_route(router, request->path);

    if (found != -1) {
      router->routes[found].method(request, response);
    } else {
      four_oh_four(request, response);
    }
  }
}
