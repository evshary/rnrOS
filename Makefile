include mk/define.mk

BOARD ?= STM32F4X

ifeq ($(BOARD), STM32F10)
	include mk/STM32F10.mk
else
	include mk/STM32F4X.mk
endif


CMSIS_PLAT_SRC = $(CMSIS_LIB)/Device/$(VENDOR)/$(PLAT)

CFLAGS += -mcpu=$(MCPU) \
		$(PLAT_DEFINE)

SRCDIR = src \
		libraries/$(PLATFORM)/driver/src \
		 libraries/$(PLATFORM)/system/src

SRC = $(wildcard $(addsuffix /*.c,$(SRCDIR))) \
      $(wildcard $(addsuffix /*.s,$(SRCDIR))) \
      $(STARTUP_CODE)

SRC := $(filter-out $(FILTER_OUT_SRC),$(SRC))

INCDIR= include \
		libraries/core \
		libraries/$(PLATFORM)/driver/inc \
		libraries/$(PLATFORM)/system/inc

INC = $(addprefix -I,$(INCDIR))

OUTDIR = $(BUILD)

OBJ := $(addprefix $(OUTDIR)/,$(patsubst %.s,%.o,$(SRC:.c=.o)))
DEP = $(OBJ:.o=.o.d)

.PHONY: all

all: $(OUTDIR)/$(TARGET).bin $(OUTDIR)/$(TARGET).lst

$(OUTDIR)/$(TARGET).bin: $(OUTDIR)/$(TARGET).elf
	@echo "    OBJCOPY "$@
	@$(CROSS_COMPILE)objcopy -Obinary $< $@

$(OUTDIR)/$(TARGET).lst: $(OUTDIR)/$(TARGET).elf
	@echo "    LIST    "$@
	@$(CROSS_COMPILE)objdump -S $< > $@

$(OUTDIR)/$(TARGET).elf: $(OBJ)
	@echo "    LD      "$@
	@echo "    MAP     "$(OUTDIR)/$(TARGET).map
	@$(CC) $(CFLAGS) -Wl,-Map=$(OUTDIR)/$(TARGET).map -o $@ $^

$(OUTDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "    CC      "$@
	@$(CC) $(CFLAGS) -MMD -MF $@.d -o $@ -c $(INC) $<

$(OUTDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo "    CC      "$@
	@$(CC) $(CFLAGS) -MMD -MF $@.d -o $@ -c $(INC) $<

clean:
	rm -rf $(OUTDIR)

qemu:
	$(QEMU) -M stm32-p103 -kernel $(OUTDIR)/$(TARGET).bin

style:
	$(ASTYLE) --style=kr --indent=spaces=4 -S -H -U -p --suffix=none "src/*.c" "include/*.h"

-include $(DEP)
