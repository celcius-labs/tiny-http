#include <stdint.h>
#include <stdlib.h>

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

  return ret;
}
