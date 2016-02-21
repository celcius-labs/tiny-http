# Tiny-HTTP

A super tiny HTTP server toolkit targeting embedded devices.

## Goals

* Tiny (small memory footprint)
* Fast (small CPU footprint)
* No Heap Usage (uses no run-time memory allocation)
* Modular (optional middleware)
* Well Tested

## Current Status

* Parse method
* Parse path
* Parse parameters
* Parse headers
* Lots of tests
* Responses
* Routing Engine

## Using

The HTTP Parser is stable, and can be used as-is.  It will parse the request
into a `method`, `path`, `parameters`, and `headers`.

It also has the ability to manage responses, as well as a built-in request
router that makes it easier to build applications with.

`Tiny-HTTP` is opinionated, and uses unsigned 8 bit integers in the form of
`uint8_t`.  It is also meant to work in as many environments as possible, and
thus has a more `C` style to it.  It is currently usable on any POSIX
compatible system (Unix/Linux/OSX), as well as the ESP8266 when using the
Arduino environment.


### Building

```
$ make
```

### Testing

```
$ make test
```

### Embedding

To use, simply include the aspects of the project that you want to use,
`Request`, `Response`, and `Routing`.

From there, link against `libtinyhttp.a`:

```
$ gcc httpd.c -ltinyhttp -o httpd
```

### Request

A `Request` contains a `method`, `path`, `parameters`, and `headers`.  If an
error occurs, or an unsupported method is sent, `method` is set to `HTTP_ERROR`.

* `method` - `uint8_t` method (`HTTP_GET`, `HTTP_POST`, `HTTP_PUT`, `HTTP_ERROR`)
* `path` - `uint8_t *` path or NULL on error
* `params` - `uint8_t **` array of parameters NULL terminated (`foo=bar`)
* `headers` - `uint8_t **` array of headers NULL terminated (`Content-type: application/json`)


#### parse_request(uint8_t \*req)

Parses a request, returning a `Request` object.

_param_ req - `uint8_t *` buffer containing the full request
_returns_ Request - `Request *` request that contains a `Request` object.

_example_:

```
uint8_t buf[128] = "GET /foo?bar=baz HTTP/1.1\r\n\r\n";

Request *request = http_parser(buf);

Serial.println((char *) request->path); // /foo
Serial.println((char *) request->params[0]); // bar=baz
```

### Response

A Response allows for setting of headers, response codes, and writing of
content back to the browser.

#### response_create

Creates a `Response` that should be passed to all subsequent `request` calls.

_Parameters_:

* socket - uint8_t (POSIX)
* client - WiFiClient (ESP8266)

_Example:_

```
Response *response = response_create(client);
```

#### response->code

One of `200`, `404`, or `500` (defaults to `200`).  Content type cannot be
set once `response_write` has been run.

_Example:_

```
response->code = 404;
```

#### response_set_content_type

Sets the `Content-type` header - defaults to `text/html`.  Content type cannot
be set once `response_write` has been run.

_Parameters:_

* response - pointer to Response object
* type - type of content to send

_Example:_

```
response_set_content_type(response, "application/json");
```

#### response_set_header

Sets a header.  Headers cannot be set once `response_write` has been run.

_Parameters:_

* response - pointer to Response object
* header - well-formed header to send

_Example:_

```
response_set_header(response, "X-Powered-By: Awesomeness");
```

#### response_write

Sends data to the client.  Once a `response_write` has been run, no more headers
can be set, this starts sending data to the client immediately, as well as
the content type and status.

_Parameters:_

* response - pointer to Response object
* data - data to send

_Example:_

```
response_write(response, "Pong!");
```

### Router

The request router manages route state, as well as turning control over to
a method to execute.  Once a router is set up, it is simply a matter of
sending the whole request string to the router, and letting the router do
all of the work for you.

Only one route will be matched, and a `500` error will be returned on error
parsing a request, as well as a `404` error if no routes are found.

#### router_create

Returns a new `Router`.

_Example:_

```
Router *router = router_create();
```

#### router_add_route

Adds a route to the router for dispensation.

_Parameters:_

* router - pointer to Router object
* route - route to match against
* method - function to call when the route is matched

_Example:_

```
void pong_responder (Request *request, Response *response) {
  response_set_content_type("application/json");
  response_write("{ \"status\": \"pong\" }");
}

Router *setup_routes ( ) {
  Router *router = router_create();

  router_add_route(router, "/ping", ping_responder);

  return router;
}

void run_routes (Router *router, WiFiClient client, uint8_t *req) {
  route(client, router, req);
}
```
