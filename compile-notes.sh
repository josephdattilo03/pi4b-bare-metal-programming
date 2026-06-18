clang \
  --target=aarch64-none-elf \
  -mcpu=cortex-a72 \
  -ffreestanding \
  -nostdlib \
  -c main.c \
  -o main.o

clang \
  --target=aarch64-none-elf \
  -mcpu=cortex-a72 \
  -c boot.S \
  -o boot.o

ld.lld \
  -T linker.ld \
  boot.o main.o \
  -o kernel.elf

llvm-objcopy -O binary kernel.elf kernel8.img

cp kernel8.img start4.elf fixup4.dat config.txt /Volumes/NO\ NAME
