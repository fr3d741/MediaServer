TARGET_EXEC := media_server

-include ./MediaServer/conanbuildinfo.mak

BUILD_DIR := bin
SRC_DIRS := .
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d | sed -e '/\.\/\./d')
INC_DIRS += $(CONAN_INCLUDE_DIRS)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP -g
CXXFLAGS := $(CONAN_CXXFLAGS) -std=c++20 -g

CFLAGS              += -g $(CONAN_CFLAGS)
LDFLAGS             += -g
CPPFLAGS            += $(addprefix -D, $(CONAN_DEFINES))
LDFLAGS             += $(addprefix -L, $(CONAN_LIB_DIRS))
LDLIBS              += $(addprefix -l, $(CONAN_LIBS))
EXELINKFLAGS        += $(CONAN_EXELINKFLAGS)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS) 
