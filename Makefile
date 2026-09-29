TERMUOS_ROOT ?= $(abspath ../../..)

TLIBC_DIR ?= $(TERMUOS_ROOT)/usr/lib/tlibc
TLIBC_A   ?= $(TLIBC_DIR)/libtlibc.a
TLIBC_INC ?= -I$(TLIBC_DIR)/include

TSYS_LIB  ?= $(TERMUOS_ROOT)/kbuild/tsys/libtsys.a
CRT0      ?= $(TERMUOS_ROOT)/tsys/lib/crt0.S

CXX := g++
CC  := gcc

CFLAGS := -static -nostdlib -no-pie -ffreestanding \
          -fno-stack-protector -fno-exceptions -fno-rtti \
          -fno-asynchronous-unwind-tables -fcf-protection=none -O2 \
          $(TLIBC_INC) \
          -I$(TERMUOS_ROOT)/tsys/lib/include \
          -Isrc \
          -Isrc/widgets \
          -Isrc/desktop \
          -Isrc/desktop/startmenu \
          -Isrc/apps

CXXFLAGS := $(CFLAGS) -std=c++20

CPPSRCS := \
	src/luna.cpp \
	src/focus.cpp \
	src/cxxstub.cpp \
	src/widgets/gfx.cpp \
	src/widgets/window.cpp \
	src/widgets/button.cpp \
	src/widgets/textfield.cpp \
	src/desktop/startmenu/startmenu.cpp \
	src/apps/about.cpp \
	src/apps/registry.cpp \
	src/apps/settings.cpp \
	src/apps/widgets.cpp \
	src/apps/explorer.cpp

OBJS := $(CPPSRCS:.cpp=.o)
OUT  := luna.tsys

.PHONY: all clean

all: $(OUT)

$(TLIBC_A):
	$(MAKE) -C $(TLIBC_DIR)

src/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT): $(OBJS) $(TLIBC_A) $(TSYS_LIB)
	$(CXX) $(CFLAGS) -o $(OUT) $(CRT0) $(OBJS) $(TLIBC_A) $(TSYS_LIB)

clean:
	rm -f $(OBJS) $(OUT)