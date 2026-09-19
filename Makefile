TERMUOS_ROOT ?= $(abspath ../../..)

TSYS_CC      := gcc
TSYS_CFLAGS  := -static -nostdlib -no-pie -ffreestanding \
                -fno-stack-protector -fno-asynchronous-unwind-tables \
                -fcf-protection=none -O2 \
                -I$(TERMUOS_ROOT)/tsys/lib/include

TSYS_CRT0    := $(TERMUOS_ROOT)/tsys/lib/crt0.S
TSYS_LIB_A   := $(TERMUOS_ROOT)/kbuild/tsys/libtsys.a

OUT          := luna.tsys

.PHONY: all clean

all: $(OUT)

$(OUT): src/main.c $(TSYS_CRT0)
	$(TSYS_CC) $(TSYS_CFLAGS) -o $(OUT) $(TSYS_CRT0) src/main.c $(TSYS_LIB_A)

clean:
	rm -f $(OUT)