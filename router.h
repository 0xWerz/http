#include <stddef.h>

#ifndef ROUTER_H
#define ROUTER_H

void handle_request(const char *path, char *response, size_t response_size);

#endif
