#!/bin/bash

nasm boot.asm -o boot.bin
nasm -felf kernel/kstub.asm  -o kstub.o
nasm -felf kernel/interrupt.asm -o interrupt.o
clang -m32 -Os -ffreestanding -mno-sse -mcmodel=kernel -Wextra -Wall -target x86_64-unknown-gnu-linux -c kernel/main.c
x86_64-unknown-linux-ld -T link.ld -o kernel.bin kstub.o main.o interrupt.o

dd if=/dev/zero of=nolli.bin bs=512 count=70
dd if=boot.bin of=nolli.bin conv=notrunc
dd if=kernel.bin of=nolli.bin conv=notrunc bs=512 seek=1

rm main.o interrupt.o kstub.o kernel.bin boot.bin
