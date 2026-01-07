# tool macros
CC ?= gcc
WARNINGS := -Wall -Wextra -Werror
CFLAGS ?= $(WARNINGS) -g -O2
LIBFLAGS ?= -I./lib
STD ?= -std=c2x
DBGFLAGS := $(WARNINGS) -g -O0
COBJFLAGS := -c $(LIBFLAGS) $(STD)

# path macros
BIN_PATH := ./build
SRC_PATH := ./src
LIB_PATH := ./lib
OBJ_PATH := ./build/obj
DBG_PATH := ./build/dbg
TEST_PATH := ./tests

# compile macros
TARGET_NAME := gb_emulator
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DBG := $(DBG_PATH)/$(TARGET_NAME)
TARGET_TEST := $(BIN_PATH)/$(TARGET_NAME)_test

# src files & obj files
SRC := $(wildcard $(SRC_PATH)/*.c)
SRC_EXCEPT_MAIN := $(filter-out $(SRC_PATH)/main.c, $(SRC))
SRC_TEST := $(wildcard $(TEST_PATH)/*.c)

# Generate object lists
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_TEST := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC_TEST))))) $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC_EXCEPT_MAIN)))))

# clean files list
CLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG) \
				  $(OBJ_TEST)
DISTCLEAN_LIST := $(TARGET) \
              $(TARGET_DBG) \
              $(CLEAN_LIST)

# default rule
default: build

# non-phony targets
$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

# Release Object Rule
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(COBJFLAGS) $(CFLAGS) -o $@ $<

# Debug Object Rule
$(DBG_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(COBJFLAGS) $(DBGFLAGS) -o $@ $<

# Test Object Rule
$(OBJ_PATH)/%.o: $(TEST_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(COBJFLAGS) $(CFLAGS) -o $@ $<

# Debug Binary Rule
$(TARGET_DBG): $(OBJ_DEBUG)
	@mkdir -p $(@D)
	$(CC) $(DBGFLAGS) $(OBJ_DEBUG) -o $@

# Test Binary Rule
$(TARGET_TEST): $(OBJ_TEST)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OBJ_TEST) -o $@

# phony rules
.PHONY: build
build: $(TARGET)

.PHONY: run
run: $(TARGET)
	@echo RUN $(TARGET)
	@$(TARGET)

.PHONY: debug
debug: $(TARGET_DBG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)

.PHONY: test
test: $(TARGET_TEST)
	@echo RUN TESTS $(TARGET_TEST)
	@$(TARGET_TEST)

