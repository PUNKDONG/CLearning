.PHONY: debug release print clean

CC = gcc
COMMON_CFLAGS = -Wall -Wextra -std=c11
SRCS = main.c math_utils.c

DEBUG_CFLAGS = $(COMMON_CFLAGS) -g -O0
RELEASE_CFLAGS = $(COMMON_CFLAGS) -O2 -DNDEBUG

DEBUG_TARGET = out_debug
RELEASE_TARGET = out_release

# Default view for `make print`
CFLAGS = $(DEBUG_CFLAGS)
TARGET = $(DEBUG_TARGET)

DEBUG_OBJS = build/debug/main.o build/debug/math_utils.o
RELEASE_OBJS = build/release/main.o build/release/math_utils.o

debug: CFLAGS = $(DEBUG_CFLAGS)
debug: TARGET = $(DEBUG_TARGET)
debug: $(DEBUG_TARGET)

release: CFLAGS = $(RELEASE_CFLAGS)
release: TARGET = $(RELEASE_TARGET)
release: $(RELEASE_TARGET)

$(DEBUG_TARGET): $(DEBUG_OBJS)
	$(CC) $(CFLAGS) $(DEBUG_OBJS) -o $(DEBUG_TARGET)

$(RELEASE_TARGET): $(RELEASE_OBJS)
	$(CC) $(CFLAGS) $(RELEASE_OBJS) -o $(RELEASE_TARGET)

build/debug/%.o: %.c math_utils.h
	mkdir -p build/debug
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

build/release/%.o: %.c math_utils.h
	mkdir -p build/release
	$(CC) $(RELEASE_CFLAGS) -c $< -o $@

print:
	@echo "CC=$(CC)"
	@echo "CFLAGS=$(CFLAGS)"
	@echo "TARGET=$(TARGET)"

clean:
	rm -rf build $(DEBUG_TARGET) $(RELEASE_TARGET)
