# tool macros
CC ?= gcc
WARNINGS := -Wall -Wextra -Werror
CFLAGS ?= $(WARNINGS) -g -O2 -fsanitize=address
LIBFLAGS ?= -I./lib
STD ?= -std=c2x
DBGFLAGS := $(WARNINGS) -g -O0
TESTFLAGS ?= -DUNITY_TEST_RUN
COBJFLAGS := -c $(LIBFLAGS) $(STD)
TEST_COBJFLAGS := $(LIBFLAGS) $(STD)

# path macros
BUILD_PATH := ./build
SRC_PATH := ./src
LIB_PATH := ./lib
OBJ_PATH := ./build/obj
DBG_PATH := ./build/dbg
TEST_PATH := ./tests
TEST_GEN_PATH := ./tests/generated
SCRIPTS_PATH := ./scripts

# compile macros
TARGET_NAME := gb_emulator
TARGET := $(BUILD_PATH)/$(TARGET_NAME)
TARGET_DBG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(wildcard $(SRC_PATH)/*.c)
SRC_EXCEPT_MAIN := $(filter-out $(SRC_PATH)/main.c, $(SRC))
SRC_TEST_MANUAL := $(wildcard $(TEST_PATH)/*.c)
SRC_TEST_MANUAL := $(filter-out $(TEST_PATH)/unity.c, $(SRC_TEST_MANUAL))
SRC_TEST_GEN := $(wildcard $(TEST_GEN_PATH)/*.c)

# Unity test runner
# convert tests/test_*.c to build/test_*
TARGET_TEST_MANUAL := $(patsubst $(TEST_PATH)/%.c, $(BUILD_PATH)/%, $(SRC_TEST_MANUAL))
TARGET_TEST_GEN := $(patsubst $(TEST_GEN_PATH)/%.c, $(BUILD_PATH)/generated/%, $(SRC_TEST_GEN))
GEN_RUNNER := $(SCRIPTS_PATH)/unity/generate_test_runner.rb

# Generate object lists
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
CLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG) \
				  $(TARGET_TEST_MANUAL) \
				  $(wildcard $(BUILD_PATH)/*_Runner.c) \
              	  $(wildcard $(BUILD_PATH)/generated/*) \
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
test: $(TARGET_TEST_MANUAL) $(TARGET_TEST_GEN)
	@echo "--- ALL TESTS PASSED ---"

# RULE A: Manual Tests
# Pattern: build/% corresponds to tests/%.c
$(TARGET_TEST_MANUAL): $(BUILD_PATH)/%: $(TEST_PATH)/%.c $(SRC_EXCEPT_MAIN) $(TEST_PATH)/unity.c
	@mkdir -p $(BUILD_PATH)
	# 1. Generate Runner (Manual)
	ruby $(GEN_RUNNER) $< $(BUILD_PATH)/$*_Runner.c
	
	# 2. Compile
	$(CC) $(CFLAGS) -o $@ \
		$< \
		$(BUILD_PATH)/$*_Runner.c \
		$(TEST_PATH)/unity.c \
		$(SRC_EXCEPT_MAIN) \
		-I$(SRC_PATH) -I$(TEST_PATH) $(LIBFLAGS)
	
	# 3. Run
	./$@

# RULE B: Generated Tests
# Pattern: build/generated/% corresponds to tests/generated/%.c
$(TARGET_TEST_GEN): $(BUILD_PATH)/generated/%: $(TEST_GEN_PATH)/%.c $(SRC_EXCEPT_MAIN) $(TEST_PATH)/unity.c
	@mkdir -p $(BUILD_PATH)/generated
	# 1. Generate Runner (Generated)
	#    We place the runner in build/generated/ to keep it clean
	ruby $(GEN_RUNNER) $< $(BUILD_PATH)/generated/$*_Runner.c
	
	# 2. Compile
	#    Note: Added -I$(TEST_GEN_PATH) so generated tests can find headers if needed
	$(CC) $(CFLAGS) -o $@ \
		$< \
		$(BUILD_PATH)/generated/$*_Runner.c \
		$(TEST_PATH)/unity.c \
		$(SRC_EXCEPT_MAIN) \
		-I$(SRC_PATH) -I$(TEST_PATH) $(LIBFLAGS) $(TESTFLAGS)
	
	# 3. Run
	./$@
