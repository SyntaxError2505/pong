TARGET := pong
CC := cc
SOURCE = $(shell find . -type f -name "*.c")
CFLAGS = -std=c23 -O2 -Wall -Wextra $(shell pkgconf --cflags sdl3)
LDFLAGS = $(shell pkgconf --libs sdl3) -lm

default: $(TARGET)

clean:
	rm -f $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(SOURCE)

format: $(SOURCE)
	clang-format -i -style=file $(SOURCE)

.PHONY: default clean format
