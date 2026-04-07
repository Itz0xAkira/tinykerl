CC      = i686-elf-gcc
AS      = nasm
LD      = i686-elf-gcc

CFLAGS  = -m32 -ffreestanding -O2 -Wall -Wextra -Isrc/include
ASFLAGS = -f elf32
LDFLAGS = -m32 -ffreestanding -nostdlib -T linker.ld

CSRC  = $(wildcard src/*.c)
ASRC  = $(wildcard src/*.asm)
COBJ  = $(CSRC:.c=.o)
AOBJ  = $(ASRC:.asm=.o)
OBJ   = $(AOBJ) $(COBJ)

BIN = tinykerl.bin
ISO = tinykerl.iso

all: $(ISO)

src/%.o: src/%.asm
	$(AS) $(ASFLAGS) $< -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	$(LD) $(LDFLAGS) -lgcc $(OBJ) -o $@

$(ISO): $(BIN)
	mkdir -p iso/boot/grub
	cp $(BIN) iso/boot/$(BIN)
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso

clean:
	rm -f src/*.o $(BIN) $(ISO)
	rm -rf iso

.PHONY: all clean
