CC := gcc

SRCS := main.c server.c router.c

OBJS := $(SRCS:.c=.o)

TARGET := bin/http_server

UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
	# Linux specific flags
	CFLAGS := -Wall -Wextra
	LDFLAGS :=
else ifeq ($(UNAME), Darwin)
	# macOS specific flags
	CFLAGS := -Wall -Wextra
	LDFLAGS :=
else ifeq ($(UNAME), Windows_NT)
	# Windows specific flags
	CC := gcc.exe
	CFLAGS := -Wall -Wextra
	LDFLAGS :=
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean