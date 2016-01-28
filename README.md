# Tiny-HTTP

A super tiny HTTP server toolkit targeting embedded devices.

## Goals

* Tiny (small memory footprint)
* Fast (small CPU footprint)
* No Heap Usage
* Modular (optional middleware)
* Well Tested

## Current Status

* Parse method
* Parse path
* Parse parameters
* Parse headers
* Lots of tests

## Using

The HTTP Parser is stable, and can be used as-is.  It will parse the request
into a `method`, `path`, `parameters`, and `headers`.

`Tiny-HTTP` is opinionated, and uses unsigned 8 bit integers in the form of
`uint8_t`.  It is also meant to work in as many environments as possible, and
thus has a more `C` style to it.
### Request

A `Request` contains a `method`, `path`, `parameters`, and `headers`.  If an
error occurs, or an unsupported method is sent, `method` is set to `HTTP_ERROR`.

* `method` - `uint8_t` method (`HTTP_GET`, `HTTP_POST`, `HTTP_PUT`, `HTTP_ERROR`)
* `path` - `uint8_t *` path or NULL on error
* `params` - `uint8_t **` array of parameters NULL terminated (`foo=bar`)
* `headers` - `uint8_t **` array of headers NULL terminated (`Content-type: application/json`)


### parse_request(uint8_t \*req)

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
