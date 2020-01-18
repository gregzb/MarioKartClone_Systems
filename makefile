TARGET_EXEC ?= kart

BUILD_DIR ?= ./out
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
#$(pkg-config --cflags --libs sdl2)
#-Wall
LDFLAGS ?= -lm $(shell pkg-config --libs --cflags sdl2) $(shell pkg-config --libs --cflags SDL2_ttf) -std=gnu11
CFLAGS = -Wall

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -ggdb3 -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c clean
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -ggdb3 -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean run auto

clean:
	-$(RM) -r $(BUILD_DIR)
	-$(RM) -r .vscode

run:
	./$(BUILD_DIR)/$(TARGET_EXEC)

auto: $(BUILD_DIR)/$(TARGET_EXEC)
	./$(BUILD_DIR)/$(TARGET_EXEC)

memcheck:
	valgrind --leak-check=yes ./$(BUILD_DIR)/$(TARGET_EXEC)

memcheck_full:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BUILD_DIR)/$(TARGET_EXEC)

bear: clean
	make -B

install:
	sudo apt install pkg-config libsdl2-dev libsdl2-ttf-dev

-include $(DEPS)

MKDIR_P ?= mkdir -p
