BUILD = build
TARGET = main

CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
CFLAGS = -fno-common -ffreestanding -O0 \
         -gdwarf-2 -g3 -Wall -Werror \
         -mthumb \
         -Wl,-Tmain.ld -nostartfiles

LINKER = main.ld

QEMU ?= ../qemu_stm32/arm-softmmu/qemu-system-arm
ASTYLE ?= astyle
