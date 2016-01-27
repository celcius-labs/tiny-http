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

  // set up some sane defaults
  ret->host = NULL;
  ret->params = NULL;
  ret->body = NULL;

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

  ret->path = uri;

  // if we still haven't seen our first CRLF, keep going
  if (crlf == 0) {
    while (request[position] != '\n') {
      position++;
    }
  }

  // move past the LF
  if (request[position] == '\r') {
    position++;
  }



  return ret;
}

uint8_t **parse_params (uint8_t *uri) {
  uint8_t **params = (uint8_t **) malloc(sizeof(uint8_t **) * (MAX_PARAMS + 1));
  uint8_t current = 0;

  if (params == NULL) {
    // out of memory
    return NULL;
  }

  // set a default
  params[0] = NULL;

  uint16_t position = 0;

  // figure out where the params start
  while (uri[position] != '\0' && uri[position] != '?') {
    position++;
  }

  if (uri[position] == '?') {
    uri[position] = '\0';
    position++;

    if (uri[position] == '\0') {
      // uri ends with ? and nothing further
      free(params);
      return NULL;
    }
  } else {
    // at the end, no params, free everything up and return NULL
    free(params);
    return NULL;
  }

  // assign the first parameter and move forward
  params[current] = &uri[position];
  current++;

  while (uri[position] != '\0') {
    if (uri[position] == '&') {
      uri[position] = '\0';

      params[current] = &uri[position + 1];
      current++;
      if (uri[position + 1] == '\0') {
        break;
      }
    }

    position++;
  }

  params[current] = NULL;

  return params;
}
