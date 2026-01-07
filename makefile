# tool macros
CC ?= gcc
WARNINGS := -Wall -Wextra -Werror
CFLAGS ?= $(WARNINGS) -g -O2
LIBFLAGS ?= -I./lib
STD ?= -std=c2x
DBGFLAGS := $(WARNINGS) -g -O0
COBJFLAGS := -c $(LIBFLAGS) $(STD)
TEST_COBJFLAGS := $(LIBFLAGS) $(STD)

# path macros
BUILD_PATH := ./build
SRC_PATH := ./src
LIB_PATH := ./lib
OBJ_PATH := ./build/obj
DBG_PATH := ./build/dbg
TEST_PATH := ./tests
SCRIPTS_PATH := ./scripts

# compile macros
TARGET_NAME := gb_emulator
TARGET := $(BUILD_PATH)/$(TARGET_NAME)
TARGET_DBG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(wildcard $(SRC_PATH)/*.c)
SRC_EXCEPT_MAIN := $(filter-out $(SRC_PATH)/main.c, $(SRC))
SRC_TEST := $(wildcard $(TEST_PATH)/*.c)
SRC_TEST_EXCEPT_UNITY := $(filter-out $(TEST_PATH)/unity.c, $(SRC_TEST))

# Unity test runner
# convert tests/test_*.c to build/test_*
TARGET_TESTS := $(patsubst $(TEST_PATH)/%.c, $(BUILD_PATH)/%, $(SRC_TEST_EXCEPT_UNITY))
GEN_RUNNER := $(SCRIPTS_PATH)/unity/generate_test_runner.rb

# Generate object lists
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
CLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG) \
				  $(TARGET_TESTS) \
				  $(wildcard $(BUILD_PATH)/*_Runner.c) \
				  $(OBJ_PATH)/*.o
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
test: $(TARGET_TESTS)
	@echo "--- ALL TESTS PASSED ---"

# To build 'build/test_alu', we need:
#   1. The original test file (test_alu.c)
#   2. The generated runner (test_alu_Runner.c)
#   3. Unity
#   4. The Emulator Core
$(BUILD_PATH)/%: $(TEST_PATH)/%.c $(SRC_EXCEPT_MAIN)
	@mkdir -p $(BUILD_PATH)

	# 1. Generate the Runner C file using Ruby
	ruby $(GEN_RUNNER) $< $(BUILD_PATH)/$*_Runner.c
	
	# 2. Compile everything together
	gcc -I$(SRC_PATH) -I$(TEST_PATH) \
	$(TEST_COBJFLAGS) $(CFLAGS) \
		-o $@ $< \
		$(BUILD_PATH)/$*_Runner.c \
		$(TEST_PATH)/unity.c \
		$(SRC_EXCEPT_MAIN)
		
	# 3. Run it immediately
	./$@
