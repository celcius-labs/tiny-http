#pragma once

#include <stdint.h>

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#endif

#include "http_response.h"
#include "http_request.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAX_ROUTE_LENGTH
#define MAX_ROUTE_LENGTH 32
#endif

#ifndef MAX_ROUTES
#define MAX_ROUTES 16
#endif

typedef struct Route {
  uint8_t route[MAX_ROUTE_LENGTH];
  void (*method)(Request *, Response *);
} Route;

typedef struct Router {
  Route routes[MAX_ROUTES];
  uint8_t num_routes;
  void (*write)(uint8_t *);
} Router;


Router *router_create ( );
uint8_t router_add_route (Router *, uint8_t *, void (*)(Request *, Response *));

#ifdef ARDUINO_ARCH_ESP8266
void route (WiFiClient, Router *, uint8_t *);
#else
void route (uint8_t, Router *, uint8_t *);
#endif

#ifdef __cplusplus
}
#endif
