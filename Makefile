CC            = gcc
DEBUG_FLAGS   = -O0 -g -fsanitize=address
RELEASE_FLAGS = -O2
CFLAGS        = -std=c99 -Iinclude
LDLIBS        = -lcurses

PREFIX      ?= /usr/local
INSTALL_DIR  = $(PREFIX)/bin
BUILD_DIR    = build
BIN_DIR      = bin
RENDER_OBJ   = $(BUILD_DIR)/render.o
PHYSICS_OBJ  = $(BUILD_DIR)/physics.o
RAIN_OBJ     = $(BUILD_DIR)/crain.o
UTILS_OBJ    = $(BUILD_DIR)/utils.o

TARGET       = $(BIN_DIR)/crain
RENDER_SRC   = src/render.c
PHYSICS_SRC  = src/physics.c
RAIN_SRC     = src/crain.c
UTILS_SRC    = src/utils.c

BUILD_TYPE  ?= debug

ifeq ($(BUILD_TYPE), debug)
	CFLAGS += $(DEBUG_FLAGS)
else ifeq ($(BUILD_TYPE), release)
	CFLAGS += $(RELEASE_FLAGS)
else
	$(error Unknown build type: $(BUILD_TYPE))
endif

.PHONY: all clean install uninstall

all: $(BUILD_DIR) $(BIN_DIR) $(TARGET)

install: $(BUILD_DIR) $(BIN_DIR) $(TARGET)
	install -m755 $(BIN_DIR)/crain $(INSTALL_DIR)/crain

uninstall:
	rm -f $(INSTALL_DIR)/crain

$(TARGET) : $(UTILS_OBJ) $(RENDER_OBJ) $(PHYSICS_OBJ) $(RAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) :
	mkdir -p $@

$(BIN_DIR) :
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(BUILD_DIR)/*
