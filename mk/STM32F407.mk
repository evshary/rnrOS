ARCH = CM4
MCPU = cortex-m4
PLATFORM = STM32F4X
STARTUP_CODE = libraries/$(PLATFORM)/system/src/startup/startup_stm32f40_41xxx.s

PLAT_DEFINE = -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER

FILTER_OUT_SRC = libraries/$(PLATFORM)/driver/src/stm32f4xx_fmc.c
