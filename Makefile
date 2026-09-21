TERMUOS_ROOT ?= $(abspath ../../..)

CXX := g++
CFLAGS := -static -nostdlib -no-pie -ffreestanding \
          -fno-stack-protector -fno-exceptions -fno-rtti \
          -fno-asynchronous-unwind-tables -fcf-protection=none -O2 \
          -I$(TERMUOS_ROOT)/tsys/lib/include -Isrc
CXXFLAGS := $(CFLAGS) -std=c++20

CRT0 := $(TERMUOS_ROOT)/tsys/lib/crt0.S
LIB  := $(TERMUOS_ROOT)/kbuild/tsys/libtsys.a

OBJS := src/main.o src/gfx.o src/window.o src/focus.o src/cursor.o src/cxxstub.o
OUT  := luna.tsys

.PHONY: all clean

all: $(OUT)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT): $(OBJS) $(CRT0) $(LIB)
	$(CXX) $(CFLAGS) -o $(OUT) $(CRT0) $(OBJS) $(LIB)

clean:
	rm -f $(OBJS) $(OUT)