#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "http_parser.h"

Request *parse_request (uint8_t *request) {
  Request *ret;
  uint16_t position = 0;

  ret = (Request *) malloc(sizeof(Request));

  // out of memory!
  if (ret == NULL) {
    return NULL;
  }


  // start iterating through the request, break out the parts.
  // this is clunky, but fast
  if (request[0] == 'G' && request[1] == 'E' && request[2] == 'T' && request[3] == ' ') {
    position = 4;
    ret->method = HTTP_GET;
  } else if (request[0] == 'P') {
    if (request[1] == 'O' && request[2] == 'S' && request[3] == 'T' && request[4] == ' ') {
      position = 5;
      ret->method = HTTP_POST;
    } else if (request[1] == 'U' && request[2] == 'T' && request[3] == ' ') {
      position = 4;
      ret->method = HTTP_PUT;
    } else {
      ret->method = HTTP_ERROR;
      return ret;
    }
  } else {
    ret->method = HTTP_ERROR;
    return ret;
  }

  // set the uri to the current position, now that we have the method
  uint8_t *uri = &request[position];

  // continue until the first space, this signifies the completion of the URI
  // keep track of how many crlf's have been seen, as if the request doesn't
  // include http version, we'll need to know this
  uint8_t crlf = 0;

  while (request[position] != '\0' && request[position] != ' ' && request[position] != '\n' && request[position] != '\r') {
    if (request[position] == '\n') {
      crlf++;
    }

    position++;
  }

  // if we're already at the end, we can consider this an invalid request
  if (request[position] == '\0') {
    ret->method = HTTP_ERROR;

    return ret;
  }

  // set the uri by terminating the string where we left off
  request[position] = '\0';

  ret->uri = uri;

  return ret;
}
