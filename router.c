#include "router.h"

#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *path;
    const char *content_type;
    const char *content;
} route_t;

static route_t *routes = NULL;
static size_t routes_count = 0;

static void read_file(const char *path, char **content) {
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        *content = NULL;
        return;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    *content = (char *)malloc(length + 1);
    if (*content == NULL) {
        fclose(file);
        return;
    }

    fread(*content, 1, length, file);
    (*content)[length] = '\0';

    fclose(file);
}

void load_routes(const char *directory) {
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
        char *content;
        read_file(path, &content);
        if (content != NULL) {
            routes = (route_t *)realloc(routes, (routes_count + 1) * sizeof(route_t));
            routes[routes_count].path = strdup(entry->d_name);
            // Get the file extension
            const char *extension = strrchr(entry->d_name, '.');
            if (extension != NULL) {
                // Check the file extension and set the content type accordingly
                if (strcmp(extension, ".html") == 0) {
                    routes[routes_count].content_type = "text/html";
                } else if (strcmp(extension, ".css") == 0) {
                    routes[routes_count].content_type = "text/css";
                } else if (strcmp(extension, ".js") == 0) {
                    routes[routes_count].content_type = "application/javascript";
                } else {
                    routes[routes_count].content_type = "text/plain";
                }
            } else {
                routes[routes_count].content_type = "text/plain";
            }
            routes[routes_count].content = content;
            routes_count++;
        }
    }
    closedir(dir);
}

static const route_t *find_route(const char *path) {
    for (size_t i = 0; i < routes_count; i++) {
        if (strcmp(routes[i].path, path) == 0) {
            printf("found route: %s\n", routes[i].path);
            return &routes[i];
        }
    }
    return NULL;
}

void handle_request(const char *path, char *response, size_t response_size) {
    const route_t *route = find_route(path);
    if (route == NULL) {
        printf("route not found\n");
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