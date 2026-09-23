TERMUOS_ROOT ?= $(abspath ../../..)

CXX := g++
CC  := gcc

CFLAGS := -static -nostdlib -no-pie -ffreestanding \
          -fno-stack-protector -fno-exceptions -fno-rtti \
          -fno-asynchronous-unwind-tables -fcf-protection=none -O2 \
          -I$(TERMUOS_ROOT)/tsys/lib/include \
          -Isrc \
          -Isrc/widgets \
          -Isrc/desktop \
          -Isrc/desktop/startmenu \
          -Isrc/apps

CXXFLAGS := $(CFLAGS) -std=c++20

CRT0 := $(TERMUOS_ROOT)/tsys/lib/crt0.S
LIB  := $(TERMUOS_ROOT)/kbuild/tsys/libtsys.a

# Skip icon.c for now (Limine). Add cxxstub for operator delete.
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
	src/apps/terminal.cpp


OBJS := $(CPPSRCS:.cpp=.o)
OUT  := luna.tsys

.PHONY: all clean

all: $(OUT)

src/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT): $(OBJS) $(CRT0) $(LIB)
	$(CXX) $(CFLAGS) -o $(OUT) $(CRT0) $(OBJS) $(LIB)

clean:
	rm -f $(OBJS) $(OUT)