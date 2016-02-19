#include <stdint.h>
#include <stdlib.h>

#include "http_request.h"

Request *parse_request (uint8_t *request) {
  static Request req = {0};
  uint16_t position = 0;


  // start iterating through the request, break out the parts.
  // this is clunky, but fast
  if (request[0] == 'G' && request[1] == 'E' && request[2] == 'T' && request[3] == ' ') {
    position = 4;
    req.method = HTTP_GET;
  } else if (request[0] == 'P') {
    if (request[1] == 'O' && request[2] == 'S' && request[3] == 'T' && request[4] == ' ') {
      position = 5;
      req.method = HTTP_POST;
    } else if (request[1] == 'U' && request[2] == 'T' && request[3] == ' ') {
      position = 4;
      req.method = HTTP_PUT;
    } else {
      req.method = HTTP_ERROR;
      return &req;
    }
  } else {
    req.method = HTTP_ERROR;
    return &req;
  }

  // set the uri to the current position, now that we have the method
  uint8_t *uri = &request[position];


  while (request[position] != '\0' && request[position] != ' ' && request[position] != '\n' && request[position] != '\r') {
    position++;
  }

  // if we're already at the end, we can consider this an invalid request
  if (request[position] == '\0') {
    req.method = HTTP_ERROR;

    return &req;
  }

  // set the uri by terminating the string where we left off
  request[position] = '\0';

  req.path = uri;

  // and set up the params
  req.params = parse_params(uri);

  position++;

  while (request[position] != '\n' && request[position] != '\r' && request[position] != '\0') {
    position++;
  }

  if (request[position] == '\0') {
    return &req;
  }

  request[position] = '\0';

  position++;

  // move past the LF
  if (request[position + 1] == '\r' || request[position + 1] == '\n') {
    position++;
  }

  position++;

  if (request[position] == '\0') {
    return &req;
  }

  req.headers = parse_headers(&request[position]);

  return &req;
}

uint8_t **parse_params (uint8_t *uri) {
  static uint8_t *params[MAX_PARAMS + 1];
  uint8_t current = 0;

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
      return NULL;
    }
  } else {
    // at the end, no params return NULL
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

uint8_t **parse_headers (uint8_t *request) {
  static uint8_t *headers[MAX_HEADERS + 1];
  uint16_t current = 0;

  if (request[0] == '\0') {
    return NULL;
  }

  uint16_t position = 0;

  headers[current] = request;
  current++;

  while (request[position] != '\0') {
    if (request[position] == '\n' || request[position] == '\r') {
      request[position] = '\0';
      position++;

      if (request[position] == '\r' || request[position] == '\n') {
        position++;
      }

      if (request[position] != '\0') {
        headers[current] = &request[position];
        current++;
      }
    }

    if (request[position] == '\0') {
      break;
    }

    position++;
  }

  headers[current] = NULL;

  return headers;
}
