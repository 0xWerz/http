#ifndef ROUTER_H
#define ROUTER_H

#include <stddef.h>

void handle_request(const char *path, char *response, size_t response_size);
void load_routes(const char *directory);

#endif