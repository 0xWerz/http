#include "router.h"

#include <stdio.h>
#include <string.h>

typedef struct {
    const char *path;
    const char *content_type;
    const char *content;
} route_t;

static const route_t ROUTES[] = {
    {"/", "text/html", "<h1>Hello, World!</h1>"},
    {"/about", "text/html", "<h1>About</h1><p>This is an example HTTP server.</p>"},
    {"/contact", "text/html", "<h1>Contact</h1><p>Please contact us at example@example.com.</p>"},
};

static const route_t *find_route(const char *path) {
    for (size_t i = 0; i < sizeof(ROUTES) / sizeof(route_t); i++) {
        if (strcmp(ROUTES[i].path, path) == 0) {
            return &ROUTES[i];
        }
    }
    return NULL;
}

void handle_request(const char *path, char *response, size_t response_size) {
    const route_t *route = find_route(path);
    if (route == NULL) {
        snprintf(response, response_size,
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "Content-Length: 9\r\n"
                 "\r\n"
                 "Not Found");
    } else {
        snprintf(response, response_size,
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: %s\r\n"
                 "Content-Length: %zu\r\n"
                 "\r\n"
                 "%s",
                 route->content_type, strlen(route->content), route->content);
    }
}
